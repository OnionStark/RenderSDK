#pragma once

#include "Common/Common.h"

class DXDevice;
class DXCommandList;
class DXCommandAllocator;
class DXRootSignature;
class DXPipelineState;
class DXDescriptorHeap;
class DXResource;

struct ClearVoxelGridInitParams
{
	DXDevice* m_pDevice;
	u16 m_NumGridCellsX;
	u16 m_NumGridCellsY;
	u16 m_NumGridCellsZ;
};

struct ClearVoxelGridRecordParams
{
	DXCommandList* m_pCommandList;
	DXCommandAllocator* m_pCommandAllocator;
	DXDescriptorHeap* m_pCBVSRVUAVDescriptorHeap;
	DXResource* m_pGridBuffer;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GridBufferUAVHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GridConfigCBVHandle;
};

class ClearVoxelGridRecorder
{
public:
	ClearVoxelGridRecorder(ClearVoxelGridInitParams* pParams);
	~ClearVoxelGridRecorder();

	void Record(ClearVoxelGridRecordParams* pParams);

private:
	DXRootSignature* m_pRootSignature;
	DXPipelineState* m_pPipelineState;

	u16 m_NumThreadGroupsX;
	u16 m_NumThreadGroupsY;
	u16 m_NumThreadGroupsZ;
};
