#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"App.h"
namespace Opengl {

    Window::Window()
    {
    }

    Window::~Window()
    {
        Window::Ondestory();
    }
    void Window::Init(const unsigned int& width, const unsigned int& height, std::string windowName)
    {
        // glfw: initialize and configure��ʼ��glfw
        // ------------------------------
        glfwInit();
        // glfw window creation��������
        // --------------------
        m_Window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        if (m_Window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();//��ֹʹ�� OpenGL �� Vulkan ��Ӧ�ó���
        }
        glfwMakeContextCurrent(m_Window);//���봰��������
        //callbake
        glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);//���ûص����������Ե������ڴ�С

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

    }
    void Window::OnUpdate()
    {
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        //if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        //    glfwSetWindowShouldClose(m_Window, true);
        processInput(m_Window);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();//���callback�¼�
    }

    void Window::Ondestory()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();

    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void Window::processInput(GLFWwindow* window)
    {
        //isRunning = false;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }


    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
        App::Get().GetWindow(). m_NewWidth = width;
        std::cout << "GLFW" << width << std::endl;
        App::Get().GetWindow(). m_NewHeight = height;
        std::cout << "GLFW" << height << std::endl;
    }


}
