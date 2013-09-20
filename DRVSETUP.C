/*--------------------------------------------------------------------------
  DRVSETUP.C -- ODBC Setup
							
  COPYRIGHT © 1990-95, VISIGENIC SOFTWARE, INC.
--------------------------------------------------------------------------*/

// Includes ----------------------------------------------------------------
#if ! OS2ODBC
#include	<windows.h>
#include	<windowsx.h>
#ifndef WIN32
#include	<w16macro.h>
#endif
#include	<ctl3d.h>

#include	<stdlib.h>
#endif

#if OS2ODBC
#include	<win2os2.h>

#endif

#ifdef WIN32
#if OS2ODBC
#define EXPFUNC
#define INTFUNC	
#define	LRESULT	LONG
#else
#define EXPFUNC	__stdcall
#define INTFUNC	__stdcall
#endif	// end of OS2ODBC
#else
#ifndef EXPORT
#define EXPORT		_export
#endif

#define EXPFUNC	EXPORT CALLBACK
#define INTFUNC	PASCAL
#endif

#include	"drvsetup.h"
#include	"regist.h"

// Prototypes --------------------------------------------------------------
BOOL     EXPFUNC DlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT  EXPFUNC WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL     INTFUNC AddDSources(HWND);
BOOL     INTFUNC AutoInstall(HWND);
void     INTFUNC CenterWindow(HWND);
void     INTFUNC Delay(void);
HPALETTE INTFUNC MakePalette(void);
#ifdef OS2ODBC
void INTFUNC SetString(LPSTR lpsz, UINT  cb, WORD  iRes);
int INTFUNC DoMessage(HWND hwnd, LPCSTR lpszMsg, UINT fuStyle);
#define ISDASH(x)		((x) == '-')
#endif

#if ( defined(WIN32) && (! OS2ODBC) )
int INTFUNC DMGetPrivateProfileString(LPCSTR lpszSection,
									  LPCSTR lpszEntry,
									  LPCSTR lpszDefault,
									  LPSTR  lpszRetBuffer,
									  int    cbRetBuffer,
									  LPCSTR lpszFilename);
#endif

// Constants ---------------------------------------------------------------
const char ODBCCLASS[]	  = ODBCSETUPCLASS;

const char ODBC_INF[]     = "ODBC.INF";
const char ODBC_INI[]     = "ODBC.INI";
const char ODBCINST_INI[] = "ODBCINST.INI";

const char SW_AUTO[]      = "/AUTO";

const char INI_DEFAULT[]  = "Sybase10";
const char INI_DSOURCES[] = "ODBC Data Sources";
const char INI_DRIVERS[]  = "ODBC Drivers";
const char INI_KDRIVER[]  = "Driver";

const char EMPTYSTR[]     = "";
const char DSNKEY_FMT[]   = "DSN=%s";
const char KEY_FMT[]      = "%s=%s";

#define BUFSIZE		4096
#define STRLEN			256
#define DELAY			5000L

#define cxDEF			620
#define cyDEF			460

#define cPALETTESIZE	256

#define ISBLANK(x)	   	((x) == ' ')
#define ISSLASH(x)		((x) == '\\')
#define ISTAB(x)	   	((x) == '\t')
#define ISWHITE(x)	   	(ISBLANK(x) || ISTAB(x))

#define WIN32S			0x80000000

#ifdef OS2ODBC
// SQLConfigDataSource request flags
#define  ODBC_ADD_DSN     1               // Add data source
	/*
	* Functions in odbccp.dll
	*/
BOOL (*SQLInstallODBC) (HWND hwndParent, 
			LPCSTR lpszInfFile, 
			LPCSTR lpszSrcPath,
			LPCSTR     lpszDrivers);
BOOL (*SQLManageDataSources)    (HWND  hwndParent);
BOOL (*SQLGetAvailableDrivers)  (LPCSTR     lpszInfFile,
                                 LPSTR      lpszBuf,
                                 WORD       cbBufMax,
                                 WORD FAR * pcbBufOut);
BOOL (*SQLConfigDataSource)     (HWND       hwndParent,
                                 WORD       fRequest,
                                 LPCSTR     lpszDriver,
                                 LPCSTR     lpszAttributes);
	/*
	* Functions in os2util.dll
	*/
void *(*GlobalAllocPtr)		(long, int);
void (*GlobalFreePtr)		(void *);
int (*GetPrivateProfileString)	( LPCSTR lpszSection, LPCSTR lpszEntry, LPCSTR lpszDefault, LPSTR  lpszRetBuffer, int cbRetBuffer, LPCSTR lpszFilename);
FARPROC (*OS2GetProcAddressString) (HANDLE , LPCSTR );
HCURSOR	(*SetCursor)		(HCURSOR);
HCURSOR (*LoadCursor)		(HWND,LPCSTR);
LPSTR	(*AnsiNext)		(LPCSTR);
#endif

// Types -------------------------------------------------------------------
typedef struct tagGLOBALS {               // Main window global variables
	HBITMAP		hbitmap;                   //   Bitmap handle
#if ! OS2ODBC
	BITMAP		bm;                        //   Bitmap size
#endif
	HPALETTE	hpal;                      //   Palette handle
} GLOBALS, FAR *LPGLOBALS;


