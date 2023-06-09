﻿#include "RenderBlockProcess.h"
#include "BasicClass/lib/Texture.h"
#include "../../Generation/Entry.h"
#include <GLFW/glfw3.h>
#include "../UserAction.h"
#pragma omp parallel for

bool				RenderBlock::shouldUpdate = 0;
unsigned			RenderBlock::offset = 0, RenderBlock::offset1 = 0, RenderBlock::offsetm[10];
RenderBlock::UBasic RenderBlock::wh[1024 * 256 * 4 * 8 * 4 ];
RenderBlock::UBasic RenderBlock::wh1[1024 * 256 * 4 * 8 * 4 ];
RenderBlock::UBasic RenderBlock::whm[10][1024 * 256]; //
unsigned			RenderBlock::RendererN = 0;
unsigned			RenderBlock::RendererN1 = 0;
unsigned			RenderBlock::RendererNm[10] = {}; //
unsigned			RenderBlock::indices[65537 * 4 * 4 * 8 * 4 ];
float				RenderBlock::GHeight, RenderBlock::GWidth;
int					RenderBlock::WinWidth = 2560, RenderBlock::WinHeight = 1440;
glm::vec3			RenderBlock::cameraPos = glm::vec3(0.0f, 100.0f, 0.0f);
bool				RenderBlock::ProgramEnd = 0;
glm::vec3			RenderBlock::camFront;
glm::vec3			RenderBlock::camUp;
GLFWwindow*			RenderBlock::window = nullptr;
float				RenderBlock::lstFrame;
float				RenderBlock::currentFrame;
glm::vec3			RenderBlock::Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
bool				RenderBlock::OnGround = 0;
bool				RenderBlock::ChunkShouldUpdate = 0;
bool				RenderBlock::LockCursor = 1;
bool				RenderBlock::LastPressE = 0;
unsigned			RenderBlock::SelectedBlock = 1U;
double				RenderBlock::MouseX, RenderBlock::MouseY;
bool				RenderBlock::IsInWater = 0;


glm::vec3 RenderBlock::PlayerLookAt(glm::vec3* surf)
{
	glm::vec3 w = RenderBlock::cameraPos;
	//w.x += 1.0f;
	//w.z += 1.0f;
	glm::vec3 lst = glm::vec3(NAN, NAN, NAN);

	int p = 500;
	while (p--)
	{
		if (w == lst) {
			//w += RenderBlock::camFront * 0.02f;
			w.x += RenderBlock::camFront.x * 0.02f;
			w.y += RenderBlock::camFront.y * 0.02f;
			w.z += RenderBlock::camFront.z * 0.02f;
			continue;
		}
		auto tmp = GenMain::WorldBlock(w.x, w.y, w.z);
		//std::cout << "Try to locate block (" << w.x << ", " << w.y << ", " << w.z << ")\n";

		if (tmp == nullptr)
			return glm::vec3(NAN, NAN, NAN);
		if (*tmp != 0)
		{
			if (surf != nullptr)
			{
				*surf = lst;
			}
			return glm::vec3(w.x, w.y, w.z);
		}
		lst = w;
		w.x += RenderBlock::camFront.x * 0.02f;
		w.y += RenderBlock::camFront.y * 0.02f;
		w.z += RenderBlock::camFront.z * 0.02f;
	}
	return glm::vec3(NAN, NAN, NAN);

}

