struct RayPayload
{
	float3 color;
};

struct AppData
{
	float3 cameraWorldPos;
	float3 cameraWorldAxisX;
	float3 cameraWorldAxisY;
	float3 cameraWorldAxisZ;
	float rayMinExtent;
	float rayMaxExtent;
};

ConstantBuffer<AppData> g_AppDataBuffer : register(b0);
RaytracingAccelerationStructure g_AccelStruct : register(t0);
RWTexture<float4> g_OutputTexture : register(u0);

[shader("raygeneration")]
void RayGeneration()
{
	uint2 pixelPos = DispatchRaysIndex().xy;
	uint2 numPixels = DispatchRaysDimensions().xy;

	float2 localPos = (float2(pixelPos) + 0.5f) / float2(numPixels); // [0..1]
	localPos = float2(2.0f, -2.0f) * localPos + float2(-1.0f, 1.0f); // [-1..1]

	float3 localDir = float3(localPos.xy, 1.0f);
	float3 worldDir = localDir.x * g_AppDataBuffer.cameraWorldAxisX + localDir.y * g_AppDataBuffer.cameraWorldAxisY + localDir.z * g_AppDataBuffer.cameraWorldAxisZ;
	worldDir = normalize(worldDir);

	RayDesc ray;
	ray.Origin = g_AppDataBuffer.cameraWorldPos;
	ray.Direction = worldDir;
	ray.TMin = g_AppDataBuffer.rayMinExtent;
	ray.TMax = g_AppDataBuffer.rayMaxExtent;

	RayPayload payload;
	payload.color = float3(0.0f, 0.0f, 0.0f);

	TraceRay(g_AccelStruct, );

	g_OutputTexture[pixelPos] = float4(payload.color, 1.0f);
}

[shader("miss")]
void RayMiss(inout RayPayload payload)
{
	payload.color = float3(0.0f, 0.0f, 1.0f);
}

[shader("closesthit")]
void RayClosestHit(inout RayPayload payload, BuiltInTriangleIntersectionAttributes intersectionAttribs)
{
	payload.color = float3(1.0f, 0.0f, 0.0f);
}