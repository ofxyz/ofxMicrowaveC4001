#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	mws.setup("/dev/i2c-1", 0x2A);
}

//--------------------------------------------------------------
void ofApp::update() {
	usleep(10000);
	if (mws.isFake()) {
		// No GPIO access
		ofLog() << "No GPIO access... ";
		currDist = mws.getFakeTargetRange();
	}
	else {
		// GPIO access
		ofLog() << "Getting Data... ";
		targetCount = mws.getSensor()->getTargetNumber();
		if(targetCount > 0) {
			//mws.getSensor()->getTargetNumber();
			currDist = mws.getSensor()->getTargetRange();
		} else {
			currDist = 0;
		}
		
		ofLog() << "Target Count: " << (int)targetCount << "\n Current Distance" << currDist;

		//if(mws.getSensor()->getTargetNumber() > 0) {
		//}

		//ofLog() << "Getting target Range ...";
		//currDist = mws.getSensor()->getTargetRange();
		//
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	if(mws.isFake()) ofDrawBitmapString("Using fake data...", 50, 50);
	
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
