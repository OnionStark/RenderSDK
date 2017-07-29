#pragma once

#include "D3DWrapper/Common.h"

struct RenderEnv;
struct Viewport;
class CommandList;
class RootSignature;
class PipelineState;

class VisualizeVoxelGridPass
{
public:
	enum VoxelDataType
	{
		VoxelDataType_DiffuseColor = 0,
		VoxelDataType_Normal
	};

	struct InitParams
	{
		RenderEnv* m_pRenderEnv;
		DXGI_FORMAT m_RTVFormat;
		VoxelDataType m_VoxelDataType;
	};
	
	struct RenderParams
	{
		RenderEnv* m_pRenderEnv;
		CommandList* m_pCommandList;
		Viewport* m_pViewport;
	};

	VisualizeVoxelGridPass(InitParams* pParams);
	~VisualizeVoxelGridPass();

	void Record(RenderParams* pParams);

private:
	RootSignature* m_pRootSignature;
	PipelineState* m_pPipelineState;
};