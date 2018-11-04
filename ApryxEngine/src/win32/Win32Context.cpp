#include "Win32Context.h"

#include "opengl/GLTexture.h"
#include "opengl/GLSurface.h"

#include "console/Console.h"
#include "graphics/FontUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace apryx {
	Win32Context::Win32Context(std::shared_ptr<Window> window)
		: m_Window(window)
	{
	}

	std::shared_ptr<Font> Win32Context::getDefaultFont()
	{
		if (m_DefaultFont == nullptr)
			m_DefaultFont = createDefaultFont(*this);

		return m_DefaultFont;
	}

	std::shared_ptr<Texture> Win32Context::createTexture()
	{
		return std::make_shared<GLTexture>();
	}

	std::shared_ptr<Texture> Win32Context::loadTexture(std::string path)
	{
		auto it = m_Textures.find(path);
		
		// Doesn't exist or is outdated
		if (it != m_Textures.end()) {
			auto shared = it->second.lock();
			
			if (shared) {
				return shared;
			}
		}

		Image image = loadImage(path);
		auto texture = createTexture();
		texture->setData(image);

		m_Textures[path] = texture;

		return texture;
	}

	Image Win32Context::loadImage(std::string path)
	{
		int width, height, channels;

		stbi_set_flip_vertically_on_load(false);

		unsigned char *image = stbi_load(path.c_str(),
			&width,
			&height,
			&channels,
			STBI_rgb_alpha);

		if (image == nullptr) {
			Console::logError("Failed to load image");
			Console::logError(path);
			return Image::colored(1, 1, Color32::magenta());
		}
		if (channels != 4) {
			Console::logError("Image has to few components");
			Console::logError(path);

			return Image::colored(1, 1, Color32::magenta());
			stbi_image_free(image);
		}

		Image img(width, height);

		int index = 0;
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				Color32 c = Color32::white();

				// Can be done in one memcopy, from image to image.
				c.r = image[index + 0];
				c.g = image[index + 1];
				c.b = image[index + 2];
				c.a = image[index + 3];

				img.setColor(i, j, c);

				index += channels;
			}
		}

		stbi_image_free(image);

		return std::move(img);
	}
	std::shared_ptr<Surface> Win32Context::createSurface(int width, int height)
	{
		return std::make_shared<GLSurface>(width, height);
	}
}
