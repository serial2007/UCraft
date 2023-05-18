#pragma once
#include "../Generation/Entry.h"
#include "../UCLient/Graph/RenderBlockProcess.h"
namespace UserAction
{
	extern void PLayerMain();
	extern bool IsStuck(glm::vec3 pos);
	extern bool IsInWater(glm::vec3 pos);
	extern void WaterPush(float deltatime);
}