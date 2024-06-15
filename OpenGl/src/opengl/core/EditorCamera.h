#pragma once
//͸�������
#include"Window.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Opengl {

    class EditorCamera 
    {
    public:

        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);//���뵽m_ProMatrix
        //
        void OnUpdate(float deltaTime);

        //λ��
        inline float GetDistance() const { return m_Distance; }
        inline void SetDistance(float distance) { m_Distance = distance; }

        //�����ӿڴ�С
        inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }
        
        //Get
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetProjection() const { return m_ProMatrix; }
        glm::mat4 GetViewProjection() const { return m_ProMatrix * m_ViewMatrix; }//���գ���ȡ��ͼͶӰ����(���ǵı༭�������
        
        //��ȡ���������ת�󣬾ֲ� �ϣ��ң�ǰ ������
        glm::vec3 GetUpDirection() const;//��ȡ�Ϸ���
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;

        //��ȡ��� λ�� || ��ת��Ԫ��
        const glm::vec3& GetPosition() const { return m_Position; }
        glm::quat GetOrientation() const;

        //��ȡ��� ��ĳ����ת�ĽǶ�
        float GetPitch() const { return m_Pitch; }
        float GetYaw() const { return m_Yaw; }
        //
        static void MouseScrolledEvent(GLFWwindow* window, double xOffset, double yOffset);

    private:

        void UpdateProjection();//����m_ProMatrix
        void UpdateView();//����m_ViewMatrix���������ƽ�ƺ���ת�����ţ�����

        //������
        //bool OnMouseScroll(MouseScrolledEvent& e);

        //���ƽ�ƣ���ת������
        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);

        //����λ��
        glm::vec3 CalculatePosition() const;

        //���ƽ�ƣ���ת�����ŵ��ٶ�
        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed() const;

    private:
        //ͶӰ������Ұ����߱ȣ���ƽ���Զƽ�棩
        float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

        //
        glm::mat4 m_ViewMatrix;//��ͼ����,
        glm::mat4 m_ProMatrix;//ͶӰ����

        //
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };//��������λ�ã�ƽ�Ƶ�Ŀ��� - �������z�����ϵľ��룩
        glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };//����Ŀ���

        //
        glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };//�����һ��λ��

        //
        float m_Distance = 10.0f;//����ԭ��ľ���
        //
        float m_Pitch = 0.0f, m_Yaw = 0.0f;//���Ǻ�ƫ����

        //
        float m_ViewportWidth = 1280, m_ViewportHeight = 720;//�ӿڴ�С

    };

}