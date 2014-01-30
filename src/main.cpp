#include "ofMain.h"
#include "MainApp.h"

//========================================================================
int main( ){



	ofSetupOpenGL(600, ceil(600*16./9.), OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new MainApp());

}
