#pragma once

#include "Common/Application.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Sphere.h"

struct HeapProperties;
struct RenderEnv;
struct Viewport;
struct Frustum;

class GraphicsDevice;
class SwapChain;
class CommandQueue;
class CommandList;
class CommandListPool;
class DescriptorHeap;
class ColorTexture;
class DepthTexture;
class Buffer;
class Fence;
class Camera;
class GeometryBuffer;
class MeshRenderResources;
class MaterialRenderResources;
class DownscaleAndReprojectDepthPass;
class FrustumMeshCullingPass;
class FillVisibilityBufferPass;
class CreateMainDrawCommandsPass;
class CreateFalseNegativeDrawCommandsPass;
class CreateShadowMapCommandsPass;
class FillMeshTypeDepthBufferPass;
class RenderGBufferPass;
class TiledLightCullingPass;
class TiledShadingPass;
class CalcShadingRectanglesPass;
class CreateVoxelizeCommandsPass;
class VisualizeTexturePass;
class VisualizeVoxelReflectancePass;
class VoxelizePass;
class Scene;
class PointLight;
class SpotLight;

struct PointLightData
{
	Vector3f m_Color;
	Vector3f m_WorldSpacePos;
	Sphere m_WorldBounds;
	f32 m_AttenStartRange;
	f32 m_AttenEndRange;
	f32 m_AffectedScreenArea;
	Vector2f m_ShadowMapTileTopLeftPos;
	f32 m_ShadowMapTileSize;
};

struct SpotLightData
{
	Vector3f m_Color;
	Vector3f m_WorldSpacePos;
	Vector3f m_WorldSpaceDir;
	Sphere m_WorldBounds;
	f32 m_AttenStartRange;
	f32 m_AttenEndRange;
	f32 m_CosHalfInnerConeAngle;
	f32 m_CosHalfOuterConeAngle;
	f32 m_AffectedScreenArea;
	Vector2f m_ShadowMapTileTopLeftPos;
	f32 m_ShadowMapTileSize;
};

//#define DEBUG_RENDER_PASS

class DXApplication : public Application
{
public:
	enum class DisplayResult
	{
		ShadingResult,
		DepthBuffer,
		ReprojectedDepthBuffer,
		NormalBuffer,
		TexCoordBuffer,
		DepthBufferWithMeshType,
		VoxelRelectance,
		Unknown
	};
	
	DXApplication(HINSTANCE hApp);
	~DXApplication();

private:
	void OnInit() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;
	void OnKeyDown(UINT8 key) override;
	void OnKeyUp(UINT8 key) override;

	void InitRenderEnv(UINT backBufferWidth, UINT backBufferHeight);
	void InitScene(UINT backBufferWidth, UINT backBufferHeight);

	void InitDownscaleAndReprojectDepthPass();
	CommandList* RecordDownscaleAndReprojectDepthPass();

	CommandList* RecordPreRenderPass();

	void InitFrustumMeshCullingPass();
	CommandList* RecordFrustumMeshCullingPass();
	
	void InitFillVisibilityBufferMainPass();
	CommandList* RecordFillVisibilityBufferMainPass();

	void InitCreateMainDrawCommandsPass();
	CommandList* RecordCreateMainDrawCommandsPass();

	void InitRenderGBufferMainPass(UINT bufferWidth, UINT bufferHeight);
	CommandList* RecordRenderGBufferMainPass();

	void InitFillVisibilityBufferFalseNegativePass();
	CommandList* RecordFillVisibilityBufferFalseNegativePass();

	void InitCreateFalseNegativeDrawCommandsPass();
	CommandList* RecordCreateFalseNegativeDrawCommandsPass();

	void InitRenderGBufferFalseNegativePass(UINT bufferWidth, UINT bufferHeight);
	CommandList* RecordRenderGBufferFalseNegativePass();
	
	void InitCalcShadingRectanglesPass();
	CommandList* RecordCalcShadingRectanglesPass();

	void InitFillMeshTypeDepthBufferPass();
	CommandList* RecordFillMeshTypeDepthBufferPass();

	CommandList* RecordUploadLightDataPass();

	void InitTiledLightCullingPass();
	CommandList* RecordTiledLightCullingPass();

	void InitCreateShadowMapCommandsPass();
	CommandList* RecordCreateShadowMapCommandsPass();

	void InitCreateVoxelizeCommandsPass();
	CommandList* RecordCreateVoxelizeCommandsPass();

	void InitVoxelizePass();
	CommandList* RecordVoxelizePass();
		
	void InitTiledShadingPass();
	CommandList* RecordTiledShadingPass();

	void InitVisualizeDepthBufferPass();
	CommandList* RecordVisualizeDepthBufferPass();

	void InitVisualizeReprojectedDepthBufferPass();
	CommandList* RecordVisualizeReprojectedDepthBufferPass();

	void InitVisualizeNormalBufferPass();
	CommandList* RecordVisualizeNormalBufferPass();

	void InitVisualizeTexCoordBufferPass();
	CommandList* RecordVisualizeTexCoordBufferPass();

	void InitVisualizeDepthBufferWithMeshTypePass();
	CommandList* RecordVisualizeDepthBufferWithMeshTypePass();

	void InitVisualizeAccumLightPass();
	CommandList* RecordVisualizeAccumLightPass();

	void InitVisualizeVoxelReflectancePass();
	CommandList* RecordVisualizeVoxelReflectancePass();

	CommandList* RecordVisualizeDisplayResultPass();
	CommandList* RecordPostRenderPass();

	void SetupPointLightDataForUpload(const Frustum& cameraWorldFrustum);
	void SetupSpotLightDataForUpload(const Frustum& cameraWorldFrustum);
			
