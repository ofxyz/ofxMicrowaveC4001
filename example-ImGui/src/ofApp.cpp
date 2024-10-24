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

    drawC4001Window(mmWaveSensors.getSensors());

    gui.end();

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::drawC4001Window(std::vector<mmSensor*>& sensors)
{
    bool vissible = true;
    ImGui::Begin("C4001 Sensors", &vissible, ImGuiWindowFlags_MenuBar);

    if(ImGui::BeginMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Save Settings")) {
                ofSavePrettyJson("C4001_Sensors.json", mmWaveSensors.getSettings());
            }
            if(ImGui::MenuItem("Load Settings")) {
                ofJson settings;
                ofFile file(ofToDataPath("C4001_Sensors.json", true));
                if(file.exists()){
                    file >> settings;
                    mmWaveSensors.setSettings(settings);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    int i = 0;
    static std::string sid;

    for(mmSensor* sensor : sensors) {
        sid = "C4001Window" + std::to_string(++i);
        ImGui::PushID(sid.c_str());

        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if(ImGui::CollapsingHeader(sensor->getName().c_str()))
        {    
            ImGui::Checkbox("Motion detected", &sensor->motionDetected);
            ImGui::Text("Target Distance: %.2f meters", sensor->targetDist);

            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 pos2 = ImVec2(ImGui::GetContentRegionAvail().x * 0.5, 25);
            float maxRadius = 25;
            float radius = ofClamp(ofMap(sensor->targetDist, 0, sensor->detectRange.y*0.01, 0, maxRadius), 0, maxRadius);
            ImGui::GetWindowDrawList()->AddCircleFilled(pos+pos2, radius, IM_COL32(0, 0, 255, 255), 20);

            ImGui::Dummy({25,50});

            ImGui::Text("Detect Range:");
            if(ImGui::DragIntRange2("", &sensor->detectRange.x, &sensor->detectRange.y, 1, 30, 2000, "Min %d cm", "Max %d cm", ImGuiSliderFlags_AlwaysClamp)){
                sensor->updateDevice();
            }
            ImGui::Dummy({5,5});
            ImGui::Text("Trigger Settings:");
            ImGui::PushItemWidth(100);
            if(ImGui::SliderScalar("Dist", ImGuiDataType_U32, &sensor->detectRange.z, &sensor->detectRange.x, &sensor->detectRange.y, "%d cm", ImGuiSliderFlags_AlwaysClamp)){
                sensor->updateDevice();
            }
            if(ImGui::SliderScalar("Trig Sens", ImGuiDataType_U8, &sensor->triggerSensitivity, &sensor->sensitivityMin, &sensor->sensitivityMax, "%d", ImGuiSliderFlags_AlwaysClamp)){
                sensor->updateDevice();
            }
            if(ImGui::SliderScalar("Keep Sens", ImGuiDataType_U8, &sensor->keepSensitivity, &sensor->sensitivityMin, &sensor->sensitivityMax, "%d", ImGuiSliderFlags_AlwaysClamp)){
                sensor->updateDevice();
            }
            ImGui::PopItemWidth();

            bool inSync = sensor->isInSync();
            ImGui::Checkbox("Synced", &inSync);

        }
    
        ImGui::PopID();
    }

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
