/*
** INFO.C - This is the ODBC sample driver code for
** executing information functions.
**
**	This code is furnished on an as-is basis as part of the ODBC SDK and is
**	intended for example purposes only.
**
*/

//	-	-	-	-	-	-	-	-	-

#include <memory.h>
#include "sample.h"

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLGetInfo(
	HDBC      hdbc,
	UWORD     fInfoType,
	PTR       rgbInfoValue,
	SWORD     cbInfoValueMax,
	SWORD FAR *pcbInfoValue)
{
	if (fInfoType == SQL_MAX_USER_NAME_LEN)
		*(SWORD FAR *)rgbInfoValue = 0;
	else if (fInfoType == SQL_DRIVER_ODBC_VER)
		lstrcpy (rgbInfoValue, SQL_SPEC_STRING);
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLGetTypeInfo(
	HSTMT	hstmt,
	SWORD	fSqlType)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLGetFunctions(
#if OS2ODBC
	HDBC	  hdbc,
#else
	LPDBC     lpdbc,
#endif
	UWORD     fFunction,
	UWORD FAR *pfExists)
{
	if (fFunction == SQL_API_ALL_FUNCTIONS)
	{
		int i;

		memset (pfExists, 0, sizeof(UWORD)*100);
		for (i = SQL_API_SQLALLOCCONNECT; i <= SQL_NUM_FUNCTIONS; i++)
			pfExists[i] = TRUE;
		for (i = SQL_EXT_API_START; i <= SQL_EXT_API_LAST; i++)
			pfExists[i] = TRUE;
	}
	else
		*pfExists = TRUE;
	return SQL_SUCCESS;
}
