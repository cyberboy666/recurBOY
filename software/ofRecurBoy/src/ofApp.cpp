#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(0, 0, 0);
    ofSetVerticalSync(false);
    ofHideCursor();
    setting.loadJson();
    ofSetFrameRate(setting.framerate);
    readVideoMode();

    if(setting.devMode){
        ofSetFullscreen(0);
        ofSetWindowShape(300,200);
        ofSetWindowPosition(50,500);
        fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
        fxFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    }
    else{
        ofSetFullscreen(1);
        fbo.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGB);
        fxFbo.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGB);
    }
    

    userInput.setupThis("actionMap.json");
    userInput.analogListening();
    userInput.midiListening(true);
    userInput.adcDelay = setting.adcSecDelay;
    userInput.adcGrain = setting.adcGrain;
    userInput.connectKeyboard(0);

    sender.setup("localhost", 9000);

    videoPage.name = "VIDEO";
    patternPage.name = "PATTERN";
    externalPage.name = "EXTERNAL";
    effectPage.name = "EFFECT";
    textPage.name = "TEXT";
    fontPage.name = "FONT";
    settingPage.name = "SETTING";
    messagePage.name = "MESSAGE";

    //videoPage.menuList = getPathsForMenu(videoPage); //getPathsInFolder("/home/pi/Videos/", "video");
    //sendMenuList("/LIST/VIDEO", videoPage.menuList);
    //patternPage.menuList = getPathsForMenu(patternPage); // getPathsInFolder("/home/pi/Shaders/", "shader");
    //sendMenuList("/LIST/PATTERN", patternPage.menuList);
    //effectPage.menuList = getPathsForMenu(effectPage);
    //sendMenuList("/LIST/EFFECT", effectPage.menuList);
    //textPage.menuList = getPathsForMenu(textPage);
    //sendMenuList("/LIST/TEXT", textPage.menuList);
    //fontPage.menuList = getPathsForMenu(fontPage);
    //sendMenuList("/LIST/FONT", fontPage.menuList);

    regenerateAndSendList(videoPage);
    regenerateAndSendList(patternPage);
    regenerateAndSendList(effectPage);
    regenerateAndSendList(textPage);
    regenerateAndSendList(fontPage);
    settingPage.menuList = setting.generateMenuList();
    sendMenuList("/LIST/SETTING", settingPage.menuList);
    
    sourceModes = {"VIDEO", "PATTERN"};
    currentSourceIndex = 0;

    isCameraDetected = detectCamera();
    if(isCameraDetected){sourceModes.push_back("EXTERNAL");}
    externalPage.menuList = {"start", "stop"};
    sendMenuList("/LIST/EXTERNAL", externalPage.menuList);
    isCameraRecording = false;

    thisTextObject = textObject();
    thisTextObject.loadFont("arial.tff");
    

    runningSource = sourceModes[0];
    currentPage = runningSource;
    isSampleImage = true;
    img.load("/home/pi/openframeworks10.1/apps/myApps/ofRecurBoy/splash.gif");
    
    recurPlayer.setup("ofxomxplayer");

    shaderPlayer.setup();
    fxPlayer.setup();

    safeShutdownCount = 0;
    safeShutdownLastTime = ofGetElapsedTimef();   

    if(setting.firstBoot){
        setting.firstBoot = false;
        setting.saveJson();
        showMessage("RESIZING PARTITION\n ON FIRST BOOT\nREBOOTING SHORTLY\nPLEASE WAIT");
        ofSleepMillis(10000);
        removeMessage();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    readActions();

    if(runningSource == "VIDEO" && videoPage.isPlaying && !isSampleImage){
        recurPlayer.update();
        fbo.begin();
            recurPlayer.playerDraw();
        fbo.end();
    }
    if(runningSource == "VIDEO" && isSampleImage){
        fbo.begin();
            drawImage();
        fbo.end();
    }
    else if(runningSource == "PATTERN" && patternPage.isPlaying){
        fbo = shaderPlayer.apply({});
    }
    else if(runningSource == "EXTERNAL" && videoInput.isReady() && externalPage.isPlaying){
        videoInput.update();
        fbo.begin();
            videoInput.draw(0,0, ofGetWidth(), ofGetHeight());
        fbo.end();
    }
    if(effectPage.isPlaying){
    fxFbo = fxPlayer.apply({fbo.getTexture(), fxFbo.getTexture()});
    }

   // if(isCameraRecording){ checkRecording();}
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(effectPage.isPlaying){
        fxFbo.draw(0,0, ofGetWidth(), ofGetHeight());
    }
    else{
        fbo.draw(0,0, ofGetWidth(), ofGetHeight());
    }

    if(textPage.isPlaying){
        thisTextObject.draw();
        // thisTextObject.myFont.drawString(thisTextObject.text, 100, 100);
    }

    if(setting.showFps){printFramerate();}
}

void ofApp::drawImage(){
    if(setting.imageRatio == "STRETCH"){
        // Stretch image across screen
        img.draw(0,0, ofGetWidth(), ofGetHeight());
        return;
    }

    float imgAspect = img.getWidth() / img.getHeight();
    float screenAspect = ofGetWidth() / (float)ofGetHeight();

    float drawWidth, drawHeight;
    float offsetX = 0, offsetY = 0;

    if (imgAspect > screenAspect) {
        // Image is wider than screen, fit by width
        drawWidth = ofGetWidth();
        drawHeight = drawWidth / imgAspect;
        offsetY = (ofGetHeight() - drawHeight) / 2; // Center vertically
    } else {
        // Image is taller than screen, fit by height
        drawHeight = ofGetHeight();
        drawWidth = drawHeight * imgAspect;
        offsetX = (ofGetWidth() - drawWidth) / 2; // Center horizontally
    }

    // Draw image with calculated size and position
    ofBackground(0);
    img.draw(offsetX, offsetY, drawWidth, drawHeight);

}

