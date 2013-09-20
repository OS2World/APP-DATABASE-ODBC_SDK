/*********************************************************************
/*  staticsql - Static SQL example.                                  *
/*********************************************************************/

#if OS2ODBC
#include 	"win2os2.h"
#endif

#include <sql.h>
#include <sqlext.h>
#include <string.h>

#ifndef NULL
#define NULL 0
#endif

#define TRUE		1
#define FALSE		0

#define MAX_STR		255
#define MAXLINE		1000
#define MAX_NAME_LEN	50
#define MAX_STMT_LEN	100


main(argc, argv)
int argc;
char *argv[];
{

HENV	henv;		/* Handle - Environment		*/
HDBC	hdbc;		/* Handle - Database connect	*/
HSTMT	hstmt;		/* Handle - SQL statement	*/

int	len;		/* length of returned line		*/
char	line[MAXLINE];	/* returned line			*/
UCHAR	uid[MAX_STR];	/* name of user from odbc.ini		*/
UCHAR	dsn[MAX_STR];	/* Data Source Name			*/
UCHAR	pwd[MAX_STR];	/* password of user from odbc.ini	*/
   
RETCODE	rc;
SDWORD	id;
UCHAR	name[MAX_NAME_LEN + 1];
SDWORD	namelen;
UCHAR	create[MAX_NAME_LEN];
UCHAR	insert[MAX_NAME_LEN];
UCHAR	select[MAX_NAME_LEN];
UCHAR	drop[MAX_NAME_LEN];

UCHAR	db[MAX_NAME_LEN + 1];
SWORD	dblen;

/*******************************************************************
/*  Get ODBC data source name; prompt for it if necessary          *
/*  Program assumes that all necessary data source information     *
/*  is in .odbc.ini file, and will prompt user for password.       *
/*  (Note that Informix driver does not require password, and      *
/*   prompt can be ignored.                                        *
/*******************************************************************/

if (argc == 1)                 /* if no Data Source Name entered  */ 
	{
	printf("Enter Data Source Name: ");
#ifdef OS2ODBC
fflush(stdout);
#endif
	len = getline(line, MAXLINE);
	strcpy((char *) dsn, line);
	} 
else 
	{
	strcpy((char *) dsn, argv[1]);
	}
     
printf("Enter password: ");
#ifdef OS2ODBC
fflush(stdout);
#endif
len = getline(line, MAXLINE);
strcpy((char *) pwd, line);

/*******************************************************************
/*  EXEC SQL CONNECT TO :server USER :uid USING :pwd               *
/*  Allocate environment handle.                                   *
/*  Allocate connection handle.                                    *
/*  Connect to the data source.                                    *
/*  Allocate a statement handle.                                   *
/*******************************************************************/

rc = SQLAllocEnv(&henv);
if (rc != SQL_SUCCESS & rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(SQL_NULL_HENV, SQL_NULL_HDBC, SQL_NULL_HSTMT));

rc = SQLAllocConnect(henv, &hdbc);
if (rc != SQL_SUCCESS & rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT));

rc = SQLConnect(hdbc, dsn, SQL_NTS, uid, SQL_NTS, pwd, SQL_NTS);
if (rc != SQL_SUCCESS & rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, hdbc, SQL_NULL_HSTMT));

rc = SQLGetInfo(hdbc, SQL_DBMS_NAME, &db, (SWORD) sizeof(db), &dblen);
if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, hdbc, SQL_NULL_HSTMT));

printf("\nODBC connection to %s successful.\n",db);

rc = SQLAllocStmt(hdbc, &hstmt);
if (rc != SQL_SUCCESS & rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, hdbc, SQL_NULL_HSTMT));

/*******************************************************************
/*  EXEC SQL CREATE TABLE NAMEID (ID integer, NAME varchar(50));   *
/*  Execute the SQL statement.                                     *
/*******************************************************************/

strcpy((char *) create, "CREATE TABLE NAMEID (ID INTEGER, NAME VARCHAR(50))");

rc = SQLExecDirect(hstmt, create, SQL_NTS);
if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, hdbc, hstmt));
   
/*******************************************************************
/*  EXEC SQL COMMIT WORK;                                          *
/*  Commit the table creation.                                     *
/*  Note that the default transaction mode for drivers that support*
/*  SQLSetConnectOtion is auto-commit and SQLTransact has no effect*
/*******************************************************************/

rc =  SQLTransact(henv, hdbc, SQL_COMMIT);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));
printf("Table NAMEID created.\n");

/*******************************************************************
/*  EXEC SQL INSERT INTO NAMEID VALUES ( :id, :name );             *
/*  Show the use of the SQLPrepare/SQLExecute method;              *
/*  Prepare the insertion and bind parameters.                     *
/*  Assign parameter values.                                       *
/*  Execute the insertion.                                         *
/*******************************************************************/

strcpy((char *) insert, "INSERT INTO NAMEID VALUES (?, ?)");

rc = SQLPrepare(hstmt, insert, SQL_NTS);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

