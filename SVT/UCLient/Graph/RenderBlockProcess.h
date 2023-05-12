#pragma once
#include "BasicClass/lib/Renderer.h"

void RenderBlockProcess();

namespace RenderBlock
{
	struct UBasic {
		int x, y, z;
		float tx, ty;
	};

	extern bool shouldUpdate;
	extern unsigned RendererN;

	extern RenderBlock::UBasic wh[1024 * 256];

	extern unsigned indices[65537 * 4];

	extern unsigned offset;
	void RegisterBlock(int x, int y, int z, unsigned short sur = 0b111111, int id = 1);

	extern void ClearAll();

	extern float GWidth;
	extern float GHeight;

	extern int WinWidth;
	extern int WinHeight;

	extern bool ProgramEnd;
	extern glm::vec3 cameraPos;
	extern bool processInput(GLFWwindow* window, float);

	extern glm::vec3 camFront;
	extern glm::vec3 camUp;

	extern bool RegisterDone;
	extern GLFWwindow* window;

	extern float lstFrame, currentFrame;
}