// Globals -----------------------------------------------------------------
#if OS2ODBC
ULONG	flFrameFlags = FCF_TITLEBAR | FCF_SYSMENU | FCF_ICON |
			FCF_SIZEBORDER | FCF_MINMAX | 
			FCF_SHELLPOSITION | FCF_TASKLIST;
BOOL	fSuccess;
HPS	hps;
HBITMAP	hbm;
RECTL	Rectl;
RECTL	rcl;
char	szsrcdrive[_MAX_DRIVE];
char	szsrcdir[_MAX_DIR];
char	szfname[_MAX_FNAME];
char	szext[_MAX_EXT];
HAB	hab;
#endif
HINSTANCE	hinst;                         // Instance handle
char		szINF[_MAX_PATH];              // INF path
char		szSrc[_MAX_PATH];              // Source path
char		szODBC_INI[_MAX_PATH];         // ODBC.INI path
char		szODBCINST_INI[_MAX_PATH];     // ODBCINST.INI path
char		szTitle[STRLEN];               // Window title
char        szName[_MAX_PATH];             // Registration User Name
char        szCompany[_MAX_PATH];          // Registration Company Name
char        szSerialNum[_MAX_PATH];        // Registration Serial Number
char        szAvailDvrs[STRLEN];          // Available drivers list
BOOL		fAuto;                         // /AUTO requested
BOOL		fAutoCtl3d;                    // Ctl3d in auto-subclass mode
BOOL	    AvailDvrsresult;


/* AddDSources -------------------------------------------------------------
	Description: add data sources by reading information from ODBC.INI

--------------------------------------------------------------------------*/
BOOL INTFUNC AddDSources(HWND hdlg)
{
	BOOL	fSuccess;
	HCURSOR	hcur;
	LPSTR	lpsz;
	LPSTR	lpszT;

	hcur = SetCursor(LoadCursor(NULL, IDC_WAIT));

	lpsz = GlobalAllocPtr(GHND, (3 * BUFSIZE) + (2 * STRLEN));

	// to see if there is a default data source
	if (GetPrivateProfileString(INI_DEFAULT, NULL, EMPTYSTR,
								lpsz, BUFSIZE, szODBC_INI)) {
		lstrcpy(lpsz, INI_DEFAULT);
		lpszT = lpsz + lstrlen(lpsz) + 1;
	}
	else
		lpszT = lpsz;

	// retrieve the list of data sources
	if (!GetPrivateProfileString(INI_DSOURCES, NULL, EMPTYSTR,
						  		lpszT, BUFSIZE-(lpszT-lpsz), szODBC_INI)) {
		LPSTR	lpszFmt;
		LPSTR	lpszMsg;

		lpszFmt = lpsz;
		lpszMsg = lpszFmt + _MAX_PATH;

		LoadString(hinst, IDS_BADODBC, lpszFmt, _MAX_PATH);
		wsprintf(lpszMsg, lpszFmt, (LPSTR)szODBC_INI);
		MessageBox(hdlg, lpszMsg, szTitle, MB_ICONEXCLAMATION | MB_OK);

		fSuccess = FALSE;
	}

	else {
		LPSTR	lpszBuf;
		LPSTR	lpszAttr;
		LPSTR	lpszDriver;
		LPSTR	lpszKey;
		LPSTR	lpszValue;
		int		cb;

		lpszBuf    = lpsz;
		lpszDriver = lpszBuf + BUFSIZE;
		lpszValue  = lpszDriver + STRLEN;

		fSuccess = TRUE;

		lpszAttr = lpszValue + STRLEN;
		// walk through all data sources
		for (; *lpsz; lpsz += lstrlen(lpsz) + 1) {

			if (lstrcmpi(lpsz, INI_DEFAULT))
				// read the driver information about this data source
				GetPrivateProfileString(INI_DSOURCES, lpsz, EMPTYSTR,
										lpszDriver, STRLEN, szODBC_INI);
			else
				// read the driver information on the default data source
				GetPrivateProfileString(INI_DEFAULT, INI_KDRIVER, EMPTYSTR,
										lpszDriver, STRLEN, szODBCINST_INI);

			lpszKey = lpszAttr + BUFSIZE;

			// list of keys for this data source
			GetPrivateProfileString(lpsz, NULL, EMPTYSTR,
									lpszKey, BUFSIZE, szODBC_INI);

			cb = wsprintf(lpszAttr, DSNKEY_FMT, lpsz);
			lpszAttr += cb + 1;

			for (; *lpszKey; lpszKey += lstrlen(lpszKey) + 1) {

				if (!lstrcmpi(lpszKey, INI_KDRIVER))
					continue;

				// value for lpszKey on data source lpsz
				GetPrivateProfileString(lpsz, lpszKey, EMPTYSTR,
										lpszValue, STRLEN, szODBC_INI);

				cb = wsprintf(lpszAttr, KEY_FMT, lpszKey, lpszValue);
				lpszAttr += cb + 1;
			}
			*lpszAttr = '\0';

			lpszAttr = lpszValue + STRLEN;

			fSuccess = SQLConfigDataSource(NULL, ODBC_ADD_DSN,
											lpszDriver, lpszAttr);

			if (!fSuccess)
				break;
		}

		if (!fSuccess) {
			LPSTR	lpszFmt;
			LPSTR	lpszMsg;

			lpszFmt = lpsz + lstrlen(lpsz) + 1;
			lpszMsg = lpszFmt + _MAX_PATH;

			LoadString(hinst, IDS_BADDS, lpszFmt, _MAX_PATH);
			wsprintf(lpszMsg, lpszFmt, lpsz);
			MessageBox(hdlg, lpszMsg, szTitle, MB_ICONEXCLAMATION | MB_OK);
		}
	}

	GlobalFreePtr(lpsz);

	SetCursor(hcur);

	return fSuccess;
}


