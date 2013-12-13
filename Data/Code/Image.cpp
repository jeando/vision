#include "Image.h"

#include <iostream>
#include "Image.h"
using namespace std;

Image::Image(int _id, string nom_fich, int w, int h)
:id(_id),name(nom_fich),width(w),height(h)
{
    //ctor
}

void Image::print()
{
	std::cout << name << " " << id << " " << width << "x" << height << std::endl;
}

Image::~Image()
{
    //dtor
}
std::vector<correspondance<double>>& Image::operator[](int i)
{
	return assoc[i];
}
