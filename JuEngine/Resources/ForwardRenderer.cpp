// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "ForwardRenderer.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "../Components/Camera.hpp"
#include "../Components/Light.hpp"
#include "../Components/MeshRenderer.hpp"
#include "../Components/Transform.hpp"
#include "../Components/World.hpp"
#include "../Entity/Pool.hpp"
#include "../Entity/Group.hpp"
#include "../App.hpp"
#include "../Services/IWindowService.hpp"
#include <GL/glew.h>
#include <sstream>

namespace JuEngine
{
static vec4 lastGlClearColor = vec4(0.f, 0.f, 0.f, 0.f);
static vec4 lastGlViewport = vec4(0.f, 0.f, 0.f, 0.f);
static vec2 lastWindowSize = vec2(0.f, 0.f);

ForwardRenderer::ForwardRenderer()
{
	SetId("forwardRenderer");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_POINT GL_LINE

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.f, 1.f);
	//glEnable(GL_DEPTH_CLAMP);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClearDepth(1.f);

	// ----------------

	glGenBuffers(1, &mGlobalMatrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, mGlobalMatrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, mGlobalMatrixBindingIndex, mGlobalMatrixUBO, 0, sizeof(mat4) * 2);

	// ----------------

	/*glGenBuffers(1, &mWorldUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, mWorldUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(vec4) + sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, mWorldBindingIndex, mWorldUBO, 0, sizeof(vec4) + sizeof(float) * 3);*/

	// ----------------

	/*glGenBuffers(1, &mMaterialUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, mMaterialUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(vec4) * 2 + sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, mMaterialBindingIndex, mMaterialUBO, 0, sizeof(vec4) * 2 + sizeof(float));*/

	// ----------------

	/*glGenBuffers(1, &mLightUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, mLightUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(vec4) * 2 * 5, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, mLightBindingIndex, mLightUBO, 0, sizeof(vec4) * 2 * 5);*/
}

ForwardRenderer::~ForwardRenderer()
{
	glDeleteBuffers(1, &mGlobalMatrixUBO);
	//glDeleteBuffers(1, &mWorldUBO);
	//glDeleteBuffers(1, &mMaterialUBO);
	//glDeleteBuffers(1, &mLightUBO);
}