/* AutoInstall -------------------------------------------------------------
	Description: read from ODBCINST.INI and install the ODBC components
				silently (no dialog boxes)
--------------------------------------------------------------------------*/
BOOL INTFUNC AutoInstall(HWND hdlg)
{
	BOOL	fSuccess;
#if OS2ODBC
	HPOINTER	oldhptr;
#else
	HCURSOR	hcur;
#endif
	LPSTR	lpsz;

#if OS2ODBC
	oldhptr = WinQueryPointer(HWND_DESKTOP);
	WinSetPointer(HWND_DESKTOP,
			WinQuerySysPointer(HWND_DESKTOP,SPTR_WAIT,FALSE) );
#else
	hcur = SetCursor(LoadCursor(NULL, IDC_WAIT));
#endif

	lpsz = GlobalAllocPtr(GHND, BUFSIZE);

	// retrieve a list of drivers from ODBCINST.INI
	if (!GetPrivateProfileString(INI_DRIVERS, NULL, EMPTYSTR,
								lpsz, BUFSIZE, szODBCINST_INI)) {
		LPSTR	lpszFmt;
		LPSTR	lpszMsg;

		lpszFmt = lpsz;
		lpszMsg = lpszFmt + _MAX_PATH;

		LoadString(hinst, IDS_BADODBCI, lpszFmt, _MAX_PATH);
		wsprintf(lpszMsg, lpszFmt, (LPSTR)szODBCINST_INI);
		MessageBox(hdlg, lpszMsg, szTitle, MB_OK);

		fSuccess = FALSE;
	}
	else
		fSuccess = SQLInstallODBC(hdlg, szINF, szSrc, lpsz); 
		// lpsz contains the list of drivers

	if (!fSuccess) {
		LPSTR	lpszMsg;

		lpszMsg = lpsz;

		LoadString(hinst, IDS_BADINST, lpszMsg, _MAX_PATH);
		MessageBox(hdlg, lpszMsg, szTitle, MB_ICONEXCLAMATION | MB_OK);
	}

	GlobalFreePtr(lpsz);

#if OS2ODBC
	WinSetPointer(HWND_DESKTOP,oldhptr);
#else
	SetCursor(hcur);
#endif

	return fSuccess;
}


/* CenterWindow ------------------------------------------------------------
	Description: place a window to the center of its parent window.
				 if parent window does not exist, place the to the 
				 center of desktop window
--------------------------------------------------------------------------*/
void INTFUNC CenterWindow(HWND hwnd)
{
	HWND	hwndParent;
	RECT	rc, rcScr, rcParent;
	int		cx, cy;

#if OS2ODBC
	SWP	swp;

	hwndParent = GetOwner(hwnd);
#else
	hwndParent = GetParent(hwnd);
#endif
	if (!hwndParent)
		hwndParent = GetDesktopWindow();

	GetWindowRect(hwnd, &rc);
	cx = rc.right  - rc.left;
	cy = rc.bottom - rc.top;

	GetWindowRect(hwndParent, &rcParent);
	rc.top    = rcParent.top  + (((rcParent.bottom - rcParent.top) - cy) >> 1);
	rc.left   = rcParent.left + (((rcParent.right - rcParent.left) - cx) >> 1);
	rc.bottom = rc.top  + cy;
	rc.right  = rc.left + cx;
#if OS2ODBC
	if (WinQueryWindowPos(hwndParent,&swp)){
		rc.left += swp.x;
		rc.top += swp.y;
	}
#endif

	GetWindowRect(GetDesktopWindow(), &rcScr);
	if (rc.bottom > rcScr.bottom) {
		rc.bottom = rcScr.bottom;
		rc.top    = rc.bottom - cy;
	}
	if (rc.right  > rcScr.right) {
		rc.right = rcScr.right;
		rc.left  = rc.right - cx;
	}

	if (rc.left < 0) rc.left = 0;
	if (rc.top  < 0) rc.top  = 0;

	MoveWindow(hwnd, rc.left, rc.top, cx, cy, TRUE);
	return;
}


/* Delay -------------------------------------------------------------------
	Description: Delay for DELAY tickcounts

--------------------------------------------------------------------------*/
void INTFUNC Delay(void)
{
	DWORD	cStart;
	HCURSOR	hcur;

	hcur = SetCursor(LoadCursor(NULL, IDC_WAIT));

	for (cStart=GetTickCount(); (GetTickCount()-cStart) < DELAY; );

	SetCursor(hcur);
	return;
}


