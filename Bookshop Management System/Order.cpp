#include "Order.h"

Order::Order()
{

}

void Order::setBookshopId(int id)
{
	bookshopId = id;
}

void Order::addOrder()
{
	int option{ 1 };
	int paymentChoice{};
	char repeat;
	
	cout << "\n\nPlease enter new order information: ";

	do
	{
		try
		{
			vector<int> bookId;		// store the book id of book that bought by customer
			vector<int> quantity;	// store the quantity of book that bought by customer

			// enter order of book and quantity
			while (option != 0)
			{
				int id;
				int qty;
				bool bookNameStatus = true;

				cout << "\n\nBook ID (-1 to enter book name, -2 to cancel): ";
				cin >> id;	

				if (id == -1)
				{
					DatabaseConnection dc;
					string bookName;

					cout << "\n\nBook Name: ";
					cin.ignore(numeric_limits <streamsize>::max(), '\n');
					getline(cin, bookName);

					dc.pstmt = dc.prepStmt("SELECT Book_ID FROM BOOK WHERE Book_Name=?");
					dc.pstmt->setString(1, bookName);
					dc.res = dc.pstmt->executeQuery();

					if (dc.res->next())
					{
						id = dc.res->getInt("Book_ID");
					} 
					else
					{
						bookNameStatus = false;
						id = 0;
						cout << "\n\nThe book is not found in the database. Please try again\n";
					}
				}
				else if (id == -2)
				{
					return;
				}

				while (inputValidation(id) || bookNameStatus == false)
				{
					bookNameStatus = true;

					cout << "\nBook ID (-1 to enter book name, -2 to cancel order): ";
					cin >> id;

					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					if (id == -1)
					{
						DatabaseConnection dc;
						string bookName;

						cout << "\n\nBook Name: ";
						getline(cin, bookName);

						dc.pstmt = dc.prepStmt("SELECT Book_ID FROM BOOK WHERE Book_Name=?");
						dc.pstmt->setString(1, bookName);
						dc.res = dc.pstmt->executeQuery();

						if (dc.res->next())
						{
							id = dc.res->getInt("Book_ID");
						}
						else
						{
							bookNameStatus = false;
							id = 0;
							cout << "\n\nThe book is not found in the database. Please try again\n";
						}
					}
					else if (id == -2)
					{
						return;
					}
				}

				bookId.push_back(id);

				cout << "Quantity: ";
				cin >> qty;

				while (inputValidation(qty))
				{
					cout << "\nQuantity: ";
					cin >> qty;
				}

				quantity.push_back(qty);

				if (hasStock(id, qty) == false)
				{
					bookId.pop_back();
					quantity.pop_back();
				}

				cout << "\nEnter 1 to add more order, 0 to payment: ";
				cin >> option;

				while (inputValidation(0, 1, option))
				{
					cin >> option;
				}
			}

			// list out the order before saving to database
			cout << "\nBook ID" << "\t\tPrice" << "\t\tQuantity\n";
			for (int index{ 0 }; index < bookId.size(); ++index)
			{
				int id = bookId.at(index);
				cout << id;
				cout << "\t\t" << getBookPrice(id);
				cout << "\t\t" << quantity.at(index);
				cout << endl;
			}

			totalPaid = calculateTotalPrice(bookId, quantity);
			cout << "\nTotal Price = RM" << totalPaid;

			cout << "\n\nMember id (enter 0 if do not have member): ";
			cin >> memberId;

			while (inputValidation(memberId) || checkMemberId(memberId) == false)
			{
				cout << "\nMember id (enter 0 if do not have member): ";
				cin >> memberId;
			}

			double discount = getMemberDiscount();

			if (memberId != 0)
			{
				cout << "\nMember Discount = " << discount * 100 << "%\n";

				totalPaid = calculateTotalDiscountedPrice(totalPaid);
				cout << "\nTotal Price After Discount= RM" << totalPaid << endl;
			}

			cout << "\nPaymeny method (0 for cash and 1 for credit card): ";
			cin.ignore();
			cin >> paymentChoice;

			while (inputValidation(0, 1, paymentChoice))
			{
				cin >> paymentChoice;
			}

			if (paymentChoice == 1)
			{
				paymentMethod = "credit card";
			}
			else
			{
				paymentMethod = "cash";
			}

			char save;
			do
			{
				cout << "\n\n***Please enter Y to save the data to database, enter N to discard the order: ";
				cin >> save;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				
			} while (tolower(save) != 'y' && save != 'n');

			if (tolower(save) == 'y')
			{
				updateStock(bookId, quantity);
				saveOrderData(bookId, quantity);
			}
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

		do
		{
			cout << "\n\n***Please enter Y to add another order, enter N to exit to previous menu: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');

		} while (tolower(repeat) != 'y' && tolower(repeat) != 'n');

	}while (tolower(repeat) == 'y');
}

bool Order::inputValidation(int input)
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

bool Order::inputValidation(int lower, int upper, int input)
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

bool Order::hasStock(int id, int quantity)
{
	DatabaseConnection dc;
	int stock;

	dc.pstmt = dc.prepStmt("SELECT Stock FROM BOOK WHERE Book_ID=?");
	dc.pstmt->setInt(1, id);
	dc.res = dc.pstmt->executeQuery();

	if (dc.res->next())
	{
		stock = dc.res->getInt("Stock");
	}
	else
	{
		cout << "\nThere is no book with book id " << id << " in the bookshop!\n";
		return false;
	}

	if (stock < quantity)
	{
		cout << "\nThe stock of this book is insufficient!!\n";
		return false;
	}

	return true;
}

void Order::updateStock(const vector<int>& bookId, const vector<int>& quantity)
{
	DatabaseConnection dc;

	for (int index{ 0 }; index < bookId.size(); ++index)
	{
		int id = bookId.at(index);
		int qty = quantity.at(index);

		dc.pstmt = dc.prepStmt("UPDATE BOOK SET Stock=Stock-? WHERE Book_ID=?");
		dc.pstmt->setInt(1, qty);
		dc.pstmt->setInt(2, id);
		dc.pstmt->executeUpdate();
	}
}

double Order::getMemberDiscount()
{
	DatabaseConnection dc;
	double discount;

	dc.pstmt = dc.prepStmt("SELECT Member_Discount FROM BOOKSHOP WHERE Bookshop_ID=?");
	dc.pstmt->setInt(1, bookshopId);
	dc.res = dc.pstmt->executeQuery();

	if (dc.res->next())
	{
		discount = dc.res->getDouble("Member_Discount");
	}
	else
	{
		cout << "\n\nError!!\n";
		return 0;
	}

	return discount;
}

double Order::getBookPrice(int id)
{
	DatabaseConnection dc;

	// get the price of each book from database
	dc.pstmt = dc.prepStmt("SELECT `Price(RM)` FROM BOOK WHERE Book_ID=?");
	dc.pstmt->setInt(1, id);
	dc.res = dc.pstmt->executeQuery();

	if (dc.res->next())
	{
		double price = dc.res->getDouble("Price(RM)");
		return price;
	}
	else
	{
		cout << "\n\nError!!\n";
		return 0;
	}
}

double Order::calculateTotalPrice(const vector<int>& bookId, const vector<int>& quantity)
{
	double totalPrice = 0;

	// calculate the total price that need to paid by customer
	for (int index{ 0 }; index < bookId.size(); ++index)
	{
		int id = bookId.at(index);

		double price = getBookPrice(id) * quantity.at(index);

		totalPrice += price;
	}

	return totalPrice ;
}

double Order::calculateTotalDiscountedPrice(double totalPrice)
{
	double discount = getMemberDiscount();
	return totalPrice * (1 - discount);
}

bool Order::checkMemberId(int id)
{
	if (id == 0)
	{
		return true;
	}

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

void Order::saveOrderData(const vector<int>& bookId, const vector<int>& quantity)
{
	DatabaseConnection dc;

	// insert to customer table in database
	if (memberId != 0)
	{
		dc.pstmt = dc.prepStmt("INSERT INTO CUSTOMER (Payment_Method,Member_ID) VALUES (?,?)");
		dc.pstmt->setString(1, paymentMethod);
		dc.pstmt->setInt(2, memberId);
		dc.pstmt->execute();
	}
	else
	{
		dc.pstmt = dc.prepStmt("INSERT INTO CUSTOMER (Payment_Method) VALUES (?)");
		dc.pstmt->setString(1, paymentMethod);
		dc.pstmt->execute();
	}

	// get the customer id that created just now 
	dc.pstmt = dc.prepStmt("SELECT Customer_ID FROM CUSTOMER ORDER BY Customer_ID DESC LIMIT 1");
	dc.res = dc.pstmt->executeQuery();
	while (dc.res->next())
	{
		customerId = dc.res->getInt("Customer_ID");
	}

	dc.pstmt = dc.prepStmt("INSERT INTO INVOICE (`Total_Paid(RM)`,Customer_ID) VALUES (?,?)");
	dc.pstmt->setDouble(1, totalPaid);
	dc.pstmt->setInt(2, customerId);
	dc.pstmt->execute();

	dc.pstmt = dc.prepStmt("SELECT Invoice_ID FROM INVOICE ORDER BY Invoice_ID DESC LIMIT 1");
	dc.res = dc.pstmt->executeQuery();
	while (dc.res->next())
		invoiceId = dc.res->getInt(1);

	for (int index{ 0 }; index < bookId.size(); ++index)
	{
		dc.pstmt = dc.prepStmt("INSERT INTO BOOK_INVOICE VALUES (?,?,?)");
		dc.pstmt->setInt(1, invoiceId);
		dc.pstmt->setInt(2, bookId.at(index));
		dc.pstmt->setInt(3, quantity.at(index));
		dc.pstmt->execute();
	}
}

void Order::searchOrder()
{
	int option;
	char repeat;

	do
	{
		system("cls");
		repeat = 'a';

		cout << "Search order by" << endl;
		cout << "1)Invoice id" << endl;
		cout << "2)Month and Year" << endl;
		cout << "Option: ";
		cin >> option;

		while (inputValidation(1, 2, option))
		{
			cin >> option;
		}

		if (option == 1)
		{
			int inputId;
			vector<int> id;

			cout << "\nInvoice id (-1 to cancel): ";
			cin >> inputId;

			if (inputId == -1)
			{
				break;
			}

			while (inputValidation(inputId))
			{
				cout << "\nInvoice id (-1 to cancel): ";
				cin >> inputId;

				if (inputId == -1)
				{
					repeat = 'n';
					break;
				}
			}

			id.push_back(inputId);

			displayOrder(id);
		}
		else if (option == 2)
		{
			int month, year;
			vector<int> id;

			cout << "\nMonth (-1 to cancel): ";
			cin >> month;

			if (month == -1)
			{
				return;
			}

			while (inputValidation(1,12,month))
			{
				cout << "\n\nMonth (-1 to cancel): ";
				cin >> month;

				if (month == -1)
				{
					return;
				}
			}

			cout << "Year: ";
			cin >> year;

			while (inputValidation(year))
			{
				cout << "\nYear: ";
				cin >> year;
			}

			DatabaseConnection dc;
			dc.pstmt = dc.prepStmt("SELECT Invoice_ID FROM INVOICE WHERE MONTH(Invoice_Date_Time)=? AND YEAR(Invoice_Date_Time)=?");
			dc.pstmt->setInt(1, month);
			dc.pstmt->setInt(2, year);
			dc.res = dc.pstmt->executeQuery();

			while (dc.res->next())
			{
				int tempId = dc.res->getInt("Invoice_ID");
				id.push_back(tempId);
			}

			displayOrder(id);
		}
		

		while (tolower(repeat) != 'y' && tolower(repeat) != 'n')
		{
			cout << "\n\n***Please enter Y to search another order information, enter N to exit to previous menu: ";
			cin >> repeat;
			cin.ignore(numeric_limits <streamsize>::max(), '\n');
		}
	} while (tolower(repeat) == 'y');
}

void Order::displayOrder()
{
	DatabaseConnection dc;
	int i = 1;

	try
	{
		dc.pstmt = dc.prepStmt("SELECT Invoice_ID,Invoice_Date_Time,`Total_Paid(RM)`,Payment_Method,Member_ID FROM INVOICE NATURAL JOIN CUSTOMER");
		dc.res = dc.pstmt->executeQuery();

		if (dc.res->next() == true)
		{
			cout << left << setw(16) << "\n   Invoice_ID" << setw(23) << "\tInvoice_Date_Time" << setw(17) << "Total_Paid(RM)";
			cout << setw(20) << "Payment_Method" << "Member_ID";
			cout << endl;

			do
			{
				invoiceId = dc.res->getInt("Invoice_ID");
				invoiceDate = dc.res->getString("Invoice_Date_Time");
				totalPaid = dc.res->getDouble("Total_Paid(RM)");
				paymentMethod = dc.res->getString("Payment_Method");
				memberId = dc.res->getInt("Member_ID");

				cout << right << setw(2) << i << ") " << left << setw(13) << invoiceId;
				cout << setw(25) << invoiceDate;
				cout << setw(16) << totalPaid;
				cout << setw(20) << paymentMethod;

				if (memberId == 0)
				{
					cout << "None";
				}
				else
				{
					cout << memberId;
				}

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

void Order::displayOrder(vector<int> id)
{
	DatabaseConnection dc;
	int i = 1;
	bool titlePrint{ true };

	if (id.empty())
	{
		cout << "\n\nThe order is not found in the database\n";
	}

	try
	{
		for (int invId : id)
		{
			dc.pstmt = dc.prepStmt("SELECT Invoice_ID,Invoice_Date_Time,`Total_Paid(RM)`,Payment_Method,Member_ID FROM INVOICE NATURAL JOIN CUSTOMER WHERE Invoice_ID=?");
			dc.pstmt->setInt(1, invId);
			dc.res = dc.pstmt->executeQuery();

			if (dc.res->next() == true)
			{
				if (titlePrint)
				{
					cout << left << setw(16) << "\n   Invoice_ID" << setw(23) << "\tInvoice_Date_Time" << setw(17) << "Total_Paid(RM)";
					cout << setw(20) << "Payment_Method" << "Member_ID";
					cout << endl;
					titlePrint = false;
				}

				do
				{
					invoiceId = dc.res->getInt("Invoice_ID");
					invoiceDate = dc.res->getString("Invoice_Date_Time");
					totalPaid = dc.res->getDouble("Total_Paid(RM)");
					paymentMethod = dc.res->getString("Payment_Method");
					memberId = dc.res->getInt("Member_ID");

					cout << right << setw(2) << i << ") " << left << setw(13) << invoiceId;
					cout << setw(25) << invoiceDate;
					cout << setw(16) << totalPaid;
					cout << setw(20) << paymentMethod;

					if (memberId == 0)
					{
						cout << "None";
					}
					else
					{
						cout << memberId;
					}

					cout << endl;
					++i;

				} while (dc.res->next());
			}
			else
			{
				cout << "\n\nThe order is not found in the database\n";
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
