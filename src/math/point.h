#ifndef __KOCHERGIN_TEST_TASK_POINT_H__
#define __KOCHERGIN_TEST_TASK_POINT_H__

/*
* Abstract point type with basic operations.
* TBH this is from one of my previous projects.
*/

template <typename T>
struct basic_point 
{
	typedef T axis_type;

	axis_type x, y;

	explicit basic_point(axis_type value = axis_type(0)) { reset(value, value); }
	basic_point(axis_type x_, axis_type y_) { reset(x_, y_); }

	basic_point& reset(axis_type x_, axis_type y_);

	basic_point& operator+=(const basic_point& p);
	basic_point& operator-=(const basic_point& p);
	basic_point& operator*=(axis_type k);
	basic_point& operator/=(axis_type k);

	basic_point operator-() const { return basic_point(-x,-y); }
};

template <typename T>
inline basic_point<T>& basic_point<T>::reset(axis_type x_, axis_type y_)
{
	x = x_; y = y_;
	return *this;
}

template <typename T>
inline basic_point<T>& basic_point<T>::operator+=(const basic_point& p)
{
	return this->reset(x + p.x, y + p.y);
}

template <typename T>
inline basic_point<T>& basic_point<T>::operator-=(const basic_point& p)
{
	return this->reset(x - p.x, y - p.y);
}

template <typename T>
inline basic_point<T>& basic_point<T>::operator*=(axis_type k)
{
	return this->reset(x * k, y * k);
}

template <typename T>
inline basic_point<T>& basic_point<T>::operator/=(axis_type k)
{
	return this->reset(x / k, y / k);
}


template <typename T>
inline bool operator==(const basic_point<T>& p1,const basic_point<T>& p2)
{
	return (p1.x == p2.x) && (p1.y == p2.y);
}

template <typename T>
inline bool operator!=(const basic_point<T>& p1,const basic_point<T>& p2)
{
	return !(p1 == p2);
}


template <typename T>
inline const basic_point<T> operator+(const basic_point<T>& p1,const basic_point<T>& p2)
{
	return basic_point<T>(p1.x + p2.x,p1.y + p2.y);
}

template <typename T>
inline const basic_point<T> operator-(const basic_point<T>& p1,const basic_point<T>& p2)
{
	return basic_point<T>(p1.x - p2.x,p1.y - p2.y);
}

template <typename T,typename U>
inline const basic_point<T> operator*(const basic_point<T>& p,U k)
{
	return basic_point<T>(p.x * k,p.y * k);
}

template <typename T,typename U>
inline const basic_point<T> operator*(U k,const basic_point<T>& p)
{
	return basic_point<T>(p.x * k,p.y * k);
}

template <typename T,typename U>
inline const basic_point<T> operator/(const basic_point<T>& p,U k)
{
	return basic_point<T>(p.x / k,p.y / k);
}

#endif // #define __KOCHERGIN_TEST_TASK_POINT_H__
