#include <iostream>
#include <string>
#include <cstdlib>
#include <conio.h>
using namespace std;
#include "DatabaseConnection.h"
#include "Employee.h"
#include "Expense.h"
#include "Book.h"
#include "Member.h"
#include "Order.h"

#ifndef BOOKSHOP_H
#define BOOKSHOP_H

class Bookshop 
{
private:
	int bookshopId{};
	double discount{};

	//these are declaration of methods used in this class
public:
	Bookshop();
	Book book{};			//create a book object
	Employee emp{};		//create a employee object
	Expense expense{};	//create a expense object
	Member member{};		//create a member object
	Order order{};		//create a order object
	
	void updateAdminUsernamePassword();	//change admin username and password
	bool adminLogin(bool logout);		//login function of system
	void updateDiscount();				//update member discount rate
	bool inputValidation(int input);		//check if the input from user is number
	bool inputValidation(int lower, int upper, int input);	//check if the input from user is valid in some range

	string convertMonthName(int month);	//convert month to its name
	void updateMonthlyIncome();		//update monthly income of bookshop
	void displayMonthlyIncome();		//display monthly income of bookshop
};

#endif