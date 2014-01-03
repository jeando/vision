/******************************************************************************
 Copyright (C) 2012-2013  TANGUY Arnaud arn.tanguy@gmail.com
 *                                                                             *
 * This program is free software; you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by        *
 * the Free Software Foundation; either version 2 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * This program is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program; if not, write to the Free Software Foundation, Inc.,     *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 ******************************************************************************/

#ifndef __Texture__
#define __Texture__

#include <iostream>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <SFML/OpenGL.hpp>

namespace cg {
/**
 * @brief Manages texture loading and passing textures to opengl
 *
 * @description This class uses SFML to load images.
 * Then it creates the corresponding opengl textures.
 * It can the be bound by calling bind, which will effectively bind the texture to the
 * GL_TEXTURE_2D unit
 *
 * See http://www.gamedev.net/page/resources/_/technical/opengl/opengl-texture-mapping-an-introduction-r947
 * for details about opengl textures
 */
class Texture {
protected:
	GLuint mTextureId;
	std::string mName;
	GLuint mTextureType;
	void uploadTexture(sf::Image *img);
public:
	Texture();
	Texture(const std::string& path);
	Texture(const GLuint id);
	virtual ~Texture();

	bool loadTexture(const std::string& path);
	bool loadTexture(sf::Image *img, const std::string& name);
	void bind();
	GLuint getTextureId() const;

	std::string getName() {
		return mName;
	}
	GLuint getTextureType() const {
		return mTextureType;
	}
};
}

#endif
