#include "ofApp.h"

#ifdef __arm__
#define FAKESENSE false
#else
#define FAKESENSE true
#endif

//--------------------------------------------------------------
void ofApp::setup() {
	mws.setup("/dev/i2c-1", 0x2A);
}

//--------------------------------------------------------------
void ofApp::update(){
	mws.update();
	currDist = mws.getSensor()->getTargetRange(FAKESENSE);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(0,255,0);
	ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, currDist*50);
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
