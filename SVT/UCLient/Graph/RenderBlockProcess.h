#pragma once
#include "BasicClass/lib/Renderer.h"

void RenderBlockProcess();

namespace RenderBlock
{
	struct UBasic {
		float x, y, z;
		float tx, ty;
		float light;
	};

	extern bool shouldUpdate;
	extern unsigned RendererN;
	extern unsigned RendererNm[10];
	extern unsigned RendererN1;

	extern RenderBlock::UBasic wh[1024 * 256 * 4 * 8 * 4 ];
	extern RenderBlock::UBasic wh1[1024 * 256 * 4 * 8 * 4 ];
	extern RenderBlock::UBasic whm[10][1024 * 256];

	extern void ProcessInput(float deltatime);
	extern unsigned indices[65537 * 4 * 4 * 8 * 4 ];

	extern unsigned offset, offset1, offsetm[10];
	extern void RegisterBlock(float x, float y, float z, unsigned short sur = 0b111111, int id = 1, unsigned layout = 0, Generation::WorldUnit* unit = nullptr, unsigned int nbt = 0U);
	extern void RegisterGUI(float stx, float sty, float endx, float endy, float wstx, float wsty, float wendx, float wendy, unsigned lay = 4);
	extern void ClearAll();

	extern float GWidth;
	extern float GHeight;

	extern int WinWidth;
	extern int WinHeight;

	extern bool ProgramEnd;
	extern glm::vec3 cameraPos;
	

	extern glm::vec3 camFront;
	extern glm::vec3 camUp;

	extern GLFWwindow* window;

	extern float lstFrame, currentFrame;
	extern glm::vec3 Velocity;

	extern bool OnGround;
	extern bool ChunkShouldUpdate;
	extern bool LockCursor;
	extern bool LastPressE;

	extern glm::vec3 PlayerLookAt(glm::vec3* surf = nullptr);

	extern unsigned SelectedBlock;
	extern double MouseX, MouseY;
	extern bool IsInWater;
}