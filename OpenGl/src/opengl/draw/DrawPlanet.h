#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawPlanet : public Draw {
	public:
		virtual ~DrawPlanet() {}

		virtual void Bind() const override;//�󶨶�������
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//����
		void OnDrawPlanet(const std::shared_ptr<Shader>& shader) const;//����
		void OnDrawRock(const std::shared_ptr<Shader>& shader) const;//����

	};
}