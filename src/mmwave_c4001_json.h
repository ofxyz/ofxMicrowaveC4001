#pragma once

#include "component_fields.h"
#include "mmwave_c4001_component.h"

namespace ecs {

template<typename V>
void visitFields(mmwave_c4001_component& c, V&& v)
{
	// Skip live detection state
	v("bus", "Bus", c.bus, 0, 8);
	v("networkDevice", "Network Device", c.networkDevice);
	v("triggerDistanceMin", "Trigger Min", c.triggerDistanceMin, 0.f, 100.f);
	v("triggerDistanceMax", "Trigger Max", c.triggerDistanceMax, 0.f, 100.f);
}

namespace json {

ECS_JSON_POD_CODECS(mmwave_c4001_component)

} // namespace json
} // namespace ecs
