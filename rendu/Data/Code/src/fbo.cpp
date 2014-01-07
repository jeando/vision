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

// Include GLEW
#include <GL/glew.h>
#include <GL/glext.h>
#include <iostream>
#include "PrintOpenglErrors.h"
#include "fbo.hpp"
#include "fboexception.hpp"

using namespace std;


namespace cg
{

    /**
     * @brief Creates a FBO (Frame Buffer Object)
     * WARNING: You need to call the proper init functions for the functionnalities that you need
     * initColorAttachement for render to texture
     * initDepthTexture for depth map
     *
     * @param width
     *      width of render texture
     * @param height
     *      height of render texture
     */
    FBO::FBO(int width, int height, const Type type) {
        mWidth = width;
        mHeight = height;

        mType = type;

        init();

    }

    FBO::~FBO() {
        glDeleteFramebuffersEXT(1, &mFrameBuffer);
        //XXX: todo clear textures
    }

    void FBO::init() {
        if(mType & COLOR) {
            std::cout << "Init Colour attachement" << std::endl;
            initColorAttachement();
        }
        if(mType & DEPTH) {
            initDepthTexture();
        }
        initFBO();
    }


    /**
     * @brief Inits a texture used for depth rendering
     */
    void FBO::initDepthTexture() {
        //32 bit depth texture, mWidth*mHeight
        glGenTextures(1, &mDepthTex);
        glBindTexture(GL_TEXTURE_2D, mDepthTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
                GL_COMPARE_R_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

        //NULL means reserve texture memory, but texels are undefined
        //You can also try GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24 for the internal format.
        //If GL_DEPTH24_STENCIL8_EXT, go ahead and use it (GL_EXT_packed_depth_stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, mWidth, mHeight, 0,
                GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    }

    /**
     * @brief Inits a texture used for color rendering
     */
    void FBO::initColorAttachement() {

        glGenTextures(1, &mColorTexture);
        glBindTexture(GL_TEXTURE_2D, mColorTexture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); // place multisampling here too!
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    /**
     * @brief inits the FBO with color and/or depth attachement
     */
    void FBO::initFBO() {
        glGenFramebuffersEXT(1, &mFrameBuffer);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBuffer);
        //Attach

        if(mType & COLOR) {
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                    GL_TEXTURE_2D, mColorTexture, 0);
            GLenum buf[1] = { GL_COLOR_ATTACHMENT0_EXT};
            glDrawBuffers(1, buf);
            printOpenGLError();
            glReadBuffer(GL_NONE);
        }
        if(mType & DEPTH) {
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                    GL_TEXTURE_2D, mDepthTex, 0);
        }



        //GLint maxbuffers;
        //glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxbuffers);
        //cout << "Max number of attachements: " << maxbuffers << endl;

        //Before checking the configuration, you should call these 2 according to the spec.
        //At the very least, you need to call glDrawBuffer(GL_NONE)
        //	glDrawBuffer(GL_NONE);
        //	glReadBuffer(GL_NONE);
        checkFBO();

        // Don't activate FBO by default
        renderToScreen();
    }

    /**
     * @brief: Describes FBO errors
     **/
    void FBO::checkFBO() throw () {
        GLenum status;
        status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
        switch (status) {
            case GL_FRAMEBUFFER_COMPLETE_EXT:
                cout << "Good Framebuffer" << endl;
                break;
            case GL_FRAMEBUFFER_UNDEFINED:
                throw new FBOException(
                        "Framebuffer undefined. Usually returned if  returned if target is the default framebuffer, but the default framebuffer does not exist.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                throw new FBOException(
                        "Incomplete Attachement: is returned if any of the framebuffer attachment points are framebuffer incomplete.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                throw new FBOException(
                        "Incomplete Missing Attachment: is returned if the framebuffer does not have at least one image attached to it.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                throw new FBOException(
                        "Incomplete Draw Buffer: is returned if the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAWBUFFERi");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                throw new FBOException(
                        "Incomplete Read Buffer: is returned if GL_READ_BUFFER is not GL_NONE and the value of\\"
                        " GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER");
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                throw new FBOException(
                        "Framebuffer Unsupported: is returned if the combination of internal formats of the attached images violates an implementation-dependent set of restrictions.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                throw new FBOException("Incomplete Multisample");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                throw new FBOException("Incomplete Layer Targets");
                break;
            default:
                throw new FBOException("Bad Framebuffer");
        }
    }

    /****
     * PUBLIC Functions
     */

    /**
     * @brief Activate the FBO.
     * After this call, all render operations will be done on texture/depth
     */
    void FBO::renderToFBO() {
        //cout << "Render to FBO: " << mFrameBuffer << endl;
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our texture
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBuffer); // Bind our frame buffer for rendering

        //-------------------------
        //----and to render to it, don't forget to call
        //At the very least, you need to call glDrawBuffer(GL_NONE)
        //	glDrawBuffer(GL_NONE);
        //	glReadBuffer(GL_NONE);
        //-------------------------
        //Does the GPU support current FBO configuration?
        //Before checking the configuration, you should call these 2 according to the spec.
        //At the very least, you need to call glDrawBuffer(GL_NONE)
        //		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
    }

    /**
     * @brief Clear the screen in the FBO
     */
    void FBO::clear() {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our texture
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrameBuffer); // Bind our frame buffer for rendering
        glClearColor(0., 0., 0.2, 1.);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    /**
     * @brief: Static function disabling FBOs and reactivating render to screen
     */
    void FBO::renderToScreen() {
        //cout << "Render to screen " << endl;
        // Finish all operations
        //glFlush();
        //-------------------------
        //If you want to render to the back buffer again, you must bind 0 AND THEN CALL glDrawBuffer(GL_BACK)
        //else GL_INVALID_OPERATION will be raised
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our texture
        glDrawBuffer(GL_BACK);
        glReadBuffer(GL_BACK);
    }

}
