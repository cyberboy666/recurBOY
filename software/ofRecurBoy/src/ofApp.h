#pragma once
#define LONG LONG_FI
#include <FreeImage.h>
#undef LONG
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
    void sendMenuList(string address, vector<string> list);
    void sendIntMessage(string address, int value);
    void sendStringMessage(string address, string value);
    void sendBoolMessage(string address, bool value);
//    bool alphabetical(string a, string b);

//	void keyPressed(int key);
    ofFbo fbo;
    ofFbo fxFbo;
    incur userInput;
    recur recurPlayer;
    conjur shaderPlayer;
    conjur fxPlayer;
    captur videoInput;

void updateSettings(string settingLine);
void readVideoMode();
void updateVideoMode();
void persistVideoMode();
void drawImage();

static string boolToString(bool input){
    return input ? "true" : "false";
}

static bool endsWith(const std::string& str, char ch) {
    if (str.empty()) return false;  // Avoid accessing out-of-bounds
    return str[str.size() - 1] == ch; // Check if last character is '|'
}

static string moveFolderUpOneLevel(const string& path) {
    // Check if the path is one level deep from home or usb0
    // Base paths to check
    string homeBase = "/home/pi";
    string usbBase = "/media/usb0";

    // Find the last slash to move up one level
    string thisPath = path;
    size_t pos = thisPath.find_last_of("/\\");
    if (pos != string::npos) {
        thisPath = thisPath.substr(0, pos); // Return everything before the last slash
    }
    
    // If the current path is exactly the base, return "/"
    if (thisPath == homeBase || thisPath == usbBase) {
        return "/";
    }
    ofLog() << "path: " << thisPath;
    // If path is one level deep from homeBase or usbBase, return "/"
    if (thisPath.rfind(homeBase + "/", 0) == 0 && thisPath.find("/", homeBase.length() + 1) == string::npos) {
        return "/";
    }
    if (thisPath.rfind(usbBase + "/", 0) == 0 && thisPath.find("/", usbBase.length() + 1) == string::npos) {
        return "/";
    }
    
    return thisPath; // Return unchanged if no slash is found
}

    struct settingObject{
        ofxJSONElement jsonObject;
        int adcSecDelay;
        int adcGrain;
        vector<int> adcGrainOptions = {2,3,4,5,6,7,8,9,10,11};
        bool devMode;
        int framerate;
        string videoMode;
        string cvbsType;
        string hdmiType;
        string cvUpper;
        bool textEnable;
        bool videoSpeedEnable;
        bool cvButton;
        string imageRatio;
        bool showFps;
        bool pixelOffset;
        bool assetServer = false;
        bool accessPoint = false;
        bool firstBoot;

        void loadJson(){
            bool parsingSuccessful = jsonObject.open("settings.json");

            adcSecDelay = jsonObject["ADC_SEC_DELAY"].asInt();
            adcGrain = jsonObject["ADC_GRAIN"].asInt();
            devMode = jsonObject["DEV_MODE"].asBool();
            framerate = jsonObject["FRAMERATE"].asInt();
            videoMode = jsonObject["VIDEO_MODE"].asString();
            cvbsType = jsonObject["CVBS_TYPE"].asString();
            hdmiType = jsonObject["HDMI_TYPE"].asString();
            cvUpper = jsonObject["CV_UPPER"].asString();
            textEnable = jsonObject["TEXT_ENABLE"].asBool();
            videoSpeedEnable = jsonObject["VIDEO_SPEED_ENABLE"].asBool();
            cvButton = jsonObject["CV_BUTTON"].asBool();
            imageRatio = jsonObject["IMAGE_RATIO"].asString();
            showFps = jsonObject["SHOW_FPS"].asBool();
            pixelOffset = jsonObject["PIXEL_OFFSET"].asBool();
            firstBoot = jsonObject["FIRST_BOOT"].asBool();

        }
        void saveJson(){

            jsonObject["ADC_SEC_DELAY"] = adcSecDelay;
            jsonObject["ADC_GRAIN"] = adcGrain;
            jsonObject["DEV_MODE"] = devMode;
            jsonObject["FRAMERATE"] = framerate;
            jsonObject["VIDEO_MODE"] = videoMode;
            jsonObject["CVBS_TYPE"] = cvbsType;
            jsonObject["HDMI_TYPE"] = hdmiType;
            jsonObject["CV_UPPER"] = cvUpper;
            jsonObject["TEXT_ENABLE"] = textEnable;
            jsonObject["VIDEO_SPEED_ENABLE"] = videoSpeedEnable;
            jsonObject["CV_BUTTON"] = cvButton;
            jsonObject["IMAGE_RATIO"] = imageRatio;
            jsonObject["SHOW_FPS"] = showFps;
            jsonObject["PIXEL_OFFSET"] = pixelOffset;
            jsonObject["FIRST_BOOT"] = firstBoot;

            jsonObject.save("settings.json", true);
        }
        vector<string> generateMenuList(){
            vector<string> menuList;
            menuList.push_back("VIDEO_MODE: " + videoMode);
            menuList.push_back("CVBS_TYPE: " + cvbsType);
            menuList.push_back("HDMI_TYPE: " + hdmiType);
            menuList.push_back("CV_UPPER: " + cvUpper);
            menuList.push_back("TEXT_ENABLE: " + boolToString(textEnable));
            menuList.push_back("VIDEO_SPEED_ENABLE: " + boolToString(videoSpeedEnable));
            menuList.push_back("CV_BUTTON: " + boolToString(cvButton));
            menuList.push_back("IMAGE_RATIO: " + imageRatio);
            menuList.push_back("SHOW_FPS: " + boolToString(showFps));
            menuList.push_back("ADC_GRAIN: " + std::to_string(adcGrain));
            menuList.push_back("PIXEL_OFFSET: " + boolToString(pixelOffset));
            menuList.push_back("EJECT_USB:");
            menuList.push_back("COPY_CONTENT:");
            menuList.push_back("ASSET_SERVER: " + boolToString(assetServer));
            menuList.push_back("ACCESS_POINT: " + boolToString(accessPoint));
            menuList.push_back("SHUTDOWN:");
            menuList.push_back("FIRMWARE: v3.1.0s");

            return menuList;
        }

        void advanceAdcGrain() {
            auto it = std::find(adcGrainOptions.begin(), adcGrainOptions.end(), adcGrain);
            if (it != adcGrainOptions.end()) {
                int index = std::distance(adcGrainOptions.begin(), it);
                index = (index + 1) % adcGrainOptions.size(); // wrap around
                adcGrain = adcGrainOptions[index];
            } else {
                // adcGrain is not currently in the options list - optionally reset or ignore
                adcGrain = adcGrainOptions[0];
            }
        }

    };

    settingObject setting;
    float lastCv0Value;
    float lastCv1Value;
    float lastCv2Value;
    float lastCv3Value;

    void printFramerate();
    void readActions();
	void runAction(string action, string amount);

	void exit();

    bool isSampleImage;
    ofImage img;

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
    void switchSource();
    void playVideo(string path);
    void closeUnusedInput();

    void setParam0(float value);
    void setParam1(float value);
    void setParam2(float value);
    void setSpeed(float value);

    void setVideoSpeed(float value);

    void setParam0Invert(float value);
    void setParam1Invert(float value);
    void setParam2Invert(float value);
    void setSpeedInvert(float value);

    void setShaderParam0(float value);
    void setShaderParam1(float value);
    void setShaderParam2(float value);
    void setShaderSpeed(float value);

    void setEffectParam0(float value);
    void setEffectParam1(float value);
    void setEffectParam2(float value);
    void setEffectSpeed(float value);

    void setTextDisplaceX(float value);
    void setTextDisplaceY(float value);
    void setTextDisplaceWidth(float value);
    void setTextDisplaceHeight(float value);
    void setTextColorHue(float value);
    void setTextColorSat(float value);
    void setTextColorBrightness(float value);
    void setTextColorAlpha(float value);

    void setParam0Cv(float value);
    void setParam1Cv(float value);
    void setParam2Cv(float value);
    void setSpeedCv(float value);

    void stepParam0();
    void stepParam1();
    void stepParam2();
    void stepSpeed();
    int stepParam0Value;
    int stepParam1Value;
    int stepParam2Value;
    int stepSpeedValue;

    bool fileIsImageExtension(string path);
    vector<string> getPathsInFolder(string folderPath, string source);
    vector<string> getPathFromCombinedRootLocations(string source);

    struct pageObject{
        string name;
        vector<string> menuList;
        string currentFolder;
        int selectedRow;
        int playingRow;
        bool isPlaying;

        pageObject() : name(""), menuList({}), currentFolder("/"), selectedRow(0), playingRow(-1), isPlaying(false) {}

    };

    pageObject videoPage;
    pageObject patternPage;
    pageObject  externalPage;
    pageObject  effectPage;
    pageObject  settingPage;
    pageObject  textPage;
    pageObject  fontPage;
    pageObject  messagePage;

    pageObject& getPagebyName(string pageName);
    vector<string> getPathsForMenu(pageObject& thisObject);
    void regenerateAndSendList(pageObject& thisObject);

    string currentPage; // currentPage is the page shown on display
    string runningSource; // runningSource is the source that is being used right now (could be different to what is shown on display)

    vector<string> sourceModes;
    // int runningSourceIndex; 
    int currentSourceIndex; 

    bool isCameraDetected;
    bool isCameraRecording;  
    // text variables
    struct textObject{
        float x, y, width, height, scale, alpha, hue, sat, brightness;
        ofTrueTypeFont myFont;
        string text;

        textObject() : x(0.2), y(0.2), width(0.2), height(0.2), alpha(1.0), hue(0.2), sat(0.7), brightness(0.7) {}

        

        void draw(){
            ofPushStyle();
            ofColor thisColor;
            thisColor.setHsb(static_cast<int>(hue * 255), static_cast<int>(sat * 255), static_cast<int>(brightness * 255), static_cast<int>(alpha * 255));
            ofSetColor(thisColor);
            ofTranslate(x * ofGetWidth(), (1.0 - y) * ofGetHeight());
            float xScale = ofMap(width, 0.0f, 1.0f, 0.2f, 5.0f);
            float yScale = ofMap(height, 0.0f, 1.0f, 0.2f, 5.0f);
            ofScale(xScale, yScale);
            myFont.drawString(this->text, 0,0);
            //ofScale(xScale, yScale);
            ofPopStyle();
        }

        void loadFont(string pathToFont){
            myFont.load(pathToFont, 40);
        }

        void loadText(string pathToTextFile){
            ofFile file(pathToTextFile);
            if (file.exists()) {
                ofBuffer buffer = file.readToBuffer(); // Read file content into a buffer
                text = buffer.getText();              // Get the text content as a string
            }
        }
    };

    void showMessage(string message);
    void removeMessage();

    void liveTextKey(int key);
    bool isLiveText;
    textObject thisTextObject;

    int safeShutdownCount;
    float safeShutdownLastTime;
    void checkSafeShutdown();
    bool getImageSize(const std::string& path, int &w, int &h);

};
