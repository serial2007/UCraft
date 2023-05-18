#include "BlockUpdate.h"

unsigned long		UserAction::tottick = 0U;
std::queue<glm::vec3>		UserAction::UpdateQueue[2];
bool				UserAction::_whcontainer = 0;

void UserAction::update()
{
	while (1) {
		float CTime = glfwGetTime();
		unsigned k = unsigned(CTime / 0.05f) - UserAction::tottick;
		while (k--) {
			
			UserAction::tick(++UserAction::tottick);
		}
	}
}

void UserAction::tick(unsigned long tick)
{
	auto it =	&(UserAction::UpdateQueue[UserAction::_whcontainer]);
	auto save =	&(UserAction::UpdateQueue[!UserAction::_whcontainer]);
	UserAction::_whcontainer = !UserAction::_whcontainer;
	while (!it->empty()) {
		UserAction::btick(save, it->front(), tick);
		it->pop();
	}
}

void UserAction::btick(std::queue<glm::vec3>* o, glm::vec3 pos, unsigned long tick)
{
	if (*(GenMain::WorldNbt(pos.x, pos.y, pos.z)) & 0x80000000) {
		auto p = GenMain::WorldBlock(pos.x, pos.y - 1.0f, pos.z);
		if (p != nullptr) {
			if (*p == 0 && (*(GenMain::WorldNbt(pos.x, pos.y - 1.0f, pos.z)) & 0x80000000) == 0) {
				*(GenMain::WorldNbt(pos.x, pos.y - 1.0f, pos.z)) |= 0x80000000;
				o->push(glm::vec3(pos.x, pos.y - 1.0f, pos.z));
			}
		}
	}
}

inline void UserAction::RegisterUpdate(float x, float y, float z)
{
	UserAction::UpdateQueue[UserAction::_whcontainer].push(glm::vec3(x, y, z));
}
