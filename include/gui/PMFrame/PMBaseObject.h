//********************************************************************************
//文件：PMBaseObject.h
//作者：feiren
//时间：2011-8-20
//用途：封装基础实体
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMBASEOBJECT_H__
#define __PMBASEOBJECT_H__


#include "TCommonTools.h"

enum TDirection
{
	DIRECT_INSIDE=0x00,
	DIRECT_TOP   =0x02,
	DIRECT_LEFT  =0x04,
	DIRECT_BOTTON=0x08,
	DIRECT_RIGHT =0x10,
};

enum TPlacement
{
	PLACE_FIRST_QUADRANT,
	PLACE_SECOND_QUADRANT,
	PLACE_THIRD_QUADRANT,
	PLACE_FOUR_QUADRANT,
};

template<typename T>
struct TPoint
{
	T x;
	T y;

	TPoint() :
	x(0),
		y(0)
	{
	}

	TPoint(T const& _x, T const& _y) :
	x(_x),
		y(_y)
	{
	}

	TPoint(TPoint const& _obj) :
	x(_obj.x),
		y(_obj.y)
	{
	}

	TPoint& operator -= (TPoint const& _obj)
	{
		x -= _obj.x;
		y -= _obj.y;
		return *this;
	}

	TPoint& operator += (TPoint const& _obj)
	{
		x += _obj.x;
		y += _obj.y;
		return *this;
	}

	TPoint operator - (TPoint const& _obj) const
	{
		return TPoint(x - _obj.x, y - _obj.y);
	}

	TPoint operator + (TPoint const& _obj) const
	{
		return TPoint(x + _obj.x, y + _obj.y);
	}

	TPoint& operator = (TPoint const& _obj)
	{
		x = _obj.x;
		y = _obj.y;
		return *this;
	}

	template<typename U>
	TPoint& operator = (TPoint<U> const& _obj)
	{
		x = _obj.x;
		y = _obj.y;
		return *this;
	}

	bool operator <(TPoint const& _obj)const
	{
		if(x<_obj.x)return true;
		if (x==_obj.x)return y<_obj.y;
		return false;
	}

	bool operator >(TPoint const& _obj)const
	{
		if(x>_obj.x)return true;
		if (x==_obj.x)return y>_obj.y;
		return false;
	}

	bool operator == (TPoint const& _obj) const
	{
		return ((x == _obj.x) && (y == _obj.y));
	}

	bool operator != (TPoint const& _obj) const
	{
		return ! ((x == _obj.x) && (y == _obj.y));
	}

	void clear()
	{
		x = y = 0;
	}

	void set(T const& _x, T const& _y)
	{
		x = _x;
		y = _y;
	}

	void swap(TPoint& _value)
	{
		TPoint tmp = _value;
		_value = *this;
		*this = tmp;
	}

	bool empty() const
	{
		return ((x == 0) && (y == 0));
	}

	TPoint intersect(TPoint& _value)
	{		
		TPoint result;
		if((y>=_value.x && y<=_value.y) || (x<=_value.y&&x>=_value.x))
		{
			if(x<=_value.x&&y<=_value.y)
			{
				result.set(_value.x,y);
			}else if(x>=_value.x&&y>=_value.y)
			{
				result.set(x,_value.y);
			}else if(x>=_value.x&&y<=_value.y)
			{
				result.set(x,y);
			}else if(x<=_value.x&&y>=_value.y)
			{
				result.set(_value.x,_value.y);
			}
		}else if(x<=_value.x && y>=_value.y)
		{
			result.set(_value.x,_value.y);
		}		
		return result;
	}

	bool intersect(int _x,int _y)
	{		
		if((y>=_x && y<=_y) || (x<=_y&&x>=_x))
		{
			if(x<=_x&&y<=_y)
			{
				return true;
			}else if(x>=_x&&y>=_y)
			{
				return true;
			}else if(x>=_x&&y<=_y)
			{
				return true;
			}else if(x<=_x&&y>=_y)
			{
				return true;
			}
		}else if(x<=_x && y>=_y)
		{
			return true;
		}		
		return false;
	}

	std::string print() const
	{
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}

