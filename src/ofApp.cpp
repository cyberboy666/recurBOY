#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0, 0, 0);
	ofSetVerticalSync(false);
    bool isDev = true;
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
    isCameraDetected = detectCamera();
    if(isCameraDetected){inputModes.push_back("CAMERA");}
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

    

}

//--------------------------------------------------------------
void ofApp::update(){
    readActions();

    if(playingMode == "SAMPLER"){
        recurPlayer.update();
        fbo.begin();
            recurPlayer.playerDraw();
        fbo.end();
    }
    else if(playingMode == "SHADERS"){
        fbo = shaderPlayer.apply({});
    }
    else if(playingMode == "CAMERA" && videoInput.isReady() ){
        videoInput.update();
        fbo.begin();
            videoInput.draw(0,0);
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
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(fxOn){
        fxFbo.draw(0,0);
    }
    else{
        fbo.draw(0,0);
    }

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
     else if(action == "switchInput"){ switchInput();}
 }

void ofApp::exit(){
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
    if(!fxScreenVisible){
        fxScreenVisible = true;
        sendIntMessage("/fxScreenVisible", 1);
    }
}

void ofApp::fxSwitch(){
    fxOn = !fxOn;
}

void ofApp::enter(){
    closeUnusedInput();
    if(fxScreenVisible){
        fxPlayer.loadShaderFiles("default.vert", fxList[selectedFxRow]);        
        playingFxRow = selectedFxRow;
        sendIntMessage("/playingFxRow", playingFxRow);
        fxOn = true;   
    } 
    else if(selectedInputMode == "SAMPLER"){
        playVideo(currentList[selectedRow]);
        playingSampleRow = selectedRow;
        sendIntMessage("/playingSampleRow", playingSampleRow);
        playingMode = selectedInputMode;
    } 
    else if(selectedInputMode == "SHADERS"){
        shaderPlayer.loadShaderFiles("default.vert", currentList[selectedRow]);
        playingShaderRow = selectedRow;
        sendIntMessage("/playingShaderRow", playingShaderRow);
        playingMode = selectedInputMode;
    }   
    else if(selectedInputMode == "CAMERA"){
        ofLog() << "camera input " << currentList[selectedRow];
        if(currentList[selectedRow] == "start"){
            videoInput.setup("piCamera", ofGetWidth(), ofGetHeight(), 30 );
            currentList = {"record"};
            isCameraOn = true;
            sendIntMessage("/isCameraOn", 1);
            playingMode = selectedInputMode;
        }
        else if(currentList[selectedRow] == "record"){
            videoInput.startRecording();
            currentList = {"stop"};
        }
        else if(currentList[selectedRow] == "stop"){
            videoInput.stopRecording();
            currentList = {"record"};
        }
        
    }
}

void ofApp::switchInput(){
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
        else{currentList = {"record"};}
        
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

vector<string> ofApp::getPathFromInternalAndExternal(string mode){
    // get a list of external devices
    vector<string> deviceList; 
    ofDirectory dir("/media/pi/");
    dir.listDir();
    for(int i = 0; i < dir.size(); i++){
        ofLog() << "the path " << dir.getPath(i); 
        deviceList.push_back(dir.getPath(i));  
    }
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

    return sourceList;
}

vector<string> ofApp::getPathsInFolder(string folderPath, string mode){
    vector<string> thisList; 
    ofDirectory dir(folderPath);
    
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
    string resp = myExec("vcgencmd get_camera");
    string substr = resp.substr(20, 21);
    ofLog() << "resp !!!!!!!!!!!" << resp << "!!!";
    ofLog() << "substr !!!!!!!!!!!" << substr << "!!!";
    ofLog() << "the retrun is " << ofToString(resp == "supported=1 detected=1\n");
    return resp == "supported=1 detected=1\n";
}

bool ofApp::diskspaceFull(){
    string info = myExec("df -H /");
    ofLog() << info;
    int pos = info.find("% ", 40); // look past the % in heading
    string capStr = info.substr(pos-3, pos-1);
    int capInt = ofToInt(capStr);
    return capInt < 98;
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


