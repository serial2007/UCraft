#include "RenderBlockProcess.h"
#include "BasicClass/lib/Texture.h"
#include "../../Generation/Entry.h"

bool				RenderBlock::shouldUpdate = 0;
unsigned			RenderBlock::offset = 0, RenderBlock::offset1 = 0;
RenderBlock::UBasic RenderBlock::wh[1024 * 256 * 4 * 8 * 4 ];
RenderBlock::UBasic RenderBlock::wh1[1024 * 256 * 4 * 8 * 4];
unsigned			RenderBlock::RendererN = 0;
unsigned			RenderBlock::RendererN1 = 0;
unsigned			RenderBlock::indices[65537 * 4 * 4 * 8 * 4 ];
float				RenderBlock::GHeight, RenderBlock::GWidth;
int					RenderBlock::WinWidth = 2560, RenderBlock::WinHeight = 1440;
glm::vec3			RenderBlock::cameraPos;
bool				RenderBlock::ProgramEnd = 0;
glm::vec3			RenderBlock::camFront;
glm::vec3			RenderBlock::camUp;
bool				RenderBlock::RegisterDone = 1;
GLFWwindow*			RenderBlock::window = nullptr;
float				RenderBlock::lstFrame;
float				RenderBlock::currentFrame;

bool RenderBlock::processInput(GLFWwindow* window, float deltatime)
{
	bool f = 0;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 30.0f * deltatime;    //用于控制移动速度
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		RenderBlock::cameraPos += cameraSpeed * RenderBlock::camFront;     //按下W，摄像机向前移动
		f = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		RenderBlock::cameraPos -= cameraSpeed * RenderBlock::camFront;     //按下S，摄像机向后移动
		f = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		//glm::cross(cameraFront, cameraUp)得出的是摄像机的右方向
	{
		RenderBlock::cameraPos -= glm::normalize(glm::cross(RenderBlock::camFront, RenderBlock::camUp)) * cameraSpeed;  //按下A，摄像机向左移动
		f = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		RenderBlock::cameraPos += glm::normalize(glm::cross(RenderBlock::camFront, RenderBlock::camUp)) * cameraSpeed;  //按下D，摄像机向右移动
		f = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		RenderBlock::cameraPos.y -= cameraSpeed;
		f = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		RenderBlock::cameraPos.y += cameraSpeed;
		f = 1;
	}

	return f;
}

void RenderBlockProcess()
{
	Renderer::ActivateImgui = 1;

	for (int i = 0, k = 0; i < 65537 * 4 * 4  - 6; i += 6, k += 4)
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
	auto projection	= glm::perspective(glm::radians(30.0f), RenderBlock::WinWidth / float(RenderBlock::WinHeight), 0.1f, 10000.0f);
	//auto view		= glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -3.0f));
	
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	VertexArray va;
	VertexBuffer vb(nullptr, 65536 * 128 * 4 * 8 * 4 , 0);

	
	VertexBufferLayout layout;
	layout.push<int>(3);
	layout.push<float>(2);
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

	while (!glfwWindowShouldClose(RenderBlock::window))
	{
		RenderBlock::currentFrame = glfwGetTime();
		//float deltaTime = currentFrame - lstFrame;
		//lstFrame = currentFrame;
		//glfwGetWindowSize(window, &RenderBlock::WinWidth, &RenderBlock::WinHeight);

		RenderBlock::processInput(RenderBlock::window, RenderBlock::currentFrame - RenderBlock::lstFrame);
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
		glBufferSubData(GL_ARRAY_BUFFER, 0, RenderBlock::offset1 * sizeof(RenderBlock::UBasic), RenderBlock::wh1);

		projection = glm::perspective(glm::radians(h), RenderBlock::WinWidth / float(RenderBlock::WinHeight), 0.1f, 100.0f);
		glEnable(GL_DEPTH_TEST);

		Renderer::Clear();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			Renderer::Draw(va, ib, shader, RenderBlock::RendererN1 * 6);

		ImGui::Begin("Test");

		ImGui::DragFloat3("CamPos", &RenderBlock::cameraPos.x);
		ImGui::DragFloat2("View", EyeAngle);
		ImGui::DragFloat("Rotate", &rg);
		ImGui::End();
		Renderer::Refresh(RenderBlock::window);
	}

	Renderer::Close(RenderBlock::window);
	
	SaveAll();

	std::cout << "Saved!\n";
	RenderBlock::ProgramEnd = 1;

	system("pause");
	
}