	static TPoint<T> parse(const std::string& _value)
	{
		TPoint<T> result;
		std::istringstream stream(_value);
		stream >> result.x >> result.y;
		if (stream.fail())
		{
			return TPoint<T>();
		}
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return TPoint<T>();
				item = stream.get();
			}
		}
		return result;
	}

	friend std::ostream& operator << (std::ostream& _stream, const TPoint<T>&  _value)
	{
		_stream << _value.x << " " << _value.y;
		return _stream;
	}

	friend std::istream& operator >> (std::istream& _stream, TPoint<T>&  _value)
	{
		_stream >> _value.x >> _value.y;
		if (_stream.fail())
			_value.clear();
		return _stream;
	}
};

template<typename T>
struct TPoint3D
{
	T x;
	T y;
	T z;

	TPoint3D() :
	x(0),y(0),z(0)
	{
	}

	TPoint3D(T const& _x, T const& _y, T const& _z=0) :
	x(_x),y(_y),z(_z)
	{
	}

	TPoint3D(TPoint3D const& _obj) :
	x(_obj.x),y(_obj.y),z(_obj.z)
	{
	}
	bool operator <(TPoint3D const& _obj)const
	{
		if(x<_obj.x)return true;
		if (x==_obj.x)
		{
			if(y<_obj.y)return true;
			if(y==_obj.y)return z<_obj.z;
		}
		return false;
	}
	void clear()
	{
		x = y = y = 0;
	}

	void set(T const& _x, T const& _y, T const& _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

template<typename T>
struct TSize
{
	T width;
	T height;

	TSize() :
	width(0),
		height(0)
	{
	}

	TSize(T const& _width, T const& _height) :
	width(_width),
		height(_height)
	{
	}

	TSize(TSize const& _obj) :
	width(_obj.width),
		height(_obj.height)
	{
	}

	TSize& operator -= (TSize const& _obj)
	{
		width -= _obj.width;
		height -= _obj.height;
		return *this;
	}

	TSize& operator += (TSize const& _obj)
	{
		width += _obj.width;
		height += _obj.height;
		return *this;
	}

	TSize operator - (TSize const& _obj) const
	{
		return TSize(width - _obj.width, height - _obj.height);
	}

	TSize operator + (TSize const& _obj) const
	{
		return TSize(width + _obj.width, height + _obj.height);
	}

	TSize& operator = (TSize const& _obj)
	{
		width = _obj.width;
		height = _obj.height;
		return *this;
	}

	template<typename U>
	TSize& operator = (TSize<U> const& _obj)
	{
		width = _obj.width;
		height = _obj.height;
		return *this;
	}

	bool operator == (TSize const& _obj) const
	{
		return ((width == _obj.width) && (height == _obj.height));
	}

	bool operator != (TSize const& _obj) const
	{
		return !((width == _obj.width) && (height == _obj.height));
	}

	void clear()
	{
		width = height = 0;
	}

	void set(T const& _width, T const& _height)
	{
		width = _width;
		height = _height;
	}

	void swap(TSize& _value)
	{
		TSize tmp = _value;
		_value = *this;
		*this = tmp;
	}

	bool empty() const
	{
		return ((width == 0) && (height == 0));
	}

	std::string print() const
	{
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}

	static TSize<T> parse(const std::string& _value)
	{
		TSize<T> result;
		std::istringstream stream(_value);
		stream >> result.width >> result.height;
		if (stream.fail())
		{
			return TSize<T>();
		}
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return TSize<T>();
				item = stream.get();
			}
		}
		return result;
	}

	friend std::ostream& operator << (std::ostream& _stream, const TSize<T>&  _value)
	{
		_stream << _value.width << " " << _value.height;
		return _stream;
	}

	friend std::istream& operator >> (std::istream& _stream, TSize<T>&  _value)
	{
		_stream >> _value.width >> _value.height;
		if (_stream.fail())
			_value.clear();
		return _stream;
	}
};


template<typename T>
struct TRect
{
	T left;
	T top;
	T right;
	T bottom;

	TRect() :
	left(0),
		top(0),
		right(0),
		bottom(0)
	{
	}

	TRect(T const& _left, T const& _top, T const& _right, T const& _bottom) :
	left(_left),
		top(_top),
		right(_right),
		bottom(_bottom)
	{
	}

