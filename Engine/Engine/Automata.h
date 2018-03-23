#pragma once

#include <vector>
#include <time.h>

class Automata {
public:
	Automata();
	Automata(const Automata &automata);
	~Automata();

	void generateMap();
	void smoothMap();
	void randomFillMap();
	int getSurroundingWallCount(int mapX, int mapY);

	std::vector<std::vector<int>> getMap() {
		return map;
	};

	int width = 25;
	int height = 25;
	int randomFillPercent = 40;
	int iterations = 1;

	std::vector<std::vector<int> > map;
};
