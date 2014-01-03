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

#include "texture.hpp"
#include "RessourcesManager.h"
#include <sstream>
//#include "Debug.h"

using namespace std;

namespace cg {
Texture::Texture() {
	mTextureId = -1;
	mTextureType = GL_TEXTURE_2D;
}

Texture::Texture(const std::string& path) {
	mTextureId = -1;
	mTextureType = GL_TEXTURE_2D;
	loadTexture(path);
}

Texture::Texture(const GLuint id) {
	mTextureId = id;
	mTextureType = GL_TEXTURE_2D;
	std::stringstream ss;
	ss << "Texture from id: " << id;
	mName = ss.str();
}

Texture::~Texture() {
	glDeleteTextures(1, &mTextureId);
}

bool Texture::loadTexture(const std::string &path) {
	mName = path;
	try {
		RessourcesManager::getInstance().loadImage(path);
	} catch (std::runtime_error &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}

	sf::Image* img = RessourcesManager::getInstance().getImage(path);
	return loadTexture(img, path);
}

bool Texture::loadTexture(sf::Image* img, const std::string& name) {
	if (img != 0) {
		mName = name;
		if (img->getSize().x % 2 != 0 || img->getSize().y % 2 != 0) {
			dwarn << "Warning: image size is not a multiple of 2, texture "
					<< name << " might cause troubles!" << std::endl;
		}

		glGenTextures(1, &mTextureId); //we need to create an unique ID for the texture
		// Store texture in VRAM or RAM (opengl handles it behind the scene).
		// There is no need to keep track of the image after that, opengl handles the data itself.
		uploadTexture(img);
		return true;
	} else {
		derr << "Unable to load texture " << name << std::endl;
		return false;
	}

}

void Texture::uploadTexture(sf::Image *img) {
	if (img != 0) {
		glBindTexture(GL_TEXTURE_2D, mTextureId); //we activate this id to tell opengl we are going to use this texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		/**
		 * To eliminate white edge effect
		 **/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// After this, there is no need to keep track of the image, it has been uploaded to the GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->getSize().x,
				img->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				img->getPixelsPtr());
	} else {
		cerr << "Invalid image " << mName << endl;
	}
}

/**
 * @brief Binds the texture.
 * It has to be done before glBegin!
 */
void Texture::bind() {
	dinf << "Texture::bind() " << mName << endl;
	glBindTexture(GL_TEXTURE_2D, mTextureId);
}

GLuint Texture::getTextureId() const {
	return mTextureId;
}
}
