#include "./BasicClass/Main.h"
#include "./TESTS/TestClearColor.h"
#include "./TESTS/TestTexture2D.h"
#include "./TESTS/TestTexture2D2.h"
#include "./TESTS/TestTexture2D3.h"
#include "./TESTS/TestTexture2D4.h"
;

int main()
{
	Renderer::ActivateImgui = 1;
	auto window = Renderer::NewWindow(960, 540);

	test::Test* currentTest = nullptr;
	test::TestMenu* testMenu = new test::TestMenu(currentTest);
	currentTest = testMenu;

	testMenu->RegisterTest<test::TestClearColor>("TestClearColor");
	testMenu->RegisterTest<test::TestTexture2D>("TestTexture2D");
	testMenu->RegisterTest<test::TestTexture2D2>("TestTexture2D2");
	testMenu->RegisterTest<test::TestTexture2D3>("TestTexture2D3");
	testMenu->RegisterTest<test::TestTexture2D4>("TestTexture2D4");

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		Renderer::Clear();
		if (currentTest)
		{
			currentTest->OnUpdate(0.0f);
			currentTest->OnRender();
			ImGui::Begin("Test");
			if (currentTest != testMenu && ImGui::Button("Terminate"))
			{
				delete currentTest;
				currentTest = testMenu;
			}
			currentTest->OnImGuiRender();
			ImGui::End();
		}
		Renderer::Refresh(window);

	}
	
	if (currentTest != testMenu)
		delete testMenu;

	delete currentTest;

	std::cout << "User Closed the window\n";

	Renderer::Close(window);
	system("pause");
	exit(0);
}