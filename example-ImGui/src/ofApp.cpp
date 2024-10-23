#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(30);
	ofSetLogLevel(OF_LOG_VERBOSE);

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

	int i = 0;
	static std::string sid;
	// Todo: Maybe we should send the C4001Window the vector
	// Then it can create a tab for every MmWaveSensor?
	for(mmSensor* s : mmWaveSensors.getSensors() ) {
		sid = "C4001Window" + std::to_string(++i);
		ImGui::PushID(sid.c_str());
		drawC4001Window(s);
		ImGui::PopID();
	}

	gui.end();

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::drawC4001Window(mmSensor* sensor)
{
	ImGui::Begin(sensor->getName().c_str());
	static float scale = 1;

	ImGui::Checkbox("Motion detected", &sensor->motionDetected);
	ImGui::Text("Target Distance: %.2f meters", sensor->targetDist);

	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 pos2 = ImVec2(ImGui::GetContentRegionAvail().x * 0.5, 25);
	float maxRadius = std::min(pos2.x, pos2.y) * 0.5;
	float radius = ofClamp(ofMap(sensor->targetDist, 0, 5, 0, maxRadius), 0, maxRadius);
	ImGui::GetWindowDrawList()->AddCircleFilled(pos+pos2, radius*scale, IM_COL32(0, 255, 0, 255), 20);

	ImGui::Dummy({50,50});

	ImGui::DragFloat("Scale", &scale, 0.25, 1, 10, "%.3f", ImGuiSliderFlags_Logarithmic );

	if(ImGui::DragIntRange2("Detect", &sensor->detectRange.x, &sensor->detectRange.y, 1, 30, 2000, "Min: %d cm", "Max %d cm", ImGuiSliderFlags_AlwaysClamp)){
		sensor->updateDevice();
	}
	if(ImGui::SliderScalar("Trigger Distance", ImGuiDataType_U32, &sensor->detectRange.z, &sensor->detectRange.x, &sensor->detectRange.y, "%d cm", ImGuiSliderFlags_AlwaysClamp)){
		sensor->updateDevice();
	}
	
	bool inSync = sensor->isInSync();
	ImGui::Checkbox("Synced", &inSync);

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
