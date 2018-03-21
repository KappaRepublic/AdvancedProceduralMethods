#pragma once

#include <vector>
#include <string>

class Item {
public:
	Item();
	Item(const Item&);
	~Item();

	void initialize();

	std::vector<char*> prefixes;
	std::vector<char*> objects;
	std::vector<char*> suffixes;

	std::string fullItemName;
	std::string obtainedMessage;

	char *itemPrefix, *itemName, *itemSuffix;
	int rarity;
	int power;
private:
};