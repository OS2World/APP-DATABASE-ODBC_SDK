/*
** CONNECT.C - This is the ODBC sample driver code for
** allocation and connection.
**
**	This code is furnished on an as-is basis as part of the ODBC SDK and is
**	intended for example purposes only.
**
*/

//	-	-	-	-	-	-	-	-	-

#include "sample.h"

#if ! OS2ODBC
#include "ctl3d.h"
#endif

//	-	-	-	-	-	-	-	-	-

//	CONNECT.C
//
//	SQLC connection functions.

//	-	-	-	-	-	-	-	-	-

//	Allocate an environment (ENV) block.

RETCODE SQL_API SQLAllocEnv(
	HENV FAR *phenv)
{

	*phenv = (HENV FAR *)NULL;
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Allocate a DBC block.

RETCODE SQL_API SQLAllocConnect(
	HENV	 henv,
	HDBC FAR *phdbc)
{
	HGLOBAL	hdbc;

    hdbc = GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof (DBC));
	if (!hdbc || (*phdbc = (HDBC)GlobalLock (hdbc)) == SQL_NULL_HDBC)
	{
		GlobalFree (hdbc);	//	Free it if lock fails
		return SQL_ERROR;
	}
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLConnect(
	HDBC	  hdbc,
	UCHAR FAR *szDSN,
	SWORD	  cbDSN,
	UCHAR FAR *szUID,
	SWORD	  cbUID,
	UCHAR FAR *szAuthStr,
	SWORD	  cbAuthStr)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Sample Dialog Proc for SQLDriverConnect.
//	This has to be exported or subtle, yet very rude, things will happen.

BOOL FAR PASCAL FDriverConnectProc(
	HWND	hdlg,
	WORD	wMsg,
	WPARAM  wParam,
	LPARAM  lParam)
{
	switch (wMsg) {
	case WM_INITDIALOG:
#if ! OS2ODBC 
		Ctl3dRegister (s_hModule);
#ifdef WIN32
		Ctl3dSubclassDlg(hdlg, CTL3D_ALL);
#else
		Ctl3dSubclassDlgEx(hdlg, CTL3D_ALL);
#endif
#endif
		return TRUE;

#if ! OS2ODBC
#ifdef WIN32
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORSCROLLBAR:	
	case WM_CTLCOLORSTATIC:
		return (BOOL)Ctl3dCtlColorEx(wMsg, wParam, lParam);

	case WM_SETTEXT:
	case WM_NCPAINT:
	case WM_NCACTIVATE:
		SetWindowLong(hdlg, DWL_MSGRESULT,
			Ctl3dDlgFramePaint(hdlg, wMsg, wParam, lParam));
		return TRUE;
#endif

	case WM_SYSCOLORCHANGE:
		return Ctl3dColorChange();
#endif // OS2ODBC

	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam)) {
		case IDOK:

		case IDCANCEL:
#if ! OS2ODBC
			Ctl3dUnregister (s_hModule);
#endif // OS2ODBC
			EndDialog(hdlg, GET_WM_COMMAND_ID(wParam, lParam) == IDOK);
			return TRUE;
		}
		break;
#if OS2ODBC
		default:
			return (BOOL) WinDefDlgProc(hdlg,wMsg,wParam,lParam);
			break;
#endif
	}
	return FALSE;
}

//	-	-	-	-	-	-	-	-	-

//	This function as its "normal" behavior is supposed to bring up a
//	dialog box if it isn't given enough information via "szConnStrIn".  If
//	it is given enough information, it's supposed to use "szConnStrIn" to
//	establish a database connection.  In either case, it returns a
//	string to the user that is the string that was eventually used to
//	establish the connection.

RETCODE SQL_API SQLDriverConnect(
	HDBC	  hdbc,
	HWND	  hwnd,
	UCHAR FAR *szConnStrIn,
	SWORD	  cbConnStrIn,
	UCHAR FAR *szConnStrOut,
	SWORD	  cbConnStrOutMax,
	SWORD FAR *pcbConnStrOut,
	UWORD	  fDriverCompletion)
{
	short	iRet;
	BOOL	fPrompt = FALSE;

	if ((szConnStrIn == NULL) || (!cbConnStrIn) ||
		((cbConnStrIn == SQL_NTS) && (!szConnStrIn[0])))
		fPrompt = TRUE;
	else {
		//	Check connection string for completeness
		if (fDriverCompletion == SQL_DRIVER_COMPLETE ||
			fDriverCompletion == SQL_DRIVER_PROMPT)
			fPrompt = TRUE;
	}
	if (fPrompt) {
		//
		//	It is not necessary to call "MakeProcInstance" if you
		//	generate a dialog box from a DLL.
		//
#if OS2ODBC
		iRet = WinDlgBox(hwnd, hwnd, (PFNWP) FDriverConnectProc, 
		s_hModule, EDRIVERCONNECT, NULL);
#else
		iRet = DialogBox(s_hModule, MAKEINTRESOURCE(EDRIVERCONNECT), 
		hwnd, FDriverConnectProc);
#endif
		if ((!iRet) || (iRet == -1))
			return SQL_NO_DATA_FOUND;
	}
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLBrowseConnect(
	HDBC	  hdbc,
	UCHAR FAR *szConnStrIn,
	SWORD	  cbConnStrIn,
	UCHAR FAR *szConnStrOut,
	SWORD	  cbConnStrOutMax,
	SWORD FAR *pcbConnStrOut)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLDisconnect(
	HDBC	  hdbc)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLFreeConnect(
	HDBC	  hdbc)
{
	GlobalUnlock (GlobalPtrHandle((LPDBC)hdbc));
	GlobalFree (GlobalPtrHandle((LPDBC)hdbc));
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

RETCODE SQL_API SQLFreeEnv(
	HENV	  henv)
{

	return SQL_SUCCESS;
}
