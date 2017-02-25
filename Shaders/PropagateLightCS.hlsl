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

static const float backFaceSolidAngle = 0.400669754f;
static const float sideFaceSolidAngle = 0.423431277f;

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

Texture3D g_PrevIntensityRCoeffsTexture : register(t0);
Texture3D g_PrevIntensityGCoeffsTexture : register(t1);
Texture3D g_PrevIntensityBCoeffsTexture : register(t2);

RWTexture3D<float4> g_IntensityRCoeffsTexture : register(u0);
RWTexture3D<float4> g_IntensityGCoeffsTexture : register(u1);
RWTexture3D<float4> g_IntensityBCoeffsTexture : register(u2);

void LoadIntensityCoeffs(int3 gridCell, inout SHSpectralCoeffs intensityCoeffs)
{
	if (IsCellOutsideGrid(g_GridConfig, gridCell))
	{
		intensityCoeffs.r = float4(0.0f, 0.0f, 0.0f, 0.0f);
		intensityCoeffs.g = float4(0.0f, 0.0f, 0.0f, 0.0f);
		intensityCoeffs.b = float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		intensityCoeffs.r = g_PrevIntensityRCoeffsTexture[gridCell];
		intensityCoeffs.g = g_PrevIntensityGCoeffsTexture[gridCell];
		intensityCoeffs.b = g_PrevIntensityBCoeffsTexture[gridCell];
	}
}

[numthreads(NUM_THREADS_X, NUM_THREADS_Y, NUM_THREADS_Z)]
void Main(int3 currCell : SV_DispatchThreadID)
{
	SHSpectralCoeffs accumIntensityCoeffs;
	accumIntensityCoeffs.r = g_PrevIntensityRCoeffsTexture[currCell];
	accumIntensityCoeffs.g = g_PrevIntensityGCoeffsTexture[currCell];
	accumIntensityCoeffs.b = g_PrevIntensityBCoeffsTexture[currCell];

	for (int neighborIndex = 0; neighborIndex < NUM_NEIGHBORS_PER_CELL; ++neighborIndex)
	{
		int3 neighborCell = currCell + g_Neighbors[neighborIndex].neighborOffset;

		SHSpectralCoeffs neighborIntensityCoeffs;
		LoadIntensityCoeffs(neighborCell, neighborIntensityCoeffs);

		for (int faceIndex = 0; faceIndex < NUM_FACES_PER_CELL; ++faceIndex)
		{
			CellFaceData faceData = g_Neighbors[neighborIndex].currCellFaces[faceIndex];
			
			float4 dirFromNeighborCenterCoeffs = SH(faceData.dirFromNeighborCenter);
			float3 intensityFromNeighbor;
			intensityFromNeighbor.r = dot(neighborIntensityCoeffs.r, dirFromNeighborCenterCoeffs);
			intensityFromNeighbor.g = dot(neighborIntensityCoeffs.g, dirFromNeighborCenterCoeffs);
			intensityFromNeighbor.b = dot(neighborIntensityCoeffs.b, dirFromNeighborCenterCoeffs);

			float3 fluxFromNeighbor = max(intensityFromNeighbor, 0.0f) * faceData.solidAngleFromNeightborCenter;

			float4 cosineCoeffs = g_FaceClampedCosineCoeffs[faceIndex];
			SHSpectralCoeffs intensityCoeffs;
			intensityCoeffs.r = fluxFromNeighbor.r * cosineCoeffs;
			intensityCoeffs.g = fluxFromNeighbor.g * cosineCoeffs;
			intensityCoeffs.b = fluxFromNeighbor.b * cosineCoeffs;

			accumIntensityCoeffs.r += intensityCoeffs.r;
			accumIntensityCoeffs.g += intensityCoeffs.g;
			accumIntensityCoeffs.b += intensityCoeffs.b;
		}
	}
		
	g_IntensityRCoeffsTexture[currCell] = accumIntensityCoeffs.r;
	g_IntensityGCoeffsTexture[currCell] = accumIntensityCoeffs.g;
	g_IntensityBCoeffsTexture[currCell] = accumIntensityCoeffs.b;
}
