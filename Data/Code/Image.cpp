#include "Image.h"

#include <iostream>
#include<sstream>
#include<cassert>

using namespace std;

Image::Image(int _id, string nom_fich, int w, int h)
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
}
std::vector<correspondance<double>>& Image::operator[](int i)
{
	return assoc[i];
}
