#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawCube : public Draw {
	public:
		virtual ~DrawCube() {}

		virtual void Bind() const override;//�󶨶�������
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//����

	};
}