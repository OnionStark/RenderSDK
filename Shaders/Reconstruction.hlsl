#ifndef __RECONSTRUCTION__
#define __RECONSTRUCTION__

/*
Note:
The pseudo-code below uses row-major matrix representation.
The shader code expects matrix to have column-major order though.

ProjMatrix = 
{
	a, 0, 0, 0,
	0, b, 0, 0,
	0, 0, c, 1,
	0, 0, d, 0
};

InvProjMatrix = 
{
	1/a, 0, 0, 0,
	0, 1/b, 0, 0,
	0, 0, 0, 1/d,
	0, 0, 1, -c/d
};

viewSpacePosition = {x, y, z, 1};

clipSpacePosition = {a * x, b * y, c * z + d, z}, w component contains view space z coord;
afterWDivideProjSpacePosition = clipSpacePosition / clipSpacePosition.w = {a * x / z, b * y / z, ((c * z) + d) / z, 1}

result = afterWDivideProjSpacePosition * InvProjMatrix = {x/z, y/z, 1, 1/z}
viewSpacePosition = result / result.w;
*/

float ComputeViewSpaceDepth(float hardwareDepth, matrix projMatrix)
{
	return projMatrix._m23 / (hardwareDepth - projMatrix._m22);
}

float NormalizeViewSpaceDepth(float viewSpaceDepth, float nearPlane, float farPlane)
{
	return (viewSpaceDepth - nearPlane) / (farPlane - nearPlane);
}

float4 ComputeViewSpacePosition(float2 texCoord, float hardwareDepth, matrix projInvMatrix)
{
	float4 postWDivideProjSpacePos = float4(2.0f * texCoord.x - 1.0f, 1.0f - 2.0f * texCoord.y, hardwareDepth, 1.0f);
	
	float4 viewSpacePos = mul(projInvMatrix, postWDivideProjSpacePos);
	viewSpacePos /= viewSpacePos.w;

	return viewSpacePos;
}

float4 ComputeWorldSpacePosition(float2 texCoord, float hardwareDepth, matrix viewProjInvMatrix)
{
	float4 postWDivideProjSpacePos = float4(2.0f * texCoord.x - 1.0f, 1.0f - 2.0f * texCoord.y, hardwareDepth, 1.0f);

	float4 worldSpacePos = mul(viewProjInvMatrix, postWDivideProjSpacePos);
	worldSpacePos /= worldSpacePos.w;

	return worldSpacePos;
}

float2 ComputeScreenSpacePosition(float4 clipSpacePos, float2 screenSize)
{
	float4 postWDivideProjSpacePos = clipSpacePos / clipSpacePos.w;
		
	float2 screenSpacePos;
	screenSpacePos.x = 0.5f * screenSize.x * (postWDivideProjSpacePos.x + 1.0f);
	screenSpacePos.y = 0.5f * screenSize.y * (1.0f - postWDivideProjSpacePos.y);

	return screenSpacePos;
}

#endif // __RECONSTRUCTION__