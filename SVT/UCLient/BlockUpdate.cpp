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
#include "Graph/DynamicRegister.h"
void UserAction::btick(std::queue<glm::vec3>* o, glm::vec3 pos, unsigned long tick)
{
	if (*(GenMain::WorldNbt(pos.x, pos.y, pos.z)) & 0xe0000000U) { // 检查水流下方方块
		auto p = GenMain::WorldBlock(pos.x, pos.y - 1.0f, pos.z);
		if (p != nullptr) {
			if (*p == 0 && (*(GenMain::WorldNbt(pos.x, pos.y - 1.0f, pos.z)) & 0xe0000000) == 0) {
				*(GenMain::WorldNbt(pos.x, pos.y - 1.0f, pos.z)) |= 0xe0000000;
				o->push(glm::vec3(pos.x, pos.y - 1.0f, pos.z));
				RenderBlock::ChunkShouldUpdate = 1;
			}
		}
	}
	if ((pos.y == 0 || *(GenMain::WorldBlock(pos.x, pos.y - 1, pos.z)) != 0) && *(GenMain::WorldNbt(pos.x, pos.y, pos.z)) & 0xc0000000U) { // 计算水流方向
		unsigned short potential[9][9];
		memset(potential, 0x7f, sizeof(potential));

		bool undone = 1;
		while (undone)
		{
			undone = 0;
			for (int i = 0; i < 9; ++i)
			for (int j = 0; j < 9; ++j)
			{
				if (*(GenMain::WorldBlock(pos.x + i - 4, pos.y,     pos.z + j - 4)) != 0)	continue;
				if (*(GenMain::WorldBlock(pos.x + i - 4, pos.y - 1, pos.z + j - 4)) == 0)	potential[i][j] = 0;
				if (i > 0 && potential[i][j] > potential[i - 1][j] + 1) { undone = 1; potential[i][j] = potential[i - 1][j] + 1; }
				if (i < 8 && potential[i][j] > potential[i + 1][j] + 1) { undone = 1; potential[i][j] = potential[i + 1][j] + 1; }
				if (j > 0 && potential[i][j] > potential[i][j - 1] + 1) { undone = 1; potential[i][j] = potential[i][j - 1] + 1; }
				if (j < 8 && potential[i][j] > potential[i][j + 1] + 1) { undone = 1; potential[i][j] = potential[i][j + 1] + 1; }
			}
		}
		unsigned short minp = std::min(std::min(potential[4][5], potential[4][3]), std::min(potential[5][4], potential[3][4]));
		unsigned grant = unsigned(*(GenMain::WorldNbt(pos.x, pos.y, pos.z)) & 0xe0000000U) - 0x20000000U;
			if (minp == potential[5][4]) {
				if (*(GenMain::WorldBlock(pos.x + 1, pos.y, pos.z)) == 0 && (*(GenMain::WorldNbt(pos.x + 1, pos.y, pos.z)) & 0xe0000000U) < grant) {
					*(GenMain::WorldNbt(pos.x + 1, pos.y, pos.z)) = grant;
					o->push(glm::vec3(pos.x + 1, pos.y, pos.z));
					RenderBlock::ChunkShouldUpdate = 1;
				}
			}
			if (minp == potential[3][4]) {
				if (*(GenMain::WorldBlock(pos.x - 1, pos.y, pos.z)) == 0 && (*(GenMain::WorldNbt(pos.x - 1, pos.y, pos.z)) & 0xe0000000U) < grant) {
					*(GenMain::WorldNbt(pos.x - 1, pos.y, pos.z)) = grant;
					o->push(glm::vec3(pos.x - 1, pos.y, pos.z));
					RenderBlock::ChunkShouldUpdate = 1;
				}
			}
			if (minp == potential[4][5]) {
				if (*(GenMain::WorldBlock(pos.x, pos.y, pos.z + 1)) == 0 && (*(GenMain::WorldNbt(pos.x, pos.y, pos.z + 1)) & 0xe0000000U) < grant) {
					*(GenMain::WorldNbt(pos.x, pos.y, pos.z + 1)) = grant;
					o->push(glm::vec3(pos.x, pos.y, pos.z + 1));
					RenderBlock::ChunkShouldUpdate = 1;
				}
			}
			if (minp == potential[4][3]) {
				if (*(GenMain::WorldBlock(pos.x, pos.y, pos.z - 1)) == 0 && (*(GenMain::WorldNbt(pos.x, pos.y, pos.z - 1)) & 0xe0000000U) < grant) {
					*(GenMain::WorldNbt(pos.x, pos.y, pos.z - 1)) = grant;
					o->push(glm::vec3(pos.x, pos.y, pos.z - 1));
					RenderBlock::ChunkShouldUpdate = 1;
				}
			}

	}
}

inline void UserAction::RegisterUpdate(float x, float y, float z)
{
	UserAction::UpdateQueue[UserAction::_whcontainer].push(glm::vec3(x, y, z));
}
