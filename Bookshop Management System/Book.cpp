#include "Book.h"

Book::Book()
{

}

void Book::editBook()
{
	int option;
	char repeat;
	
	do
	{
		system("cls");
		repeat = 'a';

		cout << "Please choose a option to edit book information\n";
		cout << "1) Add a book information\n";
		cout << "2) Update a book information\n";
		cout << "3) Delete a book information\n";
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
			addBook();
			break;
		case 2:
			displayBook();
			updateBook();
			break;
		case 3:
			displayBook();
			deleteBook();
			break;
		case 4:
			return;
		}

		while (tolower(repeat) != 'y' && tolower(repeat) != 'n')
		{
			cout << "\n\n***Please enter Y to edit another book information, enter N to exit to previous menu: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}

	} while (tolower(repeat) == 'y');
}

void Book::addBook()
{
	DatabaseConnection dc;
	char save;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	do
	{
		save = 'a';

		cout << "\n\nBook name (-1 to cancel): ";
		getline(cin, book_name);

		if (book_name == "-1")
		{
			return;
		}

		cout << "Author name: ";
		getline(cin, author_name);

		cout << "Cost: RM";
		cin >> cost;

		while (inputValidation(cost))
		{
			cout << "\n\nCost: RM";
			cin >> cost;
		}

		cout << "Price: RM";
		cin >> price;

		while (inputValidation(price))
		{
			cout << "\n\nPrice: RM";
			cin >> price;
		}

		cout << "Publisher name: ";
		cin.ignore();
		getline(cin, publisher_name);

		cout << "Stock: ";
		cin >> stock;

		while (inputValidation(stock))
		{
			cout << "\n\nStock: ";
			cin >> stock;
		}

		cout << "Category: ";
		cin.ignore();
		getline(cin, category);

		while (tolower(save) != 'y' && tolower(save) != 'n')
		{
			cout << "\n\nAre the information correct? (Y/N): ";
			cin >> save;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}

	} while (tolower(save) == 'n');

	try
	{
		dc.pstmt = dc.prepStmt("INSERT INTO BOOK (Book_Name,Author_Name,`Cost(RM)`,`Price(RM)`,Publisher_Name,Stock,Category,Bookshop_ID) VALUES (?,?,?,?,?,?,?,?)");
		dc.pstmt->setString(1, book_name);
		dc.pstmt->setString(2, author_name);
		dc.pstmt->setDouble(3, cost);
		dc.pstmt->setDouble(4, price);
		dc.pstmt->setString(5, publisher_name);
		dc.pstmt->setInt(6, stock);
		dc.pstmt->setString(7, category);
		dc.pstmt->setInt(8, bookshopId);

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

	cout << "\nThe data of book is successfully save into the database!!\n";
}

void Book::deleteBook()
{
	DatabaseConnection dc;
	int bookId;

	cout << "\n\nPlease enter the book id of book that want to delete (-1 to cancel): ";
	cin >> bookId;

	if (bookId == -1)
	{
		return;
	}

	while (inputValidation(bookId))
	{
		cout << "\n\nPlease enter the book id of book that want to delete (-1 to cancel): ";
		cin >> bookId;

		if (bookId == -1)
		{
			return;
		}

	}

	try
	{
		dc.pstmt = dc.prepStmt("DELETE FROM BOOK WHERE Book_ID=?");
		dc.pstmt->setInt(1, bookId);

		if (dc.pstmt->executeUpdate())
			cout << "\nThe book with book id " << bookId << " is successfully deleted" << endl;
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

void Book::updateBook()
{
	DatabaseConnection dc;
	int option;
	int bookId;

	cout << "\n\nPlease enter the book id of book that want to update (-1 to cancel): ";
	cin >> bookId;

	if (bookId == -1)
	{
		return;
	}

	while (inputValidation(bookId) || checkBookId(bookId) == false)
	{
		cout << "\n\nPlease enter the book id of book that want to update (-1 to cancel): ";
		cin >> bookId;

		if (bookId == -1)
		{
			return;
		}
	}

	cout << "\nPlease choose the information that want to update" << endl;
	cout << "1) Book name" << endl;
	cout << "2) Author name" << endl;
	cout << "3) Cost(RM)" << endl;
	cout << "4) Price(RM)" << endl;
	cout << "5) Publisher name" << endl;
	cout << "6) Stock" << endl;
	cout << "7) Category" << endl;
	cout << "Option: ";
	cin >> option;
	cin.ignore();

	while (inputValidation(1, 7, option))
	{
		cin >> option;
		cin.ignore();
	}

	try
	{
		switch (option)
		{
		case 1:
			cout << "\nPlease enter new book name: ";
			getline(cin, book_name);

			dc.pstmt = dc.prepStmt("UPDATE BOOK SET Book_Name = ? WHERE Book_ID =?");
			dc.pstmt->setString(1, book_name);
			dc.pstmt->setInt(2, bookId);
			break;

		case 2:
			cout << "\nPlease enter new author name: ";
			getline(cin, author_name);

			dc.pstmt = dc.prepStmt("UPDATE BOOK SET Author_Name = ? WHERE Book_ID =?");
			dc.pstmt->setString(1, author_name);
			dc.pstmt->setInt(2, bookId);
			break;

		case 3:
			cout << "\nPlease enter new cost: RM";
			cin >> cost;

			dc.pstmt = dc.prepStmt("UPDATE BOOK SET `Cost(RM)` = ? WHERE Book_ID =?");
			dc.pstmt->setDouble(1, cost);
			dc.pstmt->setInt(2, bookId);
			break;

		case 4:
			cout << "\nPlease enter new price: RM";
			cin >> price;
			cin.ignore();

			dc.pstmt = dc.prepStmt("UPDATE BOOK SET `Price(RM)` = ? WHERE Book_ID =?");
			dc.pstmt->setDouble(1, price);
			dc.pstmt->setInt(2, bookId);
			break;

		case 5:
			cout << "\nPlease enter new publisher name: ";
			getline(cin, publisher_name);

			dc.pstmt = dc.prepStmt("UPDATE BOOK SET Publisher_Name = ? WHERE Book_ID =?");
			dc.pstmt->setString(1, publisher_name);
			dc.pstmt->setInt(2, bookId);
			break;

		case 6:
			cout << "\nPlease enter new stock: ";
			cin >> stock;
			cin.ignore();

			dc.pstmt = dc.prepStmt("UPDATE BOOK SET Stock = ? WHERE Book_ID =?");
			dc.pstmt->setInt(1, stock);
			dc.pstmt->setInt(2, bookId);
			break;

		case 7:
			cout << "\nPlease enter new category: ";
			getline(cin, category);

			dc.pstmt = dc.prepStmt("UPDATE BOOK SET Category = ? WHERE Book_ID =?");
			dc.pstmt->setString(1, category);
			dc.pstmt->setInt(2, bookId);
			break;
		}

		dc.pstmt->executeUpdate();

		cout << "\n\nThe information of book is updated\n";
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

void Book::setBookshopId(int id)
{
	bookshopId = id;
}

bool Book::inputValidation(int input)
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

bool Book::inputValidation(int lower, int upper, int input)
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

bool Book::checkBookId(int id)
{
	DatabaseConnection dc;

	dc.pstmt = dc.prepStmt("SELECT Book_ID FROM BOOK WHERE Book_ID=?");
	dc.pstmt->setInt(1, id);
	dc.res = dc.pstmt->executeQuery();

	if (dc.res->next())
	{
		return true;
	}
	else
	{
		cout << "\n\nError!! There is not exist book with book id " << id << endl;
		return false;
	}
}

void Book::searchBook()
{
	int option;
	char repeat;

	do
	{
		system("cls");
		repeat = 'a';

		cout << "Search book by" << endl;
		cout << "1)Book id" << endl;
		cout << "2)Book name" << endl;
		cout << "3)Author name" << endl;
		cout << "4)Publisher name" << endl;
		cout << "5)Category" << endl;
		cout << "6)Exit to previous menu\n";
		cout << "Option: ";
		cin >> option;

		while (inputValidation(1, 6, option))
		{
			cin >> option;
		}

		if (option == 1)
		{
			int inputId;
			vector<int> id;

			cout << "\nBook id (-1 to cancel): ";
			cin >> inputId;

			if (inputId == -1)
			{
				repeat = 'y';
				continue;
			}

			while (inputValidation(inputId))
			{
				cout << "\nBook id (-1 to cancel): ";
				cin >> inputId;

				if (inputId == -1)
				{
					repeat = 'y';
					break;
				}
			}

			id.push_back(inputId);

			displayBook(id);
		}
		else if (option == 2)
		{
			string name;

			cout << "\nBook name (-1 to cancel): ";
			cin.ignore();
			getline(cin, name);

			if (name == "-1")
			{
				repeat = 'y';
				continue;
			}

			searchBook(name, SearchBook::BOOKNAME);
		}
		else if (option == 3)
		{
			string name;

			cout << "\nAuthor name (-1 to cancel): ";
			cin.ignore();
			getline(cin, name);

			if (name == "-1")
			{
				repeat = 'y';
				continue;
			}

			searchBook(name, SearchBook::AUTHORNAME);
		}
		else if (option == 4)
		{
			string name;

			cout << "\nPublisher name (-1 to cancel): ";
			cin.ignore();
			getline(cin, name);

			if (name == "-1")
			{
				repeat = 'y';
				continue;
			}

			searchBook(name, SearchBook::PUBLISHERNAME);
		}
		else if (option == 5)
		{
			string name;

			cout << "\nCategory (-1 to cancel): ";
			cin.ignore();
			getline(cin, name);

			if (name == "-1")
			{
				repeat = 'y';
				continue;
			}

			searchBook(name, SearchBook::CATEGORY);
		}
		else if (option == 6)
		{
			return;
		}

		while (tolower(repeat) != 'y' && tolower(repeat) != 'n')
		{
			cout << "\n\n***Please enter Y to search another book information, enter N to exit to previous menu: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}

	} while (tolower(repeat) == 'y');
}

void Book::searchBook(string name, SearchBook searchOption)
{
	DatabaseConnection dc;
	int inputId;
	vector<int> id{};
	string searchType;

	switch (searchOption)
	{
	case SearchBook::BOOKNAME:
		searchType = "Book_Name";
		break;
	case SearchBook::AUTHORNAME:
		searchType = "Author_Name";
		break;
	case SearchBook::PUBLISHERNAME:
		searchType = "Publisher_Name";
		break;
	case SearchBook::CATEGORY:
		searchType = "Category";
		break;
	}

	try
	{
		string sql = "SELECT Book_ID FROM BOOK WHERE " + searchType + " LIKE ?";
		dc.pstmt = dc.prepStmt(sql);
		dc.pstmt->setString(1, "%" + name + "%");
		dc.res = dc.pstmt->executeQuery();
		
		while (dc.res->next())
		{
			inputId = dc.res->getInt("Book_ID");
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

	displayBook(id);
}

void Book::displayBook()
{
	DatabaseConnection dc;
	int i = 1;

	try
	{
		dc.pstmt = dc.prepStmt("SELECT * FROM BOOK");
		dc.res = dc.pstmt->executeQuery();


		if (dc.res->next() == true)
		{
			cout << left << setw(12) << "\n   Book_ID" << setw(30) << "\tBook_Name" << setw(28) << "Author_Name";
			cout << setw(15) << "Cost(RM)" << setw(15) << "Price(RM)" << setw(25) << "Publisher_Name" << setw(13) << "  Stock";
			cout << setw(10) << "Category" << endl;

			do
			{
				bookId = dc.res->getInt("Book_ID");
				book_name = dc.res->getString("Book_Name");
				author_name = dc.res->getString("Author_Name");
				cost = dc.res->getDouble("Cost(RM)");
				price = dc.res->getDouble("Price(RM)");
				publisher_name = dc.res->getString("Publisher_Name");
				stock = dc.res->getInt("Stock");
				category = dc.res->getString("Category");

				cout << right << setw(2) << i << ") " << left << setw(13) << bookId;
				cout << setw(30) << book_name;
				cout << setw(30) << author_name;
				cout << setw(13) << cost;
				cout << setw(13) << price;
				cout << setw(27) << publisher_name;
				cout << setw(10) << stock;
				cout << setw(20) << category;
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

void Book::displayBook(vector<int> id)
{
	DatabaseConnection dc;
	int i = 1;
	bool titlePrint{ true };

	if (id.empty())
	{
		cout << "\n\nThe book is not found in the database\n";
	}

	try
	{
		for (int bkId : id)
		{
			dc.pstmt = dc.prepStmt("SELECT * FROM BOOK WHERE Book_ID=?");
			dc.pstmt->setInt(1, bkId);
			dc.res = dc.pstmt->executeQuery();

			if (dc.res->next() == true)
			{
				if (titlePrint)
				{
					cout << left << setw(12) << "\n   Book_ID" << setw(30) << "\tBook_Name" << setw(28) << "Author_Name";
					cout << setw(15) << "Cost(RM)" << setw(15) << "Price(RM)" << setw(25) << "Publisher_Name" << setw(13) << "  Stock";
					cout << setw(10) << "Category" << endl;
					titlePrint = false;
				}

				do
				{
					bookId = dc.res->getInt("Book_ID");
					book_name = dc.res->getString("Book_Name");
					author_name = dc.res->getString("Author_Name");
					cost = dc.res->getDouble("Cost(RM)");
					price = dc.res->getDouble("Price(RM)");
					publisher_name = dc.res->getString("Publisher_Name");
					stock = dc.res->getInt("Stock");
					category = dc.res->getString("Category");

					cout << right << setw(2) << i << ") " << left << setw(13) << bookId;
					cout << setw(30) << book_name;
					cout << setw(30) << author_name;
					cout << setw(13) << cost;
					cout << setw(13) << price;
					cout << setw(27) << publisher_name;
					cout << setw(10) << stock;
					cout << setw(20) << category;
					cout << endl;
					++i;

				} while (dc.res->next());
			}
			else
			{
				cout << "\n\nThe book is not found in the database\n";
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