ofApp::pageObject& ofApp::getPagebyName(string pageName){
    if(pageName == "VIDEO"){ return videoPage;}
    else if(pageName == "PATTERN"){ return patternPage;}
    else if(pageName == "EXTERNAL"){ return externalPage;}
    else if(pageName == "EFFECT"){ return effectPage;}
    else if(pageName == "TEXT"){ return textPage;}
    else if(pageName == "FONT"){ return fontPage;}
    else if(pageName == "SETTING"){ return settingPage;}
    else if(pageName == "MESSAGE"){ return messagePage;}
    else{return videoPage;}
}

void ofApp::readActions(){
    userInput.readKey();
    vector<vector<string>> actionsList = userInput.getActions();
    for( int i = 0; i < actionsList.size(); i++){
        ofLog() << "action is " << actionsList[i][0] << "value is " << actionsList[i][1];
        runAction(actionsList[i][0], actionsList[i][1]);
    }
    if(isLiveText){
        vector<int> keyPress = userInput.getKeyPassthrough();
        for( int i = 0; i < keyPress.size(); i++){
            liveTextKey(keyPress[i]);
        }
    }

}


//--------------------------------------------------------------
// a work around for now (would rather be hooking keypress from within incur buit seemd harder)
/* void ofApp::keyPressed  (int key){
     ofLog() << "isLiveText: " << isLiveText;
     if(isLiveText){
         liveTextKey(key);
     }
     else{
         userInput.onKeyPress(key);
     }
 }
*/
// also here was hoping to have a map of pointers to the function , but also seemd more tricky than it needs to be
void ofApp::runAction(string action, string amount){
     if(action == "exit"){ exit();}
     else if(action == "somethingElse"){}
     else if(action == "moveUp"){ moveUp();}
     else if(action == "moveDown"){ moveDown();}
     else if(action == "moveLeft"){ moveLeft();}
     else if(action == "moveRight"){ moveRight();}
     else if(action == "enter"){ enter();}
     else if(action == "fxSwitch"){ fxSwitch();}
     else if(action == "playSwitch"){ playSwitch();}
     else if(action == "switchSource"){ switchSource();}
     else if(action == "stepParam0"){ stepParam0();}
     else if(action == "stepParam1"){ stepParam1();}
     else if(action == "stepParam2"){ stepParam2();}
     else if(action == "stepSpeed"){ stepSpeed();}
     else if(action == "setParam0"){ setParam0(ofToFloat(amount));}
     else if(action == "setParam1"){ setParam1(ofToFloat(amount));}
     else if(action == "setParam2"){ setParam2(ofToFloat(amount));}
     else if(action == "setSpeed"){ setSpeed(ofToFloat(amount));}
     else if(action == "setParam0Invert"){ setParam0Invert(ofToFloat(amount));}
     else if(action == "setParam1Invert"){ setParam1Invert(ofToFloat(amount));}
     else if(action == "setParam2Invert"){ setParam2Invert(ofToFloat(amount));}
     else if(action == "setSpeedInvert"){ setSpeedInvert(ofToFloat(amount));}
     else if(action == "setShaderParam0"){ setShaderParam0(ofToFloat(amount));}
     else if(action == "setShaderParam1"){ setShaderParam1(ofToFloat(amount));}
     else if(action == "setShaderParam2"){ setShaderParam2(ofToFloat(amount));}
     else if(action == "setShaderSpeed"){ setShaderSpeed(ofToFloat(amount));}
     else if(action == "setEffectParam0"){ setEffectParam0(ofToFloat(amount));}
     else if(action == "setEffectParam1"){ setEffectParam1(ofToFloat(amount));}
     else if(action == "setEffectParam2"){ setEffectParam2(ofToFloat(amount));}
     else if(action == "setEffectSpeed"){ setEffectSpeed(ofToFloat(amount));}
     else if(action == "setParam0Cv"){ setParam0Cv(ofToFloat(amount));}
     else if(action == "setParam1Cv"){ setParam1Cv(ofToFloat(amount));}
     else if(action == "setParam2Cv"){ setParam2Cv(ofToFloat(amount));}
     else if(action == "setSpeedCv"){ setSpeedCv(ofToFloat(amount));}
     else if(action == "setTextDisplaceX"){ setTextDisplaceX(ofToFloat(amount));}
     else if(action == "setTextDisplaceY"){ setTextDisplaceY(ofToFloat(amount));}
     else if(action == "setTextDisplaceWidth"){ setTextDisplaceWidth(ofToFloat(amount));}
     else if(action == "setTextDisplaceHeight"){ setTextDisplaceHeight(ofToFloat(amount));}
     else if(action == "setTextColorHue"){ setTextColorHue(ofToFloat(amount));}
     else if(action == "setTextColorSat"){ setTextColorSat(ofToFloat(amount));}
     else if(action == "setTextColorBrightness"){ setTextColorBrightness(ofToFloat(amount));}
     else if(action == "setTextColorAlpha"){ setTextColorAlpha(ofToFloat(amount));}



 }

void ofApp::exit(){
    userInput.exit();
    ofExit();
 }

void ofApp::moveUp(){
    pageObject& currentPageObject = getPagebyName(currentPage);
    if(currentPageObject.selectedRow > 0){currentPageObject.selectedRow--;}
    else{currentPageObject.selectedRow = currentPageObject.menuList.size() - 1;}
    sendIntMessage("/SELECTED_ROW/" + currentPage, currentPageObject.selectedRow);
}

void ofApp::moveDown(){
    pageObject&  currentPageObject = getPagebyName(currentPage);
    if(currentPageObject.selectedRow < currentPageObject.menuList.size() - 1){currentPageObject.selectedRow++;}
    else{currentPageObject.selectedRow = 0;}
    sendIntMessage("/SELECTED_ROW/" + currentPage, currentPageObject.selectedRow);
}

