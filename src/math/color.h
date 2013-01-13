#ifndef __KOCHERGIN_TEST_TASK_COLOR_H__
#define __KOCHERGIN_TEST_TASK_COLOR_H__

/*
* Basic color data structure.
*/

template <typename T>
struct basic_rgb_color
{
	typedef T native_type;

	native_type red, green, blue;

	explicit basic_rgb_color(native_type value = native_type(0));
	basic_rgb_color(native_type red_, native_type green_, native_type blue_);

	basic_rgb_color& reset(native_type red_, native_type green_, native_type blue_);
};


template <typename T>
inline basic_rgb_color<T>::basic_rgb_color(native_type value)
{
	reset(value,value,value);
}

template <typename T>
inline basic_rgb_color<T>::basic_rgb_color(native_type red_, native_type green_, native_type blue_)
{
	reset(red_,green_,blue_);
}

template <typename T>
inline basic_rgb_color<T>& basic_rgb_color<T>::reset(native_type red_, native_type green_, native_type blue_)
{
	red = red_; green = green_; blue = blue_;
	return *this;
}

template <typename T>
inline bool operator==(const basic_rgb_color<T>& c1,const basic_rgb_color<T>& c2)
{
	return (c1.red == c2.red) && (c1.green == c2.green) && (c1.blue == c2.blue);
}

template <typename T>
inline bool operator!=(const basic_rgb_color<T>& c1,const basic_rgb_color<T>& c2)
{
	return !(c1 == c2);
}

#endif	// !defined(__KOCHERGIN_TEST_TASK_COLOR_H__)
