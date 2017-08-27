#include "Common/SceneLoader.h"
#include "Common/Scene.h"
#include "Common/Mesh.h"
#include "Common/MeshBatch.h"
#include "Common/MeshUtilities.h"
#include "Common/Light.h"
#include "Common/Color.h"
#include "Common/Material.h"
#include "Common/OBJFileLoader.h"
#include "Math/BasisAxes.h"

Scene* SceneLoader::LoadCornellBox(CornellBoxSettings settings)
{
	Scene* pScene = new Scene();
	
	const u8 vertexFormat = VertexData::FormatFlag_Position | VertexData::FormatFlag_Normal;
	MeshBatch* pMeshBatch = new MeshBatch(vertexFormat, DXGI_FORMAT_R16_UINT, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	{
		// Floor
		const Vector3f positions[] =
		{
			Vector3f(549.6f, 0.0f,   0.0f),
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
				
		Material* pMaterial = new Material(L"Floor");
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial->m_AmbientColor = Vector3f(0.725f, 0.71f, 0.68f);
			pMaterial->m_DiffuseColor = Vector3f(0.725f, 0.71f, 0.68f);
			pMaterial->m_SpecularColor = Vector3f::ZERO;
			pMaterial->m_Shininess = 0.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test2)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test3)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else 
		{
			assert(false);
		}

		u32 materialIndex = pScene->GetNumMaterials();
		pScene->AddMaterial(pMaterial);
		
		u32 numInstances = 1;
		Matrix4f* pInstanceWorldMatrices = new Matrix4f[numInstances];
		pInstanceWorldMatrices[0] = Matrix4f::IDENTITY;

		ConvertVertexAndIndexData(ConvertionFlag_LeftHandedCoordSystem, pVertexData, pIndexData);
		Mesh mesh(pVertexData, pIndexData, numInstances, pInstanceWorldMatrices, materialIndex,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		pMeshBatch->AddMesh(&mesh);
	}
	{
		// Ceiling
		const Vector3f positions[] =
		{
			Vector3f(549.6f, 548.8f,   0.0f),
			Vector3f(549.6f, 548.8f, 559.2f),
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
		
		Material* pMaterial = new Material(L"Ceiling");
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial->m_AmbientColor = Vector3f(0.725f, 0.71f, 0.68f);
			pMaterial->m_DiffuseColor = Vector3f(0.725f, 0.71f, 0.68f);
			pMaterial->m_SpecularColor = Vector3f::ZERO;
			pMaterial->m_Shininess = 0.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test2)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test3)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else
		{
			assert(false);
		}

		u32 materialIndex = pScene->GetNumMaterials();
		pScene->AddMaterial(pMaterial);

		u32 numInstances = 1;
		Matrix4f* pInstanceWorldMatrices = new Matrix4f[numInstances];
		pInstanceWorldMatrices[0] = Matrix4f::IDENTITY;

		ConvertVertexAndIndexData(ConvertionFlag_LeftHandedCoordSystem, pVertexData, pIndexData);
		Mesh mesh(pVertexData, pIndexData, numInstances, pInstanceWorldMatrices, materialIndex,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pMeshBatch->AddMesh(&mesh);
	}
	{
		// Back wall
		const Vector3f positions[] =
		{
			Vector3f(549.6f,   0.0f, 559.2f),
			Vector3f(  0.0f,   0.0f, 559.2f),
			Vector3f(  0.0f, 548.8f, 559.2f),
			Vector3f(549.6f, 548.8f, 559.2f)
		};
		const u32 numVertices = ARRAYSIZE(positions);

		const u16 indices[] = {0, 1, 2, 2, 3, 0};
		const u32 numIndices = ARRAYSIZE(indices);

		Vector3f normals[numVertices];
		ComputeNormals(numVertices, &positions[0], numIndices, &indices[0], &normals[0]);

		VertexData* pVertexData = new VertexData(numVertices, &positions[0], &normals[0]);
		IndexData* pIndexData = new IndexData(numIndices, &indices[0]);
		
		Material* pMaterial = new Material(L"Back wall");
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial->m_AmbientColor = Vector3f(0.725f, 0.71f, 0.68f);
			pMaterial->m_DiffuseColor = Vector3f(0.725f, 0.71f, 0.68f);
			pMaterial->m_SpecularColor = Vector3f::ZERO;
			pMaterial->m_Shininess = 0.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test2)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test3)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else
		{
			assert(false);
		}

		u32 materialIndex = pScene->GetNumMaterials();
		pScene->AddMaterial(pMaterial);

		u32 numInstances = 1;
		Matrix4f* pInstanceWorldMatrices = new Matrix4f[numInstances];
		pInstanceWorldMatrices[0] = Matrix4f::IDENTITY;

		ConvertVertexAndIndexData(ConvertionFlag_LeftHandedCoordSystem, pVertexData, pIndexData);
		Mesh mesh(pVertexData, pIndexData, numInstances, pInstanceWorldMatrices, materialIndex,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pMeshBatch->AddMesh(&mesh);
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
		
		Material* pMaterial = new Material(L"Right wall");
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial->m_AmbientColor = Vector3f(0.14f, 0.45f, 0.091f);
			pMaterial->m_DiffuseColor = Vector3f(0.14f, 0.45f, 0.091f);
			pMaterial->m_SpecularColor = Vector3f::ZERO;
			pMaterial->m_Shininess = 0.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test2)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test3)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else
		{
			assert(false);
		}

		u32 materialIndex = pScene->GetNumMaterials();
		pScene->AddMaterial(pMaterial);

		u32 numInstances = 1;
		Matrix4f* pInstanceWorldMatrices = new Matrix4f[numInstances];
		pInstanceWorldMatrices[0] = Matrix4f::IDENTITY;

		ConvertVertexAndIndexData(ConvertionFlag_LeftHandedCoordSystem, pVertexData, pIndexData);
		Mesh mesh(pVertexData, pIndexData, numInstances, pInstanceWorldMatrices, materialIndex,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pMeshBatch->AddMesh(&mesh);
	}
	{
		// Left wall
		const Vector3f positions[] =
		{
			Vector3f(549.6f,   0.0f,   0.0f),
			Vector3f(549.6f,   0.0f, 559.2f),
			Vector3f(549.6f, 548.8f, 559.2f),
			Vector3f(549.6f, 548.8f,   0.0f)
		};
		const u32 numVertices = ARRAYSIZE(positions);

		const u16 indices[] = {0, 1, 2, 2, 3, 0};
		const u32 numIndices = ARRAYSIZE(indices);

		Vector3f normals[numVertices];
		ComputeNormals(numVertices, &positions[0], numIndices, &indices[0], &normals[0]);

		VertexData* pVertexData = new VertexData(numVertices, &positions[0], &normals[0]);
		IndexData* pIndexData = new IndexData(numIndices, &indices[0]);
		
		Material* pMaterial = new Material(L"Left wall");
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial->m_AmbientColor = Vector3f(0.63f, 0.065f, 0.05f);
			pMaterial->m_DiffuseColor = Vector3f(0.63f, 0.065f, 0.05f);
			pMaterial->m_SpecularColor = Vector3f::ZERO;
			pMaterial->m_Shininess = 0.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test2)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test3)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else
		{
			assert(false);
		}

		u32 materialIndex = pScene->GetNumMaterials();
		pScene->AddMaterial(pMaterial);

		u32 numInstances = 1;
		Matrix4f* pInstanceWorldMatrices = new Matrix4f[numInstances];
		pInstanceWorldMatrices[0] = Matrix4f::IDENTITY;

		ConvertVertexAndIndexData(ConvertionFlag_LeftHandedCoordSystem, pVertexData, pIndexData);
		Mesh mesh(pVertexData, pIndexData, numInstances, pInstanceWorldMatrices, materialIndex,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pMeshBatch->AddMesh(&mesh);
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
		
		Material* pMaterial = new Material(L"Short block");
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial->m_AmbientColor = Vector3f(0.725f, 0.71f, 0.68f);
			pMaterial->m_DiffuseColor = Vector3f(0.725f, 0.71f, 0.68f);
			pMaterial->m_SpecularColor = Vector3f::ZERO;
			pMaterial->m_Shininess = 0.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test2)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test3)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else
		{
			assert(false);
		}

		u32 materialIndex = pScene->GetNumMaterials();
		pScene->AddMaterial(pMaterial);

		u32 numInstances = 1;
		Matrix4f* pInstanceWorldMatrices = new Matrix4f[numInstances];
		pInstanceWorldMatrices[0] = Matrix4f::IDENTITY;

		ConvertVertexAndIndexData(ConvertionFlag_LeftHandedCoordSystem, pVertexData, pIndexData);
		Mesh mesh(pVertexData, pIndexData, numInstances, pInstanceWorldMatrices, materialIndex,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pMeshBatch->AddMesh(&mesh);
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
		
		Material* pMaterial = new Material(L"Tall block");
		if (settings == CornellBoxSettings_Original)
		{
			pMaterial->m_AmbientColor = Vector3f(0.725f, 0.71f, 0.68f);
			pMaterial->m_DiffuseColor = Vector3f(0.725f, 0.71f, 0.68f);
			pMaterial->m_SpecularColor = Vector3f::ZERO;
			pMaterial->m_Shininess = 0.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test1)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test2)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else if (settings == CornellBoxSettings_Test3)
		{
			pMaterial->m_AmbientColor = Vector3f(1.0f, 0.0f, 1.0f);
			pMaterial->m_DiffuseColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_SpecularColor = Vector3f(1.0f, 0.8f, 0.0f);
			pMaterial->m_Shininess = 100.0f;
			pMaterial->m_EmissiveColor = Vector3f::ZERO;
		}
		else
		{
			assert(false);
		}
		
		u32 materialIndex = pScene->GetNumMaterials();
		pScene->AddMaterial(pMaterial);

		u32 numInstances = 1;
		Matrix4f* pInstanceWorldMatrices = new Matrix4f[numInstances];
		pInstanceWorldMatrices[0] = Matrix4f::IDENTITY;

		ConvertVertexAndIndexData(ConvertionFlag_LeftHandedCoordSystem, pVertexData, pIndexData);
		Mesh mesh(pVertexData, pIndexData, numInstances, pInstanceWorldMatrices, materialIndex,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pMeshBatch->AddMesh(&mesh);
	}

	pScene->AddMeshBatch(pMeshBatch);

	// Cornell box bounds in left-handed coordinate system
	// 0 <= x <= 549.6f
	// 0 <= y <= 548.8f
	// -559.2f (back wall) <= z <= 0
	
	if (settings == CornellBoxSettings_Original)
	{
		PointLight* pPointLight = new PointLight("pPointLight", 650.0f);
		pPointLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
		pPointLight->GetTransform().SetPosition(Vector3f(0.5f * 549.6f, 400.0f, -0.5f * 559.2f));

		pScene->AddPointLight(pPointLight);
	}
	else if (settings == CornellBoxSettings_Test1)
	{
		SpotLight* pSpotLight = new SpotLight("pSpotLight", 650.0f, PI_DIV_FOUR, PI_DIV_TWO);
		pSpotLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
		pSpotLight->GetTransform().SetPosition(Vector3f(0.5f * 549.6f, 540.0f, -0.5f * 559.2f));
		pSpotLight->GetTransform().SetRotation(CreateRotationXQuaternion(PI_DIV_TWO));

		pScene->AddSpotLight(pSpotLight);
	}
	else if (settings == CornellBoxSettings_Test2)
	{
		PointLight* pPointLight = new PointLight("pPointLight", 650.0f);
		pPointLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
		pPointLight->GetTransform().SetPosition(Vector3f(0.5f * 549.6f, 400.0f, -0.5f * 559.2f));

		pScene->AddPointLight(pPointLight);
	}
	else if (settings == CornellBoxSettings_Test3)
	{
		DirectionalLight* pDirectionalLight = new DirectionalLight("pDirectionalLight");
		pDirectionalLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
		pDirectionalLight->GetTransform().SetRotation(CreateRotationXQuaternion(PI_DIV_TWO + PI_DIV_FOUR));

		pScene->SetDirectionalLight(pDirectionalLight);
	}
	else
	{
		assert(false);
	}
	return pScene;
}

Scene* SceneLoader::LoadSponza()
{
	const wchar_t* pathToOBJFile = L"..\\..\\Resources\\Sponza\\sponza.obj";

	OBJFileLoader fileLoader;
	auto meshBatchData = fileLoader.Load(pathToOBJFile, false/*use32BitIndices*/, ConvertionFlag_LeftHandedCoordSystem);

	return nullptr;
}
