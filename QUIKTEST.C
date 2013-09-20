/*
  PROGRAM: quiktest.c
        This code is furnished on an as-is basis as part of the ODBC SDK and is
        intended for example purposes only.

  PURPOSE: This is a Quick Test of the basic functionality of an ODBC driver.
        This 2.0 version of the Quick Test does not check the driver version
        before calling 2.0 functions.

  FUNCTIONS:
	     QuickTest() - performs the quick test focusing on basic functionalities.
*/
#if !defined(OS2ODBC)
#pragma warning (disable : 4703)
#else
#include "win2os2.h"
#include "os2util.h"
#endif

#define QUIKTEST

#include "autotest.h"
#include <string.h>
#include "sql.h"
#include "sqlext.h"

#define MAX_QUERY_SIZE 2048
#define MAX_BIND_ELEMENTS 100
#define MAX_BIND_ARRAY_ELEMENT MAX_QUERY_SIZE / MAX_BIND_ELEMENTS
#define MAX_STRING_SIZE 100
#define MAX_PARAM_SIZE 129
#define MAX_DATA_SIZE 45
#define MAX_COLUMN_LIST_SIZE 200
#define MAX_COLUMN_NAME_SIZE 50
#define MAX_INSERT_VALUES_SIZE 200
#define MAX_TYPES_SUPPORTED 25
#define MAX_TYPE_NAME 129
#define MAX_FIELD 20
#define MAX_PREFIX 129
#define MAX_SUFFIX 129
#define MAX_TABLE_NAME 30
#define MAX_NUM_BUFFER 30
#define MAX_ERROR_SIZE 200
#define MAX_ROW_ITEM_LIMIT 4
#define PREFIX_SIZE 3
#define IGNORED 999
#define TEST_CONNECT_OPTION 15
#define TEST_STMT_OPTION 300
#define SCALEDIFF 4
#define ABORT -1
#define TYPE_PORTION 4

/* used to store information from GetTypeInfo for table creation */
typedef struct FieldInfo {
		int iField;
		char szType[MAX_TYPE_NAME];
		char szFieldName[MAX_FIELD];
		SWORD  wSQLType;
		char szParams[MAX_PARAM_SIZE];
		char szLength[MAX_FIELD];
		char szPrefix[MAX_PREFIX];
		char szSuffix[MAX_SUFFIX];
		SDWORD precision;
		SWORD scale;
		SWORD nullable;
		UDWORD length;
		int   fAutoUpdate;
		SWORD fSearchable;
		SDWORD fUnsigned;
		} FIELDINFO;

int FAR PASCAL ReturnCheck(RETCODE retExpected, RETCODE retReceived, LPSTR szFunction, LPSTR szFile, int iLine);
void FAR PASCAL qtDisplayError(LPSTR szFunction, LPSTR buf, LPSTR szFile, int iLine);
int FAR PASCAL FindError(LPSTR szFindState);
int FAR PASCAL qtestristr(LPCSTR szString, LPCSTR szFind);
LPSTR FAR PASCAL qtMakeData(int row, FIELDINFO FAR * rgField, LPSTR buf);
VOID WINAPI szWrite(LPSTR szStr, BOOL fNewLine);

#define RETCHECK(exp, rec, buf) ReturnCheck(exp, rec, buf, __FILE__, __LINE__)
#define DISPLAYERROR(func, string) qtDisplayError(func, string, __FILE__, __LINE__)

/* large buffers allocated as a global used for queries and other
returned information */
typedef struct  tagQtStruct {
	char    buf[MAX_STRING_SIZE];
	char    sz[MAX_QUERY_SIZE];
	char  szParamQuery[MAX_QUERY_SIZE];
	char    szParam[MAX_PARAM_SIZE];
	char    szDataItem[MAX_DATA_SIZE];
	char    szTableName[MAX_TABLE_NAME];
	char    szColNames[MAX_COLUMN_LIST_SIZE];
	char  szColName[MAX_COLUMN_NAME_SIZE];
	char    szValues[MAX_INSERT_VALUES_SIZE];
}       QTSTRUCT;

/* the storage used for data retreived using bind/fetch sequence 
and data for BindParameter */
/* only cb and one of the types will be used for each entry */
typedef struct tagDataStruct {
	SDWORD cb;
	char data[MAX_STRING_SIZE];
	SDWORD sdword;
	SWORD sword;
	SDOUBLE sdouble;
	SFLOAT sfloat;
	TIME_STRUCT time;
	DATE_STRUCT date;
	TIMESTAMP_STRUCT timestamp;
} DSTRUCT;

lpSERVERINFO lpSI;
static char vszFile[] = __FILE__;

/* these are globals so that the error functions can access them without
needing to have them passed to every error check */
	static HENV            henv;
	static HDBC            hdbc;
	static HSTMT           hstmt;


//**************************************************************************
//***************************  External Interfaces  ************************
//*  These functions are called by Gator to envoke the tests.              *
//**************************************************************************

BOOL CALLBACK AutoTestName(LPSTR szName, UINT FAR * cbTestCases)
{
	static char szTestName[] = "Quick Test";
	lstrcpy(szName, szTestName);
	*cbTestCases = 1;
	return TRUE;
}

