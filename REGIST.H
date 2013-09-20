/**************************************************************
COPYRIGHT © 1990-95, VISIGENIC SOFTWARE, INC.
*************************************************************/
/*
$Header:   X:\visiroot\odbc\archives\install\windows\drvsetup\regist.h__   1.0   30 Nov 1994 15:35:22   ric  $

$Log:   X:\visiroot\odbc\archives\install\windows\drvsetup\regist.h__  $
 * 
 *    Rev 1.0   30 Nov 1994 15:35:22   ric
 * Initial PVCS Revision
*/
/*%CPW_END%*/
//------------------------------------------------------------------------------
//      regist.h
//
//         This header file defines the constants used in regist.c.
//

// Prototypes ------------------------------------------------------------------
// Prototypes --------------------------------------------------------------
extern BOOL     EXPFUNC DlgProc(HWND, UINT, WPARAM, LPARAM);
extern LRESULT  EXPFUNC WndProc(HWND, UINT, WPARAM, LPARAM);

extern BOOL     INTFUNC AddDSources(HWND);
extern BOOL     INTFUNC AutoInstall(HWND);
extern void     INTFUNC CenterWindow(HWND);
extern void     INTFUNC Delay(void);
extern HPALETTE INTFUNC MakePalette(void);

extern char     szName[];             // Registration User Name
extern char     szCompany[];          // Registration Company Name
extern char     szSerialNum[];        // Registration Serial Number

BOOL INTFUNC RegistrationUpdate(HWND);
BOOL EXPFUNC RegDlgProc(HWND, UINT, WPARAM, LPARAM);
int INTFUNC validate_psn(LPSTR, LPSTR);
int INTFUNC stamp_psn(LPSTR, LPSTR);
void INTFUNC SaveLicenseData();

//-----------------------------------------------------------------------------
