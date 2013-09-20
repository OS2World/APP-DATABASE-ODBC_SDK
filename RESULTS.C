/*
** RESULTS.C - This is the ODBC sample driver code for
** returning results and information about results.
**
**	This code is furnished on an as-is basis as part of the ODBC SDK and is
**	intended for example purposes only.
**
*/

//	-	-	-	-	-	-	-	-	-

#include "sample.h"

//	-	-	-	-	-	-	-	-	-

//	This returns the number of columns associated with the database
//	attached to "hstmt".

RETCODE SQL_API SQLNumResultCols(
	HSTMT	  hstmt,
	SWORD FAR *pccol)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Return information about the database column the user wants
//	information about.

RETCODE SQL_API SQLDescribeCol(
	HSTMT	   hstmt,
	UWORD	   icol,
	UCHAR  FAR *szColName,
	SWORD	   cbColNameMax,
	SWORD  FAR *pcbColName,
	SWORD  FAR *pfSqlType,
	UDWORD FAR *pcbColDef,
	SWORD  FAR *pibScale,
	SWORD  FAR *pfNullable)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Returns result column descriptor information for a result set.

RETCODE SQL_API SQLColAttributes(
	HSTMT	   hstmt,
	UWORD	   icol,
	UWORD	   fDescType,
	PTR 	   rgbDesc,
	SWORD	   cbDescMax,
	SWORD  FAR *pcbDesc,
	SDWORD FAR *pfDesc)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Associate a user-supplied buffer with a database column.

RETCODE SQL_API SQLBindCol(
	HSTMT	   hstmt,
	UWORD	   icol,
	SWORD	   fCType,
	PTR 	   rgbValue,
	SDWORD	   cbValueMax,
	SDWORD FAR *pcbValue)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Returns data for bound columns in the current row ("hstmt->iCursor"),
//	advances the cursor.

RETCODE SQL_API SQLFetch(
	HSTMT	hstmt)
{
	return SQL_SUCCESS;
}

//	Returns result data for a single column in the current row.

RETCODE SQL_API SQLGetData(
	HSTMT	   hstmt,
	UWORD	   icol,
	SWORD	   fCType,
	PTR 	   rgbValue,
	SDWORD	   cbValueMax,
	SDWORD FAR *pcbValue)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	This determines whether there are more results sets available for
//	the "hstmt".

RETCODE SQL_API SQLMoreResults(
	HSTMT	hstmt)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	This returns the number of rows associated with the database
//	attached to "hstmt".

RETCODE SQL_API SQLRowCount(
	HSTMT	   hstmt,
	SDWORD FAR *pcrow)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	This positions the cursor within a block of data.

RETCODE SQL_API SQLSetPos(
	HSTMT	hstmt,
	UWORD	irow,
	UWORD	fOption,
	UWORD	fLock)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	This fetchs a block of data (rowset).

RETCODE SQL_API SQLExtendedFetch(
	HSTMT	   hstmt,
	UWORD	   fFetchType,
	SDWORD	   irow,
	UDWORD FAR *pcrow,
	UWORD  FAR *rgfRowStatus)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Returns the next SQL error information.

RETCODE SQL_API SQLError(
#if OS2ODBC
	HENV	   henv,
	HDBC	   hdbc,
#else
	LPENV	   lpenv,
	LPDBC	   lpdbc,
#endif
	HSTMT	   hstmt,
	UCHAR  FAR *szSqlState,
	SDWORD FAR *pfNativeError,
	UCHAR  FAR *szErrorMsg,
	SWORD	   cbErrorMsgMax,
	SWORD  FAR *pcbErrorMsg)
{
	return SQL_NO_DATA_FOUND;
}

//	-	-	-	-	-	-	-	-	-
