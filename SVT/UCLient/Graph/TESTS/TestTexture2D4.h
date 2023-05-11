#pragma once
#include "Test.h"

namespace test
{
	class TestTexture2D4 : public Test
	{
	public:
		TestTexture2D4();
		~TestTexture2D4();

		void OnUpdate(float deltatime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> va;
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<IndexBuffer> ib;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;
		std::unique_ptr<Texture> texture2;

		glm::mat4 proj;
		glm::mat4 view;

		glm::vec3 translation;

		struct Vertex
		{
			float Positions[3];
			float Color[4];
			float TexCoords[2];
			float TexID;
		};
		
	};
}