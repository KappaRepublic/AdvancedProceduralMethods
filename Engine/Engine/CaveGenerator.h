#pragma once

#include <list>
#include <vector>
#include <array>
#include <deque>
#include <array>
#include "Point.h"
#include "intVec2.h"

struct MapContainer {
	// std::array<int> x;
};

class CaveGenerator {
public:

	CaveGenerator();
	CaveGenerator(const CaveGenerator &cGen);
	~CaveGenerator(); 

	void debug();

	// Methods
	int build();
	bool connectCaves();

	// Parametres
	int neighbours;				// The number of closed neighbours a cell needs to be closed
	int closeCellProb;			// The probability of closing a visited cell
	int iterations;				// Number of times to visit cells
	intVec2 mapSize;			// Size of the generated level

	int lowerLimit;				// Remove any rooms smaller than this value
	int upperLimit;				// Remove any rooms larger than this value
	int emptyNeighbours;		// Remove single cells from cave edges
	int emptyCellNeighbours;	// Fills in holes within caves

	int corridorMin;			// The minimum length of a corridor
	int corridorMax;			// The maxinum length of a corridor
	int corridorMaxTurns;		// How many times a corridor can turn
	int corridorSpace;			// Distance corridor needs to be from a closed cell	
	int breakOut;				// Stop attempting to connect caves when this value is exceeded.

	int caveNumber;				// Number of caves generated

	// int map[(int)mapSize.x][(int)mapSize.y];

	std::vector<std::vector<int>> map;

	// Lookups
	std::deque<Point> directions = {
		Point(0, -1),	// North
		Point(0, 1),	// South
		Point(1, 0),	// East
		Point(-1, 0),	// West
	};

	std::deque<Point> directions1 = {
		Point(0, -1),	// North
		Point(0, 1),	// South
		Point(1, 0),	// East
		Point(-1, 0),	// West
		Point(1, -1),	// NorthEast
		Point(-1, -1),	// NorthWest
		Point (-1, 1),	// SouthWest	
		Point (1, 1),	// SouthEast
		Point(0, 0),	// Centre
	};

private:
	// Methods
	void buildCaves();
	void caveGetEdge(std::deque<Point> pCave, Point &pCavePoint, Point &pDirection);
	void getCaves();
	void locateCave(Point pCell, std::deque<Point> &pCave);
	void corridorGetEdge(Point &pLocation, Point &pDirection);
	std::list<Point> corridorAttempt(Point pStart, Point pDirection, bool pPreventBackTracking);
	bool corridorPointTest(Point pPoint, Point pDirection);
	std::list<Point> neighboursGet(Point p);
	std::list<Point> neighboursGet1(Point p);
	Point directionGet(Point p);
	Point directionGet(Point pDir, Point pDirExclude);
	Point directionReverse(Point pDir);
	bool pointCheck(Point p);
	void pointSet(Point p, int val);
	int pointGet(Point p);

	// Parameters
	std::deque<std::deque<Point>> caves;
	std::deque<Point> corridors;

	// Temp Functions
	bool compareDeques(std::deque<Point> deq1, std::deque<Point> deq2);
};