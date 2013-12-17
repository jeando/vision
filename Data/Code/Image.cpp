#include "Image.h"

#include <iostream>
#include<sstream>
#include<cassert>

template<typename T>
Image<T>::Image(int _id, std::string nom_fich, T w, T h)
:id(_id)
{
    img.push_back(image<T>(nom_fich));
	img[0][0] = vec2<T>(0,0);
	img[0][1] = vec2<T>(w,0);
	img[0][2] = vec2<T>(w,h);
	img[0][3] = vec2<T>(0,h);
}
template<typename T>
Image<T>::Image(std::string line)
{
	img.resize(1);
	std::stringstream ss(line);
	ss >> id >> img[0].name >> img[0][2].x;
	ss.ignore();//'x'
	ss >> img[0][2].y;
	img[0][0] = vec2<T>(0,0);
	img[0][1] = vec2<T>(img[0][2].x,0);
	img[0][3] = vec2<T>(0,img[0][2].y);
}
template<typename T>
Image<T>::Image()
{
	/*
	img.push_back(image(""));
	img[0][0] = vec2(0,0);
	img[0][1] = vec2(1,0);
	img[0][2] = vec2(1,1);
	img[0][3] = vec2(0,1);
	//*/
}
template<typename T>
Image<T>::~Image()
{
    //dtor
}

template<typename T>
void Image<T>::print()const
{
	std::cout << img[0].name << " " << id << " " << img[0][2].x << "x" << img[0][2].y << std::endl;
}

template<typename T>
void Image<T>::add_correspondance(const point_i<T>& p1, const point_i<T>& p2)
{
	assert(p1.id==id);
	assoc[p2.id].push_back({p1.p,p2.p});
//	std::cout << assoc[p2.id][0]<<std::endl;;
}
template<typename T>
std::vector<correspondance<T>>& Image<T>::operator[](int i)
{
	return assoc[i];
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Image<T>& i)
{
	os << i.img[0].name << " " << i.id << " " << i.img[0][2].x << "x" << i.img[0][2].y << std::endl;
//	typedef std::map<int, std::vector<correspondance<double>>>::const_iterator it_type;
    for(const auto& it : i.assoc)
	{
		os << it.first << std::endl;
		for(const correspondance<T>& c : it.second)
		{
			os << '\t' << c << std::endl;
		}
    	//for(auto iterator = i.assoc.begin(); iterator != i.assoc.end(); iterator++) {
        //os << "/t" << *iterator << std::endl;
    }
	return os;
}
template<typename T>
int Image<T>::get_id_closest_img()
{
	int val(-1);
	int tmp;
	int indice(-1);
	for(const auto& a : assoc)
	{
		tmp = a.second.size();
		if(tmp>val)
		{
			tmp=val;
			indice=a.first;
		}
	}
	return indice;
}
template<typename T>
void Image<T>::add_tex(const image<T>& i)
{
	img.push_back(i);
}
