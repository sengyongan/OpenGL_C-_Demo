#include "Input.h"

#include"App.h"
#include<GLFW/glfw3.h>

namespace Opengl {
    
    bool Input::IsKeyPressed(int keycode)
    {   
        //Applicationʵ��--window�����--m_Window����ʵ��,//��ȡ����,app->window class->m_window
        auto Window = static_cast<GLFWwindow*>(App::Get().GetWindow(). GetNativeWindow());
        auto state = glfwGetKey(Window, keycode);//��ȡ�����Ĵ��ڣ��Ƿ�ѹ�°���
        return state == GLFW_PRESS;//���³���������
    }
    bool Input::IsMouseButtonPressed(int button)
    {
        auto Window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(Window, button);
        return state == GLFW_PRESS;
    }
    std::pair<float, float> Input::GetMousePosition()//����Ԫ��
    {
        auto Window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(Window, &xpos, &ypos);//��ȡ�����Ĵ��ڣ�����λ��
        return { (float)xpos,(float)ypos };//ͨ��glfw������굱ǰλ��
    }
    float Input::GetMouseX()
    {
        auto [x, y] = GetMousePosition();
        return x;
    }
    float Input::GetMouseY()
    {
        auto [x, y] = GetMousePosition();
        return y;

    }

}