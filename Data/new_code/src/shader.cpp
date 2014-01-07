/******************************************************************************
 Copyright (C) 2012-2013  TANGUY Arnaud arn.tanguy@gmail.com
  && FAVREAU Jean-Dominique
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

#include <GL/glew.h>
#include "shader.hpp"
#include "texture.hpp"
//#include "Debug.h"

using namespace std;

namespace cg {

/**
 * @brief Creates a shader class
 * @throws runtime_error if the shaders aren't supported
 */
Shader::Shader() {
	init();
}


/**
 * @brief Loads a vertex and fragment shader from filesystem
 *
 * @param vertexShaderPath
 *      Path to the vertex shader
 * @param fragmentShaderPath
 *      Path to the fragment shader
 */
Shader::Shader(const std::string &vertexShaderPath,
		const std::string &fragmentShaderPath) {
	init();
    loadFromFile(vertexShaderPath, fragmentShaderPath);
}

Shader::~Shader() {
	// To free a shader we need to detach the vertex and fragment
	// shader pointers from the program object, then free each one.
	// Once that is done we can finally delete the program object.

	// If our vertex shader pointer is valid, free it
	if (mVertexHandle) {
		glDetachShader(mProgramHandle, mVertexHandle);
		glDeleteShader(mVertexHandle);
		mVertexHandle = 0;
	}

	// If our fragment shader pointer is valid, free it
	if (mFragmentHandle) {
		glDetachShader(mProgramHandle, mFragmentHandle);
		glDeleteShader(mFragmentHandle);
		mFragmentHandle = 0;
	}

	// If our geometry shader pointer is valid, free it
	if (mGeometryHandle) {
		glDetachShader(mProgramHandle, mGeometryHandle);
		glDeleteShader(mGeometryHandle);
		mGeometryHandle = 0;
	}

	// If our program object pointer is valid, free it
	if (mProgramHandle) {
		glDeleteShader(mProgramHandle);
		mProgramHandle = 0;
	}
	for(const auto& a : buffer_vbo)
	{
		glDeleteBuffers(1,&a.second);
	}
	buffer_vbo.clear();
	if(vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao=0;
	}
}

void Shader::loadFromFile(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::string vertex = loadFile(vertexShaderPath);
    std::string fragment = loadFile(fragmentShaderPath);
	loadVertexShader(vertex);
	loadFragmentShader(fragment);
}

void Shader::loadFromMemory(const std::string& vertex, const std::string& fragment)
{
	loadVertexShader(vertex);
	loadFragmentShader(fragment);
}

/**
 * @brief Checks for shader support
 */
void Shader::init() {
	if (hasShaderSupport()) {
		//Create a mProgramHandle handle.
		mProgramHandle = glCreateProgram();
		mVertexHandle = 0;
		mFragmentHandle = 0;
        mGeometryHandle = 0;
		vao=0;

	} else {
		throw std::runtime_error(
				"Error: GL Shaders not supported or Glew not initialized properly!");
	}
}

std::string Shader::loadFile(const std::string &str) {
	//First, let us load the vertex shader.
	std::fstream dataFile(str.c_str(), std::ios::in);
	std::string data;

	if (dataFile.is_open()) {
		//This is to help store the file's buffer.
		std::stringstream buffer;
		//Here, get all of the file's data by streaming the file's stream into our buffer stream.
		buffer << dataFile.rdbuf();
		//Now, buffer contains the vertex shader's data
		data = buffer.str();
	} else {
		throw std::runtime_error("Error: Unable to load shader file " + str);
	}
	if (mShaderName.length() == 0)
		mShaderName = str;
	return data;
}

/**
 * @brief Loads a vertex shader
 *
 * @param vertexPath
 *      Vertex program
 *
 */
void Shader::loadVertexShaderFromFile(const std::string &vertexShader) {
    loadVertexShader(loadFile(vertexShader));
}

/**
 * @brief Loads a fragment shader
 *
 * @param fragmentShader
 *      Fragment program
 */
void Shader::loadFragmentShaderFromFile(const std::string &fragmentShader) {
    loadFragmentShader(loadFile(fragmentShader));
}
/**
 * @brief Loads a vertex shader
 *
 * @param vertexPath
 *      Vertex program
 *
 */
void Shader::loadVertexShader(const std::string &vertexShader) {
	mVertexHandle = createSharedObjects(vertexShader, GL_VERTEX_SHADER);
	linkShader(mVertexHandle);
}

