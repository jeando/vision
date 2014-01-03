/******************************************************************************
 Copyright (C) 2012-2013  TANGUY Arnaud arn.tanguy@gmail.com
 &&  FAVREAU Jean-Dominique
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

/**
 * Inspired by
 * http://content.gpwiki.org/index.php/OpenGL:Tutorials:Loading_and_using_GLSL_shaders
 **/

#ifndef __Shader__
#define __Shader__

#include <memory>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "glm/glm.hpp"
#include "glm/ext.hpp"



namespace cg {

class Texture;

/**
 * @brief Provides an easy way to load, enable and disable shaders (vertex,
 * geometry, fragment)
 * Uses the glew library, don't forget to initialize it!
 * This class only depends on Glew and Opengl
 */
class Shader {
private:
	std::string mShaderName;
	GLenum shader_type;
	GLhandleARB mVertexHandle;
	GLhandleARB mFragmentHandle;
	GLhandleARB mGeometryHandle;
	GLhandleARB mProgramHandle;

	GLuint vao;
	std::map<std::string,GLuint> buffer_vbo;
	// uniform ID, texture_id
	std::map<GLuint,std::shared_ptr<Texture>> m_textures;
	void bindTextures();

	std::string loadFile(const std::string &str);
	GLhandleARB createSharedObjects(const std::string shader,
			GLenum shaderType);
	void linkShader(GLhandleARB);

	void init();

	int getVariableId(const std::string& strVariable) const;


    void printLog(GLuint object);


public:
	Shader();
	Shader(const std::string &vertexShaderPath,
			const std::string &fragmentShaderPath);
	virtual ~Shader();

    void loadFromFile(const std::string& vertex, const std::string& fragment);
    void loadFromMemory(const std::string& vertex, const std::string& fragment);

	void loadVertexShaderFromFile(const std::string &);
	void loadVertexShader(const std::string &);

	void loadFragmentShaderFromFile(const std::string &);
	void loadFragmentShader(const std::string &);

    void loadGeometryShaderFromFile(const std::string &shader);
    void loadGeometryShader(const std::string &shader);

    void setName(const std::string& name) {
        mShaderName = name;
    }

	void enable();
	static void disable();

	void setTexture(const std::string &uniformLocation, GLuint texture);
	void setTexture(const std::string &uniformLocation, const std::shared_ptr<Texture>& texture);
	void clear_texture();

	void setFloat(const std::string &uniformVarName, float value);
	void setInt(const std::string &uniformVarName, int value);
	void setVec2(const std::string &uniformVarName, const glm::vec2& vector);
	void setVec3(const std::string &uniformVarName, const glm::vec3& vector);
	void setVec4(const std::string &uniformVarName, const glm::vec4& vector);
	void setMat4(const std::string &uniformVarName, const glm::mat4& mat);

	void setFloatArray(const std::string &uniformVarName,
			const std::vector<float>& f);
	void setVec3Array(const std::string &uniformVarName,
			const std::vector<sf::Vector3f>& f);

	bool hasShaderSupport();

	GLhandleARB getProgramHandle() const;


    operator GLuint() { return mProgramHandle; }
    void operator()() {  enable();  }
	void set_in_f(int n, const std::string &in_name,
		const std::vector<float>& pos) ;
};

}

#endif
