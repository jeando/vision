#include <iostream>
#include "mosaic.h"
#include <GL/glew.h>
#include <GL/gl.h>

#include<map>
#include<SFML/Window.hpp>
#include<memory>
#include"texture.hpp"
#include"shader.hpp"
#include<vector>
#define BUFFER_OFFSET(i)   ((char *)NULL + (i))
using namespace cg;
typedef double TYPE;

int main(int argc, char** argv)
{
	sf::Window window(sf::VideoMode(800, 600), "My window");
	glewInit();
    
	glEnable(GL_DEPTH_TEST);
	
	
	mosaic<TYPE> m;
	m.lecture_appariement("../Mosaics/Eglise/i.matchs");
	//m.compute_mosaic(Draw);
	m.init_compute_mosaic();
	std::map<std::string,std::shared_ptr<Texture>> tex;
	const int nb_image = m.size()+1;
	std::vector<GLfloat> vertex_data__(20*nb_image);
	GLfloat* vertex_data = vertex_data__.data();
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	GLfloat* ptr_quad = vertex_data + 12*nb_image;
	for(int i=0; i<m.size(); i++)
	{
		*ptr_quad++ = 0;
		*ptr_quad++ = 0;
		*ptr_quad++ = 1;
		*ptr_quad++ = 0;
		*ptr_quad++ = 1;
		*ptr_quad++ = 1;
		*ptr_quad++ = 0;
		*ptr_quad++ = 1;
	}
	glBufferData(GL_ARRAY_BUFFER, (4*(3+2))*nb_image*sizeof(GLfloat), vertex_data, GL_DYNAMIC_DRAW);


	while(m.size())
	{
		const std::vector<image<TYPE>>& img_v = m.get_result().get_img();
		ptr_quad = vertex_data;
		for(const image<TYPE>& img : img_v)
		{
			if(tex.find(img.name)==tex.end())
			{
				std::cout << "try to load img "<< img.name<<std::endl;
				tex[img.name] = std::shared_ptr<Texture>(new
						Texture(std::string("../Mosaics/Eglise/")+img.name));
			}
			for(vec2<TYPE> v : img.vertex)
			{
				*ptr_quad++ = v.x;
				*ptr_quad++ = v.y;
				*ptr_quad++ = 0;
			}
		}
		

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, 20*nb_image*sizeof(GLfloat), vertex_data, GL_DYNAMIC_DRAW);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glTexCoordPointer(2,GL_FLOAT,0,BUFFER_OFFSET(12*nb_image*sizeof(GLfloat)));

		//***************
		for(int i=0; i<img_v.size(); i++)
		{
			Shader shader;
			shader.loadVertexShaderFromFile("shader.vert");
			shader.loadFragmentShaderFromFile("shader.frag");
			shader.enable();
			shader.setTexture("Texture", tex[img_v[0].name]);


			glDrawArrays(GL_QUADS, i*12, (i+1)*12);
		}
		//*************
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		window.display();		
		
		m.compute_next_mosaic();
		std::cout << m.size()<<std::endl;
	}
}


