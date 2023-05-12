#include "DynamicRegister.h"



void DynamicRegister()
{
	//RenderBlock::cameraPos;
	while(RenderBlock::window == nullptr) {}
	int lx = 0xffffff, ly = 0xffffff;
	while (!RenderBlock::ProgramEnd)
	{
		//RenderBlock::RegisterDone = 0;
		//while(RenderBlock::shouldUpdate) {}
		
		int x = IntDiv(RenderBlock::cameraPos.x, 16);
		int y = -IntDiv(RenderBlock::cameraPos.z, 16);
		std::cout << x << ' ' << y << std::endl;
		RenderBlock::RegisterDone = 1;
		

		if (RenderBlock::processInput(RenderBlock::window, RenderBlock::currentFrame - RenderBlock::lstFrame) && (x != lx || y != ly) || lx == 0xffffff)
		{
			RenderBlock::shouldUpdate = 0;
			RenderBlock::ClearAll();
			for (int i = -3; i <= 3; ++i)
				for (int j = -3; j <= 3; ++j)
				{
					auto p = Enquiry(i + x, j + y);
					UGraph::DrawChunk(p);
					//std::cout << "Registered chunk (" << i + x << ", " << j + y << ")\n";
				}
			lx = x; ly = y;

			int xu = IntDiv(x, 16);
			int yu = IntDiv(y, 16);

			std::map<std::pair<int, int>, Generation::WorldUnit*>::iterator it;
			for (it = GenMain::WorldUnitTmp.begin(); it != GenMain::WorldUnitTmp.end();)
			{
				if (it->second->x < xu - 1 || it->second->x > xu + 1 || it->second->y > yu + 1 || it->second->y < yu - 1)
				{
					it->second->Save();
					std::cout << "Uninstalled Unit (" << it->second->x << ", " << it->second->y << ")\n";
					GenMain::WorldUnitTmp.erase(it++);

					//system("pause");
				}
				else  it++;

			}
		}
		RenderBlock::lstFrame = RenderBlock::currentFrame;
		RenderBlock::shouldUpdate = 1;
	}
}
