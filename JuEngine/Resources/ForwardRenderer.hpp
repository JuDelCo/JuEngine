// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "Renderer.hpp"

namespace JuEngine
{
class JUENGINEAPI ForwardRenderer : public Renderer
{
	public:
		ForwardRenderer();
		~ForwardRenderer();

		void Render();

	protected:
		void RenderMeshNode(MeshNode* meshNode, Shader* shader);

	private:
		uint32_t mGlobalMatrixBindingIndex{0};
		uint32_t mGlobalMatrixUBO;
		//uint32_t mWorldBindingIndex{1};
		//uint32_t mWorldUBO;
		//uint32_t mMaterialBindingIndex{2};
		//uint32_t mMaterialUBO;
		//uint32_t mLightBindingIndex{3};
		//uint32_t mLightUBO;
};
}
