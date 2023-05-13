#pragma once

#include "BasicHeaders.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "../../../../Generation/Entry.h"

namespace Renderer
{
	extern inline void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, int count = -1);
	extern inline void Clear();
	extern inline void Refresh(GLFWwindow* window);
	extern inline void Close(GLFWwindow* window);

	extern inline void __TEST();
	extern bool ActivateImgui;
	extern inline GLFWwindow* NewWindow(int width, int height, const char* title = "Program");
}