rc = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
			0, 0, &id, 0, NULL);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

rc = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
			MAX_NAME_LEN, 0, name, 0, NULL);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

id=500; strcpy((char *) name, "Babbage"); 
	if (SQLExecute(hstmt) != SQL_SUCCESS)
		return(print_err(henv, hdbc, hstmt));

id=501; strcpy((char *) name, "Boole"); 
	if (SQLExecute(hstmt) != SQL_SUCCESS)
		return(print_err(henv, hdbc, hstmt));
id=502; strcpy((char *) name, "Knuth"); 
	if (SQLExecute(hstmt) != SQL_SUCCESS)
		return(print_err(henv, hdbc, hstmt));
id=503; strcpy((char *) name, "Leibniz"); 
	if (SQLExecute(hstmt) != SQL_SUCCESS)
		return(print_err(henv, hdbc, hstmt));
id=504; strcpy((char *) name, "Pascal"); 
	if (SQLExecute(hstmt) != SQL_SUCCESS)
		return(print_err(henv, hdbc, hstmt));
id=505; strcpy((char *) name, "Turing"); 
	if (SQLExecute(hstmt) != SQL_SUCCESS)
		return(print_err(henv, hdbc, hstmt));
id=506; strcpy((char *) name, "Von Neumann"); 
	if (SQLExecute(hstmt) != SQL_SUCCESS)
		return(print_err(henv, hdbc, hstmt));
printf("7 rows inserted into NAMEID.\n\n");

SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

/*******************************************************************
/*  EXEC SQL COMMIT WORK;                                          *
/*  Commit the insertion.                                          *
/*******************************************************************/

rc =  SQLTransact(henv, hdbc, SQL_COMMIT);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

/*******************************************************************
/*  EXEC SQL DECLARE c1 CURSOR FOR SELECT ID, NAME FROM NAMEID;    *
/*  EXEC SQL OPEN c1;                                              *
/*  Show the use of the SQLExecDirect method.                      *
/*  Execute the selection.                                         *
/*  Note that the application does not declare a cursor.           *
/*******************************************************************/

strcpy((char *) select, "SELECT ID, NAME FROM NAMEID");

rc =  SQLExecDirect(hstmt, select, SQL_NTS);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

/*******************************************************************
/*  EXEC SQL FETCH c1 INTO :id, :name;                             *
/*  Bind the columns of the result set with SQLBindCol.            *
/*  Fetch the first row.                                           *
/*******************************************************************/

rc = SQLBindCol(hstmt, 1, SQL_C_SLONG, &id, 0, NULL);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

rc = SQLBindCol(hstmt, 2, SQL_C_CHAR, name, (SDWORD) sizeof(name), &namelen);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

while (TRUE) 
	{
	rc = SQLFetch(hstmt);
	if (rc == SQL_NO_DATA_FOUND) break;
	if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
		return(print_err(henv, hdbc, hstmt));
	printf("Fetched row: ID = %d; Name = %s.\n",id, name);
	}

/*******************************************************************
/*  EXEC SQL COMMIT WORK;                                          *
/*  Commit the transaction.                                        *
/*******************************************************************/

rc =  SQLTransact(henv, hdbc, SQL_COMMIT);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

/*******************************************************************
/*  EXEC SQL CLOSE c1;                                             *
/*  Free the statement handle.                                     *
/*******************************************************************/

SQLFreeStmt(hstmt, SQL_CLOSE);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

/*******************************************************************
/*  EXEC SQL DROP TABLE NAMEID ;                                   *
/*  Execute the SQL statement.                                     *
/*******************************************************************/

strcpy((char *) drop, "DROP TABLE NAMEID");

rc = SQLExecDirect(hstmt, drop, SQL_NTS);
if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, hdbc, hstmt));

/*******************************************************************
/*  EXEC SQL COMMIT WORK;                                          *
/*  Commit the table deletion.                                     *
/*  Note that the default transaction mode for drivers that support*
/*  SQLSetConnectOtion is auto-commit and SQLTransact has no effect*
/*******************************************************************/

rc =  SQLTransact(henv, hdbc, SQL_COMMIT);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));
printf("\nTable NAMEID dropped.\n");

/*******************************************************************
/*  Free the statement handle.                                     *
/*******************************************************************/

rc = SQLFreeStmt(hstmt, SQL_DROP);  /* free the statement handle  */
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

/*******************************************************************
/*  Disconnect from the data source.                               *
/*  Free the connection handle.                                    *
/*  Free the environment handle.                                   *
/*******************************************************************/

rc = SQLDisconnect(hdbc);      /* Disconnect from the data source */
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, SQL_NULL_HSTMT));
 
rc = SQLFreeConnect(hdbc);     /* Free the connection handle      */
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, SQL_NULL_HSTMT));
 
rc = SQLFreeEnv(henv);         /* Free the environment handle     */
if (rc != SQL_SUCCESS)
	return(print_err(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT));
printf("ODBC connection closed.\n");
}
