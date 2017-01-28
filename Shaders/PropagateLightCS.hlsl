#define NUM_NEIGHBORS_PER_CELL		6
#define NUM_FACES_PER_CELL			6

#include "SphericalHarmonics.hlsl"
#include "VoxelGrid.hlsl"

struct CellFaceData
{
	float3 dirFromNeighborCenter;
	float solidAngleFromNeightborCenter;
};

struct NeighborCellData
{
	int3 neighborOffset;
	CellFaceData currCellFaces[NUM_FACES_PER_CELL];
};

static const float backFaceSolidAngle = 0.5f;
static const float sideFaceSolidAngle = 0.2f;

static const float3 cellCenter = float3(0.5f, 0.5f, 0.5f);

static const float3 neighborPosXOffset = float3( 1.0f,  0.0f,  0.0f);
static const float3 neighborNegXOffset = float3(-1.0f,  0.0f,  0.0f);
static const float3 neighborPosYOffset = float3( 0.0f,  1.0f,  0.0f);
static const float3 neighborNegYOffset = float3( 0.0f, -1.0f,  0.0f);
static const float3 neighborPosZOffset = float3( 0.0f,  0.0f,  1.0f);
static const float3 neighborNegZOffset = float3( 0.0f,  0.0f, -1.0f);

static const float3 neighborCenterPosX = cellCenter + neighborPosXOffset;
static const float3 neighborCenterNegX = cellCenter + neighborNegXOffset;
static const float3 neighborCenterPosY = cellCenter + neighborPosYOffset;
static const float3 neighborCenterNegY = cellCenter + neighborNegYOffset;
static const float3 neighborCenterPosZ = cellCenter + neighborPosZOffset;
static const float3 neighborCenterNegZ = cellCenter + neighborNegZOffset;

static const float3 faceCenterPosX = cellCenter + 0.5f * neighborPosXOffset;
static const float3 faceCenterNegX = cellCenter + 0.5f * neighborNegXOffset;
static const float3 faceCenterPosY = cellCenter + 0.5f * neighborPosYOffset;
static const float3 faceCenterNegY = cellCenter + 0.5f * neighborNegYOffset;
static const float3 faceCenterPosZ = cellCenter + 0.5f * neighborPosZOffset;
static const float3 faceCenterNegZ = cellCenter + 0.5f * neighborNegZOffset;

static const NeighborCellData g_Neighbors[NUM_NEIGHBORS_PER_CELL] =
{
	{
		neighborPosXOffset,
		{
			{normalize(faceCenterPosX - neighborCenterPosX), 0.0f},
			{normalize(faceCenterNegX - neighborCenterPosX), backFaceSolidAngle},
			{normalize(faceCenterPosY - neighborCenterPosX), sideFaceSolidAngle},
			{normalize(faceCenterNegY - neighborCenterPosX), sideFaceSolidAngle},
			{normalize(faceCenterPosZ - neighborCenterPosX), sideFaceSolidAngle},
			{normalize(faceCenterNegZ - neighborCenterPosX), sideFaceSolidAngle}
		}
	},
	{
		neighborNegXOffset,
		{
			{normalize(faceCenterPosX - neighborCenterNegX), backFaceSolidAngle},
			{normalize(faceCenterNegX - neighborCenterNegX), 0.0f},
			{normalize(faceCenterPosY - neighborCenterNegX), sideFaceSolidAngle},
			{normalize(faceCenterNegY - neighborCenterNegX), sideFaceSolidAngle},
			{normalize(faceCenterPosZ - neighborCenterNegX), sideFaceSolidAngle},
			{normalize(faceCenterNegZ - neighborCenterNegX), sideFaceSolidAngle}
		}
	},
	{
		neighborPosYOffset,
		{
			{normalize(faceCenterPosX - neighborCenterPosY), sideFaceSolidAngle},
			{normalize(faceCenterNegX - neighborCenterPosY), sideFaceSolidAngle},
			{normalize(faceCenterPosY - neighborCenterPosY), 0.0f},
			{normalize(faceCenterNegY - neighborCenterPosY), backFaceSolidAngle},
			{normalize(faceCenterPosZ - neighborCenterPosY), sideFaceSolidAngle},
			{normalize(faceCenterNegZ - neighborCenterPosY), sideFaceSolidAngle}
		}
	},
	{
		neighborNegYOffset,
		{
			{normalize(faceCenterPosX - neighborCenterNegY), sideFaceSolidAngle},
			{normalize(faceCenterNegX - neighborCenterNegY), sideFaceSolidAngle},
			{normalize(faceCenterPosY - neighborCenterNegY), backFaceSolidAngle},
			{normalize(faceCenterNegY - neighborCenterNegY), 0.0f},
			{normalize(faceCenterPosZ - neighborCenterNegY), sideFaceSolidAngle},
			{normalize(faceCenterNegZ - neighborCenterNegY), sideFaceSolidAngle}
		}
	},
	{
		neighborPosZOffset,
		{
			{normalize(faceCenterPosX - neighborCenterPosZ), sideFaceSolidAngle},
			{normalize(faceCenterNegX - neighborCenterPosZ), sideFaceSolidAngle},
			{normalize(faceCenterPosY - neighborCenterPosZ), sideFaceSolidAngle},
			{normalize(faceCenterNegY - neighborCenterPosZ), sideFaceSolidAngle},
			{normalize(faceCenterPosZ - neighborCenterPosZ), 0.0f},
			{normalize(faceCenterNegZ - neighborCenterPosZ), backFaceSolidAngle}
		}
	},
	{
		neighborNegZOffset,
		{
			{normalize(faceCenterPosX - neighborCenterNegZ), sideFaceSolidAngle},
			{normalize(faceCenterNegX - neighborCenterNegZ), sideFaceSolidAngle},
			{normalize(faceCenterPosY - neighborCenterNegZ), sideFaceSolidAngle},
			{normalize(faceCenterNegY - neighborCenterNegZ), sideFaceSolidAngle},
			{normalize(faceCenterPosZ - neighborCenterNegZ), backFaceSolidAngle},
			{normalize(faceCenterNegZ - neighborCenterNegZ), 0.0f}
		}
	}
};

