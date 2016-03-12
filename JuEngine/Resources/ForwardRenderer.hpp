// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "Renderer.hpp"

typedef unsigned int GLuint;

namespace JuEngine
{
class JUENGINEAPI ForwardRenderer : public Renderer
{
	public:
		ForwardRenderer();
		~ForwardRenderer();

		void Render();

	private:
		GLuint mGlobalMatrixBindingIndex{0};
		GLuint mGlobalMatrixUBO;
		GLuint mWorldBindingIndex{1};
		GLuint mWorldUBO;
		GLuint mMaterialBindingIndex{2};
		GLuint mMaterialUBO;
		GLuint mLightBindingIndex{3};
		GLuint mLightUBO;
};
}
