#include "Scene/SceneLoader.h"
#include "Scene/OBJFileLoader.h"
#include "Scene/Light.h"
#include "Scene/Scene.h"

Scene* SceneLoader::LoadCube()
{
	Scene* pScene = LoadSceneFromOBJFile(L"..\\..\\Resources\\Cube\\cube.obj");

	Camera* pCamera = new Camera(
		Camera::ProjType_Perspective,
		0.1f/*nearClipPlane*/,
		100.0f/*farClipPlane*/,
		1.0f/*aspectRatio*/,
		0.3f/*maxMoveSpeed*/,
		0.1f/*maxRotationSpeed*/);
	
	pCamera->GetTransform().SetPosition(Vector3f(1.98481f, 2.67755f, -2.91555f));
	pCamera->GetTransform().SetRotation(Quaternion(0.312956f, -0.25733f, 0.0987797f, 0.908892f));
	
	pScene->SetCamera(pCamera);
	
	PointLight* pPointLight = new PointLight("Point light", 10.0f, 0.1f);
	pPointLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
	pPointLight->SetIntensity(1.0f);
	pPointLight->GetTransform().SetPosition(Vector3f(1.98481f, 2.67755f, -2.91555f));
	pPointLight->GetTransform().SetRotation(Quaternion(0.312956f, -0.25733f, 0.0987797f, 0.908892f));
		
	pScene->AddPointLight(pPointLight);

	SpotLight* pSpotLight = new SpotLight("Spot light", 5.0f, PI_DIV_4, PI_DIV_2, 0.1f);
	pSpotLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
	pSpotLight->SetIntensity(1.0f);
	pSpotLight->GetTransform().SetPosition(Vector3f(0.0f, 0.0f, -1.0f));

	pScene->AddSpotLight(pSpotLight);

	return pScene;
}

Scene* SceneLoader::LoadErato()
{
	Scene* pScene = LoadSceneFromOBJFile(L"..\\..\\Resources\\Erato\\erato-1.obj");
	return pScene;
}

Scene* SceneLoader::LoadCrytekSponza()
{
	Scene* pScene = LoadSceneFromOBJFile(L"..\\..\\Resources\\CrytekSponza\\sponza.obj");
	
	const AxisAlignedBox& worldBounds = pScene->GetWorldBounds();
	const Vector3f minPoint = worldBounds.m_Center - worldBounds.m_Radius; // {-1920.94592f, -126.442497f, -1105.42603f}
	const Vector3f maxPoint = worldBounds.m_Center + worldBounds.m_Radius; // {1799.90808f, 1429.43323f, 1182.80713f}
		
	Camera* pCamera = new Camera(Camera::ProjType_Perspective,
		1.5f/*nearClipPlane*/,
		3800.0f/*farClipPlane*/,
		1.0f/*aspectRatio*/,
		3.0f/*maxMoveSpeed*/,
		2.0f/*maxRotationSpeed*/);

	pCamera->GetTransform().SetPosition(Vector3f(1190.48f, 204.495f, 38.693f));
	pCamera->GetTransform().SetRotation(Quaternion(0.0f, 0.707107f, 0.0f, -0.707107f));
	pScene->SetCamera(pCamera);
		
#if 1
	PointLight* pPointLight = new PointLight("Point light", 1900.0f, 0.1f);
	pPointLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
	pPointLight->SetIntensity(1.0f);
	pPointLight->GetTransform().SetPosition(Vector3f(-61.0f, 652.0f, 39.0f));
	pScene->AddPointLight(pPointLight);
#endif

#if 1
	SpotLight* pSpotLight = new SpotLight("Spot Light", 780.0f, ToRadians(45.0f), ToRadians(60.0f), 0.1f);
	pSpotLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
	pSpotLight->SetIntensity(1.0f);
	pSpotLight->GetTransform().SetPosition(Vector3f(-61.0f, 652.0f, 39.0f));
	pSpotLight->GetTransform().SetRotation(CreateRotationXQuaternion(PI_DIV_2));
	pScene->AddSpotLight(pSpotLight);
#endif

	return pScene;
}

