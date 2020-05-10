//#import "c:\program files\common files\system\ado\msado15.dll" rename ("EOF","adoEOF") no_namespace
#pragma once
#include <string>
#include <cstring>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;

class CSql
{
public:
	CSql();
	~CSql();
	string showSQLError(unsigned int handleType, const SQLHANDLE& handle);
	int CreateConnection(string ConnectionString);
	int DestroyConnection();
	string ProcessQuery(string pSql);

private:
	string _sqlQuery;
	string _sqlConnection;
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;
};

//"DRIVER={SQL Server}; SERVER=localhost, 1433; DATABASE=myDB; UID=myID; PWD=myPW;"
int CSql::CreateConnection(string ConnectionString) 
{
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle))
		return EXIT_FAILURE;// Allocates the environment

	if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))		
		return EXIT_FAILURE;// Sets attributes that govern aspects of environments

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle))	
		return EXIT_FAILURE;// Allocates the connection

	if (SQL_SUCCESS != SQLSetConnectAttr(SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
		return EXIT_FAILURE;// Sets attributes that govern aspects of connections
		

	SQLWCHAR retConString[1024]; // Conection string
	switch (SQLDriverConnect(SQLConnectionHandle, NULL, (SQLWCHAR*)ConnectionString.c_str(), SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT))
	{
		// Establishes connections to a driver and a data source
		case SQL_SUCCESS:
			break;
		case SQL_SUCCESS_WITH_INFO:
			break;
		case SQL_NO_DATA_FOUND:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		case SQL_INVALID_HANDLE:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		case SQL_ERROR:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		default:
			break;
	}
	//TODO: Pas définir a ce moment
	if (retCode == true)
	{
		_sqlConnection = string((const char *)retConString);
	}
	return EXIT_SUCCESS;
}

int CSql::DestroyConnection()
{
	//TODO: Pas définir a ce moment
	return 0;
}

string CSql::ProcessQuery(string pSql)
{

	if (retCode == -1)
		return NULL;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle))
		// Allocates the statement
		return NULL;

	if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLWCHAR*)pSql.c_str(), SQL_NTS)) 
	{
		// Executes a preparable statement
		showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
		return NULL;
	}
	else 
	{
		char name[256];
		int age;
		while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) 
		{
			// Fetches the next rowset of data from the result
			SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &name, sizeof(name), NULL);
			SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &age, sizeof(age), NULL);
			// Retrieves data for a single column in the result set
			cout << name << " " << age << endl;
		}
	}
	//TODO: Pas définir a ce moment
	_sqlQuery = pSql;

	return NULL;
}

CSql::CSql()
{
	//TODO: Pas définir a ce moment mais je le definissais pour me mettre en memoire.
}

CSql::~CSql()
{
	//TODO: Pas définir a ce moment mais je le definissais pour me mettre en memoire.
}

string CSql::showSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
	SQLWCHAR SQLState[1024];
	SQLWCHAR message[1024];
	string retMsg;

	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
	{ 
		retMsg = string("SQL driver message: ") + string((char *)message) + string("\nSQL state: ") + string((char*)((SQLCHAR)SQLState)) + string(".");
		// Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
		return retMsg;
	}
	else
	{
		return NULL;
	}		
}