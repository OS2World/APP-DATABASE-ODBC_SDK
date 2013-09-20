/*
** SAMPLE.H - This is the ODBC sample driver include file.
**
**	This code is furnished on an as-is basis as part of the ODBC SDK and is
**	intended for example purposes only.
**
*/

//	-	-	-	-	-	-	-	-	-

#if ! OS2ODBC
#define WINVER 0x0300 // prevent Windows 3.1 feature usage
#include  <windows.h>                     // Windows include file
#include  <windowsx.h>					  // message crackers
#endif	// ! OS2ODBC

//	-	-	-	-	-	-	-	-	-

#if ! OS2ODBC
#include "w16macro.h"
#else
#include "win2os2.h"
#endif	// ! OS2ODBC

#include "sql.h"
#include "sqlext.h"

//	Definitions to be used in function prototypes.
//	The SQL_API is to be used only for those functions exported for driver
//		manager use.
//	The EXPFUNC is to be used only for those functions exported but used
//		internally, ie, dialog procs.
//	The INTFUNC is to be used for all other functions.
#ifdef WIN32
#if OS2ODBC
#define INTFUNC  
#define EXPFUNC 
#else
#define INTFUNC  __stdcall
#define EXPFUNC  __stdcall
#endif
#else
#define INTFUNC PASCAL
#define EXPFUNC __export CALLBACK
#endif

//	-	-	-	-	-	-	-	-	-

//	Environment information.

typedef	struct	tagENV {
    short   DummyEntry;
#if OS2ODBC     // ++
    Handle  savedHandle;	// to recover OS/2 handle from pointer
#endif  // OS2ODBC ++
}	ENV,
	FAR * LPENV;

//	-	-	-	-	-	-	-	-	-

//	Database connection information.  This is allocated by "SQLAllocConnect".

typedef struct	tagDBC {
    short   DummyEntry;
#if OS2ODBC     // ++
    Handle  savedHandle;	// to recover OS/2 handle from pointer
#endif  // OS2ODBC ++
}	DBC,
	FAR * LPDBC;

//	-	-	-	-	-	-	-	-	-

//  Statment information.  This is allocated by "SQLAllocStmt".

typedef struct	tagSTMT {
    short   DummyEntry;
#if OS2ODBC     // ++
    Handle  savedHandle;	// to recover OS/2 handle from pointer
#endif  // OS2ODBC ++
}	STMT,
	FAR *LPSTMT;

//	-	-	-	-	-	-	-	-	-

extern HINSTANCE NEAR s_hModule;	// DLL handle.

//	-	-	-	-	-	-	-	-	-

//	Resource defines for "SQLDriverConnect" dialog box.

#define	ID_LISTBOX	100

//	-	-	-	-	-	-	-	-	-

//	Resource defines for "Setup" dialog boxes.

#define CONFIGDSN                   1001
#define CONFIGDEFAULT               1002
#define EDRIVERCONNECT				1003
#define IDC_DSNAME                  400
#define IDC_DSNAMETEXT              401
#define IDC_OPTION1                 402
#define IDC_OPTION2                 403
#define IDC_DESC                    404
#define IDC_SELECT					405
#define IDC_TRANS_NAME				406

#define IDS_MSGTITLE                500
#define IDS_BADDSN                  501