void ofApp::moveLeft(){
    if(currentPage == "FONT"){
        currentPage = "TEXT";
    }
    else if(currentPage == "TEXT"){
        currentPage = "EFFECT";
    }
    if(currentPage == "EFFECT"){
        currentPage = runningSource;
    }
    else if(currentPage == "VIDEO" || currentPage == "PATTERN" || currentPage == "EXTERNAL"){
        currentPage = "SETTING";
    }
    sendStringMessage("/CURRENT_PAGE", currentPage);
}

void ofApp::moveRight(){
    ofLog() << "move right! ";
    if(currentPage == "SETTING"){
        currentPage = runningSource;
    }
    else if(currentPage == "VIDEO" || currentPage == "PATTERN" || currentPage == "EXTERNAL"){
        currentPage = "EFFECT";
    }
    else if(currentPage == "EFFECT" && setting.textEnable){
        currentPage = "TEXT";
    }
    else if(currentPage == "TEXT"){
        currentPage = "FONT";
    }
    sendStringMessage("/CURRENT_PAGE", currentPage);
}

void ofApp::fxSwitch(){
    if(currentPage == "TEXT" || currentPage == "FONT"){
        textPage.isPlaying = !textPage.isPlaying;
        fontPage.isPlaying = textPage.isPlaying;
        sendBoolMessage("/PLAYING/TEXT",textPage.isPlaying);
        sendBoolMessage("/PLAYING/FONT",fontPage.isPlaying);
    }
    else{
        effectPage.isPlaying = !effectPage.isPlaying;
        sendBoolMessage("/PLAYING/EFFECT",effectPage.isPlaying);
    }

}

void ofApp::playSwitch(){
    checkSafeShutdown();
    pageObject& runningSourcePage = getPagebyName(runningSource);
    runningSourcePage.isPlaying = !runningSourcePage.isPlaying;
    if(runningSource == "VIDEO"){recurPlayer.setPlay(runningSourcePage.isPlaying);}
    else if(runningSource == "PATTERN"){ shaderPlayer.setPlay(runningSourcePage.isPlaying); }
    sendIntMessage("/PLAYING/" + runningSource, runningSourcePage.isPlaying);
}

void ofApp::stepParam0(){
    stepParam0Value = (stepParam0Value + 1) % 10;
    float amountToSet = (float)stepParam0Value/10.0;
    ofLog() << "amount is " << ofToString(amountToSet);
    setParam0(amountToSet);
}

void ofApp::stepParam1(){
    stepParam1Value = (stepParam1Value + 1) % 10;
    float amountToSet = (float)stepParam1Value/10.0;
    ofLog() << "amount is " << ofToString(amountToSet);
    setParam1(amountToSet);
}

void ofApp::stepParam2(){
    stepParam2Value = (stepParam2Value + 1) % 10;
    float amountToSet = (float)stepParam2Value/10.0;
    ofLog() << "amount is " << ofToString(amountToSet);
    setParam2(amountToSet);
}

void ofApp::stepSpeed(){
    stepSpeedValue = (stepSpeedValue + 1) % 10;
    float amountToSet = (float)stepSpeedValue/10.0;
    ofLog() << "amount is " << ofToString(amountToSet);
    setSpeed(amountToSet);
} 
 

void ofApp::setParam0(float value){
    if(currentPage == "TEXT"){setTextDisplaceX(value);}
    else if(currentPage == "FONT"){setTextColorHue(value);}
    else if(currentPage == "PATTERN" ){setShaderParam0(value);}
    else{setEffectParam0(value);}
}

void ofApp::setParam1(float value){
    if(currentPage == "TEXT"){setTextDisplaceY(value);}
    else if(currentPage == "FONT"){setTextColorSat(value);}
    else if(currentPage == "PATTERN" ){setShaderParam1(value);}
    else{setEffectParam1(value);}
}

void ofApp::setParam2(float value){
    if(currentPage == "TEXT"){setTextDisplaceWidth(value);}
    else if(currentPage == "FONT"){setTextColorBrightness(value);}
    else if(currentPage == "PATTERN" ){setShaderParam2(value);}
    else{setEffectParam2(value);}
}

void ofApp::setSpeed(float value){
    if(currentPage == "TEXT"){setTextDisplaceHeight(value);}
    else if(currentPage == "FONT"){setTextColorAlpha(value);}
    else if(currentPage == "PATTERN" ){setShaderSpeed(value);}
    else{setEffectSpeed(value);}
}

void ofApp::setParam0Invert(float value){setParam0(1.0 - value);}
void ofApp::setParam1Invert(float value){setParam1(1.0 - value);}
void ofApp::setParam2Invert(float value){setParam2(1.0 - value);}
void ofApp::setSpeedInvert(float value){setSpeed(1.0 - value);}

void ofApp::setShaderParam0(float value){shaderPlayer.shaderParams[0] = ofClamp(value, 0.0, 1.0);}
void ofApp::setShaderParam1(float value){shaderPlayer.shaderParams[1] = ofClamp(value, 0.0, 1.0);}
void ofApp::setShaderParam2(float value){shaderPlayer.shaderParams[2] = ofClamp(value, 0.0, 1.0);}
void ofApp::setShaderSpeed(float value){shaderPlayer.setSpeed(ofClamp(value, 0.0, 1.0));}

void ofApp::setEffectParam0(float value){fxPlayer.shaderParams[0] = ofClamp(value, 0.0, 1.0);}
void ofApp::setEffectParam1(float value){fxPlayer.shaderParams[1] = ofClamp(value, 0.0, 1.0);}
void ofApp::setEffectParam2(float value){fxPlayer.shaderParams[2] = ofClamp(value, 0.0, 1.0);}
void ofApp::setEffectSpeed(float value){fxPlayer.setSpeed(ofClamp(value, 0.0, 1.0));}

