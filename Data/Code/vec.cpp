#include"vec.h"


template<typename T>
inline vec2<T>::vec2()
	:x(1),
	y(1)
{
}
template<typename T>
inline vec2<T>::vec2(T _x, T _y)
	:x(_x),
	y(_y)
{
}
template<typename T>
inline vec2<T>::vec2(const vec2<T>& v)
	:x(v.x),
	y(v.y)
{
}
template<typename T>
inline vec2<T>::vec2(const vec3<T>& v)
	:x(v.x/v.z),
	y(v.y/v.z)
{
}

template<typename T>
inline vec2<T>& vec2<T>::operator=(const vec3<T>& v)
{
	x=v.x/v.z;
	y=v.y/v.z;
	return *this;
}
template<typename T>
inline vec2<T>& vec2<T>::operator=(const vec2& v)
{
	x=v.x;
	y=v.y;
	return *this;
}
template<typename T>
inline std::ostream& operator<<(std::ostream& os, const vec2<T>& c)
{
	os << "(" << c.x << ", " << c.y << ")";
	return os;
}


template<typename T>
inline vec3<T>::vec3()
	:x(1),
	y(1),z(1)
{
}
template<typename T>
inline vec3<T>::vec3(T _x, T _y, T _z)
	:x(_x),
	y(_y),
	z(_z)
{
}
template<typename T>
inline vec3<T>::vec3(const vec3<T>& v)
	:x(v.x),
	y(v.y),
	z(v.z)
{
}
template<typename T>
inline vec3<T>::vec3(const vec2<T>& v)
	:x(v.x),
	y(v.y),
	z(1)
{
}
template<typename T>
inline vec3<T>& vec3<T>::operator=(const vec3<T>& v)
{
	x=v.x;
	y=v.y;
	z=v.z;
	return *this;
}
template<typename T>
inline vec3<T>& vec3<T>::operator=(const vec2<T>& v)
{
	x=v.x;
	y=v.y;
	z=1;
	return *this;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const vec3<T>& c)
{
	os << "(" << c.x << ", " << c.y << ")";
	return os;
}