//-----------------------------------------------------------------------
//      Function:               AutoTestDesc
//      Purpose:                        Provides gator with a list of tests which can be run.
//-----------------------------------------------------------------------
BOOL CALLBACK AutoTestDesc(UWORD iTest, LPSTR szName, LPSTR szDesc)
{
	switch(iTest)
	{
		case 1:
			if(szName)
				lstrcpy(szName,"QuickTest");
			if(szDesc)
				lstrcpy(szDesc,"QuickTest");
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------
//      Function:               AutoTestFunc
//-----------------------------------------------------------------------
void CALLBACK AutoTestFunc(lpSERVERINFO pTestSource)
{
//      extern int failed;

	RETCODE         returncode;
	SWORD           wType;
	SWORD           wNull;
	SWORD           cb;
	SWORD           wNum;
	UWORD           w;
	UWORD           i, j;
	UWORD           ind;
	UWORD           cTypes;
	SDWORD          sdw;
	UDWORD          dwLen;
	UDWORD          udw;
	BOOL            fFoundTable = 0;
	PTR             ptr;
	LPSTR                   pch;
	int             cTableName;
	int             fColNames;
	int             fConnectOption;
	int             cErrorCount = 0;
	int             cStateErrorCount = 0;
	int             cFieldName = 0;
	UWORD           fIndex = 0;
	char              szNum[MAX_NUM_BUFFER];   /* used with atoi and itoa conversions */
	int             fSearchable = FALSE;
	int             same;
	SDWORD          cColsSelected, cMaxTypeSize, cMaxRowSize = 0;
	int             cMaxConnections;
	UWORD           fLevel2; 
	long            fSupportedOpt;
	long            fSupportedCon;
	FIELDINFO FAR *rgFields = (FIELDINFO FAR *)AllocateMemory(sizeof(FIELDINFO) * MAX_TYPES_SUPPORTED);
	QTSTRUCT FAR *lpqt = (QTSTRUCT FAR *)AllocateMemory(sizeof (QTSTRUCT));
	DSTRUCT FAR *lpd = (DSTRUCT FAR *)AllocateMemory(sizeof (DSTRUCT) * MAX_TYPES_SUPPORTED);
	UWORD		fBindParam;
	UWORD		uDMVer;

	lpSI = pTestSource;
	lpSI->failed = 0;
	henv = NULL;
	hdbc = NULL;
	hstmt = NULL;

	if(*pTestSource->szValidServer0) { /* The signal for test to use it's
	own allocated handles, is if the a server name is passed in.  If it
	is not, the test should use the hdbc passed in */
		returncode = SQLAllocEnv(&henv);
		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLAllocEnv"))
			goto ErrorRet;

		returncode = SQLAllocConnect(henv, &hdbc);
		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLAllocConnect"))
			goto ErrorRet;

		returncode = SQLSetConnectOption(hdbc,SQL_ODBC_CURSORS,lpSI->vCursorLib);
		RETCHECK(SQL_SUCCESS,returncode,"SQLSetConnectOption");

		/* create a DriverConnect string */
		wsprintf(lpqt->sz, "dsn=%s;uid=%s;pwd=%s;",
			(LPSTR)pTestSource->szValidServer0,
			(LPSTR)pTestSource->szValidLogin0,
			(LPSTR)pTestSource->szValidPassword0);

		/* since SQL_DRIVER_COMPLETE is used, it is possible that the
			driver will bring up a dialog asking for more information. */
		returncode = SQLDriverConnect(hdbc, pTestSource->hwnd, lpqt->sz,
			SQL_NTS, lpqt->buf, MAX_STRING_SIZE, NULL, SQL_DRIVER_COMPLETE);

		if(returncode != SQL_SUCCESS && returncode !=
			SQL_SUCCESS_WITH_INFO) {
			/* in this situation, RETCHECK is used to display the error
			message.  RETCHECK will display that SQL_SUCCESS was expected
			although the error will only be displayed if neither SQL_SUCCESS
			or SQL_SUCCESS_WITH_INFO was returned */
			RETCHECK(SQL_SUCCESS, returncode, "SQLDriverConnect");
			goto ErrorRet;
		}

		returncode = SQLDisconnect(hdbc);
		RETCHECK(SQL_SUCCESS, returncode, "SQLDisconnect");

		returncode = SQLConnect(hdbc, pTestSource->szValidServer0, SQL_NTS,
			pTestSource->szValidLogin0, SQL_NTS,
			pTestSource->szValidPassword0, SQL_NTS);

		if(returncode != SQL_SUCCESS && returncode !=
			SQL_SUCCESS_WITH_INFO) {
			RETCHECK(SQL_SUCCESS, returncode, "SQLConnect");
			goto ErrorRet;
		}
	}
	else {
		henv = pTestSource->henv;
		hdbc = pTestSource->hdbc;
		
		returncode = SQLGetConnectOption(hdbc,SQL_ODBC_CURSORS,&dwLen);
		if(RETCHECK(SQL_SUCCESS, returncode, "SQLGetConnectOption"))
			lpSI->vCursorLib = dwLen;
	}



	returncode = SQLGetInfo(hdbc, SQL_ODBC_SQL_CONFORMANCE, &fIndex,
		sizeof (fIndex), &cb);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetInfo");

	if(fIndex != 0 && fIndex != 1 && fIndex != 2) {
		/* one of these two values should always be returned */
		DISPLAYERROR("SQLGetInfo", "SQL_ODBC_API_CONFORMANCE - invalid value");
	}
	
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLBINDPARAMETER, &fBindParam);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");
	
	returncode = SQLGetInfo(hdbc, SQL_ODBC_VER, &szNum, MAX_NUM_BUFFER, NULL);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetInfo");
	uDMVer = (UWORD)atoi(strtok(szNum, "."));

	/* Set some options then get them and make sure they come back the
		same. */

    returncode = SQLSetConnectOption(hdbc, SQL_ACCESS_MODE,
		SQL_MODE_READ_WRITE);
	if(returncode != SQL_SUCCESS)
		if(!FindError("S1C00"))
			RETCHECK(SQL_SUCCESS, returncode, "SQLSetConnectOption");

    returncode = SQLSetConnectOption(hdbc, SQL_AUTOCOMMIT, TRUE);
	if(returncode != SQL_SUCCESS)
		if(!FindError("S1C00"))
			RETCHECK(SQL_SUCCESS, returncode, "SQLSetConnectOption");

    returncode = SQLSetConnectOption(hdbc, SQL_LOGIN_TIMEOUT, TEST_CONNECT_OPTION);
	if(returncode != SQL_SUCCESS) {
		if(!FindError("S1C00"))
			RETCHECK(SQL_SUCCESS, returncode, "SQLSetConnectOption");
		else
			fConnectOption = FALSE;
	} else
		fConnectOption = TRUE;

	returncode = SQLGetConnectOption(hdbc, SQL_LOGIN_TIMEOUT, &sdw);
	if(returncode != SQL_SUCCESS) {
		if(!FindError("S1C00"))
			RETCHECK(SQL_SUCCESS, returncode, "SQLGetConnectOption");
	} else
		if(sdw != TEST_CONNECT_OPTION && fConnectOption)
			DISPLAYERROR("SQLGetConnectOption", "SQL_LOGIN_TIMEOUT returned incorrect value");

	returncode = SQLAllocStmt(hdbc, &hstmt);
	RETCHECK(SQL_SUCCESS, returncode, "SQLAllocStmt");

	returncode = SQLGetStmtOption(hstmt, SQL_MAX_LENGTH, &wNum);
	if(returncode != SQL_SUCCESS)
		if(!FindError("S1C00"))
			RETCHECK(SQL_SUCCESS, returncode, "SQLGetConnectOption");

	returncode = SQLSetStmtOption(hstmt, SQL_MAX_LENGTH, TEST_STMT_OPTION);
	if(returncode != SQL_SUCCESS)
		if(!FindError("S1C00")) {
			fConnectOption = FALSE;
			RETCHECK(SQL_SUCCESS, returncode, "SQLGetConnectOption");
		} else {
			fConnectOption = TRUE;
		}

	returncode = SQLGetStmtOption(hstmt, SQL_MAX_LENGTH, &dwLen);
	if(returncode != SQL_SUCCESS) {
		if(!FindError("S1C00"))
			RETCHECK(SQL_SUCCESS, returncode, "SQLGetConnectOption");
	} else
		if(dwLen != TEST_STMT_OPTION && returncode == SQL_SUCCESS && fConnectOption)
			DISPLAYERROR("SQLGetStmtOption", "incorrect SQL_MAX_LENGTH value returned");

/* Get the type information to use in a create statement for a table */

	returncode = SQLGetInfo(hdbc, SQL_MAX_COLUMN_NAME_LEN, &cFieldName,
		sizeof cFieldName, NULL);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetInfo");

	returncode = SQLGetInfo(hdbc, SQL_MAX_ROW_SIZE, &cMaxRowSize,
		sizeof cMaxRowSize, NULL);
	/* don't check for SQL_SUCCES here, it's a 2.0 function.  If it's successful
		That's great, if not then no maximum is assumed. */
	
	cMaxTypeSize = cMaxRowSize / MAX_TYPES_SUPPORTED;
	
/* TO DO: add checks in here for dos file types GetInfo SQL_QUALIFIER_NAME_SEPARATER */

	if(cFieldName > MAX_FIELD - 1)
		cFieldName = MAX_FIELD - 1;
	if(cFieldName < PREFIX_SIZE) {
		DISPLAYERROR("SQLGetInfo", "MAX_COLUMN_NAME_LEN is too small for autotest to run");
		goto ErrorRet;
	}

	returncode = SQLGetTypeInfo(hstmt, SQL_ALL_TYPES);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetTypeInfo");

	for(i = 0; i < MAX_TYPES_SUPPORTED; i++) {
		returncode = SQLFetch(hstmt);
		if(returncode != SQL_SUCCESS)
			break;

		*rgFields[i].szType = *rgFields[i].szLength = *rgFields[i].szParams = '\0';

		/* get type name */
		returncode = SQLGetData(hstmt, 1, SQL_C_CHAR,
			(LPSTR)rgFields[i].szType, MAX_TYPE_NAME, &sdw);

		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLGetData"))
			goto ErrorRet;

		wsprintf(rgFields[i].szFieldName,"c%02u", i);
		/* copy first portion of type name for easy reference */
		strncat(rgFields[i].szFieldName, rgFields[i].szType, TYPE_PORTION);


		/* change spaces in field name to '_' */
		while((pch = (LPSTR)strchr(rgFields[i].szFieldName, ' ')) != NULL)
			*pch = '_';

		/* get the SQLType */
	returncode = SQLGetData(hstmt, 2, SQL_C_DEFAULT,
			(SWORD FAR *)&rgFields[i].wSQLType, IGNORED,
			&sdw);
		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLGetData"))
			goto ErrorRet;

		/* Max length */
		returncode = SQLGetData(hstmt, 3, SQL_C_CHAR,
			(LPSTR)rgFields[i].szLength, MAX_FIELD, &sdw);
		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLGetData"))
			goto ErrorRet;

		/* limit the row size for those drivers that don't support a long
			enough row length for all the fields to be at the max length */
		switch(rgFields[i].wSQLType) {
			case SQL_CHAR:
			case SQL_VARCHAR:
			case SQL_VARBINARY:
			case SQL_BINARY:
			case SQL_LONGVARCHAR:
			case SQL_LONGVARBINARY:

				if(cMaxTypeSize) {
					if(atol (rgFields[i].szLength) > cMaxTypeSize)
						_ltoa(cMaxTypeSize, rgFields[i].szLength, 10);
				}
		}

		/* prefix */
		returncode = SQLGetData(hstmt, 4, SQL_C_CHAR,
			(LPSTR)rgFields[i].szPrefix, MAX_PREFIX, &sdw);
		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLGetData"))
			goto ErrorRet;

		/* suffix */
		returncode = SQLGetData(hstmt, 5, SQL_C_CHAR,
			(LPSTR)rgFields[i].szSuffix, MAX_SUFFIX, &sdw);
		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLGetData"))
			goto ErrorRet;

		/* create params */
		returncode = SQLGetData(hstmt, 6, SQL_C_CHAR,
			(LPSTR)rgFields[i].szParams, MAX_PARAM_SIZE, &sdw);
		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLGetData"))
			goto ErrorRet;

		/* nullable */
		returncode = SQLGetData(hstmt, 7, SQL_C_SHORT, &rgFields[i].nullable, IGNORED,
			&sdw);
		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLGetData"))
			goto ErrorRet;

		/* searchable */
		returncode = SQLGetData(hstmt, 9, SQL_C_SHORT, &rgFields[i].fSearchable, IGNORED,
			&sdw);
		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLGetData"))
			goto ErrorRet;

/* If a line below is compiled in, that type will not be used in creating
 the table.  (useful for when one specific type is causing problems in
 a particular test, this way the test can be run for all other types)
*/
			if(!lstrcmp(rgFields[i].szType, "LONG RAW")) {
				i--;
				continue;
			}
			if(!lstrcmp(rgFields[i].szType, "LONG VARCHAR")) {
				i--;
				continue;
			}
			if(!lstrcmp(rgFields[i].szType, "LONG VARCHAR FOR BIT DATA")) {
				i--;
				continue;
			}
	}
	cTypes = i;

	/* if the type name contains spaces, replace them with _ because
		most servers don't allow spaces in field names */

	RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

	returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	returncode = SQLGetInfo(hdbc, SQL_MAX_TABLE_NAME_LEN, &cTableName,
		sizeof (int), NULL);
	if(returncode != SQL_SUCCESS && returncode != SQL_SUCCESS_WITH_INFO)
		RETCHECK(SQL_SUCCESS, returncode, "SQLGetInfo");

	returncode = SQLGetInfo(hdbc, SQL_QUALIFIER_NAME_SEPARATOR, lpqt->buf,
		MAX_STRING_SIZE, NULL);
	if(returncode != SQL_SUCCESS && returncode != SQL_SUCCESS_WITH_INFO)
		RETCHECK(SQL_SUCCESS, returncode, "SQLGetInfo");

	if(0 == lstrcmp("\\", lpqt->buf))
		cTableName -= 4;
	if(cTableName > MAX_TABLE_NAME)
		cTableName = MAX_TABLE_NAME;
	wsprintf(lpqt->szTableName, "q%ld", hstmt);
	lpqt->szTableName[cTableName] = '\0';

	/* build create statement */
	/* the column names will be ctypname (where typename is the
		type name returned by the source in SQLGetTypeInfo) */

	*lpqt->sz = '\0';
	lstrcpy(lpqt->sz, "Create Table ");
	lstrcat(lpqt->sz, lpqt->szTableName);
	lstrcat(lpqt->sz, " (");
	for(i = 0; i < cTypes; i++) {
			char szParamType[50];
			*lpqt->szParam = '\0';

		/* if SQLGetTypeInfo returned params they need to be used in
			the create statement */
		
		if(rgFields[i].szParams == NULL || lstrlen(rgFields[i].szParams) == 0)
			*lpqt->szParam = '\0';
		else if(strchr(rgFields[i].szParams, ',') == NULL)
			wsprintf(lpqt->szParam, "(%s)", rgFields[i].szLength);
		else {
			lstrcpy(szNum, rgFields[i].szLength);
			wsprintf(lpqt->szParam, "(%s, %d)", rgFields[i].szLength,
				atoi(szNum) - SCALEDIFF);
		}

		lstrcpy(szParamType, rgFields[i].szType);
		if(pch = strchr((LPSTR)szParamType, '(')) {
			*pch = '\0';
			lstrcat(szParamType, lpqt->szParam);
			lstrcat(szParamType, (LPSTR)strchr(rgFields[i].szType, ')') + 1);
		} else {
			lstrcat(szParamType, lpqt->szParam);
		}
		wsprintf(&lpqt->sz[lstrlen(lpqt->sz)], " %s %s, ",
			(LPSTR)rgFields[i].szFieldName, (LPSTR)szParamType);
	}

	/* remove the last comma and space */
	lpqt->sz[lstrlen(lpqt->sz) - 2] = '\0';
	lstrcat(lpqt->sz, ")");

	returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
	if(returncode != SQL_SUCCESS && returncode != SQL_SUCCESS_WITH_INFO)
		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLExecDirect"))
			goto ErrorRet;

	/* now that table was created, call SQLColumns.  Both as
		a test and to get necessary information to insert data */

	returncode = SQLColumns(hstmt, NULL, SQL_NTS, NULL, SQL_NTS,
		lpqt->szTableName, SQL_NTS, NULL, SQL_NTS);
	RETCHECK(SQL_SUCCESS, returncode, "SQLColumns");

	for(i = 0; i < cTypes; i++) {
		returncode = SQLFetch(hstmt);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFetch");

		/* precision */
		returncode = SQLGetData(hstmt, 7, SQL_C_LONG, &rgFields[i].precision, IGNORED,
			&sdw);
		RETCHECK(SQL_SUCCESS, returncode, "SQLGetData");

		/* length */
		if(rgFields[i].precision == 0)
		returncode = SQLGetData(hstmt, 8, SQL_C_LONG, &rgFields[i].precision, IGNORED,
			&sdw);
		RETCHECK(SQL_SUCCESS, returncode, "SQLGetData");

		/* numeric scale */
		returncode = SQLGetData(hstmt, 9, SQL_C_SHORT, &rgFields[i].scale, IGNORED,
			&sdw);
		RETCHECK(SQL_SUCCESS, returncode, "SQLGetData");
	}

	returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	/* look for columns the test should not try to update by running
		a select * query and calling ColAttributes on the result fields */
	/* select * does not return the fields in a defined order, so field order
		must be specified */

	lstrcpy(lpqt->sz, "select ");
	for(i = 0; i < cTypes; i ++) {
		if(i)
			lstrcat(lpqt->sz, ",");
		lstrcat(lpqt->sz, rgFields[i].szFieldName);
	}

	lstrcat(lpqt->sz, " from ");
	lstrcat(lpqt->sz, lpqt->szTableName);

	returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
	RETCHECK(SQL_SUCCESS, returncode, "SQLExecDirect");

	for(i = 1; i <= cTypes; i++) {

		returncode = SQLColAttributes(hstmt, i, SQL_COLUMN_UPDATABLE, NULL,
			0, NULL, &sdw);
		RETCHECK(SQL_SUCCESS, returncode, "SQLColAttributes");

		rgFields[i - 1].fAutoUpdate = sdw == SQL_ATTR_READONLY;

		returncode = SQLColAttributes(hstmt, i, SQL_COLUMN_UNSIGNED, NULL,
			0, NULL, &sdw);
		RETCHECK(SQL_SUCCESS, returncode, "SQLColAttributes");

		rgFields[i - 1].fUnsigned = sdw;
	}

	returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	/* put together the insert statement, and set the parameters */
	/* parameters are only set the first time through, after which
	the contents of the pointers is changed */

	*lpqt->szColNames = '\0';
	*lpqt->szValues = '\0';
	fColNames = TRUE;

	for(i = 0; i < cTypes; i++) {
		for(ind = 0, w = 0; ind < cTypes; ind++) {
			pch = qtMakeData(i, &rgFields[ind], lpqt->szDataItem);

			if(!pch) /* current type is READ_ONLY) */
				continue;

			/* for every nullable type, that field will be set to
			null when the row number corresponds with the field
			number */

			if(*pch) {
				lstrcpy(lpd[w].data, pch);
				lpd[w].cb = SQL_NTS;
			}
			else {
				lstrcpy(lpd[w].data, "");
				lpd[w].cb = SQL_NULL_DATA;
			}

			if(fColNames) { /* the first time throught, the insert
				statement itself is created */
				lstrcat(lpqt->szColNames, rgFields[ind].szFieldName);
				lstrcat(lpqt->szColNames, ", ");
				lstrcat(lpqt->szValues, " ?, ");

				/* and parameters are set */
				/* set all the parameters using pointers to buffers in
				the param struct. */
				if(!fBindParam){
					returncode = SQLSetParam(hstmt, (UWORD)(w+1),
						SQL_C_CHAR, rgFields[ind].wSQLType,
						rgFields[ind].precision, rgFields[ind].scale,
						&lpd[w].data,
						&lpd[w].cb);
					RETCHECK(SQL_SUCCESS, returncode, "SQLSetParam");
					}
				else{
					returncode = SQLBindParameter(hstmt, (UWORD)(w+1),
						SQL_PARAM_INPUT,
						SQL_C_CHAR, rgFields[ind].wSQLType,
						rgFields[ind].precision, rgFields[ind].scale,
						&lpd[w].data, 0,
						&lpd[w].cb);
					RETCHECK(SQL_SUCCESS, returncode, "SQLBindParameter");
					}
			}
			w++;
		}

		if(fColNames) { /* the first time through, the insert
			statement needs to be SQLPrepare'd */
			/* remove the ", " at the end of the string */
			lpqt->szColNames[lstrlen(lpqt->szColNames) - 2] = '\0';
			lpqt->szValues[lstrlen(lpqt->szValues) - 2] = '\0';

			wsprintf(lpqt->sz, "insert into %s (%s) VALUES(%s)",
				lpqt->szTableName, lpqt->szColNames, lpqt->szValues);

			returncode = SQLPrepare(hstmt, lpqt->sz, SQL_NTS);
			if(!RETCHECK(SQL_SUCCESS, returncode, "SQLPrepare"))
				goto ErrorRet;

			fColNames = FALSE; /* no more first times through */
		}

		returncode = SQLExecute(hstmt); /* insert a row */
		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLExecute"))
			goto ErrorRet;

		returncode = SQLRowCount(hstmt, &sdw);
		RETCHECK(SQL_SUCCESS, returncode, "SQLRowCount");

		if(sdw != 1) /* the execute inserted 1 row */
			DISPLAYERROR("SQLRowCount", "Insert single row");

		/* FreeStmt SQL_CLOSE */
		returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");
	}

	/* for the last row use SQLParamData and SQLPutData */

	sdw = SQL_DATA_AT_EXEC;

	for(ind = 0, w = 0; ind < cTypes; ind++) { /* set all the params */
		if(rgFields[ind].fAutoUpdate)
			continue;

		w++;
		if(!fBindParam){
			returncode = SQLSetParam(hstmt, w,
				SQL_C_CHAR, rgFields[ind].wSQLType,
				rgFields[ind].precision, rgFields[ind].scale,
				(LPSTR)(UDWORD)ind, &sdw);
			RETCHECK(SQL_SUCCESS, returncode, "SQLSetParam");
			}
		else{
			returncode = SQLBindParameter(hstmt, w, SQL_PARAM_INPUT,
				SQL_C_CHAR, rgFields[ind].wSQLType,
				rgFields[ind].precision, rgFields[ind].scale,
				(LPSTR)(UDWORD)ind, 0, &sdw);
			RETCHECK(SQL_SUCCESS, returncode, "SQLBindParameter");
			}
	}

	cColsSelected = w;

	returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
	RETCHECK(SQL_NEED_DATA, returncode, "SQLExecDirect");

	for(ind = 0; ind <= cTypes; ind++) {
		returncode = SQLParamData(hstmt, &ptr);
		if(returncode != SQL_NEED_DATA)
			break;

		pch = qtMakeData(cTypes, &rgFields[(unsigned long)ptr],
			lpqt->szDataItem);

		if(*pch) {
			returncode = SQLPutData(hstmt, pch, SQL_NTS);
			RETCHECK(SQL_SUCCESS, returncode, "SQLPutData");
		} else {
			returncode = SQLPutData(hstmt, (LPSTR)IGNORED, SQL_NULL_DATA);
			RETCHECK(SQL_SUCCESS, returncode, "SQLPutData");
		}
	}

	RETCHECK(SQL_SUCCESS, returncode, "SQLParamData");

	returncode = SQLRowCount(hstmt, &sdw);
	RETCHECK(SQL_SUCCESS, returncode, "SQLRowCount");

	if(sdw != 1)
		DISPLAYERROR("SQLRowCount", "Insert single row");

	returncode = SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	/* now that the table is created and has data, make sure it
		all comes back correctly */

	wsprintf(lpqt->sz, "select %s from %s", lpqt->szColNames, lpqt->szTableName);

	returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
	RETCHECK(SQL_SUCCESS, returncode, "SQLExecDirect");

	lpqt->sz[0] = '\0';

	/* the cursor name should be created by the driver since one was
		not specified */

	returncode = SQLGetCursorName(hstmt, lpqt->sz, MAX_STRING_SIZE, &cb);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetCursorName");

	if(cb > MAX_STRING_SIZE) {
		DISPLAYERROR("SQLGetCursorName", "invalid cb");
		cb = MAX_STRING_SIZE;
	}

	if(!*lpqt->sz) { /* don't check the name itself, just make sure that 
		something was returned */
		DISPLAYERROR("SQLGetCursorName", "no name returned");
	}

	returncode = SQLNumResultCols(hstmt, &wNum);
	RETCHECK(SQL_SUCCESS, returncode, "SQLNumResultCols");

	if(wNum != cColsSelected) {
		DISPLAYERROR("SQLNumResultCols", "incorrect value returned");
	}

	for(i = 0, w = 0; i < cTypes; i++) {
		dwLen = 0;
		wNum = 0;
		wNull = 0;

		/* information returned by SQLDescribeCol should match info
			used to create table */

		if(!qtMakeData(1, &rgFields[i],
			lpqt->szDataItem))
			continue;

		w++;
		returncode = SQLDescribeCol(hstmt, w, lpqt->buf,
			MAX_STRING_SIZE, NULL, &wType, &dwLen, &wNum, &wNull);
		RETCHECK(SQL_SUCCESS, returncode, "SQLDescribeCol");

		/* verify that column name returned is column name created */

		if(0 != lstrcmpi(rgFields[i].szFieldName, lpqt->buf)) {
			DISPLAYERROR("SQLDescribeCol", "incorrect column name");
		}

		if(wType != rgFields[i].wSQLType) {
			DISPLAYERROR("SQLDescribeCol", "incorrect SQLType");
		}

		if(dwLen != (UDWORD)rgFields[i].precision) {
			DISPLAYERROR("SQLDescribeCol", "incorrect precision");
		}

		if(wNum != rgFields[i].scale) {
			DISPLAYERROR("SQLDescribeCol", "incorrect scale");
		}

		if(wNull != rgFields[i].nullable && wNull != SQL_NULLABLE_UNKNOWN &&
			rgFields[i].nullable != SQL_NULLABLE_UNKNOWN) {
			DISPLAYERROR("SQLDescribeCol", "incorrect nullable");
		}
	}

/* bind all fields to a variable of the correct type for retreiving data */

	for(i = 0, w = 0; i < cTypes; i++) {

		if(!qtMakeData(1, &rgFields[i],
			lpqt->szDataItem))
			continue;

		w++;

		switch(rgFields[i].wSQLType) {
			case SQL_INTEGER:
		returncode = SQLBindCol(hstmt, w, SQL_C_DEFAULT,
					&lpd[i].sdword, IGNORED, &lpd[i].cb);
				break;
			case SQL_SMALLINT:
		returncode = SQLBindCol(hstmt, w, SQL_C_DEFAULT,
					&lpd[i].sword, IGNORED, &lpd[i].cb);
				break;
			case SQL_FLOAT:
			case SQL_DOUBLE:
		returncode = SQLBindCol(hstmt, w, SQL_C_DEFAULT,
					&lpd[i].sdouble, IGNORED, &lpd[i].cb);
				break;
			case SQL_REAL:
		returncode = SQLBindCol(hstmt, w, SQL_C_DEFAULT,
					&lpd[i].sfloat, IGNORED, &lpd[i].cb);
				break;
			case SQL_DATE:
		returncode = SQLBindCol(hstmt, w, SQL_C_DEFAULT,
					&lpd[i].date, IGNORED, &lpd[i].cb);
				break;
			case SQL_TIME:
		returncode = SQLBindCol(hstmt, w, SQL_C_DEFAULT,
					&lpd[i].time, IGNORED, &lpd[i].cb);
				break;
			case SQL_TIMESTAMP:
		returncode = SQLBindCol(hstmt, w, SQL_C_DEFAULT,
					&lpd[i].timestamp, IGNORED, &lpd[i].cb);
				break;
			case SQL_CHAR:
			case SQL_VARCHAR:
			case SQL_NUMERIC:
			case SQL_DECIMAL:
			default:
		returncode = SQLBindCol(hstmt, w, SQL_C_DEFAULT,
					lpd[i].data, MAX_STRING_SIZE, &lpd[i].cb);
				break;
		}

		if(!RETCHECK(SQL_SUCCESS, returncode, "SQLBindCol"))
			goto ErrorRet;
	}

	for(ind = 0;; ind++) {

		/* Get the data back */
		returncode = SQLFetch(hstmt);
		if(returncode != SQL_SUCCESS && returncode != SQL_SUCCESS_WITH_INFO)
			break;

		for(i = 0; i < cTypes; i++) { /* make sure it's original data placed
			in that field/row location */
			pch = qtMakeData(ind, &rgFields[i], lpqt->szDataItem);

			if(!pch)
				continue;

			if(!*pch) {
				if(lpd[i].cb != SQL_NULL_DATA)
					DISPLAYERROR("returned data", "should have been NULL");
				continue;
			}
			switch(rgFields[i].wSQLType) { /* check the outlen and data
				returned for each type. */
				case SQL_INTEGER:
					if(lpd[i].cb != sizeof(SDWORD))
						DISPLAYERROR("returned data", "incorrect outlen");

					lstrcpy(szNum, pch);
					same = atol(szNum) == lpd[i].sdword;
					break;
				case SQL_SMALLINT:
					if(lpd[i].cb != sizeof(SWORD))
						DISPLAYERROR("returned data", "incorrect outlen");

					lstrcpy(szNum, pch);
					same = atoi(szNum) == lpd[i].sword;
					break;

				case SQL_FLOAT:
				case SQL_DOUBLE:
					if(lpd[i].cb != sizeof(SDOUBLE))
						DISPLAYERROR("returned data", "incorrect outlen");

					lstrcpy(szNum, pch);
					same = atof(szNum) - lpd[i].sdouble < 0.001 &&
						atof(szNum) - lpd[i].sdouble > -0.001;
					break;

				case SQL_REAL:
					if(lpd[i].cb != sizeof(SFLOAT))
						DISPLAYERROR("returned data", "incorrect outlen");

					lstrcpy(szNum, pch);
					same = atof(szNum) - lpd[i].sfloat < 0.001 &&
						atof(szNum) - lpd[i].sfloat > -0.001;
					break;

				case SQL_TINYINT:
					if(lpd[i].cb != sizeof(char))
						DISPLAYERROR("returned data", "incorrect outlen");

					lstrcpy(szNum, pch);
					same = (char)atoi(szNum) == (char)lpd[i].data[0];
					break;

				case SQL_DECIMAL:
				case SQL_NUMERIC:
					if(lpd[i].cb > MAX_STRING_SIZE)
						DISPLAYERROR("returned data", "incorrect outlen");

					if(lpd[i].cb > lstrlen(lpd[i].data))
						DISPLAYERROR("returned data", "incorrect outlen");

					{
					char szNum2[MAX_NUM_BUFFER];

					lstrcpy(szNum, pch);
					lstrcpy(szNum2, pch);
					same = atof(szNum) - atof(szNum2) < 0.001 &&
						atof(szNum) - atof(szNum2) > -0.001;
					}

					break;


				case SQL_VARCHAR:
				case SQL_LONGVARCHAR:
					if(lpd[i].cb > MAX_STRING_SIZE)
						DISPLAYERROR("returned data", "incorrect outlen");

					if(lpd[i].cb > lstrlen(lpd[i].data))
						DISPLAYERROR("returned data", "incorrect outlen");

					same = !!strstr(lpd[i].data, pch);

					break;

				case SQL_VARBINARY:
				case SQL_LONGVARBINARY:
					if(lpd[i].cb > MAX_STRING_SIZE)
						DISPLAYERROR("returned data", "incorrect outlen");

					if(lpd[i].cb  * 2 != lstrlen(pch))
						DISPLAYERROR("returned data", "incorrect outlen");

					same = TRUE; /* not checking returned data for binary */

					break;

				case SQL_CHAR:
					if(lpd[i].cb > MAX_STRING_SIZE && returncode == SQL_SUCCESS)
						DISPLAYERROR("returned data", "incorrect return code for outlen");

					if(lpd[i].cb != rgFields[i].precision && returncode == SQL_SUCCESS)
						DISPLAYERROR("returned data", "incorrect outlen");

					same = !strncmp(lpd[i].data, pch, lstrlen(pch));

					break;
				case SQL_BINARY:
				default:
					if(lpd[i].cb > MAX_STRING_SIZE && returncode == SQL_SUCCESS)
						DISPLAYERROR("returned data", "incorrect outlen");
					same = TRUE; /* not checking returned data for binary */
			}
		if(!same)
			DISPLAYERROR("returned data", "invalid data");
		}
	}

	RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

	if(ind != cTypes + 1)
		DISPLAYERROR("SQLFetch", "Incorrect number of result rows");

	returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	returncode = SQLFreeStmt(hstmt, SQL_UNBIND);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	returncode = SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	wsprintf(lpqt->sz, "select %s from %s where ", lpqt->szColNames,
		lpqt->szTableName);

    sdw = SQL_DATA_AT_EXEC;

	for(i = 0, w = 0; i < cTypes; i++) {
		pch = qtMakeData(ind, &rgFields[i], lpqt->szDataItem);
		if ((rgFields[i].fSearchable == SQL_SEARCHABLE ||
			rgFields[i].fSearchable == SQL_ALL_EXCEPT_LIKE) &&
			pch && *pch) {
			w++;
			switch(rgFields[i].wSQLType) {
				case SQL_REAL:
				case SQL_FLOAT:
				case SQL_DOUBLE:
					wsprintf(&lpqt->sz[lstrlen(lpqt->sz)], "%s < ? + 1 and ",
						(LPSTR)rgFields[i].szFieldName);
					break;
				default:
					wsprintf(&lpqt->sz[lstrlen(lpqt->sz)], "%s = ? and ",
					(LPSTR)rgFields[i].szFieldName);
			}

			if(!fBindParam){
				returncode = SQLSetParam(hstmt, w,
					SQL_C_CHAR, rgFields[i].wSQLType,
					rgFields[i].precision, rgFields[i].scale,
					(LPSTR)(UDWORD)i, &sdw);
				RETCHECK(SQL_SUCCESS, returncode, "SQLSetParam");
				}
			else{
				returncode = SQLBindParameter(hstmt, w, SQL_PARAM_INPUT,
					SQL_C_CHAR, rgFields[i].wSQLType,
					rgFields[i].precision, rgFields[i].scale,
					(LPSTR)(UDWORD)i, 0, &sdw);
				RETCHECK(SQL_SUCCESS, returncode, "SQLBindParameter");
				}

			ind++;
		}
	}
	/* remove the final "and " */
	lpqt->sz[lstrlen(lpqt->sz) - 5] = '\0';
	lstrcpy(lpqt->szParamQuery, lpqt->sz);

	returncode = SQLPrepare(hstmt, lpqt->sz, SQL_NTS);
	RETCHECK(SQL_SUCCESS, returncode, "SQLPrepare");

	returncode = SQLExecute(hstmt);
	RETCHECK(SQL_NEED_DATA, returncode, "SQLExecute");

	udw = cTypes;

	for(i = 0; ; i++) {
		returncode = SQLParamData(hstmt, (PTR FAR *)&udw);
		if(returncode != SQL_NEED_DATA)
			break;

		if(udw < cTypes)
			pch = qtMakeData(cTypes, &rgFields[udw], lpqt->szDataItem);
		else {
			DISPLAYERROR("SQLParamData", "invalid rgbValue");
		}

		if(*pch) {
			returncode = SQLPutData(hstmt, pch, SQL_NTS);
			RETCHECK(SQL_SUCCESS, returncode, "SQLPutData");
		} else {
			returncode = SQLPutData(hstmt, (LPSTR)IGNORED, SQL_NULL_DATA);
			RETCHECK(SQL_SUCCESS, returncode, "SQLPutData");
		}
	}
	RETCHECK(SQL_SUCCESS, returncode, "SQLParamData");

	for(i = 0;; i++) {
		returncode = SQLFetch(hstmt);

		if(returncode != SQL_SUCCESS)
			break;
	}
	RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

	/* should have gotten 1 row back */

	if(i != 1) {
		DISPLAYERROR("Param/PutData", "incorrect number of rows returned");
	}


	returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	/* > 1K query */
	wsprintf(lpqt->sz, "select %s from %s where ", lpqt->szColNames,
		lpqt->szTableName);

	for(i = 0; i < cTypes; i++) {
		if (rgFields[i].fSearchable == SQL_SEARCHABLE ||
			rgFields[i].fSearchable == SQL_ALL_EXCEPT_LIKE)
			break;
	}

	pch = qtMakeData(cTypes, &rgFields[i], lpqt->szDataItem);
	while(lstrlen(lpqt->sz) < 1024L) {                  
		int li=lstrlen(lpqt->sz);
		switch(rgFields[i].wSQLType) {
			case SQL_REAL:
			case SQL_FLOAT:
			case SQL_DOUBLE:
				wsprintf(&lpqt->sz[lstrlen(lpqt->sz)], "%s < %s + 1 and ",
					(LPSTR)rgFields[i].szFieldName, (LPSTR)pch);
				break;
			default:
				wsprintf(&lpqt->sz[lstrlen(lpqt->sz)], "%s = %s%s%s and ",
				(LPSTR)rgFields[i].szFieldName, (LPSTR)rgFields[i].szPrefix, (LPSTR)pch, (LPSTR)rgFields[i].szSuffix);
				break;
		}
	}

	/* remove the final "and " */
	lpqt->sz[lstrlen(lpqt->sz) - 5] = '\0';
				
	returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
	RETCHECK(SQL_SUCCESS, returncode, "SQLExecDirect");

	for(i = 0;; i++) {
		returncode = SQLFetch(hstmt);

		if(returncode != SQL_SUCCESS)
			break;
	}
	RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

	/* should have gotten at least 1 row back */

	if(i < 1) {
		DISPLAYERROR("> 1K query", "incorrect number of rows returned");
	}

	
	/* SQLFreeStmt with SQL_CLOSE to re-use the hstmt */

	returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	/* verify table shows up in SQLTables */

	returncode = SQLTables(hstmt, NULL, 0, NULL, 0, "q%",
		SQL_NTS, "'TABLE'", SQL_NTS); /* this call may return many 
			tables, as long as the one created earlier shows up it
			will pass. */
	RETCHECK(SQL_SUCCESS, returncode, "SQLTables");

	for(i = 0;; i++) {
		returncode = SQLFetch(hstmt);
		if(returncode != SQL_SUCCESS)
			break;

		/* column 3 is tablename */
		returncode = SQLGetData(hstmt, 3, SQL_C_CHAR, lpqt->sz, MAX_STRING_SIZE, NULL);
		RETCHECK(SQL_SUCCESS, returncode, "SQLGetData"); /* should not overflow
			and return SQL_SUCCESS_WITH_INFO because the buffer is larger than the
			table name */
		fFoundTable += 0 == lstrcmpi(lpqt->sz, lpqt->szTableName);
	}

	RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

	if(1 != fFoundTable) {
		DISPLAYERROR("SQLTables", "table not found");
	}

	returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	/* create an index and verify that it is returned by SQLStatistics */

	lstrcpy(lpqt->szDataItem, lpqt->szTableName);
	lpqt->szDataItem[0] = 'i';
	for(i = 1; i < cTypes; i++)
		if(rgFields[i].wSQLType == SQL_INTEGER ||
			rgFields[i].wSQLType == SQL_SMALLINT)
			break;
	if(i == cTypes)
		i = 0;

	lstrcpy(lpqt->buf, rgFields[i].szFieldName);
	wsprintf(lpqt->sz, "create index %s on %s (%s)",
		lpqt->szDataItem, lpqt->szTableName, lpqt->buf);

	returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
	if(fIndex < 1)
		/* if this is minimal grammar, don't count on indexes being available */
		fIndex = returncode == SQL_SUCCESS;
	else
		RETCHECK(SQL_SUCCESS, returncode, "SQLExecDirect");

	/* this should return a keyset if one exists for the table. */

	returncode = SQLSpecialColumns(hstmt, SQL_BEST_ROWID, NULL, 0, NULL, 0,
		lpqt->szTableName, (SWORD)(lstrlen(lpqt->szTableName)), SQL_SCOPE_TRANSACTION,
		SQL_NULLABLE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLSpecialColumns");

	for(i = 0;; i++){
		returncode = SQLFetch(hstmt);
		if(returncode != SQL_SUCCESS)
			break;
	}

	RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

	returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	/* verify the index shows up in SQLStatistics */

	returncode = SQLStatistics(hstmt, NULL, 0, NULL, 0, lpqt->szTableName,
	SQL_NTS, SQL_INDEX_ALL, SQL_ENSURE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLStatistics");

	fFoundTable = 0;
	for(i = 0;; i++) {
		returncode = SQLFetch(hstmt);
		if(returncode != SQL_SUCCESS)
			break;

		returncode = SQLGetData(hstmt, 3, SQL_C_CHAR, lpqt->sz, MAX_STRING_SIZE,
			NULL);
		RETCHECK(SQL_SUCCESS, returncode, "SQLGetData");
		if (lstrcmpi(lpqt->sz, lpqt->szTableName) == 0)
		{
			returncode = SQLGetData(hstmt, 6, SQL_C_CHAR, lpqt->sz, MAX_STRING_SIZE,
				NULL);
			RETCHECK(SQL_SUCCESS, returncode, "SQLGetData");
			fFoundTable += 0 == lstrcmpi(lpqt->sz, lpqt->szDataItem);
		}
	}

	RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

	if(1 != fFoundTable && fIndex) {
		DISPLAYERROR("SQLStatistics", "index not returned");
	}

	if(i > 2 || i < 1) { /* one row represents original table, the other represents
					the index */
		DISPLAYERROR("SQLStatistics", "too many rows");
	}

	returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	for(i = 0; i < cTypes; i++) {
		if(rgFields[i].fSearchable == SQL_LIKE_ONLY ||
			rgFields[i].fSearchable == SQL_SEARCHABLE) {
			lstrcpy(lpqt->buf, rgFields[i].szFieldName);
			break;
		}
	}
	if(i < cTypes) {
	/* execute a query using like.  This query should return all records */

		wsprintf(lpqt->sz, "select * from %s where %s not like 'a'", lpqt->szTableName,
			lpqt->buf, lpqt->buf);
		/* this query should return all rows in the table */

		returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
		RETCHECK(SQL_SUCCESS, returncode, "SQLExecDirect");

		for(i = 0;; i++) {
			returncode = SQLFetch(hstmt);
			if(returncode != SQL_SUCCESS)
				break;

			returncode = SQLGetData(hstmt, 1, SQL_C_CHAR, lpqt->sz, MAX_STRING_SIZE,
				NULL);
			if(returncode != SQL_SUCCESS && returncode != SQL_SUCCESS_WITH_INFO)
				RETCHECK(SQL_SUCCESS, returncode, "SQLGetData");
		}

		RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

		if(i != cTypes) {
			DISPLAYERROR("'LIKE' query", "incorrect number of result rows");
		}
	}
	/* SQLFreeStmt SQL_CLOSE to re-use it */

	returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	/* attempt execution of Level 2 functionality */

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLFOREIGNKEYS,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLForeignKeys(hstmt, NULL, 0, NULL, 0, lpqt->szTableName, SQL_NTS, NULL, 0, NULL, 0, NULL, 0);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLForeignKeys", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLForeignKeys");
	} else {
		returncode = SQLForeignKeys(hstmt, NULL, 0, NULL, 0, lpqt->szTableName, SQL_NTS, NULL, 0, NULL, 0, NULL, 0);
		RETCHECK(SQL_SUCCESS, returncode, "SQLForeignKeys");
		while(returncode == SQL_SUCCESS) {
			returncode = SQLFetch(hstmt);
		}
		RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

		returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLBROWSECONNECT,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	returncode = SQLGetInfo(hdbc, SQL_ACTIVE_CONNECTIONS, &cMaxConnections, sizeof(int), NULL);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetInfo");

	if(cMaxConnections != 1) {
		char szBCString[40];
		char szDSN[40];
		HDBC hdbcb;
		lstrcpy(szBCString, "DSN=");
		lstrcat(szBCString, pTestSource->szValidServer0);
		
		if(pTestSource->szValidServer0[0] == 0){
			returncode = SQLGetInfo(hdbc, SQL_DATA_SOURCE_NAME, &szDSN, 40, NULL);
			lstrcat(szBCString, szDSN);
			}

		if(!fLevel2) {
			HDBC thdbc;

			returncode = SQLAllocConnect(henv, &hdbcb);
			RETCHECK(SQL_SUCCESS, returncode, "SQLAllocConnect");

			returncode = SQLBrowseConnect(hdbcb, szBCString, SQL_NTS, NULL, 0, NULL);
			thdbc = hdbc;
			hdbc = hdbcb;
			if(!FindError("IM001"))
				DISPLAYERROR("SQLBrowseConnect", "did not return Not supported message");
			RETCHECK(SQL_ERROR, returncode, "SQLBrowseConnect");
			hdbc = thdbc;

			returncode = SQLFreeConnect(hdbcb);
			RETCHECK(SQL_SUCCESS, returncode, "SQLFreeConnect");
		
		} else {
			returncode = SQLAllocConnect(henv, &hdbcb);
			RETCHECK(SQL_SUCCESS, returncode, "SQLAllocConnect");

			returncode = SQLBrowseConnect(hdbcb, szBCString, SQL_NTS, NULL, 0, NULL);
			RETCHECK(SQL_NEED_DATA, returncode, "SQLBrowseConnect");

			returncode = SQLDisconnect(hdbcb);
			RETCHECK(SQL_SUCCESS, returncode, "SQLDisconnect");
			returncode = SQLFreeConnect(hdbcb);
			RETCHECK(SQL_SUCCESS, returncode, "SQLFreeConnect");
		}
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLDATASOURCES,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLDataSources(henv, SQL_FETCH_FIRST, NULL, 0, NULL, NULL, 0, NULL);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLDataSources", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLDataSources");
	} else {
		returncode = SQLDataSources(henv, SQL_FETCH_FIRST, NULL, 0, NULL, NULL, 0, NULL);
		RETCHECK(SQL_SUCCESS, returncode, "SQLDataSources");
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLDRIVERS,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLDrivers(henv, SQL_FETCH_FIRST, NULL, 0, NULL, NULL, 0, NULL);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLDrivers", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLDataSources");
	} else {
		returncode = SQLDrivers(henv, SQL_FETCH_FIRST, NULL, 0, NULL, NULL, 0, NULL);
		if(returncode != SQL_SUCCESS)
			RETCHECK(SQL_SUCCESS_WITH_INFO, returncode, "SQLDrivers");
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLMORERESULTS,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		wsprintf(lpqt->sz, "select * from %s", lpqt->szTableName);
		returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
		RETCHECK(SQL_SUCCESS, returncode, "SQLExecDirect");
		returncode = SQLMoreResults(hstmt);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLMoreResults", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLMoreResults");
	} else {
		wsprintf(lpqt->sz, "select * from %s", lpqt->szTableName);
		returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
		RETCHECK(SQL_SUCCESS, returncode, "SQLExecDirect");
		returncode = SQLMoreResults(hstmt);
		RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLMoreResults");
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLNATIVESQL,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLNativeSql(hdbc, lpqt->sz, SQL_NTS, NULL, 0, NULL);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLNativeSql", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLNativeSql");
	} else {
		returncode = SQLNativeSql(hdbc, lpqt->sz, SQL_NTS, NULL, 0, NULL);
		RETCHECK(SQL_SUCCESS, returncode, "SQLNativeSql");
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLDESCRIBEPARAM,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLPrepare(hstmt, lpqt->szParamQuery, SQL_NTS);
		RETCHECK(SQL_SUCCESS, returncode, "SQLPrepare");
		returncode = SQLDescribeParam(hstmt, 1, NULL, NULL, NULL, NULL);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLDescribeParam", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLDescribeParam");
	} else {
		returncode = SQLPrepare(hstmt, lpqt->szParamQuery, SQL_NTS);
		RETCHECK(SQL_SUCCESS, returncode, "SQLPrepare");
		returncode = SQLDescribeParam(hstmt, 1, NULL, NULL, NULL, NULL);
		RETCHECK(SQL_SUCCESS, returncode, "SQLDescribeParam");
	}
			
	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLNUMPARAMS,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLNumParams(hstmt, NULL);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLNumParams", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLNumParams");
	} else {
		returncode = SQLNumParams(hstmt, NULL);
		RETCHECK(SQL_SUCCESS, returncode, "SQLNumParams");
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLPARAMOPTIONS,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLParamOptions(hstmt, 1, NULL);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLParamOptions", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLParamOptions");
	} else {
		returncode = SQLParamOptions(hstmt, 1, NULL);
		RETCHECK(SQL_SUCCESS, returncode, "SQLParamOptions");
	}

	returncode = SQLFreeStmt(hstmt, SQL_DROP);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");
	returncode = SQLAllocStmt(hdbc, &hstmt);
	RETCHECK(SQL_SUCCESS, returncode, "SQLAllocStmt");

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLPRIMARYKEYS,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLPrimaryKeys(hstmt, NULL, 0, NULL, 0, lpqt->szTableName, SQL_NTS);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLPrimaryKeys", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLPrimaryKeys");
	} else {
		returncode = SQLPrimaryKeys(hstmt, NULL, 0, NULL, 0, lpqt->szTableName, SQL_NTS);
		RETCHECK(SQL_SUCCESS, returncode, "SQLPrimaryKeys");
		while(returncode == SQL_SUCCESS) {
			returncode = SQLFetch(hstmt);
		}
		RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

		returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLPROCEDURECOLUMNS,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLProcedureColumns(hstmt, NULL, 0, NULL, 0, NULL, 0, NULL, 0);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLProcedureColumns", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLProcedureColumns");
	} else {
		returncode = SQLProcedureColumns(hstmt, NULL, 0, NULL, 0, NULL, 0, NULL, 0);
		RETCHECK(SQL_SUCCESS, returncode, "SQLProcedureColumns");
		while(returncode == SQL_SUCCESS) {
			returncode = SQLFetch(hstmt);
		}
		RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

		returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLPROCEDURES,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLProcedures(hstmt, NULL, 0, NULL, 0, NULL, 0);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLProcedures", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLProcedures");
	} else {
		returncode = SQLProcedures(hstmt, NULL, 0, NULL, 0, NULL, 0);
		RETCHECK(SQL_SUCCESS, returncode, "SQLProcedures");
		while(returncode == SQL_SUCCESS) {
			returncode = SQLFetch(hstmt);
		}
		RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

		returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLTABLEPRIVILEGES,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLTablePrivileges(hstmt, NULL, 0, NULL, 0, NULL, 0);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLTablePrivileges", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLTablePrivileges");
	} else {
		returncode = SQLTablePrivileges(hstmt, NULL, 0, NULL, 0, NULL, 0);
		RETCHECK(SQL_SUCCESS, returncode, "SQLTablePrivileges");
		while(returncode == SQL_SUCCESS) {
			returncode = SQLFetch(hstmt);
		}
		RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

		returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLCOLUMNPRIVILEGES,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLColumnPrivileges(hstmt, NULL, 0, NULL, 0, lpqt->szTableName, SQL_NTS,
			NULL, 0);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLColummPrivileges", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLColumnPrivileges");
	} else {
		returncode = SQLColumnPrivileges(hstmt, NULL, 0, NULL, 0, lpqt->szTableName, SQL_NTS,
			NULL, 0);
		RETCHECK(SQL_SUCCESS, returncode, "SQLColumnPrivileges");
		while(returncode == SQL_SUCCESS) {
			returncode = SQLFetch(hstmt);
		}
		RETCHECK(SQL_NO_DATA_FOUND, returncode, "SQLFetch");

		returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLSETSCROLLOPTIONS,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		returncode = SQLSetScrollOptions(hstmt, SQL_CONCUR_READ_ONLY, SQL_SCROLL_FORWARD_ONLY,
			1);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLSetScrollOptionss", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLSetScrollOptions");
	} else {
		typedef struct SupportOptList {
			long Support;
			SDWORD Option;
			} SUPPORTOPTINFO;

		typedef struct SupportConcurList {
			long Support;
			UWORD Option;
			} SUPPORTCONCURINFO;

		SUPPORTOPTINFO OptionList[] = {SQL_SO_FORWARD_ONLY, SQL_SCROLL_FORWARD_ONLY,
											SQL_SO_KEYSET_DRIVEN, SQL_SCROLL_KEYSET_DRIVEN,
											SQL_SO_DYNAMIC, SQL_SCROLL_DYNAMIC};

		SUPPORTCONCURINFO ConcurList[] = {SQL_SCCO_READ_ONLY, SQL_CONCUR_READ_ONLY,
											SQL_SCCO_LOCK, SQL_CONCUR_LOCK,
											SQL_SCCO_OPT_TIMESTAMP, SQL_CONCUR_TIMESTAMP,
											SQL_SCCO_OPT_VALUES, SQL_CONCUR_VALUES};

		returncode = SQLGetInfo(hdbc, SQL_SCROLL_CONCURRENCY, &fSupportedCon, 4, NULL);
		RETCHECK(SQL_SUCCESS, returncode, "SQLGetInfo");
		returncode = SQLGetInfo(hdbc, SQL_SCROLL_OPTIONS, &fSupportedOpt, 4, NULL);
		RETCHECK(SQL_SUCCESS, returncode, "SQLGetInfo");
		for(i = 0; i < sizeof(OptionList) / sizeof(SUPPORTOPTINFO); i ++) {
			for(j = 0; j < sizeof(ConcurList) / sizeof(SUPPORTCONCURINFO); j++) {
				if(fSupportedOpt & OptionList[i].Support && fSupportedCon & ConcurList[j].Support) {
					if(!((ConcurList[j].Option == SQL_CONCUR_VALUES) && ((lpSI->vCursorLib == 
							SQL_CUR_USE_IF_NEEDED) || (lpSI->vCursorLib == SQL_CUR_USE_ODBC)))){
						returncode = SQLSetScrollOptions(hstmt, ConcurList[j].Option,
							OptionList[i].Option, 1);
						RETCHECK(SQL_SUCCESS, returncode, "SQLSetScrollOptions");
					}
				}
			}
		}
	}

	fLevel2 = FALSE;
	returncode = SQLGetFunctions(hdbc, SQL_API_SQLEXTENDEDFETCH,&fLevel2);
	RETCHECK(SQL_SUCCESS, returncode, "SQLGetFunctions");

	if(!fLevel2) {
		wsprintf(lpqt->sz, "select * from %s", lpqt->szTableName);
		returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
		RETCHECK(SQL_SUCCESS, returncode, "SQLExecDirect");
		returncode = SQLBindCol(hstmt, 1, SQL_C_BINARY, lpqt->sz, MAX_BIND_ARRAY_ELEMENT,
			NULL);
		RETCHECK(SQL_SUCCESS, returncode, "SQLBindCol");

		returncode = SQLExtendedFetch(hstmt, SQL_FETCH_FIRST, IGNORED, NULL, NULL);
		if(!FindError("IM001"))
			DISPLAYERROR("SQLExtendedFetch", "did not return Not supported message");
		RETCHECK(SQL_ERROR, returncode, "SQLExtendedFetch");

		returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");
	} else {
		wsprintf(lpqt->sz, "select * from %s", lpqt->szTableName);
		returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
		RETCHECK(SQL_SUCCESS, returncode, "SQLExecDirect");
		returncode = SQLBindCol(hstmt, 1, SQL_C_BINARY, lpqt->sz, MAX_BIND_ARRAY_ELEMENT,
			NULL);
		RETCHECK(SQL_SUCCESS, returncode, "SQLBindCol");

		returncode = SQLGetStmtOption(hstmt,SQL_CURSOR_TYPE,&dwLen);
		RETCHECK(SQL_SUCCESS, returncode, "SQLGetStmtOption");

		if(dwLen != SQL_CURSOR_FORWARD_ONLY){
			returncode = SQLExtendedFetch(hstmt, SQL_FETCH_FIRST, IGNORED, NULL, NULL);
			RETCHECK(SQL_SUCCESS, returncode, "SQLExtendedFetch");
			}
		else{
			returncode = SQLExtendedFetch(hstmt, SQL_FETCH_FIRST, IGNORED, NULL, NULL);
			RETCHECK(SQL_ERROR, returncode, "SQLExtendedFetch");
			}

		returncode = SQLFreeStmt(hstmt, SQL_CLOSE);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");
	}



	/* finished testing, clean up */
		
	lstrcpy(lpqt->sz, "drop table ");
	lstrcat(lpqt->sz, lpqt->szTableName);

	returncode = SQLExecDirect(hstmt, lpqt->sz, SQL_NTS);
	if(!RETCHECK(SQL_SUCCESS, returncode, "SQLExecDirect"))
		szWrite("Unable to drop table", TRUE);

	/* SQLCancel has the same functionality as SQLFreeStmt w/ SQL_CLOSE
		in a non-asynchronous environment */

	returncode = SQLCancel(hstmt);
	RETCHECK(SQL_SUCCESS, returncode, "SQLCancel");

	returncode = SQLFreeStmt(hstmt, SQL_DROP);
	RETCHECK(SQL_SUCCESS, returncode, "SQLFreeStmt");

	if(*pTestSource->szValidServer0) { /* if the connection was made in the
										test, it should be disconnected
						in the test, otherwise it should be left connected */

		returncode = SQLDisconnect(hdbc);
		RETCHECK(SQL_SUCCESS, returncode, "SQLDisconnect");

		returncode = SQLFreeConnect(hdbc);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFreeConnect");

		returncode = SQLFreeEnv(henv);
		RETCHECK(SQL_SUCCESS, returncode, "SQLFreeEnv");
	}

	ReleaseMemory(lpd);
	ReleaseMemory(lpqt);
	ReleaseMemory(rgFields);

	pTestSource->cErrors = lpSI->failed;

	return;


