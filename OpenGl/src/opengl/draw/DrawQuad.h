#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawQuad : public Draw {
	public:
		virtual ~DrawQuad() {}

		virtual void Bind() const override;//�󶨶�������
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//����

	};
}