void RenderBlock::ProcessInput(float deltatime)
{
	if (glfwGetKey(RenderBlock::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(RenderBlock::window, true);
	
	float cameraSpeed = (RenderBlock::IsInWater ? 1.0f : 3.0f) * deltatime;
	glm::vec3 ktot(0.0f);

	if (glfwGetKey(RenderBlock::window, GLFW_KEY_W) == GLFW_PRESS)
	{
		auto k = RenderBlock::camFront;
		k.y = 0;
		float tot = std::sqrtf(k.x * k.x + k.z * k.z);
		k.x /= tot;
		k.z /= tot;
		ktot += k;
	}
	if (glfwGetKey(RenderBlock::window, GLFW_KEY_S) == GLFW_PRESS)
	{
		auto k = RenderBlock::camFront;
		k.y = 0;
		float tot = std::sqrtf(k.x * k.x + k.z * k.z);
		k.x /= tot;
		k.z /= tot;
		ktot -= k;
	}
	if (glfwGetKey(RenderBlock::window, GLFW_KEY_A) == GLFW_PRESS)
	{
		auto k = glm::normalize(glm::cross(RenderBlock::camFront, RenderBlock::camUp));
		k.y = 0;
		float tot = std::sqrtf(k.x * k.x + k.z * k.z);
		k.x /= tot;
		k.z /= tot;
		ktot -= k;
	}
	if (glfwGetKey(RenderBlock::window, GLFW_KEY_D) == GLFW_PRESS)
	{
		auto k = glm::normalize(glm::cross(RenderBlock::camFront, RenderBlock::camUp));
		k.y = 0;
		float tot = std::sqrtf(k.x * k.x + k.z * k.z);
		k.x /= tot;
		k.z /= tot;
		ktot += k;
	}
	if (glfwGetKey(RenderBlock::window, GLFW_KEY_E) == GLFW_PRESS)
	{
		if(LastPressE == 0)
		RenderBlock::LockCursor = 1 - RenderBlock::LockCursor;
		LastPressE = 1;
	}
	else LastPressE = 0;
	float tot = std::sqrtf(ktot.x * ktot.x + ktot.z * ktot.z);
	if (tot > 0.0001f)
	{
		ktot.x /= tot;
		ktot.z /= tot;
		RenderBlock::Velocity += ktot * cameraSpeed;
	}
	if (glfwGetKey(RenderBlock::window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (RenderBlock::IsInWater) {
			RenderBlock::Velocity.y = max(RenderBlock::Velocity.y + 0.4 * deltatime, 0.02f);
		}
		else if (RenderBlock::OnGround)
		{
			RenderBlock::Velocity.y = 0.15f;
		}

	}
	return;
}
#include "../ImportInfo.h"
#include "../BlockUpdate.h"
void RenderBlockProcess()
{
	Renderer::ActivateImgui = 1;
	
	for (int i = 0, k = 0; i < 65537 * 4 * 4 * 8 * 4 - 6; i += 6, k += 4)
	{
		RenderBlock::indices[i]		= k;
		RenderBlock::indices[i + 1]	= k + 1;
		RenderBlock::indices[i + 2]	= k + 2;
		RenderBlock::indices[i + 3]	= k + 1;
		RenderBlock::indices[i + 4]	= k + 2;
		RenderBlock::indices[i + 5]	= k + 3;
	}

	RenderBlock::window		= Renderer::NewWindow(RenderBlock::WinWidth, RenderBlock::WinHeight);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	auto projection	= glm::perspective(glm::radians(30.0f), RenderBlock::WinWidth / float(RenderBlock::WinHeight), 0.1f, 10000.0f);

	VertexArray va;
	VertexBuffer vb(nullptr, 65536 * 128 * 4 * 8 * 4 , 0);

	
	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(2);
	layout.push<float>(1);
	va.AddBuffer(vb, layout);
	IndexBuffer ib(RenderBlock::indices, 65536 * 8 * 2 * 4 * 4 );

	Shader shader("UCLient/Basic.shader");
	shader.Bind();

	
	Texture texture("Resources/index.png");
	texture.Bind();

	RenderBlock::GHeight = texture.GetHeight();
	RenderBlock::GWidth = texture.GetWidth();

	float h = 30.0f;

	RenderBlock::cameraPos = glm::vec3(2.0f, 8.0f, 2.0f);
	RenderBlock::camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	RenderBlock::camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float EyeAngle[2] = { -2.0f, -75.0f };
	float rg = -90.0f;
	RenderBlock::lstFrame = glfwGetTime();

	glClearColor(161 / 255.0f, 219 / 255.0f, 255 / 255.0f, 1.0f);
	
	double dMouseX, dMouseY;
	bool MouseButtonDown = 0;
	bool MouseRightButton = 0;
	bool LastCursorLock = 1;
	float DeltaTime;
	ShowCursor(false);
	while (!glfwWindowShouldClose(RenderBlock::window))
	{
		glfwGetCursorPos(RenderBlock::window, &RenderBlock::MouseX, &RenderBlock::MouseY);
		if (LastCursorLock)
		{
			dMouseX = RenderBlock::MouseX - RenderBlock::WinWidth / 2;
			dMouseY = RenderBlock::MouseY - RenderBlock::WinHeight / 2;
		}
		else
		{
			dMouseX = dMouseY = 0;
		}

		if (RenderBlock::LockCursor)
		{
			glfwSetCursorPos(RenderBlock::window, RenderBlock::WinWidth / 2, RenderBlock::WinHeight / 2);
			if(!LastCursorLock)
				ShowCursor(false);
		}
		else
		{
			if(LastCursorLock)
				ShowCursor(true);
		}
		LastCursorLock = RenderBlock::LockCursor;
		
		
		RenderBlock::currentFrame = glfwGetTime();
		DeltaTime = RenderBlock::currentFrame - RenderBlock::lstFrame;

		glfwGetWindowSize(RenderBlock::window, &RenderBlock::WinWidth, &RenderBlock::WinHeight);
		glViewport(0, 0, RenderBlock::WinWidth, RenderBlock::WinHeight);

		RenderBlock::IsInWater = UserAction::IsInWater(RenderBlock::cameraPos);

		if (RenderBlock::IsInWater) UserAction::WaterPush(DeltaTime);

		RenderBlock::ProcessInput(DeltaTime);
		if (glfwGetMouseButton(RenderBlock::window, GLFW_MOUSE_BUTTON_LEFT) && RenderBlock::LockCursor)
		{
			if (!MouseButtonDown)
			{
				auto look = RenderBlock::PlayerLookAt();
				if (!isnan(look.x))
				{
					*GenMain::WorldBlock(look.x, look.y, look.z) = 0;
					RenderBlock::ChunkShouldUpdate = 1;

					for(int i = -2; i <= 2; ++i)
					for(int j = -2; j <= 2; ++j)
					for(int k = -2; k <= 2; ++k)
						UserAction::RegisterUpdate((look.x + i), (look.y + j), (look.z + k));
				}
			}
			MouseButtonDown = 1;
		}
		else
		{
			MouseButtonDown = 0;
		}

		if (glfwGetMouseButton(RenderBlock::window, GLFW_MOUSE_BUTTON_RIGHT) && RenderBlock::LockCursor)
		{
			if (!MouseRightButton)
			{
				glm::vec3 surf;
				auto look = RenderBlock::PlayerLookAt(&surf);
				auto p = RenderBlock::cameraPos;
				if (!isnan(look.x))
				{
					auto r = GenMain::WorldBlock(surf.x, surf.y, surf.z);

					if (r != nullptr)
					{
						if (RenderBlock::SelectedBlock == 32)
						{
							auto u = GenMain::WorldNbt(surf.x, surf.y, surf.z);
							if (u != nullptr)
								*u |= 0xe0000000;
						}
						else
						{
							*r = RenderBlock::SelectedBlock;
							if (UserAction::IsStuck(p))
								*r = 0;
							if (ImportInfo::IsSpecialModel[RenderBlock::SelectedBlock] == 2)
							{
								if (ImportInfo::nbtinfo[RenderBlock::SelectedBlock].nbttype == 1)
								{
									if (FloatEqual(surf.x, look.x - 1) && FloatEqual(surf.y, look.y) && FloatEqual(surf.z, look.z)) { *(GenMain::WorldNbt(surf.x, surf.y, surf.z)) = 0; }
									if (FloatEqual(surf.x, look.x) && FloatEqual(surf.y, look.y - 1) && FloatEqual(surf.z, look.z)) { *(GenMain::WorldNbt(surf.x, surf.y, surf.z)) = 1; }
									if (FloatEqual(surf.x, look.x) && FloatEqual(surf.y, look.y) && FloatEqual(surf.z, look.z - 1)) { *(GenMain::WorldNbt(surf.x, surf.y, surf.z)) = 2; }
									if (FloatEqual(surf.x, look.x + 1) && FloatEqual(surf.y, look.y) && FloatEqual(surf.z, look.z)) { *(GenMain::WorldNbt(surf.x, surf.y, surf.z)) = 3; }
									if (FloatEqual(surf.x, look.x) && FloatEqual(surf.y, look.y + 1) && FloatEqual(surf.z, look.z)) { *(GenMain::WorldNbt(surf.x, surf.y, surf.z)) = 4; }
									if (FloatEqual(surf.x, look.x) && FloatEqual(surf.y, look.y) && FloatEqual(surf.z, look.z + 1)) { *(GenMain::WorldNbt(surf.x, surf.y, surf.z)) = 5; }

									std::cout << "nbt = " << *(GenMain::WorldNbt(surf.x, surf.y, surf.z)) << '\n';
									std::cout << "look = " << look.x << ' ' << look.y << ' ' << look.z << '\n';
									std::cout << "surf = " << surf.x << ' ' << surf.y << ' ' << surf.z << '\n';
								}
							}
						}
						RenderBlock::ChunkShouldUpdate = 1;
					}

					for(int i = -2; i <= 2; ++i)
					for(int j = -2; j <= 2; ++j)
					for(int k = -2; k <= 2; ++k)
						UserAction::RegisterUpdate(surf.x + i, surf.y + j, surf.z + k);
				}
			}
			MouseRightButton = 1;

		}
		else
		{
			MouseRightButton = 0;
		}

		RenderBlock::lstFrame = RenderBlock::currentFrame;
		
		auto look = glm::perspective(glm::radians(45.0f), RenderBlock::WinWidth / float(RenderBlock::WinHeight), 0.1f, 300.0f);
		auto model = glm::rotate(glm::mat4(1.0), glm::radians(rg), glm::vec3(1.0f, 0.0f, 0.0f));

		glClear(GL_DEPTH_BUFFER_BIT);
		shader.SetUniformMat4f("proj", look);
		
		RenderBlock::camFront.x = cos(glm::radians(EyeAngle[0])) * cos(glm::radians(EyeAngle[1]));
		RenderBlock::camFront.y = sin(glm::radians(EyeAngle[0]));
		RenderBlock::camFront.z = cos(glm::radians(EyeAngle[0])) * sin(glm::radians(EyeAngle[1]));


		auto view = glm::lookAt(RenderBlock::cameraPos, RenderBlock::cameraPos + RenderBlock::camFront, RenderBlock::camUp);
		shader.SetUniformMat4f("view", view);
		shader.SetUniformMat4f("model", model);
		

		vb.Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, min(RenderBlock::offset1 * sizeof(RenderBlock::UBasic), sizeof(RenderBlock::wh1)), RenderBlock::wh1);

		projection = glm::perspective(glm::radians(h), RenderBlock::WinWidth / float(RenderBlock::WinHeight), 0.1f, 100.0f);
		Renderer::Clear();
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer::Draw(va, ib, shader, RenderBlock::RendererN1 * 6);


		EyeAngle[0] -= dMouseY * 0.12f;
		EyeAngle[1] += dMouseX * 0.12f;

		EyeAngle[0] = max(-89.5f, EyeAngle[0]);
		EyeAngle[0] = min( 89.5f, EyeAngle[0]);

		RenderBlock::Velocity.y += (RenderBlock::IsInWater ? -0.3f : - 0.7f) * DeltaTime;
		float df = std::powf(0.00005f, DeltaTime);
		RenderBlock::Velocity.x *= df;
		RenderBlock::Velocity.z *= df;

		Renderer::Refresh(RenderBlock::window);
	}

	Renderer::Close(RenderBlock::window);
	
	SaveAll();

	std::cout << "Saved!\n";
	RenderBlock::ProgramEnd = 1;
	exit(0);

	system("pause");
	
}

#include "../ImportInfo.h"
#include "../Graph/SmoothLight.h"
#define __SP_DRAW__(_WH) \
for (auto& m : ImportInfo::spbinfo[id][_WH])																											\
{																																						\
	RenderBlock::UBasic w[] = {																															\
	{x + m.PosStartX,	y + m.PosStartY,	z + m.PosStartZ,	m.TexStartX / RenderBlock::GWidth,	m.TexStartY / RenderBlock::GHeight, mxsurlit},		\
	{x + m.PosStartX + m.PosEndX - m.PosMidX,   y + m.PosStartY + m.PosEndY - m.PosMidY,	z + m.PosStartZ + m.PosEndZ - m.PosMidZ,					\
																m.TexEndX / RenderBlock::GWidth,  m.TexStartY / RenderBlock::GHeight, mxsurlit},		\
	{x + m.PosMidX,		y + m.PosMidY,		z + m.PosMidZ,		m.TexStartX / RenderBlock::GWidth,  m.TexEndY / RenderBlock::GHeight, mxsurlit},		\
	{x + m.PosEndX,		y + m.PosEndY,		z + m.PosEndZ,		m.TexEndX / RenderBlock::GWidth,  m.TexEndY / RenderBlock::GHeight, mxsurlit} };		\
																																						\
	if (m.layer + layout)																																\
	{																																					\
		memcpy(RenderBlock::whm[m.layer + layout] + RenderBlock::offsetm[m.layer + layout], w, sizeof(w));												\
		++RenderBlock::RendererNm[m.layer + layout];																									\
		RenderBlock::offsetm[m.layer + layout] += 4;																									\
	}																																					\
	else																																				\
	{																																					\
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));																					\
		++RenderBlock::RendererN;																														\
		RenderBlock::offset += 4;																														\
	}																																					\
}

