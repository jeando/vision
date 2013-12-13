#ifndef CORRESPONDANCE_H
#define CORRESPONDANCE_H

#include<vector>
#include<opencv2/core/core.hpp>
#include<iostream>


template<typename T>
class correspondance;

template<typename T>
std::ostream& operator<<(std::ostream& os, const correspondance<T>& c);
template<typename T>
class correspondance
{
	public:
	T x1,x2,y1,y2;
	friend std::ostream& operator<< <>(std::ostream& os, const correspondance& c);
};
template<typename T>
std::ostream& operator<<(std::ostream& os, const correspondance<T>& c)
{
	os << "(" << c.x1 << ", " << c.y1 << ")  ->  (" << c.x2 << ", " << c.y2 << ")";
	return os;
}

class point_i
{
	public:
	int id;
	double x,y;
};
template<typename T>
cv::Mat_<T> compute_homographie(std::vector<correspondance<T>>& appariement)
{

}
#endif