/* DlgProc -----------------------------------------------------------------
	Description: a dialog procedure handles OK, CANCEL etc

--------------------------------------------------------------------------*/
BOOL EXPFUNC DlgProc(HWND hdlg, UINT msg, WPARAM wparam, LPARAM lparam)
{
	DWORD	fCancelOK;

#if ! OS2ODBC
	if (msg > WM_USER)
		WinUpdateWindow(hdlg);
#endif

	fCancelOK = (DWORD)GetWindowLong(hdlg, DWL_USER);

	switch (msg) {
		case WM_INITDIALOG:
			CenterWindow(hdlg);

			SetWindowLong(hdlg, DWL_USER, (LONG)lparam);

			if (!fAutoCtl3d)
#ifdef WIN32
				Ctl3dSubclassDlg(hdlg, CTL3D_ALL);
#else
				Ctl3dSubclassDlgEx(hdlg, CTL3D_ALL);
#endif
			
			if (fAuto)
				PostMessage(hdlg, WMU_DELAY, 0, 0L);
			return TRUE;

#if ! OS2ODBC
		case WM_SYSCOLORCHANGE:
			if (!fAutoCtl3d) 
				return Ctl3dColorChange();
			break;
#ifdef WIN32
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORMSGBOX:
		case WM_CTLCOLORSCROLLBAR:	
		case WM_CTLCOLORSTATIC:
			if (!fAutoCtl3d) 
				return (BOOL)Ctl3dCtlColorEx(msg, wparam, lparam);
			break;

		case WM_SETTEXT:
		case WM_NCPAINT:
		case WM_NCACTIVATE:
			if (!fAutoCtl3d) 
			{
				SetWindowLong(hdlg, DWL_MSGRESULT,
					Ctl3dDlgFramePaint(hdlg, msg, wparam, lparam));
				return TRUE;
			}
			break;
#endif
#endif

		case WM_COMMAND:
			switch (GET_WM_COMMAND_ID(wparam, lparam)) {
				case IDCANCEL:
					if (!fCancelOK) {
						if (IDOK == DialogBoxParam(hinst,
												MAKEINTRESOURCE(ASKQUIT),
						  						hdlg, DlgProc, CANCELOK))
							return TRUE;
					}

				case IDX:
				case IDOK:
					EndDialog(hdlg, GET_WM_COMMAND_ID(wparam ,lparam));
					return TRUE;
			}
			break;

		case WMU_DELAY:
			Delay();
#if OS2ODBC
			PostMessage(hdlg, WM_COMMAND, IDOK,MAKELONG(0,0));
#else
			PostMessage(hdlg, WM_COMMAND, GET_WM_COMMAND_MPS(IDOK,0,0));
#endif
			return TRUE;
#if OS2ODBC
		default:
			return WinDefDlgProc(hdlg,msg,wparam,lparam);
#endif
	}
	return FALSE;
}

#if ! OS2ODBC
/* MakePalette -------------------------------------------------------------
	Description: Make color palette for the setup background

--------------------------------------------------------------------------*/
HPALETTE INTFUNC MakePalette(void)
{
	LPLOGPALETTE	ppal;
	LPPALETTEENTRY	pent;
	HPALETTE		hpal;
	int	i;

	ppal = (LOGPALETTE FAR *)GlobalAllocPtr(GHND,
								sizeof(LOGPALETTE) +
									(sizeof(PALETTEENTRY) * cPALETTESIZE));

	ppal->palVersion    = 0x300;
	ppal->palNumEntries = cPALETTESIZE;

	pent = ppal->palPalEntry;
	for (i=cPALETTESIZE-1; i >= 0; i--, pent++) {
		pent->peRed   = 0;
		pent->peGreen = 0;
		pent->peBlue  = i;
		pent->peFlags = 0;
	}

	hpal = CreatePalette(ppal);

	GlobalFreePtr(ppal);

	return hpal;
}
#endif


