#include "Image.h"

#include <iostream>
#include "Image.h"
using namespace std;

Image::Image(int num, string id, int w, int h)
:numero(num),identity(id),width(w),height(h)
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
