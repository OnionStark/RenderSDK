#pragma once

#include "D3DWrapper/GraphicsResource.h"

struct RenderEnv;
class Buffer;
class ColorTexture;
class CommandList;
class RootSignature;
class PipelineState;

class CubeMapToSHCoefficientsPass
{
public:
	enum { kNumSHCoefficients = 9 };

	struct ResourceStates
	{
		D3D12_RESOURCE_STATES m_CubeMapState;
		D3D12_RESOURCE_STATES m_SHCoefficientBufferState;
	};

	struct InitParams
	{
		const char* m_pName = nullptr;
		RenderEnv* m_pRenderEnv = nullptr;
		u32 m_CubeMapFaceSize = 0;
	};

	struct RenderParams
	{
		RenderEnv* m_pRenderEnv = nullptr;
		CommandList* m_pCommandList = nullptr;
		ColorTexture* m_pCubeMap = nullptr;
		Buffer* m_pSHCoefficientBuffer = nullptr;
		ResourceStates m_InputResourceStates;
	};

	CubeMapToSHCoefficientsPass(InitParams* pParams);
	~CubeMapToSHCoefficientsPass();

	void Record(RenderParams* pParams);
	const ResourceStates* GetOutputResourceStates() const { return &m_OutputResourceStates; }

private:
	void InitResources(InitParams* pParams);

	void InitIntegrateRootSignature(InitParams* pParams);
	void InitIntegratePipelineState(InitParams* pParams);

	void InitMergeRootSignature(InitParams* pParams);
	void InitMergePipelineState(InitParams* pParams);

private:
	std::string m_Name;
	u32 m_CubeMapFaceSize = 0;

	RootSignature* m_pIntegrateRootSignature = nullptr;
	PipelineState* m_IntegratePipelineStates[kNumSHCoefficients];
	DescriptorHandle m_IntegrateSRVHeapStart;

	RootSignature* m_pMergeRootSignature = nullptr;
	PipelineState* m_pMergePipelineState = nullptr;
	DescriptorHandle m_MergeSRVHeapStart;

	ResourceStates m_OutputResourceStates;
	Buffer* m_pSumPerRowBuffer = nullptr;
};
