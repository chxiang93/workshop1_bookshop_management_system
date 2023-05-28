#include "BookshopUI.h"

BookshopUI::BookshopUI()
{

}

void BookshopUI::startProgram()
{
	system("cls");
	cout << "=============================================================================================\n";
	cout << "## _______________________________________________________________________________________ ##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                              WELCOME TO                                               |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                          BOOKSHOP MANAGEMENT SYSTEM                                   |##\n";
	cout << "##|                                           		                                  |##\n";
	cout << "##|                          PLEASE LOGIN TO THE SYSTEM                                   |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                           1) CUSTOMER                                                 |##\n";
	cout << "##|                           2) ADMIN                                                    |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|_______________________________________________________________________________________|##\n";
	cout << "=============================================================================================\n";
	cout << "Please enter 1 if you are customer and 2 if you are employee: ";
	int choiceLogin;
	cin >> choiceLogin;

	//check the choicw
	while (choiceLogin < 1 || choiceLogin > 2 || cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits <streamsize>::max(), '\n');
		cout << "\nI'm sorry but the valid choice is only 1 or 2.Please try again" << endl;
		cout << "Please enter 1 if you are customer and 2 if you are employee: ";
		cin >> choiceLogin;
	}

	if (choiceLogin == 1)
	{
		//if choice is customer main menu
		//display customer main menu
		customerMainMenu();
	}
	else if (choiceLogin == 2)
	{
		//if choice is admin
		//display admin login menu

		system("cls");
		bool loginStatus = bookshop.adminLogin(logoutStatus);

		logoutStatus = false;

		if (loginStatus == true)
		{
			adminMainMenu();
		}
		else if (loginStatus == false)
		{
			system("pause");
			system("cls");
			startProgram();
		}
	}
}

// customer system main menu
void BookshopUI::customerMainMenu()
{
	char repeat{ 'y' };

	while (true)
	{
		repeat = 'a';
		system("cls");
		cout << "************************************************************************" << endl;
		cout << "                  BOOKSHOP MANAGEMENT SYSTEM" << endl;
		cout << "************************************************************************" << endl;
		cout << "Welcome to the bookshop management system" << endl;
		cout << "What do you want to do with this system?" << endl;
		cout << "1. Register as a member" << endl;
		cout << "2. Search book information" << endl;
		cout << "3. Logout from the system" << endl;
		cout << "4. Exit the system" << endl;
		cout << "************************************************************************" << endl;
		cout << "Please enter number 1 to 4 to proceed: ";
		int choiceCustMainMenu;
		cin >> choiceCustMainMenu;

		//check choice
		while (inputValidation(1, 4, choiceCustMainMenu))
		{
			cin >> choiceCustMainMenu;
		}

		//call different function based on the choice
		switch (choiceCustMainMenu)
		{
		case 1:
			bookshop.member.addMember();
			system("pause");
			break;
		case 2:
			system("cls");
			bookshop.book.searchBook();
			repeat = 'y';
			break;
		case 3:
			startProgram();
			break;
		case 4:
			exit(EXIT_SUCCESS);
		}

		/*while (tolower(repeat) != 'y' && tolower(repeat) != 'n')
		{
			cout << "\n\n***Please enter Y to return to main menu, enter N to exit the program: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}*/
	}
}

//admin main menu
void BookshopUI::adminMainMenu()
{
	system("cls");
	cout << "=============================================================================================\n";
	cout << "## _______________________________________________________________________________________ ##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                                           		                                  |##\n";
	cout << "##|                       PLEASE CHOOSE A SYSTEM MODULE TO RUN                            |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                         1) INFORMATION UPDATE AND DELETE                              |##\n";
	cout << "##|                         2) INFORMATION SEARCH                                         |##\n";
	cout << "##|                         3) REPORT PRODUCTION                                          |##\n";
	cout << "##|                         4) LOGOUT FROM THE SYSTEM                                     |##\n";
	cout << "##|                         5) Exit the system                                            |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|                                                                                       |##\n";
	cout << "##|_______________________________________________________________________________________|##\n";
	cout << "=============================================================================================\n";
	cout << "Please enter number 1-5 to proceed: ";
	int choiceAdminMainMenu;
	cin >> choiceAdminMainMenu;

	//check choice
	while (inputValidation(1, 5, choiceAdminMainMenu))
	{
		cin >> choiceAdminMainMenu;
	}

	//call different function based on the choice
	switch (choiceAdminMainMenu)
	{
	case 1:
		adminUpdateDeleteModule();
		break;
	case 2:
		adminSearchModule();
		break;
	case 3:
		adminReport();
		break;
	case 4:
		logoutStatus = true;
		startProgram();
		break;
	case 5:
		exit(EXIT_SUCCESS);
	}
}