ErrorRet:

	/* a failure in an ODBC function that prevents completion of the
		test - for example, connect to the server */

	szWrite("\t\t *** Unrecoverable Quick Test FAILURE ***", TRUE);

	ReleaseMemory(lpd);
	ReleaseMemory(lpqt);
	ReleaseMemory(rgFields);

	pTestSource->cErrors = ABORT;

	return;
}

LPSTR FAR PASCAL RetcodeToChar(RETCODE retcode, LPSTR buf)
{
switch (retcode) {
	case SQL_SUCCESS:
		lstrcpy (buf, "SQL_SUCCESS");
		break;
	case SQL_ERROR:
		lstrcpy (buf, "SQL_ERROR");
		break;
	case SQL_SUCCESS_WITH_INFO:
		lstrcpy (buf, "SQL_SUCCESS_WITH_INFO");
		break;
	case SQL_NO_DATA_FOUND:
		lstrcpy (buf, "SQL_NO_DATA_FOUND");
		break;
	case SQL_NEED_DATA:
		lstrcpy (buf, "SQL_NEED_DATA");
		break;
	case SQL_INVALID_HANDLE:
		lstrcpy (buf, "SQL_INVALID_HANDLE");
		break;
	case SQL_STILL_EXECUTING:
		lstrcpy (buf, "SQL_STILL_EXECUTING");
		break;
	default:
		lstrcpy(buf, "UNKNOWN RETURNCODE");
}

return buf;
}

