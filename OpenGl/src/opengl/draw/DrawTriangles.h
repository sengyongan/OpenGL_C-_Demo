#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawTriangles : public Draw {
	public:
		virtual ~DrawTriangles() {}

		virtual void Bind() const override;//�󶨶�������
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//����

	};
}