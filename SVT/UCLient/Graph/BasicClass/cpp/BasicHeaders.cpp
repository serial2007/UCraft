#include "../lib/BasicHeaders.h"

void glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
void glCheckError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")\n";
	}
}

bool glLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << ' ' << file << ':' << line << '\n';
		return false;
	}
	return true;
}



