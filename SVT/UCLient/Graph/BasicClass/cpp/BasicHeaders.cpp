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

int IntDiv(int a, int b)
{
	if (a >= 0) return a / b;
	if (a % b == 0) return a / b;
	else return a / b - 1;
	return 0;
}

int IntMod(int a, int b)
{
	if (a >= 0) return a % b;
	if (a % b == 0) return 0;
	return b + a % b;
}



