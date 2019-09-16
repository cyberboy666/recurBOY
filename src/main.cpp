#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofSetLogLevel(OF_LOG_VERBOSE);
	//ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context
    //ofSetupOpenGL(1024,768, OF_WINDOW);
	ofGLESWindowSettings settings;
    settings.setSize(1280, 720);
	settings.setGLESVersion(2);
	ofCreateWindow(settings);


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());


}
