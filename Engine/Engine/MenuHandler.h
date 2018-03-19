#pragma once

#include "imgui.h"
#include "Item.h"
#include "Player.h"
#include <vector>


class MenuHandler {
public:
	MenuHandler();
	MenuHandler(const MenuHandler &menuHandler);
	~MenuHandler();

	void init();
	void shutDown();

	// Functions to call the various menues within the game
	void callMainMenu(bool isVisible, bool &inventoryActive);
	void callInventory(bool isVisible, Player &player, std::vector<Item*> &inventory);
	void callPlayerStatus(bool isVisible, Player& player);
private:
	bool menuActive = true;
};