void ofApp::setTextDisplaceX(float value){thisTextObject.x = ofClamp(value, 0.0, 1.0);}
void ofApp::setTextDisplaceY(float value){thisTextObject.y = ofClamp(value, 0.0, 1.0);}
void ofApp::setTextDisplaceWidth(float value){thisTextObject.width = ofClamp(value, 0.0, 1.0);}
void ofApp::setTextDisplaceHeight(float value){thisTextObject.height = ofClamp(value, 0.0, 1.0);}
void ofApp::setTextColorHue(float value){thisTextObject.hue = ofClamp(value, 0.0, 1.0);}
void ofApp::setTextColorSat(float value){thisTextObject.sat = ofClamp(value, 0.0, 1.0);}
void ofApp::setTextColorBrightness(float value){thisTextObject.brightness = ofClamp(value, 0.0, 1.0);}
void ofApp::setTextColorAlpha(float value){thisTextObject.alpha = ofClamp(value, 0.0, 1.0);}

void ofApp::setParam0Cv(float value){
    if(setting.cvButton){
        if(lastCv0Value < 0.5 && value > 0.5){
        playSwitch();
        }
        lastCv0Value = value;
        return;
    }
    if(setting.cvUpper == "1V"){value = value * 5.0;}
    setParam0(value);
}

void ofApp::setParam1Cv(float value){ 
    if(setting.cvButton && lastCv1Value < 0.5 && value > 0.5){
        lastCv1Value = value;
        fxSwitch();
        return;
    }
    if(setting.cvUpper == "1V"){value = value * 5.0;}
    setParam1(value);
}

void ofApp::setParam2Cv(float value){ 
    if(setting.cvButton && lastCv2Value < 0.5 && value > 0.5){
        lastCv2Value = value;
        moveDown();
        return;
    }
    if(setting.cvUpper == "1V"){value = value * 5.0;}
    setParam2(value);
}

void ofApp::setSpeedCv(float value){ 
    if(setting.cvButton && lastCv3Value < 0.5 && value > 0.5){
        lastCv3Value = value;
        enter();
        return;
    }
    if(setting.cvUpper == "1V"){value = value * 5.0;}
    setSpeed(value);
}

void ofApp::enter(){
    closeUnusedInput();
    pageObject& currentPageObject = getPagebyName(currentPage);

    currentPageObject.playingRow = currentPageObject.selectedRow;     
    sendIntMessage("/PLAYING_ROW/" + currentPage, currentPageObject.playingRow);
    currentPageObject.isPlaying = true;
    sendBoolMessage("/PLAYING/" + currentPage,true);

    string menu_item = currentPageObject.menuList[currentPageObject.selectedRow];
    ofLog() << "menu_item: " << menu_item;

    if(endsWith(menu_item, '|')){
        // going into a folder
        menu_item.pop_back();
        currentPageObject.currentFolder = menu_item;
        currentPageObject.menuList = getPathsForMenu(currentPageObject);
        currentPageObject.playingRow = -1;
        currentPageObject.selectedRow = 0;
        sendMenuList("/LIST/" + currentPage, currentPageObject.menuList);
        sendIntMessage("/PLAYING_ROW/" + currentPage, currentPageObject.playingRow);
        sendIntMessage("/SELECTED_ROW/" + currentPage, currentPageObject.selectedRow);
    }
    else if(endsWith(menu_item, '^')){
        // going out of folder
        menu_item.pop_back();
        currentPageObject.currentFolder = moveFolderUpOneLevel(currentPageObject.currentFolder);
        currentPageObject.menuList = getPathsForMenu(currentPageObject);
        currentPageObject.playingRow = -1;
        currentPageObject.selectedRow = 0;
        sendMenuList("/LIST/" + currentPage, currentPageObject.menuList);
        sendIntMessage("/PLAYING_ROW/" + currentPage, currentPageObject.playingRow);
        sendIntMessage("/SELECTED_ROW/" + currentPage, currentPageObject.selectedRow);
    }
    else if(currentPage == "EFFECT"){
        fxPlayer.loadShaderFiles("default.vert", menu_item);   
    } 
    else if(currentPage == "VIDEO"){
        runningSource = "VIDEO";
if(fileIsImageExtension(menu_item)){

    int w, h;

    if(getImageSize(menu_item, w, h)){
        ofLogNotice() << "Image size: " << w << "x" << h;

        if(w < 2000 && h < 2000){
            img.load(menu_item);
            isSampleImage = true;
        }
        else{
        showMessage("IMAGE TOO LARGE TO LOAD");
        ofSleepMillis(3000);
        removeMessage();
            ofLogWarning() << "Skipping large image: " << w << "x" << h;
        }
    }
    else{
        ofLogError() << "Could not read image metadata: " << menu_item;
    }
}
//        if(fileIsImageExtension(menu_item)){
            //img.load(menu_item);

            //isSampleImage = True;
            //img.load(menu_item);
//        }
        else{
            isSampleImage = False;
            playVideo(menu_item);
        }
    } 
    else if(currentPage == "PATTERN"){
        runningSource = "PATTERN";
        shaderPlayer.loadShaderFiles("default.vert", menu_item);
    }   
    else if(currentPage == "EXTERNAL"){
        runningSource = "EXTERNAL";
        if(menu_item == "start"){
            videoInput.setup("vidGrabber", ofGetWidth(), ofGetHeight(), 25, 2);
        }
        else if(menu_item == "stop"){
            videoInput.close();
            externalPage.isPlaying = false;
            sendIntMessage("/IS_PLAYING/EXTERNAL", externalPage.isPlaying);

        }
    }
    else if(currentPage == "FONT"){
        thisTextObject.loadFont(menu_item);
        textPage.isPlaying = true;
        sendBoolMessage("/PLAYING/TEXT",true);
    }
    else if(currentPage == "TEXT"){
        if(menu_item == "LIVE"){
            thisTextObject.text = "";
            isLiveText = true;
            userInput.isKeyPassthrough = true;
        }
        else{
            isLiveText = false;
            userInput.isKeyPassthrough = false;
            thisTextObject.loadText(menu_item);
        }
        
        fontPage.isPlaying = true;
        sendBoolMessage("/PLAYING/FONT",true);
    }
    else if(currentPage == "SETTING"){
        updateSettings(menu_item);
        settingPage.menuList = setting.generateMenuList();
        sendMenuList("/LIST/SETTING", settingPage.menuList);
        sendIntMessage("/PLAYING_ROW/SETTING", -1);
        sendBoolMessage("/PLAYING/SETTING",false);
    }
}

