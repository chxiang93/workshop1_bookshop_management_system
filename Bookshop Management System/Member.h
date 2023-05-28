#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;
#include "DatabaseConnection.h"

#ifndef MEMBER_H
#define MEMBER_H

class Member
{
	//these are the variable that will store the data from database 
private:
	int bookshopId;
	int memberId;
	string name{};
	string phone{};
	string address{};
	string email{};

	//these are declaration of methods used in this class
public:
	Member();
	void editMember();			//edit the member information
	void addMember();			//add member
	void deleteMember();			//delete member
	void updateMember();			//update member
	void setBookshopId(int id);	//set bookshop id
	bool inputValidation(int input);		//check if the input from user is number
	bool inputValidation(int lower, int upper, int input);	//check if the input from user is valid in some range
	bool emailValidation(string email);	//check is the email enter is valid

	bool checkMemberId(int id);		//check if the member exist
	void searchMember();				//search member
	void searchMember(string name);	//search member by name

	void displayMember();			//display all the member
	void displayMember(vector<int> id);	//display specific member
};

#endif
