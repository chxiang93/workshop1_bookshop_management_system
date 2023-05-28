#include <string>
using namespace std;
#include <mysql/jdbc.h>
using namespace sql;
#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

class DatabaseConnection
{
private:
	Connection* connection;

public:
	ResultSet* res;	//store the result of query
	PreparedStatement* pstmt;	//store sql command

	DatabaseConnection();
	~DatabaseConnection();
	PreparedStatement* prepStmt(string query);
};

#endif 