/* WndProc -----------------------------------------------------------------
  Description:  Main window message handler

--------------------------------------------------------------------------*/
LRESULT EXPFUNC WndProc(HWND    hwnd,
                        UINT    msg,
                        WPARAM  wparam,
                        LPARAM  lparam)
{
    char *currDvr;
    char currDvrPath[_MAX_PATH];
    int stamp_result;
	LPGLOBALS	lpglb;
    BOOL result;
    UINT availlen;

#ifdef _WIN32
	DWORD	dwVersion;
#endif	//	_WIN32

	if (msg > WM_USER)
		WinUpdateWindow(hwnd);

	lpglb = (LPGLOBALS)GetWindowLong(hwnd, 0);

	switch (msg) {

		case WM_CREATE:
			CenterWindow(hwnd);

			lpglb = (LPGLOBALS)GlobalAllocPtr(GHND, sizeof(GLOBALS));

			SetWindowLong(hwnd, 0, (LONG)lpglb);
#if  OS2ODBC
			hps = WinBeginPaint(hwnd,NULLHANDLE,&rcl);
			hbm = GpiLoadBitmap(hps,NULLHANDLE,IDI_BITMAP,0L,0L);
			fSuccess = WinDrawBitmap(hps,
					hbm,
					NULL,	// draw whole bitmap
					(PPOINTL)&rcl,
					0L,
					0L,
					DBM_NORMAL|DBM_STRETCH);
			WinEndPaint(hps);
#else
			lpglb->hbitmap = LoadBitmap(hinst, MAKEINTRESOURCE(IDI_BITMAP));
			lpglb->hpal    = MakePalette();

			GetObject(lpglb->hbitmap, sizeof(BITMAP), &lpglb->bm);
#endif

//			PostMessage(hwnd, WMU_WELCOME, 0, 0L);
			return FALSE;
			break;

		case WMU_WELCOME:
			if (IDOK == DialogBoxParam(hinst,
										(fAuto
											? MAKEINTRESOURCE(AWELCOME)
											: MAKEINTRESOURCE(WELCOME)),
										hwnd,
										DlgProc,
										CANCELNOTOK))
				PostMessage(hwnd, WMU_INSTALL, 0, 0L);
			else
				PostMessage(hwnd, WMU_EXIT, EXITQUIT, 0L);
			return TRUE;

		case WMU_INSTALL:
#if ! OS2ODBC
#ifdef _WIN32
			//	Determine Operating System version
			dwVersion = GetVersion();

			if (LOBYTE(LOWORD(dwVersion)) <= 3) {

				// can not install 32 bit driver under Win32s
				if (dwVersion & WIN32S) { 
					LPSTR	lpszMsg;

					lpszMsg = GlobalAllocPtr(GHND, BUFSIZE);

					if( lpszMsg ) {
						LoadString(hinst, IDS_WIN32S, lpszMsg, _MAX_PATH);
						MessageBox(hwnd, lpszMsg, szTitle,
								   MB_ICONEXCLAMATION | MB_OK);

						GlobalFreePtr(lpszMsg);
					}

				   	PostMessage(hwnd, WMU_EXIT, EXITQUIT, 0L);
				}
			}
#endif	//	_WIN32
#endif	//	! OS2ODBC


            // Obtain Serial #, Name, and Company name
            // Validate Serial # for stamping purposes later
            // Failed validation results in an exit.

	if(!fAuto)
            if (RegistrationUpdate(hwnd))
            {
				PostMessage(hwnd, WMU_EXIT, EXITQUIT, 0L);
                return (TRUE);
            }
	    // obtain the list of drivers which the user may install.
            AvailDvrsresult = SQLGetAvailableDrivers(szINF, szAvailDvrs, STRLEN,
                                          &availlen);

	    // install silently or install interactively
	    // if it is successful, add the data source
	    if ((fAuto && AutoInstall(hwnd)) ||
		(!fAuto && SQLInstallODBC(hwnd, szINF, szSrc, NULL)))
            {
            //    SaveLicenseData();  // load Name and Company into VSODBC.LIC

                if (fAuto)
        			PostMessage(hwnd, WMU_DSOURCE, 0, 0L);
                else
    				PostMessage(hwnd, WMU_STAMP, 0, 0L);
            }
			else
				PostMessage(hwnd, WMU_EXIT, EXITQUIT, 0L);
			return TRUE;

        case WMU_STAMP:
            // Stamp the loaded drivers with the validated Serial #
	    result = AvailDvrsresult;
	    if (AvailDvrsresult)
            {
		
                currDvr = szAvailDvrs;
                stamp_result = 0;

                while (*currDvr && !stamp_result)
                {
#if ( defined(WIN32) && (! OS2ODBC) )
                    if (DMGetPrivateProfileString(currDvr, "Driver", "",
                                                currDvrPath, _MAX_PATH,
                                                "odbcinst.ini"))
#else
                    if (GetPrivateProfileString(currDvr, "Driver", "",
                                                currDvrPath, _MAX_PATH,
                                                "odbcinst.ini"))
#endif
                        stamp_result = stamp_psn(szSerialNum, currDvrPath);

                    for (;*currDvr; currDvr++);
                    currDvr++;
                }

                if (stamp_result)
                {
        			DialogBoxParam(hinst, MAKEINTRESOURCE(STAMP_FAILED), hwnd,
    			               DlgProc, CANCELOK);
                    result = FALSE;
                }
            }

            if (result == TRUE)
    			PostMessage(hwnd, WMU_DSOURCE, 0, 0L);
			else
				PostMessage(hwnd, WMU_EXIT, EXITQUIT, 0L);

            return TRUE;

		case WMU_DSOURCE:
			if (fAuto)
				PostMessage(hwnd,
							WMU_EXIT,
							(AddDSources(hwnd)
								? AEXITSUCCESS
								: AEXITFAILURE),
							0L);
			else {
				SQLManageDataSources(hwnd);
				PostMessage(hwnd, WMU_EXIT, EXITSUCCESS, 0L);
			}
			return TRUE;

		case WMU_EXIT:
			fAuto = (wparam == AEXITSUCCESS);
			DialogBoxParam(hinst, MAKEINTRESOURCE(wparam),
							hwnd, DlgProc, CANCELOK);
			PostMessage(hwnd, WM_CLOSE, 0, 0L);
			return TRUE;
#if OS2ODBC
		case WM_SIZE:
			Rectl.xLeft = Rectl.yBottom = 0;
			Rectl.xRight = SHORT1FROMMP(lparam);
			Rectl.yTop = SHORT2FROMMP(lparam);
			break;

#endif

		case WM_PAINT: {
#if OS2ODBC

			hps = WinBeginPaint (hwnd,0,&rcl);
			fSuccess = WinDrawBitmap(hps,
					hbm,
					NULL,	// draw whole bitmap
					(PPOINTL)&Rectl,
					0L,
					0L,
					DBM_NORMAL|DBM_STRETCH);
/*
			WinFillRect(hps,&Rectl,CLR_GREEN);
*/
			WinEndPaint(hps);
			break;
		}
#else
			PAINTSTRUCT	ps;

		 	BeginPaint(hwnd, &ps);

			// Paint blue background
			{	HPALETTE	hpal;
				HBRUSH		hbr;
				RECT		rc;
				int			cy;
				int			i;

				hpal = SelectPalette(ps.hdc, lpglb->hpal, FALSE);
				RealizePalette(ps.hdc);

				GetClientRect(hwnd, &rc);

				cy = (rc.bottom - rc.top) / cPALETTESIZE;
				if ((rc.bottom - rc.top) % cPALETTESIZE)
					cy++;

				rc.bottom = rc.top + cy;

				for (i=0; i < cPALETTESIZE; i++) {

					hbr = CreateSolidBrush(PALETTEINDEX(i));

					FillRect(ps.hdc, &rc, hbr);

					DeleteObject(hbr);

					rc.top    += cy;
					rc.bottom += cy;
				}

				SelectPalette(ps.hdc, hpal, FALSE);
			}

			// Paint bitmap
			{	HDC		hdc;
				HBITMAP	hbitmap;

				hdc = CreateCompatibleDC(ps.hdc);
				hbitmap = SelectObject(hdc, lpglb->hbitmap);

				BitBlt(ps.hdc,
						4, 4, lpglb->bm.bmWidth, lpglb->bm.bmHeight, 
						hdc, 0, 0, 0x00220326);

				BitBlt(ps.hdc,
						0, 0, lpglb->bm.bmWidth, lpglb->bm.bmHeight, 
						hdc, 0, 0, SRCPAINT);

				SelectObject(hdc, hbitmap);
				DeleteDC(hdc);
			}

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_PALETTECHANGED:
			if ((HWND)wparam == hwnd)
				break;

		case WM_QUERYNEWPALETTE: {
			HDC			hdc;
			HPALETTE	hpal;
			UINT		i;

			hdc  = GetDC(hwnd);
			hpal = SelectPalette(hdc, lpglb->hpal, FALSE);

			i = RealizePalette(hdc);

			SelectPalette(hdc, hpal, FALSE);
			ReleaseDC(hwnd, hdc);

			if (i)
				InvalidateRect(hwnd, NULL, TRUE);
			return TRUE;
		}

		case WM_SYSCOLORCHANGE:
			Ctl3dColorChange();
			break;
#endif

		case WM_DESTROY:
#if ! OS2ODBC
			if (lpglb->hbitmap) DeleteObject(lpglb->hbitmap);
			if (lpglb->hpal)    DeleteObject(lpglb->hpal);
#endif

			GlobalFreePtr(lpglb);
#if OS2ODBC
			PostMessage(hwnd, WM_QUIT, 0, 0L);
#else
			PostQuitMessage(0);
#endif
			break;

		default:
#if OS2ODBC
			return WinDefWindowProc(hwnd, msg, wparam, lparam);
#else
			return DefWindowProc(hwnd, msg, wparam, lparam);
#endif
	}

	return 0;
}


