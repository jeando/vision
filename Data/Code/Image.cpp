#include "Image.h"

#include <iostream>
#include<sstream>
#include<cassert>

Image::Image(int _id, std::string nom_fich, int w, int h)
:id(_id),name(nom_fich),width(w),height(h)
{
    //ctor
}
Image::Image(std::string line)
{
	std::stringstream ss(line);
	ss >> id >> name >> width;
	ss.ignore();//'x'
	ss >> height;
}
Image::~Image()
{
    //dtor
}

void Image::print()const
{
	std::cout << name << " " << id << " " << width << "x" << height << std::endl;
}

void Image::add_correspondance(const point_i& p1, const point_i& p2)
{
	assert(p1.id==id);
	assoc[p2.id].push_back({p1.x,p1.y,p2.x,p2.y});
	std::cout << assoc[p2.id][0]<<std::endl;;
}
std::vector<correspondance<double>>& Image::operator[](int i)
{
	return assoc[i];
}

std::ostream& operator<<(std::ostream& os, const Image& i)
{
	os << i.name << " " << i.id << " " << i.width << "x" << i.height << std::endl;
	typedef std::map<int, std::vector<correspondance<double>>>::iterator it_type;
    for(it_type iterator = i.assoc.begin(); iterator != i.assoc.end(); iterator++) {
        os << "/t" << *it_type << std::endl;
    }
	return os;
}
