#pragma once

#include "ofMain.h"
#include "ofJson.h"
#include "ofxImGui.h"
#include "ImHelpers.h"
#include "imgui_internal.h"
#include "ofxMicrowaveC4001.h"

class ofApp: public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		ofxImGui::Gui gui;
		void drawC4001Window();

		ofxMicrowaveC4001 C4001;
		std::string devicePath = "/dev/i2c-1";
		uint8_t deviceAddress = 0x2A;
		// Move these to ofxMicrowaveC4001
		float targetDist = 0;
		uint8_t targetCount = 0;

};