/**
 * @brief Loads a fragment shader
 *
 * @param fragmentShader
 *      Fragment program
 */
void Shader::loadFragmentShader(const std::string &fragmentShader) {
	mFragmentHandle = createSharedObjects(fragmentShader, GL_FRAGMENT_SHADER);
	linkShader(mFragmentHandle);
}

/**
 * @brief Loads a geometry shader from path
 *
 * @param shaderPath
 *      Path of the geometric shader
 */
void Shader::loadGeometryShaderFromFile(const std::string &shaderPath) {
    loadGeometryShader(loadFile(shaderPath));
}


void Shader::loadGeometryShader(const std::string &shader) {
	mGeometryHandle = createSharedObjects(shader, GL_GEOMETRY_SHADER);
	linkShader(mGeometryHandle);
}

/**
 * @brief: Display compilation errors from the OpenGL shader compiler
 * WARNING: This doesn't display glErrors which might still happen depending on
 * the state machine's state.
 */
void Shader::printLog(GLuint object)
{
    GLint log_length = 0;
    if (glIsShader(object))
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else if (glIsProgram(object))
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else {
        std::cerr <<  "Shader::printLog: Not a shader or a program" << std::endl;
        return;
    }

    char* log = new char[log_length];

    if (glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, log);

    std::cerr << log << std::endl;
    delete log;
}

/**
 * @brief Compiles the given shader
 *
 * @param shader
 *        A string containing the shader program
 * @param shaderType
 *        Shader type. Must be one of GL_COMPUTE_SHADER, GL_VERTEX_SHADER,
 *        GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER or GL_FRAGMENT_SHADER
 *
 * @return
 *        The handle of the built shader
 */
GLuint Shader::createSharedObjects(const std::string shader,
        GLenum shaderType) {

    //Obtain a valid handle to a vertex shader object.
    GLuint shaderHandle = glCreateShader(shaderType);

    if (!shaderHandle) {
        //We have failed creating the vertex shader object.
        std::cerr << "Shader::createSharedObjects: Failed creating shader object";
    }

    //Now, compile the shader source.
    //Note that glShaderSource takes an array of chars. This is so that one can load multiple vertex shader files at once.
    //This is similar in function to linking multiple C++ files together. Note also that there can only be one "void main" definition
    //In all of the linked source files that are compiling with this funciton.
    const char *te = (shader.c_str());
    glShaderSource(shaderHandle, //The handle to our shader
            1, //The number of files.
            &te, //An array of const char * data, which represents the source code of the shaders
            NULL); //An array of string lengths. For have null terminated strings, pass NULL.

    //Attempt to compile the shader.
    glCompileShader(shaderHandle);

    GLint compile_ok = GL_FALSE;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE) {
        std::cerr << "Shader::createSharedObjects: Shader " + shader + " failed compilation." << std::endl;
        printLog(shaderHandle);
        glDeleteShader(shaderHandle);
        return 0;
    }

	//Attach the shaders.
    // This is very similar to the linking process in C, only the attached objects take part of the linking process.
	glAttachShader(mProgramHandle, shaderHandle);

    return shaderHandle;
}

/**
 * @brief Links the shader object to the shader program
 * The shader has to be built first by calling Shader::createSharedObjects
 *
 * @param shaderHandle
 *    Handle of the shader to link
 */
void Shader::linkShader(GLuint shaderHandle) {

	//Link the mProgramHandle.
	glLinkProgram(mProgramHandle);

    GLint Success;
    glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        printLog(mProgramHandle);
    }
	//Now the program can be used.
}

/**
 * @brief Enables the shader. This will effectively replace the render pipeline functions by
 * the program shader
 * In case the geometry shader fails
 * - Check whether the input geometry primitive type sent by the draw call is
 *   the same as the shader's
 */
void Shader::enable() {
    // Validate the program
    /**
     * XXX: do only during developpement to avoid this overhead in release
     **/
	static int i=0;
	if(i==0)
	{
    	glValidateProgram(mProgramHandle);
		i++;
	}
	else
	{
//		std::cout << "aezfgbfvdnjk"<<std::endl;
	}
    GLenum errCode;

    if ((errCode = glGetError()) != GL_NO_ERROR) {
        std::cerr << "Shader::enable: Error: " << std::endl;
        const GLubyte *errString;
        errString = gluErrorString(errCode);
        std::cerr<< "OpenGL Error (" << errCode << "): "<< errString << std::endl;
        if(errCode = GL_INVALID_OPERATION)  {
            std::cerr << "GL_INVALID_OPERATION is generated if a geometry shader is active and mode is incompatible with the input primitive type of the geometry shader in the currently installed program object."<< std::endl
                << "GL_INVALID_OPERATION is generated if a non-zero buffer object name is bound to an enabled array or the element array and the buffer object's data store is currently mapped." << std::endl;
            exit(1);
        }
    }

    // Enable the program
    glUseProgram(mProgramHandle);

    // Bind the textures
    bindTextures();
	if(vao)
	{
		glBindVertexArray(vao);
	}
}

