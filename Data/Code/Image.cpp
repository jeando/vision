#include "Image.h"

#include <iostream>
#include "Image.h"
using namespace std;

Image::Image(int id, string nom_fich, int w, int h)
:nom_fichier(nom_fich),identity(id),width(w),height(h)
{
    //ctor
}

void Image::print()
{
    cout << numero << " " << identity << " " << width << "x" << height << endl;
}

Image::~Image()
{
    //dtor
}
