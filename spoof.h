#pragma once
#include "Mapper/linker.h"

void loadSpoof() {
	std::cout << _xor_("\n\n   Starting to spoof..").c_str();
	int res1 = startMapper(2);
	if (res1 != 1) {
		std::cout << _xor_("\n\n   Failed to load spoofer, please restart PC or contact a staff! Error #001").c_str();
		Sleep(3000);
		exit(0);
	}
	Sleep(1000);
	system(_xor_("cls").c_str());
}

void beginSpoof() {

	string userInput;
	int optionSelected = 1;
	while (optionSelected > 0) {
		system(_xor_("cls").c_str());
		cout << _xor_("\n\n   Do you want to load Target Spoofer? (Y/N)\n   ");
		getline(std::cin, userInput);
		if (userInput == "Y" || userInput == "y") {
			loadSpoof();
		}
		system(_xor_("cls").c_str());
		optionSelected = 0;
	}
}