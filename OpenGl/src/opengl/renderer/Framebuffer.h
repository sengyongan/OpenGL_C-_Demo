#pragma once
//֡����
#include"hzpch.h"
#include"opengl/core/App.h"
namespace Opengl {
    enum class FramebufferTextureFormat//֡���������ʽ
    {
        None = 0,

        // Color//��ɫ
        RGBA16F,
        RGBA,//��ɫ����

        // Depth/stencil//���/ģ��
        DEPTH24STENCIL8,

        // Defaults
        Depth = DEPTH24STENCIL8//depth��Ĭ��ֵ
    };

    struct FramebufferTextureSpecification//֡��������淶
    {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format)//��ʽ
            : TextureFormat(format) {}

        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;//�����ʽ����
        // TODO: filtering/wrap
    };

    struct FramebufferAttachmentSpecification//֡�����������淶
    {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)//����淶(�����б�
            : Attachments(attachments) {}

        std::vector<FramebufferTextureSpecification> Attachments;//��������
    };

    struct FramebufferSpecification {//֡�������淶
        uint32_t Width, Height;
        uint32_t Samples = 1;//���ز���
        FramebufferAttachmentSpecification Attachments;//�����淶-
        bool SwapChainTarget = false;//������Ŀ��
    };

    class Framebuffer {
    public:
        Framebuffer(const FramebufferSpecification& spec);
        ~Framebuffer();

        //init
        void initMultisampleAttachment();//MSAA(����ݣ����ز���
        void initColorAttachment();//��ͨ����ɫ�������������Ⱦ���󸽼�
        void iniDepthAttachment();//��ȸ�����ƽ�й���Ӱ
        void initDepthCubeAttachment();//��������帽��������Ӱ
        void InvalidateMRT();//MRT
        void Initpingpong();//pingpong
        void InitSSAO();//pingpong

        //bind ID
        void BindMultisample();
        void BindRendererID();
        void BindDepthRendererID();
        void BindDepthCubeRendererID();
        //bind Attach
        void BindMultisampleTexture();
        void BindTexture();
        //
        void BindMRTFramebuffer();
        void BindPingPongFramebuffer(const int index);
        //
        void BindSSAOFramebuffer();
        void BindSSAOBlurFramebuffer();
        //
        void Unbind();
        //�ƶ�buffer
        void BlitFramebuffer();

        //Get
        uint32_t GetMultisampleRendererID() const { return m_MultisampleRendererID; };//��ȡ��ɫ���帽��

        uint32_t GetRendererID() const { return m_RendererID; };//��ȡ��ɫ���帽��
        uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; };//��ȡ��ɫ���帽��

        uint32_t GetDepthRendererID() const { return m_DepthRendererID; };//��ȡ��ɫ����
        uint32_t GetDepthAttachmentRendererID() const { return m_DepthMapAttachment; };//��ȡ��Ȼ��帽�� 

        uint32_t GetDepthCubeRendererID() const { return m_DepthCubeRendererID; };//��ȡ��ɫ����
        uint32_t GetDepthCubeAttachmentRendererID() const { return m_DepthMapCubeAttachment; };//��ȡ��Ȼ��帽�� 

        uint32_t GetMRTRendererID() const { return m_MRTRendererID; };//��ȡ��ɫ����
        uint32_t GetMRTAttachmentRendererID(const int index) const { return colorBuffers[index]; };//��ȡ��ɫ����
        //
        uint32_t GetPingPongRendererID(const int index) const { return pingpongFBO[index]; };//��ȡ��ɫ����
        uint32_t GetPingPongAttachmentRendererID(const int index) const { return pingpongColorbuffers[index]; };//��ȡ��ɫ����
        //
        uint32_t GetSSAOColorBufferAttachmentID() const { return ssaoColorBuffer; };//��ȡ��ɫ����
        uint32_t GetSSAOColorBufferBlurAttachmentID() const { return ssaoColorBufferBlur; };//��ȡ��ɫ����

        //
        const FramebufferSpecification& GetSpecification() const { return m_Specification; };
        //����size
        void framebuffer_size();

    public:
        //MSAA////////////////////////////////////////////////////////////
        uint32_t m_MultisampleRendererID;//֡����id---������Ⱦ
        uint32_t m_MultisampleColorAttachment;//����id

        //��ͨ////////////////////////////////////////////////////////////
        uint32_t m_RendererID;//֡����id---��ɫ����
        uint32_t m_ColorAttachment;//����id
        uint32_t m_DepthAttachment;//����id

        //����Ӱ////////////////////////////////////////////////////////////
        uint32_t m_DepthRendererID;//֡����id---��ȸ���
        uint32_t m_DepthMapAttachment;//��ȸ���

        //������////////////////////////////////////////////////////////////
        uint32_t m_DepthCubeRendererID;//֡����id---��������帽��
        uint32_t m_DepthMapCubeAttachment;//��ȸ���

        //pingpong////////////////////////////////////////////////////////////
        uint32_t pingpongFBO[2];
        uint32_t pingpongColorbuffers[2];
        //SSAO////////////////////////////////////////////////////////////
        uint32_t ssaoFBO, ssaoBlurFBO;
        uint32_t ssaoColorBuffer, ssaoColorBufferBlur;

        /////////////////////////////////////////////////////////////////////
        FramebufferSpecification m_Specification;//֡�����ʽ


        //����ȾĿ��MRT
        uint32_t m_MRTRendererID;//֡����id---��������帽��
        //�������淶����ʽ��
        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;//֡��������ɫ�����淶���飨�����ж����
        FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;////֡��������Ⱥ�ģ�帽���淶////Ĭ���ƶ���ֵ����������FramebufferTextureSpecification�е�TextureFormat����Ϊformat.TextureFormat
        //����id
        unsigned int colorBuffers[10];
        uint32_t m_MRTDepthAttachment = 0;


    };
}