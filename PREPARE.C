/*
** PREPARE.C - This is the ODBC sample driver code for
** preparing SQL Commands and other functions prior to execution.
**
**	This code is furnished on an as-is basis as part of the ODBC SDK and is
**	intended for example purposes only.
**
*/

//	-	-	-	-	-	-	-	-	-

#include "sample.h"

//	-	-	-	-	-	-	-	-	-

//	Allocate a SQL statement

RETCODE SQL_API SQLAllocStmt(
	HDBC	  hdbc,
	HSTMT FAR *phstmt)
{
	HGLOBAL	hstmt;

    hstmt = GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof (STMT));
	if (!hstmt || (*phstmt = (HSTMT)GlobalLock (hstmt)) == SQL_NULL_HSTMT)
	{
		GlobalFree (hstmt);	//	Free it if lock fails
		return SQL_ERROR;
	}
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLFreeStmt(
	HSTMT	  hstmt,
	UWORD	  fOption)
{
	if (fOption == SQL_DROP)
	{
		GlobalUnlock (GlobalPtrHandle((LPSTMT)hstmt));
		GlobalFree (GlobalPtrHandle((LPSTMT)hstmt));
	}
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Perform a Prepare on the SQL statement

RETCODE SQL_API SQLPrepare(
	HSTMT	  hstmt,
	UCHAR FAR *szSqlStr,
	SDWORD	  cbSqlStr)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Bind parameters on a statement handle

RETCODE SQL_API SQLBindParameter(
	HSTMT	   hstmt,
	UWORD	   ipar,
	SWORD	   fParamType,
	SWORD	   fCType,
	SWORD	   fSqlType,
	UDWORD	   cbColDef,
	SWORD	   ibScale,
	PTR 	   rgbValue,
	SDWORD	   cbValueMax,
	SDWORD FAR *pcbValue)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Returns the description of a parameter marker.

RETCODE SQL_API SQLDescribeParam(
	HSTMT	   hstmt,
	UWORD	   ipar,
	SWORD  FAR *pfSqlType,
	UDWORD FAR *pcbColDef,
	SWORD  FAR *pibScale,
	SWORD  FAR *pfNullable)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Sets multiple values (arrays) for the set of parameter markers.

RETCODE SQL_API SQLParamOptions(
	HSTMT	   hstmt,
	UDWORD	   crow,
	UDWORD FAR *pirow)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Returns the number of parameter markers.

RETCODE SQL_API SQLNumParams(
	HSTMT	   hstmt,
	SWORD  FAR *pcpar)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Sets options that control the behavior of cursors.

RETCODE SQL_API SQLSetScrollOptions(
	HSTMT	   hstmt,
	UWORD	   fConcurrency,
	SDWORD	crowKeyset,
	UWORD	   crowRowset)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Set the cursor name on a statement handle

RETCODE SQL_API SQLSetCursorName(
	HSTMT	  hstmt,
	UCHAR FAR *szCursor,
	SWORD	  cbCursor)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Return the cursor name for a statement handle

RETCODE SQL_API SQLGetCursorName(
	HSTMT	  hstmt,
	UCHAR FAR *szCursor,
	SWORD	  cbCursorMax,
	SWORD FAR *pcbCursor)
{
	return SQL_SUCCESS;
}
