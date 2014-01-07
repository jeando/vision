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

#ifndef __RessourcesManager__
#define __RessourcesManager__

#include <map>
#include <stdexcept>
#include <iostream>

#include <SFML/Graphics/Image.hpp>
#include "Debug.h"

#define R() cg::RessourcesManager::getInstance()

namespace cg {

class Texture;

/**
 * @brief Manages the ressources.
 * Each ressource is identified by an unique string (path, name...)
 */
class RessourcesManager {
private:
	inline explicit RessourcesManager() {
	}
	~RessourcesManager();
	inline explicit RessourcesManager(RessourcesManager const&) {
	}
	inline RessourcesManager& operator=(RessourcesManager const&) {
		return *this;
	}

protected:
	std::map<std::string, sf::Image*> mImages;
	std::map<std::string, Texture*> mTextures;

public:
	inline static RessourcesManager& getInstance() {
		static RessourcesManager instance;
		return instance;
	}

	bool loadImage(const std::string& path);
	void addImage(sf::Image *, const std::string& id);
	sf::Image* getImage(const std::string& path) const;

	Texture* loadTexture(const std::string &path);
	bool addTexture(Texture* texture, const std::string &path);
};

}

#endif
