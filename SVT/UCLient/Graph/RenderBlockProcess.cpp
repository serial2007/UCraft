#include "RenderBlockProcess.h"
#include "BasicClass/lib/Texture.h"
#include "../../Generation/Entry.h"
#include <GLFW/glfw3.h>
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
glm::vec3			RenderBlock::cameraPos = glm::vec3(0.0f, 10.0f, 0.0f);
bool				RenderBlock::ProgramEnd = 0;
glm::vec3			RenderBlock::camFront;
glm::vec3			RenderBlock::camUp;
bool				RenderBlock::RegisterDone = 1;
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


glm::vec3 RenderBlock::PlayerLookAt(glm::vec3* surf)
{
	glm::vec3 w = RenderBlock::cameraPos;
	w.z += 1.0f;
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
		auto tmp = GenMain::WorldBlock(floorf(w.x), floorf(w.y), floorf(w.z));
		//std::cout << "Try to locate block (" << w.x << ", " << w.y << ", " << w.z << ")\n";

		if (tmp == nullptr)
			return glm::vec3(NAN, NAN, NAN);
		if (*tmp != 0)
		{
			if (surf != nullptr)
			{
				*surf = lst;
			}
			return glm::vec3(floorf(w.x), floorf(w.y), floorf(w.z));
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
	//std::cout << "inp";
	if (glfwGetKey(RenderBlock::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(RenderBlock::window, true);
	float cameraSpeed = 3.0f * deltatime;
	glm::vec3 ktot(0.0f);

	if (glfwGetKey(RenderBlock::window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//while (1) std::cout << '_';
		auto k = RenderBlock::camFront;     //按下W，摄像机向前移动
		k.y = 0;
		float tot = std::sqrtf(k.x * k.x + k.z * k.z);
		k.x /= tot;
		k.z /= tot;
		ktot += k;
		//RenderBlock::Velocity += k * cameraSpeed;
		
	}
	if (glfwGetKey(RenderBlock::window, GLFW_KEY_S) == GLFW_PRESS)
	{
		auto k = RenderBlock::camFront;     //按下W，摄像机向前移动
		k.y = 0;
		float tot = std::sqrtf(k.x * k.x + k.z * k.z);
		k.x /= tot;
		k.z /= tot;
		ktot -= k;
		//RenderBlock::Velocity -= k * cameraSpeed;
	}
	if (glfwGetKey(RenderBlock::window, GLFW_KEY_A) == GLFW_PRESS)
		//glm::cross(cameraFront, cameraUp)得出的是摄像机的右方向
	{
		auto k = glm::normalize(glm::cross(RenderBlock::camFront, RenderBlock::camUp));  //按下A，摄像机向左移动
		k.y = 0;
		float tot = std::sqrtf(k.x * k.x + k.z * k.z);
		k.x /= tot;
		k.z /= tot;
		ktot -= k;
		//RenderBlock::Velocity -= k * cameraSpeed;
	}
	if (glfwGetKey(RenderBlock::window, GLFW_KEY_D) == GLFW_PRESS)
	{
		auto k = glm::normalize(glm::cross(RenderBlock::camFront, RenderBlock::camUp));  //按下D，摄像机向右移动
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
	/*if (glfwGetKey(RenderBlock::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		RenderBlock::cameraPos.y -= cameraSpeed;

	}*/
	float tot = std::sqrtf(ktot.x * ktot.x + ktot.z * ktot.z);
	if (tot > 0.0001f)
	{
		ktot.x /= tot;
		ktot.z /= tot;
		RenderBlock::Velocity += ktot * cameraSpeed;
	}
	if (glfwGetKey(RenderBlock::window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (RenderBlock::OnGround)
		{
			//while (1)
			RenderBlock::Velocity.y = 0.25f;
		}

	}
	return;
}

void RenderBlockProcess()
{
	Renderer::ActivateImgui = 1;
	//glDisable(GLFW_MOUSE_CURSOR);
	
	for (int i = 0, k = 0; i < 65537 * 4 * 4 * 8 * 4 - 6; i += 6, k += 4)
	{
		RenderBlock::indices[i]		= k;
		RenderBlock::indices[i + 1]	= k + 1;
		RenderBlock::indices[i + 2]	= k + 2;
		RenderBlock::indices[i + 3]	= k + 1;
		RenderBlock::indices[i + 4]	= k + 2;
		RenderBlock::indices[i + 5]	= k + 3;
	}

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	RenderBlock::window		= Renderer::NewWindow(RenderBlock::WinWidth, RenderBlock::WinHeight);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	auto projection	= glm::perspective(glm::radians(30.0f), RenderBlock::WinWidth / float(RenderBlock::WinHeight), 0.1f, 10000.0f);
	//auto view		= glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -3.0f));
	
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	//auto view = glm::lookAt(glm::vec3(0.0f, -10.0f, 3.0f),   //摄像机的位置
	//	glm::vec3(0.0f, 0.0f, 0.0f),           //摄像机看向的顶点坐标，这里朝向原点
	//	glm::vec3(0.0f, 1.0f, 0.0f));

	RenderBlock::cameraPos = glm::vec3(2.0f, 8.0f, 2.0f);
	RenderBlock::camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	RenderBlock::camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float EyeAngle[2] = { -2.0f, -75.0f };
	float rg = -90.0f;
	RenderBlock::lstFrame = glfwGetTime();

	glClearColor(161 / 255.0f, 219 / 255.0f, 255 / 255.0f, 1.0f);
	
	/*double MouseX, MouseY;*/
	double dMouseX, dMouseY;
	bool MouseButtonDown = 0;
	bool MouseRightButton = 0;
	bool LastCursorLock = 1;
	/*glm::mat4 ortho = glm::ortho(0.0f, (float)RenderBlock::WinWidth, 0.0f, (float)RenderBlock::WinWidth, 0.1f, 20.0f);
	shader.SetUniformMat4f("ortho", ortho);*/

	while (!glfwWindowShouldClose(RenderBlock::window))
	{
		//std::cout << RenderBlock::cameraPos.x << ' ' << RenderBlock::cameraPos.y << ' ' << RenderBlock::cameraPos.z << std::endl;
		

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
			ShowCursor(false);
		}
		else
		{
			ShowCursor(true);
		}
		LastCursorLock = RenderBlock::LockCursor;
		ShowCursor(true);
		
		
		RenderBlock::currentFrame = glfwGetTime();
		//float deltaTime = currentFrame - lstFrame;
		//lstFrame = currentFrame;
		//glfwGetWindowSize(window, &RenderBlock::WinWidth, &RenderBlock::WinHeight);

		RenderBlock::ProcessInput(RenderBlock::currentFrame - RenderBlock::lstFrame);
		if (glfwGetMouseButton(RenderBlock::window, GLFW_MOUSE_BUTTON_LEFT) && RenderBlock::LockCursor)
		{
			if (!MouseButtonDown)
			{
				auto look = RenderBlock::PlayerLookAt();
				if (!isnan(look.x))
				{
					*GenMain::WorldBlock(look.x, look.y, look.z) = 0;
					RenderBlock::ChunkShouldUpdate = 1;
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
				if (!isnan(look.x))
				{
					auto r = GenMain::WorldBlock(surf.x, surf.y, surf.z);
					if (r != nullptr)
					{
						*r = RenderBlock::SelectedBlock;
						RenderBlock::ChunkShouldUpdate = 1;
					}
					
					
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



		//float currentFrame = glfwGetTime();
		//while(!RenderBlock::shouldUpdate) {
		////	//RenderBlock::processInput(window, deltaTime);
		//}
		
		
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
		glEnable(GL_DEPTH_TEST);

		Renderer::Clear();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			Renderer::Draw(va, ib, shader, RenderBlock::RendererN1 * 6);


		EyeAngle[0] -= dMouseY * 0.12f;
		EyeAngle[1] += dMouseX * 0.12f;

		EyeAngle[0] = max(-89.5f, EyeAngle[0]);
		EyeAngle[0] = min( 89.5f, EyeAngle[0]);

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
void RenderBlock::RegisterBlock(float x, float y, float z, unsigned short sur, int id, unsigned layout, Generation::WorldUnit* unit)
{
	/*binfo[id] = {
		stx, sty, endx, endy,
		stx, sty, endx, endy,
		stx, sty, endx, endy,
		stx, sty, endx, endy,
		stx, sty, endx, endy,
		stx, sty, endx, endy,
	};*/

	

	if (ImportInfo::binfo.find(id) == ImportInfo::binfo.end())
	{
		std::cout << "Texture of Block id = " << id << " Cannot find.\n";
		id = 1;
	}

	if (sur & 0b010000)
	{
		auto lit = ULight::GenLight(x, y, z, unit, 0b010000);

		RenderBlock::UBasic w[] = {
		{x,     y    , z,  ImportInfo::binfo[id][0 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0]},
		{x,     y,  z + 1, ImportInfo::binfo[id][0 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 3] / RenderBlock::GHeight, lit.l[1][0]},
		{x + 1, y,     z,  ImportInfo::binfo[id][0 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 1] / RenderBlock::GHeight, lit.l[0][1]},
		{x + 1, y, z + 1,  ImportInfo::binfo[id][0 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1]} };
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
		{x,     y    , z,  ImportInfo::binfo[id][1 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0]},
		{x,     y + 1,  z, ImportInfo::binfo[id][1 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 3] / RenderBlock::GHeight, lit.l[0][1]},
		{x + 1, y,     z,  ImportInfo::binfo[id][1 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 1] / RenderBlock::GHeight, lit.l[1][0]},
		{x + 1, y + 1, z,  ImportInfo::binfo[id][1 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1]} };
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
		{x,     y    , z,   ImportInfo::binfo[id][2 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0]},
		{ x,     y,  z + 1, ImportInfo::binfo[id][2 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 3] / RenderBlock::GHeight, lit.l[1][0]},
		{ x, y + 1,     z,  ImportInfo::binfo[id][2 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 1] / RenderBlock::GHeight, lit.l[0][1]},
		{ x, y + 1, z + 1,  ImportInfo::binfo[id][2 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1]} };
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
		{x, y + 1, z,           ImportInfo::binfo[id][3 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0]},
		{ x,     y + 1,  z + 1, ImportInfo::binfo[id][3 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 3] / RenderBlock::GHeight, lit.l[1][0] },
		{ x + 1, y + 1,     z,  ImportInfo::binfo[id][3 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 1] / RenderBlock::GHeight, lit.l[0][1] },
		{ x + 1, y + 1, z + 1,  ImportInfo::binfo[id][3 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1] } };
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
		{x,     y    , z + 1, ImportInfo::binfo[id][4 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0]},
		{x,     y + 1, z + 1, ImportInfo::binfo[id][4 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 3] / RenderBlock::GHeight, lit.l[0][1]},
		{x + 1, y,     z + 1, ImportInfo::binfo[id][4 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 1] / RenderBlock::GHeight, lit.l[1][0]},
		{x + 1, y + 1, z + 1, ImportInfo::binfo[id][4 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1]} };
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
		{x + 1,     y    , z,  ImportInfo::binfo[id][5 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 1] / RenderBlock::GHeight, lit.l[0][0]},
		{x + 1,     y,  z + 1, ImportInfo::binfo[id][5 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 3] / RenderBlock::GHeight, lit.l[1][0]},
		{x + 1, y + 1,     z,  ImportInfo::binfo[id][5 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 1] / RenderBlock::GHeight, lit.l[0][1]},
		{x + 1, y + 1, z + 1,  ImportInfo::binfo[id][5 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 3] / RenderBlock::GHeight, lit.l[1][1]} };
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