#define STATE_SIZE 6
#define COMBINED_SIZE MAX_ERROR_SIZE + 30

void FAR PASCAL DisplayAllErrors()
{
	char buf[MAX_ERROR_SIZE];
	char largebuf[COMBINED_SIZE];
	char szState[STATE_SIZE];
	RETCODE returncode;

	for(returncode = SQLError(henv, NULL, NULL, szState, NULL, buf, MAX_ERROR_SIZE, NULL)
		; returncode == SQL_SUCCESS || returncode == SQL_SUCCESS_WITH_INFO
		; returncode = SQLError(henv, NULL, NULL, szState, NULL, buf, MAX_ERROR_SIZE, NULL)) {
		wsprintf(largebuf, "\tState: %s", (LPSTR)szState);
		szWrite(largebuf, TRUE);
		wsprintf(largebuf, "\tError: %s", (LPSTR)buf);
		szWrite(largebuf, TRUE);
	}
	for(returncode = SQLError(NULL, hdbc, NULL, szState, NULL, buf, MAX_ERROR_SIZE, NULL)
		; returncode == SQL_SUCCESS || returncode == SQL_SUCCESS_WITH_INFO
		; returncode = SQLError(NULL, hdbc, NULL, szState, NULL, buf, MAX_ERROR_SIZE, NULL)) {
		wsprintf(largebuf, "\tState: %s", (LPSTR)szState);
		szWrite(largebuf, TRUE);
		wsprintf(largebuf, "\tError: %s", (LPSTR)buf);
		szWrite(largebuf, TRUE);
	}
	for(returncode = SQLError(NULL, NULL, hstmt, szState, NULL, buf, MAX_ERROR_SIZE, NULL)
		; returncode == SQL_SUCCESS || returncode == SQL_SUCCESS_WITH_INFO
		; returncode = SQLError(NULL, NULL, hstmt, szState, NULL, buf, MAX_ERROR_SIZE, NULL)) {
		wsprintf(largebuf, "\tState: %s", (LPSTR)szState);
		szWrite(largebuf, TRUE);
		wsprintf(largebuf, "\tError: %s", (LPSTR)buf);
		szWrite(largebuf, TRUE);
	}
}