	TRect(TRect const& _obj) :
	left(_obj.left),
		top(_obj.top),
		right(_obj.right),
		bottom(_obj.bottom)
	{
	}
	RECT ToRECT()
	{
		RECT rect;
		rect.top	= top;
		rect.left	= left;
		rect.right	= right;
		rect.bottom = bottom;
		return rect;
	}
	void Move(T const& _X,T const& _Y)
	{
		left+=_X;
		right+=_X;
		top+=_Y;
		bottom+=_Y;
	}
	void MoveX(T const& _X)
	{
		left+=_X;
		right+=_X;
	}
	void MoveY(T const& _Y)
	{
		top+=_Y;
		bottom+=_Y;
	}
	bool operator <(TRect const& _obj)const
	{
		if(left<_obj.left)return true;
		if (left==_obj.left)
		{
			if(top<_obj.top)return true;
			if(top==_obj.top)
			{
				if(right<_obj.right)return true;
				if(right==_obj.right)return bottom<_obj.bottom;
			}
		}
		return false;
	}
	TRect& operator -= (TRect const& _obj)
	{
		left -= _obj.left;
		top -= _obj.top;
		right -= _obj.right;
		bottom -= _obj.bottom;
		return *this;
	}

	TRect& operator += (TRect const& _obj)
	{
		left += _obj.left;
		top += _obj.top;
		right += _obj.right;
		bottom += _obj.bottom;
		return *this;
	}

	TRect& operator += (TPoint<T> const& _obj)
	{
		left += _obj.x;
		top += _obj.y;
		right += _obj.x;
		bottom += _obj.y;
		return *this;
	}

	TRect& operator += (T const& x)
	{
		left += x;
		right += x;
		return *this;
	}

	TRect operator - (TRect const& _obj) const
	{
		return TRect(left - _obj.left, top - _obj.top, right - _obj.right, bottom - _obj.bottom);
	}

	TRect operator + (TRect const& _obj) const
	{
		return TRect(left + _obj.left, top + _obj.top, right + _obj.right, bottom + _obj.bottom);
	}

	TRect& operator = (TRect const& _obj)
	{
		left = _obj.left;
		top = _obj.top;
		right = _obj.right;
		bottom = _obj.bottom;
		return *this;
	}
	TRect& operator = (RECT const& _obj)
	{
		left   = _obj.left;
		top    = _obj.top;
		right  = _obj.right;
		bottom = _obj.bottom;
		return *this;
	}
	template<typename U>
	TRect& operator = (TRect<U> const& _obj)
	{
		left = _obj.left;
		top = _obj.top;
		right = _obj.right;
		bottom = _obj.bottom;
		return *this;
	}

	bool operator == (TRect const& _obj) const
	{
		return ((left == _obj.left) && (top == _obj.top) && (right == _obj.right) && (bottom == _obj.bottom));
	}

	bool operator != (TRect const& _obj) const
	{
		return !((left == _obj.left) && (top == _obj.top) && (right == _obj.right) && (bottom == _obj.bottom));
	}

	T width() const
	{
		return right - left;
	}

	T height() const
	{
		return bottom - top;
	}
	void zoom(T const& _delta)
	{
		left  -= _delta;
		top   -= _delta;
		right += _delta;
		bottom+= _delta;
	}
	void zoom(T const& _deltaX,T const& _deltaY)
	{
		left  -= _deltaX;
		top   -= _deltaX;
		right += _deltaY;
		bottom+= _deltaY;
	}
	void clear()
	{
		left = top = right = bottom = 0;
	}

	void set(T const& _left, T const& _top, T const& _right, T const& _bottom)
	{
		left = _left;
		top = _top;
		right = _right;
		bottom = _bottom;
	}

	void setNoNull(T const& _left, T const& _top, T const& _right, T const& _bottom)
	{
		left = _left==0?1:_left;
		top = _top==0?1:_top;
		right = _right==0?1:_right;
		bottom = _bottom==0?1:_bottom;
	}

	void swap(TRect& _value)
	{
		TRect tmp = _value;
		_value = *this;
		*this = tmp;
	}

	bool empty() const
	{
		return ((left == 0) && (top == 0) && (right == 0) && (bottom == 0));
	}

	bool inside(const TRect<T>&  _value) const
	{
		return ((_value.left >= left) && (_value.right <= right) && (_value.top >= top) && (_value.bottom <= bottom));
	}	

