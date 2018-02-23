#pragma once

#include <vector>

class PerlinNoise {
	// Permutation vector
	std::vector<int> p;
public:
	// Initialise with the reference values for the permutation vector
	PerlinNoise();
	// Generate a new permutation vector based on the value of the seed
	PerlinNoise(unsigned int seed);
	// Get a noise value, for 2D, z can have any value
	double noise(double x, double y, double z);
private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};

