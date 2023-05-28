#include "Employee.h"

Employee::Employee()
{
	
}

void Employee::editEmployee()
{
	int option;
	char repeat;

	do
	{
		system("cls");
		repeat = 'a';

		cout << "Please choose a option to edit employee information\n";
		cout << "1) Add a employee information\n";
		cout << "2) Update a employee information\n";
		cout << "3) Delete a employee information\n";
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
			addEmployee();
			break;
		case 2:
			displayEmployee();
			updateEmployee();
			break;
		case 3:
			displayEmployee();
			deleteEmployee();
			break;
		case 4:
			return;
		}

		while (tolower(repeat) != 'y' && tolower(repeat) != 'n')
		{
			cout << "\n\n***Please enter Y to edit another employee information, enter N to exit to previous menu: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}

	} while (tolower(repeat) == 'y');
	
}

void Employee::addEmployee()
{
	DatabaseConnection dc;
	char save;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	do
	{
		save = 'a';

		cout << "\n\nPlease enter employee name (-1 to cancel): ";
		getline(cin, name);

		if (name == "-1")
		{
			return;
		}

		cout << "Please enter employee address: ";
		getline(cin, address);

		cout << "Please enter employee contact numnber: ";
		getline(cin, contactNo);

		cout << "Please enter employee salary : RM";
		cin >> salary;

		while (inputValidation(salary))
		{
			cout << "\n\nPlease enter employee salary : RM";
			cin >> salary;
		}

		cout << "Please enter employee IC number: ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, ic);

		while (tolower(save) != 'y' && tolower(save) != 'n')
		{
			cout << "\n\nAre the information correct? (Y/N): ";
			cin >> save;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}

	} while (tolower(save) == 'n');

	try
	{
		dc.pstmt = dc.prepStmt("INSERT INTO EMPLOYEE (Employee_name,Employee_Address,Employee_Phone,`Employee_Salary(RM)`,Employee_IC,Bookshop_ID) VALUES (?,?,?,?,?,?)");
		dc.pstmt->setString(1, name);
		dc.pstmt->setString(2, address);
		dc.pstmt->setString(3, contactNo);
		dc.pstmt->setDouble(4, salary);
		dc.pstmt->setString(5, ic);
		dc.pstmt->setInt(6, bookshopId);
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

	cout << "\nThe data of employee is successfully save into the database!!\n";
}

void Employee::deleteEmployee()
{
	DatabaseConnection dc;
	int empID;

	cout << "\n\nPlease enter the employee id of employee that want to delete (-1 to cancel): ";
	cin >> empID;

	if (empID == -1)
	{
		return;
	}

	while (inputValidation(empID))
	{
		cout << "\n\nPlease enter the employee id of employee that want to delete (-1 to cancel): ";
		cin >> empID;

		if (empID == -1)
		{
			return;
		}
	}

	try
	{
		dc.pstmt = dc.prepStmt("DELETE FROM EMPLOYEE WHERE Employee_ID=?");
		dc.pstmt->setInt(1, empID);

		if (dc.pstmt->executeUpdate())
		{
			cout << endl << "The employee with employee id " << empID << " is successfully deleted" << endl;
		}
		else
		{
			cout << "\nError!!The query does not run correctly!";
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

void Employee::updateEmployee()
{
	DatabaseConnection dc;
	int option;
	int empID;

	cout << "\n\nPlease enter the employee id of employee that want to update (-1 to cancel): ";
	cin >> empID;

	if (empID == -1)
	{
		return;
	}

	while (inputValidation(empID) || checkEmployeeId(empID) == false)
	{
		cout << "\n\nPlease enter the employee id of employee that want to update (-1 to cancel): ";
		cin >> empID;

		if (empID == -1)
		{
			return;
		}
	}

	cout << "\nPlease choose the information that want to update" << endl;
	cout << "1) Employee name" << endl;
	cout << "2) Employee address" << endl;
	cout << "3) Employee contact number" << endl;
	cout << "4) Employee salary" << endl;
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
			cout << "\nPlease enter new employee name: ";
			getline(cin, name);

			dc.pstmt = dc.prepStmt("UPDATE EMPLOYEE SET Employee_Name = ? WHERE Employee_ID =?");
			dc.pstmt->setString(1, name);
			dc.pstmt->setInt(2, empID);
			break;
		case 2:
			cout << "\nPlease enter new address: ";
			getline(cin, address);

			dc.pstmt = dc.prepStmt("UPDATE EMPLOYEE SET Employee_Address = ? WHERE Employee_ID =?");
			dc.pstmt->setString(1, address);
			dc.pstmt->setInt(2, empID);
			break;
		case 3:
			cout << "\nPlease enter new contact number: ";
			getline(cin, contactNo);
			dc.pstmt = dc.prepStmt("UPDATE EMPLOYEE SET Employee_Phone = ? WHERE Employee_ID =?");
			dc.pstmt->setString(1, contactNo);
			dc.pstmt->setInt(2, empID);
			break;
		case 4:
			cout << "\nPlease enter new salary: RM";
			cin >> salary;

			dc.pstmt = dc.prepStmt("UPDATE EMPLOYEE SET `Employee_Salary(RM)` = ? WHERE Employee_ID =?");
			dc.pstmt->setDouble(1, salary);
			dc.pstmt->setInt(2, empID);
			cin.ignore();
			break;
		}

		dc.pstmt->executeUpdate();

		cout << "\n\nThe information of employee is updated\n";
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

void Employee::setBookshopId(int id)
{
	bookshopId = id;
}

bool Employee::inputValidation(int input)
{
	if (cin.fail()||input<0)
	{
		cin.clear();
		cin.ignore(numeric_limits <streamsize>::max(), '\n');
		cout << "\nI'm sorry but the valid choice is only positive numeric number. Please try again" << endl;

		return true;
	}

	return false;
}

bool Employee::inputValidation(int lower, int upper, int input)
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

bool Employee::checkEmployeeId(int id)
{
	DatabaseConnection dc;

	dc.pstmt = dc.prepStmt("SELECT Employee_ID FROM EMPLOYEE WHERE Employee_ID=?");
	dc.pstmt->setInt(1, id);
	dc.res = dc.pstmt->executeQuery();

	if (dc.res->next())
	{
		return true;
	}
	else
	{
		cout << "\n\nError!! There is not exist employee with employee id " << id << endl;
		return false;
	}
}

void Employee::searchEmployee()
{
	int option;
	char repeat;

	do
	{
		system("cls");
		repeat = 'a';

		cout << "Search employee by" << endl;
		cout << "1)Employee id" << endl;
		cout << "2)Employee name" << endl;
		cout << "3)Employee IC" << endl;
		cout << "4)Exit to previous menu\n";
		cout << "Option: ";
		cin >> option;

		while (inputValidation(1, 4, option))
		{
			cin >> option;
		}

		if (option == 1)
		{
			int inputId;
			vector<int> id;

			cout << "\nEmployee id (-1 to cancel): ";
			cin >> inputId;

			if (inputId == -1)
			{
				repeat = 'y';
				continue;
			}

			while (inputValidation(inputId))
			{
				cout << "\nEmployee id (-1 to cancel): ";
				cin >> inputId;

				if (inputId == -1)
				{
					repeat = 'y';
					break;
				}
			}

			id.push_back(inputId);

			displayEmployee(id);
		}
		else if (option == 2)
		{
			string empName;

			cout << "\nEmployee name (-1 to cancel): ";
			cin.ignore();
			getline(cin, empName);

			if (empName == "-1")
			{
				repeat = 'y';
				continue;
			}

			searchEmployee(empName, SearchEmployee::EMPLOYEENAME);
		}
		else if (option == 3)
		{
			string ic;

			cout << "\nEmployee IC (-1 to cancel): ";
			cin.ignore();
			getline(cin, ic);

			if (ic == "-1")
			{
				repeat = 'y';
				continue;
			}

			searchEmployee(ic, SearchEmployee::EMPLOYEEIC);
		}
		else if (option == 4)
		{
			return;
		}

		while (tolower(repeat) != 'y' && tolower(repeat) != 'n')
		{
			cout << "\n\n***Please enter Y to search another employee information, enter N to exit to previous menu: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}

	} while (tolower(repeat) == 'y');

	
}

void Employee::searchEmployee(string searchContent, SearchEmployee searchOption)
{
	DatabaseConnection dc;
	int inputId{};
	vector<int> id{};
	string searchType;

	switch (searchOption)
	{
	case SearchEmployee::EMPLOYEENAME:
		searchType = "Employee_Name";
		break;
	case SearchEmployee::EMPLOYEEIC:
		searchType = "Employee_IC";
		break;
	}

	try
	{
		string sql = "SELECT Employee_ID FROM EMPLOYEE WHERE " + searchType + " LIKE ?";
		dc.pstmt = dc.prepStmt(sql);
		dc.pstmt->setString(1, "%" + searchContent + "%");
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
		{
			inputId = dc.res->getInt("Employee_ID");
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

	displayEmployee(id);
}

void Employee::displayEmployee()
{
	DatabaseConnection dc;
	int i = 1;

	try
	{
		dc.pstmt = dc.prepStmt("SELECT * FROM EMPLOYEE");
		dc.res = dc.pstmt->executeQuery();

		if (dc.res->next() == true)
		{
			cout << endl << left << setw(12) << " Employee_ID" << setw(35) << "\tEmployee_Name" << setw(45) << "Employee_Address";
			cout << setw(20) << "Employee_Phone" << setw(11) << "Employee_Salary(RM)" << setw(20) << "\tEmployee_IC";
			cout << endl;

			do
			{
				employeeId = dc.res->getInt("Employee_ID");
				name = dc.res->getString("Employee_Name");
				address = dc.res->getString("Employee_Address");
				contactNo = dc.res->getString("Employee_Phone");
				salary = dc.res->getDouble("Employee_Salary(RM)");
				ic = dc.res->getString("Employee_IC");

				cout << right << setw(2) << i << ") " << left << setw(13) << employeeId;
				cout << setw(35) << name;
				cout << setw(45) << address;
				cout << setw(26) << contactNo;
				cout << setw(14) << salary;
				cout << setw(20) << ic;
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

void Employee::displayEmployee(vector<int> id)
{
	DatabaseConnection dc;
	int i = 1;
	bool titlePrint{ true };

	if (id.empty())
	{
		cout << "\n\nThe employee is not found in the database\n";
	}

	try
	{
		for (int empId : id)
		{
			dc.pstmt = dc.prepStmt("SELECT * FROM EMPLOYEE WHERE Employee_ID=?");
			dc.pstmt->setInt(1, empId);
			dc.res = dc.pstmt->executeQuery();

			if (dc.res->next() == true)
			{
				if (titlePrint)
				{
					cout << endl << left << setw(12) << " Employee_ID" << setw(35) << "\tEmployee_Name" << setw(45) << "Employee_Address";
					cout << setw(20) << "Employee_Phone" << setw(11) << "Employee_Salary(RM)" << setw(20) << "\tEmployee_IC";
					cout << endl;
					titlePrint = false;
				}

				do
				{
					employeeId = dc.res->getInt("Employee_ID");
					name = dc.res->getString("Employee_Name");
					address = dc.res->getString("Employee_Address");
					contactNo = dc.res->getString("Employee_Phone");
					salary = dc.res->getDouble("Employee_Salary(RM)");
					ic = dc.res->getString("Employee_IC");

					cout << right << setw(2) << i << ") " << left << setw(13) << employeeId;
					cout << setw(35) << name;
					cout << setw(45) << address;
					cout << setw(26) << contactNo;
					cout << setw(14) << salary;
					cout << setw(20) << ic;
					cout << endl;
					++i;

				} while (dc.res->next());
			}
			else
			{
				cout << "\n\nThe employee is not found in the database!!!";
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