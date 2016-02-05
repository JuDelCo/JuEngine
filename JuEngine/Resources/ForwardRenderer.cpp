// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "ForwardRenderer.hpp"
#include "../ECS/EntityManager.hpp"
#include "../Managers/WindowManager.hpp"
#include "../ECS/Entity.hpp"
#include "Shader.hpp"
#include "../Components/Camera.hpp"
#include "../Components/Light.hpp"
#include "../Components/MeshRenderer.hpp"
#include "../Components/Transform.hpp"
#include "../Components/World.hpp"
#include "../Math.hpp"

namespace JuEngine
{
ForwardRenderer::ForwardRenderer()
{
	SetName("forwardRenderer");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	//glEnable(GL_DEPTH_CLAMP);

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClearDepth(1.0f);

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
	float gammaCorrection = 1.0f / EntityManager::GetWorld()->GetGammaCorrection();

	// Actualizamos el Uniform Block "World"
	glBindBuffer(GL_UNIFORM_BUFFER, mWorldUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(vec3), Math::GetDataPtr(EntityManager::GetWorld()->GetAmbientColor()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4), sizeof(float), &EntityManager::GetWorld()->GetAmbientIntensity());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) + sizeof(float), sizeof(float), &EntityManager::GetWorld()->GetLightAttenuation());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) + sizeof(float) * 2, sizeof(float), &gammaCorrection);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Limpiamos los buffers de color y profundidad
	auto skyColor = EntityManager::GetWorld()->GetSkyColor();
	glClearColor(pow(skyColor.x,gammaCorrection), pow(skyColor.y,gammaCorrection), pow(skyColor.z,gammaCorrection), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec2 screenSize = WindowManager::GetSize();

	// Dibujamos la escena por cada cámara activa
	auto cameras = EntityManager::GetEntities<Camera>();
	for(auto& cameraEntity : cameras)
	{
		// TODO: "continue;" si la cámara está inactiva (check por componente?)

		// Actualizamos el viewport dependiendo de la cámara y el tamaño de la pantalla
		auto camera = cameraEntity->GetComponent<Camera>();
		camera->SetScreenSize(screenSize);
		vec4 viewport = camera->GetViewport();
		glViewport(viewport.x, viewport.y, (screenSize.x * viewport.z), (screenSize.y * viewport.w));

		// Actualizamos el Uniform Block "GlobalMatrix"
		glBindBuffer(GL_UNIFORM_BUFFER, mGlobalMatrixUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), Math::GetDataPtr(camera->GetPerspectiveMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), Math::GetDataPtr(camera->GetViewMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Renderizamos todas las entidades con un meshRenderer
		auto entities = EntityManager::GetEntities<MeshRenderer>();
		for(auto& entity : entities)
		{
			MeshRenderer* meshRenderer = entity->GetComponent<MeshRenderer>();
			Material* material = meshRenderer->GetMaterial();
			Mesh* mesh = meshRenderer->GetMesh();

			vec3 diffuseColor = material->GetDiffuseColor();
			vec3 specularColor = material->GetSpecularColor();
			float shininessFactor = material->GetShininessFactor();

			// Actualizamos el Uniform Block "Material"
			glBindBuffer(GL_UNIFORM_BUFFER, mMaterialUBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(vec3), Math::GetDataPtr(diffuseColor));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4), sizeof(vec3), Math::GetDataPtr(specularColor));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) * 2, sizeof(float), &shininessFactor);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			// TODO: ForwardRenderer: Renderizar usando shader por defecto si no se encuentra
			if(material)
			{
				material->Use();

				if(material->GetShader())
				{
					auto modelMatrix = entity->GetComponent<Transform>()->GetMatrix();
					material->GetShader()->SetUniform("modelToWorldMatrix", modelMatrix);

					// =======================================================================
					// TEMP: Lights
					struct PerLight { vec4 lightPosType; vec4 lightIntensity; } lightsStruct[5];
					auto lights = EntityManager::GetEntities<Light>();
					unsigned int lightCounter = 0;
					for(auto& light : lights)
					{
						lightsStruct[lightCounter].lightPosType = vec4(
							cameraEntity->GetComponent<Transform>()->InverseTransformPoint(light->GetComponent<Transform>()->GetPosition()),
							light->GetComponent<Light>()->GetType() == LightType::LIGHT_POINT ? 1.0f : 0.f);
						lightsStruct[lightCounter].lightIntensity = vec4((
							light->GetComponent<Light>()->GetColor() *
							light->GetComponent<Light>()->GetIntensity()
							), 1.0f);
						lightCounter++;
						if(lightCounter == 5) break;
					}
					glBindBuffer(GL_UNIFORM_BUFFER, mLightUBO);
					glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(vec4) * 2 * 5, &lightsStruct);
					glBindBuffer(GL_UNIFORM_BUFFER, 0);
					// =======================================================================
				}
			}

			// TODO: ForwardRenderer: Renderizar modelo por defecto si no se encuentra
			if(mesh)
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
