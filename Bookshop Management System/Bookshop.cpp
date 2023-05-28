#include "Bookshop.h"

Bookshop::Bookshop()
{
	DatabaseConnection dc;

	try
	{
		dc.pstmt = dc.prepStmt("SELECT Bookshop_ID FROM BOOKSHOP");
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
			bookshopId = dc.res->getInt("Bookshop_ID");
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

	emp.setBookshopId(bookshopId);
	book.setBookshopId(bookshopId);
	expense.setBookshopId(bookshopId);
	member.setBookshopId(bookshopId);
	order.setBookshopId(bookshopId);
}

void Bookshop::updateAdminUsernamePassword()
{
	DatabaseConnection dc;
	string username;
	string password = "";
	string newPassword = "";
	string newUsername;
	bool status = false;
	 
	cout << "Original Username: ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, username);

	cout << "Original password: ";
	char ch = _getch();
	while (ch != 13)
	{
		password.push_back(ch);
		cout << '*';
		ch = _getch();
	}

	try
	{
		dc.pstmt = dc.prepStmt("SELECT Admin_Username, Admin_Password FROM BOOKSHOP WHERE Bookshop_ID=?");
		dc.pstmt->setInt(1, bookshopId);
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
		{
			if (username == dc.res->getString("Admin_Username") && password == dc.res->getString("Admin_Password"))
				status = true;
		}

		if (status == false)
		{
			cout << "\nWrong username or password\n";
			system("pause");
			return;
		}

		cout << "\n\nNew username: ";
		getline(cin, newUsername);

		cout << "New password: ";
		char chr = _getch();
		while (chr != 13)
		{
			newPassword.push_back(chr);
			cout << '*';
			chr = _getch();
		}

		dc.pstmt = dc.prepStmt("UPDATE BOOKSHOP SET Admin_Username=?, Admin_Password=? WHERE Bookshop_ID=?");
		dc.pstmt->setString(1, newUsername);
		dc.pstmt->setString(2, newPassword);
		dc.pstmt->setInt(3, bookshopId);

		if (dc.pstmt->executeUpdate())
			cout << "\n\nThe admin username and password is updated successfully\n " << endl;
		else
			cout << "\n\nError!! Something wrong!!\n";

		system("pause");
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

//admin login page
bool Bookshop::adminLogin(bool logout=false)
{
	cin.ignore(numeric_limits <streamsize>::max(), '\n');
	string username;
	string password = "";
	static bool loginStatus{ false };

	if (logout == true)
	{
		loginStatus = false;
	}

	if (loginStatus == true)
	{
		return true;
	}

	DatabaseConnection dc;

	cout << "=============================================================================================\n";
	cout << "## _______________________________________________________________________________________ ##\n";
	cout << "Please enter your username and password to access the system\n";
	cout << "Username: ";
	getline(cin, username);
	cout << "Password: ";
	char ch = _getch();
	while (ch != 13)
	{
		password.push_back(ch);
		cout << '*';
		ch = _getch();
	}

	try
	{
		dc.pstmt = dc.prepStmt("SELECT Admin_Username, Admin_Password FROM BOOKSHOP WHERE Bookshop_ID=?");
		dc.pstmt->setInt(1, bookshopId);
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
		{
			if (username == dc.res->getString("Admin_Username") && password == dc.res->getString("Admin_Password"))
				loginStatus = true;
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

	if (loginStatus == false)
	{
		cout << "\n\nYour username or password is incorrect. Please try again" << endl;
	}

	return loginStatus;
}

void Bookshop::updateDiscount()
{
	DatabaseConnection dc;

	try
	{
		dc.pstmt = dc.prepStmt("SELECT Member_Discount FROM BOOKSHOP WHERE Bookshop_ID=?");
		dc.pstmt->setInt(1, bookshopId);
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
		{
			discount = dc.res->getDouble("Member_Discount");
		}

		cout << "Original member discount: " << discount << endl << endl;

		cout << "Enter new member discount (-1 to cancel): ";
		cin >> discount;

		if (discount == -1)
		{
			return;
		}

		while (inputValidation(0,1,discount))
		{
			cout << "\n\nEnter new member discount (-1 to cancel): ";
			cin >> discount;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			if (discount == -1)
			{
				return;
			}
		}

		dc.pstmt = dc.prepStmt("UPDATE BOOKSHOP SET Member_Discount=? WHERE Bookshop_ID=?");
		dc.pstmt->setDouble(1, discount);
		dc.pstmt->setInt(2, bookshopId);
		dc.pstmt->executeUpdate();

		cout << "\n\nThe member discount is updated successfully\n";
	}
	catch (SQLException& e)
	{
		/* what() (derived from std::runtime_error) fetches error message */
		cout << "# ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	catch (...)
	{
		cout << "\n\nError!! Something unexpected occur!!\n";
	}
}

bool Bookshop::inputValidation(int input)
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

bool Bookshop::inputValidation(int lower, int upper, int input)
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

string Bookshop::convertMonthName(int month)
{
	switch (month)
	{
	case 1: 
		return "January";
	case 2:
		return "February";
	case 3:
		return "March";
	case 4:
		return "April";
	case 5:
		return "May";
	case 6:
		return "June";
	case 7:
		return "July";
	case 8:
		return "August";
	case 9:
		return "September";
	case 10:
		return "October";
	case 11:
		return "November";
	case 12:
		return "December";
	}
}

void Bookshop::updateMonthlyIncome()
{
	DatabaseConnection dc;
	int i = 1;
	double monthlyExpense{};
	double monthlySalaryPayed{};
	double monthlySales{};
	int month{};
	int year{};
	int monthInc{};
	int yearInc{};
	bool updateIncome{ false };

	try
	{
		//get monthly expense
		dc.pstmt = dc.prepStmt("SELECT SUM(`Amount(RM)`) AS amount FROM MONTHLY_EXPENSE");
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
		{
			monthlyExpense = dc.res->getDouble("amount");
		}

		//get sum of all employee salary
		dc.pstmt = dc.prepStmt("SELECT SUM(`Employee_Salary(RM)`) AS salary FROM EMPLOYEE");
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
		{
			monthlySalaryPayed = dc.res->getDouble("salary");
		}

		//get latest month and year from invoice
		dc.pstmt = dc.prepStmt("SELECT MONTH(Invoice_Date_Time) AS month, YEAR(Invoice_Date_Time) AS year FROM INVOICE ORDER BY Invoice_Date_Time DESC LIMIT 1");
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
		{
			month = dc.res->getInt("month");
			year = dc.res->getInt("year");
		}

		// Get the monthly sales
		dc.pstmt = dc.prepStmt("SELECT MONTH(Invoice_Date_Time) AS month,YEAR(Invoice_Date_Time) AS year,SUM(`Total_Paid(RM)`) AS sales FROM INVOICE GROUP BY month HAVING month=? AND year=?");
		dc.pstmt->setInt(1, month);
		dc.pstmt->setInt(2, year);
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
		{
			monthlySales = dc.res->getDouble("sales");
		}

		//calculate monthly income
		double monthlyIncome = monthlySales - monthlyExpense - monthlySalaryPayed;

		//get latest month and year from monthly_income table
		dc.pstmt = dc.prepStmt("SELECT Month, Year FROM MONTHLY_INCOME");
		dc.res = dc.pstmt->executeQuery();

		while (dc.res->next())
		{
			monthInc = dc.res->getInt("month");
			yearInc = dc.res->getInt("year");

			if (month == monthInc && year == yearInc)
			{
				updateIncome = true;
			}
		}

		//if the latest invoice date is same with that with monthly_income date
		//this means that we havent reach end of the month or is on the last day
		//of the month, hence we need to update the income for that month
		if (updateIncome)
		{
			dc.pstmt = dc.prepStmt("UPDATE MONTHLY_INCOME SET `Monthly_Sales(RM)`=?, `Monthly_Expense(RM)`=?, `Monthly_Employee_Salary(RM)`=?,`Monthly_Income(RM)`=? WHERE Month=? AND Year=? ");
			dc.pstmt->setDouble(1, monthlySales);
			dc.pstmt->setDouble(2, monthlyExpense);
			dc.pstmt->setDouble(3, monthlySalaryPayed);
			dc.pstmt->setDouble(4, monthlyIncome);
			dc.pstmt->setInt(5, month);
			dc.pstmt->setInt(6, year);
			dc.pstmt->executeUpdate();
		}
		else //if reach next month already, create a new monthly income record
		{
			dc.pstmt = dc.prepStmt("INSERT INTO MONTHLY_INCOME (Month,Year,`Monthly_Sales(RM)`,`Monthly_Expense(RM)`,`Monthly_Employee_Salary(RM)`,`Monthly_Income(RM)`,Bookshop_ID) VALUES (?,?,?,?,?,?,?)");
			dc.pstmt->setInt(1, month);
			dc.pstmt->setInt(2, year);
			dc.pstmt->setDouble(3, monthlySales);
			dc.pstmt->setDouble(4, monthlyExpense);
			dc.pstmt->setDouble(5, monthlySalaryPayed);
			dc.pstmt->setDouble(6, monthlyIncome);
			dc.pstmt->setInt(7, bookshopId);
			dc.pstmt->executeUpdate();
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

void Bookshop::displayMonthlyIncome()
{
	DatabaseConnection dc;
	int i = 1;
	double monthlyExpense;
	double monthlySalaryPayed;
	double monthlySales;
	double monthlyIncome;
	int month;
	int year;

	updateMonthlyIncome();

	try
	{
		dc.pstmt = dc.prepStmt("SELECT * FROM MONTHLY_INCOME ORDER BY Year DESC, Month DESC");
		dc.res = dc.pstmt->executeQuery();

		if (dc.res->next() == true)
		{
			cout << left << setw(12) << "     Month" << setw(20) <<"\t    Year" << setw(18) << "Monthly_Sales(RM)" << setw(25) << "Monthly_Expense(RM)";
			cout << setw(30) << "Monthly_Employee_Salary(RM)" << setw(20) << "Monthly_Income(RM)";
			cout << endl;

			do
			{
				month = dc.res->getInt("Month");
				year = dc.res->getInt("Year");
				monthlySales = dc.res->getDouble("Monthly_Sales(RM)");
				monthlyExpense = dc.res->getDouble("Monthly_Expense(RM)");
				monthlySalaryPayed = dc.res->getDouble("Monthly_Employee_Salary(RM)");
				monthlyIncome = dc.res->getDouble("Monthly_Income(RM)");

				cout << right << setw(2) << i << ") " << left << setw(16) << convertMonthName(month);
				cout << setw(20) << year;
				cout << setw(22) << monthlySales;
				cout << setw(28) << monthlyExpense;
				cout << setw(26) << monthlySalaryPayed;
				cout << setw(20) << monthlyIncome;
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
