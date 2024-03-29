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
//    bool alphabetical(string a, string b);

	void keyPressed(int key);
    ofFbo fbo;
    ofFbo fxFbo;
    incur userInput;
    recur recurPlayer;
    conjur shaderPlayer;
    conjur fxPlayer;
    captur videoInput;

    ofxJSONElement settings;
    bool clip1v;
    bool isDev;
    bool showFramerate;
    float adcDelay; 
    int appFramerate;

    void printFramerate();
    void readSettings();
    void readActions();
	void runAction(string action, string amount);

	void exit();

    bool isSampleImage;
    ofImage img;

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
    void playSwitch();
    void switchInput();
    void playVideo(string path);
    void closeUnusedInput();

    void setParam0(float value);
    void setParam1(float value);
    void setParam2(float value);
    void setSpeed(float value);

    void setShaderParam0(float value);
    void setShaderParam1(float value);
    void setShaderParam2(float value);
    void setShaderSpeed(float value);

    void setEffectParam0(float value);
    void setEffectParam1(float value);
    void setEffectParam2(float value);
    void setEffectSpeed(float value);

    void setParam0Cv(float value);
    void setParam1Cv(float value);
    void setParam2Cv(float value);
    void setSpeedCv(float value);

    void stepParam0();
    void stepParam1();
    void stepParam2();
    int stepParam0Value;
    int stepParam1Value;
    int stepParam2Value;

    bool fileIsImageExtension(string path);
    vector<string> getPathsInFolder(string folderPath, string source);
    vector<string> getPathFromInternalAndExternal(string source);
    void checkRecording();
    void renameNewSample();

    vector<string> sampleList;
    vector<string> shaderList;
    vector<string> fxList;
    vector<string> cameraList;
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
    bool playOn;
    bool isCameraOn;
    bool isCameraDetected;
    bool isCameraRecording;  
    int safeShutdownCount;
    float safeShutdownLastTime;
    void checkSafeShutdown();

};
