#include "RenderBlockProcess.h"
#include "BasicClass/lib/Texture.h"

bool				RenderBlock::shouldUpdate = 0;
unsigned			RenderBlock::offset = 0;
RenderBlock::UBasic RenderBlock::wh[8 * 256];
unsigned			RenderBlock::RendererN = 0;
unsigned			RenderBlock::indices[65537];

void RenderBlockProcess()
{
	Renderer::ActivateImgui = 1;

	for (int i = 0, k = 0; i < 65537 - 6; i += 6, k += 4)
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

	auto window		= Renderer::NewWindow(800, 600);
	auto projection	= glm::perspective(glm::radians(30.0f), 800 / 600.0f, 0.1f, 100.0f);
	//auto view		= glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -3.0f));
	


	VertexArray va;
	VertexBuffer vb(nullptr, 65536 * 128, 0);


	VertexBufferLayout layout;
	layout.push<int>(3);
	layout.push<float>(2);
	va.AddBuffer(vb, layout);
	IndexBuffer ib(RenderBlock::indices, 65536);

	Shader shader("UCLient/Basic.shader");
	shader.Bind();

	
	Texture texture("UCLient/test.png");
	texture.Bind();

	float h = 30.0f;

	auto look = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.1f, 100.0f);
	//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	//auto view = glm::lookAt(glm::vec3(0.0f, -10.0f, 3.0f),   //摄像机的位置
	//	glm::vec3(0.0f, 0.0f, 0.0f),           //摄像机看向的顶点坐标，这里朝向原点
	//	glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 cameraPos = glm::vec3(2.0f, 8.0f, 2.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float EyeAngle[2] = { -2.0f, -75.0f };
	float rg = -90.0f;
	while (!glfwWindowShouldClose(window))
	{
		Renderer::Clear();
		auto model = glm::rotate(glm::mat4(1.0), glm::radians(rg), glm::vec3(1.0f, 0.0f, 0.0f));



		//float currentFrame = glfwGetTime();
		while(!RenderBlock::shouldUpdate) {}
		glClear(GL_DEPTH_BUFFER_BIT);
		shader.SetUniformMat4f("proj", look);
		
		cameraFront.x = cos(glm::radians(EyeAngle[0])) * cos(glm::radians(EyeAngle[1]));
		cameraFront.y = sin(glm::radians(EyeAngle[0]));
		cameraFront.z = cos(glm::radians(EyeAngle[0])) * sin(glm::radians(EyeAngle[1]));


		auto view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.SetUniformMat4f("view", view);
		shader.SetUniformMat4f("model", model);
		

		vb.Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, RenderBlock::offset * sizeof(RenderBlock::UBasic), RenderBlock::wh);

		projection = glm::perspective(glm::radians(h), 800 / 600.0f, 0.1f, 100.0f);
		glEnable(GL_DEPTH_TEST);
		Renderer::Draw(va, ib, shader, RenderBlock::RendererN * 6);

		ImGui::Begin("Test");

		ImGui::DragInt3("VertxPos", &RenderBlock::wh->x);
		ImGui::DragFloat3("CamPos", &cameraPos.x);
		ImGui::DragFloat2("View", EyeAngle);
		ImGui::DragFloat("Rotate", &rg);
		ImGui::End();
		Renderer::Refresh(window);
	}

	Renderer::Close(window);
	system("pause");
}

void RenderBlock::RegisterBlock(int x, int y, int z, unsigned short sur)
{
	if (sur & 0b010000)
	{
		RenderBlock::UBasic w[] = {
		{x,     y    , z,  0.0f, 0.0f},
		{x,     y,  z + 1, 0.0f, 1.0f},
		{x + 1, y,     z,  1.0f, 0.0f},
		{x + 1, y, z + 1,  1.0f, 1.0f} };
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
		++RenderBlock::RendererN;
		RenderBlock::offset += 4;
	}

	if (sur & 0b001000)
	{
		RenderBlock::UBasic w[] = {
		{x,     y    , z,  0.0f, 0.0f},
		{x,     y + 1,  z, 0.0f, 1.0f},
		{x + 1, y,     z,  1.0f, 0.0f},
		{x + 1, y + 1, z,  1.0f, 1.0f} };
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
		++RenderBlock::RendererN;
		RenderBlock::offset += 4;
	}

	if (sur & 0b100000)
	{
		RenderBlock::UBasic w[] = {
		{x,     y    , z,  0.0f, 0.0f },
		{ x,     y,  z + 1, 0.0f, 1.0f },
		{ x, y + 1,     z,  1.0f, 0.0f },
		{ x, y + 1, z + 1,  1.0f, 1.0f } };
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
		++RenderBlock::RendererN;
		RenderBlock::offset += 4;
	}

	if (sur & 0b000010)
	{
		RenderBlock::UBasic w[] = {
		{x, y + 1, z, 0.0f, 0.0f},
		{ x,     y + 1,  z + 1, 0.0f, 1.0f },
		{ x + 1, y + 1,     z,  1.0f, 0.0f },
		{ x + 1, y + 1, z + 1,  1.0f, 1.0f } };
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
		++RenderBlock::RendererN;
		RenderBlock::offset += 4;
	}

	if (sur & 0b000001)
	{
		RenderBlock::UBasic w[] = {
		{x,     y    , z + 1, 0.0f, 0.0f},
		{x,     y + 1, z + 1, 0.0f, 1.0f},
		{x + 1, y,     z + 1, 1.0f, 0.0f},
		{x + 1, y + 1, z + 1, 1.0f, 1.0f} };
		memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
		++RenderBlock::RendererN;
		RenderBlock::offset += 4;
	}

	if (sur & 0b000100)
	{
		RenderBlock::UBasic w[] = {
		{x + 1,     y    , z,  0.0f, 0.0f},
		{x + 1,     y,  z + 1, 0.0f, 1.0f},
		{x + 1, y + 1,     z,  1.0f, 0.0f},
		{x + 1, y + 1, z + 1,  1.0f, 1.0f} };
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