	void UpdateDisplayResult(DisplayResult displayResult);
		
#ifdef DEBUG_RENDER_PASS
	void OuputDebugRenderPassResult();
#endif // DEBUG_RENDER_PASS
	
private:
	enum { kNumBackBuffers = 3 };
		
	DisplayResult m_DisplayResult = DisplayResult::Unknown;
	
	GraphicsDevice* m_pDevice = nullptr;
	SwapChain* m_pSwapChain = nullptr;
	CommandQueue* m_pCommandQueue = nullptr;
	CommandListPool* m_pCommandListPool = nullptr;
	HeapProperties* m_pUploadHeapProps = nullptr;
	HeapProperties* m_pDefaultHeapProps = nullptr;
	HeapProperties* m_pReadbackHeapProps = nullptr;
	DescriptorHeap* m_pShaderInvisibleRTVHeap = nullptr;
	DescriptorHeap* m_pShaderInvisibleDSVHeap = nullptr;
	DescriptorHeap* m_pShaderInvisibleSRVHeap = nullptr;
	DescriptorHeap* m_pShaderInvisibleSamplerHeap = nullptr;
	DescriptorHeap* m_pShaderVisibleSRVHeap = nullptr;
	DescriptorHeap* m_pShaderVisibleSamplerHeap = nullptr;
	DepthTexture* m_pDepthTexture = nullptr;
	ColorTexture* m_pAccumLightTexture = nullptr;
	Viewport* m_pBackBufferViewport = nullptr;
	RenderEnv* m_pRenderEnv = nullptr;
	Fence* m_pFence = nullptr;
	UINT64 m_FrameCompletionFenceValues[kNumBackBuffers] = {0, 0, 0};
	UINT m_BackBufferIndex = 0;

	Camera* m_pCamera = nullptr;
	MeshRenderResources* m_pMeshRenderResources = nullptr;
	MaterialRenderResources* m_pMaterialRenderResources = nullptr;
	GeometryBuffer* m_pGeometryBuffer = nullptr;
	DownscaleAndReprojectDepthPass* m_pDownscaleAndReprojectDepthPass = nullptr;
	FrustumMeshCullingPass* m_pFrustumMeshCullingPass = nullptr;
	FillVisibilityBufferPass* m_pFillVisibilityBufferMainPass = nullptr;
	CreateMainDrawCommandsPass* m_pCreateMainDrawCommandsPass = nullptr;
	RenderGBufferPass* m_pRenderGBufferMainPass = nullptr;
	FillVisibilityBufferPass* m_pFillVisibilityBufferFalseNegativePass = nullptr;
	CreateFalseNegativeDrawCommandsPass* m_pCreateFalseNegativeDrawCommandsPass = nullptr;
	RenderGBufferPass* m_pRenderGBufferFalseNegativePass = nullptr;
	FillMeshTypeDepthBufferPass* m_pFillMeshTypeDepthBufferPass = nullptr;
	CalcShadingRectanglesPass* m_pCalcShadingRectanglesPass = nullptr;
	CreateShadowMapCommandsPass* m_pCreateShadowMapCommandsPass = nullptr;
	CreateVoxelizeCommandsPass* m_pCreateVoxelizeCommandsPass = nullptr;
	VoxelizePass* m_pVoxelizePass = nullptr;
	TiledLightCullingPass* m_pTiledLightCullingPass = nullptr;
	TiledShadingPass* m_pTiledShadingPass = nullptr;
	VisualizeTexturePass* m_pVisualizeAccumLightPasses[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	VisualizeTexturePass* m_VisualizeDepthBufferPasses[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	VisualizeTexturePass* m_VisualizeReprojectedDepthBufferPasses[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	VisualizeTexturePass* m_VisualizeNormalBufferPasses[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	VisualizeTexturePass* m_VisualizeTexCoordBufferPasses[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	VisualizeTexturePass* m_VisualizeDepthBufferWithMeshTypePasses[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	VisualizeVoxelReflectancePass* m_VisualizeVoxelReflectancePasses[kNumBackBuffers] = {nullptr, nullptr, nullptr};

	u32 m_NumPointLights = 0;
	PointLightData* m_pPointLights = nullptr;
	
	u32 m_NumVisiblePointLights = 0;
	PointLightData** m_ppVisiblePointLights = nullptr;

	u32 m_NumSpotLights = 0;
	SpotLightData* m_pSpotLights = nullptr;

	u32 m_NumVisibleSpotLights = 0;
	SpotLightData** m_ppVisibleSpotLights = nullptr;

	Buffer* m_pVisiblePointLightWorldBoundsBuffer = nullptr;
	Buffer* m_pVisiblePointLightPropsBuffer = nullptr;

	Buffer* m_pVisibleSpotLightWorldBoundsBuffer = nullptr;
	Buffer* m_pVisibleSpotLightPropsBuffer = nullptr;

	Buffer* m_pUploadAppDataBuffers[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	void* m_pUploadAppData[kNumBackBuffers] = {nullptr, nullptr, nullptr};

	Buffer* m_pUploadVisiblePointLightWorldBoundsBuffers[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	void* m_pUploadVisiblePointLightWorldBounds[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	Buffer* m_pUploadVisiblePointLightPropsBuffers[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	void* m_pUploadVisiblePointLightProps[kNumBackBuffers] = {nullptr, nullptr, nullptr};

	Buffer* m_pUploadVisibleSpotLightWorldBoundsBuffers[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	void* m_pUploadVisibleSpotLightWorldBounds[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	Buffer* m_pUploadVisibleSpotLightPropsBuffers[kNumBackBuffers] = {nullptr, nullptr, nullptr};
	void* m_pUploadVisibleSpotLightProps[kNumBackBuffers] = {nullptr, nullptr, nullptr};
};