int FAR PASCAL ReturnCheck(RETCODE retExpected, RETCODE retReceived, LPSTR szFunction, LPSTR szFile, int iLine)
{
	char buf[MAX_STRING_SIZE];
//      extern int failed;

	if(retExpected == retReceived)
		return TRUE;

	szWrite("", TRUE);

	szWrite("\t", FALSE);
	szWrite(szFunction, TRUE);

	szWrite("\tExpected: ", FALSE);
	szWrite(RetcodeToChar(retExpected, buf), TRUE);

	szWrite("\tReceived: ", FALSE);
	szWrite(RetcodeToChar(retReceived, buf), TRUE);

	DisplayAllErrors();

	wsprintf(buf, "\t%s: %d", szFile, iLine);
	szWrite(buf, TRUE);

	szWrite("  --------  ", TRUE);

	lpSI->failed++;

	return FALSE;
}

void FAR PASCAL qtDisplayError(LPSTR szFunction, LPSTR buf, LPSTR szFile, int iLine)
{
//      extern int failed;
	char szTmp[MAX_STRING_SIZE];

	szWrite("", TRUE);
	szWrite("\t", FALSE);
	szWrite(szFunction, FALSE);
	szWrite("  FAILED", TRUE);
	szWrite("\t", FALSE);
	szWrite(buf, TRUE);

	wsprintf(szTmp, "\t%s: %d", szFile, iLine);
	szWrite(szTmp, TRUE);

	szWrite("  --------  ", TRUE);

	lpSI->failed++;

	return;
}

