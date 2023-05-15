#pragma once
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_glfw_gl3.h"
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory> 

#define ASSERT(x) if(!(x)) __debugbreak();
#define glCall(x) {glClearError();x;ASSERT(glLogCall(#x, __FILE__, __LINE__));}

extern void glClearError();
extern void glCheckError();
extern bool glLogCall(const char* function, const char* file, int line);

extern int IntDiv(float a, int b);
extern int IntMod(float a, int b);