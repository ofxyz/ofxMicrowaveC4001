#include "ofApp.h"

#ifndef WIN32
#define Sleep(x) usleep(1000 * x)
#endif

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(30);

	//TODO: Can we try and read the dev/list to find the correct i2c bus?
	/*
	std::vector<std::string> getI2CBusPaths() {
		std::vector<std::string> i2cPaths;
		std::regex i2cPattern("/dev/i2c-\\d+");
		for (const auto& entry : std::filesystem::directory_iterator("/dev")) {
			std::string path = entry.path().string();
			if (std::regex_match(path, i2cPattern)) {
				i2cPaths.push_back(path);
			}
		}
		return i2cPaths;
	}

	std::vector<std::string> i2cBuses = getI2CBusPaths();
	if (!i2cBuses.empty()) {
		// Don't use the first found I2C bus but select one via UI
		C4001.setup(i2cBuses[0].c_str(), 0x2A);
	} else {
		std::cerr << "No I2C buses found." << std::endl;
	}
	*/

	C4001.setup(devicePath.c_str(), deviceAddress);

	gui.setup(nullptr, false, ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable, true);
	ImGui::StyleColorsLight();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (C4001.isFake()) {
		// No GPIO access
		ofLog() << "No GPIO access... ";
		targetDist = C4001.getFakeTargetRange();
	}
	else {
		// GPIO access
		ofLog() << "Getting Data... ";
		if(C4001.getSensor()->getTargetNumber() > 0) {
			targetDist = C4001.getSensor()->getTargetRange();
		} else {
			targetDist = 0;
		}

		ofLog() << "Target Count: " << (int)targetCount << "\n Current Distance" << targetDist;

	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	gui.begin();

	// Define the ofWindow as a docking space
	ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);

	drawC4001Window();

	gui.end();

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::drawC4001Window()
{
	ImGui::Begin("C4001");

	ImGui::Text("Target Distance: %f", targetDist);
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 pos2 = ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y * 0.5);
	float maxRadius = std::min(pos2.x, pos2.y) * 0.5;
	float radius = ofMap(targetDist, 0, 12, 0, maxRadius);
	ImGui::GetWindowDrawList()->AddCircleFilled(pos+pos2, radius, IM_COL32(0, 255, 0, 255), 20);

	ImGui::End();
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
