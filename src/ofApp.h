#pragma once

#include "ofMain.h"
#include "incur.h"
#include "ofxOsc.h"
#include "recur.h"
#include "conjur.h"
#include "captur.h"


class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

    ofxOscSender sender;
    void sendListMessage(string address, vector<string> list);
    void sendIntMessage(string address, int value);
    void sendStringMessage(string address, string value);


	void keyPressed(int key);
    ofFbo fbo;
    ofFbo fxFbo;
    incur userInput;
    recur recurPlayer;
    conjur shaderPlayer;
    conjur fxPlayer;
    captur videoInput;

    void readActions();
	void runAction(string action, string amount);

	void exit();

    ofImage splashImg;

    bool diskspaceFull();
    bool detectCamera();
    string myExec(char* cmd);


    void exit(string input);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void enter();
    void fxSwitch();
    void switchInput();
    void playVideo(string path);
    void closeUnusedInput();

    vector<string> getPathsInFolder(string folderPath);

    vector<string> sampleList;
    vector<string> shaderList;
    vector<string> fxList;
    vector<string> currentList;
    string selectedInputMode;
    string playingMode;
    int selectedRow;
    int selectedFxRow;
    int playingShaderRow;
    int playingSampleRow;
    int playingFxRow;
    vector<string> inputModes;
    int inputIndex; 
    bool fxScreenVisible;
    bool fxOn;
    bool isCameraOn;
    bool isCameraDetected;
    bool isCameraRecording;  
};
