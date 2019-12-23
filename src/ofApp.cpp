#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0, 0, 0);

    ofSetVerticalSync(false);
    
    readSettings();
    ofSetFrameRate(appFramerate);

	//ofSleepMillis(10000);
    if(isDev){
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
    userInput.adcDelay = adcDelay;

    sender.setup("localhost", 9000);

    sampleList = getPathFromInternalAndExternal("SAMPLER"); //getPathsInFolder("/home/pi/Videos/", "video");
    sendListMessage("/sampleList", sampleList);
    shaderList = getPathFromInternalAndExternal("SHADERS"); // getPathsInFolder("/home/pi/Shaders/", "shader");
    sendListMessage("/shaderList", shaderList);
    fxList = getPathFromInternalAndExternal("FX"); //getPathsInFolder("/home/pi/Fx/", "shader");
    sendListMessage("/fxList", fxList);
    
    inputModes = {"SAMPLER", "SHADERS"};
    inputIndex = 0;
    currentList = sampleList;
    fxScreenVisible = false;
    fxOn = false;
    playOn = false;
    isCameraDetected = detectCamera();
    if(isCameraDetected){inputModes.push_back("CAMERA");}
    cameraList = {"preview"};
    sendListMessage("/cameraList", cameraList);
    isCameraOn = false;
    isCameraRecording = false;
    
    selectedRow = 0;
    selectedFxRow = 0;

    playingSampleRow = -1;
    playingShaderRow = -1;
    playingFxRow = -1;

    selectedInputMode = inputModes[0];
    playingMode = "SPLASH";
    splashImg.load("/home/pi/openframeworks10.1/apps/myApps/recurBOY/splash.gif");
    
    recurPlayer.setup();

    shaderPlayer.setup();
    fxPlayer.setup();

    safeShutdownCount = 0;
    safeShutdownLastTime = ofGetElapsedTimef();   

}

//--------------------------------------------------------------
void ofApp::update(){
    readActions();

    if(playingMode == "SAMPLER" && playOn){
        recurPlayer.update();
        fbo.begin();
            recurPlayer.playerDraw();
        fbo.end();
    }
    else if(playingMode == "SHADERS" && playOn){
        fbo = shaderPlayer.apply({});
    }
    else if(playingMode == "CAMERA" && videoInput.isReady() && playOn){
        videoInput.update();
        fbo.begin();
            videoInput.draw(0,0, ofGetWidth(), ofGetHeight());
        fbo.end();
    }
    else if(playingMode == "SPLASH"){
        fbo.begin();
            splashImg.draw(0,0, ofGetWidth(), ofGetHeight());
        fbo.end();
    }
    if(fxOn){
    fxFbo = fxPlayer.apply({fbo.getTexture()});
    }

    if(isCameraRecording){ checkRecording();}
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(fxOn){
        fxFbo.draw(0,0, ofGetWidth(), ofGetHeight());
    }
    else{
        fbo.draw(0,0, ofGetWidth(), ofGetHeight());
    }

    if(showFramerate){printFramerate();}
}

void ofApp::readSettings(){
    bool parsingSuccessful = settings.open("settings.json");
    clip1v = settings["CLIP_ON_1V"].asBool();
    adcDelay = settings["ADC_SEC_DELAY"].asFloat();
    isDev = settings["DEV_MODE"].asBool();
    appFramerate = settings["FRAMERATE"].asInt();
    showFramerate = settings["SHOW_FRAMERATE"].asBool();
    }

void ofApp::readActions(){

    vector<vector<string>> actionsList = userInput.getActions();
    for( int i = 0; i < actionsList.size(); i++){
        ofLog() << "action is " << actionsList[i][0] << "value is " << actionsList[i][1];
        runAction(actionsList[i][0], actionsList[i][1]);
    }

}


