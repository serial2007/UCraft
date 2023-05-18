#pragma once
#include "../Generation/Entry.h"

namespace UserAction {
	extern void update();
	extern void tick(unsigned long tick);
	extern void btick(std::queue<glm::vec3>* o, glm::vec3 pos, unsigned long tick);
	extern unsigned long tottick;
	extern std::queue<glm::vec3> UpdateQueue[2];
	extern bool _whcontainer;
	extern inline void RegisterUpdate(float x, float y, float z);
}