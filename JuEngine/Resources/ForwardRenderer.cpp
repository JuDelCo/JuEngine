// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "ForwardRenderer.hpp"
#include "Shader.hpp"
#include "../Components/Camera.hpp"
#include "../Components/Light.hpp"
#include "../Components/MeshRenderer.hpp"
#include "../Components/Transform.hpp"
#include "../Components/World.hpp"
#include "../Entity/Pool.hpp"
#include "../Entity/Group.hpp"
#include "../OpenGL.hpp"
#include "../App.hpp"
#include "../Services/IWindowService.hpp"

namespace JuEngine
{
ForwardRenderer::ForwardRenderer()
{
	SetId("forwardRenderer");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.f, 1.f);

	//glEnable(GL_DEPTH_CLAMP);

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClearDepth(1.f);

	// ----------------

	glGenBuffers(1, &mGlobalMatrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, mGlobalMatrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, mGlobalMatrixBindingIndex, mGlobalMatrixUBO, 0, sizeof(mat4) * 2);

	// ----------------

	glGenBuffers(1, &mWorldUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, mWorldUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(vec4) + sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, mWorldBindingIndex, mWorldUBO, 0, sizeof(vec4) + sizeof(float) * 3);

	// ----------------

	glGenBuffers(1, &mMaterialUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, mMaterialUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(vec4) * 2 + sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, mMaterialBindingIndex, mMaterialUBO, 0, sizeof(vec4) * 2 + sizeof(float));

	// ----------------

	glGenBuffers(1, &mLightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, mLightUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(vec4) * 2 * 5, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, mLightBindingIndex, mLightUBO, 0, sizeof(vec4) * 2 * 5);
}

ForwardRenderer::~ForwardRenderer()
{
	glDeleteBuffers(1, &mGlobalMatrixUBO);
	glDeleteBuffers(1, &mWorldUBO);
	glDeleteBuffers(1, &mMaterialUBO);
	glDeleteBuffers(1, &mLightUBO);
}

void ForwardRenderer::Render()
{
	if(mPools.size() == 0)
	{
		return;
	}

	// TODO: ForwardRenderer: Cache entities
	std::vector<EntityPtr> cameras;
	std::vector<EntityPtr> entities;
	std::vector<EntityPtr> lights;

	for(const auto &pool : mPools)
	{
		auto poolCameras = pool->GetGroup(Matcher_AllOf(Transform, Camera))->GetEntities();
		auto poolEntities = pool->GetGroup(Matcher_AllOf(Transform, MeshRenderer))->GetEntities();
		auto poolLights = pool->GetGroup(Matcher_AllOf(Transform, Light))->GetEntities();

		cameras.insert(cameras.end(), poolCameras.begin(), poolCameras.end());
		entities.insert(entities.end(), poolEntities.begin(), poolEntities.end());
		lights.insert(lights.end(), poolLights.begin(), poolLights.end());
	}

	for(const auto &pool : mPools)
	{
		if(pool->GetGroup(Matcher_AllOf(World))->Count() == 0)
		{
			continue;
		}

		auto world = pool->GetGroup(Matcher_AllOf(World))->GetSingleEntity()->Get<World>();

		float gammaCorrection = 1.f / world->GetGammaCorrection();

		// Actualizamos el Uniform Block "World"
		glBindBuffer(GL_UNIFORM_BUFFER, mWorldUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(vec3), Math::GetDataPtr(world->GetAmbientColor()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4), sizeof(float), &world->GetAmbientIntensity());
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) + sizeof(float), sizeof(float), &world->GetLightAttenuation());
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) + sizeof(float) * 2, sizeof(float), &gammaCorrection);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Limpiamos los buffers de color y profundidad
		auto skyColor = world->GetSkyColor();
		glClearColor(pow(skyColor.x,gammaCorrection), pow(skyColor.y,gammaCorrection), pow(skyColor.z,gammaCorrection), 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		break;
	}

	// Dibujamos la escena por cada cámara activa
	for(const auto &cameraEntity : cameras)
	{
		// Actualizamos el viewport dependiendo de la cámara y el tamaño de la pantalla
		auto camera = cameraEntity->Get<Camera>();
		camera->SetScreenSize(App::Window()->GetSize());
		vec4 viewport = camera->GetViewport();
		glViewport(viewport.x, viewport.y, (App::Window()->GetSize().x * viewport.z), (App::Window()->GetSize().y * viewport.w));

		// Actualizamos el Uniform Block "GlobalMatrix"
		glBindBuffer(GL_UNIFORM_BUFFER, mGlobalMatrixUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), Math::GetDataPtr(camera->GetPerspectiveMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), Math::GetDataPtr(camera->GetViewMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Renderizamos todas las entidades con un meshRenderer
		for(const auto &entity : entities)
		{
			MeshRenderer* meshRenderer = entity->Get<MeshRenderer>();
			Material* material = meshRenderer->GetMaterial();
			Mesh* mesh = meshRenderer->GetMesh();

			// TODO: ForwardRenderer: Check "material != nullptr"
			vec3 diffuseColor = material->GetDiffuseColor();
			vec3 specularColor = material->GetSpecularColor();
			float shininessFactor = material->GetShininessFactor();

			// Actualizamos el Uniform Block "Material"
			glBindBuffer(GL_UNIFORM_BUFFER, mMaterialUBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(vec3), Math::GetDataPtr(diffuseColor));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4), sizeof(vec3), Math::GetDataPtr(specularColor));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) * 2, sizeof(float), &shininessFactor);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			// TODO: ForwardRenderer: Render using default shader if material is not defined
			if(material != nullptr)
			{
				material->Use();

				if(material->GetShader() != nullptr)
				{
					auto modelMatrix = entity->Get<Transform>()->GetMatrix();
					material->GetShader()->SetUniform("modelToWorldMatrix", modelMatrix);

					// =======================================================================
					// TEMP: Lights
					// =======================================================================
					unsigned int lightCounter = 0;
					struct PerLight { vec4 lightPosType; vec4 lightIntensity; } lightsStruct[5];
					Light* light = nullptr;

					for(const auto &lightEntity : lights)
					{
						light = lightEntity->Get<Light>();

						lightsStruct[lightCounter].lightPosType = vec4(
							cameraEntity->Get<Transform>()->InverseTransformPoint(lightEntity->Get<Transform>()->GetPosition()),
							light->GetType() == LightType::LIGHT_POINT ? 1.f : 0.f
						);
						lightsStruct[lightCounter].lightIntensity = vec4((light->GetColor() * light->GetIntensity()), 1.f);

						if(++lightCounter == 5)
						{
							break;
						}
					}
					glBindBuffer(GL_UNIFORM_BUFFER, mLightUBO);
					glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(vec4) * 2 * 5, &lightsStruct);
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
					// =======================================================================
				}
			}

			// TODO: ForwardRenderer: Render default model if mesh is not defined
			if(mesh != nullptr)
			{
				mesh->Use();

				if(meshRenderer->GetForceDraw())
				{
					glDisable(GL_DEPTH_TEST);
				}

				glDrawElements(mesh->GetDrawMode(), mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);

				if(meshRenderer->GetForceDraw())
				{
					glEnable(GL_DEPTH_TEST);
				}
			}
		}
	}
}
}
