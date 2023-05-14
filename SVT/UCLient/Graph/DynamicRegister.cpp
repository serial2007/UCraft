#include "DynamicRegister.h"
#include "SmoothLight.h"
#pragma omp parallel for



void DynamicRegister()
{
	bool GoOverWrite = 0;
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

		if ((x != lx || y != ly) || lx == 0xffffff)
		{
			DynamicBackup::tx = x;
			DynamicBackup::ty = y;
			GoOverWrite = 1;
			RenderBlock::ClearAll();
			for (int i = -4; i <= 4; ++i)
				for (int j = -4; j <= 4; ++j)
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
		else if (RenderBlock::ChunkShouldUpdate)
		{
			RenderBlock::ChunkShouldUpdate = 0;
			RenderBlock::offsetm[2] = RenderBlock::offsetm[3] = 0;
			RenderBlock::RendererNm[2] = RenderBlock::RendererNm[3] = 0;

			for (int i = DynamicBackup::tx - 1; i <= DynamicBackup::tx + 1; ++i)
			for (int j = DynamicBackup::ty - 1; j <= DynamicBackup::ty + 1; ++j)
			{
				auto p = Enquiry(i, j);
				if (p != nullptr)
				UGraph::DrawChunk(p, GenMain::WorldUnitTmp[std::make_pair(IntDiv(i, 16), IntDiv(j, 16))], 1);
			}

			GoOverWrite = 1;
			
		}

		if (GoOverWrite)
		{
			GoOverWrite = 0;
			unsigned off = RenderBlock::offset;
			unsigned RendN = RenderBlock::RendererN;
			for (int i = 1; i < 4; ++i)
			{
				memcpy(RenderBlock::wh + off, RenderBlock::whm[i], RenderBlock::offsetm[i] * sizeof(RenderBlock::UBasic));
				off += RenderBlock::offsetm[i];
				RendN += RenderBlock::RendererNm[i];

				
				
			}
			memcpy(RenderBlock::wh1, RenderBlock::wh, sizeof(RenderBlock::wh));
			RenderBlock::RendererN1 = RendN;
			RenderBlock::offset1 = off;

			RenderBlock::lstFrame = RenderBlock::currentFrame;
			RenderBlock::shouldUpdate = 0;
		}
	}
}
