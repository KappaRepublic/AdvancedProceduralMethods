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
	void callPlayerActions(bool isVisible, Player& player, vector<Object*>& objs, vector<Item*>& inv);
private:
	bool menuActive = true;
};
