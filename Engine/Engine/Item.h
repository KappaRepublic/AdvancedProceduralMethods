#pragma once

#include <vector>

class Item {
public:
	Item();
	Item(const Item&);
	~Item();

	void initialize();

	std::vector<char*> prefixes;
	std::vector<char*> objects;
	std::vector<char*> suffixes;

	char *itemPrefix, *itemName, *itemSuffix;
	int rarity;
private:
};