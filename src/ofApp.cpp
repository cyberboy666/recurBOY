#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0, 0, 0);
	ofSetVerticalSync(false);
    
    // screen in dev modefor testing ...
    ofSetFullscreen(1);
    //ofSetWindowShape(300,200);
    //ofSetWindowPosition(50,500);
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    fxFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);

    userInput.setupThis("actionMap.json");

    sender.setup("localhost", 9000);

    sampleList = getPathsInFolder("/home/pi/Videos/");
    sendListMessage("/sampleList", sampleList);
    shaderList = getPathsInFolder("/home/pi/Shaders/");
    sendListMessage("/shaderList", shaderList);
    fxList = getPathsInFolder("/home/pi/Fx/");
    sendListMessage("/fxList", fxList);

    inputModes = {"SAMPLER", "SHADERS", "CAMERA"};
    inputIndex = 0;
    currentList = sampleList;
    fxScreenVisible = false;
    fxOn = false;
    isCameraOn = false;
    isCameraRecording = false;

    selectedRow = 0;

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
    sendListMessage("/sampleList", sampleList);
    //make ofExit();
 }


void ofApp::moveUp(){
    if (selectedRow > 0){ selectedRow--;}
    sendIntMessage("/selectedRow", selectedRow);
}

void ofApp::moveDown(){
    if(selectedRow < currentList.size() - 1){ 
        selectedRow++; 
        ofLog() << "incrementing the selected row tp " << selectedRow;
}
    sendIntMessage("/selectedRow", selectedRow);
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
        ofLog() << fxList[selectedRow];
        fxPlayer.loadShaderFiles("default.vert", fxList[selectedRow]);        
        fxOn = true;   
    } 
    else if(selectedInputMode == "SAMPLER"){
        playVideo(currentList[selectedRow]);
        playingMode = selectedInputMode;
    } 
    else if(selectedInputMode == "SHADERS"){
        shaderPlayer.loadShaderFiles("default.vert", currentList[selectedRow]);
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
    }
    else if(selectedInputMode != "CAMERA"){
        videoInput.close();
        isCameraOn = false;
        sendIntMessage("/isCameraOn", 0);
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

vector<string> ofApp::getPathsInFolder(string folderPath){
    vector<string> thisList;
    ofDirectory dir(folderPath);
    dir.listDir();  

    for(int i = 0; i < dir.size(); i++){
        thisList.push_back(dir.getPath(i));  
    }
    return thisList;
}

