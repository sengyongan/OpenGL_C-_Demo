#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawPoint : public Draw {
	public:
		virtual ~DrawPoint() {}

		virtual void Bind() const override;//�󶨶�������
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//����

	};
}