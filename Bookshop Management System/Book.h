#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;
#include "DatabaseConnection.h"

#ifndef BOOK_H
#define BOOK_H

enum class SearchBook;	

class Book
{
	//these are the variable that will store the data from database 
private:
	int bookshopId{};
	int bookId{};
	string book_name{};
	string author_name{};
	double cost{};
	double price{};
	string publisher_name{};
	int stock{};
	string category{};

	//these are declaration of methods used in this class
public:
	Book();
	void editBook();		//edit the book information
	void addBook();		//add book
	void deleteBook();	//delete book
	void updateBook();	//update book
	void setBookshopId(int id);	//set bookshop id
	bool inputValidation(int input);		//check if the input from user is number
	bool inputValidation(int lower, int upper, int input);	//check if the input from user is valid in some range

	bool checkBookId(int id);	//check if the book exist
	void searchBook();			//search book
	//search book by four option which are book name, author name,
	//publisher name and category
	void searchBook(string name, SearchBook searchOption);

	void displayBook();	//display all the book
	void displayBook(vector<int> id);	//display specific book
};

enum class SearchBook
{
	//this is the option for searching book
	BOOKNAME,
	AUTHORNAME,
	PUBLISHERNAME,
	CATEGORY
};
#endif