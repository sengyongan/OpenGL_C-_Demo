#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawPointLight : public Draw {
	public:
		virtual ~DrawPointLight() {}

		virtual void Bind() const override;//�󶨶�������
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//����

	};
}