#include "TestClearColor.h"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
test::TestClearColor::TestClearColor()
	: m_ClearColor{ 0.0f, 0.0f, 1.0f, 1.0f }
{

}

test::TestClearColor::~TestClearColor()
{
}

void test::TestClearColor::OnUpdate(float deltatime)
{
}

void test::TestClearColor::OnRender()
{
	glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void test::TestClearColor::OnImGuiRender()
{
	ImGui::ColorEdit4("Clear Color", m_ClearColor);
}
