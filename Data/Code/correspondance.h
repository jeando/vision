#ifndef CORRESPONDANCE_H
#define CORRESPONDANCE_H

#include<vector>
#include<opencv2/core/core.hpp>

template<typename T>
class correspondance
{
	public:
	T x1,x2,y1,y2;
};

template<typename T>
cv::Mat_<T> compute_homographie(std::vector<correspondance<T>>& appariement)
{

}
#endif

