#pragma once
//֡����
#include"hzpch.h"
#include"opengl/core/App.h"
namespace Opengl {

    struct FramebufferSpecification {
        uint32_t Width, Height;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class Framebuffer {
    public:
        Framebuffer(const FramebufferSpecification& spec);
        ~Framebuffer();

        void initMultisampleAttachment();//MSAA(����ݣ�
        void initColorAttachment();//��ͨframebuffer
        void iniDepthAttachment();//��ͨframebuffer

        void BindMultisample();
        void BindRendererID();
        void BindDepthRendererID();

        void BindMultisampleTexture();
        void BindTexture();

        void Unbind();

        void BlitFramebuffer();

        uint32_t GetMultisampleRendererID() const { return m_MultisampleRendererID; };//��ȡ��ɫ���帽��

        uint32_t GetRendererID() const { return m_RendererID; };//��ȡ��ɫ���帽��
        uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; };//��ȡ��ɫ���帽��

        uint32_t GetDepthRendererID() const { return m_DepthRendererID; };//��ȡ��ɫ����
        uint32_t GetDepthAttachmentRendererID() const { return m_DepthMapAttachment; };//��ȡ��Ȼ��帽�� 


        const FramebufferSpecification& GetSpecification() const { return m_Specification; };


        void framebuffer_size();


    public:
        uint32_t m_MultisampleRendererID;//֡����id---������Ⱦ
        uint32_t m_MultisampleColorAttachment;//����id

        uint32_t m_RendererID;//֡����id---��ɫ����
        uint32_t m_ColorAttachment;//����id

        uint32_t m_DepthRendererID;//֡����id---��ȸ���
        uint32_t m_DepthMapAttachment;//��ȸ���

        FramebufferSpecification m_Specification;//֡�����ʽ
    };
}