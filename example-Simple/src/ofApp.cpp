#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	mmWaveSensors.setup();
	ofBackground(ofColor(0,0,255));
	ofSetCircleResolution(4);
}

//--------------------------------------------------------------
void ofApp::update()
{	
	mmWaveSensors.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255,255,255);
	if(mmWaveSensors.getSensors().size() == 0) {
		ofDrawBitmapString("[I2C] No DfRobot C4001 sensor found...", 50, 50);
	} else {
		ofDrawBitmapString(mmWaveSensors.getSensors()[0]->getName(), 50, 50);
		float maxRadius = std::min(ofGetWidth(), ofGetHeight())*0.5;
		float radius = ofMap(mmWaveSensors.getSensors()[0]->targetDist, 0, 5, 1, maxRadius);
		ofSetColor(255,0,0);
		ofDrawCircle(ofGetWidth()*0.5, ofGetHeight()*0.5, radius);
	}
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