/* WinMain -----------------------------------------------------------------
  Description:  Windows entry point

--------------------------------------------------------------------------*/
#if OS2ODBC
	main(int argc,char *argv[])
#else
int PASCAL WinMain(HANDLE hInstance,
                   HANDLE hPrevInstance,
                   LPSTR  szCommand,
                   int    nCmdShow)
#endif
{
	HWND    hwnd;
	MSG		msg;

#if OS2ODBC
#define	STDSTRLEN	512
	HAB	hab;
	HMQ	hmq;
	char	szSrcDrive[_MAX_DRIVE];
	char	szSrcExt[_MAX_EXT];
	char	szSrcDir[_MAX_DIR];
	char	szSrcFname[_MAX_FNAME];
	char	stdstr[STDSTRLEN];
	char	szFmt[STDSTRLEN];
	char	modulename[_MAX_PATH];
	char	objectname[_MAX_PATH];
	ULONG	SrcDriveNumber;
	APIRET	rc;
	HMODULE	hmodule;
	char	szCommand[512];
	int	i;
	HANDLE	hInstance;
	UINT	ErrorCode;
	HWND	hwndClient;

	hInstance=NULLHANDLE;
	szCommand[0]=0;
	for (i=0;i < argc;i++) {
		strcat(szCommand," ");
		strcat(szCommand,argv[i]);
	}
	hab = WinInitialize(0UL);
	hmq = WinCreateMsgQueue(hab,0UL);
#endif
	hinst = hInstance;

#if OS2ODBC
	_splitpath(argv[0],szSrcDrive,szSrcDir,szSrcFname,szSrcExt);
	SrcDriveNumber = szSrcDrive[0] - 'A' + 1;
	_makepath(modulename,szSrcDrive,szSrcDir,"os2util","dll");

	rc=DosLoadModule(objectname,sizeof(objectname),modulename,&hmodule);
	if (rc) {
		SetString(szFmt,sizeof(szFmt),IDS_DLLLOADERR);
		wsprintf(stdstr,szFmt,modulename);
		DoMessage(HWND_DESKTOP,stdstr,MB_OK | MB_ERROR | MB_SYSTEMMODAL) ;
		exit(1);
	}
	DosQueryProcAddr(hmodule, 0, "OS2GetProcAddressString", &OS2GetProcAddressString);
	DosQueryProcAddr(hmodule, 0, "SetCursor", &SetCursor);
	DosQueryProcAddr(hmodule, 0, "LoadCursor", &LoadCursor);
	DosQueryProcAddr(hmodule, 0, "GlobalAllocPtr", &GlobalAllocPtr);
	DosQueryProcAddr(hmodule, 0, "GlobalFreePtr", &GlobalFreePtr);
	DosQueryProcAddr(hmodule, 0, "GetPrivateProfileString", &GetPrivateProfileString);
	DosQueryProcAddr(hmodule, 0, "AnsiNext", &AnsiNext);

	_splitpath(argv[0],szSrcDrive,szSrcDir,szSrcFname,szSrcExt);
	SrcDriveNumber = szSrcDrive[0] - 'A' + 1;
	_makepath(modulename,szSrcDrive,szSrcDir,"odbccp","dll");

	rc=DosLoadModule(objectname,sizeof(objectname),modulename,&hmodule);
	if (rc) {
		SetString(szFmt,sizeof(szFmt),IDS_DLLLOADERR);
		wsprintf(stdstr,szFmt,modulename);
		DoMessage(HWND_DESKTOP,stdstr,MB_OK | MB_ERROR | MB_SYSTEMMODAL) ;
		exit(1);
	}
	SQLInstallODBC =  (BOOL (*)())OS2GetProcAddressString(hmodule,"SQLInstallODBC");
	SQLConfigDataSource =  (BOOL (*)())OS2GetProcAddressString(hmodule,"SQLConfigDataSource");
	SQLManageDataSources =  (BOOL (*)())OS2GetProcAddressString(hmodule,"SQLManageDataSources");
	SQLGetAvailableDrivers =  (BOOL (*)())OS2GetProcAddressString(hmodule,"SQLGetAvailableDrivers");

	// Initialize by registering a window class and ensuring that only a
	// single instance is executing
	// If a previous instance is executing, then bring it forward
	if (hwnd = WinWindowFromID(HWND_DESKTOP,IDS_FRAMETITLE)){
		WinSetFocus(HWND_DESKTOP,hwnd);
		WinSetActiveWindow(HWND_DESKTOP,hwnd);
		return FALSE;
	}
#else
	if (hwnd = FindWindow(ODBCCLASS, NULL)) {
		hwnd = GetLastActivePopup(hwnd);
		if (IsIconic(hwnd))
			OpenIcon(hwnd);
		else
			BringWindowToTop(hwnd);
		return FALSE;
	}
#endif

	// If this is the first instance, register and create the main window
	else {
#if ! OS2ODBC
		WNDCLASS	wc;
#endif
		LPSTR		lpszS, lpszD;

		// Get source file path
		for (lpszS=szCommand; ISWHITE(*lpszS); lpszS++);
		for (lpszD=szSrc; *lpszS && !ISWHITE(*lpszS); ) *lpszD++ = *lpszS++;
		*lpszD = '\0';

		// Check for /AUTO switch
		for (; ISWHITE(*lpszS); lpszS++);
		fAuto = !lstrcmpi(lpszS, SW_AUTO);

		if (!fAuto && *lpszS != 0)
		{	// if garbage on command line
			char szText[256], szTitle[256];

			LoadString (hInstance, IDS_FRAMETITLE, szTitle, sizeof(szTitle));
			LoadString (hInstance, IDS_BADOPT, szText, sizeof(szText));
			MessageBox (0, szText, szTitle, MB_OK);
			return FALSE;
		}

		// Get other file paths
#if OS2ODBC
		_splitpath(argv[0],szsrcdrive,szsrcdir,szfname,szext);
		_makepath(szINF,szsrcdrive,szsrcdir,"ODBC","INF");
		_makepath(szODBC_INI,szsrcdrive,szsrcdir,"ODBC","INI");
		_makepath(szODBCINST_INI,szsrcdrive,szsrcdir,"ODBCINST","INI");
		
#else
		GetModuleFileName(hinst, szINF, _MAX_PATH);
		for (lpszS=lpszD=szINF; *lpszS; lpszS++)
			if (ISSLASH(*lpszS))
				lpszD = lpszS;
		if (ISSLASH(*lpszD))
			lpszD++;
		*lpszD = '\0';
		lstrcpy(szODBC_INI, szINF);
		lstrcpy(szODBCINST_INI, szINF);

		lstrcat(szINF, ODBC_INF);
		lstrcat(szODBC_INI, ODBC_INI);
		lstrcat(szODBCINST_INI, ODBCINST_INI);
#endif

		// Register window class
#if OS2ODBC
		if (!WinRegisterClass(hab,
				ODBCCLASS,
				(PFNWP)WndProc,
				0UL,
				0UL)) 
		return FALSE;
		WinLoadString(hab,hinst, IDS_FRAMETITLE, sizeof(szTitle),szTitle);
		hwnd = WinCreateStdWindow(HWND_DESKTOP,
//			WS_VISIBLE,
			0L,	// INVISIBLE
			&flFrameFlags,
			ODBCCLASS,
			szTitle,
			0L,
			NULLHANDLE,
			(long)IDS_FRAMETITLE,
			&hwndClient);
		ErrorCode = WinGetLastError(hab);
	// Move the (invisible) window to the center of the screen
	{	RECT	rc;
		RECT	Framerc;
		int		nLeft, nBottom;

		GetWindowRect(HWND_DESKTOP, &rc);

		nLeft = (rc.xRight - rc.xLeft) >> 1;
		nBottom  = (rc.yTop - rc.yBottom) >> 1;
		GetWindowRect(hwnd,&Framerc);
		nLeft -= (Framerc.xRight - Framerc.xLeft)/2;
		nBottom -= (Framerc.yTop - Framerc.yBottom)/2 ;

		WinSetWindowPos(hwnd,HWND_TOP,nLeft,nBottom,0,0,
				SWP_SHOW | SWP_MOVE | SWP_MAXIMIZE);
	}
#else
		wc.style         = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc   = WndProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = sizeof(LPGLOBALS);
		wc.hInstance     = hinst;
		wc.hIcon         = LoadIcon(hinst, MAKEINTRESOURCE(IDI_ICON));
		wc.hCursor       = NULL;
		wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = ODBCCLASS;  
		if (!RegisterClass(&wc))
			return FALSE;

		// Create the main window
		LoadString(hinst, IDS_FRAMETITLE, szTitle, sizeof(szTitle));
		if (!(hwnd = CreateWindow(ODBCCLASS,
				  				szTitle,
				  				WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, CW_USEDEFAULT, cxDEF, cyDEF,
				  				HWND_DESKTOP,
				  				NULL,
				  				hinst,
				  				NULL)))
			return FALSE;

		Ctl3dRegister(hinst);
#ifndef WIN32
		fAutoCtl3d = Ctl3dAutoSubclass(hinst);
#endif
#endif	// end of OS2ODBC
	}

	// Show the window
#if OS2ODBC
	WinShowWindow(hwnd, TRUE);
#else
	ShowWindow(hwnd, nCmdShow);
#endif
	WinUpdateWindow(hwnd);
	if (IDOK == DialogBoxParam(hinst,
		(fAuto ? MAKEINTRESOURCE(AWELCOME) : MAKEINTRESOURCE(WELCOME)),
				hwnd,
				DlgProc,
				CANCELNOTOK))
				PostMessage(hwnd, WMU_INSTALL, 0, 0L);
			else
				PostMessage(hwnd, WMU_EXIT, EXITQUIT, 0L);

	// Get and dispatch messages
	while (GetMessage(&msg, (HWND)NULL, 0, 0)) {
#if ! OS2ODBC
		TranslateMessage(&msg);
#endif
		DispatchMessage(&msg);
	}

	Ctl3dUnregister(hinst);
#if OS2ODBC
	// Destory main (invisible) window

	WinDestroyWindow(hwnd);
	WinDestroyMsgQueue (hmq) ;
	WinTerminate (hab) ;
#endif

	return TRUE;
}

