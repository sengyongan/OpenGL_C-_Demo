#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawScreenQuad : public Draw {
	public:
		virtual ~DrawScreenQuad() {}

		virtual void Bind() const override;//�󶨶�������
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//����

	};
}