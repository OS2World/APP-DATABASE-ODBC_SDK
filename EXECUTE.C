/*
** EXECUTE.C - This is the ODBC sample driver code for
** executing SQL Commands.
**
**	This code is furnished on an as-is basis as part of the ODBC SDK and is
**	intended for example purposes only.
**
*/

//	-	-	-	-	-	-	-	-	-

#include "sample.h"

//	-	-	-	-	-	-	-	-	-

//	Execute a prepared SQL statement

RETCODE SQL_API SQLExecute(
	HSTMT	hstmt)		// statement to execute.
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Performs the equivalent of SQLPrepare, followed by SQLExecute.

RETCODE SQL_API SQLExecDirect(
	HSTMT     hstmt,
	UCHAR FAR *szSqlStr,
	SDWORD    cbSqlStr)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Returns the SQL string as modified by the driver.

RETCODE SQL_API SQLNativeSql(
#if OS2ODBC
	HDBC	   hdbc,
#else
	LPDBC      lpdbc,
#endif
	UCHAR FAR *szSqlStrIn,
	SDWORD     cbSqlStrIn,
	UCHAR FAR *szSqlStr,
	SDWORD     cbSqlStrMax,
	SDWORD FAR *pcbSqlStr)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Supplies parameter data at execution time.	Used in conjuction with
//	SQLPutData.

RETCODE SQL_API SQLParamData(
	HSTMT	hstmt,
	PTR FAR *prgbValue)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Supplies parameter data at execution time.	Used in conjunction with
//	SQLParamData.

RETCODE SQL_API SQLPutData(
	HSTMT   hstmt,
	PTR     rgbValue,
	SDWORD  cbValue)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLCancel(
	HSTMT	hstmt)	// Statement to cancel.
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-