int FAR PASCAL FindError(LPSTR szFindState)
{
	char buf[MAX_STRING_SIZE];
	RETCODE returncode;
	char szState[6];
	int found = FALSE;

	for(returncode = SQLError(henv, NULL, NULL, szState, NULL, buf, MAX_STRING_SIZE, NULL)
		; !found && (returncode == SQL_SUCCESS || returncode == SQL_SUCCESS_WITH_INFO)
		; returncode = SQLError(henv, NULL, NULL, szState, NULL, buf, MAX_STRING_SIZE, NULL)) {
		found = lstrcmp(szState, szFindState) == 0;
	}

	for(returncode = SQLError(NULL, hdbc, NULL, szState, NULL, buf, MAX_STRING_SIZE, NULL)
		; !found && (returncode == SQL_SUCCESS || returncode == SQL_SUCCESS_WITH_INFO)
		; returncode = SQLError(NULL, hdbc, NULL, szState, NULL, buf, MAX_STRING_SIZE, NULL)) {
		found = lstrcmp(szState, szFindState) == 0;
	}

	for(returncode = SQLError(NULL, NULL, hstmt, szState, NULL, buf, MAX_STRING_SIZE, NULL)
		; !found && (returncode == SQL_SUCCESS || returncode == SQL_SUCCESS_WITH_INFO)
		; returncode = SQLError(NULL, NULL, hstmt, szState, NULL, buf, MAX_STRING_SIZE, NULL)) {
		found = lstrcmp(szState, szFindState) == 0;
	}

	return found;
}