void ofApp::switchSource(){
    // regenerate file lists again - incase something has changed

    //videoPage.menuList = getPathsForMenu(videoPage); //getPathsInFolder("/home/pi/Videos/", "video");
    //sendMenuList("/LIST/VIDEO", videoPage.menuList);
    //patternPage.menuList = getPathsForMenu(patternPage); // getPathsInFolder("/home/pi/Shaders/", "shader");
    //sendMenuList("/LIST/PATTERN", patternPage.menuList);
    //effectPage.menuList = getPathsForMenu(effectPage);
    //sendMenuList("/LIST/EFFECT", effectPage.menuList);
    //textPage.menuList = getPathsForMenu(textPage);
    //sendMenuList("/LIST/TEXT", textPage.menuList);
    //fontPage.menuList = getPathsForMenu(fontPage);
    //sendMenuList("/LIST/FONT", fontPage.menuList);

    regenerateAndSendList(videoPage);
    regenerateAndSendList(patternPage);
    regenerateAndSendList(effectPage);
    regenerateAndSendList(textPage);
    regenerateAndSendList(fontPage);

    currentSourceIndex = (currentSourceIndex + 1) % sourceModes.size() ;
    currentPage = sourceModes[currentSourceIndex];
    // currentPage = runningSource;

    sendStringMessage("/CURRENT_PAGE", currentPage);
    // check again if any midi devices are attached
    userInput.midiListening(true);
    // check if a keyboard is plugged in
    userInput.connectKeyboard(0);
    // check if external source is detected

    if(isCameraDetected && !detectCamera()){
       sourceModes.pop_back();
       isCameraDetected = false;
       // videoInput.close();
       // externalPage.isPlaying = false;
       // sendIntMessage("/IS_PLAYING/EXTERNAL", externalPage.isPlaying);
        }
    else if(!isCameraDetected && detectCamera()){
        sourceModes.push_back("EXTERNAL");
        isCameraDetected = true;
    }
}

void ofApp::closeUnusedInput(){
    if(runningSource != "VIDEO"){
        recurPlayer.closeAll();
        videoPage.playingRow = -1;
        sendIntMessage("/PLAYING_ROW/VIDEO", videoPage.playingRow);
    }
    else if(runningSource != "EXTERNAL"){
        videoInput.close();
        externalPage.isPlaying = false;
        sendIntMessage("/IS_PLAYING/EXTERNAL", externalPage.isPlaying);
    }
    else if(runningSource != "PATTERN"){
        patternPage.playingRow = -1;
        sendIntMessage("/PLAYING_ROW/PATTERN", patternPage.playingRow);
    }
}

void ofApp::liveTextKey(int key){
    ofLog() << "this key press is: " << key;
    if(key == 127){
        // backspace was pressed
        if(thisTextObject.text.size() > 0){
            thisTextObject.text.pop_back();
        }
        
    }
    else if(key == 27){
        // ESCAPE  was pressed leaving LIVE TEXT MODE
        textPage.isPlaying = false;
        fontPage.isPlaying = false;
        sendBoolMessage("/PLAYING/TEXT",false);
        sendBoolMessage("/PLAYING/FONT",false);

        textPage.playingRow = -1;     
        sendIntMessage("/PLAYING_ROW/TEXT", -1);
        
        isLiveText = false;
        userInput.isKeyPassthrough = false;
    }
    else if(key == 80){
        // F1 was pressed - save current text to file

        std::string fileNameBase = thisTextObject.text.substr(0, 12);  // Get first 12 chars + "..."
        // Replace spaces & special characters to make a safe filename
        for (char& c : fileNameBase) {
            if (c == ' ' || c == '/' || c == '\\' || c == ':') {
                c = '_';  // Replace with underscores
            }
        }
        std::string filepath = "/home/pi/TEXT/" + fileNameBase + ".txt";  
        ofBuffer buffer;
        buffer.set(thisTextObject.text);
        ofBufferToFile(filepath, buffer);
        thisTextObject.text = "";
        // update list
        textPage.menuList = getPathsForMenu(textPage);
        sendMenuList("/LIST/TEXT", textPage.menuList);
    }
    else if(key >= 32 && key <= 126 || key == 10){
        thisTextObject.text = thisTextObject.text + ofToString(static_cast<char>(key));
    }
    else{
        ofLog() << "key press out of range";
    }
    
}

void ofApp::sendStringMessage(string address, string value){
    ofxOscMessage response;
    response.setAddress(address);
    response.addStringArg(value);

    sender.sendMessage(response, true);
}

void ofApp::playVideo(string path){
    fbo.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGB);
    recurPlayer.startSingle(path);
}

void ofApp::sendIntMessage(string address, int value){
    ofxOscMessage response;
    response.setAddress(address);
    response.addIntArg(value);

    sender.sendMessage(response, true);
}

void ofApp::sendBoolMessage(string address, bool value){
    ofxOscMessage response;
    response.setAddress(address);
    response.addBoolArg(value);

    sender.sendMessage(response, true);
}