/**
 * @brief Restores the render pipeline
 */
void Shader::disable() {
//	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glUseProgram(0);
}

/**
 * @brief Checks for vertex and fragment shader support
 *
 * @return
 */
bool Shader::hasShaderSupport() {
	return GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader;
}

/**
 * @brief Sets an uniform GLSL float value
 * WARNING: This has to be called after the shader has been enabled (by calling the enable() function)!
 *
 * @param uniformVarName
 *      The uniform variable name in the GLSL shader
 * @param value
 *      The value to set
 */
void Shader::setFloat(const std::string &uniformVarName, float value) {
//	std::cout << getVariableId(uniformVarName)<<std::endl;
	glUniform1f(getVariableId(uniformVarName), value);
}

void Shader::setInt(const std::string &uniformVarName, int value) {
	glUniform1i(getVariableId(uniformVarName), value);
}

void Shader::setVec2(const std::string &uniformVarName,
		const glm::vec2& vector) {
	glUniform2f(getVariableId(uniformVarName), vector.x, vector.y);
}
void Shader::setVec3(const std::string &uniformVarName,
		const glm::vec3& vector) {
	glUniform3f(getVariableId(uniformVarName), vector.x, vector.y, vector.z);
}

void Shader::setVec4(const std::string &uniformVarName, const glm::vec4& vector) {
	glUniform4f(getVariableId(uniformVarName), vector.x, vector.y, vector.z, vector.t);
}

void Shader::setMat4(const std::string &uniformVarName, const glm::mat4& mat) {
	glUniformMatrix4fv(getVariableId(uniformVarName), 1, GL_FALSE,
			glm::value_ptr(mat));
}

void Shader::setFloatArray(const std::string &uniformVarName,
		const std::vector<float>& f) {
	float *farray;
	farray = new float[f.size()];
	int i = 0;
	for (int i = 0; i < f.size(); i++) {
		farray[i] = f[i];
	}
	glUniform1fv(getVariableId(uniformVarName), f.size(), farray);
	std::cerr << "fuite de memoire??"<<std::endl;
}

void Shader::setVec3Array(const std::string &uniformVarName,
		const std::vector<sf::Vector3f>& f) {
	GLfloat *farray;
	farray = new float[f.size() * 3];
	int glIndex = 0;
	for (int i = 0; i < f.size(); i++) {
		farray[glIndex++] = f[i].x;
		farray[glIndex++] = f[i].y;
		farray[glIndex++] = f[i].z;
	}
	glUniform3fv(getVariableId(uniformVarName), 1, farray);
	std::cerr << "fuite de memoire??"<<std::endl;
}

/**
 * @brief Sets a texture from an OpenGL texture ID
 *
 * @param uniformLocation
 * @param texture
 */
void Shader::setTexture(const std::string &uniformLocation, GLuint texture) {
	int uniformID = getVariableId(uniformLocation);
	//cout << "setTexture " << texture << " for uniform " << uniformLocation << "("<<uniformID<<")" << endl;
	m_textures[uniformID] = shared_ptr<Texture>(new Texture(texture));
}

/**
 * @brief Sets a texture from a Texture object
 *
 * @param uniformLocation
 * @param texture
 *     A valid Texture object
 */
void Shader::setTexture(const std::string &uniformLocation, const shared_ptr<Texture>& texture) {
	int uniformID = getVariableId(uniformLocation);
	//cout << "setTexture " << texture << " for uniform " << uniformLocation << "("<<uniformID<<")" << endl;
//	m_textures[texture] = uniformID;
	m_textures[uniformID] = texture;
}

void Shader::clear_texture()
{
	m_textures.clear();
}
/**
 * Bind textures to GL_TEXTURE0 + some number
 * For all the textures assigned to the shader, bind them.
 */
