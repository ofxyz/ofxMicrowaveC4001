#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(30);

	mmWaveSensors.setup();

	gui.setup(nullptr, false, ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable, true);
	ImGui::StyleColorsLight();
}

//--------------------------------------------------------------
void ofApp::update() {

	mmWaveSensors.update();

}

//--------------------------------------------------------------
void ofApp::draw() {
	gui.begin();

	// Define the ofWindow as a docking space
	ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);

	for(mmSensor* s : mmWaveSensors.getSensors() ) {
		drawC4001Window(s);
	}

	gui.end();

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::drawC4001Window(mmSensor* sensor)
{
	ImGui::Begin(sensor->getName().c_str());

	ImGui::Text("Target Distance: %.2f meters", sensor->targetDist);

	ImGui::DragIntRange2("Detect", &sensor->detectRange.x, &sensor->detectRange.y, 1, 30, 2000, "Min: %d cm", "Max %d cm", ImGuiSliderFlags_AlwaysClamp);
	ImGui::SliderScalar("Trigger Distance", ImGuiDataType_U32, &sensor->detectRange.z, &sensor->detectRange.x, &sensor->detectRange.y, "%d cm", ImGuiSliderFlags_AlwaysClamp);

	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 pos2 = ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y * 0.5);
	float maxRadius = std::min(pos2.x, pos2.y) * 0.5;
	float radius = ofMap(sensor->targetDist, 0, 12, 0, maxRadius);
	ImGui::GetWindowDrawList()->AddCircleFilled(pos+pos2, radius, IM_COL32(0, 255, 0, 255), 20);
	
	ImGui::Checkbox("Motion detected", &sensor->motionDetected);
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