	bool inside(const POINT&  _value) const
	{
		return ((_value.x >= left) && (_value.x <= right) && (_value.y >= top) && (_value.y <= bottom));
	}

	int GetPlacement(T const &x, T const &y)const
	{
		T halfX = (left+right)/2;
		T halfY = (top+bottom)/2;
		if (x>=halfX&&y<halfY)
		{
			return PLACE_FIRST_QUADRANT;
		}
		if (x>=halfX&&y>=halfY)
		{
			return PLACE_FOUR_QUADRANT;
		}
		if (x<halfX&&y<halfY)
		{
			return PLACE_SECOND_QUADRANT;
		}
		return PLACE_THIRD_QUADRANT;
	}
	int GetAreaDirection(T const &x, T const &y)const
	{
		int di = DIRECT_INSIDE;
		if(x<left)
		{
			di |= DIRECT_TOP;
		}else if(x>right)
		{
			di |= DIRECT_BOTTON;
		}
		if(y<top)
		{
			di |= DIRECT_LEFT;
		}else if(y>bottom)
		{
			di |= DIRECT_RIGHT;
		}
		return di;
	}
	int GetDirection(T const &x, T const &y)const
	{
		int di = DIRECT_INSIDE;
		if(x<left)
		{
			di |= DIRECT_LEFT;
		}else if(x>right)
		{
			di |= DIRECT_RIGHT;
		}
		if(y<top)
		{
			di |= DIRECT_TOP;
		}else if(y>bottom)
		{
			di |= DIRECT_BOTTON;
		}
		return di;
	}

	TRect<T>   GetIntersection(const TRect<T>&  _value)const
	{
		TRect<T> intersection;
		TPoint<T> pt1(left,right),pt2(_value.left,_value.right),pt3;
		pt3=pt1.intersect(pt2);
		intersection.left=pt3.x;
		intersection.right=pt3.y;
		pt1.set(top,bottom);
		pt2.set(_value.top,_value.bottom);
		pt3=pt1.intersect(pt2);
		intersection.top=pt3.x;
		intersection.bottom=pt3.y;
		return intersection;
	}

	bool intersect(const TRect<T>&  _value) const
	{
		return (((unsigned int)_value.left <= (unsigned int)right) && 
			    ((unsigned int)_value.right >= (unsigned int)left) && 
				((unsigned int)_value.top <= (unsigned int)bottom) && 
				((unsigned int)_value.bottom >= (unsigned int)top));
	}

	bool inside(int x,int y) const
	{
		return ((x >= left) && (x <= right) && (y >= top) && (y <= bottom));
	}

	bool inside(const TPoint<T>&  _value) const
	{
		return ((_value.x >= left) && (_value.x <= right) && (_value.y >= top) && (_value.y <= bottom));
	}

	bool inside(const RECT &_value) const
	{
		TPoint<int> pt1,pt2;
		pt1.set(top,bottom);
		pt2.set(left,right);
		if(pt2.intersect(_value.left,_value.right) && (pt1.intersect(_value.top,_value.bottom)))return true;
		return false;
	}

	std::string print() const
	{
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}

	static TRect<T> parse(const std::string& _value)
	{
		TRect<T> result;
		std::istringstream stream(_value);
		stream >> result.left >> result.top >> result.right >> result.bottom;
		if (stream.fail())
		{
			return TRect<T>();
		}
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return TRect<T>();
				item = stream.get();
			}
		}
		return result;
	}

	friend std::ostream& operator << (std::ostream& _stream, const TRect<T>&  _value)
	{
		_stream << _value.left << " " << _value.top << " " << _value.right << " " << _value.bottom;
		return _stream;
	}

	friend std::istream& operator >> (std::istream& _stream, TRect<T>&  _value)
	{
		_stream >> _value.left >> _value.top >> _value.right >> _value.bottom;
		if (_stream.fail())
			_value.clear();
		return _stream;
	}
};


template<typename T>
struct TCoord
{
	T left;
	T top;
	T width;
	T height;

	TCoord() :
	left(0),
		top(0),
		width(0),
		height(0)
	{
	}

	TCoord(T const& _left, T const& _top, T const& _width, T const& _height) :
	left(_left),
		top(_top),
		width(_width),
		height(_height)
	{
	}

	TCoord(TCoord const& _obj) :
	left(_obj.left),
		top(_obj.top),
		width(_obj.width),
		height(_obj.height)
	{
	}

