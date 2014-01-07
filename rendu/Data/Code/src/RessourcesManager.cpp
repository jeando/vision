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

#include "RessourcesManager.h"
#include "texture.hpp"
#include "Debug.h"

namespace cg {

/**
 * @brief Free all allocated ressources
 */
RessourcesManager::~RessourcesManager() {
	dinf << "Deleting all entities" << std::endl;

	dinf << "Deleting all ressources" << std::endl;
	std::map<std::string, sf::Image*>::const_iterator it;
	for (it = mImages.begin(); it != mImages.end(); it++) {
		dinf << "Deleting image " << it->first << std::endl;
		delete it->second;
	}

	dinf << "Deleting all textures" << std::endl;
	std::map<std::string, Texture*>::const_iterator texIt;
	for (texIt = mTextures.begin(); texIt != mTextures.end(); texIt++) {
		dinf << "Deleting image " << texIt->first << std::endl;
		delete texIt->second;
	}
}

/**
 * @brief Loads an image if it isn't already loaded
 *
 * @param path
 *        Path of the image
 *
 * @return
 *        true if a new image was loaded
 *        false if the given image was already loaded
 *
 * @throws
 *       std::exception if an error occured during loading
 */
bool RessourcesManager::loadImage(const std::string& path) {
	if (mImages.find(path) != mImages.end()) {
		std::cout << "Image " << path << " already loaded" << std::endl;
		return false;
	} else {
		sf::Image * img = new sf::Image();
		try {
			img->loadFromFile(path);
		} catch (...) {
			return false;
			throw new std::runtime_error(
					"Error: Image " + path + " loading failed!");
		}
		mImages[path] = img;
	}
	return true;
}
void RessourcesManager::addImage(sf::Image *img, const std::string& id) {
	dinf << "RessourcesManager: Image " << id << " added." << std::endl;
	mImages[id] = img;
}

Texture * RessourcesManager::loadTexture(const std::string &path) {
	dinf << "Loading texture " << path << std::endl;
	if (mTextures.find(path) != mTextures.end()) {
		dinf << "Texture " << path << " already loaded" << std::endl;
		return mTextures[path];
	}
	Texture *texture = new Texture();
	if (texture->loadTexture(path)) {
		dinf << "Texture " << path << " loaded" << std::endl;
		mTextures[path] = texture;
		return texture;
	} else {
		derr << "Error: texture " << path << " not loaded!" << std::endl;
		exit(0);
	}
	return 0;
}

bool RessourcesManager::addTexture(Texture *texture, const std::string &path) {
	if (mTextures.find(path) != mTextures.end()) {
		dinf << "Texture " << path << " already loaded" << std::endl;
		return false;
	} else {
		if (texture != 0) {
			dinf << "Texture " << path << " added." << std::endl;
			mTextures[path] = texture;
			return true;
		} else {
			derr << "Error: Unable to add texture: " << path << "! Null texture"
					<< std::endl;
			return false;
		}
	}

}

/**
 * @brief Returns an image
 *
 * @param path
 *        The path used during image loading
 *
 * @return
 *        A pointer to the image, or NULL if the image isn't loaded
 */
sf::Image* RessourcesManager::getImage(const std::string& path) const {
	std::map<std::string, sf::Image*>::const_iterator it = mImages.find(path);
	if (it != mImages.end()) {
		return it->second;
	} else {
		return 0;
	}
}

}

