#include "Expense.h"

Expense::Expense()
{

}

void Expense::editExpense()
{
	int option;
	char repeat;

	do
	{
		system("cls");
		repeat = 'a';

		cout << "Please choose a option to edit monthly expense information\n";
		cout << "1) Add a monthly expense information\n";
		cout << "2) Update a monthly expense information\n";
		cout << "3) Delete a monthly expense information\n";
		cout << "4) Exit to previous menu\n";
		cout << "Please enter number 1 to 4 to proceed: ";
		cin >> option;

		while (inputValidation(1, 4, option))
		{
			cin >> option;
		}

		switch (option)
		{
		case 1:
			addExpense();
			break;
		case 2:
			displayExpense();
			updateExpense();
			break;
		case 3:
			displayExpense();
			deleteExpense();
			break;
		case 4:
			return;
		}

		while (tolower(repeat) != 'y' && tolower(repeat) != 'n')
		{
			cout << "\n\n***Please enter Y to edit another monthly expense information, enter N to exit to previous menu: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}
	} while (tolower(repeat) == 'y');

	
}

void Expense::addExpense()
{
	DatabaseConnection dc;
	char save;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	do
	{
		save = 'a';

		cout << "\n\nExpense name (-1 to cancel): ";
		getline(cin, name);

		if (name == "-1")
		{
			return;
		}

		cout << "Amount: RM";
		cin >> amount;
		cin.ignore();

		while (inputValidation(amount))
		{
			cout << "\n\nAmount: RM";
			cin >> amount;
			cin.ignore();
		}

		while (tolower(save) != 'y' && tolower(save) != 'n')
		{
			cout << "\n\nAre the information correct? (Y/N): ";
			cin >> save;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}

	} while (tolower(save) == 'n');

	try
	{
		dc.pstmt = dc.prepStmt("INSERT INTO MONTHLY_EXPENSE (Expense_name,`Amount(RM)`,Bookshop_ID) VALUES (?,?,?)");
		dc.pstmt->setString(1, name);
		dc.pstmt->setDouble(2, amount);
		dc.pstmt->setInt(3, bookshopId);

		dc.pstmt->execute();
	}
	catch (SQLException& e)
	{
		/* what() (derived from std::runtime_error) fetches error message */
		cout << "\n# ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	catch (...)
	{
		cout << "\n\nError!! Something unexpected occur!!\n";
	}

	cout << "\nThe data of monthly expense is successfully save into the database!!\n";
}

void Expense::deleteExpense()
{
	DatabaseConnection dc;

	int expenseId;

	cout << "\n\nPlease enter the expense id of monthly expense that want to delete (-1 to cancel): ";
	cin >> expenseId;

	if (expenseId == -1)
	{
		return;
	}

	while (inputValidation(expenseId))
	{
		cout << "\n\nPlease enter the expense id of monthly expense that want to delete (-1 to cancel): ";
		cin >> expenseId;

		if (expenseId == -1)
		{
			return;
		}
	}

	try
	{
		dc.pstmt = dc.prepStmt("DELETE FROM MONTHLY_EXPENSE WHERE Expense_ID=?");
		dc.pstmt->setInt(1, expenseId);

		if (dc.pstmt->executeUpdate())
			cout << "\nThe monthly expense with expense id " << expenseId << " is successfully deleted" << endl;
		else
			cout << "\nThe query does not run correctly";
	}
	catch (SQLException& e)
	{
		/* what() (derived from std::runtime_error) fetches error message */
		cout << "\n\n# ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	catch (...)
	{
		cout << "\n\nError!! Something unexpected occur!!\n";
	}
}

void Expense::updateExpense()
{
	DatabaseConnection dc;

	int option;
	int expenseId;

	cout << "\n\nPlease enter the expense id of monthly expense that want to update (-1 to cancel): ";
	cin >> expenseId;

	if (expenseId == -1)
	{
		return;
	}

	while (inputValidation(expenseId) || checkExpenseId(expenseId) == false)
	{
		cout << "\n\nPlease enter the expense id of monthly expense that want to update (-1 to cancel): ";
		cin >> expenseId;

		if (expenseId == -1)
		{
			return;
		}
	}

	cout << "\nPlease choose the information that want to update" << endl;
	cout << "1) Monthly expense name" << endl;
	cout << "2) Amount(RM)" << endl;
	cout << "Option: ";
	cin >> option;
	cin.ignore();

	while (inputValidation(1, 3, option))
	{
		cin >> option;
		cin.ignore();
	}

	try
	{
		switch (option)
		{
		case 1:
			cout << "\nPlease enter new monthly expense name: ";
			getline(cin, name);

			dc.pstmt = dc.prepStmt("UPDATE MONTHLY_EXPENSE SET Expense_Name = ? WHERE Expense_ID =?");
			dc.pstmt->setString(1, name);
			dc.pstmt->setInt(2, expenseId);
			break;

		case 2:
			cout << "\nPlease enter new amount: RM";
			cin >> amount;

			dc.pstmt = dc.prepStmt("UPDATE MONTHLY_EXPENSE SET `Amount(RM)` = ? WHERE Expense_ID =?");
			dc.pstmt->setDouble(1, amount);
			dc.pstmt->setInt(2, expenseId);
			break;
		}

		dc.pstmt->executeUpdate();

		cout << "\n\nThe information of monthly expense is updated\n";
	}
	catch (SQLException& e)
	{
		/* what() (derived from std::runtime_error) fetches error message */
		cout << "\n\n# ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	catch (...)
	{
		cout << "\n\nError!! Something unexpected occur!!\n";
	}
}

void Expense::setBookshopId(int id)
{
	bookshopId = id;
}

bool Expense::inputValidation(int input)
{
	if (cin.fail() || input < 0)
	{
		cin.clear();
		cin.ignore(numeric_limits <streamsize>::max(), '\n');
		cout << "\nI'm sorry but the valid choice is only positive numeric number. Please try again" << endl;

		return true;
	}

	return false;
}

bool Expense::inputValidation(int lower, int upper, int input)
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

bool Expense::checkExpenseId(int id)
{
	DatabaseConnection dc;

	dc.pstmt = dc.prepStmt("SELECT Expense_ID FROM MONTHLY_EXPENSE WHERE Expense_ID=?");
	dc.pstmt->setInt(1, id);
	dc.res = dc.pstmt->executeQuery();

	if (dc.res->next())
	{
		return true;
	}
	else
	{
		cout << "\n\nError!! There is not exist monthly expense with expense id " << id << endl;
		return false;
	}
}

void Expense::searchExpense()
{
	int option;
	char repeat;

	do
	{
		system("cls");
		repeat = 'a';

		cout << "Search monthly expense by" << endl;
		cout << "1)Expense id" << endl;
		cout << "2)Expense name" << endl;
		cout << "3)Exit to previous menu\n";
		cout << "Option: ";
		cin >> option;

		while (inputValidation(1, 3, option))
		{
			cin >> option;
		}

		if (option == 1)
		{
			int inputId;
			vector<int> id;

			cout << "\nExpense id (-1 to cancel): ";
			cin >> inputId;

			if (inputId == -1)
			{
				repeat = 'y';
				continue;
			}

			while (inputValidation(inputId))
			{
				cout << "\nExpense id (-1 to cancel): ";
				cin >> inputId;

				if (inputId == -1)
				{
					repeat = 'y';
					break;
				}
			}

			id.push_back(inputId);

			displayExpense(id);
		}
		else if (option == 2)
		{
			string name;

			cout << "\nExpense name (-1 to cancel): ";
			cin.ignore();
			getline(cin, name);

			if (name == "-1")
			{
				repeat = 'y';
				continue;
			}

			searchExpense(name);
		}
		else if (option == 3)
		{
			return;
		}

		while (tolower(repeat) != 'y' && tolower(repeat) != 'n')
		{
			cout << "\n\n***Please enter Y to search another monthly expense information, enter N to exit to previous menu: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}
	} while (tolower(repeat) == 'y');
}

void Expense::searchExpense(string name)
{
	DatabaseConnection dc;
	int inputId{};
	vector<int> id{};

	try
	{
		dc.pstmt = dc.prepStmt("SELECT Expense_ID FROM MONTHLY_EXPENSE WHERE Expense_Name LIKE ?");
		dc.pstmt->setString(1, "%" + name + "%");
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
		{
			inputId = dc.res->getInt("Expense_ID");
			id.push_back(inputId);
		}
	}
	catch (SQLException& e)
	{
		/* what() (derived from std::runtime_error) fetches error message */
		cout << "\n\n# ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	catch (...)
	{
		cout << "\n\nError!! Something unexpected occur!!\n";
	}

	displayExpense(id);
}

void Expense::displayExpense()
{
	DatabaseConnection dc;
	int i = 1;

	try
	{
		dc.pstmt = dc.prepStmt("SELECT * FROM MONTHLY_EXPENSE");
		dc.res = dc.pstmt->executeQuery();

		if (dc.res->next() == true)
		{
			cout << left << setw(12) << "\n   Expense_ID" << setw(30) << "\tExpense_Name" << "Amount(RM)";
			cout << endl;

			do
			{
				expenseId = dc.res->getInt("Expense_ID");
				name = dc.res->getString("Expense_Name");
				amount = dc.res->getDouble("Amount(RM)");

				cout << right << setw(2) << i << ") " << left << setw(13) << expenseId;
				cout << setw(30) << name;
				cout << setw(10) << amount;
				cout << endl;
				++i;

			} while (dc.res->next());
		}
		else
		{
			cout << "\n\nError!!!Something unexpected occur\n";
		}
	}
	catch (SQLException& e)
	{
		/* what() (derived from std::runtime_error) fetches error message */
		cout << "\n\n# ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	catch (...)
	{
		cout << "\n\nError!! Something unexpected occur!!\n";
	}
}

void Expense::displayExpense(vector<int> id)
{
	DatabaseConnection dc;
	int i = 1;
	bool titlePrint{ true };

	if (id.empty())
	{
		cout << "\n\nThe monthly expense is not found in the database\n";
	}

	try
	{
		for (int expId : id)
		{
			dc.pstmt = dc.prepStmt("SELECT * FROM MONTHLY_EXPENSE WHERE Expense_ID=?");
			dc.pstmt->setInt(1, expId);
			dc.res = dc.pstmt->executeQuery();

			if (dc.res->next() == true)
			{
				if (titlePrint)
				{
					cout << left << setw(12) << "\n   Expense_ID" << setw(30) << "\tExpense_Name" << "Amount(RM)";
					cout << endl;
					titlePrint = false;
				}

				do
				{
					expenseId = dc.res->getInt("Expense_ID");
					name = dc.res->getString("Expense_Name");
					amount = dc.res->getDouble("Amount(RM)");

					cout << right << setw(2) << i << ") " << left << setw(13) << expenseId;
					cout << setw(30) << name;
					cout << setw(10) << amount;
					cout << endl;
					++i;

				} while (dc.res->next());
			}
			else
			{
				cout << "\n\nThe monthly expense is not found in the database\n";
			}
		}
	}
	catch (SQLException& e)
	{
		/* what() (derived from std::runtime_error) fetches error message */
		cout << "\n\n# ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	catch (...)
	{
		cout << "\n\nError!! Something unexpected occur!!\n";
	}
}
