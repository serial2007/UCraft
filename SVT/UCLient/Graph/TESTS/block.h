#include "BasicClass/Main.h"

namespace UGraph
{
	extern void InitEngine();
	extern void RegisterBlock(float x, float y, float z, int id);
	extern void ClearRegister();

	extern GLFWwindow* window;
	extern glm::mat4 projection, view, model;

	extern float BlockBuffer[8 * 24];
	extern unsigned int IndexBuffer[65537];
	extern VertexArray* va;
	extern VertexBuffer* vb;
	extern ::IndexBuffer* ib;
	extern ::Shader* shader;
	extern unsigned RegisterN;
	extern unsigned offset, offset2;
	extern void RenderAll();
}