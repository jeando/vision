#include <iostream>
#include "mosaic.h"
#include <GL/glew.h>
#include <GL/gl.h>

#include<map>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<memory>
#include"texture.hpp"
#include"shader.hpp"
#include<vector>
#define BUFFER_OFFSET(i)   ((char *)NULL + (i))
using namespace cg;
typedef double TYPE;
//typedef long double TYPE;

int main(int argc, char** argv)
{
//	sf::Window window(sf::VideoMode(800, 600), "My window");
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	//window.PreserveOpenGLStates(true);
	glewInit();
    
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );
	glBlendFunc (GL_ONE, GL_ONE);
	
	
	mosaic<TYPE> m;
	//m.lecture_appariement("../Mosaics/Eglise/i.matchs");
	m.lecture_appariement("matching_points");
	//m.compute_mosaic(Draw);
	m.init_compute_mosaic();
	std::map<std::string,std::shared_ptr<Texture>> tex;
	const int nb_image = m.size()+1;
	std::vector<GLfloat> vertex_data__(20*nb_image);
	GLfloat* vertex_data = vertex_data__.data();
	GLfloat* ptr_quad = vertex_data + 12*nb_image;
	GLfloat* ptr_tex = vertex_data + 12*nb_image;
	for(int i=0; i<nb_image; i++)
	{
		*ptr_quad++ = 0;
		*ptr_quad++ = 0;
	
		*ptr_quad++ = 0;
		*ptr_quad++ = 1;
		
		*ptr_quad++ = 1;
		*ptr_quad++ = 1;
	
		*ptr_quad++ = 1;
		*ptr_quad++ = 0;
	}
		
	Shader shader;
	shader.loadVertexShaderFromFile("shader.vert");
	shader.loadFragmentShaderFromFile("shader.frag");
	
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
						//Texture(std::string("../Mosaics/Eglise/")+img.name));
						Texture(img.name));
			}
			//*
			for(vec2<TYPE> v : img.vertex)
			{
				*ptr_quad++ = v.x/1000;
				*ptr_quad++ = v.y/1000;
				*ptr_quad++ = 0;
			}//*/
		/*
				*ptr_quad++ = 0;
				*ptr_quad++ = 0;
				*ptr_quad++ = 0;
				*ptr_quad++ = 1;
				*ptr_quad++ = 0;
				*ptr_quad++ = 0;
				*ptr_quad++ = 1;
				*ptr_quad++ = 1;
				*ptr_quad++ = 0;
				*ptr_quad++ = 0;
				*ptr_quad++ = 1;
				*ptr_quad++ = 0;
				//*/
		}
		

    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

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
		ptr_tex = vertex_data + 12*nb_image;
		shader.clear_texture();
		shader.enable();
#define SIZE_GRILLE 50.0f
		for(int i=0; i<img_v.size(); i++)
		{
		//	if(i!=img_v.size()-1)
		//		continue;
			shader.clear_texture();
			shader.setTexture("Texture", tex[img_v[i].name]);
			shader.enable();
			glBegin(GL_QUADS);
//			for(int j=0; j<4; j++)
//			{//765x509
			for(int row=0; row<SIZE_GRILLE; row++)
			{
				for(int col=0; col<SIZE_GRILLE; col++)
				{
				//glTexCoord2fv(ptr_tex);
				//glVertex3fv(ptr_quad);
					__mat<TYPE> mm(img_v[i].h);

//					std::cout << img_v[i].cols << std::endl;
//					std::cout << img_v[i].rows << std::endl;
				glTexCoord2f(col/SIZE_GRILLE, row/SIZE_GRILLE);
				vec2<TYPE> v = mm * vec2<TYPE>(img_v[i].cols*(col)/SIZE_GRILLE,img_v[i].rows*(row)/SIZE_GRILLE);

			//	std::cout << v.x<< " " << v.y << std::endl;
				glVertex3f(v.x/1000,v.y/1000,0);
				
				glTexCoord2f(col/SIZE_GRILLE, (row+1)/SIZE_GRILLE);
					v = mm * vec2<TYPE>(img_v[i].cols*(col)/SIZE_GRILLE,img_v[i].rows*(row+1)/SIZE_GRILLE);
				glVertex3f(v.x/1000,v.y/1000,0);
				
				glTexCoord2f((col+1)/SIZE_GRILLE, (row+1)/SIZE_GRILLE);
					v = mm * vec2<TYPE>(img_v[i].cols*(col+1)/SIZE_GRILLE,img_v[i].rows*(row+1)/SIZE_GRILLE);
				glVertex3f(v.x/1000,v.y/1000,0);
				
				glTexCoord2f((col+1)/SIZE_GRILLE, row/SIZE_GRILLE);
					v = mm * vec2<TYPE>(img_v[i].cols*(col+1)/SIZE_GRILLE,img_v[i].rows*(row)/SIZE_GRILLE);
				glVertex3f(v.x/1000,v.y/1000,0);
//				std::cout << ptr_tex[0] << " " << ptr_tex[1] << std::endl;
//				std::cout << ptr_quad[0] << " " << ptr_quad[1] << " " << ptr_quad[2] <<std::endl;
//				ptr_quad += 3;
//				ptr_tex += 2;
				}
			}

//			}
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

		sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
//            if (event.type == sf::Event::Closed)
  //              window.close();
        if (event.type == sf::Event::KeyPressed)
		{
		if (event.key.code == sf::Keyboard::F1)
		{
			    sf::Image Screen = window.capture();
			    Screen.saveToFile("screenshot.jpg");
		}
		else if(event.key.code == sf::Keyboard::F2)
		{
			if(m.size()>=0)
				m.compute_next_mosaic();
			std::cout << m.size()<<std::endl;
		}
        }
		}
	}
}


