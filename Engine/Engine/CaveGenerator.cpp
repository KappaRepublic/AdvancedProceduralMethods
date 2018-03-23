#include "CaveGenerator.h"

CaveGenerator::CaveGenerator()
{
	neighbours = 4;
	iterations = 50000;
	closeCellProb = 45;

	lowerLimit = 16;
	upperLimit = 500;

	mapSize = intVec2(100, 100);

	emptyNeighbours = 3;
	emptyCellNeighbours = 4;

	corridorSpace = 2;
	corridorMaxTurns = 10;
	corridorMin = 2;
	corridorMax = 5;

	breakOut = 100000;

	// Fill in map
	for (int i = 0; i < 105; i++) {
		map.push_back(std::vector<int>());
	}

	for (int i = 0; i < 105; i++) {
		for (int j = 0; j < 105; j++) {
			map[i].push_back(0);
		}
	}
}

CaveGenerator::CaveGenerator(const CaveGenerator & cGen)
{
}

CaveGenerator::~CaveGenerator()
{
}

void CaveGenerator::debug()
{
	std::list<Point> aTest;

	aTest = neighboursGet(Point(23, 20));
	aTest = neighboursGet1(Point(23, 20));

	return;
}

int CaveGenerator::build()
{
	buildCaves();
	getCaves();

	

	std::vector<std::vector<int> > test = map;

	return caves.size();
}

bool CaveGenerator::connectCaves()
{
	if (caves.size() == 0) {
		return false;
	}

	std::deque<Point> currentCave;
	std::deque<std::deque<Point>> connectedCaves; // = std::deque<std::deque<Point>>();
	Point corPoint = Point();
	Point corDirection = Point();
	std::list<Point> potentialCorridor; // = std::list<Point>();
	int breakOutCtr = 0;

	// corridors = std::deque<Point>(); // Built corridors

	// Randomly select a cave
	int random = rand() % caves.size();
	currentCave = caves[random];
	connectedCaves.push_back(currentCave);
	caves.erase(caves.begin() + random - 1);

	// Starting builder
	do {
		if (corridors.size() == 0) {
			currentCave = connectedCaves[rand() % connectedCaves.size()];
			caveGetEdge(currentCave, corPoint, corDirection);
		}
		else {
			// Corridors are present
			// Choose to get a start point from corridor or cave
			if ((rand() % 100 + 1) > 50) {
				currentCave = connectedCaves[rand() % connectedCaves.size()];
				caveGetEdge(currentCave, corPoint, corDirection);
			}
			else {
				// currentCave = std::deque<Point>();
				corridorGetEdge(corPoint, corDirection);
			}

			// Using the determined points, build a corridor
			potentialCorridor = corridorAttempt(corPoint, corDirection, true);


			if (potentialCorridor.size() > 0) {
				// Examine all the caves
				for (int ctr; ctr < caves.size(); ctr++) {
					// Check if the last point in the corridor list is a cave
					bool contains = false;
					for (Point &p : caves[ctr]) {
						if (p == potentialCorridor.back()) {
							contains = true;
						}
					}

					
					if (contains) {

						

						if (currentCave.empty() || !compareDeques(currentCave, caves[ctr])) {
							// The last corridor point intrudes on the room, so remove it
							potentialCorridor.pop_back();
							// Add the corridor to the corridor collection
							for (Point &p : potentialCorridor) {
								corridors.push_back(p);
							}
							// Write it to the map
							for (Point &p : potentialCorridor) {
								pointSet(p, 1);
							}

							// The room reached is added to the connected list
							connectedCaves.push_back(caves[ctr]);
							// And removed from the caves list
							caves.erase(caves.begin() + ctr - 1);


							break;

						}
					}	
				}
			}

			// break out
			if (breakOutCtr > breakOut) {
				return false;
			}
		}

	} while (caves.size() > 0);

	for (std::deque<Point> &pList : connectedCaves) {
		caves.push_back(pList);
	}

	connectedCaves.clear();
	return true;
}

