#ifndef CORRESPONDANCE_H
#define CORRESPONDANCE_H

#include<vector>
#include<opencv2/core/core.hpp>
#include<iostream>
#include"vec.h"


template<typename T>
class correspondance;
template<typename T>
std::ostream& operator<< (std::ostream& os, const correspondance<T>& c);

template<typename T>
class correspondance
{
	public:
	vec2<T> p1,p2;//1,y1,x2,y2;
	correspondance(vec2<T> _p1 = vec2<T>(), vec2<T> _p2 = vec2<T>()):p1(_p1),p2(_p2){}
	template<typename U> bool operator==(const correspondance<U>& c)const
	{
		return p1==c.p1 && p2==c.p2;
	}
	template<typename U> bool operator<(const correspondance<U>& c)const
	{
		return p1<c.p1 || (p1==c.p1&& p2<c.p2);
	}
	friend std::ostream& operator<< <>(std::ostream& os, const correspondance& c);
};
template<typename T>
std::ostream& operator<<(std::ostream& os, const correspondance<T>& c)
{
	os << c.p1 << "  ->  " << c.p2;
	return os;
}

template<typename T>
class point_i
{
	public:
	int id;
	vec2<T> p;//double x,y;
};
/*template<typename T>
cv::Mat_<T> compute_homographie(std::vector<correspondance>& appariement)
{

}
//*/
#endif