Scene* SceneLoader::LoadDabrovicSponza()
{
	Scene* pScene = LoadSceneFromOBJFile(L"..\\..\\Resources\\DabrovicSponza\\sponza.obj");

	const AxisAlignedBox& worldBounds = pScene->GetWorldBounds();
	const Vector3f minPoint = worldBounds.m_Center - worldBounds.m_Radius; // {-17.4027596, -0.906688690, -7.80148792}
	const Vector3f maxPoint = worldBounds.m_Center + worldBounds.m_Radius; // {17.4172401, 15.6533108, 7.79851246}

	Camera* pCamera = new Camera(Camera::ProjType_Perspective,
		0.1f/*nearClipPlane*/,
		50.0f/*farClipPlane*/,
		1.0f/*aspectRatio*/,
		0.1f/*maxMoveSpeed*/,
		0.4f/*maxRotationSpeed*/);

	pCamera->GetTransform().SetPosition(Vector3f(-10.0f, 7.0f, 0.0f));
	pCamera->GetTransform().SetRotation(CreateRotationYQuaternion(PI_DIV_2));
	pScene->SetCamera(pCamera);

#if 1
	PointLight* pPointLight = new PointLight("Point light", 35.0f, 0.1f);
	pPointLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
	pPointLight->SetIntensity(1.0f);
	pPointLight->GetTransform().SetPosition(Vector3f(0.0f, 7.5f, 0.0f));
	pScene->AddPointLight(pPointLight);
#endif
	
	return pScene;
}

Scene* SceneLoader::LoadSibenik()
{
	Scene* pScene = LoadSceneFromOBJFile(L"..\\..\\Resources\\Sibenik\\sibenik.obj", true/*use32BitIndices*/);

	const AxisAlignedBox& worldBounds = pScene->GetWorldBounds();
	const Vector3f minPoint = worldBounds.m_Center - worldBounds.m_Radius; // {-20.1410999, -15.3123074, -8.49680042}
	const Vector3f maxPoint = worldBounds.m_Center + worldBounds.m_Radius; // {20.1410999, 15.3000011, 8.49680042}

	Camera* pCamera = new Camera(Camera::ProjType_Perspective,
		1.0f/*nearClipPlane*/,
		45.0f/*farClipPlane*/,
		1.0f/*aspectRatio*/,
		0.4f/*maxMoveSpeed*/,
		0.8f/*maxRotationSpeed*/);

	pCamera->GetTransform().SetPosition(Vector3f(-19.7f, -11.0f, 0.0f));
	pCamera->GetTransform().SetRotation(CreateRotationYQuaternion(PI_DIV_2));
	pScene->SetCamera(pCamera);

#if 1
	PointLight* pPointLight = new PointLight("Point light", 50.0f, 0.1f);
	pPointLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
	pPointLight->SetIntensity(1.0f);
	pPointLight->GetTransform().SetPosition(Vector3f(0.0f, 0.0f, 0.0f));
	pScene->AddPointLight(pPointLight);
#endif

	return pScene;
}

Scene* SceneLoader::LoadCornellBox()
{
	Scene* pScene = LoadSceneFromOBJFile(L"..\\..\\Resources\\CornellBox\\CornellBox-Sphere.obj");

	const AxisAlignedBox& worldBounds = pScene->GetWorldBounds();
	const Vector3f minPoint = worldBounds.m_Center - worldBounds.m_Radius;
	const Vector3f maxPoint = worldBounds.m_Center + worldBounds.m_Radius;

	return pScene;
}

Scene* SceneLoader::LoadSanMiguel()
{
#if 0
	Scene* pScene = LoadSceneFromOBJFile(L"..\\..\\Resources\\SanMiguel\\san-miguel-low-poly.obj", true/*use32BitIndices*/);
#else
	Scene* pScene = LoadSceneFromOBJFile(L"..\\..\\Resources\\SanMiguel\\san-miguel.obj", true/*use32BitIndices*/);
#endif

	const AxisAlignedBox& worldBounds = pScene->GetWorldBounds();
	const Vector3f minPoint = worldBounds.m_Center - worldBounds.m_Radius; // {-22.2742996, -0.269336700, -14.9373989}
	const Vector3f maxPoint = worldBounds.m_Center + worldBounds.m_Radius; // {46.7743988, 14.6000004, 12.0422001}

	Camera* pCamera = new Camera(Camera::ProjType_Perspective,
		0.2f/*nearClipPlane*/,
		80.0f/*farClipPlane*/,
		1.0f/*aspectRatio*/,
		0.2f/*maxMoveSpeed*/,
		0.4f/*maxRotationSpeed*/);

	pCamera->GetTransform().SetPosition(Vector3f(12.0f, 7.0f, 0.0f));
	pCamera->GetTransform().SetRotation(CreateRotationYQuaternion(PI_DIV_2));
	pScene->SetCamera(pCamera);

#if 1
	PointLight* pPointLight = new PointLight("Point light", 50.0f, 0.1f);
	pPointLight->SetColor(Vector3f(0.78f, 0.78f, 0.78f));
	pPointLight->SetIntensity(1.0f);
	pPointLight->GetTransform().SetPosition(Vector3f(12.0f, 7.0f, 0.0f));
	pScene->AddPointLight(pPointLight);
#endif

	return pScene;
}
