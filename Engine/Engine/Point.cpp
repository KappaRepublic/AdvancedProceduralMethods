#include "Point.h"

Point::Point()
{
}

Point Point::operator+(Point p) {
	return Point(xCoord + p.xCoord, yCoord + p.yCoord);
}

bool Point::operator==(Point p) {
	return (xCoord == p.xCoord && yCoord == p.yCoord);
}

bool Point::operator==(int i) {
	return false;
}

bool Point::operator!=(Point p) {
	return (xCoord != p.xCoord || yCoord != p.yCoord);
}

Point::Point(int x, int y) {
	xCoord = x;
	yCoord = y;
}

void Point::offset(Point offset)
{
	xCoord += offset.xCoord;
	yCoord += offset.yCoord;
}

bool Point::equals(Point comparison) {
	return xCoord == comparison.xCoord && yCoord == comparison.yCoord;
}
