#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawGeometry : public Draw {
	public:
		virtual ~DrawGeometry() {}

		virtual void Bind() const override;//�󶨶�������
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//����

	};
}