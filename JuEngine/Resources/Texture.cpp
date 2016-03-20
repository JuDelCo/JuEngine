// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Texture.hpp"
#include "../App.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STBI_ONLY_PNG
#define STBI_ONLY_HDR
#define STB_IMAGE_IMPLEMENTATION
#include "../ImGui/stb_image.h"

namespace JuEngine
{
Texture::Texture(const std::string& texturePath, const bool generateMipMaps)
{
	static auto shadersPath = "Assets/Textures/";

	glGenTextures(1, &mTexture);

	glBindTexture(GL_TEXTURE_2D, mTexture);

	int componentsPerPixel;
	unsigned char *image = stbi_load((shadersPath + texturePath).c_str(), &mHeight, &mWidth, &componentsPerPixel, 0 /* STBI_rgb_alpha == 4 */);

	if(image == nullptr)
	{
		App::Log()->Error("Error: Failed to load image texture '%s': %s", texturePath.c_str(), stbi_failure_reason());

		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &mTexture);

		return;
	}

	if(componentsPerPixel == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	else if(componentsPerPixel == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	else
	{
		App::Log()->Error("Error: Error loading an image with 3 or 4 components per pixel '%s', image has %u", texturePath.c_str(), componentsPerPixel);
	}

	stbi_image_free(image);

	if(generateMipMaps)
	{
		GenerateMipMaps();
	}

	SetTextureWrapping(TextureWrappingMode::Repeat);
	SetTextureMinFiltering(TextureFilteringMode::Nearest);
	SetTextureMagFiltering(TextureFilteringMode::Nearest);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mTexture);
}

void Texture::Use(unsigned int unit)
{
	ChangeTextureUnit(unit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture::DisableTextures(unsigned int unit)
{
	ChangeTextureUnit(unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::GetHeight() -> unsigned int
{
	return mHeight;
}

auto Texture::GetWidth() -> unsigned int
{
	return mWidth;
}

auto Texture::GenerateMipMaps() -> Texture*
{
	glGenerateMipmap(GL_TEXTURE_2D);

	return this;
}

auto Texture::SetTextureWrapping(const TextureWrappingMode mode) -> Texture*
{
	unsigned int glMode = GL_REPEAT;

	switch (mode)
	{
		case TextureWrappingMode::Repeat:
			glMode = GL_REPEAT;
			break;
		case TextureWrappingMode::MirroredRepeat:
			glMode = GL_MIRRORED_REPEAT;
			break;
		case TextureWrappingMode::ClampToEdge:
			glMode = GL_CLAMP_TO_EDGE;
			break;
		case TextureWrappingMode::ClampToBorder:
			glMode = GL_CLAMP_TO_BORDER;
			break;
		default:
			break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glMode);

	return this;
}

auto Texture::SetTextureWrappingBorder(const vec4 color) -> Texture*
{
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, Math::GetDataPtr(color));

	return this;
}

auto Texture::SetTextureMinFiltering(const TextureFilteringMode mode) -> Texture*
{
	unsigned int glMode = GL_NEAREST;

	switch (mode)
	{
		case TextureFilteringMode::Nearest:
			glMode = GL_NEAREST;
			break;
		case TextureFilteringMode::Linear:
			glMode = GL_LINEAR;
			break;
		case TextureFilteringMode::MipMapNearestNearest:
			glMode = GL_NEAREST_MIPMAP_NEAREST;
			break;
		case TextureFilteringMode::MipMapNearestLinear:
			glMode = GL_NEAREST_MIPMAP_LINEAR;
			break;
		case TextureFilteringMode::MipMapLinearNearest:
			glMode = GL_LINEAR_MIPMAP_NEAREST;
			break;
		case TextureFilteringMode::MipMapLinearLinear:
			glMode = GL_LINEAR_MIPMAP_LINEAR;
			break;
		default:
			break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMode);

	return this;
}

auto Texture::SetTextureMagFiltering(const TextureFilteringMode mode) -> Texture*
{
	unsigned int glMode = GL_NEAREST;

	switch (mode)
	{
		case TextureFilteringMode::Nearest:
			glMode = GL_NEAREST;
			break;
		case TextureFilteringMode::Linear:
			glMode = GL_LINEAR;
			break;
		default:
			break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMode);

	return this;
}

void Texture::ChangeTextureUnit(unsigned int unit)
{
	if(unit >= 16)
	{
		unit = 0;
	}

	glActiveTexture(GL_TEXTURE0 + (0x1 * unit));
}
}
