#include "hzpch.h"
#include "EditorCamera.h"

#include"Input.h"
#include"App.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Opengl {
    // camera
    // timing
    //float deltaTime = 0.0f;	// time between current frame and last frame
    //float lastFrame = 0.0f;

    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip),
          m_ProMatrix(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
    {
        glfwSetScrollCallback(App::Get().GetWindow().GetNativeWindow(), MouseScrolledEvent);
        UpdateView();
    }

    void EditorCamera::UpdateProjection()//���¸���ͶӰ����
    {
        m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
        m_ProMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    }

    void EditorCamera::UpdateView()//�����ӿ�λ��
    {

        m_Position = CalculatePosition();//��ȡλ��

        glm::quat orientation = GetOrientation();//��ͼ�����ʾ���������λ����ת������
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);//��Ԫ������ת��Ϊ����
        m_ViewMatrix = glm::inverse(m_ViewMatrix);//��ת���ı䳡���Ļ���
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::pair<float, float> EditorCamera::PanSpeed() const//���ӿڴ�СӰ���ٶ�
    {
        float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f������ƣ��ӿ�ԽС���ƶ�Խ��
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor };
    }

    float EditorCamera::RotationSpeed() const
    {
        return 0.8f;
    }

    float EditorCamera::ZoomSpeed() const
    {
        float distance = m_Distance * 0.2f;
        distance = std::max(distance, 0.0f);//������СΪ0
        float speed = distance * distance;//ƽ������������Խ���ƶ�Խ�죬����ԽС�ƶ�Խ��
        speed = std::min(speed, 100.0f); // �ٶ����Ϊ100
        return speed;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorCamera::OnUpdate(float deltaTime)
    {
        // ts
        // --------------------
        //
        if (Input::IsKeyPressed(GLFW_KEY_LEFT_ALT))//��סalt
        {
            //��ȡ����ƶ���ֵ
            const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };//��ȡ�������ڴ���λ��
            glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;//��������ƶ����루������2D��Ļ��
            m_InitialMousePosition = mouse;//��һ�ε�λ��
            
            //
            if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))//����м�ƽ�ƣ�//�ٶȸ����ӿڴ�С&&����ı䣩
                MousePan(delta);
            else if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))//��������ת���ٶȲ��䣩
                MouseRotate(delta);
            else if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))//����Ҽ�����
                MouseZoom(delta.y);

        }

        //
        float cameraSpeed = static_cast<float>(2.5 * deltaTime);
        auto [xSpeed, ySpeed] = PanSpeed();//��ȡƽ���ٶ�

        if (Input::IsKeyPressed(GLFW_KEY_W))
            m_FocalPoint +=  GetUpDirection() * cameraSpeed * xSpeed * m_Distance;//����*��������������m_FocalPoint + �ƶ�����
        if (Input::IsKeyPressed(GLFW_KEY_S))
            m_FocalPoint -=  GetUpDirection() * cameraSpeed * xSpeed * m_Distance;
        if (Input::IsKeyPressed(GLFW_KEY_A))
            m_FocalPoint -= GetRightDirection() * cameraSpeed * ySpeed * m_Distance;//��ˣ����滯Ϊ-1����1
        if (Input::IsKeyPressed(GLFW_KEY_D))
            m_FocalPoint += GetRightDirection() * cameraSpeed * ySpeed * m_Distance;

        //
        UpdateView();//����m_ViewMatrix
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorCamera::MouseScrolledEvent(GLFWwindow* window, double xOffset, double yOffset)
    {
        double delta = yOffset * 0.1f;
        App::Get().GetCamera().MouseZoom(delta);
        App::Get().GetCamera().UpdateView();
    }

    void EditorCamera::MousePan(const glm::vec2& delta)//�������Ĳ�
    {
        auto [xSpeed, ySpeed] = PanSpeed();//��ȡƽ���ٶ�
        m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;//vec2��x,y)��������Ʋ�ֵΪ������Ҫ�����������
        m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;//������Ʋ�ֵΪ��
        //std::cout << delta.x << std::endl;
        //std::cout << delta.y << std::endl;
    }

    void EditorCamera::MouseRotate(const glm::vec2& delta)
    {
        //float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;//������ת����
        m_Pitch += delta.y * RotationSpeed();//������Ʋ�ֵΪ������Ҫ����ֲ�����������ת�������Ǹ�ֵ
        m_Yaw +=  delta.x * RotationSpeed();//������Ʋ�ֵΪ������Ҫ����ֲ�����������ת������������
        //std::cout << m_Pitch << m_Yaw <<std::endl;
        //std::cout << m_Yaw << std::endl;

    }

    void EditorCamera::MouseZoom(float delta)//delta.y���������ţ��ı�m_Positionʵ�ʵ������
    {
        m_Distance -= delta * ZoomSpeed();//�������ֵԽС��-= ԽԶ
        if (m_Distance < 1.0f)
        {
            m_FocalPoint += GetForwardDirection();
            std::cout << m_Distance <<std::endl;
            m_Distance = 1.0f;
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    glm::vec3 EditorCamera::GetUpDirection() const//������ʽvec3(0.0, -m_Yaw��0.0��
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetRightDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }
    //
    glm::vec3 EditorCamera::CalculatePosition() const//��������յ���λ������
    {   //-����Ϊ������ԽԶ��ֵԽ����Ҫ����Ϊ-z�� Ӧȡ��
        return m_FocalPoint - GetForwardDirection() * m_Distance;//�µ�λ�� = ƽ�Ƶ�Ŀ��� - �������z�����ϵľ���
    }

    glm::quat EditorCamera::GetOrientation() const//����ת����
    {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));//ת��Ϊ4Ԫ��
    }

}