//--------------------------------------------------------------
// a work around for now (would rather be hooking keypress from within incur buit seemd harder)
void ofApp::keyPressed  (int key){
    userInput.onKeyPress(key);
}
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
     else if(action == "switchInput"){ switchInput();}
     else if(action == "setShaderParam0"){ setShaderParam0(ofToFloat(amount));}
     else if(action == "setShaderParam1"){ setShaderParam1(ofToFloat(amount));}
     else if(action == "setShaderParam2"){ setShaderParam2(ofToFloat(amount));}
     else if(action == "setShaderSpeed"){ setShaderSpeed(ofToFloat(amount));}
    else if(action == "setShaderParam0Cv"){ setShaderParam0Cv(ofToFloat(amount));}
     else if(action == "setShaderParam1Cv"){ setShaderParam1Cv(ofToFloat(amount));}
     else if(action == "setShaderParam2Cv"){ setShaderParam2Cv(ofToFloat(amount));}
     else if(action == "setShaderSpeedCv"){ setShaderSpeedCv(ofToFloat(amount));}
 }

void ofApp::exit(){
    userInput.exit();
    ofExit();
 }

void ofApp::moveUp(){
    if(fxScreenVisible){
        if (selectedFxRow > 0){ selectedFxRow--;}
        sendIntMessage("/selectedFxRow", selectedFxRow);
    }
    else{
        if (selectedRow > 0){ selectedRow--;}
        sendIntMessage("/selectedRow", selectedRow);
    }
}

void ofApp::moveDown(){
    if(fxScreenVisible){
        if (selectedFxRow < fxList.size() - 1){ selectedFxRow++;}
        sendIntMessage("/selectedFxRow", selectedFxRow);
    }
    else{
        if (selectedRow < currentList.size() - 1){ selectedRow++;}
        sendIntMessage("/selectedRow", selectedRow);
    }
}

void ofApp::moveLeft(){
    if(fxScreenVisible){
        fxScreenVisible = false;
        sendIntMessage("/fxScreenVisible", 0);
    }
}

void ofApp::moveRight(){
    ofLog() << "move right! ";
    if(!fxScreenVisible){
        fxScreenVisible = true;
        sendIntMessage("/fxScreenVisible", 1);
    }
}

void ofApp::fxSwitch(){
    fxOn = !fxOn;
    int fxOnInt = fxOn;
    sendIntMessage("/fxOn",fxOnInt);
}

void ofApp::playSwitch(){
    checkSafeShutdown();
    playOn = !playOn;
    int playOnInt = playOn;
    if(playingMode == "SAMPLER"){ recurPlayer.setPlay(playOn);}
    else if(playingMode == "SHADERS"){ shaderPlayer.setPlay(playOn); }
    sendIntMessage("/playOn", playOnInt);
}

void ofApp::setShaderParam0(float value){
    if(selectedInputMode == "SHADERS" && !fxScreenVisible ){
        shaderPlayer.shaderParams[0] = value;
    }
    else{
        fxPlayer.shaderParams[0] = value;
    }
}

void ofApp::setShaderParam1(float value){
    if(selectedInputMode == "SHADERS" && !fxScreenVisible ){
        shaderPlayer.shaderParams[1] = value;
    }
    else{
        fxPlayer.shaderParams[1] = value;
    }
}

void ofApp::setShaderParam2(float value){
    if(selectedInputMode == "SHADERS" && !fxScreenVisible ){
        shaderPlayer.shaderParams[2] = value;
    }
    else{
        fxPlayer.shaderParams[2] = value;
    }
}

void ofApp::setShaderSpeed(float value){
    if(selectedInputMode == "SHADERS" && !fxScreenVisible ){
        shaderPlayer.setSpeed(value);
    }
    else{
        fxPlayer.setSpeed(value);
    }
}

void ofApp::setShaderParam0Cv(float value){
    if(clip1v){value = value * 5.0;}
    if(selectedInputMode == "SHADERS" && !fxScreenVisible ){
        shaderPlayer.shaderParams[0] = value;
    }
    else{
        fxPlayer.shaderParams[0] = value;
    }
}

void ofApp::setShaderParam1Cv(float value){
    if(clip1v){value = value * 5.0;}
    if(selectedInputMode == "SHADERS" && !fxScreenVisible ){
        shaderPlayer.shaderParams[1] = value;
    }
    else{
        fxPlayer.shaderParams[1] = value;
    }
}

void ofApp::setShaderParam2Cv(float value){
    if(clip1v){value = value * 5.0;}
    if(selectedInputMode == "SHADERS" && !fxScreenVisible ){
        shaderPlayer.shaderParams[2] = value;
    }
    else{
        fxPlayer.shaderParams[2] = value;
    }
}

