#include "RenderPasses/ShadowRenderer.h"
#include "D3DWrapper/RenderEnv.h"
#include "D3DWrapper/CommandSignature.h"
#include "Math/Frustum.h"
#include "Math/OverlapTest.h"
#include "Scene/Light.h"
#include "Scene/MeshBatch.h"

namespace
{
	struct ShadowMapCommand
	{
		UINT m_InstanceOffset;
		DrawIndexedArguments m_Args;
	};

	void CreateRenderCommands(std::vector<u32>& meshInstanceIndices,
		std::vector<ShadowMapCommand>& renderCommands,
		const Frustum& lightWorldFrustum,
		const MeshBatch* pMeshBatch)
	{
		const u32 numMeshes = pMeshBatch->GetNumMeshes();
		const MeshInfo* meshInfos = pMeshBatch->GetMeshInfos();
		const AxisAlignedBox* meshInstanceWorldAABBs = pMeshBatch->GetMeshInstanceWorldAABBs();

		for (u32 meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			u32 numVisibleMeshInstances = 0;

			const MeshInfo& meshInfo = meshInfos[meshIndex];
			const u32 meshLastInstanceIndex = meshInfo.m_InstanceOffset + meshInfo.m_InstanceCount;

			for (u32 meshInstanceIndex = meshInfo.m_InstanceOffset; meshInstanceIndex < meshLastInstanceIndex; ++meshInstanceIndex)
			{
				const AxisAlignedBox& meshInstanceWorldAABB = meshInstanceWorldAABBs[meshInstanceIndex];
				if (TestAABBAgainstFrustum(lightWorldFrustum, meshInstanceWorldAABB))
				{
					++numVisibleMeshInstances;
					meshInstanceIndices.push_back(meshInstanceIndex);
				}
			}

			if (numVisibleMeshInstances > 0)
			{
				ShadowMapCommand shadowMapCommand;
				shadowMapCommand.m_InstanceOffset = meshInstanceIndices.size() - numVisibleMeshInstances;
				shadowMapCommand.m_Args.m_IndexCountPerInstance = meshInfo.m_IndexCountPerInstance;
				shadowMapCommand.m_Args.m_InstanceCount = numVisibleMeshInstances;
				shadowMapCommand.m_Args.m_StartIndexLocation = meshInfo.m_StartIndexLocation;
				shadowMapCommand.m_Args.m_BaseVertexLocation = meshInfo.m_BaseVertexLocation;
				shadowMapCommand.m_Args.m_StartInstanceLocation = 0;

				renderCommands.push_back(shadowMapCommand);
			}
		}
	}
}

ShadowRenderer::ShadowRenderer(InitParams* pParams)
{
	InitResources(pParams);

	CreateSpotLightRenderStaticGeometryCommands(pParams->m_NumSpotLights, pParams->m_ppSpotLights, pParams->m_pMeshBatch);
	CreatePointLightRenderStaticGeometryCommands(pParams->m_NumPointLights, pParams->m_ppPointLights, pParams->m_pMeshBatch);
}

ShadowRenderer::~ShadowRenderer()
{
	SafeDelete(m_pActiveShadowMaps);
	SafeDelete(m_pStaticCache);
	SafeDelete(m_pESMStaleCache);
	SafeDelete(m_pRenderStaticGeometryCommands);
}

void ShadowRenderer::InitResources(InitParams* pParams)
{
	RenderEnv* pRenderEnv = pParams->m_pRenderEnv;
	DepthStencilValue optimizedClearDepth(1.0f);

	assert(m_pActiveShadowMaps == nullptr);
	assert(false && "fix format to D16");
	DepthTexture2DDesc activeShadowMapsDesc(DXGI_FORMAT_UNKNOWN, pParams->m_ShadowMapSize, pParams->m_ShadowMapSize,
		true/*createDSV*/, true/*createSRV*/, pParams->m_NumActiveShadowMaps);
	m_pActiveShadowMaps = new DepthTexture(pRenderEnv, pRenderEnv->m_pDefaultHeapProps, &activeShadowMapsDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, &optimizedClearDepth, L"ShadowRenderer::m_pActiveShadowMaps");

	assert(m_pStaticCache == nullptr);
	assert(false && "fix format to D16");
	DepthTexture2DDesc staticCacheDesc(DXGI_FORMAT_UNKNOWN, pParams->m_ShadowMapSize, pParams->m_ShadowMapSize,
		true/*createDSV*/, true/*createSRV*/, pParams->m_NumShadowMapsInStaticCache);
	m_pStaticCache = new DepthTexture(pRenderEnv, pRenderEnv->m_pDefaultHeapProps, &staticCacheDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, &optimizedClearDepth, L"ShadowRenderer::m_pStaticCache");

	const u32 ESMShadowMapSize = pParams->m_ShadowMapSize / 2;
	
	assert(m_pESMStaleCache == nullptr);
	assert(false && "fix format to 16 bit UNORM");
	ColorTexture2DDesc ESMStaleCacheDesc(DXGI_FORMAT_UNKNOWN, ESMShadowMapSize, ESMShadowMapSize,
		false/*createRTV*/, true/*createSRV*/, true/*createUAV*/, pParams->m_NumShadowMapsInStaleCache);
	m_pESMStaleCache = new ColorTexture(pRenderEnv, pRenderEnv->m_pDefaultHeapProps, &ESMStaleCacheDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr/*optimizedClearColor*/, L"ShadowRenderer::m_pESMStaleCache");
}

