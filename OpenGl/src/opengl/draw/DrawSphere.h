#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawSphere : public Draw {
	public:
		virtual ~DrawSphere() {}

		virtual void Bind() const override;//�󶨶�������
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//����

	};
}