#pragma once

#include "D3DWrapper/GraphicsResource.h"

struct RenderEnv;
struct Viewport;
class CommandList;
class RootSignature;
class PipelineState;

class FillDepthBufferWithMeshTypePass
{
public:
	struct ResourceStates
	{
		D3D12_RESOURCE_STATES m_MaterialIDTextureState;
		D3D12_RESOURCE_STATES m_MeshTypePerMaterialIDBufferState;
		D3D12_RESOURCE_STATES m_DepthTextureWithMeshTypeState;
	};

	struct InitParams
	{
		RenderEnv* m_pRenderEnv;
		ResourceStates m_InputResourceStates;
		ColorTexture* m_pMaterialIDTexture;
		Buffer* m_pMeshTypePerMaterialIDBuffer;
	};

	struct RenderParams
	{
		RenderEnv* m_pRenderEnv;
		CommandList* m_pCommandList;
		UINT m_NumMeshTypes;
		Viewport* m_pViewport;
	};

	FillDepthBufferWithMeshTypePass(InitParams* pParams);
	~FillDepthBufferWithMeshTypePass();

	void Record(RenderParams* pParams);
	const ResourceStates* GetOutputResourceStates() const { return &m_OutputResourceStates; }
	DepthTexture* GetDepthTextureWithMeshType() { return m_pDepthTextureWithMeshType; }

private:
	void InitResources(InitParams* pParams);
	void InitRootSignature(InitParams* pParams);
	void InitPipelineState(InitParams* pParams);
	void CreateResourceBarrierIfRequired(GraphicsResource* pResource, D3D12_RESOURCE_STATES currState, D3D12_RESOURCE_STATES requiredState);

private:
	RootSignature* m_pRootSignature;
	PipelineState* m_pPipelineState;
	DescriptorHandle m_SRVHeapStart;
	DescriptorHandle m_DSVHeapStart;
	std::vector<ResourceBarrier> m_ResourceBarriers;
	ResourceStates m_OutputResourceStates;
	DepthTexture* m_pDepthTextureWithMeshType;
};
