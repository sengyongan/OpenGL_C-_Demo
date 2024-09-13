#pragma once
#include "App.h"
#include <memory>  // for std::unique_ptr

int main(int argc, char** argv) {
    // ʹ������ָ����� App �����Զ������ڴ��ͷ�
    std::unique_ptr<Opengl::App> app = std::make_unique<Opengl::App>();

    // ����Ӧ�ó���
    app->Run();

    // �� main ��������ʱ������ָ����Զ����ٶ���������ʽ delete
    return 0;
}