static const float4 g_FaceClampedCosineCoeffs[NUM_FACES_PER_CELL] =
{
	SHProjectClampedCosine(float3( 1.0f,  0.0f,  0.0f)),
	SHProjectClampedCosine(float3(-1.0f,  0.0f,  0.0f)),
	SHProjectClampedCosine(float3( 0.0f,  1.0f,  0.0f)),
	SHProjectClampedCosine(float3( 0.0f, -1.0f,  0.0f)),
	SHProjectClampedCosine(float3( 0.0f,  0.0f,  1.0f)),
	SHProjectClampedCosine(float3( 0.0f,  0.0f, -1.0f))
};

cbuffer GridConfigDataBuffer : register(b0)
{
	GridConfig g_GridConfig;
}

Texture3D g_FluxRCoeffsTexture : register(t0);
Texture3D g_FluxGCoeffsTexture : register(t1);
Texture3D g_FluxBCoeffsTexture : register(t2);

RWTexture3D g_GatheredFluxRCoeffsTexture : register(u0);
RWTexture3D g_GatheredFluxGCoeffsTexture : register(u1);
RWTexture3D g_GatheredFluxBCoeffsTexture : register(u2);

void LoadGridCellFluxCoeffs(int3 gridCell, inout SHSpectralCoeffs fluxCoeffs)
{
	if (IsCellOutsideGrid(g_GridConfig, gridCell))
	{
		fluxCoeffs.r = float4(0.0f, 0.0f, 0.0f, 0.0f);
		fluxCoeffs.g = float4(0.0f, 0.0f, 0.0f, 0.0f);
		fluxCoeffs.b = float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		fluxCoeffs.r = g_FluxRCoeffsTexture[gridCell];
		fluxCoeffs.g = g_FluxGCoeffsTexture[gridCell];
		fluxCoeffs.b = g_FluxBCoeffsTexture[gridCell];
	}
}

[numthreads(NUM_THREADS_X, NUM_THREADS_Y, NUM_THREADS_Z)]
void Main(int3 currCell : SV_DispatchThreadID)
{
	SHSpectralCoeffs gatheredFluxCoeffs;
	gatheredFluxCoeffs.r = float4(0.0f, 0.0f, 0.0f, 0.0f);
	gatheredFluxCoeffs.g = float4(0.0f, 0.0f, 0.0f, 0.0f);
	gatheredFluxCoeffs.b = float4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int neighborIndex = 0; neighborIndex < NUM_NEIGHBORS_PER_CELL; ++neighborIndex)
	{
		int3 neighborCell = currCell + g_Neighbors[neighborIndex].neighborOffset;

		SHSpectralCoeffs neighborFluxCoeffs;
		LoadGridCellFluxCoeffs(neighborCell, neighborFluxCoeffs);

		for (int faceIndex = 0; faceIndex < NUM_FACES_PER_CELL; ++faceIndex)
		{
			CellFaceData faceData = g_Neighbors[neighborIndex].currCellFaces[faceIndex];
			float4 dirFromNeighborCoeffs = SH(faceData.dirFromNeighborCenter);
			
			float3 fluxFromNeighborToFace;
			fluxFromNeighborToFace.r = dot(neighborFluxCoeffs.r, dirFromNeighborCoeffs);
			fluxFromNeighborToFace.g = dot(neighborFluxCoeffs.g, dirFromNeighborCoeffs);
			fluxFromNeighborToFace.b = dot(neighborFluxCoeffs.b, dirFromNeighborCoeffs);
			fluxFromNeighborToFace = max(0.0f, fluxFromNeighborToFace) * faceData.solidAngleFromNeightborCenter;

			float4 dirToFaceCoeffs = g_FaceClampedCosineSHCoeffs[faceIndex];
			gatheredFluxSHCoeffs.r += fluxFromNeighborToFace.r * dirToFaceCoeffs;
			gatheredFluxSHCoeffs.g += fluxFromNeighborToFace.g * dirToFaceCoeffs;
			gatheredFluxSHCoeffs.b += fluxFromNeighborToFace.b * dirToFaceCoeffs;
		}
	}

	g_GatheredFluxRCoeffsTexture[currCell] = gatheredFluxCoeffs.r;
	g_GatheredFluxGCoeffsTexture[currCell] = gatheredFluxCoeffs.g;
	g_GatheredFluxBCoeffsTexture[currCell] = gatheredFluxCoeffs.b;
}