char FAR PASCAL qtlower (char ch)
{
	if(ch >= 'A' && ch <= 'Z')
		return ch + 'a' - 'A';
	else
		return ch;
}

int FAR PASCAL qtestristr(LPCSTR szString, LPCSTR szFind)
{
	int i;
	LPSTR bufString = AllocateMemory(lstrlen(szString) + 1);
	LPSTR bufFind = AllocateMemory(lstrlen(szFind) + 1);

	i = 0;
	while(szString[i]) {
		bufString[i] = qtlower(szString[i]);
		i++;
	}

	i = 0;
	while(szFind[i]) {
		bufFind[i] = qtlower(szFind[i]);
		i++;
	}

	i = !!strstr(bufString, bufFind);

	ReleaseMemory(bufString);
	ReleaseMemory(bufFind);

	return i;

}

LPSTR FAR PASCAL qtMakeData(int row, FIELDINFO FAR *rgField, LPSTR buf)
{
	if(rgField->fAutoUpdate)
		return NULL;

	if(rgField->nullable && 0 == row) {
		lstrcpy(buf, "");
		return buf;
	}

	switch(rgField->wSQLType) {
		case SQL_CHAR:
		case SQL_VARCHAR:
		case SQL_LONGVARCHAR:
		case SQL_BINARY:
		case SQL_VARBINARY:
		case SQL_LONGVARBINARY:
			wsprintf(buf, "%d%d", row, row);
			break;
		case SQL_DECIMAL:
		case SQL_NUMERIC:
		case SQL_REAL:
		case SQL_FLOAT:
		case SQL_DOUBLE:
			if(row == 2 && !rgField->fUnsigned) /* make the second row negative for variety */
				wsprintf(buf, "-%d.%d", row, row);
			else
				wsprintf(buf, "%d.%d", row, row);
			break;

		case SQL_BIT:
			if(row > 2)
				wsprintf(buf, "%d", 1);
			else
				wsprintf(buf, "%d", 0);

			break;

		case SQL_SMALLINT:
		case SQL_INTEGER:
		case SQL_TINYINT:
		case SQL_BIGINT:
			if(row == 2 && !rgField->fUnsigned) /* make the second row negative for variety */
				wsprintf(buf, "-%d", row);
			else
				wsprintf(buf, "%d", row);
			break;

		case SQL_TIME:
			wsprintf(buf, "{t '01:%02d:%02d'}", row % 60, row % 60);
			break;
		case SQL_DATE:
			wsprintf(buf, "{d '1994-%02d-%02d'}", (row % 12) + 1, (row % 30) + 1);
			break;
		case SQL_TIMESTAMP:
			wsprintf(buf, "{ts '1994-%02d-%02d 01:%02d:%02d'}", (row % 12) + 1, (row % 30) + 1, row % 60, row % 60);
			break;
	}
	return buf;
}


VOID WINAPI szWrite(LPSTR szStr, BOOL fNewLine)
{
	szLogPrintf(lpSI, FALSE, szStr);
	if(fNewLine)
		szLogPrintf(lpSI, FALSE, (LPSTR)"\r\n");
}
