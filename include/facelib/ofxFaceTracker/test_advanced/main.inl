#include "ofApp.h"
#include "ofAppGlutWindow.h"

int test_advanced() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 640, 480, OF_WINDOW);
	ofRunApp(new ofApp());
}
