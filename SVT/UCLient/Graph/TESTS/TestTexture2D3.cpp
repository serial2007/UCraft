#include "TestTexture2D3.h"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


test::TestTexture2D3::TestTexture2D3():
	proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
	view(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
	translation(200, 200, 0)

{
	float positions[] = {
	 -50.0f, -50.0f,  0.0f, 0.0f,	1.0f, 1.0f, 0.0f ,0.0f,
	  50.0f, -50.0f,  1.0f, 0.0f,	1.0f, 0.0f, 0.0f ,0.0f,
	  50.0f,  50.0f,  1.0f, 1.0f,	1.0f, 0.0f, 0.0f ,0.0f,
	 -50.0f,  50.0f,  0.0f, 1.0f,	1.0f, 0.0f, 0.0f ,0.0f,

	 100.0f, -150.0f, 0.0f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,
	 150.0f, -150.0f, 1.0f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,
	 150.0f,  150.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f, 1.0f,
	 100.0f,  150.0f, 0.0f, 1.0f,	0.0f, 1.0f, 1.0f, 1.0f
	};


	unsigned indices[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4
	};

	va = std::make_unique<VertexArray>();
	vb = std::make_unique<VertexBuffer>(positions, sizeof(positions));
	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	layout.push<float>(3);
	layout.push<float>(1);

	va->AddBuffer(*vb, layout);
	ib = std::make_unique<IndexBuffer>(indices, 6 * 2);

	texture = std::make_unique<Texture>("test.png");
	texture2 = std::make_unique<Texture>("test2.png");

	shader = std::make_unique<Shader>("TESTS/MultiTexture.shader");
	shader->Bind();

}



test::TestTexture2D3::~TestTexture2D3()
{
}

void test::TestTexture2D3::OnUpdate(float deltatime)
{
}

void test::TestTexture2D3::OnRender()
{
	



	int samplers[2] = { 0,1 };

	shader->SetUniform1iv("u_Texture", 2, samplers);

	texture->Bind(1);
	texture2->Bind(0);

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
		glm::mat4 mvp = proj * view * model * glm::translate(glm::mat4(1.0f), translation);
		
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", mvp);
		//Renderer::Draw(*va, *ib, *shader);

		va->Bind();
		ib->Bind();
		glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}

void test::TestTexture2D3::OnImGuiRender()
{
	ImGui::SliderFloat3("float", &translation.x, -100.0f, 100.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
