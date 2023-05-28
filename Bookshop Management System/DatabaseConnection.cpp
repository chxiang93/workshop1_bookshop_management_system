#include "DatabaseConnection.h"

DatabaseConnection::DatabaseConnection()
{
	try
	{
		mysql::MySQL_Driver* driver = mysql::get_mysql_driver_instance();
		connection = driver->connect("tcp://127.0.0.1:3306", "root", "");

		connection->setSchema("bookshop");
	}
	catch (SQLException& e)
	{
		cout << "Error!! Failed to connect to database!!!\n";
	}
	catch (...)
	{
		cout << "\n\nError!! Something unexpected occur!!\n";
	}

	pstmt = nullptr;
	res = nullptr;
}

DatabaseConnection::~DatabaseConnection()
{
	connection->close();
	delete connection;
	delete res;
	delete pstmt;
}

PreparedStatement* DatabaseConnection::prepStmt(string query)
{
	return connection->prepareStatement(query);
}