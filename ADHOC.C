/*********************************************************************
/*  ad_hoc_query - Interactive AD HOC Query example.                 *
/*********************************************************************/
 
#if OS2ODBC
#include 	"win2os2.h"
#endif

#include <sql.h>
#include <sqlext.h>
#include <string.h>
#include <stdlib.h>

#define TRUE		1
#define FALSE		0

#define MAX_STR		255
#define MAXLINE		1000
#define MAX_NAME_LEN	50
#define MAXCOLS		100

main(argc, argv)
int argc;
char *argv[];
{

HENV	henv;
HDBC	hdbc;
HSTMT	hstmt;

int	len;			/* length of returned line         */
char	line[MAXLINE];		/* returned line                   */
UCHAR	dsn[MAX_STR];		/* Data Source Name                */
UCHAR	pwd[MAX_STR];		/* password of user from odbc.ini  */
UCHAR	uid[MAX_STR];		/* name of user from odbc.ini      */
UCHAR	sqlstr[MAX_STR];	/* SQL String used for testing     */

RETCODE	rc;
int	i;
UCHAR	errmsg[256];
UCHAR	colname[32];
SWORD	coltype;
SWORD	colnamelen;
SWORD	nullable;
UDWORD	collen[MAXCOLS];
SWORD	scale;
SDWORD	outlen[MAXCOLS];
UCHAR	*data[MAXCOLS];
SWORD	nresultcols;
SDWORD	rowcount;
UCHAR	db[MAX_NAME_LEN + 1];
SWORD	dblen;

/*******************************************************************
/*  Get ODBC data source name; prompt for it if necessary          *
/*  Program assumes that all necessary data source information     *
/*  is in .odbc.ini file, and will prompt user for password.       *
/*  (Not that Informix driver does not require password, and       *
/*  prompt can be ingored.)                                        *
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
/*  Get AD-HOC SQL query from user.                                *
/*******************************************************************/

printf("Enter SQL string: ");
#ifdef OS2ODBC
fflush(stdout);
#endif
len = getline(line, MAXLINE);
strcpy((char *) sqlstr, line);

/*******************************************************************
/*  Allocate environment and connection handles                    *
/*  Connect to the data source                                     *
/*  Allocate a statement handle                                    *
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
if (rc != SQL_SUCCESS & rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, hdbc, SQL_NULL_HSTMT));

printf("\nODBC connection to %s successful.\n\n",db);

rc = SQLAllocStmt(hdbc, &hstmt);    
if (rc != SQL_SUCCESS & rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, hdbc, SQL_NULL_HSTMT));

/*******************************************************************
/*  Execute the SQL statement                                      *
/*******************************************************************/

rc = SQLExecDirect(hstmt, sqlstr, SQL_NTS);
if (rc != SQL_SUCCESS)
	return(print_err(henv, hdbc, hstmt));

/*******************************************************************
/*  See what kind of statement it was.  If there are no result     *
/*  columns, the statement is not a SELECT statement. If the numbe * 
/*  of affected rows is greater than 0, the statement was probably *
/*  an UPDATE, INSERT or DELETE statement, so print thhe number of *
/*  affected rows.  If the number of affected rows is 0, the       *
/*  statement is probably a DDL statement, so print that the       *
/*  operation was successful and commit it.                        *
/*******************************************************************/

rc = SQLNumResultCols(hstmt, &nresultcols);
if (nresultcols == 0)
	{
	SQLRowCount(hstmt, &rowcount);
	if (rowcount > 0)
		printf("%ld rows affected.\n", rowcount);
	else
		printf("Operation was successful.\n");
	SQLTransact(henv, hdbc, SQL_COMMIT);
	}

/*******************************************************************
/*  Otherwise, display the column names of the result set and use  *
/*  the display_size() function to compute the length needed by    *
/*  each data type.  Next, bind the columns and specify all        *
/*  data will be converted to char.  Finally, fetch and print each *
/*  row, printing truncation messages as necessary.                *
/*******************************************************************/

else
	{
	for (i = 0; i < nresultcols; i++)
		{
		SQLDescribeCol(hstmt, i+1, colname, (SWORD) sizeof(colname),
			&colnamelen, &coltype, &collen[i], &scale, &nullable);
		collen[i] = display_size(coltype, collen[i], colname);
		data[i] = (UCHAR *) malloc(collen[i] + 1);
		printf("%*.*s ",collen[i], collen[i], colname);
		SQLBindCol(hstmt, i+1, SQL_C_CHAR, data[i], collen[i]+1, 
			&outlen[i]);
		}
	printf("\n");

	while (TRUE)
		{
		rc = SQLFetch(hstmt);
		if (rc == SQL_NO_DATA_FOUND) break;
		if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO)
			{
			errmsg[0] = '\0';
			for (i = 0; i < nresultcols; i++)
				{
				if (outlen[i] == SQL_NULL_DATA)
					strcpy((char *)data[i], "NULL");
				else
					if (outlen[i] > collen[i])
						{
						sprintf((char *) &errmsg[strlen((char *) errmsg)],
							"%d chars truncated, col %d\n",
							outlen[i] - collen[i] + 1,
							i + 1);
						}                  /* end if outlen ...               */
					printf("%*.*s ", collen[i], collen[i], data[i]);
				}                      /* end for loop                    */
			printf("\n%s", errmsg);  /* display the error message here  */
			}                        /* end if rc == SQL_SUCCESS ...    */
		else
			return(print_err(henv,hdbc,hstmt));
		}                          /* end while TRUE                  */
	}                            /* end else nresultcols != 0       */
 
/*******************************************************************
/*  Free the data buffers                                          *
/*******************************************************************/

for (i = 0; i < nresultcols; i++)
	{
	free(data[i]);
	}

rc = SQLFreeStmt(hstmt, SQL_DROP);  /* free the statement handle  */
if (rc != SQL_SUCCESS & rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, hdbc, hstmt));

rc = SQLDisconnect(hdbc);      /* Disconnect from the data source */
if (rc != SQL_SUCCESS & rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, hdbc, SQL_NULL_HSTMT));
 
SQLFreeConnect(hdbc);          /* Free the connection handle      */
if (rc != SQL_SUCCESS & rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, hdbc, SQL_NULL_HSTMT));
 
SQLFreeEnv(henv);              /* Free the environment handle     */
if (rc != SQL_SUCCESS & rc != SQL_SUCCESS_WITH_INFO)
	return(print_err(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT));
printf("\nODBC connection closed.\n");
}
