#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;
#include "DatabaseConnection.h"

#ifndef ORDER_H
#define ORDER_H

class Order
{
	//these are the variable that will store the data from database 
private:
	int bookshopId{};
	int invoiceId{};
	string invoiceDate{};
	double totalPaid{};
	string paymentMethod{};
	int memberId{};
	int customerId{};

	//these are declaration of methods used in this class
public:
	Order();
	void addOrder();		//this method add a new order into the database
	void setBookshopId(int id);	//set bookshop id
	bool inputValidation(int input);		//check if the input from user is number
	bool inputValidation(int lower, int upper, int input);	//check if the input from user is valid in some range
	
	bool hasStock(int id, int quantity);		//check if the book has stock
	void updateStock(const vector<int>& bookId, const vector<int>& quantity);	//update the stock of book
	double getMemberDiscount();		//get member discount
	double getBookPrice(int id);		//get the price of book based on its book id
	double calculateTotalPrice(const vector<int>& bookId, const vector<int>& quantity);		//calculate total price of order
	double calculateTotalDiscountedPrice(double totalPrice);		//calculate price for member
	bool checkMemberId(int id);		//check if the member exist
	void saveOrderData(const vector<int>& bookId, const vector<int>& quantity);		//save order data

	void searchOrder();		//search for a order

	void displayOrder();		//display all the order
	void displayOrder(vector<int> id);	//display specific order
};
#endif