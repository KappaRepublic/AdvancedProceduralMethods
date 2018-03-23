#pragma once

class Point {
public:
	Point();
	Point(int x, int y);

	Point operator+(Point p);
	bool operator==(Point p);
	bool operator==(int i);
	bool operator!=(Point p);

	void offset(Point offset);
	bool equals(Point comparison);

	int xCoord, yCoord;
};

