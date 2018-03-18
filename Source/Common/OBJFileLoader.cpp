#include "Common/OBJFileLoader.h"
#include "Common/FileUtilities.h"
#include "Common/Material.h"
#include "Common/Mesh.h"
#include "Common/MeshBatch.h"
#include "Common/Scene.h"
#include "Common/StringUtilities.h"
#include <experimental/filesystem>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace
{
	void AddAssimpMeshes(Scene* pScene, const aiScene* pAssimpScene)
	{
		assert(pAssimpScene->HasMeshes());

		const u8 vertexFormat = VertexData::FormatFlag_Position | VertexData::FormatFlag_Normal | VertexData::FormatFlag_TexCoords;
		const DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT;
		const D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		const D3D12_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		MeshBatch* pMeshBatch = new MeshBatch(vertexFormat, indexFormat, primitiveTopologyType, primitiveTopology);

		for (decltype(pAssimpScene->mNumMeshes) meshIndex = 0; meshIndex < pAssimpScene->mNumMeshes; ++meshIndex)
		{
			const aiMesh* pAssimpMesh = pAssimpScene->mMeshes[meshIndex];

			assert(pAssimpMesh->HasPositions());
			assert(pAssimpMesh->HasNormals());
			assert(pAssimpMesh->HasTextureCoords(0));
			assert(pAssimpMesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE);

			const u32 numVertices = pAssimpMesh->mNumVertices;

			Vector3f* pPositions = new Vector3f[numVertices];
			Vector3f* pNormals = new Vector3f[numVertices];
			Vector2f* pTexCoords = new Vector2f[numVertices];

			for (u32 vertexIndex = 0; vertexIndex < numVertices; ++vertexIndex)
			{
				const aiVector3D& position = pAssimpMesh->mVertices[vertexIndex];
				pPositions[vertexIndex] = Vector3f(position.x, position.y, position.z);
			}
			for (u32 vertexIndex = 0; vertexIndex < numVertices; ++vertexIndex)
			{
				const aiVector3D& normal = pAssimpMesh->mNormals[vertexIndex];
				pNormals[vertexIndex] = Vector3f(normal.x, normal.y, normal.z);
			}
			for (u32 vertexIndex = 0; vertexIndex < numVertices; ++vertexIndex)
			{
				const aiVector3D& texCoords = pAssimpMesh->mTextureCoords[0][vertexIndex];
				pTexCoords[vertexIndex] = Vector2f(texCoords.x, texCoords.y);
			}

			const u32 numIndices = 3 * pAssimpMesh->mNumFaces;
			u16* pIndices = new u16[numIndices];

			for (decltype(pAssimpMesh->mNumFaces) faceIndex = 0; faceIndex < pAssimpMesh->mNumFaces; ++faceIndex)
			{
				const aiFace& face = pAssimpMesh->mFaces[faceIndex];
				assert(face.mNumIndices == 3);

				pIndices[3 * faceIndex + 0] = u16(face.mIndices[0]);
				pIndices[3 * faceIndex + 1] = u16(face.mIndices[1]);
				pIndices[3 * faceIndex + 2] = u16(face.mIndices[2]);
			}

			VertexData* pVertexData = new VertexData(numVertices, pPositions, pNormals, pTexCoords);
			IndexData* pIndexData = new IndexData(numIndices, pIndices);

			const u8 numInstances = 1;
			Matrix4f* pInstanceWorldMatrices = new Matrix4f[numInstances];
			pInstanceWorldMatrices[0] = Matrix4f::IDENTITY;

			Mesh mesh(pVertexData, pIndexData, numInstances, pInstanceWorldMatrices,
				pAssimpMesh->mMaterialIndex, primitiveTopologyType, primitiveTopology);

			pMeshBatch->AddMesh(&mesh);
		}
		pScene->AddMeshBatch(pMeshBatch);
	}

	void AddAssimpMaterials(Scene* pScene, const aiScene* pAssimpScene, const std::experimental::filesystem::path& materialDirectoryPath)
	{
		assert(pAssimpScene->HasMaterials());

		aiString assimpName;
		aiString assimpMapPath;
		aiColor3D assimpColor;

		for (decltype(pAssimpScene->mNumMaterials) materialIndex = 0; materialIndex < pAssimpScene->mNumMaterials; ++materialIndex)
		{
			aiMaterial* pAssimpMaterial = pAssimpScene->mMaterials[materialIndex];
			
			pAssimpMaterial->Get(AI_MATKEY_NAME, assimpName);
			Material* pMaterial = new Material(AnsiToWideString(assimpName.C_Str()));

			if (pAssimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &assimpMapPath) == aiReturn_SUCCESS)
				pMaterial->m_DiffuseMapFilePath = materialDirectoryPath / std::experimental::filesystem::path(AnsiToWideString(assimpMapPath.C_Str()));
			else if (pAssimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, assimpColor) == aiReturn_SUCCESS)
				pMaterial->m_DiffuseColor = Vector3f(assimpColor.r, assimpColor.g, assimpColor.b);
			else
				assert(false);

			if (pAssimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &assimpMapPath) == aiReturn_SUCCESS)
				pMaterial->m_SpecularMapFilePath = materialDirectoryPath / std::experimental::filesystem::path(AnsiToWideString(assimpMapPath.C_Str()));
			else if (pAssimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, assimpColor) == aiReturn_SUCCESS)
				pMaterial->m_SpecularColor = Vector3f(assimpColor.r, assimpColor.g, assimpColor.b);
			else
				assert(false);

			if (pAssimpMaterial->GetTexture(aiTextureType_SHININESS, 0, &assimpMapPath) == aiReturn_SUCCESS)
				pMaterial->m_ShininessMapFilePath = materialDirectoryPath / std::experimental::filesystem::path(AnsiToWideString(assimpMapPath.C_Str()));
			else if (pAssimpMaterial->Get(AI_MATKEY_SHININESS, assimpColor.r) == aiReturn_SUCCESS)
				pMaterial->m_Shininess = assimpColor.r;
			else
				assert(false);
			
			pScene->AddMaterial(pMaterial);
		}
	}
}

Scene* LoadSceneFromOBJFile(const wchar_t* pFilePath)
{
	Assimp::Importer importer;
	
	u32 importFlags = aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_SortByPType |
		aiProcess_MakeLeftHanded |
		aiProcess_FlipUVs |
		aiProcess_FlipWindingOrder |
		aiProcess_JoinIdenticalVertices |
		aiProcess_OptimizeMeshes |
		aiProcess_RemoveRedundantMaterials;

	const aiScene* pAssimpScene = importer.ReadFile(WideToAnsiString(pFilePath), importFlags);
	if (pAssimpScene == nullptr)
	{
		OutputDebugStringA(importer.GetErrorString());
		return nullptr;
	}

	Scene* pScene = new Scene();
	AddAssimpMeshes(pScene, pAssimpScene);
			
	std::experimental::filesystem::path materialDirectoryPath(pFilePath);
	materialDirectoryPath.remove_filename();

	AddAssimpMaterials(pScene, pAssimpScene, materialDirectoryPath);
	
	return pScene;
}