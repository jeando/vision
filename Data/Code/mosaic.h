#ifndef _LECTURE_H_
#define _LECTURE_H_
#include <iostream>
#include <map>
#include <memory>
#include "Image.h"
#include <vector>
template<typename T>
class mosaic
{
	private:
		std::map<int, std::unique_ptr<Image<T>>> img;
		Image<T> result;
	public:
		mosaic();
		void lecture_appariement(std::string file);
		void compute_mosaic();	
		void test();
};
#include"mosaic.cpp"
#endif