void CaveGenerator::buildCaves()
{
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			if (((rand() % 100) + 1) < closeCellProb) {
				map.at(x).at(y) = 1;
			}
		}
	}

	Point cell;

	// Pick cells at random
	for (int x = 0; x <= iterations; x++) {
		cell = Point(((rand() % mapSize.x) + 1), (rand() % mapSize.y) + 1);

		// If the randomly selected cell has more closed neighbours than the property neighbours
		// set it to closed, else set it to open

		int cellCount = 0;

		for (Point &p : neighboursGet1(cell)) {
			if (pointGet(p) == 1) {
				cellCount += 1;
			}
		}

		if (cellCount >= neighbours) {
			pointSet(cell, 1);
		}
		else {
			pointSet(cell, 0);
		}
	}

	// Smooth out the rough edges of the cave by making several passes on the map
	// and removing any cells with 3 or more empty neighbours

	for (int ctr = 0; ctr < 5; ctr++) {
		// Examine each cell individually
		for (int x = 0; x < mapSize.x; x++) {
			for (int y = 0; y < mapSize.y; y++) {
				cell = Point(x + 1, y + 1);

				int cellCount = 0;

				for (Point &p : neighboursGet(cell)) {
					if (pointGet(p) == 0) {
						cellCount += 1;
					}
				}

				if (pointGet(cell) > 0 && cellCount >= emptyNeighbours) {
					pointSet(cell, 0);
				}
			}
		}
	}

	// Fill in any empty cells that have 4 full neighbours
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			cell = Point(x + 1, y + 1);

			int cellCount = 0;

			for (Point &p : neighboursGet(cell)) {
				if (pointGet(p) == 1) {
					cellCount += 1;
				}
			}

			if (pointGet(cell) == 0 && cellCount >= emptyCellNeighbours) {
				pointSet(cell, 1);
			}
		}
	}

}

void CaveGenerator::caveGetEdge(std::deque<Point> pCave, Point & pCavePoint, Point & pDirection)
{
	do {
		// Random point within the cave
		pCavePoint = pCave.at(rand() % pCave.size());
		pDirection = directionGet(pDirection);

		do {
			pCavePoint.offset(pDirection);
			if (!pointCheck(pCavePoint)) {
				break;
			}
			else if (pointGet(pCavePoint) == 0) {
				return;
			}
		} while (true);


	} while (true);
}

void CaveGenerator::getCaves()
{
	// caves = std::deque<std::deque<Point>>();

	std::deque<Point> cave;
	Point cell;

	// examine each cell in the map
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			cell = Point(x, y);
			// If the cell is closed, and the cell doesn't occur in the list of caves

			// Check if the cell doesn't occur within the list of caves
			// for ()

			int testInt = 0;
			for (std::deque<Point> &pList : caves) {
				for (Point &p : pList) {
					if (cell == p) {
						testInt++;
					}
				}
			}

			if (pointGet(cell) > 0 && testInt == 0) {
				// cave = std::deque<Point>();

				// Launch the recursive
				locateCave(cell, cave);

				// Check that cave falls within the specified ranges
				if (cave.size() <= lowerLimit || cave.size() > upperLimit) {
					// Falls outside, destroy
					for (Point &p : cave) {
						pointSet(p, 0);
					}
				}
				else {
					caves.push_back(cave);
				}
			}

			// ????????????????????????????????????????????????????????
			// ????????????????????????????????????????????????????????
			// ????????????????????????????????????????????????????????
			// ????????????????????????????????????????????????????????
		}
	}
}

void CaveGenerator::locateCave(Point pCell, std::deque<Point> &pCave)
{
	std::list<Point> tempList;

	for (Point &p : neighboursGet(pCell)) {
		if (pointGet(p) > 0) {
			tempList.push_back(p);
		}
	}

	for (Point &p : tempList) {
		for (Point &q : pCave) {
			if (q == p) {
				pCave.push_back(p);
				locateCave(p, pCave);
			}
		}
	}
}

void CaveGenerator::corridorGetEdge(Point & pLocation, Point & pDirection)
{
	std::deque<Point> validDirections; // = std::deque<Point>();

	do {
		// The modfiers below prevent the first or last point being chosen
		pLocation = corridors[rand() % (corridors.size() - 1) + 1];

		// Attempt to locale all the empty map points around the location
		// using the directions to offset the randomly chosen point
		for (Point &p : directions) {
			if (pointCheck(Point(pLocation.xCoord + p.xCoord, pLocation.yCoord + p.yCoord))) {
				if (pointGet(Point(pLocation.xCoord + p.xCoord, pLocation.yCoord + pLocation.yCoord)) == 0) {
					validDirections.push_back(p);
				}
			}
		}


	} while (validDirections.size() == 0);

	pDirection = validDirections[rand() % validDirections.size()];
	pLocation.offset(pDirection);
}