void ForwardRenderer::Render()
{
	if(mPools.size() == 0)
	{
		return;
	}

	// TODO: ForwardRenderer: Cache entities
	std::stringstream ss;
	std::vector<EntityPtr> cameras;
	std::vector<EntityPtr> entities;
	std::vector<EntityPtr> lights;
	World* world = nullptr;

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

		world = pool->GetGroup(Matcher_AllOf(World))->GetSingleEntity()->Get<World>();

		float gammaCorrection = 1.f / world->GetGammaCorrection();

		// Actualizamos el Uniform Block "World"
		/*glBindBuffer(GL_UNIFORM_BUFFER, mWorldUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(vec3), Math::GetDataPtr(world->GetAmbientColor()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4), sizeof(float), &world->GetAmbientIntensity());
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) + sizeof(float), sizeof(float), &world->GetLightAttenuation());
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) + sizeof(float) * 2, sizeof(float), &gammaCorrection);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);*/

		// Actualizamos el valor que usará la limpieza de buffer de color
		auto skyColor = world->GetSkyColor();
		if(lastGlClearColor != vec4(skyColor, 1.f))
		{
			glClearColor(pow(skyColor.x,gammaCorrection), pow(skyColor.y,gammaCorrection), pow(skyColor.z,gammaCorrection), 1.f);
			lastGlClearColor = vec4(skyColor, 1.f);
		}

		break;
	}

	// Limpiamos los buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Dibujamos la escena por cada cámara activa
	for(const auto &cameraEntity : cameras)
	{
		// Actualizamos el viewport dependiendo de la cámara y el tamaño de la pantalla
		auto camera = cameraEntity->Get<Camera>();
		vec2 windowSize = App::Window()->GetSize();
		camera->SetScreenSize(windowSize);
		vec4 viewport = camera->GetViewport();
		if(viewport != lastGlViewport || windowSize != lastWindowSize)
		{
			glViewport(viewport.x, viewport.y, (windowSize.x * viewport.z), (windowSize.y * viewport.w));
			lastGlViewport = viewport;
			lastWindowSize = windowSize;
		}

		// Actualizamos el Uniform Block "GlobalMatrix"
		glBindBuffer(GL_UNIFORM_BUFFER, mGlobalMatrixUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), Math::GetDataPtr(camera->GetPerspectiveMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), Math::GetDataPtr(camera->GetViewMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Actualizamos el Uniform Block "Light"
		/*unsigned int lightCounter = 0;
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
		glBindBuffer(GL_UNIFORM_BUFFER, 0);*/

		// Renderizamos todas las entidades con un meshRenderer
		for(const auto &entity : entities)
		{
			MeshRenderer* meshRenderer = entity->Get<MeshRenderer>();
			Shader* shader = meshRenderer->GetShader();
			MeshNode* rootMeshNode = meshRenderer->GetMeshNode();

			if(shader != nullptr)
			{
				shader->Use();

				shader->SetUniform("modelToWorldMatrix", entity->Get<Transform>()->GetMatrix());

				// TEMP (World):
				if(world)
				{
					shader->SetUniform("world.ambient", world->GetAmbientColor() * world->GetAmbientIntensity());
				}

				// TEMP (Others):
				shader->SetUniform("normalMatrix", mat3(glm::transpose(glm::inverse(entity->Get<Transform>()->GetMatrix()))));
				shader->SetUniform("cameraPosition", cameraEntity->Get<Transform>()->GetPosition());
				//shader->SetUniform("lightPosition", vec3(lights[0]->Get<Transform>()->GetPosition())); // Gouraud Shading

				// TEMP (Lights):
				unsigned int lightDirCounter = 0;
				unsigned int lightPointCounter = 0;
				unsigned int lightSpotCounter = 0;
				unsigned int maxLightDir = 2;
				unsigned int maxLightPoint = 5;
				unsigned int maxLightSpot = 1;
				Light* light = nullptr;
				for(const auto &lightEntity : lights)
				{
					light = lightEntity->Get<Light>();

					if(light->GetType() == LightType::LIGHT_DIRECTIONAL)
					{
						if(lightDirCounter >= maxLightDir)
						{
							continue;
						}

						ss.str(std::string());
						ss << "dirLights[" << lightDirCounter << "].";
						shader->SetUniform(ss.str() + "direction", cameraEntity->Get<Transform>()->InverseTransformDirection(lightEntity->Get<Transform>()->Forward()));
						shader->SetUniform(ss.str() + "color", light->GetColor() * light->GetIntensity());

						++lightDirCounter;
					}
					else if(light->GetType() == LightType::LIGHT_POINT)
					{
						if(lightPointCounter >= maxLightPoint)
						{
							continue;
						}

						ss.str(std::string());
						ss << "pointLights[" << lightPointCounter << "].";
						shader->SetUniform(ss.str() + "position", cameraEntity->Get<Transform>()->InverseTransformPoint(lightEntity->Get<Transform>()->GetPosition()));
						shader->SetUniform(ss.str() + "color", light->GetColor() * light->GetIntensity());
						shader->SetUniform(ss.str() + "constant", 1.0f);
						shader->SetUniform(ss.str() + "linear", light->GetLinearAttenuation());
						shader->SetUniform(ss.str() + "quadratic", light->GetQuadraticAttenuation());

						++lightPointCounter;
					}
					else if(light->GetType() == LightType::LIGHT_SPOT)
					{
						if(lightSpotCounter >= maxLightSpot)
						{
							continue;
						}

						ss.str(std::string());
						ss << "spotLights[" << lightSpotCounter << "].";
						shader->SetUniform(ss.str() + "position", cameraEntity->Get<Transform>()->InverseTransformPoint(lightEntity->Get<Transform>()->GetPosition()));
						shader->SetUniform(ss.str() + "color", light->GetColor() * light->GetIntensity());
						shader->SetUniform(ss.str() + "constant", 1.0f);
						shader->SetUniform(ss.str() + "linear", light->GetLinearAttenuation());
						shader->SetUniform(ss.str() + "quadratic", light->GetQuadraticAttenuation());
						shader->SetUniform(ss.str() + "direction", cameraEntity->Get<Transform>()->InverseTransformDirection(lightEntity->Get<Transform>()->Forward()));
						shader->SetUniform(ss.str() + "cutOff", light->GetSpotCutOff());
						shader->SetUniform(ss.str() + "outerCutOff", light->GetSpotOuterCutOff());

						++lightSpotCounter;
					}
				}

				// Set to zero all remaining light uniforms
				for(unsigned int i = lightDirCounter; i < maxLightDir; ++i)
				{
					ss.str(std::string());
					ss << "dirLights[" << i << "].";
					shader->SetUniform(ss.str() + "direction", vec3(0.f, 0.f, 1.f));
					shader->SetUniform(ss.str() + "color", vec3(0.f, 0.f, 0.f));
				}
				for(unsigned int i = lightPointCounter; i < maxLightPoint; ++i)
				{
					ss.str(std::string());
					ss << "pointLights[" << i << "].";
					shader->SetUniform(ss.str() + "position", vec3(0.f, 0.f, 0.f));
					shader->SetUniform(ss.str() + "color", vec3(0.f, 0.f, 0.f));
					shader->SetUniform(ss.str() + "constant", 1.0f);
					shader->SetUniform(ss.str() + "linear", 0.09f);
					shader->SetUniform(ss.str() + "quadratic", 0.032f);
				}
				for(unsigned int i = lightSpotCounter; i < maxLightSpot; ++i)
				{
					ss.str(std::string());
					ss << "spotLights[" << i << "].";
					shader->SetUniform(ss.str() + "position", vec3(0.f, 0.f, 0.f));
					shader->SetUniform(ss.str() + "color", vec3(0.f, 0.f, 0.f));
					shader->SetUniform(ss.str() + "constant", 1.0f);
					shader->SetUniform(ss.str() + "linear", 0.09f);
					shader->SetUniform(ss.str() + "quadratic", 0.032f);
					shader->SetUniform(ss.str() + "direction", vec3(0.f, 0.f, 1.f));
					shader->SetUniform(ss.str() + "cutOff", 0.9f);
					shader->SetUniform(ss.str() + "outerCutOff", 0.82f);
				}
			}

			this->RenderMeshNode(rootMeshNode, shader);
		}
	}
}

void ForwardRenderer::RenderMeshNode(MeshNode* meshNode, Shader* shader)
{
	for(auto &mesh : meshNode->GetMeshList())
	{
		mesh->Use(shader);

		glDrawElements(Mesh::GetDrawModeGL(mesh->GetDrawMode()), mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
	}

	for(auto &childMeshNode : meshNode->GetMeshNodeList())
	{
		this->RenderMeshNode(childMeshNode, shader);
	}
}
}
