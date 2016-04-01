// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Math.hpp"

namespace JuEngine
{
enum class TextureWrappingMode
{
	Repeat,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder
};

enum class TextureFilteringMode
{
	Nearest,
	Linear,
	MipMapNearestNearest,
	MipMapNearestLinear,
	MipMapLinearNearest,
	MipMapLinearLinear
};

class JUENGINEAPI Texture : public IObject
{
	public:
		Texture(const std::string& texturePath, const bool generateMipMaps = true);
		~Texture();

		void Use(unsigned int unit = 0);
		static void DisableTextures(unsigned int unit = 0);

		auto GetHeight() const -> unsigned int;
		auto GetWidth() const -> unsigned int;
		auto GetPath() const -> const std::string&;

		auto GenerateMipMaps() -> Texture*;
		auto SetTextureWrapping(const TextureWrappingMode mode) -> Texture*;
		auto SetTextureWrappingBorder(const vec4 color) -> Texture*;
		auto SetTextureMinFiltering(const TextureFilteringMode mode) -> Texture*;
		auto SetTextureMagFiltering(const TextureFilteringMode mode) -> Texture*;

	private:
		static void ChangeTextureUnit(unsigned int unit);

		uint32_t mTexture{0};
		int mHeight{0};
		int mWidth{0};
		std::string mPath;
};
}
