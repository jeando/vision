/******************************************************************************
*     Copyright (C) 2013 TANGUY Arnaud arn.tanguy@gmail.com                   *
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

//-----------------------------------------------------------------
// Print OpenGL errors
//
// Returns 1 if an OpenGL error occurred, 0 otherwise.
//

#include <GL/gl.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define oerr() printOglError(__FILE__, __LINE__)

int printOglError(const char *file, int line) {

	GLenum glErr;
	int retCode = 0;

	glErr = glGetError();
	if (glErr != GL_NO_ERROR) {
		printf("glError in file %s @ line %d: %s\n", file, line,
				gluErrorString(glErr));
		retCode = 1;
	}
	return retCode;
}
