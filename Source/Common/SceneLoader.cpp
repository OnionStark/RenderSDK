#include "Common/SceneLoader.h"
#include "Common/Scene.h"
#include "Common/MeshBatchData.h"
#include "Common/MeshData.h"
#include "Common/MeshDataUtilities.h"
#include "Common/Light.h"
#include "Common/Color.h"
#include "Math/BasisAxes.h"

Scene* SceneLoader::LoadCornellBox(CornellBoxSettings settings)
{
	Scene* pScene = new Scene();

	const u8 meshBatchVertexFormat = VertexData::FormatFlag_Position | VertexData::FormatFlag_Normal;
	MeshBatchData* pMeshBatchData = new MeshBatchData(meshBatchVertexFormat, DXGI_FORMAT_R16_UINT, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	{
		// Floor
		const Vector3f positions[] =
		{
			Vector3f(552.8f, 0.0f,   0.0f),
			Vector3f(  0.0f, 0.0f,   0.0f),
			Vector3f(  0.0f, 0.0f, 559.2f),
			Vector3f(549.6f, 0.0f, 559.2f)
		};
		const u32 numVertices = ARRAYSIZE(positions);

		const u16 indices[] = {0, 1, 2, 2, 3, 0};
		const u32 numIndices = ARRAYSIZE(indices);

		Vector3f normals[numVertices];
		ComputeNormals(numVertices, &positions[0], numIndices, &indices[0], &normals[0]);

		VertexData* pVertexData = new VertexData(numVertices, &positions[0], &normals[0]);
		IndexData* pIndexData = new IndexData(numIndices, &indices[0]);
				
		Material* pMaterial = nullptr;
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial = new Material(
				Vector4f(0.725f, 0.71f, 0.68f, 1.0f),
				Vector4f(0.725f, 0.71f, 0.68f, 1.0f),
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial = new Material(
				Vector4f::ZERO,
				Color::BISQUE,
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else 
		{
			assert(false);
		}

		MeshData meshData(pVertexData, pIndexData, pMaterial, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ConvertMeshData(&meshData, ConvertionFlag_LeftHandedCoordSystem);

		pMeshBatchData->Append(&meshData);
	}
	{
		// Ceiling
		const Vector3f positions[] =
		{
			Vector3f(556.0f, 548.8f,   0.0f),
			Vector3f(556.0f, 548.8f, 559.2f),
			Vector3f(  0.0f, 548.8f, 559.2f),
			Vector3f(  0.0f, 548.8f,   0.0f)
		};
		const u32 numVertices = ARRAYSIZE(positions);

		const u16 indices[] = {0, 1, 2, 2, 3, 0};
		const u32 numIndices = ARRAYSIZE(indices);

		Vector3f normals[numVertices];
		ComputeNormals(numVertices, &positions[0], numIndices, &indices[0], &normals[0]);

		VertexData* pVertexData = new VertexData(numVertices, &positions[0], &normals[0]);
		IndexData* pIndexData = new IndexData(numIndices, &indices[0]);
		
		Material* pMaterial = nullptr;
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial = new Material(
				Vector4f(0.725f, 0.71f, 0.68f, 1.0f),
				Vector4f(0.725f, 0.71f, 0.68f, 1.0f),
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial = new Material(
				Vector4f::ZERO,
				Color::BLANCHED_ALMOND,
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else
		{
			assert(false);
		}
		
		MeshData meshData(pVertexData, pIndexData, pMaterial, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ConvertMeshData(&meshData, ConvertionFlag_LeftHandedCoordSystem);

		pMeshBatchData->Append(&meshData);
	}
	{
		// Back wall
		const Vector3f positions[] =
		{
			Vector3f(549.6f,   0.0f, 559.2f),
			Vector3f(  0.0f,   0.0f, 559.2f),
			Vector3f(  0.0f, 548.8f, 559.2f),
			Vector3f(556.0f, 548.8f, 559.2f)
		};
		const u32 numVertices = ARRAYSIZE(positions);

		const u16 indices[] = {0, 1, 2, 2, 3, 0};
		const u32 numIndices = ARRAYSIZE(indices);

		Vector3f normals[numVertices];
		ComputeNormals(numVertices, &positions[0], numIndices, &indices[0], &normals[0]);

		VertexData* pVertexData = new VertexData(numVertices, &positions[0], &normals[0]);
		IndexData* pIndexData = new IndexData(numIndices, &indices[0]);
		
		Material* pMaterial = nullptr;
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial = new Material(
				Vector4f(0.725f, 0.71f, 0.68f, 1.0f),
				Vector4f(0.725f, 0.71f, 0.68f, 1.0f),
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial = new Material(
				Vector4f::ZERO,
				Color::BLUE_VIOLET,
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else
		{
			assert(false);
		}
		
		MeshData meshData(pVertexData, pIndexData, pMaterial, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ConvertMeshData(&meshData, ConvertionFlag_LeftHandedCoordSystem);

		pMeshBatchData->Append(&meshData);
	}
	{
		// Right wall
		const Vector3f positions[] =
		{
			Vector3f(0.0f,   0.0f, 559.2f),
			Vector3f(0.0f,   0.0f,   0.0f),
			Vector3f(0.0f, 548.8f,   0.0f),
			Vector3f(0.0f, 548.8f, 559.2f)
		};
		const u32 numVertices = ARRAYSIZE(positions);

		const u16 indices[] = {0, 1, 2, 2, 3, 0};
		const u32 numIndices = ARRAYSIZE(indices);

		Vector3f normals[numVertices];
		ComputeNormals(numVertices, &positions[0], numIndices, &indices[0], &normals[0]);

		VertexData* pVertexData = new VertexData(numVertices, &positions[0], &normals[0]);
		IndexData* pIndexData = new IndexData(numIndices, &indices[0]);
		
		Material* pMaterial = nullptr;
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial = new Material(
				Vector4f(0.14f, 0.45f, 0.091f, 1.0f),
				Vector4f(0.14f, 0.45f, 0.091f, 1.0f),
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial = new Material(
				Vector4f(0.14f, 0.45f, 0.091f, 1.0f),
				Vector4f(0.14f, 0.45f, 0.091f, 1.0f),
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else
		{
			assert(false);
		}
		
		MeshData meshData(pVertexData, pIndexData, pMaterial, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ConvertMeshData(&meshData, ConvertionFlag_LeftHandedCoordSystem);

		pMeshBatchData->Append(&meshData);
	}
	{
		// Left wall
		const Vector3f positions[] =
		{
			Vector3f(552.8f,   0.0f,   0.0f),
			Vector3f(549.6f,   0.0f, 559.2f),
			Vector3f(556.0f, 548.8f, 559.2f),
			Vector3f(556.0f, 548.8f,   0.0f)
		};
		const u32 numVertices = ARRAYSIZE(positions);

		const u16 indices[] = {0, 1, 2, 2, 3, 0};
		const u32 numIndices = ARRAYSIZE(indices);

		Vector3f normals[numVertices];
		ComputeNormals(numVertices, &positions[0], numIndices, &indices[0], &normals[0]);

		VertexData* pVertexData = new VertexData(numVertices, &positions[0], &normals[0]);
		IndexData* pIndexData = new IndexData(numIndices, &indices[0]);
		
		Material* pMaterial = nullptr;
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial = new Material(
				Vector4f(0.63f, 0.065f, 0.05f, 1.0f),
				Vector4f(0.63f, 0.065f, 0.05f, 1.0f),
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial = new Material(
				Vector4f(0.63f, 0.065f, 0.05f, 1.0f),
				Vector4f(0.63f, 0.065f, 0.05f, 1.0f),
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else
		{
			assert(false);
		}

		MeshData meshData(pVertexData, pIndexData, pMaterial, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ConvertMeshData(&meshData, ConvertionFlag_LeftHandedCoordSystem);

		pMeshBatchData->Append(&meshData);
	}
	{
		// Short block
		const Vector3f positions[] =
		{
			Vector3f(130.0f, 165.0f,  65.0f),
			Vector3f( 82.0f, 165.0f, 225.0f),
			Vector3f(240.0f, 165.0f, 272.0f),
			Vector3f(290.0f, 165.0f, 114.0f),

			Vector3f(290.0f,   0.0f, 114.0f),
			Vector3f(290.0f, 165.0f, 114.0f),
			Vector3f(240.0f, 165.0f, 272.0f),
			Vector3f(240.0f,   0.0f, 272.0f),

			Vector3f(130.0f,   0.0f,  65.0f),
			Vector3f(130.0f, 165.0f,  65.0f),
			Vector3f(290.0f, 165.0f, 114.0f),
			Vector3f(290.0f,   0.0f, 114.0f),

			Vector3f( 82.0f,   0.0f, 225.0f),
			Vector3f( 82.0f, 165.0f, 225.0f),
			Vector3f(130.0f, 165.0f,  65.0f),
			Vector3f(130.0f,   0.0f,  65.0f),

			Vector3f(240.0f,   0.0f, 272.0f),
			Vector3f(240.0f, 165.0f, 272.0f),
			Vector3f( 82.0f, 165.0f, 225.0f),
			Vector3f( 82.0f,   0.0f, 225.0f)
		};
		const u32 numVertices = ARRAYSIZE(positions);

		const u16 indices[] =
		{
			 0,  1,  2,  2,  3,  0,
			 4,  5,  6,  6,  7,  4,
			 8,  9, 10, 10, 11,  8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16
		};
		const u32 numIndices = ARRAYSIZE(indices);

		Vector3f normals[numVertices];
		ComputeNormals(numVertices, &positions[0], numIndices, &indices[0], &normals[0]);

		VertexData* pVertexData = new VertexData(numVertices, &positions[0], &normals[0]);
		IndexData* pIndexData = new IndexData(numIndices, &indices[0]);
		
		Material* pMaterial = nullptr;
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial = new Material(
				Vector4f(0.725f, 0.71f, 0.68f, 1.0f),
				Vector4f(0.725f, 0.71f, 0.68f, 1.0f),
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial = new Material(
				Vector4f::ZERO,
				Color::BLUE,
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else
		{
			assert(false);
		}
		
		MeshData meshData(pVertexData, pIndexData, pMaterial, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ConvertMeshData(&meshData, ConvertionFlag_LeftHandedCoordSystem);

		pMeshBatchData->Append(&meshData);
	}
	{
		// Tall block
		const Vector3f positions[] =
		{
			Vector3f(423.0f, 330.0f, 247.0f),
			Vector3f(265.0f, 330.0f, 296.0f),
			Vector3f(314.0f, 330.0f, 456.0f),
			Vector3f(472.0f, 330.0f, 406.0f),

			Vector3f(423.0f,   0.0f, 247.0f),
			Vector3f(423.0f, 330.0f, 247.0f),
			Vector3f(472.0f, 330.0f, 406.0f),
			Vector3f(472.0f,   0.0f, 406.0f),

			Vector3f(472.0f,   0.0f, 406.0f),
			Vector3f(472.0f, 330.0f, 406.0f),
			Vector3f(314.0f, 330.0f, 456.0f),
			Vector3f(314.0f,   0.0f, 456.0f),

			Vector3f(314.0f,   0.0f, 456.0f),
			Vector3f(314.0f, 330.0f, 456.0f),
			Vector3f(265.0f, 330.0f, 296.0f),
			Vector3f(265.0f,   0.0f, 296.0f),

			Vector3f(265.0f,   0.0f, 296.0f),
			Vector3f(265.0f, 330.0f, 296.0f),
			Vector3f(423.0f, 330.0f, 247.0f),
			Vector3f(423.0f,   0.0f, 247.0f)
		};
		const u32 numVertices = ARRAYSIZE(positions);

		const u16 indices[] =
		{
			 0,  1,  2,  2,  3,  0,
			 4,  5,  6,  6,  7,  4,
			 8,  9, 10, 10, 11,  8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16
		};
		const u32 numIndices = ARRAYSIZE(indices);

		Vector3f normals[numVertices];
		ComputeNormals(numVertices, &positions[0], numIndices, &indices[0], &normals[0]);

		VertexData* pVertexData = new VertexData(numVertices, &positions[0], &normals[0]);
		IndexData* pIndexData = new IndexData(numIndices, &indices[0]);
		
		Material* pMaterial = nullptr;
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial = new Material(
				Vector4f(0.725f, 0.71f, 0.68f, 1.0f),
				Vector4f(0.725f, 0.71f, 0.68f, 1.0f),
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial = new Material(
				Vector4f(0.725f, 0.71f, 0.68f, 1.0f),
				Color::GOLD,
				Vector4f::ZERO, 0.0f,
				Vector4f::ZERO);
		}
		else
		{
			assert(false);
		}
		
		MeshData meshData(pVertexData, pIndexData, pMaterial, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ConvertMeshData(&meshData, ConvertionFlag_LeftHandedCoordSystem);

		pMeshBatchData->Append(&meshData);
	}

	pScene->AddMeshBatch(pMeshBatchData);

	// Cornell box bounds in left-handed coordinate system
	// 0 < x < 549
	// 0 < y < 548
	// -559 (back wall) < z < 0
	
	SpotLight* pSpotLight = nullptr;
	if (settings == CornellBoxSettings_Original)
	{
		pSpotLight = new SpotLight("SpotLight", 550.0f, Radian(0.5 * PI_DIV_FOUR), Radian(PI_DIV_FOUR));
		pSpotLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
		pSpotLight->GetTransform().SetPosition(Vector3f(275.0f, 540.0f, -280.0f));
		pSpotLight->GetTransform().SetRotation(CreateRotationXQuaternion(Radian(PI_DIV_TWO)));
	}
	else if (settings == CornellBoxSettings_Test1)
	{
		pSpotLight = new SpotLight("SpotLight", 550.0f, Radian(0.5 * PI_DIV_FOUR), Radian(PI_DIV_FOUR));
		pSpotLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
		pSpotLight->GetTransform().SetPosition(Vector3f(275.0f, 540.0f, -280.0f));
		pSpotLight->GetTransform().SetRotation(CreateRotationXQuaternion(Radian(PI_DIV_TWO)));
	}
	else
	{
		assert(false);
	}

	pScene->AddSpotLight(pSpotLight);
	return pScene;
}