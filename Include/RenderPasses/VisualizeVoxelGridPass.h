#pragma once

#include "D3DWrapper/Common.h"

class CommandList;
class RootSignature;
class PipelineState;

struct RenderEnv;
struct BindingResourceList;
struct Viewport;

class VisualizeVoxelGridPass
{
public:
	struct InitParams
	{
		RenderEnv* m_pRenderEnv;
		DXGI_FORMAT m_RTVFormat;
	};
	
	struct RenderParams
	{
		RenderEnv* m_pRenderEnv;
		CommandList* m_pCommandList;
		BindingResourceList* m_pResources;
		Viewport* m_pViewport;
	};

	VisualizeVoxelGridPass(InitParams* pParams);
	~VisualizeVoxelGridPass();

	void Record(RenderParams* pParams);

private:
	RootSignature* m_pRootSignature;
	PipelineState* m_pPipelineState;
};