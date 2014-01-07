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
#include<sstream>
#include<set>
#include<type_traits>
#include<list>
#include<deque>
#include"correspondance.h"
#define BUFFER_OFFSET(i)   ((char *)NULL + (i))
using namespace cg;
typedef double TYPE;
typedef long long int TYPE_ID;
/*
 
id
id -> string_name -> id
string_name -> texture
id -> texture
id -> correspondance



   */
//int main(int argc, char** argv)
int main()
{
	sf::Context c;
//	glewInit();
	if(glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW init failed!" << std::endl;
		return EXIT_FAILURE;
	}

	sf::ContextSettings settings;
	settings.majorVersion = 3;
	//settings.minorVersion = 1;
	settings.minorVersion = 0;

	GLfloat window_w = 800.0f;
	GLfloat window_h = 600.0f;
//	sf::Window window(sf::VideoMode(800, 600), "My window", sf::Style::Default, settings);
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window", sf::Style::Default, settings);
	sf::Font font;
	if (!font.loadFromFile("fonts/OpenSans-Light.ttf"))
	{
		std::cerr << "Couldn't find fonts/OpenSans-Light.ttf" << std::endl;
		return EXIT_FAILURE;
	}
	sf::Text text;
	text.setFont(font);
			//text.setColor(sf::Color::Red);	
			text.setColor(sf::Color::White);	
	GLfloat x_text;
	GLfloat y_text;
	const GLfloat x_text_ini(-0.95f);
	const GLfloat y_text_ini(-0.5f);

	GLfloat char_size;
	GLfloat delta_y_text;

	const GLfloat image_size_w = 765.0f;
	const GLfloat image_size_h = 509.0f;

	GLfloat windows_ratio = window_w/window_h;
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
	glBlendFunc (GL_ONE, GL_ONE);


	std::shared_ptr<Texture> gauche_tex;
	std::shared_ptr<Texture> droite_tex;
	TYPE_ID gauche_id;
	TYPE_ID droite_id;

	//std::map<std::string,std::shared_ptr<Texture>> tex;
	//map pour pas charger toutes textures dans ram : on charge entre n-ket n+k
	//std::map<TYPE_ID,std::shared_ptr<Texture>> tex;
	//std::vector<std::shared_ptr<Texture>> tex(5);
	std::deque<std::shared_ptr<Texture>> tex;
	std::vector<std::string> img_name;
	{
		std::ifstream ifs;
		ifs.open("config");
		size_t n;
		ifs>>n;
		img_name.resize(n);
		ifs.ignore();//'\n'
		for(std::string& s : img_name)
		{
			std::getline(ifs,s);
		}
	}
/*	img_name[0] = "aa";
	img_name[1] = "zz";
	img_name[2] = "ee";
	img_name[3] = "rr";
	img_name[4] = "tt";
	//*/
	std::map<std::string, TYPE_ID> img_id;
	for(size_t i=0; i<img_name.size(); i++)
	{
		img_id[img_name[i]] = i;
	}
//ID = first_list_loaded + n whith n corresponding to the texture tex[n]
	tex.resize(img_name.size());
	size_t first_list_loaded(0);
//	decltype(tex)::iterator it_first_list_loaded(tex.begin());
	std::transform(img_name.begin(), img_name.end(), tex.begin(),
			[](decltype(*img_name.begin())& val)
			{return std::shared_ptr<Texture>(new Texture(val));});
	/*
	tex[0] = std::shared_ptr<Texture>(new Texture("../Mosaics/Eglise/i.25.png"));
	tex[1] = std::shared_ptr<Texture>(new Texture("../Mosaics/Eglise/i.26.png"));
	tex[2] = std::shared_ptr<Texture>(new Texture("../Mosaics/Eglise/i.27.png"));
	tex[3] = std::shared_ptr<Texture>(new Texture("../Mosaics/Eglise/i.28.png"));
	tex[4] = std::shared_ptr<Texture>(new Texture("../Mosaics/Eglise/i.29.png"));
	//*/
	gauche_id = 1 + first_list_loaded;
	droite_id = 3 + first_list_loaded;
	gauche_tex = tex[gauche_id-first_list_loaded];
	droite_tex = tex[droite_id-first_list_loaded];

	//std::vector<std::vector<correspondance<TYPE>>> corr(img_name.size());
	std::map<TYPE_ID,std::map<TYPE_ID,std::set<correspondance<TYPE>>>> corr;
	//decltype(corr[gauche_id][droite_id])::type::const_iterator corr_begin = corr[gauche_id][droite_id].begin();
	std::remove_reference<decltype((corr[gauche_id][droite_id]))>::type::const_iterator corr_begin = corr[gauche_id][droite_id].begin();
	decltype(corr_begin) corr_begin_begin = corr_begin;
	decltype(corr_begin) corr_end = corr[gauche_id][droite_id].end();
	int delta_text = 0;
	std::list<sf::FloatRect> text_box;
	//std::map<TYPE_ID,std::map<TYPE_ID,std::map<correspondance<TYPE>,sf::Text>>> corr;
	correspondance<TYPE> tmp_corr;
	bool gauche_corr_set = false;
	bool droite_corr_set = false;
	std::vector<GLfloat> vertex_data__(12*(2));
	std::vector<GLfloat> vertex_sample_data(12);
	std::vector<GLfloat> tex_data__(8*(2));
	std::vector<GLfloat> tex_sample_data(8);
	GLfloat* const vertex_data = vertex_data__.data();
	//GLuint buffer;
	GLfloat* ptr_vertex = vertex_data;

		const GLfloat ratio = image_size_w/image_size_h;
	GLfloat sample_translation_x(0.0f);
	GLfloat sample_size_x(0.0f);
	GLfloat sample_size_y(0.0f);
	GLfloat texture_haut_sx(ratio/windows_ratio);
	GLfloat texture_haut_sy(1.0f);
		//GLfloat size_x = std::min(ratio,1.0f);
		//GLfloat size_y = size_x/ratio;
		GLfloat size_x = 1.0f;
		GLfloat size_y = 1.0f;//size_x/ratio;

auto set_default_tex = [](std::vector<GLfloat>& vf){
	const int n = vf.size()/8;
	GLfloat* ptr_tex = vf.data();
	for(int i=0; i<n; i++)
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
};
set_default_tex(tex_data__);
set_default_tex(tex_sample_data);
		

auto set_quad_vertex = [&ptr_vertex](GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{

	*ptr_vertex++=x1;
		*ptr_vertex++=y1;
		*ptr_vertex++=0;

		*ptr_vertex++=x1;
		*ptr_vertex++=y2;
		*ptr_vertex++=0;

		*ptr_vertex++=x2;
		*ptr_vertex++=y2;
		*ptr_vertex++=0;

		*ptr_vertex++=x2;
		*ptr_vertex++=y1;
		*ptr_vertex++=0;
};
set_quad_vertex(-size_x,size_y,0,0);		
set_quad_vertex(0,size_y,size_x,0);		
	

	//size_vignette
		//translation_x vignette; -> param du shader

		size_y = 0.3f;
		size_x = size_y*ratio;
		sample_size_x = 0.1f+size_x;
		sample_size_y = size_y;
		ptr_vertex = vertex_sample_data.data();
		set_quad_vertex(-1.0f,-0.1f,size_x-1.0f,-0.1f-size_y);		
	
	Shader shader;
	shader.loadVertexShaderFromFile("shader/haut.vert");
	shader.loadFragmentShaderFromFile("shader/haut.frag");
	Shader shader_sample;
	shader_sample.loadVertexShaderFromFile("shader/sample.vert");
	shader_sample.loadFragmentShaderFromFile("shader/sample.frag");

	shader.set_in_f(2,"tex",tex_data__);
	shader.set_in_f(3,"vertex",vertex_data__);
	shader_sample.set_in_f(2,"tex",tex_sample_data);
	shader_sample.set_in_f(3,"vertex",vertex_sample_data);
	Shader::disable();
auto set_window_size = [&] (GLfloat w, GLfloat h)
{
	window_w = w;
	window_h = h;
	glViewport(0, 0, window_w, window_h);
	windows_ratio = window_w/window_h;
	windows_ratio = window_w/window_h;
	GLfloat tmp = ratio/windows_ratio;
	if(tmp<=1.0f)
	{
		texture_haut_sx = tmp;
		texture_haut_sy = 1;
	}
	else
	{
		texture_haut_sx = 1;
		texture_haut_sy = 1.0f/tmp;
	}
	shader_sample.enable();
	shader_sample.setFloat("wr",windows_ratio);
	shader.enable();
	shader.setFloat("sx",texture_haut_sx);
	shader.setFloat("sy",texture_haut_sy);
	Shader::disable();
	window.setView(sf::View(sf::FloatRect(0, 0, window_w, window_h)));
	char_size = std::min(window_h/20.0f,window_w/40.0f);
	delta_y_text = char_size*1.1f;;
	x_text = (1.0f+x_text_ini)/2.0f * window_w;
	y_text = (1.0f-y_text_ini)/2.0f * window_h;
	text.setCharacterSize(char_size);
};
set_window_size(window_w,window_h);

std::ostringstream oss;
	while(window.isOpen())
	{
	
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
			shader.enable();
			shader.setTexture("Texture",gauche_tex);
			shader.enable();
			glDrawArrays(GL_QUADS, 0, 4);
			shader.setTexture("Texture",droite_tex);
			shader.enable();
			glDrawArrays(GL_QUADS, 4, 4);
			shader_sample.enable();
			shader_sample.setFloat("tx",sample_translation_x);
			shader_sample.setFloat("wr",windows_ratio);
			for(size_t i=0; i<img_name.size(); i++)
			{
				shader_sample.setTexture("Texture",tex[img_id[img_name[i]]]);
				shader_sample.enable();
				shader_sample.setFloat("tx",sample_translation_x+i*sample_size_x);
				shader_sample.setFloat("selected",gauche_id-first_list_loaded==i||droite_id-first_list_loaded==i);
				glDrawArrays(GL_QUADS, 0, 4);
			}
			Shader::disable();
//			window.resetGLStates();
//	window.clear();
//		sf::Shader::bind(NULL);
			window.pushGLStates();
		//	window.resetGLStates();

//		sf::Shader::bind(NULL);sf::Texture::bind(NULL);
			text.setPosition(x_text,y_text);
			text_box.clear();
			auto draw_text = [&](std::string&& s)
			{
				text.setString(s);
				window.draw(text);
				text_box.push_back(text.getGlobalBounds());
				text.move(0,delta_y_text);
			};
			//corr_begin
			std::for_each(corr_begin, corr_end, [&](decltype(*corr_begin) a)
			{
						oss.str("");
						oss << a.p1 << " -> " << a.p2;
						draw_text(oss.str());
			});
//
//			FloatRect sf::Text::getGlobalBounds	(		)	 const
//			FloatRect sf::Text::getLocalBounds	(		)	 const
//file:///mnt/home/favreau/doc/sfml/www.sfml-dev.org/documentation/2.0/classsf_1_1Rect.php.html



/*
			for(const auto& a : corr[gauche_id][droite_id])
			{
				oss.str("");
				oss << a.p1 << " -> " << a.p2;
				draw_text(oss.str());
			}
			//*/
/*
			draw_text("abcdefghijklmnopqrstuvwxyz");
			draw_text("abcdefghijklmnopqrstuvwxyz");
			draw_text("abcdefghijklmnopqrstuvwxyz");
			draw_text("abcdefghijklmnopqrstuvwxyz");
			draw_text("abcdefghijklmnopqrstuvwxyz");
//*/			
			window.popGLStates();
		window.display();
		 sf::Event event;
		 while (window.pollEvent(event))
		 {
			 switch(event.type)
			 {
				 case sf::Event::Closed:
					 window.close();
					 break;
				 case sf::Event::KeyPressed:
					 switch(event.key.code)
					 {
						 case sf::Keyboard::Escape:
							 window.close();
							 break;
						 case sf::Keyboard::Left:
							 sample_translation_x += sample_size_x/10;
							 break;
						 case sf::Keyboard::Right:
							 sample_translation_x -= sample_size_x/10;
							 break;
						 case sf::Keyboard::Up:
							 if(corr_begin!=corr_begin_begin)
							 {
							 	delta_text--;
							 	corr_begin--;
							 }

							 break;
						case sf::Keyboard::Down:
							 if(corr_begin!=corr_end)
							 {
							 	delta_text++;
							 	corr_begin++;
							 }
							 break;
						 case sf::Keyboard::W:
							 [&](){
							 	std::ofstream ofs;
								ofs.open("matching_points");
								ofs << img_name.size() << std::endl;
								std::for_each(img_name.begin(), img_name.end(), [&ofs,&image_size_w,&image_size_h](decltype(*img_name.begin())& s){
										static int i=0; ofs<<i++ <<" "<<s<<" "<< image_size_w << "x" << image_size_h << std::endl;});
#define FOR_EACH(a,b,...) std::for_each(a.begin(), a.end(), [__VA_ARGS__](decltype(*a.begin())& b){
#define FOR_EACH_END });
		FOR_EACH(corr,a,&ofs)
			FOR_EACH(a.second,z,&ofs,&a)
				FOR_EACH(z.second,e,&ofs,&a,&z)
					ofs << a.first << " " << e.p1.x << " " << e.p1.y << " " << z.first
								<< " " << e.p2.x << " " << e.p2.y<<std::endl;
				FOR_EACH_END
			FOR_EACH_END
		FOR_EACH_END
								ofs.close();
							 }();
							 break;
						 default:
							 break;
					 }
					 break;
				 case sf::Event::MouseButtonPressed:
					{
						sf::Vector2i localPosition_tmp = sf::Mouse::getPosition(window);
						sf::Vector2f local_position(localPosition_tmp.x*2.0f/window_w-1.0f,1.0f-localPosition_tmp.y*2.0f/window_h);
						//localPosition_in\[-1,1\]^2
						std::cout << "(" << local_position.x << ", " << local_position.y << ")" << std::endl;
						int n = floor((local_position.x*windows_ratio+1-sample_translation_x)/sample_size_x);
						if(n>=0&&n<img_name.size()&& local_position.y<=-0.1f && local_position.y>=-0.1f-sample_size_y)
						{
						std::cout << n << std::endl;
							gauche_id = droite_id;
							gauche_tex = droite_tex;
							droite_id = n + first_list_loaded;
							droite_tex = tex[n];
							gauche_corr_set = false;
							droite_corr_set = false;
							corr_begin = corr[gauche_id][droite_id].begin();
							corr_begin_begin = corr_begin;
							delta_text = 0;
							corr_end = corr[gauche_id][droite_id].end();
						}
						else if(local_position.y>0.0f&&local_position.y<texture_haut_sy)
						{//y<f(ratio) et x<f(ratio)//transfo inverse shader.frag
							std::cout << texture_haut_sy<<std::endl;
							if(local_position.x<0&&local_position.x>-texture_haut_sx)
							{
								tmp_corr.p1.x = (1+local_position.x/texture_haut_sx)*image_size_w;
								tmp_corr.p1.y =  (1.0f-local_position.y/texture_haut_sy)*image_size_h;
								std::cout << "p1: " << tmp_corr.p1 << std::endl;
								gauche_corr_set = true;
								std::cout << "gauche" << std::endl;
							}
							else if(local_position.x>0&&local_position.x<texture_haut_sx)
							{
								tmp_corr.p2.x =  local_position.x/texture_haut_sx*image_size_w;
								tmp_corr.p2.y =  (1.0f-local_position.y/texture_haut_sy)*image_size_h;
								std::cout << "p2: " << tmp_corr.p2 << std::endl;
								droite_corr_set = true;
								std::cout << "droite" << std::endl;
							}
							if(gauche_corr_set&&droite_corr_set)
							{	gauche_corr_set = false;
								droite_corr_set = false;
								corr[gauche_id][droite_id].insert(tmp_corr);
								corr[droite_id][gauche_id].insert(decltype(tmp_corr)(tmp_corr.p2,tmp_corr.p1));
//								if(corr[gauche_id][droite_id].size()==1)
//								{//si on rajoute avant le debut(c'est trie ...) il faut remettre a jour le begin ...
									corr_begin = corr[gauche_id][droite_id].begin();
									corr_begin_begin = corr_begin;
//								}
								corr_end = corr[gauche_id][droite_id].end();
							}
						}
						else
						{
							int aaa=delta_text;
							for(auto& a : text_box)
							{
								++aaa;
								if(a.contains(localPosition_tmp.x,localPosition_tmp.y))
								{
									std::cout << "texte n°" << aaa << std::endl;;
								//	std::cout << a.top << " " << localPosition_tmp.y << " " << a.height<<std::endl;;
									break;
								}
							}
						}
					}
					 break;
				 case sf::Event::Resized:
					 set_window_size(event.size.width, event.size.height);
				 break;
				 default:
					 break;
			  }
		 }
	}


	return EXIT_SUCCESS;
}
