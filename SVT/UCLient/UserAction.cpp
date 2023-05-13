#include "UserAction.h"







void UserAction::PLayerMain()
{
	//while (1) std::cout << '1';
	unsigned short* footd;
	
	

	RenderBlock::lstFrame = glfwGetTime();
	while (1)
	{
		

		auto tmp = RenderBlock::cameraPos;
		while(GenMain::WorldBlock(RenderBlock::cameraPos.x, RenderBlock::cameraPos.y, RenderBlock::cameraPos.z) == nullptr) {
			RenderBlock::Velocity.x = RenderBlock::Velocity.y = RenderBlock::Velocity.z = 0;
			RenderBlock::cameraPos = tmp;
		}

		footd = GenMain::WorldBlock(RenderBlock::cameraPos.x, RenderBlock::cameraPos.y - 2.1f, RenderBlock::cameraPos.z);
		
		auto VelocityLock = RenderBlock::Velocity;
		float x, y, z;
		for (x = 0.0f; x < 1.0f; x += 0.05f)
		{
			auto xv = x * VelocityLock.x;
			if (IsStuck(glm::vec3(RenderBlock::cameraPos.x + xv, RenderBlock::cameraPos.y, RenderBlock::cameraPos.z))) {
				x -= 0.05f; break;
			}
		}
		RenderBlock::cameraPos.x += VelocityLock.x * x;

		for (z = 0.0f; z < 1.0f; z += 0.05f)
		{
			auto zv = z * VelocityLock.z;
			if (IsStuck(glm::vec3(RenderBlock::cameraPos.x, RenderBlock::cameraPos.y, RenderBlock::cameraPos.z + zv))) {
				z -= 0.05f; break;
			}
		}
		RenderBlock::cameraPos.z += VelocityLock.z * z;

		for (y = 0.0f; y < 1.0f; y += 0.05f)
		{
			auto yv = y * VelocityLock.y;
			if (IsStuck(glm::vec3(RenderBlock::cameraPos.x, RenderBlock::cameraPos.y + yv, RenderBlock::cameraPos.z))) {
				y -= 0.05f; break;
			}
		}
		RenderBlock::cameraPos.y += VelocityLock.y * y;

		//RenderBlock::cameraPos.x += MovableV.x;
		//RenderBlock::cameraPos.y += MovableV.y;
		//RenderBlock::cameraPos.z += MovableV.z;
		if (footd != nullptr)
		{
			if (*footd == 0)
			{
				RenderBlock::Velocity.y += -0.02;
				RenderBlock::OnGround = 0;
			}
			else
			{
				if(RenderBlock::Velocity.y < 0)
					RenderBlock::Velocity.y = 0;
				RenderBlock::OnGround = 1;
			}
		}

		RenderBlock::Velocity.x *= 0.8;
		RenderBlock::Velocity.z *= 0.8;
		//glfwSetCursorPosCallback(RenderBlock::window, );
		
		Sleep(10);
	}
}

bool UserAction::IsStuck(glm::vec3 pos)
{
	for(short i = 0; i < 2; ++i)
	for(short j = 0; j < 2; ++j)
	for(short k = 0; k < 2; ++k)
	{
		auto p = pos + glm::vec3(
			(i ? 0.3f : -0.3f),
			(j ? -0.2f : -1.2f),
			(k ? 0.3f : -0.3f)
		);
		auto m = GenMain::WorldBlock(p.x, p.y, p.z);
		if (m == nullptr || *m != 0)
			return true;
	}
	return false;
}