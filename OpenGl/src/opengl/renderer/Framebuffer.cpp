#include"Framebuffer.h"
#include<glad/glad.h>
#include"Renderer.h"
namespace Opengl {
    namespace Utils//ʵ�ù���
    {
        //����Ŀ��
        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;//����Ƕ��ز�������Ϊ���أ����򷵻���ͨ
        }
        //��������
        static void CreateTextures(uint32_t* outID, uint32_t count)//outID�����ʶ�������������ָ�룬count���������С
        {   //glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
            glGenTextures( count, outID);
        }
        //������
        static void BindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }
        //��ɫ���帽������
        static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                /////����������ռ�
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, nullptr);//Ϊ�������Data

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);////��������󸽼ӵ�֡�������� ��ɫ������
        }
        //��Ȼ��帽������
        static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 1, format, width, height, 0, format, GL_FLOAT, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }
        //�Ƿ�����ȸ�ʽ
        static bool IsDepthFormat(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
            }

            return false;
        }
        //��FramebufferTextureFormat�еĸ�ʽת��ΪGL_��ͷ�ĸ�ʽ
        static GLenum HazelFBTextureFormatToGL(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
            case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
            }

            return 0;
        }
    }

	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
        : m_Specification(spec)
    {
        for (auto spec : m_Specification.Attachments.Attachments) {//FramebufferTextureSpecification.Attachments����
            if (!Utils::IsDepthFormat(spec.TextureFormat))//������Ȼ���
                m_ColorAttachmentSpecifications.emplace_back(spec);//��ӵ���ɫ�����ʽ����
            else
                m_DepthAttachmentSpecification = spec;
        }
    }
    Framebuffer::~Framebuffer()
    {
        //glDeleteFramebuffers(1, &m_MultisampleRendererID);
    }
    void Framebuffer::InvalidateMRT()
    {
        glGenFramebuffers(1, &m_MRTRendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_MRTRendererID);
                // ---------------------------------------
        //unsigned int hdrFBO;

        ////Attachments����/////////////////////////////////////////////////////////////////////////////////////////////
        //bool multisample = m_Specification.Samples > 1;//���ز���Samples�����>1��������ز���������GL_TEXTURE_2D_MULTISAMPLE
        ////��ɫ��ʽ����
        if (m_ColorAttachmentSpecifications.size())
        {
                glGenTextures(2, colorBuffers);
                for (unsigned int i = 0; i < 2; i++)
                {
                    glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1024, 1024, 0, GL_RGBA, GL_FLOAT, NULL);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                    // attach texture to framebuffer
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
                }


        }
        //��ȸ�ʽ����
        if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
        {
            unsigned int rboDepth;
            glGenRenderbuffers(1, &rboDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

        }
        //��Ⱦ�������ɫ����
        //if (colorBuffers > 1)
        //{
            unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
            glDrawBuffers(2, attachments);

        //}
        //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        //    std::cout << "Framebuffer not complete!" << std::endl;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);//�����֡�������İ�
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    void Framebuffer::Initpingpong()
    {
        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongColorbuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1024, 1024, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
            // also check if framebuffers are complete (no need for depth buffer)
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Framebuffer not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    void Framebuffer::initMultisampleAttachment()
    {

        glGenFramebuffers(1, &m_MultisampleRendererID);//����֡����
        glBindFramebuffer(GL_FRAMEBUFFER, m_MultisampleRendererID);
        ///ColorAttachment////////////////////////////////////////////////////////////////////////////////
        glGenTextures(1, &m_MultisampleColorAttachment);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MultisampleColorAttachment);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, m_Specification.Width, m_Specification.Height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_MultisampleColorAttachment, 0);

        ///rendererBufferObj///////////////////////////////////////////////////////////////////////////////
        uint32_t rendererBufferObj;
        glGenRenderbuffers(1, &rendererBufferObj);
        glBindRenderbuffer(GL_RENDERBUFFER, rendererBufferObj);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rendererBufferObj);


        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        //
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void Framebuffer::initColorAttachment()
    {
        glGenFramebuffers(1, &m_RendererID);
        // - Create floating point color buffer
        glGenTextures(1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1024, 1024, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // - Create depth buffer (renderbuffer)
        GLuint rboDepth;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
        // - Attach buffers
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }
    void Framebuffer::iniDepthAttachment()
    {
        //Gen
        glGenFramebuffers(1, &m_DepthRendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthRendererID);
        //���������
        glGenTextures(1, &m_DepthMapAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthMapAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        //�����������
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);//���������Զ���������Ϊ
        //���ӵ�֡����
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMapAttachment, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //Check
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    void Framebuffer::initDepthCubeAttachment()
    {  
        // configure depth map FBO
        // -----------------------
        glGenFramebuffers(1, &m_DepthCubeRendererID);
        // create depth cubemap texture
        glGenTextures(1, &m_DepthMapCubeAttachment);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthMapCubeAttachment);
        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024,1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeRendererID);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthMapCubeAttachment, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete4!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void Framebuffer::BindMultisample()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_MultisampleRendererID);

    }
    void Framebuffer::BindRendererID()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    }
    void Framebuffer::BindDepthRendererID()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthRendererID);
    }
    void Framebuffer::BindDepthCubeRendererID()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeRendererID);
    }
    void Framebuffer::BindMultisampleTexture()
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MultisampleColorAttachment);
    }
    void Framebuffer::BindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    }
    void Framebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    void Framebuffer::BlitFramebuffer()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MultisampleRendererID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID);
        glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    }
    void Framebuffer::BindMRTFramebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_MRTRendererID);
    }
    void Framebuffer::BindPingPongFramebuffer(int index)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[index]);
    }
    void Framebuffer::framebuffer_size()
    {
        if ((m_Specification.Width == App::Get().GetWindow().GetNewWidth() &&
            m_Specification.Height == App::Get().GetWindow().GetNewHeight())
            ||
            (App::Get().GetWindow().GetNewWidth() < 0 || App::Get().GetWindow().GetNewHeight() < 0))
        {
            return;
        }
        m_Specification.Width = App::Get().GetWindow().GetNewWidth();
        m_Specification.Height = App::Get().GetWindow().GetNewHeight();

        initMultisampleAttachment();
        initColorAttachment();
        iniDepthAttachment();

    }
}