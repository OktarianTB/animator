#include "Point.h"
#include <cmath>

Point::Point(void)
	:x(0.0),
	 y(0.0)
{

}

Point::Point(const float& new_x, const float& new_y)
	:x(new_x),
	 y(new_y)
{

}

void Point::toStream(std::ostream & output_stream) const
{
	output_stream << x << std::endl;
	output_stream << y << std::endl;
}

void Point::fromStream(std::istream & input_stream)
{
	input_stream >> x;
	input_stream >> y;
}

bool operator==(const Point& p1, const Point& p2)
{
	if (abs(p1.x - p2.x) < 0.01 && abs(p1.y - p2.y) < 0.01)
		return true;
	return false;
}

std::ostream & operator<<(std::ostream & output_stream, const Point & point)
{
	point.toStream(output_stream);

	return output_stream;
}

std::istream & operator>>(std::istream & input_stream, Point & point)
{
	point.fromStream(input_stream);

	return input_stream;
}

bool PointSmallerXCompare::operator()(const Point & first, const Point & second) const
{
	bool hasSmallerX = (first.x < second.x);

	return hasSmallerX;
}

bool PointLargerXCompare::operator()(const Point & first, const Point & second) const
{
	bool hasLargerX = (first.x > second.x);

	return hasLargerX;
}
