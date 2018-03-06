#include "Item.h"

Item::Item()
{
	prefixes = {
		"Seb's ",
		"Jack's ",
		"Rory's ",
		"Indomitable ",
		"Phantasmagoric ",
		"Procedural ",
		"A E S T H E T I C ",
		"NULL ",
		"Off-Brand ",
		"Discount ",
		"Prefixless ",
		"Just a ",
		""
	};

	objects = {
		"Sword",
		"Axe",
		"Hoe",
		"Shovel",
		"Bottle Cap",
		"Painting",
		"Glasses",
		"POINTER",
		"Rivers",
		"Monitor",
		"Juice",
		"Hat"
	};

	suffixes = {
		" of Generation",
		" of Mild Irritation",
		" of Dissapointment",
		" of A+ Grades Please",
		" of Pointiness",
		" of Rudeness",
		" OF 2147483647",
		" in the Desert",
		" with no Suffix",
		""
	};
}

Item::Item(const Item &)
{
}

Item::~Item()
{
}

void Item::initialize()
{
	itemPrefix = prefixes.at(rand() % prefixes.size());
	itemName = objects.at(rand() % objects.size());
	itemSuffix = suffixes.at(rand() % suffixes.size());
}
