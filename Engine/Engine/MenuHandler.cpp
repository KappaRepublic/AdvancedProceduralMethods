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

void MenuHandler::callMainMenu(bool isVisible, bool &inventoryActive, bool &equipmentActive)
{
	if (isVisible) {
		// Menu System
		ImGui::Begin("Menu", &menuActive, ImVec2(0, 0), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
		if (ImGui::Button("Inventory", ImVec2(72, 72))) {
			inventoryActive = !inventoryActive;
		}
		ImGui::SameLine();
		if (ImGui::Button("Equipment", ImVec2(72, 72))) {
			equipmentActive = !equipmentActive;
		}
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
		ImGui::End(); 
	}
}

void MenuHandler::callEquipment(bool isVisible, Player &player)
{
	if (isVisible) {
		ImGui::Begin("Equipment", &menuActive);
		// ImGui::Button()
		ImGui::Image(player.getMesh()->getTexture2(), ImVec2(256, 397));
		ImGui::End();
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

void MenuHandler::callPlayerActions(bool isVisible, Player & player, vector<Object*>& objs, vector<Item*>& inv, Object& crystal)
{
	if (isVisible) {
		ImGui::Begin("Actions", &menuActive);
		if (ImGui::Button("Attack", ImVec2(72, 72))) {
			updateLogAddUpdate(player.attack(objs));
		}
		ImGui::SameLine();
		if (ImGui::Button("Interact", ImVec2(72, 72))) {
			updateLogAddUpdate(player.interact(objs, inv, crystal));	
		}
		ImGui::End();
	}
}

void MenuHandler::updateLogAddUpdate(const char * text)
{
	logWindowContents.push_back(text);
}

void MenuHandler::callUpdateLog(bool isVisible)
{
	if (isVisible) {
		ImGui::Begin("Log", &menuActive);
		
		for (auto const& i : logWindowContents) {
			ImGui::Text(i);
		}
		if (ImGui::Button("Clear Log")) {
			clearUpdateLog();
		}
		ImGui::End();
	}
}

void MenuHandler::clearUpdateLog()
{
	logWindowContents.clear();
}