void Shader::bindTextures() {
	int texture_slot_index = 0;
	for (auto it = m_textures.begin(); it != m_textures.end(); it++) {
	//	dinf << "Bind texture " << it->first<< " on texture unit unit "
	//			<< texture_slot_index << std::endl;
		glActiveTexture(GL_TEXTURE0 + texture_slot_index);
		glBindTexture(it->second->getTextureType(), it->second->getTextureId());
		// Binds to the shader
		//dinf << "Bind to shader uniform: " << it->second << endl;
		glUniform1i(it->first, texture_slot_index);
		texture_slot_index++;
	}
	// Make sure that the texture unit which is left active is the number 0
	glActiveTexture(GL_TEXTURE0);
}

GLuint Shader::getProgramHandle() const {
	return mProgramHandle;
}

/**
 * @brief Returns the uniform ID of a shader's variable
 *
 * @param strVariable
 *      Name of the variable
 * @return
 *      Uniform ID of variable
 */
int Shader::getVariableId(const std::string& strVariable) const {
	// If we don't have an active program object, let's return -1
	if (!mProgramHandle)
		return -1;

	int uniformId = glGetUniformLocation(mProgramHandle, strVariable.c_str());
    if (uniformId == -1)
        cerr << "Uniform \"" << strVariable << "\" not found in shader " << mShaderName << std::endl;

	//cout << "getVariableId("<<strVariable<<") = " << uniformId << endl;
	// This returns the variable ID for a variable that is used to find
	// the address of that variable in memory.
	return uniformId;
}


void Shader::set_in_f(int n, const std::string &in_name,
		const std::vector<float>& pos) {


	if(!vao)
	{
		glGenVertexArrays(1,&vao);
	}
	glBindVertexArray(vao);
//	glBindAttribLocation(mProgramHandle, 0, in_name.c_str());
//	vertexLoc = glGetAttribLocation(mProgramHandle,in_name.c_str()); replace the previews one
//	glLinkProgram(mProgramHandle);


	GLint vertexLoc;
//	glLinkProgram(mProgramHandle);
	
	
	GLuint buffer;
	if(buffer_vbo.find(in_name)==buffer_vbo.end())
	{
		glGenBuffers(1, &buffer);
		buffer_vbo[in_name] = buffer;
	}
	else
	{
		buffer = buffer_vbo[in_name];
	}
	vertexLoc = glGetAttribLocation(mProgramHandle,in_name.c_str());
	std::cout << "loc: "<<vertexLoc << std::endl;
	
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pos.size(), pos.data(), GL_STATIC_DRAW);

//void glBufferSubDataARB(GLenum target, GLint offset, GLsizei size, void* data)
	/*

GL_STATIC_DRAW_ARB
GL_STATIC_READ_ARB
GL_STATIC_COPY_ARB
GL_DYNAMIC_DRAW_ARB
GL_DYNAMIC_READ_ARB
GL_DYNAMIC_COPY_ARB
GL_STREAM_DRAW_ARB
GL_STREAM_READ_ARB
GL_STREAM_COPY_ARB

	   */
	//to update: methode 1:glBufferDATA/glBufferSubData
	//methode 2:
	/*
// bind then map the VBO
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	float* ptr = (float*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);

// if the pointer is valid(mapped), update VBO
	if(ptr)
	{
    	updateMyVBO(ptr, ...);//fonction a moi->je fais ce que je veux sur ptr                 // modify buffer data
	    glUnmapBufferARB(GL_ARRAY_BUFFER_ARB); // unmap it after use
	}
avec comme option du map:

GL_READ_ONLY_ARB
GL_WRITE_ONLY_ARB
GL_READ_WRITE_ARB

	  */

    GLenum errCode;
    if ((errCode = glGetError()) != GL_NO_ERROR) {
		std::cout << "error1                    aaa"<<std::endl;
        const GLubyte *errString;
        errString = gluErrorString(errCode);
        std::cerr<< "OpenGL Error (" << errCode << "): "<< errString << std::endl;
		exit(0);
	}


	glEnableVertexAttribArray(vertexLoc );
	glVertexAttribPointer(vertexLoc , n, GL_FLOAT, 0, 0, 0);
    if ((errCode = glGetError()) != GL_NO_ERROR) {
		std::cout << "error2                    aaa"<<std::endl;
        const GLubyte *errString;
        errString = gluErrorString(errCode);
        std::cerr<< "OpenGL Error (" << errCode << "): "<< errString << std::endl;
		exit(0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

}
