//#include "Graph/BasicClass/Main.h"
#include "RenderBlockProcess.h"

bool RenderBlock::shouldUpdate = 0;
unsigned RenderBlock::offset = 0;
float RenderBlock::wh[8 * 32] = {};
unsigned RenderBlock::RendererN = 0;
unsigned RenderBlock::indices[65537];
//int main()
void RenderBlockProcess()
{
	// memset(RenderBlock::wh, 1, sizeof(RenderBlock::wh));
	// system("pause");
	for (int i = 0, k = 0; i < 65537 - 6; i += 6, k += 4)
	{
		RenderBlock::indices[i]		= k;
		RenderBlock::indices[i + 1]	= k + 1;
		RenderBlock::indices[i + 2]	= k + 2;
		RenderBlock::indices[i + 3]	= k + 1;
		RenderBlock::indices[i + 4]	= k + 2;
		RenderBlock::indices[i + 5]	= k + 3;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	auto window		= Renderer::NewWindow(800, 600);
	auto projection	= glm::perspective(glm::radians(30.0f), 800 / 600.0f, 0.1f, 100.0f);
	auto view		= glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -3.0f));
	auto model		= glm::rotate(glm::mat4(1.0), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));


	VertexArray va;
	VertexBuffer vb(nullptr, 1024, 1);


	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	layout.push<float>(4);
	va.AddBuffer(vb, layout);
	IndexBuffer ib(RenderBlock::indices, 1024);

	Shader shader("Basic.shader");
	shader.Bind();

	

	

	float h = 30.0f;

	while (!glfwWindowShouldClose(window))
	{
		while(!RenderBlock::shouldUpdate) {}

		Renderer::Clear();
		shader.SetUniformMat4f("proj", projection);
		shader.SetUniformMat4f("view", view);
		shader.SetUniformMat4f("model", model);
		glClear(GL_DEPTH_BUFFER_BIT);

		//set dynamic vertex buffer
		vb.Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, RenderBlock::offset * sizeof(float), RenderBlock::wh);

		//h += 0.1;
		RenderBlock::wh[0] += -0.01;
		RenderBlock::wh[32] += -0.01;
		vb.Bind();

		

		projection = glm::perspective(glm::radians(h), 800 / 600.0f, 0.1f, 100.0f);


		//shader.SetUniformMat4f("proj", projection);
		
		Renderer::Draw(va, ib, shader, RenderBlock::RendererN * 6);

		Renderer::Refresh(window);
	}

	Renderer::Close(window);

	system("pause");
}

void RenderBlock::RegisterBlock(int x, int y, int z)
{
	float w[] = {
	 x,      y,       0.0f, 0.0f,	  0.0f, 1.0f, 1.0f, 1.0f,
	 x,      y + 1,   0.0f, 1.0f,	  0.0f, 1.0f, 0.0f, 1.0f,
	 x + 1,  y,       1.0f, 0.0f,	  1.0f, 0.0f, 1.0f, 1.0f,
	 x + 1,  y + 1,   1.0f, 1.0f,	  1.0f, 1.0f, 1.0f, 1.0f,
	};

	memcpy(RenderBlock::wh + RenderBlock::offset, w, sizeof(w));
	++RenderBlock::RendererN;
	RenderBlock::offset += 32;
}