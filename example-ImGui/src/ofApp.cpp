#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_NOTICE);

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
			if(ImGui::MenuItem("Scan for new Devices")) {
				mmWaveSensors.scanAdd();
			}
			if(ImGui::MenuItem("New fake Sensor")) {
				mmWaveSensors.addToySensor();
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
            ImGui::Text("Distance: %.2f meters", sensor->targetDist);
			
			float energy = (float)sensor->targetEnergy * 0.001;
			ImGui::Text("Energy: %.2f", energy);

			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 pos2 = ImVec2(ImGui::GetContentRegionAvail().x * 0.5, 25);
			float maxRadius = 25;
			float radius = ofClamp(ofMap(sensor->targetDist, 0, sensor->detectRange.y*0.01, 0, maxRadius), 0, maxRadius);
			ImGui::GetWindowDrawList()->AddCircleFilled(pos+pos2, radius*sensor->zoom, IM_COL32(0, 0, 255, 255), 20);

            ImGui::Dummy({25,50});
			
			ImGui::PushItemWidth(100);
			ImGui::SliderFloat("Zoom", &sensor->zoom, 1, 10, "%.2f", ImGuiSliderFlags_Logarithmic);

            ImGui::Text("Detect Range:");
            if(ImGui::DragIntRange2("cm", &sensor->detectRange.x, &sensor->detectRange.y, 1, 30, 2000, "Min %d", "Max %d", ImGuiSliderFlags_AlwaysClamp)){
                sensor->updateDevice();
            };
            ImGui::Dummy({5,5});
            ImGui::Text("Trigger Settings:");
            
            if(ImGui::DragScalar("Dist", ImGuiDataType_U32, &sensor->detectRange.z, 1, &sensor->detectRange.x, &sensor->detectRange.y, "%d cm", ImGuiSliderFlags_AlwaysClamp)){
                sensor->updateDevice();
            };
            if(ImGui::DragScalar("Trig Sens", ImGuiDataType_U8, &sensor->m_uiTriggerSensitivity, 1, &sensor->sensitivityMin, &sensor->sensitivityMax, "%d", ImGuiSliderFlags_AlwaysClamp)){
                sensor->updateDevice();
            };
            if(ImGui::DragScalar("Keep Sens", ImGuiDataType_U8, &sensor->m_uiKeepSensitivity, 1, &sensor->sensitivityMin, &sensor->sensitivityMax, "%d", ImGuiSliderFlags_AlwaysClamp)){
                sensor->updateDevice();
            };

            ImGui::PopItemWidth();

            bool inSync = sensor->isInSync();
            ImGui::Checkbox("Synced", &inSync);

			ImGui::SameLine();
			if(ImGui::Button("Delete")){
				sensor->m_bDead = true; 
			};

        };
    
        ImGui::PopID();
    };

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
