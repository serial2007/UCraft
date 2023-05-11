#include "block.h"

float			UGraph::BlockBuffer[8 * 24] = {};
unsigned int	UGraph::IndexBuffer[65537] = {0, 1, 2, 1, 2, 3, 0, 1, 2, 1, 2, 3};

GLFWwindow*		UGraph::window = nullptr;
glm::mat4		UGraph::projection, UGraph::view, UGraph::model;
VertexArray*	UGraph::va = nullptr;
VertexBuffer*	UGraph::vb = nullptr;
unsigned		UGraph::RegisterN = 0;
unsigned		UGraph::offset, UGraph::offset2;
::IndexBuffer*	UGraph::ib = nullptr;
::Shader*		UGraph::shader = nullptr;


void UGraph::InitEngine()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	UGraph::window = Renderer::NewWindow(800, 600);
	UGraph::vb = new VertexBuffer(nullptr, 1024, 1);
	UGraph::va = new ::VertexArray();
	UGraph::ib = new ::IndexBuffer(UGraph::IndexBuffer, 12);
	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	layout.push<float>(4);
	va->AddBuffer(*vb, layout);
	va->Bind();

	
	UGraph::shader = new ::Shader("Basic.shader");
	
	
	UGraph::projection	= glm::perspective(glm::radians(30.0f), 800 / 600.0f, 0.1f, 100.0f);
	UGraph::view		= glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -3.0f));
	UGraph::model		= glm::rotate(glm::mat4(1.0), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	UGraph::RegisterN	= 0;
	UGraph::offset		= 0;
	UGraph::offset2		= 0;

	

	/*unsigned int tmp2[] = {
		0, 1, 2,
		1, 2, 3
	};
	unsigned terms = 65535 / 10;*/
	/*while (terms--)
	{
		memcpy(UGraph::BlockBuffer + UGraph::offset2, tmp2, sizeof(tmp2));
		UGraph::offset2 += sizeof(tmp2);
	}*/
	UGraph::shader->Bind();
	//UGraph::shader->SetUniform1f("test", 0.0f);
	UGraph::shader->SetUniformMat4f("proj", UGraph::projection);
	UGraph::shader->SetUniformMat4f("view", UGraph::view);
	UGraph::shader->SetUniformMat4f("model", UGraph::model);
}


void UGraph::RegisterBlock(float x, float y, float z, int id)
{
	++UGraph::RegisterN;
	float tmp[] = {
		x,	y,	0,	0,	1,	0,	0,	1,
		x+10,y,	1,	0,	0,	1,	0,	1,
		x,	y+10,0,	1,	0,	0,	1,	1,
		x+10,y+10,1,	1,	1,	1,	1,	1
	};
	memcpy(UGraph::BlockBuffer + UGraph::offset, tmp, sizeof(tmp));
	UGraph::offset += sizeof(tmp);
	sizeof(tmp);

	std::cout << "Register\n";
	
}

void UGraph::ClearRegister()
{
	UGraph::offset		= 0;
	UGraph::offset2		= 0;
	UGraph::RegisterN	= 0;
}

void UGraph::RenderAll()
{
	VertexArray va1;
	VertexBuffer vb1(UGraph::BlockBuffer, 1024);
	VertexBufferLayout ly;
	ly.push<float>(2);
	ly.push<float>(2);
	ly.push<float>(4);
	va1.AddBuffer(vb1, ly);
	Renderer::Draw(va1, *UGraph::ib, *UGraph::shader);



	UGraph::vb->Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(UGraph::BlockBuffer), UGraph::BlockBuffer);
	UGraph::ib->Bind();
	UGraph::vb->Bind();
	glCall(Renderer::Draw(*UGraph::va, *UGraph::ib, *UGraph::shader));
	//Renderer::Draw(*UGraph::va, *UGraph::ib, *UGraph::shader, 1);
	//std::cout << "Render\n";
}