void RenderBlock::RegisterBlock(float x, float y, float z, unsigned short sur, int id, unsigned layout, Generation::WorldUnit* unit, unsigned int nbt)
{
	float surlit[6], mxsurlit = 0.0f;
	memset(surlit, 0, sizeof(surlit));

	for (unsigned k = 0; k < 6; ++k)
	{
		float _i = x;
		float _j = y;
		float _k = z;
		switch (k)
		{
		case 0:
			_i -= 1;
			break;
		case 1:
			_j -= 1;
			break;
		case 2:
			_k -= 1;
			break;
		case 3:
			_i += 1;
			break;
		case 4:
			_j += 1;
			break;
		case 5:
			_k += 1;
			break;
		}
		auto p = GenMain::WorldLit(_i, _j, _k);
		if (p == nullptr)	surlit[k] = 0.5f;
		else				surlit[k] = *p;
		mxsurlit = max(mxsurlit, surlit[k]);
	}
	
	if (ImportInfo::IsSpecialModel[id] == 1) {
		if (id == 32 || id == 555) {
			unsigned p[3][3], maxp = 0U;
			unsigned l[2][2];
			int mi, mj;
			for(int i = 0; i < 3; ++i)
			for(int j = 0; j < 3; ++j){
				auto o = GenMain::WorldNbt(x + i - 1, z, -(y + j - 1));
				if (o == nullptr)	p[i][j] = 0;
				else				p[i][j] = *o & 0xe0000000U;
				if (maxp < p[i][j]) {
					maxp = p[i][j];
					mi = i; mj = j;
				}
			}
			if (maxp == p[1][1]) l[0][0] = l[0][1] = l[1][0] = l[1][1] = maxp;
			else if (mi != 1 && mj != 1) {
				l[mi / 2][mj / 2]			= maxp;
				l[1 - mi / 2][mj / 2]		= max(p[1][1], max(p[2 - mi][mj], p[1][mj]));
				l[mi / 2][1 - mj / 2]		= max(p[1][1], max(p[mi][2 - mj], p[mi][1]));
				l[1 - mi / 2][1 - mj / 2]	= p[1][1];
			}
			else {
				if (mi == 1) {
					l[0][mj / 2]		= l[1][mj / 2]		= maxp;
					l[0][1 - mj / 2]	= l[1][1 - mj / 2]	= p[1][1];
				}
				else {
					l[mi / 2][0]		= l[mi / 2][1]		= maxp;
					l[1 - mi / 2][0]	= l[1 - mi / 2][1]	= p[1][1];
				}
			}
			float h[2][2];
			for(int i = 0; i < 2; ++i)
			for(int j = 0; j < 2; ++j){
				if (l[i][j] == 0xe0000000U && id == 555) h[i][j] = 1.0f;
				else h[i][j] = l[i][j] / float(0x100000000L);
			}

			{
				RenderBlock::UBasic w[] = {
				{x,     y    , z + h[0][0],  80 / RenderBlock::GWidth, 32 / RenderBlock::GHeight, mxsurlit},
				{x,     y + 1, z + h[0][1], 80 / RenderBlock::GWidth, 48 / RenderBlock::GHeight, mxsurlit},
				{x + 1, y,     z + h[1][0],  96 / RenderBlock::GWidth, 32 / RenderBlock::GHeight, mxsurlit},
				{x + 1, y + 1, z + h[1][1],  96 / RenderBlock::GWidth, 48 / RenderBlock::GHeight, mxsurlit} };
				if (layout)
				{
					memcpy(RenderBlock::whm[layout] + RenderBlock::offsetm[layout], w, sizeof(w));
					++RenderBlock::RendererNm[layout];
					RenderBlock::offsetm[layout] += 4;
				}
				else
				{
					memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
					++RenderBlock::RendererN;
					RenderBlock::offset += 4;
				}
			}
			if (sur & 0b100000) {
				RenderBlock::UBasic w[] = {
				{x,     y    , z,  80 / RenderBlock::GWidth, 32 / RenderBlock::GHeight, mxsurlit},
				{x,     y,  z + h[0][0],  80 / RenderBlock::GWidth, 48 / RenderBlock::GHeight, mxsurlit},
				{x, y + 1,     z,  96 / RenderBlock::GWidth, 32 / RenderBlock::GHeight, mxsurlit},
				{x, y + 1, z + h[0][1],  96 / RenderBlock::GWidth, 48 / RenderBlock::GHeight, mxsurlit} };
				if (layout)
				{
					memcpy(RenderBlock::whm[layout] + RenderBlock::offsetm[layout], w, sizeof(w));
					++RenderBlock::RendererNm[layout];
					RenderBlock::offsetm[layout] += 4;
				}
				else
				{
					memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
					++RenderBlock::RendererN;
					RenderBlock::offset += 4;
				}
			}
			if (sur & 0b010000) {
				RenderBlock::UBasic w[] = {
				{x,     y    , z,  80 / RenderBlock::GWidth, 32 / RenderBlock::GHeight, mxsurlit},
				{x,     y,  z + h[0][0], 80 / RenderBlock::GWidth, 48 / RenderBlock::GHeight, mxsurlit},
				{x + 1, y,     z,  96 / RenderBlock::GWidth, 32 / RenderBlock::GHeight, mxsurlit},
				{x + 1, y, z + h[1][0],  96 / RenderBlock::GWidth, 48 / RenderBlock::GHeight, mxsurlit}};
				if (layout)
				{
					memcpy(RenderBlock::whm[layout] + RenderBlock::offsetm[layout], w, sizeof(w));
					++RenderBlock::RendererNm[layout];
					RenderBlock::offsetm[layout] += 4;
				}
				else
				{
					memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
					++RenderBlock::RendererN;
					RenderBlock::offset += 4;
				}
			}
			if (sur & 0b000100) {
				RenderBlock::UBasic w[] = {
				{x + 1,     y    , z,  80 / RenderBlock::GWidth, 32 / RenderBlock::GHeight, mxsurlit},
				{x + 1,     y,  z + h[1][0],  80 / RenderBlock::GWidth, 48 / RenderBlock::GHeight, mxsurlit},
				{x + 1, y + 1,     z,  96 / RenderBlock::GWidth, 32 / RenderBlock::GHeight, mxsurlit},
				{x + 1, y + 1, z + h[1][1],  96 / RenderBlock::GWidth, 48 / RenderBlock::GHeight, mxsurlit} };
				if (layout)
				{
					memcpy(RenderBlock::whm[layout] + RenderBlock::offsetm[layout], w, sizeof(w));
					++RenderBlock::RendererNm[layout];
					RenderBlock::offsetm[layout] += 4;
				}
				else
				{
					memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
					++RenderBlock::RendererN;
					RenderBlock::offset += 4;
				}
			}
			if (sur & 0b000010) {
				RenderBlock::UBasic w[] = {
				{x,     y + 1    , z,  80 / RenderBlock::GWidth, 32 / RenderBlock::GHeight, mxsurlit},
				{x,     y + 1,  z + h[0][1], 80 / RenderBlock::GWidth, 48 / RenderBlock::GHeight, mxsurlit},
				{x + 1, y + 1,     z,  96 / RenderBlock::GWidth, 32 / RenderBlock::GHeight, mxsurlit},
				{x + 1, y + 1, z + h[1][1],  96 / RenderBlock::GWidth, 48 / RenderBlock::GHeight, mxsurlit} };
				if (layout)
				{
					memcpy(RenderBlock::whm[layout] + RenderBlock::offsetm[layout], w, sizeof(w));
					++RenderBlock::RendererNm[layout];
					RenderBlock::offsetm[layout] += 4;
				}
				else
				{
					memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
					++RenderBlock::RendererN;
					RenderBlock::offset += 4;
				}
			}
		}
		else {
			__SP_DRAW__(6);
			if (sur & 0b100000) __SP_DRAW__(0);
			if (sur & 0b010000) __SP_DRAW__(1);
			if (sur & 0b001000) __SP_DRAW__(2);
			if (sur & 0b000100) __SP_DRAW__(3);
			if (sur & 0b000010) __SP_DRAW__(4);
			if (sur & 0b000001) __SP_DRAW__(5);
		}
	}
	else if (ImportInfo::IsSpecialModel[id] == 2)
	{
		RenderBlock::RegisterBlock(x, y, z, sur, ImportInfo::nbtinfo[id].go[nbt & 0x7fffffff], layout, unit, 0U);
	}

	else
	{
		if (ImportInfo::binfo.find(id) == ImportInfo::binfo.end())
		{
			std::cout << "Texture of Block id = " << id << " Cannot find.\n";
			id = 1;
		}

		if (sur & 0b010000)
		{
			auto lit = ULight::GenLight(x, y, z, unit, 0b010000);

			RenderBlock::UBasic w[] = {
			{x,     y    , z,  ImportInfo::binfo[id][0 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0] - 1.0f + surlit[1]},
			{x,     y,  z + 1, ImportInfo::binfo[id][0 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 3] / RenderBlock::GHeight, lit.l[1][0] - 1.0f + surlit[1]},
			{x + 1, y,     z,  ImportInfo::binfo[id][0 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 1] / RenderBlock::GHeight, lit.l[0][1] - 1.0f + surlit[1]},
			{x + 1, y, z + 1,  ImportInfo::binfo[id][0 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1] - 1.0f + surlit[1]} };
			if (layout)
			{
				memcpy(RenderBlock::whm[layout] + RenderBlock::offsetm[layout], w, sizeof(w));
				++RenderBlock::RendererNm[layout];
				RenderBlock::offsetm[layout] += 4;
			}
			else
			{
				memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
				++RenderBlock::RendererN;
				RenderBlock::offset += 4;
			}
		}

		if (sur & 0b001000)
		{
			auto lit = ULight::GenLight(x, y, z, unit, 0b001000);

			RenderBlock::UBasic w[] = {
			{x,     y    , z,  ImportInfo::binfo[id][1 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0] - 1.0f + surlit[2]},
			{x,     y + 1,  z, ImportInfo::binfo[id][1 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 3] / RenderBlock::GHeight, lit.l[0][1] - 1.0f + surlit[2]},
			{x + 1, y,     z,  ImportInfo::binfo[id][1 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 1] / RenderBlock::GHeight, lit.l[1][0] - 1.0f + surlit[2]},
			{x + 1, y + 1, z,  ImportInfo::binfo[id][1 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1] - 1.0f + surlit[2]} };
			if (layout)
			{
				memcpy(RenderBlock::whm[layout] + RenderBlock::offsetm[layout], w, sizeof(w));
				++RenderBlock::RendererNm[layout];
				RenderBlock::offsetm[layout] += 4;
			}
			else
			{
				memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
				++RenderBlock::RendererN;
				RenderBlock::offset += 4;
			}
		}

		if (sur & 0b100000)
		{
			auto lit = ULight::GenLight(x, y, z, unit, 0b100000);

			RenderBlock::UBasic w[] = {
			{x,     y    , z,   ImportInfo::binfo[id][2 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0] - 1.0f + surlit[0]},
			{ x,     y,  z + 1, ImportInfo::binfo[id][2 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 3] / RenderBlock::GHeight, lit.l[1][0] - 1.0f + surlit[0]},
			{ x, y + 1,     z,  ImportInfo::binfo[id][2 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 1] / RenderBlock::GHeight, lit.l[0][1] - 1.0f + surlit[0]},
			{ x, y + 1, z + 1,  ImportInfo::binfo[id][2 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1] - 1.0f + surlit[0]} };
			if (layout)
			{
				memcpy(RenderBlock::whm[layout] + RenderBlock::offsetm[layout], w, sizeof(w));
				++RenderBlock::RendererNm[layout];
				RenderBlock::offsetm[layout] += 4;
			}
			else
			{
				memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
				++RenderBlock::RendererN;
				RenderBlock::offset += 4;
			}
		}

		if (sur & 0b000010)
		{
			auto lit = ULight::GenLight(x, y, z, unit, 0b000010);
			RenderBlock::UBasic w[] = {
			{x, y + 1, z,           ImportInfo::binfo[id][3 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0] - 1.0f + surlit[4]},
			{ x,     y + 1,  z + 1, ImportInfo::binfo[id][3 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 3] / RenderBlock::GHeight, lit.l[1][0] - 1.0f + surlit[4] },
			{ x + 1, y + 1,     z,  ImportInfo::binfo[id][3 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 1] / RenderBlock::GHeight, lit.l[0][1] - 1.0f + surlit[4] },
			{ x + 1, y + 1, z + 1,  ImportInfo::binfo[id][3 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1] - 1.0f + surlit[4] } };
			if (layout)
			{
				memcpy(RenderBlock::whm[layout] + RenderBlock::offsetm[layout], w, sizeof(w));
				++RenderBlock::RendererNm[layout];
				RenderBlock::offsetm[layout] += 4;
			}
			else
			{
				memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
				++RenderBlock::RendererN;
				RenderBlock::offset += 4;
			}
		}

		if (sur & 0b000001)
		{
			auto lit = ULight::GenLight(x, y, z, unit, 0b000001);
			RenderBlock::UBasic w[] = {
			{x,     y    , z + 1, ImportInfo::binfo[id][4 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0] - 1.0f + surlit[5]},
			{x,     y + 1, z + 1, ImportInfo::binfo[id][4 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 3] / RenderBlock::GHeight, lit.l[0][1] - 1.0f + surlit[5]},
			{x + 1, y,     z + 1, ImportInfo::binfo[id][4 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 1] / RenderBlock::GHeight, lit.l[1][0] - 1.0f + surlit[5]},
			{x + 1, y + 1, z + 1, ImportInfo::binfo[id][4 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1] - 1.0f + surlit[5]} };
			if (layout)
			{
				memcpy(RenderBlock::whm[layout] + RenderBlock::offsetm[layout], w, sizeof(w));
				++RenderBlock::RendererNm[layout];
				RenderBlock::offsetm[layout] += 4;
			}
			else
			{
				memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
				++RenderBlock::RendererN;
				RenderBlock::offset += 4;
			}
		}

		if (sur & 0b000100)
		{
			auto lit = ULight::GenLight(x, y, z, unit, 0b000100);
			RenderBlock::UBasic w[] = {
			{x + 1,     y    , z,  ImportInfo::binfo[id][5 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0] - 1.0f + surlit[3]},
			{x + 1,     y,  z + 1, ImportInfo::binfo[id][5 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 3] / RenderBlock::GHeight, lit.l[1][0] - 1.0f + surlit[3]},
			{x + 1, y + 1,     z,  ImportInfo::binfo[id][5 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 1] / RenderBlock::GHeight, lit.l[0][1] - 1.0f + surlit[3]},
			{x + 1, y + 1, z + 1,  ImportInfo::binfo[id][5 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1] - 1.0f + surlit[3]} };
			if (layout)
			{
				memcpy(RenderBlock::whm[layout] + RenderBlock::offsetm[layout], w, sizeof(w));
				++RenderBlock::RendererNm[layout];
				RenderBlock::offsetm[layout] += 4;
			}
			else
			{
				memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
				++RenderBlock::RendererN;
				RenderBlock::offset += 4;
			}
		}
	}
}

void RenderBlock::RegisterGUI(float stx, float sty, float endx, float endy, float wstx, float wsty, float wendx, float wendy, unsigned lay)
{
	RenderBlock::UBasic w[] = {
	{ stx,  sty,  -100, wstx  / RenderBlock::GWidth, wsty  / RenderBlock::GHeight, 1.0f},
	{ endx, sty,  -100, wendx / RenderBlock::GWidth, wsty  / RenderBlock::GHeight, 1.0f},
	{ stx,  endy, -100, wstx  / RenderBlock::GWidth, wendy / RenderBlock::GHeight, 1.0f},
	{ endx, endy, -100, wendx / RenderBlock::GWidth, wendy / RenderBlock::GHeight, 1.0f} };

	memcpy(RenderBlock::whm[lay] + RenderBlock::offsetm[lay], w, sizeof(w));
	++RenderBlock::RendererNm[lay];
	RenderBlock::offsetm[lay] += 4;
}

void RenderBlock::ClearAll()
{
	RenderBlock::RendererN = 0;
	RenderBlock::offset = 0;
	memset(RenderBlock::RendererNm, 0, sizeof(RenderBlock::RendererNm));
	memset(RenderBlock::offsetm, 0, sizeof(RenderBlock::offsetm));
}
