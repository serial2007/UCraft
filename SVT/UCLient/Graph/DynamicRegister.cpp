#include "DynamicRegister.h"
#include "SmoothLight.h"
#pragma omp parallel for



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
		//std::cout << x << ' ' << y << std::endl;
		RenderBlock::RegisterDone = 1;
		
		//std::cout << "keybo";

		if ((x != lx || y != ly) || lx == 0xffffff || RenderBlock::ChunkShouldUpdate)
		{
			
			RenderBlock::ClearAll();
			for (int i = -7; i <= 7; ++i)
				for (int j = -7; j <= 7; ++j)
				{
					auto p = Enquiry(i + x, j + y);
					//std::cout << "Register chunk (" << i + x << ", " << j + y << ")\n";
					if(p != nullptr)
					UGraph::DrawChunk(p, GenMain::WorldUnitTmp[std::make_pair(IntDiv(i + x, 16), IntDiv(j + y, 16))]);



					
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
					delete it->second;
					GenMain::WorldUnitTmp.erase(it++);

					//system("pause");
				}
				else  it++;

			}
			RenderBlock::ChunkShouldUpdate = 0;
		}

		for (int i = 1; i < 2; ++i)
		{
			memcpy(RenderBlock::wh + RenderBlock::offset, RenderBlock::whm[i], RenderBlock::offsetm[i] * sizeof(RenderBlock::UBasic));
			RenderBlock::offset += RenderBlock::offsetm[i];
			RenderBlock::RendererN += RenderBlock::RendererNm[i];
		}




		RenderBlock::shouldUpdate = 0;
		memcpy(RenderBlock::wh1, RenderBlock::wh, sizeof(RenderBlock::wh));

		RenderBlock::RendererN1 = RenderBlock::RendererN;
		RenderBlock::offset1 = RenderBlock::offset;

		RenderBlock::lstFrame = RenderBlock::currentFrame;
		RenderBlock::shouldUpdate = 1;
	}
}
