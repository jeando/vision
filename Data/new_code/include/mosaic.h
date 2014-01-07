#ifndef _LECTURE_H_
#define _LECTURE_H_
#include <iostream>
#include <map>
#include <memory>
#include "Image.h"
#include <vector>
#include<opencv2/core/core.hpp>

template<typename T>
class mosaic
{
	private:
		std::map<int, std::unique_ptr<Image<T>>> img;
		Image<T> result;
	public:
		mosaic();
		void compute_homographie(const std::vector<correspondance<T>>& c, cv::Mat_<T>& h);
		void lecture_appariement(std::string file);
		void compute_next_mosaic();
		int size(){return img.size();}
		void init_compute_mosaic();	
		void test();
	
		Image<T>& get_result(){return result;}
};
#include"mosaic.cpp"
#endif

