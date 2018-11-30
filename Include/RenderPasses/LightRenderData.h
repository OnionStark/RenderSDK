#pragma once

#include "Math/Matrix4.h"
#include "Math/Sphere.h"
#include "Math/Vector3.h"
#include "Math/Frustum.h"

struct SpotLightRenderData
{
	Matrix4f m_ViewProjMatrix;
	Vector3f m_RadiantIntensity;
	f32 m_RcpSquaredRange;
	Vector3f m_WorldSpacePos;
	f32 m_AngleFalloffScale;
	Vector3f m_WorldSpaceDir;
	f32 m_AngleFalloffOffset;
	Frustum m_WorldFrustum;
	Sphere m_WorldBounds;
	f32 m_NegativeExpShadowMapConstant;
	f32 m_LightViewNearPlane;
	f32 m_LightRcpViewClipRange;
	f32 m_LightProjMatrix43;
	f32 m_LightProjMatrix33;
	u32 m_LightID;
};
