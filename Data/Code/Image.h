#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <vector>
#include <map>
#include "correspondance.h"
#include<array>

#include"vec.h"

template<typename T>
class image
{
	public:
	std::string name;
	std::array<vec2<T>,4> vertex;//A,B,C,D;
	image():name(""){};
	image(std::string s):name(s){};
	image(std::string s, std::array<vec2<T>,4> v):name(s),vertex(v){};
	vec2<T>& operator[](int i)
	{
		return vertex[i];
	}
	const vec2<T>& operator[](int i)const
	{
		return vertex[i];
	}

	//"coller" texture(name)(0,0) en A, (1,0) en B, (1,1) en C et (0,1) en D
};
template<typename T>
class Image;
template<typename T>
std::ostream& operator<< (std::ostream& os, const Image<T>& i);
template<typename T>
class Image
{
    public:
        Image(int _id, std::string nom_fich, T w, T h);
        Image(std::string line);
        Image(int id=-1);
        void print()const;
        virtual ~Image();
		size_t nb_img_assoc(){return assoc.size();}
		std::vector<correspondance<T>>& operator[](int i);
		void erase(int i);
		int get_id(){return id;}
		void add_correspondance(const point_i<T>& p1, const point_i<T>& p2);
		int get_id_closest_img();
		friend std::ostream& operator<< <>(std::ostream& os, const Image<T>& i);
		void add_tex(const image<T>& i);
		const std::vector<image<T>>& get_img()const{return img;}
		const std::map<int,std::vector<correspondance<T>>>& get_assoc(){return assoc;}
    protected:

    private:
        int id;
        //std::string name;
		std::map<int,std::vector<correspondance<T>>> assoc;
		//vec2 A,B,C,D;
		std::vector<image<T>> img;
/*

  A----B
  |    |
  D----C

*/
};
#include"Image.cpp"
#endif // IMAGE_H

