/*
** OPTIONS.C - This is the ODBC sample driver code for
** executing Set/GetConnect/StmtOption.
**
**	This code is furnished on an as-is basis as part of the ODBC SDK and is
**	intended for example purposes only.
**
*/

//	-	-	-	-	-	-	-	-	-

#include "sample.h"

RETCODE SQL_API SQLSetConnectOption(
	HDBC	hdbc,
	UWORD	fOption,
	UDWORD	vParam)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLSetStmtOption(
	HSTMT	hstmt,
	UWORD	fOption,
	UDWORD	vParam)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLGetConnectOption(
	HDBC	hdbc,
	UWORD	fOption,
	PTR     pvParam)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLGetStmtOption(
	HSTMT	hstmt,
	UWORD	fOption,
	PTR     pvParam)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-
