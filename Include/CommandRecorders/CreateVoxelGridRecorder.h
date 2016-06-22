#pragma once

#include "Common/Common.h"

class DXDevice;
class DXRootSignature;
class DXPipelineState;
class DXCommandList;
class DXCommandAllocator;
class DXCommandSignature;
class DXBuffer;
class MeshBatch;

struct DXViewport;
struct DXRenderEnvironment;
struct DXBindingResourceList;

class CreateVoxelGridRecorder
{
public:
	struct InitParams
	{
		DXRenderEnvironment* m_pRenderEnv;
		MeshBatch* m_pMeshBatch;
	};

	struct RenderPassParams
	{
		DXRenderEnvironment* m_pRenderEnv;
		DXCommandList* m_pCommandList;
		DXCommandAllocator* m_pCommandAllocator;
		DXBindingResourceList* m_pResources;
		DXViewport* m_pViewport;
		MeshBatch* m_pMeshBatch;
		DXBuffer* m_pDrawMeshCommandBuffer;
		DXBuffer* m_pNumDrawMeshesBuffer;
	};

	CreateVoxelGridRecorder(InitParams* pParams);
	~CreateVoxelGridRecorder();

	void Record(RenderPassParams* pParams);

private:
	DXRootSignature* m_pRootSignature;
	DXPipelineState* m_pPipelineState;
	DXCommandSignature* m_pCommandSignature;
};