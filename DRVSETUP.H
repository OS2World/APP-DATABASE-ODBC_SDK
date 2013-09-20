
/*--------------------------------------------------------------------------
  Drvsetup.h -- Resource IDs used by ODBC Setup

  COPYRIGHT © 1990-95, VISIGENIC SOFTWARE, INC.
--------------------------------------------------------------------------*/


// Bitmaps and ICONs -------------------------------------------------------
#define IDI_ICON		100
#define IDI_BITMAP		200


// Controls ----------------------------------------------------------------
#define IDX				300


// String IDs --------------------------------------------------------------
#define IDS_FRAMETITLE	0x0010
#define IDS_BADODBCI	0x0011
#define IDS_BADODBC		0x0012
#define IDS_BADDS		0x0013
#define IDS_BADINST		0x0014
#define IDS_BADOPT		0x0015
#define IDS_WIN32S		0x0016
#define IDS_DLLLOADERR		0x0017
#define IDS_WINTITLE		0x0018


// Dialogs IDs -------------------------------------------------------------
#define WELCOME			1000
#define ASKQUIT			1001
#define EXITSUCCESS		1002
#define EXITQUIT		1003
#define EXITFAILURE		1004
#define AWELCOME		2000
#define AEXITSUCCESS	2001
#define AEXITFAILURE	2002

//
//	Common defines used by ODBC Setup and regist.c
//

#define ODBCSETUPCLASS "ODBCSETUP"

#define WMU_WELCOME	(WM_USER+1000)
#define WMU_INSTALL	(WM_USER+1001)
#define WMU_DSOURCE	(WM_USER+1002)
#define WMU_EXIT	(WM_USER+1003)
#define WMU_STAMP   (WM_USER+1004)

#define WMU_DELAY		(WM_USER+2000)

#define CANCELOK		((LPARAM)0x00000001L)
#define CANCELNOTOK	((LPARAM)0x00000000L)

// Ids for registration update dialog box
//
#define GETREGINFO                  29000
#define ID_GETREGINFO_NAME          29001
#define ID_GETREGINFO_CO            29002
#define ID_GETREGINFO_SERNUM        29009

#define REGINFO_CONFIRM             4000
#define REGINFO_INVALID             4010
#define STAMP_FAILED                4020

#define ID_INVALID_PRODUCT          2
#define ID_BAD_SERNUM_LEN           3
#define ID_INVALID_SERNUM           5
#define ID_FILEERROR                6
#define ID_NOSTAMPFOUND             7