#if OS2ODBC

/* SetString ---------------------------------------------------------------
  Description:	Initialize a buffer with a string resource
  Input      :	lpsz ---- Buffer pointer
		cb ------ Buffer size
		iRes ---- Resource number
  Output     :	None (Buffer updated)
--------------------------------------------------------------------------*/
void INTFUNC SetString(LPSTR lpsz,
						UINT  cb,
						WORD  iRes)
{
	if (!LoadString(NULLHANDLE, iRes, lpsz, cb)) {
		*lpsz = '\0';
	}
	return;
}


/* DoMessage ---------------------------------------------------------------
	Description:
	Input      :
	Output     :
--------------------------------------------------------------------------*/
int INTFUNC DoMessage(HWND hwnd, LPCSTR lpszMsg, UINT fuStyle)
{
	char	szTitle[STDSTRLEN];
	LPSTR	lpsz;
	HWND	hwndParent;
	HWND	hwndNext;

	// BUG #2894
	if (hwnd == GetDesktopWindow())
		hwnd = NULLHANDLE;

	// Locate the top-level parent window
	for (hwndParent = hwnd;
		hwndNext    = GetParent(hwndParent);
		hwndParent  = hwndNext);

	// Get the window title
	// If a dash exists in the title, truncate everything past it
	// (this handles MDI windows)
	if (hwndParent) {
		GetWindowText(hwndParent, szTitle, sizeof(szTitle));
		for (lpsz=szTitle; *lpsz; lpsz = AnsiNext(lpsz)) {
			if (ISDASH(*lpsz)) {
				*lpsz = '\0';
				break;
			}
		}
	} else {
		SetString(szTitle, sizeof(szTitle), IDS_WINTITLE);
	}

	return MessageBox(hwnd, (char *)lpszMsg, szTitle, fuStyle);
}

#endif


