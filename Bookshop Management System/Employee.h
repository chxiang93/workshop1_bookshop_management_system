#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;
#include "DatabaseConnection.h"

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

enum class SearchEmployee;	

class Employee
{
	//these are the variable that will store the data from database 
private:
	string name{};
	string address{};
	string contactNo{};
	double salary{};
	string ic{};
	int employeeId{};
	int bookshopId{};

	//these are declaration of methods used in this class
public:
	Employee();
	void editEmployee();			//edit the employee information
	void addEmployee();			//add employee
	void deleteEmployee();		//delete employee
	void updateEmployee();		//update employee
	void setBookshopId(int id);	//set bookshop id
	bool inputValidation(int input);		//check if the input from user is number
	bool inputValidation(int lower, int upper, int input);	//check if the input from user is valid in some range

	bool checkEmployeeId(int id);	//check if the employee exist
	void searchEmployee();			//search employee
	//search employee by two option which are name or ic
	void searchEmployee(string searchContent, SearchEmployee searchOption);

	void displayEmployee();	//display all the employee
	void displayEmployee(vector<int> id);	//display specific employee
};

enum class SearchEmployee	
{
	//this is the option for searching employee
	EMPLOYEENAME,
	EMPLOYEEIC
};

#endif