void ofApp::sendMenuList(string address, vector<string> list) {
    // start by clearing the current list
    sendBoolMessage("/CLEAR" + address, true);


    const int OSC_MESSAGE_SIZE_LIMIT = 7500;
    int estimatedSize = address.size() + 4;
    ofxOscMessage response;
    response.setAddress(address);

    for (int i = 0; i < list.size(); i++) {
        // Calculate the size of this string argument
        int argSize = list[i].size() + 4; // Each argument has its size rounded up to a multiple of 4 bytes
        estimatedSize += argSize;

        // Check if adding this argument would exceed the limit
        if (estimatedSize > OSC_MESSAGE_SIZE_LIMIT) {
            // Send the current message
            sender.sendMessage(response, true);
            ofSleepMillis(10);

            // Start a new message
            response.clear();
            response.setAddress(address);
            estimatedSize = address.size() + 4 + argSize; // Reset estimated size for the new message

            ofLogNotice() << "OSC message size limit reached. Sending current message and starting a new one.";
        }

        // Add the argument to the current message
        response.addStringArg(list[i]);
    }

    // Send the final message if it contains any arguments
    if (response.getNumArgs() > 0) {
        sender.sendMessage(response, true);
    }
}

// void ofApp::sendMenuList(string address, vector<string> list){
//     ofxOscMessage response;
//     response.setAddress(address);
//     for( int i = 0; i < list.size(); i++ ){
//         response.addStringArg(list[i]);    
//     }
//     //response.addStringArg(value);

//     sender.sendMessage(response, true);
// }

void ofApp::regenerateAndSendList(pageObject& thisObject) {
    thisObject.menuList = getPathsForMenu(thisObject);
    thisObject.selectedRow = 0;
    thisObject.playingRow = -1;
    sendMenuList("/LIST/" + thisObject.name, thisObject.menuList);
    sendIntMessage("/SELECTED_ROW/" + thisObject.name, 0);
    sendIntMessage("/PLAYING_ROW/" + thisObject.name, -1);
}

bool alphabetical(string a, string b){return a<b;}

vector<string> ofApp::getPathsForMenu(pageObject& thisObject){
    vector<string> combinedPaths;
    vector<string> folderPaths;

    ofSort(combinedPaths, alphabetical);

    if(thisObject.currentFolder == "/"){
        // this object needs combined root menu
        folderPaths = getPathFromCombinedRootLocations(thisObject.name); 
        combinedPaths.insert(combinedPaths.end(), folderPaths.begin(), folderPaths.end());
    }
    else{
        // otherwise get objects in specific folder and append folder name at top:
        folderPaths = getPathsInFolder(thisObject.currentFolder, thisObject.name);
        combinedPaths.insert(combinedPaths.end(),folderPaths.begin(), folderPaths.end());
        combinedPaths.insert(combinedPaths.begin(), thisObject.currentFolder + "^");  // Add item to front
    }
    if(thisObject.name == "TEXT"){
        combinedPaths.insert(combinedPaths.begin(), "LIVE");
    }
    return combinedPaths;
}

vector<string> ofApp::getPathFromCombinedRootLocations(string source){

    // get a list of external devices
    vector<string> deviceList = {"/media/usb0"}; 
//    ofDirectory dir("/media/usb0/");
//	dir.getSorted();
//    dir.getSorted(); //listDir();
//    for(int i = 0; i < 1; i++){ // dir.size(); i++){
//        ofLog() << "the path " << dir.getPath(i); 
//        deviceList.push_back(dir.getPath(i));  
	   // }
    string sourcePath;
    string legacySourcePath = "";

    if(source == "VIDEO"){
        sourcePath = "/VIDEO/";
        legacySourcePath = "/Videos/";
        }
    else if(source == "PATTERN"){
        
        sourcePath = "/PATTERN/";
        legacySourcePath = "/Shaders/";
        }
    else if(source == "EFFECT"){
        sourcePath = "/EFFECT/";
        legacySourcePath = "/Fx/";
        }
    else if(source == "TEXT"){
        sourcePath = "/TEXT/";
        }
    else if(source == "FONT"){
        sourcePath = "/FONT/";
        }
    else {sourcePath = "/";}
    vector<string> sourceList;
    // for each external device get the source files from it and add the sourceList
    if(deviceList.size() > 0){
        for(int i = 0; i < deviceList.size(); i++){
            vector<string> theseFiles = getPathsInFolder(deviceList[i] + sourcePath, source);
            sourceList.insert(sourceList.end(), theseFiles.begin(), theseFiles.end());
            // check both new folder names and old folder names
            if(legacySourcePath != ""){
                vector<string> theseFiles = getPathsInFolder(deviceList[i] + legacySourcePath, source);
                sourceList.insert(sourceList.end(), theseFiles.begin(), theseFiles.end());
            }
        }
    }
    // get the source files from the internal path
    vector<string> internalFiles = getPathsInFolder("/home/pi" + sourcePath, source);
    sourceList.insert(sourceList.end(), internalFiles.begin(), internalFiles.end());
    // check both new folder names and old folder names
    if(legacySourcePath != ""){
        vector<string> internalFiles = getPathsInFolder("/home/pi" + legacySourcePath, source);
        sourceList.insert(sourceList.end(), internalFiles.begin(), internalFiles.end());
    }
    
    ofSort(sourceList, alphabetical);

    for(int i = 0; i < sourceList.size(); i++){
        ofLog() << "sortlist " << sourceList[i];
    }

    return sourceList;
}


