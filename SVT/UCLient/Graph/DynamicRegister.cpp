#include "DynamicRegister.h"
#include "SmoothLight.h"
#include "../ImportInfo.h"
#include "GenBasicLight.h"
#pragma omp parallel for



void DynamicRegister()
{
	bool GoOverWrite = 0;
	//RenderBlock::cameraPos;
	while(RenderBlock::window == nullptr) {}
	int lx = 0xffffff, ly = 0xffffff;
	bool LstPressE = 0;
	while (!RenderBlock::ProgramEnd)
	{
		//RenderBlock::RegisterDone = 0;
		//while(RenderBlock::shouldUpdate) {}
		
		int x = IntDiv(RenderBlock::cameraPos.x, 16);
		int y = -IntDiv(RenderBlock::cameraPos.z, 16);
		//std::cout << x << ' ' << y << std::endl;
		RenderBlock::RegisterDone = 1;
		
		//std::cout << "keybo";
		if (!RenderBlock::LockCursor)
		{
			//std::cout << "GUI\n";
			//std::map<int, ImportInfo::BlockInfo> ImportInfo::binfo;
			unsigned blockn = 0; float ex = -0.8f, ey = 0.8f;
			//RenderBlock::RegisterGUI()

			float mx = RenderBlock::MouseX / RenderBlock::WinWidth * 2 - 1.0f;
			float my = 1.0f - RenderBlock::MouseY / RenderBlock::WinHeight * 2;

			for (auto& it : ImportInfo::binfo) {
				if (glfwGetMouseButton(RenderBlock::window, GLFW_MOUSE_BUTTON_LEFT))
				{
					if (mx >= ex && mx <= float(ex + 160.0f / RenderBlock::WinWidth) && my >= ey - float(160.0f / RenderBlock::WinHeight) && my <= ey)
					{
						RenderBlock::offsetm[7] = 0;
						RenderBlock::RendererNm[7] = 0;

						RenderBlock::SelectedBlock = it.first;
						std::cout << it.first << std::endl;
					}
				}
				RenderBlock::RegisterGUI(ex, ey, ex + 160.0f / RenderBlock::WinWidth, ey - 160.0f / RenderBlock::WinHeight, it.second[2], it.second[3], it.second[0], it.second[1], 6);
				if (RenderBlock::SelectedBlock == it.first)
				{
					RenderBlock::RegisterGUI(
						ex - 10.0f / RenderBlock::WinWidth,
						ey + 10.0f / RenderBlock::WinHeight,
						ex + 170.0f / RenderBlock::WinWidth, 
						ey - 170.0f / RenderBlock::WinHeight, 
						0, 9, 1, 10, 7);
				}

				ex += 200.0f / RenderBlock::WinWidth;
				if (ex > 0.8f)
				{
					ex = -0.8f;
					ey += -200.0f / RenderBlock::WinHeight;
				}

				++blockn;
			}
			GoOverWrite = 1;
			LstPressE = 1;
		}
		else
		{
			if (LstPressE)
			{
				GoOverWrite = 1;
				LstPressE = 0;
				lx = 0xffffff;
			}
		}

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
				memset(p->lit, 0, sizeof(p->lit));
				ULight::GenBsLight(p);
				if (p != nullptr)
				UGraph::DrawChunk(p, GenMain::WorldUnitTmp[std::make_pair(IntDiv(i, 16), IntDiv(j, 16))], 1);
			}

			

			GoOverWrite = 1;
			
		}


		if (GoOverWrite)
		{
			//RenderBlock::RegisterBlock(-0.5f, -0.5f, -10, 1);
			RenderBlock::RegisterGUI(-40.0f / RenderBlock::WinWidth, -40.0f / RenderBlock::WinHeight, 40.0f / RenderBlock::WinWidth, 40.0f / RenderBlock::WinHeight, 0, 0, 9, 9);
			


			GoOverWrite = 0;
			unsigned off = RenderBlock::offset;
			unsigned RendN = RenderBlock::RendererN;
			for (int i = 1; i < 10; ++i)
			{
				memcpy(RenderBlock::wh + off, RenderBlock::whm[i], RenderBlock::offsetm[i] * sizeof(RenderBlock::UBasic));
				off += RenderBlock::offsetm[i];
				RendN += RenderBlock::RendererNm[i];

				
				
			}
			RenderBlock::RendererN1 = RendN;
			RenderBlock::offset1 = off;
			memcpy(RenderBlock::wh1, RenderBlock::wh, sizeof(RenderBlock::wh));

			RenderBlock::lstFrame = RenderBlock::currentFrame;
			RenderBlock::shouldUpdate = 0;
		}
	}
}