void ShadowRenderer::CreateSpotLightRenderStaticGeometryCommands(u32 numSpotLights, SpotLight** ppSpotLights, const MeshBatch* pMeshBatch)
{
	for (u32 lightIndex = 0; lightIndex < numSpotLights; ++lightIndex)
	{
		const SpotLight* pLight = ppSpotLights[lightIndex];

		const Transform& lightWorldSpaceTransform = pLight->GetTransform();
		const Vector3f& lightWorldSpacePos = lightWorldSpaceTransform.GetPosition();
		const BasisAxes lightWorldSpaceBasis = ExtractBasisAxes(lightWorldSpaceTransform.GetRotation());

		assert(IsNormalized(lightWorldSpaceBasis.m_ZAxis));
		const Vector3f& lightWorldSpaceDir = lightWorldSpaceBasis.m_ZAxis;
		assert(IsNormalized(lightWorldSpaceBasis.m_YAxis));
		const Vector3f& lightWorldSpaceUpDir = lightWorldSpaceBasis.m_YAxis;

		const Matrix4f viewMatrix = CreateLookAtMatrix(lightWorldSpacePos, lightWorldSpacePos + lightWorldSpaceDir, lightWorldSpaceUpDir);
		const Matrix4f projMatrix = CreatePerspectiveFovProjMatrix(pLight->GetOuterConeAngle(), 1.0f, pLight->GetShadowNearPlane(), pLight->GetRange());
		const Matrix4f viewProjMatrix = viewMatrix * projMatrix;

		const Frustum lightWorldFrustum(viewProjMatrix);

		std::vector<u32> meshInstanceIndices;
		std::vector<ShadowMapCommand> renderCommands;

		CreateRenderCommands(meshInstanceIndices, renderCommands, lightWorldFrustum, pMeshBatch);
	}
	assert(false && "Fill buffer");
}

void ShadowRenderer::CreatePointLightRenderStaticGeometryCommands(u32 numPointLights, PointLight** ppPointLights, const MeshBatch* pMeshBatch)
{
	Vector3f lookAtDir[kNumCubeMapFaces];
	lookAtDir[kCubeMapFacePosX] = Vector3f::RIGHT;
	lookAtDir[kCubeMapFaceNegX] = Vector3f::LEFT;
	lookAtDir[kCubeMapFacePosY] = Vector3f::UP;
	lookAtDir[kCubeMapFaceNegY] = Vector3f::DOWN;
	lookAtDir[kCubeMapFacePosZ] = Vector3f::FORWARD;
	lookAtDir[kCubeMapFaceNegZ] = Vector3f::BACK;

	Vector3f upDir[kNumCubeMapFaces];
	upDir[kCubeMapFacePosX] = Vector3f::UP;
	upDir[kCubeMapFaceNegX] = Vector3f::UP;
	upDir[kCubeMapFacePosY] = Vector3f::FORWARD;
	upDir[kCubeMapFaceNegY] = Vector3f::BACK;
	upDir[kCubeMapFacePosZ] = Vector3f::UP;
	upDir[kCubeMapFaceNegZ] = Vector3f::UP;

	for (u32 lightIndex = 0; lightIndex < numPointLights; ++lightIndex)
	{
		const PointLight* pLight = ppPointLights[lightIndex];

		const Transform& lightWorldSpaceTransform = pLight->GetTransform();
		const Vector3f& lightWorldSpacePos = lightWorldSpaceTransform.GetPosition();

		const Matrix4f projMatrix = CreatePerspectiveFovProjMatrix(PI_DIV_2, 1.0f, pLight->GetShadowNearPlane(), pLight->GetRange());
		for (u8 faceIndex = 0; faceIndex < kNumCubeMapFaces; ++faceIndex)
		{
			const Matrix4f viewMatrix = CreateLookAtMatrix(lightWorldSpacePos, lightWorldSpacePos + lookAtDir[faceIndex], upDir[faceIndex]);
			const Matrix4f viewProjMatrix = viewMatrix * projMatrix;

			const Frustum lightWorldFrustum(viewProjMatrix);

			std::vector<u32> meshInstanceIndices;
			std::vector<ShadowMapCommand> renderCommands;

			CreateRenderCommands(meshInstanceIndices, renderCommands, lightWorldFrustum, pMeshBatch);
		}
	}
	assert(false && "Fill buffer");
}
