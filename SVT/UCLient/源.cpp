#include "Graph/RenderBlockProcess.h"
#include "Graph/imgui/imgui.h"
#include <thread>
int main()
{
	Renderer::ActivateImgui = 1;

	std::thread Ren(RenderBlockProcess);
	RenderBlock::RegisterBlock(0, 0, 5);
	RenderBlock::RegisterBlock(0, 1, 6);
	RenderBlock::RegisterBlock(0, 2, 3);
	RenderBlock::RegisterBlock(0, -1, 0.9);
//	RenderBlock::RegisterBlock(-1, 0, 0.9);
	//float pp[3] = { 0.0f, 0.0f, 0.0f };
	//ImGui::
	while (1)
	{
		
		RenderBlock::shouldUpdate = 1;
	}
}