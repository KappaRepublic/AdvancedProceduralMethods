#pragma once

#include "imgui.h"
#include "Item.h"
#include "Player.h"
#include <vector>
#include <list>


class MenuHandler {
public:
	MenuHandler();
	MenuHandler(const MenuHandler &menuHandler);
	~MenuHandler();

	void init();
	void shutDown();

	std::list<const char*> logWindowContents;

	// Functions to call the various menues within the game
	void callMainMenu(bool isVisible, bool &inventoryActive, bool &equipmentActive);
	void callInventory(bool isVisible, Player &player, std::vector<Item*> &inventory);
	void callEquipment(bool isVisible, Player &player);
	void callPlayerStatus(bool isVisible, Player& player);
	void callPlayerActions(bool isVisible, Player& player, vector<Object*>& objs, vector<Item*>& inv);
	void updateLogAddUpdate(const char* text);
	void callUpdateLog(bool isVisible);
	void clearUpdateLog();
private:
	bool menuActive = true;
};