#include "../ImportInfo.h"

void RenderBlock::RegisterBlock(int x, int y, int z, unsigned short sur, int id, unsigned layout)
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
		RenderBlock::UBasic w[] = {
		{x,     y    , z,  ImportInfo::binfo[id][0 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 1] / RenderBlock::GHeight},
		{x,     y,  z + 1, ImportInfo::binfo[id][0 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 3] / RenderBlock::GHeight},
		{x + 1, y,     z,  ImportInfo::binfo[id][0 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 1] / RenderBlock::GHeight},
		{x + 1, y, z + 1,  ImportInfo::binfo[id][0 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][0 * 4 + 3] / RenderBlock::GHeight} };
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
		++RenderBlock::RendererN;
		RenderBlock::offset += 4;
	}

	if (sur & 0b001000)
	{
		RenderBlock::UBasic w[] = {
		{x,     y    , z,  ImportInfo::binfo[id][1 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 1] / RenderBlock::GHeight},
		{x,     y + 1,  z, ImportInfo::binfo[id][1 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 3] / RenderBlock::GHeight},
		{x + 1, y,     z,  ImportInfo::binfo[id][1 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 1] / RenderBlock::GHeight},
		{x + 1, y + 1, z,  ImportInfo::binfo[id][1 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][1 * 4 + 3] / RenderBlock::GHeight} };
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
		++RenderBlock::RendererN;
		RenderBlock::offset += 4;
	}

	if (sur & 0b100000)
	{
		RenderBlock::UBasic w[] = {
		{x,     y    , z,   ImportInfo::binfo[id][2 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 1] / RenderBlock::GHeight},
		{ x,     y,  z + 1, ImportInfo::binfo[id][2 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 3] / RenderBlock::GHeight},
		{ x, y + 1,     z,  ImportInfo::binfo[id][2 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 1] / RenderBlock::GHeight},
		{ x, y + 1, z + 1,  ImportInfo::binfo[id][2 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][2 * 4 + 3] / RenderBlock::GHeight} };
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
		++RenderBlock::RendererN;
		RenderBlock::offset += 4;
	}

	if (sur & 0b000010)
	{
		RenderBlock::UBasic w[] = {
		{x, y + 1, z,           ImportInfo::binfo[id][3 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 1] / RenderBlock::GHeight},
		{ x,     y + 1,  z + 1, ImportInfo::binfo[id][3 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 3] / RenderBlock::GHeight },
		{ x + 1, y + 1,     z,  ImportInfo::binfo[id][3 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 1] / RenderBlock::GHeight },
		{ x + 1, y + 1, z + 1,  ImportInfo::binfo[id][3 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][3 * 4 + 3] / RenderBlock::GHeight } };
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
		++RenderBlock::RendererN;
		RenderBlock::offset += 4;
	}

	if (sur & 0b000001)
	{
		RenderBlock::UBasic w[] = {
		{x,     y    , z + 1, ImportInfo::binfo[id][4 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 1] / RenderBlock::GHeight},
		{x,     y + 1, z + 1, ImportInfo::binfo[id][4 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 3] / RenderBlock::GHeight},
		{x + 1, y,     z + 1, ImportInfo::binfo[id][4 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 1] / RenderBlock::GHeight},
		{x + 1, y + 1, z + 1, ImportInfo::binfo[id][4 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][4 * 4 + 3] / RenderBlock::GHeight} };
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
		++RenderBlock::RendererN;
		RenderBlock::offset += 4;
	}

	if (sur & 0b000100)
	{
		RenderBlock::UBasic w[] = {
		{x + 1,     y    , z,  ImportInfo::binfo[id][5 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 1] / RenderBlock::GHeight},
		{x + 1,     y,  z + 1, ImportInfo::binfo[id][5 * 4 + 0] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 3] / RenderBlock::GHeight},
		{x + 1, y + 1,     z,  ImportInfo::binfo[id][5 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 1] / RenderBlock::GHeight},
		{x + 1, y + 1, z + 1,  ImportInfo::binfo[id][5 * 4 + 2] / RenderBlock::GWidth, ImportInfo::binfo[id][5 * 4 + 3] / RenderBlock::GHeight} };
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
		++RenderBlock::RendererN;
		RenderBlock::offset += 4;
	}
}

void RenderBlock::ClearAll()
{
	RenderBlock::RendererN = 0;
	RenderBlock::offset = 0;
}
