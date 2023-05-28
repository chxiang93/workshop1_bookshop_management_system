#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;
#include "DatabaseConnection.h"

#ifndef EXPENSE_H
#define EXPENSE_H

class Expense
{
	//these are the variable that will store the data from database 
private:
	int bookshopId{};
	int expenseId{};
	string name{};
	double amount{};

	//these are declaration of methods used in this class
public:
	Expense();
	void editExpense();				 //edit the monthly expense information
	void addExpense();				 //add monthly expense
	void deleteExpense();			 //delete monthly expense
	void updateExpense();			 //update monthly expense
	void setBookshopId(int id);		 //set bookshop id
	bool inputValidation(int input);	 //check if the input from user is number
	bool inputValidation(int lower, int upper, int input);	//check if the input from user is valid in some range

	bool checkExpenseId(int id);			//check if the monthly expense exist
	void searchExpense();				//search monthly expense
	void searchExpense(string name);		//search monthly expense by name

	void displayExpense();	//display all monthly expense
	void displayExpense(vector<int> id);		//display specific monthly expense
};

#endif
