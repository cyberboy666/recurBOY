#pragma once

#include "ofMain.h"
#include "incur.h"
#include "ofxOsc.h"
#include "recur.h"


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
    incur userInput;
    recur recurPlayer;

    void readActions();
	void runAction(string action, string amount);

	void exit();

    void exit(string input);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void enter();
    void switchInput();
    void playVideo(string path);

    vector<string> getPathsInFolder(string folderPath);

    vector<string> sampleList;
    vector<string> shaderList;
    vector<string> fxList;
    vector<string> currentList;
    string selectedInputMode;
    int selectedRow;
    vector<string> inputModes;
    int inputIndex; 
    bool fxScreenVisible;

};
