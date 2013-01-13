#ifndef __KOCHERGIN_TEST_TASK_CIRCLE_H__
#define __KOCHERGIN_TEST_TASK_CIRCLE_H__

/*
* Abstract circle type. 
* Brainless but simple.
*/

#include "math/point.h"


template <typename T>
struct basic_circle
{
	typedef T axis_type;
	typedef basic_point<axis_type> point_type;

	point_type pos;
	axis_type radius;

	explicit basic_circle(axis_type value = axis_type(0)) { pos.reset(value, value); radius = value; }
	basic_circle(axis_type x_, axis_type y_, axis_type radius_) { pos.reset(x_, y_); radius = radius_; }

	bool is_inside(point_type pt) const;
};


template <typename T>
bool basic_circle<T>::is_inside(point_type pt) const
{
	point_type c_pt = pt - pos; // pt coordinates from circle center
	return radius * radius > c_pt.x * c_pt.x + c_pt.y * c_pt.y;
}


#endif // #define __KOCHERGIN_TEST_TASK_CIRCLE_H__