void ofApp::setShaderSpeedCv(float value){
    if(clip1v){value = value * 5.0;}
    if(selectedInputMode == "SHADERS" && !fxScreenVisible ){
        shaderPlayer.setSpeed(value);
    }
    else{
        fxPlayer.setSpeed(value);
    }
}

void ofApp::enter(){
    closeUnusedInput();
    if(fxScreenVisible){
        fxPlayer.loadShaderFiles("default.vert", fxList[selectedFxRow]);        
        playingFxRow = selectedFxRow;
        sendIntMessage("/playingFxRow", playingFxRow);
        fxOn = true;   
        sendIntMessage("/fxOn",1);
    } 
    else if(selectedInputMode == "SAMPLER"){
        playVideo(currentList[selectedRow]);
        playingSampleRow = selectedRow;
        sendIntMessage("/playingSampleRow", playingSampleRow);
        playingMode = selectedInputMode;
        playOn = true;
        sendIntMessage("/playOn",1);
    } 
    else if(selectedInputMode == "SHADERS"){
        shaderPlayer.loadShaderFiles("default.vert", currentList[selectedRow]);
        playingShaderRow = selectedRow;
        sendIntMessage("/playingShaderRow", playingShaderRow);
        playingMode = selectedInputMode;
        playOn = true;
        sendIntMessage("/playOn",1);
    }   
    else if(selectedInputMode == "CAMERA"){
        if(currentList[selectedRow] == "start"){
            videoInput.setup("vidGrabber", ofGetWidth(), ofGetHeight(), 25 );
            isCameraOn = true;
            sendIntMessage("/isCameraOn", 1);
            // uncomment this to allow sample recording
            //cameraList = {"record"};
            //currentList = cameraList;
            //sendListMessage("/cameraList", cameraList);
            playingMode = selectedInputMode;
            playOn = true;
        }
        else if(currentList[selectedRow] == "record"){
            videoInput.startRecording();
            isCameraRecording = true;
            cameraList = {"stop"};
            currentList = cameraList;
            sendListMessage("/cameraList", cameraList);
        }
        else if(currentList[selectedRow] == "stop"){
            videoInput.stopRecording();
            cameraList = {"saving..."};
            currentList = cameraList;
            sendListMessage("/cameraList", cameraList);
        }
        
    }
}

void ofApp::switchInput(){
	//fxList = getPathFromInternalAndExternal("FX");
//	//sendListMessage("fxList", fxList);
    inputIndex = (inputIndex + 1) % inputModes.size() ;
    selectedInputMode = inputModes[inputIndex];

    sendStringMessage("/inputMode", selectedInputMode);
    if(selectedInputMode == "SAMPLER"){
        currentList = sampleList;
        sendListMessage("/sampleList", sampleList);
    }
    else if(selectedInputMode == "SHADERS"){
        currentList = shaderList;
        sendListMessage("/shaderList", shaderList);
    }
    else if(selectedInputMode == "CAMERA"){
        if(!isCameraOn){currentList = {"start"};}
        //else{currentList = {"record"};}
        
    } 

    selectedRow = 0;
    sendIntMessage("/selectedRow", selectedRow);
}

void ofApp::closeUnusedInput(){
    if(selectedInputMode != "SAMPLER"){
        recurPlayer.closeAll();
        playingSampleRow = -1;
        sendIntMessage("/playingSampleRow", playingSampleRow);
    }
    else if(selectedInputMode != "CAMERA"){
        videoInput.close();
        isCameraOn = false;
        cameraList = {"preview"};
        sendListMessage("/cameraList", cameraList);
        sendIntMessage("/isCameraOn", 0);
    }
    else if(selectedInputMode != "SHADERS"){
        playingShaderRow = -1;
        sendIntMessage("/playingShaderRow", playingShaderRow);
    }
}

void ofApp::sendStringMessage(string address, string value){
    ofxOscMessage response;
    response.setAddress(address);
    response.addStringArg(value);

    sender.sendMessage(response, true);
}

void ofApp::playVideo(string path){
    recurPlayer.startSingle(path);
}

void ofApp::sendIntMessage(string address, int value){
    ofxOscMessage response;
    response.setAddress(address);
    response.addIntArg(value);

    sender.sendMessage(response, true);
}

void ofApp::sendListMessage(string address, vector<string> list){
    ofxOscMessage response;
    response.setAddress(address);
    for( int i = 0; i < list.size(); i++ ){
        response.addStringArg(list[i]);    
    }
    //response.addStringArg(value);

    sender.sendMessage(response, true);
}