vector<string> ofApp::getPathsInFolder(string folderPath, string mode){
    vector<string> thisList; 
    vector<string> allowedExt;
    ofDirectory dir(folderPath);
//	dir = dir.getSorted();
    
    if(mode == "VIDEO"){
// video file types:
        allowedExt.push_back("mp4");
        allowedExt.push_back("mov");
        allowedExt.push_back("avi");
        allowedExt.push_back("mkv");
// image file types:
        allowedExt.push_back("png");
        allowedExt.push_back("jpg");
        allowedExt.push_back("jpeg");
        allowedExt.push_back("gif");
    } else if(mode == "PATTERN" || mode == "EFFECT"){
        allowedExt.push_back("frag");
        allowedExt.push_back("glsl");
        allowedExt.push_back("glslf");
        allowedExt.push_back("fsh");
    } else if(mode == "TEXT"){
        allowedExt.push_back("txt");
    } else if(mode == "FONT"){
        allowedExt.push_back("ttf");
        allowedExt.push_back("otf");
    }
    if(dir.exists()){
        dir.listDir();

        for(int i = 0; i < dir.size(); i++){
            if (dir.getFile(i).isDirectory()) {
                thisList.push_back(dir.getPath(i) + "|"); // Add directory
            }
            else {      
                // Get the file's extension
                std::string fileExtension = dir.getFile(i).getExtension();

                // Check if the file's extension is in the allowed list
                if (std::find(allowedExt.begin(), allowedExt.end(), fileExtension) != allowedExt.end()) {
                    thisList.push_back(dir.getPath(i)); // Add the file if its extension is allowed
                }
            } 
        }
        ofSort(thisList, alphabetical);

        return thisList;
    }
    else{
        return {};
    }

}

bool ofApp::fileIsImageExtension(string path){
    ofFile file(ofToDataPath(path));
    string ext = file.getExtension();
    if(ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "gif"){
        return True;
    }
    else{
        return False;
    }
}

bool ofApp::detectCamera(){
    // this code is used to detect a camera in csi port (not needed for now)
    //string resp = myExec("vcgencmd get_camera");
    //string substr = resp.substr(20, 21);
    //return resp == "supported=1 detected=1\n";

    //this code is to detect usb capture devices
    try {
        ofVideoGrabber vidGrabber;
        vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
        return devices.size() > 2;
        }
    catch (const std::exception& e) {
        ofLogError() << "General exception caught: " << e.what();
        return False;
    }
}

string ofApp::myExec(char* cmd){
    FILE* pipe = popen(cmd, "r");
    if(!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)){
        if(fgets(buffer, 128, pipe) != NULL)
        result += buffer;
    }
    pclose(pipe);
    return result;
}

void ofApp::updateSettings(string settingLine){
    vector<string> result = ofSplitString(settingLine, ":", false,true);
    ofLog() << "result[0]: " << result[0] << " result[1]: " << result[1];
    if(result[0] == "VIDEO_MODE"){
        if(result[1] == "CVBS"){
            showMessage("enable HDMI by rebooting \nwith HDMI cable attached");
            ofSleepMillis(3000);
            removeMessage();
        }
        else{
            showMessage("enable CVBS by rebooting \nwithout HDMI cable attached");
            ofSleepMillis(3000);
            removeMessage();
        }
    }
    else if(result[0] == "CVBS_TYPE"){
        if(result[1] == "NTSC"){ setting.cvbsType = "PAL"; }
        else{ setting.cvbsType = "NTSC"; }
        persistVideoMode();

        showMessage("enable new CVBS mode by\nrebooting without \nHDMI cable attached");
        ofSleepMillis(3000);
        removeMessage();

       // if(setting.videoMode == "CVBS"){updateVideoMode();}
    }
    else if(result[0] == "HDMI_TYPE"){
        if(result[1] == "480P"){ setting.hdmiType = "720P"; }
        else{ setting.hdmiType = "480P"; }
        persistVideoMode();
        showMessage("enable new HDMI mode by\nrebooting with HDMI \ncable attached");
        ofSleepMillis(3000);
        removeMessage();

       // if(setting.videoMode == "HDMI"){updateVideoMode();}
    }
    else if(result[0] == "CV_UPPER"){
        if(result[1] == "5V"){ setting.cvUpper = "1V"; }
        else{ setting.cvUpper = "5V"; }
    }
    else if(result[0] == "TEXT_ENABLE"){
        if(result[1] == "true"){ setting.textEnable = false; }
        else{ setting.textEnable = true; }
    }
    else if(result[0] == "CV_BUTTON"){
        if(result[1] == "true"){ setting.cvButton = false; }
        else{ setting.cvButton = true; }
    }

    else if(result[0] == "IMAGE_RATIO"){
        if(result[1] == "ORIGINAL"){ setting.imageRatio = "STRETCH"; }
        else{ setting.imageRatio = "ORIGINAL"; }
    }
    else if(result[0] == "SHOW_FPS"){
        if(result[1] == "true"){ setting.showFps = false; }
        else{ setting.showFps = true; }
    }
    else if(result[0] == "ADC_GRAIN"){
        setting.advanceAdcGrain();
        userInput.adcGrain = setting.adcGrain;
    }
    else if(result[0] == "PIXEL_OFFSET"){
        if(result[1] == "true"){ setting.pixelOffset = false; }
        else{ setting.pixelOffset = true; }
        sendBoolMessage("/RESTART_DISPLAY",true);
    }
    else if(result[0] == "EJECT_USB"){
        string mountPoint = "/media/usb0";
        // Unmount the USB stick
	std::string command = "sudo umount " + mountPoint;
	std::vector<char> cmd(command.begin(), command.end());
	cmd.push_back('\0'); // Ensure null termination
	myExec(cmd.data());  // Pass mutable char* to myExec
        
        int unmountStatus = system("mountpoint -q /media/usb0 || echo 0");
        if (unmountStatus == 0) {
            showMessage("eject successful");
            ofSleepMillis(3000);
            removeMessage();
        } else {
            showMessage("eject failed");
            ofSleepMillis(3000);
            removeMessage();
            }
    }
    else if(result[0] == "COPY_CONTENT"){
        showMessage("copying please wait...");
        int status = std::system("bash /home/pi/openframeworks10.1/apps/myApps/ofRecurBoy/copy_usb_to_internal.sh");
        if (status == 0) {
            showMessage("copy successful");
        } else if (status == 256) { // Exit code 1 (Not enough space)
            showMessage("not enough disk space");
        } else if (status == 512) { // Exit code 2 (Copy error)
            showMessage("error while copying");
        }
        ofSleepMillis(3000);
        removeMessage();
    }

    else if(result[0] == "SHUTDOWN"){
        showMessage("shutting down please wait...");
        system("sudo shutdown -h now");
    }
    setting.saveJson();
    
}