std::list<Point> CaveGenerator::corridorAttempt(Point pStart, Point pDirection, bool pPreventBackTracking)
{
	std::list<Point> lPotentialCorridor; // = std::list<Point>();
	lPotentialCorridor.push_back(pStart);

	int corridorLength;
	Point startDirection = Point(pDirection.xCoord, pDirection.yCoord);

	int pTurns = corridorMaxTurns;

	while (pTurns >= 0) {
		pTurns--;

		corridorLength = rand() % (corridorMax - corridorMin + 1) + corridorMin;

		// Build corridor
		while (corridorLength > 0) {
			corridorLength--;

			// Make a point and offset it
			pStart.offset(pDirection);

			if (pointCheck(pStart) && pointGet(pStart) == 1) {
				lPotentialCorridor.push_back(pStart);
				return lPotentialCorridor;
			}

			if (!pointCheck(pStart)) {
				return std::list<Point>();
			} else if (!corridorPointTest(pStart, pDirection)) {
				return std::list<Point>();
			}

			lPotentialCorridor.push_back(pStart);
		}

		if (pTurns > 1) {
			if (!pPreventBackTracking) {
				pDirection = directionGet(pDirection);
			}
			else {
				pDirection = directionGet(pDirection, startDirection);
			}
		}
	}
	return  std::list<Point>();
}

bool CaveGenerator::corridorPointTest(Point pPoint, Point pDirection)
{
	// Using corridorSpace, cheeck that number of cells on
	// either side of the point are empty

	for (int r = -corridorSpace; r < 2 * corridorSpace + 1; r++) {
		if (pDirection.xCoord == 0) {		// North or South
			if (pointCheck(Point(pPoint.xCoord + r, pPoint.yCoord))) {
				if (pointGet(Point(pPoint.xCoord + r, pPoint.yCoord)) != 0) {
					return false;
				}
			}
		}
		else if (pDirection.yCoord == 0) {	//East or West
			if (pointCheck(Point(pPoint.xCoord, pPoint.yCoord + r))) {
				if (pointGet(Point(pPoint.xCoord, pPoint.yCoord + r)) != 0) {
					return false;
				}
			}
		}
	}

	return true;
}

std::list<Point> CaveGenerator::neighboursGet(Point p)
{
	std::list<Point> tempList;

	for (Point const& i : directions) {
		if (i.xCoord == -1 || i.yCoord == -1) {

		}
		else {
			tempList.push_back(p + i);
		}
		
	}

	return tempList;
}

std::list<Point> CaveGenerator::neighboursGet1(Point p)
{
	std::list<Point> tempList;

	for (Point const& i : directions1) {
		if (i.xCoord == -1 || i.yCoord == -1) {

		}
		else {
			tempList.push_back(p + i);
		}
	}

	return tempList;
}

Point CaveGenerator::directionGet(Point p)
{
	Point newDir;
	do {
		newDir = directions.at(rand() % directions.size());
	} while (newDir.xCoord != -p.xCoord && newDir.yCoord != -p.yCoord);

	return Point();
}

Point CaveGenerator::directionGet(Point pDir, Point pDirExclude)
{
	Point newDir;
	do {
		newDir = directions.at(rand() % directions.size());
	} while (directionReverse(newDir).equals(pDir) | directionReverse(newDir).equals(pDirExclude));

	return Point();
}

Point CaveGenerator::directionReverse(Point pDir)
{
	return Point(-pDir.xCoord, -pDir.yCoord);
}

bool CaveGenerator::pointCheck(Point p)
{
	return p.xCoord >= 0 & p.xCoord < mapSize.x & p.yCoord >= 0 & p.yCoord < mapSize.y;;
}

void CaveGenerator::pointSet(Point p, int val)
{
	map[p.xCoord][p.yCoord] = val;
}

int CaveGenerator::pointGet(Point p)
{
	return map[p.xCoord][p.yCoord];
}

bool CaveGenerator::compareDeques(std::deque<Point> deq1, std::deque<Point> deq2)
{
	if (deq1.size() != deq2.size()) {
		return false;
	}

	for (int i = 0; i < deq1.size(); i++) {
		if (deq1[i] != deq2[i]) {
			return false;
		}
	}

	return true;
}
