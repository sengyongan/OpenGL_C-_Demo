#pragma once
//��ڵ�
#include"App.h"

Opengl::App* app = new Opengl::App();

int main(int argc, char** argv) {
	app->Run();
	delete app;
}
