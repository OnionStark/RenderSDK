#include "RenderPasses/VisualizeTexturePass.h"
#include "D3DWrapper/CommandList.h"
#include "D3DWrapper/RenderEnv.h"
#include "D3DWrapper/PipelineState.h"
#include "D3DWrapper/Profiler.h"
#include "D3DWrapper/RootSignature.h"
#include "D3DWrapper/GraphicsDevice.h"
#include "D3DWrapper/GraphicsUtils.h"

namespace
{
	enum RootParams
	{
		kRootCBVParam = 0,
		kRootSRVTableParam,
		kNumRootParams
	};
}

VisualizeTexturePass::VisualizeTexturePass(InitParams* pParams)
	: m_Name(pParams->m_pName)
{
	InitResources(pParams);
	InitRootSignature(pParams);
	InitPipelineState(pParams);
}

VisualizeTexturePass::~VisualizeTexturePass()
{
	SafeDelete(m_pPipelineState);
	SafeDelete(m_pRootSignature);
}

void VisualizeTexturePass::Record(RenderParams* pParams)
{
	RenderEnv* pRenderEnv = pParams->m_pRenderEnv;
	CommandList* pCommandList = pParams->m_pCommandList;
	Profiler* pProfiler = pRenderEnv->m_pProfiler;

	pCommandList->Begin(m_pPipelineState);
#ifdef ENABLE_GPU_PROFILING
	u32 profileIndex = pProfiler->StartProfile(pCommandList, m_Name.c_str());
#endif // ENABLE_GPU_PROFILING

	pCommandList->SetGraphicsRootSignature(m_pRootSignature);

	if (!m_ResourceBarriers.empty())
		pCommandList->ResourceBarrier((UINT)m_ResourceBarriers.size(), m_ResourceBarriers.data());

	pCommandList->SetDescriptorHeaps(pRenderEnv->m_pShaderVisibleSRVHeap);
	pCommandList->SetGraphicsRootConstantBufferView(kRootCBVParam, pParams->m_pAppDataBuffer);
	pCommandList->SetGraphicsRootDescriptorTable(kRootSRVTableParam, m_SRVHeapStart);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapStart = m_RTVHeapStart;
	pCommandList->OMSetRenderTargets(1, &rtvHeapStart);

	pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pCommandList->IASetVertexBuffers(0, 1, nullptr);
	pCommandList->IASetIndexBuffer(nullptr);

	Rect scissorRect(ExtractRect(pParams->m_pViewport));
	pCommandList->RSSetViewports(1, pParams->m_pViewport);
	pCommandList->RSSetScissorRects(1, &scissorRect);

	pCommandList->DrawInstanced(3, 1, 0, 0);

#ifdef ENABLE_GPU_PROFILING
	pProfiler->EndProfile(pCommandList, profileIndex);
#endif // ENABLE_GPU_PROFILING
	pCommandList->End();
}

void VisualizeTexturePass::InitResources(InitParams* pParams)
{
	RenderEnv* pRenderEnv = pParams->m_pRenderEnv;
	
	m_OutputResourceStates.m_InputTextureState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	m_OutputResourceStates.m_BackBufferState = D3D12_RESOURCE_STATE_RENDER_TARGET;
	
	assert(m_ResourceBarriers.empty());
	AddResourceBarrierIfRequired(pParams->m_pInputTexture,
		pParams->m_InputResourceStates.m_InputTextureState,
		m_OutputResourceStates.m_InputTextureState);

	AddResourceBarrierIfRequired(pParams->m_pBackBuffer,
		pParams->m_InputResourceStates.m_BackBufferState,
		m_OutputResourceStates.m_BackBufferState);

	m_SRVHeapStart = pRenderEnv->m_pShaderVisibleSRVHeap->Allocate();
	pRenderEnv->m_pDevice->CopyDescriptor(m_SRVHeapStart,
		pParams->m_InputTextureSRV, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	m_RTVHeapStart = pParams->m_pBackBuffer->GetRTVHandle();
}

void VisualizeTexturePass::InitRootSignature(InitParams* pParams)
{
	assert(m_pRootSignature == nullptr);

	D3D12_ROOT_PARAMETER rootParams[kNumRootParams];
	rootParams[kRootCBVParam] = RootCBVParameter(0, D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_DESCRIPTOR_RANGE descriptorRanges[] = {SRVDescriptorRange(1, 0)};
	rootParams[kRootSRVTableParam] = RootDescriptorTableParameter(ARRAYSIZE(descriptorRanges), descriptorRanges, D3D12_SHADER_VISIBILITY_PIXEL);

	StaticSamplerDesc samplerDesc(StaticSamplerDesc::Point, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	RootSignatureDesc rootSignatureDesc(kNumRootParams, rootParams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	m_pRootSignature = new RootSignature(pParams->m_pRenderEnv->m_pDevice, &rootSignatureDesc, L"VisualizeTexturePass::m_pRootSignature");
}

void VisualizeTexturePass::InitPipelineState(InitParams* pParams)
{
	assert(m_pRootSignature != nullptr);
	assert(m_pPipelineState == nullptr);
	
	std::string textureTypeStr = std::to_string(pParams->m_TextureType);
	const ShaderMacro shaderDefines[] =
	{
		ShaderMacro("TEXTURE_TYPE", textureTypeStr.c_str()),
		ShaderMacro()
	};

	Shader vertexShader(L"Shaders//FullScreenTriangleVS.hlsl", "Main", "vs_4_0");
	Shader pixelShader(L"Shaders//VisualizeTexturePS.hlsl", "Main", "ps_4_0", shaderDefines);

	GraphicsPipelineStateDesc pipelineStateDesc;
	pipelineStateDesc.SetRootSignature(m_pRootSignature);
	pipelineStateDesc.SetVertexShader(&vertexShader);
	pipelineStateDesc.SetPixelShader(&pixelShader);
	pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineStateDesc.SetRenderTargetFormat(GetRenderTargetViewFormat(pParams->m_pBackBuffer->GetFormat()));

	m_pPipelineState = new PipelineState(pParams->m_pRenderEnv->m_pDevice, &pipelineStateDesc, L"VisualizeTexturePass::m_pPipelineState");
}

void VisualizeTexturePass::AddResourceBarrierIfRequired(GraphicsResource* pResource, D3D12_RESOURCE_STATES currState, D3D12_RESOURCE_STATES requiredState)
{
	if (currState != requiredState)
		m_ResourceBarriers.emplace_back(pResource, currState, requiredState);
}
