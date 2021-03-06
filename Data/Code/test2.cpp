#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

#include<map>
#include<SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include<memory>
#include"texture.hpp"
#include"shader.hpp"
#include<vector>
#define BUFFER_OFFSET(i)   ((char *)NULL + (i))
using namespace cg;
typedef double TYPE;

int main(int argc, char** argv)
{
	sf::Context c;
	glewInit();
	sf::ContextSettings settings;
	settings.majorVersion = 3;
	//settings.minorVersion = 1;
	settings.minorVersion = 0;

	sf::Window window(sf::VideoMode(800, 600), "My window", sf::Style::Default, settings);
    
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
	glBlendFunc (GL_ONE, GL_ONE);


	std::map<std::string,std::shared_ptr<Texture>> tex;
	const int nb_image = m.size()+1;
	std::vector<GLfloat> vertex_data__(12*nb_image);
	std::vector<GLfloat> tex_data__(6*nb_image);
	GLfloat* vertex_data = vertex_data__.data();
	//GLuint buffer;
	GLfloat* ptr_quad = vertex_data;
	GLfloat* ptr_tex = tex_data__.data();
	for(int i=0; i<nb_image; i++)
	{
		*ptr_tex++=0;
		*ptr_tex++=0;
		
		*ptr_tex++=0;
		*ptr_tex++=1;
		
		*ptr_tex++=1;
		*ptr_tex++=1;
		
		*ptr_tex++=1;
		*ptr_tex++=0;
	}


	Shader shader;
	shader.loadVertexShaderFromFile("shader.vert");
	shader.loadFragmentShaderFromFile("shader.frag");

	shader.enable();
//	GLuint vao;
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
	//shader.set_in_f(2,"multiTexCoord0",tex_data__);
//	shader.set_in_f(2,"aa",tex_data__);
//	shader.enable();
//	shader.enable();
//	shader.enable();

	while(m.size()||true)
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
				*ptr_quad++ = v.x/1000;
				*ptr_quad++ = v.y/1000;
				*ptr_quad++ = 0;
			}
		}
		
		std::cout << "aaasssscccc"<<std::endl;
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
//		glBindBuffer(GL_ARRAY_BUFFER, buffer);
	/*	
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glTexCoordPointer(2,GL_FLOAT,0,BUFFER_OFFSET(12*nb_image*sizeof(GLfloat)));

		//***************
		for(int i=0; i<img_v.size(); i++)
		{
			shader.enable();
			shader.setTexture("Texture", tex[img_v[i].name]);


			glDrawArrays(GL_QUADS, i*12, 12);
//			glDrawArrays(GL_QUADS, i*12*sizeof(float), 12);
//			glDrawArrays(GL_QUADS, 0, (i+1)*12);
		}
		//*************
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//*/
		ptr_quad = vertex_data;
		ptr_tex = tex_data__.data();
//		shader.enable();
//		std::cout << "aaaaaaaazertyu"<<std::endl<<std::endl;
//		shader.clear_texture();
//		std::cout << "aaaaaaaazertyu"<<std::endl<<std::endl;
//		shader.enable();
		std::cout << "azertyu"<<std::endl<<std::endl;
		//	shader.clear_texture();
//			shader.setTexture("Texture", tex[img_v[i].name]);
		//	glBindVertexArray(vao);
//			shader.set_in_f(3,"vertex",vertex_data__);
//			shader.set_in_f(2,"multiTexCoord0",tex_data__);
		//	glBindVertexArray(vao[0]);
//		for(int i=0;i<img_v.size(); i++)
//		{
//			shader.setTexture("Texture", tex[img_v[i].name]);	
//			shader.enable();
			//glDrawArrays(GL_QUADS, 4*i, 4);
	//shader.set_in_f(2,"aa",tex_data__);
//	glBindVertexArray(vao);
//			glDrawArrays(GL_QUADS, 0, 4);
//		}
		ptr_quad = vertex_data;
		ptr_tex = tex_data__.data();
//		ptr_quad = vertex_data + 12*(img_v.size()-1);
//		for(int i=img_v.size()-1; i>=0;i--)
		for(int i=0; i<img_v.size(); i++)
		{
			shader.enable();
			shader.clear_texture();
			shader.setTexture("Texture", tex[img_v[i].name]);
			shader.enable();
		//	glBindVertexArray(vao[0]);
	//		shader.set_in_f(3,"vertex",vertex_data__);
//			shader.set_in_f(2,"multiTexCoord0",tex_data__);
//			glBindVertexArray(vao[0]);
//			glDrawArrays(GL_QUADS, 0, img_v.size());
			glBegin(GL_QUADS);
			for(int j=0; j<4; j++)
			{
				glTexCoord2fv(ptr_tex);
				glVertex3fv(ptr_quad);
				std::cout << ptr_tex[0] << " " << ptr_tex[1] << std::endl;
				std::cout << ptr_quad[0] << " " << ptr_quad[1] << " " << ptr_quad[2] <<std::endl;
				ptr_quad += 3;
				ptr_tex += 2;
			}
//			ptr_quad-=24;
/*			glTexCoord2f(0,0);
			glVertex3f(0,0,0);
			glTexCoord2f(0,1);
			glVertex3f(0,1,0);
			glTexCoord2f(1,1);
			glVertex3f(1,1,0);
			glTexCoord2f(1,0);
			glVertex3f(1,0,0);//*/
			glEnd();
		}
		window.display();		

	}
}


