#pragma once
#include "Test.h"

namespace test
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltatime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> va;
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<IndexBuffer> ib;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;

		glm::mat4 proj;
		glm::mat4 view;

		glm::vec3 translation;
	};
}