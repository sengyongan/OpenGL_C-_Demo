#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawNewCube : public Draw {
	public:
		virtual ~DrawNewCube() {}

		virtual void Bind() const override;//�󶨶�������
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//����

	};
}