//information update and delete module
void BookshopUI::adminUpdateDeleteModule()
{
	char repeat;

	while(true)
	{
		repeat = 'a';

		system("cls");
		cout << "************************************************************************" << endl;
		cout << "                  BOOKSHOP MANAGEMENT SYSTEM" << endl;
		cout << "************************************************************************" << endl;
		cout << "                  INFORMATION UPDATE AND DELETE MODULE" << endl;
		cout << "************************************************************************" << endl;
		cout << "Welcome to the bookshop management system" << endl;
		cout << "What do you want to do with this system?" << endl;
		cout << "1. Enter a new order detail" << endl;
		cout << "2. Update or delete employee information" << endl;
		cout << "3. Update or delete book information" << endl;
		cout << "4. Update or delete member information" << endl;
		cout << "5. Update or delete monthly expense information" << endl;
		cout << "6. Update admin username and password" << endl;
		cout << "7. Update member discount" << endl << endl;
		cout << "8. Go to Information Search Module" << endl;
		cout << "9. Go to Report Production Module" << endl;
		cout << "10. Exit to main menu" << endl;
		cout << "************************************************************************" << endl;
		cout << "Please enter number 1 to 10 to proceed: ";
		int choiceUpdateDeleteModule;
		cin >> choiceUpdateDeleteModule;

		//check choice
		while (inputValidation(1, 10, choiceUpdateDeleteModule))
		{
			cin >> choiceUpdateDeleteModule;
		}

		system("cls");
		//call different function based on the choice
		switch (choiceUpdateDeleteModule)
		{
		case 1:
			bookshop.order.addOrder();
			break;
		case 2:
			bookshop.emp.editEmployee();
			break;
		case 3:
			bookshop.book.editBook();
			break;
		case 4:
			bookshop.member.editMember();
			break;
		case 5:
			bookshop.expense.editExpense();
			break;
		case 6:
			bookshop.updateAdminUsernamePassword();
			break;
		case 7:
			bookshop.updateDiscount();
			system("pause");
			break;
		case 8:
			adminSearchModule();
			break;
		case 9:
			adminReport();
			break;
		case 10:
			adminMainMenu();
			break;
		}
	}
}

//information search module
void BookshopUI::adminSearchModule()
{
	while (true)
	{
		system("cls");
		cout << "************************************************************************" << endl;
		cout << "                  BOOKSHOP MANAGEMENT SYSTEM" << endl;
		cout << "************************************************************************" << endl;
		cout << "                  INFORMATION SEARCH MODULE" << endl;
		cout << "************************************************************************" << endl;
		cout << "Welcome to the bookshop management system" << endl;
		cout << "What do you want to do with this system?" << endl;
		cout << "1. Search a employee information" << endl;
		cout << "2. Search a book information" << endl;
		cout << "3. Search a member information" << endl;
		cout << "4. Search a order information" << endl;
		cout << "5. Search a monthly expense information" << endl << endl;;
		cout << "6. Go to Information Update And Delete Module" << endl;
		cout << "7. Go to Report Production Module" << endl;
		cout << "8. Exit to main menu" << endl;
		cout << "************************************************************************" << endl;
		cout << "Please enter number 1 to 8 to proceed: ";
		int choiceSearchModule;
		cin >> choiceSearchModule;

		//check choice
		while (inputValidation(1, 8, choiceSearchModule))
		{
			cin >> choiceSearchModule;
		}

		system("cls");
		//call different function based on the choice
		switch (choiceSearchModule)
		{
		case 1:
			bookshop.emp.searchEmployee();
			break;
		case 2:
			bookshop.book.searchBook();
			break;
		case 3:
			bookshop.member.searchMember();
			break;
		case 4:
			bookshop.order.searchOrder();
			break;
		case 5:
			bookshop.expense.searchExpense();
			break;
		case 6:
			adminUpdateDeleteModule();
			break;
		case 7:
			adminReport();
			break;
		case 8:
			adminMainMenu();
			break;
		}
	}
}

//report production module
void BookshopUI::adminReport()
{
	char repeat;

	do
	{
		repeat = 'a';

		system("cls");
		cout << "************************************************************************" << endl;
		cout << "                  BOOKSHOP MANAGEMENT SYSTEM" << endl;
		cout << "************************************************************************" << endl;
		cout << "                  REPORT PRODUCTION MODULE" << endl;
		cout << "************************************************************************" << endl;
		cout << "Welcome to the bookshop management system" << endl;
		cout << "What do you want to do with this system?" << endl;
		cout << "1. List out employee information" << endl;
		cout << "2. List out book information" << endl;
		cout << "3. List out member information" << endl;
		cout << "4. List out monthly expenses information" << endl;
		cout << "5. List out order information" << endl;
		cout << "6. List out monthly income of bookshop" << endl << endl;
		cout << "7. Go to Information Update And Delete Module" << endl;
		cout << "8. Go to Information Search Module" << endl;
		cout << "9. Exit to main menu" << endl;
		cout << "************************************************************************" << endl;
		cout << "Please enter number 1 to 9 to proceed: ";
		int choiceReportModule;
		cin >> choiceReportModule;

		//check choice
		while (inputValidation(1, 9, choiceReportModule))
		{
			cin >> choiceReportModule;
		}

		system("cls");
		//call different function based on the choice
		switch (choiceReportModule)
		{
		case 1:
			bookshop.emp.displayEmployee();
			break;
		case 2:
			bookshop.book.displayBook();
			break;
		case 3:
			bookshop.member.displayMember();
			break;
		case 4:
			bookshop.expense.displayExpense();
			break;
		case 5:
			bookshop.order.displayOrder();
			break;
		case 6:
			bookshop.displayMonthlyIncome();
			break;
		case 7:
			adminUpdateDeleteModule();
			break;
		case 8:
			adminSearchModule();
			break;
		case 9:
			adminMainMenu();
			break;
		}

		while (tolower(repeat) != 'y' && tolower(repeat) != 'n')
		{
			cout << "\n\n***Please enter Y to return to previous menu, enter N to exit to the main menu: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}

	} while (tolower(repeat) == 'y');

	adminMainMenu();
}

bool BookshopUI::inputValidation(int lower, int upper, int input)
{
	if (input < lower || input>upper || cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits <streamsize>::max(), '\n');
		cout << "\nI'm sorry but the valid choice is only ranges from " << lower << " to " << upper << ".Please try again" << endl;
		cout << "Please enter number " << lower << " to " << upper << " to proceed: ";

		return true;
	}

	return false;
}