	TCoord(TPoint<T> const& _point, TSize<T> const& _size) :
	left(_point.x),
		top(_point.y),
		width(_size.width),
		height(_size.height)
	{
	}

	TCoord& operator -= (TCoord const& _obj)
	{
		left -= _obj.left;
		top -= _obj.top;
		width -= _obj.width;
		height -= _obj.height;
		return *this;
	}

	TCoord& operator += (TCoord const& _obj)
	{
		left += _obj.left;
		top += _obj.top;
		width += _obj.width;
		height += _obj.height;
		return *this;
	}

	TCoord operator - (TCoord const& _obj) const
	{
		return TCoord(left - _obj.left, top - _obj.top, width - _obj.width, height - _obj.height);
	}

	TCoord operator - (TPoint<T> const& _obj) const
	{
		return TCoord(left - _obj.x, top - _obj.y, width, height);
	}

	TCoord operator - (TSize<T> const& _obj) const
	{
		return TCoord(left, top, width - _obj.width, height - _obj.height);
	}

	TCoord operator + (TCoord const& _obj) const
	{
		return TCoord(left + _obj.left, top + _obj.top, width + _obj.width, height + _obj.height);
	}

	TCoord operator + (TPoint<T> const& _obj) const
	{
		return TCoord(left + _obj.x, top + _obj.y, width, height);
	}

	TCoord operator + (TSize<T> const& _obj) const
	{
		return TCoord(left, top, width + _obj.width, height + _obj.height);
	}

	TCoord& operator = (TCoord const& _obj)
	{
		left = _obj.left;
		top = _obj.top;
		width = _obj.width;
		height = _obj.height;
		return *this;
	}

	template< typename U >
	TCoord& operator = (TCoord<U> const& _obj)
	{
		left = _obj.left;
		top = _obj.top;
		width = _obj.width;
		height = _obj.height;
		return *this;
	}

	TCoord& operator = (TPoint<T> const& _obj)
	{
		left = _obj.x;
		top = _obj.y;
		return *this;
	}

	TCoord& operator = (TSize<T> const& _obj)
	{
		width = _obj.width;
		height = _obj.height;
		return *this;
	}

	bool operator == (TCoord const& _obj) const
	{
		return ((left == _obj.left) && (top == _obj.top) && (width == _obj.width) && (height == _obj.height));
	}

	bool operator != (TCoord const& _obj) const
	{
		return !((left == _obj.left) && (top == _obj.top) && (width == _obj.width) && (height == _obj.height));
	}

	T right() const
	{
		return left + width;
	}

	T bottom() const
	{
		return top + height;
	}

	void clear()
	{
		left = top = width = height = 0;
	}

	void set(T const& _left, T const& _top, T const& _width, T const& _height)
	{
		left = _left;
		top = _top;
		width = _width;
		height = _height;
	}

	void swap(TCoord& _value)
	{
		TCoord tmp = _value;
		_value = *this;
		*this = tmp;
	}

	bool empty() const
	{
		return ((left == 0) && (top == 0) && (width == 0) && (height == 0));
	}

	TPoint<T> point() const
	{
		return TPoint<T>(left, top);
	}

	TSize<T> size() const
	{
		return TSize<T>(width, height);
	}

	bool inside(const TPoint<T>&  _value) const
	{
		return ((_value.x >= left) && (_value.x <= right()) && (_value.y >= top) && (_value.y <= bottom()));
	}

	std::string print() const
	{
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}

	static TCoord<T> parse(const std::string& _value)
	{
		TCoord<T> result;
		std::istringstream stream(_value);
		stream >> result.left >> result.top >> result.width >> result.height;
		if (stream.fail())
		{
			return TCoord<T>();
		}
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return TCoord<T>();
				item = stream.get();
			}
		}
		return result;
	}

	friend std::ostream& operator << (std::ostream& _stream, const TCoord<T>&  _value)
	{
		_stream << _value.left << " " << _value.top << " " << _value.width << " " << _value.height;
		return _stream;
	}

	friend std::istream& operator >> (std::istream& _stream, TCoord<T>&  _value)
	{
		_stream >> _value.left >> _value.top >> _value.width >> _value.height;
		if (_stream.fail())
			_value.clear();
		return _stream;
	}
};


#endif