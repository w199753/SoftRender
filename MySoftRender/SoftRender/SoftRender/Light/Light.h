#pragma once
#include<memory>
#include<iostream>
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include"../Math/fMath.h"
#include"../Render/Object.h"
#include"../Shade/UnlitShader.h"
#include"../Render/Transform.hpp"
namespace softRD
{
	class Light
	{
	public:
		Light(const glm::vec3& pos,float i,const glm::vec3& col,std::unique_ptr<Object> _obj)
			:position(pos), intensity(i), color(col),dirction(glm::vec3(0,0,-1)),trans(std::make_unique<Transform>()),obj(std::move(_obj))
		{ 
			std::cout << "light" << std::endl;
			InitLightObj();
		}
		virtual ~Light() {}

		glm::vec3 position;
		float intensity;
		glm::vec3 color;
		glm::vec3 dirction;
		std::unique_ptr<Transform> trans;
		const glm::vec3& getPosition()const { return position; }
		const glm::vec3& getDirction()const { return dirction; }
		virtual void setTransform(const glm::vec3& pos, const glm::vec3& rot) = 0;
		virtual float getAttenuation(const glm::vec3& target) = 0;
		void InitLightObj()
		{
			obj->material.SetColor(glm::vec4(color,1));
			obj->SetTranslate(position.x,position.y,position.z);
			obj->SetScale(0.01, 0.01, 0.01);
		}
		std::unique_ptr<Object> obj;
	private:
		
	};

	class DirectionLight:public Light
	{
	public:
		//using Light::Light;
		DirectionLight(const glm::vec3& pos, float i, const glm::vec3& col, std::unique_ptr<Object> _obj) :Light(pos, i, col, std::move(_obj)) {
			//setTransform(pos, rot);
		}
		~DirectionLight() {}

		void setTransform(const glm::vec3& pos, const glm::vec3& rot) override
		{
			position = pos;
			dirction = fMath::Euler2Dir(rot);
			obj->SetTranslate(pos.x, pos.y, pos.z);
			obj->SetRotate(rot.x, rot.y, rot.z);
			obj->SetScale(0.1, 0.1, 0.1);
			obj->material.SetColor(glm::vec4(color, 1));
		}

		//平行光设定没有衰减
		float getAttenuation(const glm::vec3& target)override
		{
			return 1;
		}


	private:

	};

	class PointLight :public Light
	{
	private:

		float _c=1.0f, _l, _e;
		float m_range;
	public:
		PointLight(const glm::vec3& pos, float i, const glm::vec3& col, std::unique_ptr<Object> _obj,float range):Light(pos,i,col,std::move(_obj)),m_range(range) {
			std::cout << "Pointlight"<<position.x<<" "<<position.y <<position.z<< std::endl;
			SetRange(range);
			//setTransform(pos, rot);
		}

		void SetRange(float range)
		{
			const float R = 7.f;
			const float L = 0.7f;
			const float E = 1.8f;
			m_range = range;

			float ratio = range / R;
			_l = L / ratio;
			_e = E / ratio * ratio;
		}

		void setTransform(const glm::vec3& pos, const glm::vec3& rot) override
		{
			position = pos;
			dirction = glm::vec3(0);  //无方向
			obj->SetTranslate(pos.x, pos.y, pos.z);
			obj->SetRotate(dirction.x, dirction.y, dirction.z);
			obj->SetScale(0.1, 0.1, 0.1);
		}

		//衰减：intensity/C+L*dis+E*dis*dis
		float getAttenuation(const glm::vec3& target)override
		{
			float d = fMath::Distance(target,position);
			float att = 1.0 / (_c + _l * d + _e * d * d);
			//std::cout << d << " " << att << " " <<position.z<<" "<<"       "<<target.x<<" "<<target.y<<" "<<target.z<<"      "<<glm::length(position-target)<< std::endl;
			if (d>m_range)
			{
				return fMath::Lerp(att, 0.f, fMath::Saturate(std::abs(m_range - d) / (d))*3*m_range);
			}
			return att;
		}


		~PointLight() {}
	};
}


