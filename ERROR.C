/*********************************************************************
/*  print_err - Use SQLError to get error data, then print it.       *
/*********************************************************************/

#if OS2ODBC
#include 	"win2os2.h"
#endif
 
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MSG_LNG    256            /* Maximum message length          */

//#define max(a,b) (a>b?a:b)

int print_err(henv, hdbc, hstmt)
SQLHENV	henv;
SQLHDBC	hdbc;
SQLHSTMT	hstmt;
{
RETCODE	rc;			/* general return code for API		*/ 
UCHAR	szSqlState[MSG_LNG];	/* SQL state string			*/
SDWORD	pfNativeError;		/* Native error code			*/
UCHAR	szErrorMsg[MSG_LNG];	/* Error msg text buffer pointer	*/
SWORD	pcbErrorMsg;		/* Error msg text Available bytes	*/
char	msgtext[MSG_LNG];	/* message text work area		*/

rc = SQLError(henv, hdbc, hstmt, szSqlState, &pfNativeError, szErrorMsg, 
		MSG_LNG, &pcbErrorMsg);
if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
	{
	switch (rc)
		{
		case SQL_NO_DATA_FOUND:
			printf("SQLERROR() couldn't find text, RC=%d\n", rc);
			break;
		case SQL_ERROR:
			printf("SQLERROR() couldn't access text, RC=%d\n", rc);
			break;
		case SQL_INVALID_HANDLE:
			printf("SQLERROR() had invalid handle, RC=%d\n", rc);
			break;
		default:
			printf("SQLERROR() unknown return code, RC=%d\n", rc);
			break;
		}
		printf( msgtext );	/* display error message for user  */
	}
else
	printf("{error} STATE=%s, CODE=%ld, MSG=%s\n", szSqlState, 
			pfNativeError, szErrorMsg);
 
return (1);                    /* TRUE is secondary return code   */
}                                /* end print_err function          */
 
/*********************************************************************
/*  The following function is included for completeness, but is not  *
/*  relevant for understanding the function of ODBC.                 *
/*********************************************************************/

#define MAX_NUM_PRECISION 15

/*********************************************************************
/*  Define max length of char string representation of number as:    *
/*    =  max(precision) + leading sign + E + exp sign + max exp leng *
/*    =  15             + 1            + 1 + 1        + 2            *
/*    =  15 + 5                                                      *
/*********************************************************************/

#define MAX_NUM_STRING_SIZE (MAX_NUM_PRECISION + 5)

UDWORD display_size (coltype, collen, colname)
SWORD	coltype;
UDWORD	collen;
UCHAR	*colname;

{
switch (coltype)
	{
	case SQL_CHAR:
	case SQL_VARCHAR:
	case SQL_DATE:
	case SQL_TIMESTAMP:
	case SQL_BIT:
		return(max((int) collen, (int) strlen((char *) colname)));
	case SQL_SMALLINT:
	case SQL_INTEGER:
	case SQL_TINYINT:
		return(max((int) collen+1, (int) strlen((char *) colname)));
	case SQL_DECIMAL:
	case SQL_NUMERIC:
		return(max((int) collen+2, (int) strlen((char *) colname)));
	case SQL_REAL:
	case SQL_FLOAT:
	case SQL_DOUBLE:
		return(max((int) MAX_NUM_STRING_SIZE, (int)strlen((char *) colname)));
	case SQL_BINARY:
	case SQL_VARBINARY:
		return(max((int) 2*collen, (int) strlen((char *) colname)));
	case SQL_LONGVARBINARY:
	case SQL_LONGVARCHAR:
		printf("Unsupported datatype, %d\n", coltype);
		return (0);
	default:
		printf("Unknown datatype, %d\n", coltype);
		return (0);
	}                            /* end switch (coltype)            */
}                                /* end display_size function       */

/*********************************************************************
/*  Use K&R getline function to get an input line from stdin.        *
/*********************************************************************/
 
int getline (char s[], int lim)
{
int c, i;
 
for (i=0; i < lim-1 && (c=getchar()) != EOF && c!='\n'; ++i)
	s[i] = c;
s[i] = '\0';
return i;
}
