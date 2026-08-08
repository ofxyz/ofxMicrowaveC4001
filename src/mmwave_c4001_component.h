#pragma once

#include "ofMain.h"
#include <entt.hpp>

// ============================================================================
// DFRobot C4001 mmWave — ECS POD owned by ofxMicrowaveC4001
// One entity per bus (0=onboard I2C, 1=UART1, 2=UART2).
// Network ingress (HTTP/Pebble) lives in ofxESP32::SensorIngressSystem.
// ============================================================================

namespace ecs {

struct mmwave_c4001_component {
	int bus = 0;
	entt::entity networkDevice = entt::null;

	bool connected = false;
	bool present = false;
	bool triggered = false;
	bool wasTriggered = false;
	bool justTriggered = false;
	bool justCleared = false;
	bool approaching = false;
	bool receding = false;

	float distance = 0.0f;
	float speed = 0.0f;
	uint8_t targets = 0;

	float triggerDistanceMin = 0.0f;
	float triggerDistanceMax = 3.0f;

	void clearEdges() {
		justTriggered = false;
		justCleared = false;
	}
};

} // namespace ecs
