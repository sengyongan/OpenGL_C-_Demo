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

        void BindMultisample();
        void BindRendererID();

        void BindMultisampleTexture();
        void BindTexture();

        void Unbind();

        void BlitFramebuffer();

        uint32_t GetMultisampleRendererID() const { return m_MultisampleRendererID; };//��ȡ��ɫ���帽��

        uint32_t GetRendererID() const { return m_RendererID; };//��ȡ��ɫ���帽��
        uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; };//��ȡ��ɫ���帽��

        const FramebufferSpecification& GetSpecification() const { return m_Specification; };


        void framebuffer_size();


    public:
        uint32_t m_MultisampleRendererID;//֡����id
        uint32_t m_MultisampleColorAttachment;//����id

        uint32_t m_RendererID;//֡����id
        uint32_t m_ColorAttachment;//����id

        FramebufferSpecification m_Specification;//֡�����ʽ
    };
}