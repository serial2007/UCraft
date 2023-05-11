#include "../lib/Renderer.h"
;

inline void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, int count)
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	glCall(glDrawElements(GL_TRIANGLES, (count >= 0 ? count : ib.GetCount()), GL_UNSIGNED_INT, nullptr));
}

inline void Renderer::Clear()
{
	glCall(glClear(GL_COLOR_BUFFER_BIT));
	if (Renderer::ActivateImgui)
	{
		ImGui_ImplGlfwGL3_NewFrame();
	}
}

inline void Renderer::Refresh(GLFWwindow* window)
{
	if (Renderer::ActivateImgui)
	{
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

inline void Renderer::Close(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

inline void Renderer::__TEST()
{
	std::cout << "Success\n";
}

bool Renderer::ActivateImgui = 0;

inline GLFWwindow* Renderer::NewWindow(int width, int height, const char* title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	if (!glfwInit())
		std::cout << "Init Failure\n";
	GLFWwindow* window;
	window = glfwCreateWindow(width, height, title, 0, 0);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!\n";
		exit(1);
	}

	if (Renderer::ActivateImgui)
	{
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();
	}
	return window;
}