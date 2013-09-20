/*
** TRANSACT.C - This is the ODBC sample driver code for
** processing transactions.
**
**	This code is furnished on an as-is basis as part of the ODBC SDK and is
**	intended for example purposes only.
**
*/

//	-	-	-	-	-	-	-	-	-

#include "sample.h"

//	-	-	-	-	-	-	-	-	-

//	SQLC transaction control functions.

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLTransact(
	HENV	henv,
	HDBC	hdbc,
	UWORD	fType)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-
