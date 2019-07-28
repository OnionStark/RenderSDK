#pragma once

#include "Common/Application.h"

class GraphicsDevice;
class SwapChain;
class CommandQueue;
class CommandListPool;
class DescriptorHeap;
class RootSignature;
class PipelineState;
class StateObject;
class Buffer;
class ColorTexture;
class Fence;

struct HeapProperties;
struct RenderEnv;
struct VertexBufferView;
struct IndexBufferView;
struct Viewport;
struct Rect;

class DXApplication : public Application
{
public:
	DXApplication(HINSTANCE hApp);
	~DXApplication();

private:
	void OnInit() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;

	void InitRenderEnvironment();
	void BuildGeometryBuffers();
	void BuildAccelerationStructures();
	void CreateRootSignatures();
	void CreateStateObject();
	void CreateShaderTables();
	
private:
	enum { kNumBackBuffers = 3 };

	GraphicsDevice* m_pDevice;
	SwapChain* m_pSwapChain;
	CommandQueue* m_pCommandQueue;
	CommandListPool* m_pCommandListPool;
	DescriptorHeap* m_pShaderInvisibleRTVHeap;
	DescriptorHeap* m_pShaderInvisibleSRVHeap;
	Buffer* m_pVertexBuffer;
	Buffer* m_pIndexBuffer;
	Buffer* m_pBLASBuffer;
	Buffer* m_pTLASBuffer;
	Buffer* m_pInstanceBuffer;
	Buffer* m_pRayGenShaderTable;
	Buffer* m_pMissShaderTable;
	Buffer* m_pHitGroupTable;
	RootSignature* m_pRayGenLocalRootSignature;
	RootSignature* m_pEmptyLocalRootSignature;
	StateObject* m_pStateObject;
	HeapProperties* m_pDefaultHeapProps;
	HeapProperties* m_pUploadHeapProps;
	RenderEnv* m_pRenderEnv;
	Fence* m_pFence;
	Viewport* m_pViewport;
	Rect* m_pScissorRect;
	UINT m_BackBufferIndex;
	UINT64 m_FrameCompletionFenceValues[kNumBackBuffers];
};
