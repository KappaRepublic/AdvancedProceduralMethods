#include "MenuHandler.h"

MenuHandler::MenuHandler()
{
	menuActive = true;
}

MenuHandler::MenuHandler(const MenuHandler & menuHandler)
{
}

MenuHandler::~MenuHandler()
{
}

void MenuHandler::callMainMenu(bool isVisible, bool &inventoryActive)
{
	if (isVisible) {
		// Menu System
		ImGui::Begin("Menu", &menuActive, ImVec2(0, 0), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
		if (ImGui::Button("Inventory", ImVec2(72, 72))) {
			inventoryActive = !inventoryActive;
		}
		ImGui::SameLine();
		ImGui::Button("Equipment", ImVec2(72, 72));
		ImGui::SameLine(); // :)
		if (ImGui::Button("Quit Game", ImVec2(72, 72))) {
			exit(-1);
		}
		ImGui::End();
	}
}

void MenuHandler::callInventory(bool isVisible, Player &player, std::vector<Item*> &inventory)
{
	if (isVisible) {
		ImGui::Begin("Inventory", &menuActive, ImVec2(0, 0), ImGuiWindowFlags_AlwaysAutoResize);
		for (int i = 0; i < inventory.size(); i++) {
			char test[150];

			// sprintf(test, inventory.at(i)->itemPrefix);

			strcpy_s(test, inventory.at(i)->itemPrefix);
			strcat_s(test, inventory.at(i)->itemName);
			strcat_s(test, inventory.at(i)->itemSuffix);
			// strcat_s(test, (char*)inventory.at(i)->power);

			if (ImGui::Selectable(test, player.equippedWeapon == i)) {
				player.equippedWeapon = i;
			}
			ImGui::SameLine(350); ImGui::Text("(+%d DMG)", inventory.at(i)->power);
		}
		ImGui::End(); // (:
	}
}

void MenuHandler::callPlayerStatus(bool isVisible, Player & player)
{
	if (isVisible) {
		ImGui::Begin("Player Status", &menuActive);
		char buff[32];
		sprintf(buff, "HP: %d/%d", (player.getHealthCur()), player.getHealthMax());

		float hpPercentage = (float)player.getHealthCur() / (float)player.getHealthMax();
		ImGui::ProgressBar(hpPercentage, ImVec2(-1.0f, 0.f), buff);
	}


	ImGui::End();
}