void ofApp::readVideoMode(){
    string info = myExec("tvservice -s");

    if (info.find("NTSC") != std::string::npos) {
        setting.videoMode = "CVBS";
        setting.cvbsType = "NTSC";
    }
    else if (info.find("PAL") != std::string::npos) {
        setting.videoMode = "CVBS";
        setting.cvbsType = "PAL";
    }
    else if (info.find("HDMI CEA (1)") != std::string::npos) {
        setting.videoMode = "HDMI";
        setting.hdmiType = "480P";
    }
    else if (info.find("HDMI CEA (4)") != std::string::npos) {
        setting.videoMode = "HDMI";
        setting.hdmiType = "720P";
    }
    // setting.saveJson();
    // settingPage.menuList = setting.generateMenuList();
    // sendMenuList("/LIST/SETTING", settingPage.menuList);

}

void ofApp::persistVideoMode(){
    // making sure the hdmi_group mode is set correctly
    myExec("sudo sed -i 's/^hdmi_group=.*/hdmi_group=1/' /boot/config.txt");
    // persit type in config.txt
    if(setting.hdmiType == "720P"){
        myExec("sudo sed -i 's/^hdmi_mode=.*/hdmi_mode=4/' /boot/config.txt"); // set config.txt: hdmi_mode=4
    }
    else if(setting.hdmiType == "480P"){
        myExec("sudo sed -i 's/^hdmi_mode=.*/hdmi_mode=1/' /boot/config.txt"); // set config.txt: hdmi_mode=1
    }

    if(setting.cvbsType == "NTSC"){
        myExec("sudo sed -i 's/^sdtv_mode=.*/sdtv_mode=0/' /boot/config.txt"); // set config.txt: sdtv_mode=0
    }
    else if(setting.cvbsType == "PAL"){
        myExec("sudo sed -i 's/^sdtv_mode=.*/sdtv_mode=2/' /boot/config.txt"); // set config.txt: sdtv_mode=2
    }
}

void ofApp::updateVideoMode(){
    showMessage("updading video mode...");
    // set the mode for right now:
    if(setting.videoMode == "HDMI" && setting.hdmiType == "720P"){
        myExec("tvservice -e='CEA 4 HDMI'");
    }
    else if(setting.videoMode == "HDMI" && setting.hdmiType == "480P"){
        myExec("tvservice -e='CEA 1 HDMI'");
    }
    else if(setting.videoMode == "CVBS" && setting.cvbsType == "NTSC"){
        myExec("tvservice --sdtvon='NTSC 4:3'");
    }
    else if(setting.videoMode == "CVBS" && setting.cvbsType == "PAL"){
        myExec("tvservice --sdtvon='PAL 4:3'");
    }
    else{
        // dont update right now otherwise!
        readVideoMode();
        return;
    }
    //refresh the framebuffer
    myExec("fbset -depth 16; fbset -depth 32");
    // read and update the current mode
    readVideoMode();
    //ofSleepMillis(5000);
    myExec("sh ~/openframeworks10.1/apps/myApps/ofRecurBoy/startRecurBoy");
   // myExec("./startRecurBoy");
    //ofExit();
}

void ofApp::showMessage(string message){
    currentPage = "MESSAGE";
    sendStringMessage("/CURRENT_PAGE", currentPage);

    // Clear the existing menu list
    messagePage.menuList = {};
    // Split the message by '\n' and add each line to the menuList
    stringstream ss(message);
    string line;
    while (getline(ss, line, '\n')) {
        messagePage.menuList.push_back(line);
    }

    sendMenuList("/LIST/MESSAGE", messagePage.menuList);
    sendIntMessage("/PLAYING_ROW/TEXT", -1);
    sendIntMessage("/SELECTED_ROW/TEXT", 0);
}

void ofApp::removeMessage(){
    currentPage = "SETTING";
    sendStringMessage("/CURRENT_PAGE", currentPage);
}

void ofApp::printFramerate(){
    stringstream info;
    float framerate = roundf(ofGetFrameRate());
    info << "FPS: " << framerate ;
    ofDrawBitmapStringHighlight(info.str(), 50, 50, ofColor::black, ofColor::yellow);

}

void ofApp::checkSafeShutdown(){
    float nowGetTime = ofGetElapsedTimef();
    float timeDiff = nowGetTime - safeShutdownLastTime;
    if(timeDiff < 0.5){safeShutdownCount++;}
    else{safeShutdownCount = 0;}
    if(safeShutdownCount > 5){
        showMessage("shutting down please wait...");
        ofSleepMillis(2000);

        ofLog() << "do the shutdown now !!";
        system("sudo shutdown -h now");
    }
    safeShutdownLastTime = nowGetTime;
}

bool ofApp::getImageSize(const std::string& path, int &w, int &h)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path.c_str(), 0);

    if (fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(path.c_str());
    }

    if (fif == FIF_UNKNOWN) {
        ofLogError() << "Unknown image format: " << path;
        return false;
    }

    // IMPORTANT: FIF_LOAD_NOPIXELS avoids full decode
    FIBITMAP* dib = FreeImage_Load(fif, path.c_str(), FIF_LOAD_NOPIXELS);

    if (!dib) {
        ofLogError() << "FreeImage failed to open: " << path;
        return false;
    }

    w = FreeImage_GetWidth(dib);
    h = FreeImage_GetHeight(dib);

    FreeImage_Unload(dib);
    return true;
}
