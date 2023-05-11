#include "TestTexture2D.h"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


test::TestTexture2D::TestTexture2D():
	proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
	view(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
	translation(200, 200, 0)

{
	float positions[] = {
	-50.0f, -50.0f, 0.0f, 0.0f,
	 50.0f, -50.0f, 1.0f, 0.0f,
	 50.0f,  50.0f, 1.0f, 1.0f,
	-50.0f,  50.0f, 0.0f, 1.0f
	};

	unsigned indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	va = std::make_unique<VertexArray>();
	vb = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	va->AddBuffer(*vb, layout);
	ib = std::make_unique<IndexBuffer>(indices, 6);


	shader = std::make_unique<Shader>("Basic.shader");
	shader->Bind();
	shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	texture = std::make_unique<Texture>("test.png");
	/*texture->Bind();*/
	shader->SetUniform1i("u_Texture", 0);
}



test::TestTexture2D::~TestTexture2D()
{
}

void test::TestTexture2D::OnUpdate(float deltatime)
{
}

void test::TestTexture2D::OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	texture->Bind();

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
		glm::mat4 mvp = proj * view * model * glm::translate(glm::mat4(1.0f), translation);
		
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", mvp);
		Renderer::Draw(*va, *ib, *shader);
	}
}

void test::TestTexture2D::OnImGuiRender()
{
	ImGui::SliderFloat3("float", &translation.x, -100.0f, 100.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
