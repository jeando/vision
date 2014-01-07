#ifndef VEC_HHH
#define VEC_HHH

#include<iostream>

template<typename T>
class vec3;
template<typename T>
class vec2;
template<typename T>
std::ostream& operator<<(std::ostream& os, const vec2<T>& c);
template<typename T>
std::ostream& operator<<(std::ostream& os, const vec3<T>& c);

template<typename T>
class vec2
{
	public:
	T x,y;
	vec2();
	vec2(T _x, T _y);
	vec2(const vec2<T>& v);
	vec2(const vec3<T>& v);
	vec2& operator=(const vec2<T>& v);
	vec2& operator=(const vec3<T>& v);
	template<typename U> bool operator==(const vec2<U>& v)const
	{
		return x==v.x && y==v.y;
	}
	template<typename U> bool operator<(const vec2<U>& v)const
	{
		return x<v.x || (x==v.x&& y<v.y);
	}
	friend std::ostream& operator<< <>(std::ostream& os, const vec2& v);
};
template<typename T>
class vec3
{
	public:
	T x,y,z;
	vec3();
	vec3(T _x, T _y, T _z);
	vec3(const vec3<T>& v);
	vec3(const vec2<T>& v);
	vec3& operator=(const vec3<T>& v);
	vec3& operator=(const vec2<T>& v);
	friend std::ostream& operator<< <>(std::ostream& os, const vec3& v);
};

template<typename T>
vec3<T> operator*(const cv::Mat_<T>& m, const vec2<T>& v)
{
	return vec3<T>(m[0][0]*v.x+m[0][1]*v.y+m[0][2],m[1][0]*v.x+m[1][1]*v.y+m[1][2],m[2][0]*v.x+m[2][1]*v.y+m[2][2]);
}
template<typename T>
class __mat
{
	public:
		const T* m;
		__mat(const T aa[9]):m(aa){};
		const T& operator[](int i)const{return m[i];}
};
template<typename T>
vec3<T> operator*(const __mat<T>& m, const vec2<T>& v)
{
	return vec3<T>(m[0]*v.x+m[1]*v.y+m[2],m[3]*v.x+m[4]*v.y+m[5],m[6]*v.x+m[7]*v.y+m[8]);
}

#include"vec.cpp"
#endif

