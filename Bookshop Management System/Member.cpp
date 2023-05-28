#include "Member.h"

Member::Member()
{

}

void Member::editMember()
{
	int option;
	char repeat;

	do
	{
		system("cls");
		repeat = 'a';

		cout << "Please choose a option to edit member information\n";
		cout << "1) Add a member information\n";
		cout << "2) Update a member information\n";
		cout << "3) Delete a member information\n";
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
			addMember();
			break;
		case 2:
			displayMember();
			updateMember();
			break;
		case 3:
			displayMember();
			deleteMember();
			break;
		case 4:
			return;
		}

		while (tolower(repeat) != 'y' && tolower(repeat) != 'n')
		{
			cout << "\n\n***Please enter Y to edit another member information, enter N to exit to previous menu: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}
	} while (tolower(repeat) == 'y');

	
}

void Member::addMember()
{
	DatabaseConnection dc;
	char save;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	do
	{
		save = 'a';

		cout << "\n\nMember name (-1 to cancel): ";
		getline(cin, name);

		if (name == "-1")
		{
			return;
		}

		cout << "Member contact number: ";
		getline(cin, phone);

		cout << "Member address: ";
		getline(cin, address);

		cout << "Member email: ";
		getline(cin, email);

		while (emailValidation(email) == false)
		{
			cout << "Member email: ";
			getline(cin, email);
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
		dc.pstmt = dc.prepStmt("INSERT INTO MEMBER (Member_name,Member_Phone,Member_Address,Member_Email,Bookshop_ID) VALUES (?,?,?,?,?)");
		dc.pstmt->setString(1, name);
		dc.pstmt->setString(2, phone);
		dc.pstmt->setString(3, address);
		dc.pstmt->setString(4, email);
		dc.pstmt->setInt(5, bookshopId);

		dc.pstmt->execute();
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

	cout << "\nYour data is successfully save into the database!!\n";
}

void Member::deleteMember()
{
	DatabaseConnection dc;

	int memberId;

	cout << "\nPlease enter the member id of member that want to delete (-1 to cancel): ";
	cin >> memberId;

	if (memberId == -1)
	{
		return;
	}

	while (inputValidation(memberId))
	{
		cout << "\nPlease enter the member id of member that want to delete (-1 to cancel): ";
		cin >> memberId;

		if (memberId == -1)
		{
			return;
		}
	}

	try
	{
		dc.pstmt = dc.prepStmt("UPDATE CUSTOMER SET Member_ID=NULL WHERE Member_ID=?");
		dc.pstmt->setInt(1, memberId);
		dc.pstmt->executeUpdate();

		dc.pstmt = dc.prepStmt("DELETE FROM MEMBER WHERE Member_ID=?");
		dc.pstmt->setInt(1, memberId);

		if (dc.pstmt->executeUpdate())
			cout << "\nThe member with member id " << memberId << " is successfully deleted" << endl;
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


void Member::updateMember()
{
	DatabaseConnection dc;

	int option;
	int memberId;

	cout << "\n\nPlease enter the member id of member that want to update (-1 to cancel): ";
	cin >> memberId;

	if (memberId == -1)
	{
		return;
	}

	while (inputValidation(memberId) || checkMemberId(memberId) == false)
	{
		cout << "\n\nPlease enter the member id of member that want to update (-1 to cancel): ";
		cin >> memberId;

		if (memberId == -1)
		{
			return;
		}
	}

	cout << "\nPlease choose the information that want to update" << endl;
	cout << "1) Member name" << endl;
	cout << "2) Member contact number" << endl;
	cout << "3) Member address" << endl;
	cout << "4) Member email" << endl;
	cout << "Option: ";
	cin >> option;
	cin.ignore();

	while (inputValidation(1, 4, option))
	{
		cin >> option;
		cin.ignore();
	}

	try
	{
		switch (option)
		{
		case 1:
			cout << "\nPlease enter new member name: ";
			getline(cin, name);

			dc.pstmt = dc.prepStmt("UPDATE MEMBER SET Member_Name = ? WHERE Member_ID =?");
			dc.pstmt->setString(1, name);
			dc.pstmt->setInt(2, memberId);
			break;

		case 2:
			cout << "\nPlease enter new member contact number: ";
			getline(cin, phone);

			dc.pstmt = dc.prepStmt("UPDATE MEMBER SET Member_Phone = ? WHERE Member_ID =?");
			dc.pstmt->setString(1, phone);
			dc.pstmt->setInt(2, memberId);
			break;

		case 3:
			cout << "\nPlease enter new member address: ";
			getline(cin, address);

			dc.pstmt = dc.prepStmt("UPDATE MEMBER SET Member_Address = ? WHERE Member_ID =?");
			dc.pstmt->setString(1, address);
			dc.pstmt->setInt(2, memberId);
			break;

		case 4:
			cout << "\nPlease enter new member email: ";
			getline(cin, email);

			while (emailValidation(email) == false)
			{
				cout << "Please enter new member email: ";
				getline(cin, email);
			}

			dc.pstmt = dc.prepStmt("UPDATE MEMBER SET Member_Email = ? WHERE Member_ID =?");
			dc.pstmt->setString(1, email);
			dc.pstmt->setInt(2, memberId);
			break;
		}

		dc.pstmt->executeUpdate();

		cout << "\n\nThe information of member is updated\n";
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

void Member::setBookshopId(int id)
{
	bookshopId = id;
}

bool Member::inputValidation(int input)
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

bool Member::inputValidation(int lower, int upper, int input)
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

// function to check email is valid or not
bool Member::emailValidation(string email)
{
	// check the first character is alphabet or not
	if (!isalpha(email[0]))
	{
		cout << "\nEmail Invalid!! Email must start with an alphabet!!\n\n";
		return false;
	}

	// variable to store the position of @ and .
	int at = -1;
	int dot = -1;

	// traverse the email to locate @ and .
	for (int i{ 0 }; i < email.length(); ++i)
	{
		if (email[i] == '@')
		{
			at = i;
		}
		else if (email[i] == '.')
		{
			dot = i;
		}
	}

	// if @ and . is not presence return false(not valid)
	if (at == -1 || dot == -1)
	{
		cout << "\nEmail Invalid!! Email must include @ and .!!\n\n";
		return false;
	}
	else if (at > dot) // if @ is located after . return false(not valid)
	{
		cout << "\nEmail Invalid!! The @ character come before .!!\n\n";
		return false;
	}
	else if (dot >= (email.length() - 1)) // check if . present at the last character
	{
		cout << "\nEmail Invalid!!\n\n";
		return false;
	}

	return true;
}

bool Member::checkMemberId(int id)
{
	DatabaseConnection dc;

	dc.pstmt = dc.prepStmt("SELECT Member_ID FROM MEMBER WHERE Member_ID=?");
	dc.pstmt->setInt(1, id);
	dc.res = dc.pstmt->executeQuery();

	if (dc.res->next())
	{
		return true;
	}
	else
	{
		cout << "\n\nError!! There is not exist member with member id " << id << endl;
		return false;
	}
}

void Member::searchMember()
{
	int option;
	char repeat;

	do
	{
		system("cls");
		repeat = 'a';

		cout << "Search member by" << endl;
		cout << "1)Member id" << endl;
		cout << "2)Member name" << endl;
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

			cout << "\nMember id (-1 to cancel): ";
			cin >> inputId;

			if (inputId == -1)
			{
				repeat = 'y';
				continue;
			}

			while (inputValidation(inputId))
			{
				cout << "\nMember id (-1 to cancel): ";
				cin >> inputId;

				if (inputId == -1)
				{
					repeat = 'y';
					break;
				}
			}

			id.push_back(inputId);

			displayMember(id);
		}
		else if (option == 2)
		{
			string name;

			cout << "Member name (-1 to cancel): ";
			cin.ignore();
			getline(cin, name);

			if (name == "-1")
			{
				repeat = 'y';
				continue;
			}

			searchMember(name);
		}
		else if (option == 3)
		{
			return;
		}

		while (tolower(repeat) != 'y' && tolower(repeat) != 'n')
		{
			cout << "\n\n***Please enter Y to search another member information, enter N to exit to previous menu: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}
	} while (tolower(repeat) == 'y');
}

void Member::searchMember(string member_name)
{
	DatabaseConnection dc;
	int inputId{};
	vector<int> id{};

	try
	{
		dc.pstmt = dc.prepStmt("SELECT Member_ID FROM MEMBER WHERE Member_Name LIKE ?");
		dc.pstmt->setString(1, "%" + member_name + "%");
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
		{
			inputId = dc.res->getInt("Member_ID");
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

	displayMember(id);
}

void Member::displayMember()
{
	DatabaseConnection dc;
	int i = 1;

	try
	{
		dc.pstmt = dc.prepStmt("SELECT * FROM MEMBER");
		dc.res = dc.pstmt->executeQuery();

		if (dc.res->next() == true)
		{
			cout << left << setw(12) << "\n   Member_ID" << setw(30) << "\tMember_Name" << setw(20) << "Member_Phone";
			cout << setw(45) << "Member_Address" << setw(30) << "Member_Email";
			cout << endl;

			do
			{
				memberId = dc.res->getInt("Member_ID");
				name = dc.res->getString("Member_Name");
				phone = dc.res->getString("Member_Phone");
				address = dc.res->getString("Member_Address");
				email = dc.res->getString("Member_Email");

				cout << right << setw(2) << i << ") " << left << setw(13) << memberId;
				cout << setw(30) << name;
				cout << setw(20) << phone;
				cout << setw(44) << address;
				cout << email;
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

void Member::displayMember(vector<int> id)
{
	DatabaseConnection dc;
	int i = 1;
	bool titlePrint{ true };

	if (id.empty())
	{
		cout << "\n\nThe member is not found in the database\n";
	}

	try
	{
		for (int memId : id)
		{
			dc.pstmt = dc.prepStmt("SELECT * FROM MEMBER WHERE Member_ID=?");
			dc.pstmt->setInt(1, memId);
			dc.res = dc.pstmt->executeQuery();

			if (dc.res->next() == true)
			{
				if (titlePrint)
				{
					cout << left << setw(12) << "\n   Member_ID" << setw(30) << "\tMember_Name" << setw(20) << "Member_Phone";
					cout << setw(45) << "Member_Address" << setw(30) << "Member_Email";
					cout << endl;
					titlePrint = false;
				}

				do
				{
					memberId = dc.res->getInt("Member_ID");
					name = dc.res->getString("Member_Name");
					phone = dc.res->getString("Member_Phone");
					address = dc.res->getString("Member_Address");
					email = dc.res->getString("Member_Email");

					cout << right << setw(2) << i << ") " << left << setw(13) << memberId;
					cout << setw(30) << name;
					cout << setw(20) << phone;
					cout << setw(44) << address;
					cout << email;
					cout << endl;
					++i;

				} while (dc.res->next());
			}
			else
			{
				cout << "\n\nThe member is not found in the database\n";
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