bool alphabetical(string a, string b){return a<b;}

vector<string> ofApp::getPathFromInternalAndExternal(string mode){
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
    if(mode == "SAMPLER"){sourcePath = "/Videos/";}
    else if(mode == "SHADERS"){sourcePath = "/Shaders/";}
    else if(mode == "FX"){sourcePath = "/Fx/";}
    else {sourcePath = "/";}
    vector<string> sourceList;
    // for each external device get the source files from it and add the sourceList
    if(deviceList.size() > 0){
        for(int i = 0; i < deviceList.size(); i++){
            vector<string> theseFiles = getPathsInFolder(deviceList[i] + sourcePath, mode);
            sourceList.insert(sourceList.end(), theseFiles.begin(), theseFiles.end());
        }
    }
    // get the source files from the internal path
    vector<string> internalFiles = getPathsInFolder("/home/pi" + sourcePath, mode);
    sourceList.insert(sourceList.end(), internalFiles.begin(), internalFiles.end());
    
    ofSort(sourceList, alphabetical);

    for(int i = 0; i < sourceList.size(); i++){
        ofLog() << "sortlist " << sourceList[i];
    }

    return sourceList;
}


vector<string> ofApp::getPathsInFolder(string folderPath, string mode){
    vector<string> thisList; 
    ofDirectory dir(folderPath);
//	dir = dir.getSorted();
    
    if(mode == "SAMPLER"){
        dir.allowExt("mp4");
        dir.allowExt("mov");
        dir.allowExt("avi");
        dir.allowExt("mkv");      
    } else if(mode == "SHADERS" || mode == "FX"){
        dir.allowExt("frag");
        dir.allowExt("glsl");
        dir.allowExt("glslf");
        dir.allowExt("fsh");
    }
    dir.listDir();

    for(int i = 0; i < dir.size(); i++){
        thisList.push_back(dir.getPath(i));  
    }
    return thisList;
}

bool ofApp::detectCamera(){
    // this code is used to detect a camera in csi port (not needed for now)
    //string resp = myExec("vcgencmd get_camera");
    //string substr = resp.substr(20, 21);
    //return resp == "supported=1 detected=1\n";

    //this code is to detect usb capture devices
    ofVideoGrabber vidGrabber;
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    return devices.size() > 0;
}

bool ofApp::diskspaceFull(){
    string info = myExec("df -H /");
    ofLog() << info;
    int pos = info.find("% ", 40); // look past the % in heading
    string capStr = info.substr(pos-3, pos-1);
    int capInt = ofToInt(capStr);
    return capInt > 98;
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

void ofApp::checkRecording(){
    if(cameraList[0] == "saving..."){
        if(videoInput.isRecordingFinished){
            isCameraRecording = false;
            ofLog() << "check recording says finished";
            cameraList = {"record"};
            currentList = cameraList;
            sendListMessage("/cameraList", cameraList);
            renameNewSample();
            sampleList = getPathFromInternalAndExternal("SAMPLER");
            sendListMessage("/sampleList", sampleList);
        }    
    }
    else if(diskspaceFull()){
            videoInput.stopRecording();
            cameraList = {"saving..."};
            currentList = cameraList;
            sendListMessage("/cameraList", cameraList);
    }
}

void ofApp::renameNewSample(){
    string sampleBase = ofGetTimestampString("%F");
    int count = 0;
    string rawPath = "/home/pi/Videos/raw.mp4";
    ofFile raw(rawPath, ofFile::ReadOnly);
    ofLog() << "does raw.mp4 exist ? *****" << raw.exists();
    if(raw.exists()){
        string newPath = "";
        while(true){
            newPath = "/home/pi/Videos/rec-" + sampleBase + + "-" + ofToString(count) + ".mp4";
            ofFile newFile(newPath);
            ofLog() << "does " + newPath + " exist ??? " << newFile.exists() ;    
            if(newFile.exists()){count++;}
            else{break;}
        }
    //system("mv " + rawPath + " " + newPath);
    raw.moveTo(newPath, false);       
    }    
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
        ofLog() << "do the shutdown now !!";
        system("sudo shutdown -h now");
    }
    safeShutdownLastTime = nowGetTime;
}
