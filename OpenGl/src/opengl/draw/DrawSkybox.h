#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawSkybox : public Draw {
	public:
		virtual ~DrawSkybox() {}

		virtual void Bind() const override;//�󶨶�������
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//����

	};
}