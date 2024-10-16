#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	mws.setup("/dev/i2c-1", 0x2A);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (mws.isFake()) {
		// No GPIO access
		currDist = mws.getFakeRange();
	}
	else {
		// GPIO access
		currDist = mws.getSensor()->getTargetRange();
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(0,255,0);
	float maxRadius = std::min(ofGetWidth(), ofGetHeight())*0.5;
	float radius = ofMap(currDist, 0, 12, 0, maxRadius);
	ofDrawCircle(ofGetWidth()*0.5, ofGetHeight()*0.5, radius);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
