#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <cstdlib>
using namespace std;
#include "Bookshop.h"

#ifndef BOOKSHOPUI_H
#define BOOKSHOPUI_H

class BookshopUI
{
private:
	bool logoutStatus{ false };
	Bookshop bookshop{};

	//these are declaration of methods used in this class
public:
	BookshopUI();
	void startProgram();			//start the system
	void customerMainMenu();		//display customer main menu
	void adminMainMenu();		//display admin main menu
	void adminUpdateDeleteModule();	//display information update and delete module
	void adminSearchModule();		//display information search module
	void adminReport();				//display report production module
	
	bool inputValidation(int lower, int upper, int input);	//check if the input from user is valid in some range
};

#endif
