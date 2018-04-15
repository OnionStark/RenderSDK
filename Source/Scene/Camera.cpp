#include "Scene/Camera.h"
#include "Common/Color.h"
#include "Math/Math.h"

Camera::Camera(ProjType projType, f32 nearClipPlane, f32 farClipPlane, f32 aspectRatio, f32 maxMoveSpeed, f32 maxRotationSpeed)
	: SceneObject("Camera")
	, m_ProjType(projType)
	, m_BackgroundColor(Color::BLUE)
	, m_NearClipPlane(nearClipPlane)
	, m_FarClipPlane(farClipPlane)
	, m_AspectRatio(aspectRatio)
	, m_FovYInRadians(PI_DIV_4)
	, m_SizeY(10.0f)
	, m_MaxMoveSpeed(maxMoveSpeed)
	, m_MaxRotationSpeed(maxRotationSpeed)
	, m_DirtyFlags(DirtyFlag_ProjMatrix)
{
}

f32 Camera::GetAspectRatio() const
{
	return m_AspectRatio;
}

void Camera::SetAspectRatio(f32 aspectRatio)
{
	m_AspectRatio = aspectRatio;
	m_DirtyFlags |= DirtyFlag_ProjMatrix;
}

f32 Camera::GetFovY() const
{
	assert(m_ProjType == ProjType_Perspective);
	return m_FovYInRadians;
}

void Camera::SetFovY(f32 fovYInRadians)
{
	assert(m_ProjType == ProjType_Perspective);
	m_FovYInRadians = fovYInRadians;
	m_DirtyFlags |= DirtyFlag_ProjMatrix;
}

f32 Camera::GetSizeY() const
{
	assert(m_ProjType == ProjType_Ortho);
	return m_SizeY;
}

void Camera::SetSizeY(f32 sizeY)
{
	assert(m_ProjType == ProjType_Ortho);
	m_SizeY = sizeY;
	m_DirtyFlags |= DirtyFlag_ProjMatrix;
}

f32 Camera::GetMaxMoveSpeed() const
{
	return m_MaxMoveSpeed;
}

void Camera::SetMaxMoveSpeed(f32 maxMoveSpeed)
{
	m_MaxMoveSpeed = maxMoveSpeed;
}

f32 Camera::GetMaxRotationSpeed() const
{
	return m_MaxRotationSpeed;
}

void Camera::SetMaxRotationSpeed(f32 maxRotationSpeed)
{
	m_MaxRotationSpeed = maxRotationSpeed;
}

Camera::ProjType Camera::GetProjType() const
{
	return m_ProjType;
}

void Camera::SetProjType(Camera::ProjType projType)
{
	m_ProjType = projType;
	m_DirtyFlags |= DirtyFlag_ProjMatrix;
}

const Vector4f& Camera::GetBackgroundColor() const
{
	return m_BackgroundColor;
}

void Camera::SetBackgroundColor(const Vector4f& backgroundColor)
{
	m_BackgroundColor = backgroundColor;
}

f32 Camera::GetNearClipPlane() const
{
	return m_NearClipPlane;
}

void Camera::SetNearClipPlane(f32 nearClipPlane)
{
	m_NearClipPlane = nearClipPlane;
	m_DirtyFlags |= DirtyFlag_ProjMatrix;
}

f32 Camera::GetFarClipPlane() const
{
	return m_FarClipPlane;
}

void Camera::SetFarClipPlane(f32 farClipPlane)
{
	m_FarClipPlane = farClipPlane;
	m_DirtyFlags |= DirtyFlag_ProjMatrix;
}

const Matrix4f& Camera::GetViewMatrix() const
{
	return GetTransform().GetWorldToLocalMatrix();
}

const Matrix4f& Camera::GetProjMatrix() const
{
	if (m_DirtyFlags & DirtyFlag_ProjMatrix)
	{
		if (m_ProjType == ProjType_Ortho)
		{
			m_ProjMatrix = CreateOrthoProjMatrix(m_AspectRatio * m_SizeY, m_SizeY, m_NearClipPlane, m_FarClipPlane);
		}
		else if (m_ProjType == ProjType_Perspective)
		{
			m_ProjMatrix = CreatePerspectiveFovProjMatrix(m_FovYInRadians, m_AspectRatio, m_NearClipPlane, m_FarClipPlane);
		}
		else
		{
			assert(false);
		}
		m_DirtyFlags &= ~DirtyFlag_ProjMatrix;
	}
	return m_ProjMatrix;
}