/*
 * FBO.h
 *
 *  Created on: 28 Mar 2013
 *      Author: arnaud
 */

#ifndef FBO_H_
#define FBO_H_

#include <GL/gl.h>

/**
 * See http://www.songho.ca/opengl/gl_fbo.html for explanations of FBOs
 */
namespace cg
{

    class FBO {
        protected:
            GLuint mFrameBuffer;
            GLuint mDepthTex;
            GLuint mTexture;
            GLuint mColorTexture;

            int mWidth;
            int mHeight;

            void checkFBO() throw ();
            void initFBO();
            void initDepthTexture();
            void initColorAttachement();

        public:
            enum Type {
                DEPTH = (1u << 0),
                COLOR = (1u << 1)
            };

            Type mType;

            FBO(int width, int height, const Type type);
            virtual ~FBO();
            void init();

            void renderToFBO();
            static void renderToScreen();
            void clear();
            GLuint getDepthTextureId() const {
                return mDepthTex;
            }
            GLuint getColorTextureId() const {
                return mColorTexture;
            }
    };

}

#endif /* FBO_H_ */
