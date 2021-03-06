/*
** SETUP.C - This is the ODBC sample driver code for
** setup.
**
**	This code is furnished on an as-is basis as part of the ODBC SDK and is
**	intended for example purposes only.
**
*/
/*--------------------------------------------------------------------------
  setup.c -- Sample ODBC setup

  This code demonstrates how to interact with the ODBC Installer.  These
  functions may be part of your ODBC driver or in a separate DLL.

  The ODBC Installer allows a driver to control the management of
  data sources by calling the ConfigDSN entry point in the appropriate
  DLL.  When called, ConfigDSN receives four parameters:

    hwndParent ---- Handle of the parent window for any dialogs which
                    may need to be created.  If this handle is NULL,
                    then no dialogs should be displayed (that is, the
                    request should be processed silently).

    fRequest ------ Flag indicating the type of request (add, configure
                    (edit), or remove).

    lpszDriver ---- Far pointer to a null-terminated string containing
                    the name of your driver.  This is the same string you
                    supply in the ODBC.INF file as your section header
                    and which ODBC Setup displays to the user in lieu
                    of the actual driver filename.  This string needs to
                    be passed back to the ODBC Installer when adding a
                    new data source name.

    lpszAttributes- Far pointer to a list of null-terminated attribute
                    keywords.  This list is similar to the list passed
                    to SQLDriverConnect, except that each key-value
                    pair is separated by a null-byte rather than a
                    semicolon.  The entire list is then terminated with
                    a null-byte (that is, two consecutive null-bytes
                    mark the end of the list).  The keywords accepted
                    should be those for SQLDriverConnect which are
                    applicable, any new keywords you define for ODBC.INI,
                    and any additional keywords you decide to document.

  ConfigDSN should return TRUE if the requested operation succeeds and
  FALSE otherwise.  The complete prototype for ConfigDSN is:

  BOOL FAR PASCAL ConfigDSN(HWND    hwndParent,
                            WORD    fRequest,
                            LPSTR   lpszDriver,
                            LPCSTR  lpszAttributes)

  Your setup code should not write to ODBC.INI directly to add or remove
  data source names.  Instead, link with ODBCINST.LIB (the ODBC Installer
  library) and call SQLWriteDSNToIni and SQLRemoveDSNFromIni.
  Use SQLWriteDSNToIni to add data source names.  If the data source name
  already exists, SQLWriteDSNToIni will delete it (removing all of its
  associated keys) and rewrite it.  SQLRemoveDSNToIni removes a data
  source name and all of its associated keys.

  For NT compatibility, the driver code should not use the
  Get/WritePrivateProfileString windows functions for ODBC.INI, but instead,
  use SQLGet/SQLWritePrivateProfileString functions that are macros (16 bit) or
  calls to the odbcinst.dll (32 bit).

--------------------------------------------------------------------------*/


// Includes ----------------------------------------------------------------
#include  "sample.h"					// Local include files

#if ! OS2ODBC
#include  "ctl3d.h"
#endif

#include  "odbcinst.h"					// ODBC installer prototypes
#include  <string.h>					// C include files
#include  <stdlib.h>

#ifdef OS2ODBC
extern   HAB   hab;
#endif

// Constants ---------------------------------------------------------------
#define MIN(x,y)      ((x) < (y) ? (x) : (y))

#define MAXPATHLEN      (255+1)           // Max path length
#define MAXKEYLEN       (15+1)            // Max keyword length
#define MAXDESC         (255+1)           // Max description length
#define MAXDSNAME       (32+1)            // Max data source name length

const char EMPTYSTR  []= "";
const char OPTIONON  []= "Yes";
const char OPTIONOFF []= "No";

// ODBC.INI keywords
const char ODBC_INI    []="ODBC.INI";     // ODBC initialization file
const char INI_KDESC   []="Description";  // Data source description
const char INI_KOPT1   []="Option1";      // First option
const char INI_KOPT2   []="Option2";      // Second option
const char INI_SDEFAULT[] = "Default";    // Default data source name
const char szTranslateName[] = "TranslationName";
const char szTranslateDLL[] = "TranslationDLL";
const char szTranslateOption[] = "TranslationOption";
const char szUnkTrans[] = "Unknown Translator";

// Attribute key indexes (into an array of Attr structs, see below)
#define KEY_DSN 		0
#define KEY_DESC		1
#define KEY_OPT1		2
#define KEY_OPT2		3
#define KEY_TRANSNAME	4
#define KEY_TRANSOPTION 5
#define KEY_TRANSDLL	6
#define NUMOFKEYS		7				// Number of keys supported

// Attribute string look-up table (maps keys to associated indexes)
static struct {
  char  szKey[MAXKEYLEN];
  int    iKey;
} s_aLookup[] = { "DSN",			KEY_DSN,
				  "DESC",			KEY_DESC,
				  "Description",	KEY_DESC,
				  "Option1",		KEY_OPT1,
				  "Option2",		KEY_OPT2,
				  "TranslateName",	KEY_TRANSNAME,
				  "TranslateDLL",	KEY_TRANSDLL,
				  "TranslateOption",KEY_TRANSOPTION,
				  "",				0
                };


// Types -------------------------------------------------------------------
typedef struct tagAttr {
	BOOL  fSupplied;
	char  szAttr[MAXPATHLEN];
} Attr, FAR * LPAttr;


// Globals -----------------------------------------------------------------
// NOTE:  All these are used by the dialog procedures
typedef struct tagSETUPDLG {
	HWND	hwndParent; 					// Parent window handle
	LPCSTR	lpszDrvr;						// Driver description
	Attr	aAttr[NUMOFKEYS];				// Attribute array
	char	szDSN[MAXDSNAME];				// Original data source name
	BOOL	fNewDSN;						// New data source flag
	BOOL	fDefault;						// Default data source flag

} SETUPDLG, FAR *LPSETUPDLG;



// Prototypes --------------------------------------------------------------
void INTFUNC CenterDialog	  (HWND    hdlg);
int  CALLBACK ConfigDlgProc	(HWND	hdlg,
					   WORD    wMsg,
					   WPARAM  wParam,
					   LPARAM  lParam);
void INTFUNC ParseAttributes (LPCSTR	lpszAttributes, LPSETUPDLG lpsetupdlg);
BOOL CALLBACK SetDSNAttributes(HWND	hwnd, LPSETUPDLG lpsetupdlg);

/* ConfigDSN ---------------------------------------------------------------
  Description:  ODBC Setup entry point
                This entry point is called by the ODBC Installer
                (see file header for more details)
  Input      :  hwnd ----------- Parent window handle
                fRequest ------- Request type (i.e., add, config, or remove)
                lpszDriver ----- Driver name
                lpszAttributes - data source attribute string
  Output     :  TRUE success, FALSE otherwise
--------------------------------------------------------------------------*/
BOOL CALLBACK ConfigDSN
						(HWND	 hwnd,
						 WORD	 fRequest,
						 LPCSTR  lpszDriver,
						 LPCSTR  lpszAttributes)
{
	BOOL  fSuccess;						   // Success/fail flag
	GLOBALHANDLE hglbAttr;
	LPSETUPDLG lpsetupdlg;

	// Allocate attribute array
	hglbAttr = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(SETUPDLG));
	if (!hglbAttr)
		return FALSE;
	lpsetupdlg = (LPSETUPDLG)GlobalLock(hglbAttr);

	// Parse attribute string
	if (lpszAttributes)
		ParseAttributes(lpszAttributes, lpsetupdlg);

	// Save original data source name
	if (lpsetupdlg->aAttr[KEY_DSN].fSupplied)
		lstrcpy(lpsetupdlg->szDSN, lpsetupdlg->aAttr[KEY_DSN].szAttr);
	else
		lpsetupdlg->szDSN[0] = '\0';

	// Remove data source
	if (ODBC_REMOVE_DSN == fRequest) {
		// Fail if no data source name was supplied
		if (!lpsetupdlg->aAttr[KEY_DSN].fSupplied)
			fSuccess = FALSE;

		// Otherwise remove data source from ODBC.INI
		else
			fSuccess = SQLRemoveDSNFromIni(lpsetupdlg->aAttr[KEY_DSN].szAttr);
	}

	// Add or Configure data source
	else {
		// Save passed variables for global access (e.g., dialog access)
		lpsetupdlg->hwndParent = hwnd;
		lpsetupdlg->lpszDrvr	 = lpszDriver;
		lpsetupdlg->fNewDSN	 = (ODBC_ADD_DSN == fRequest);
		lpsetupdlg->fDefault	 =
			!lstrcmpi(lpsetupdlg->aAttr[KEY_DSN].szAttr, INI_SDEFAULT);

		// Display the appropriate dialog (if parent window handle supplied)
		if (hwnd) {
			// Display dialog(s)
			  fSuccess = (IDOK == DialogBoxParam(s_hModule,
										  MAKEINTRESOURCE(CONFIGDSN),
										  hwnd,
										  ConfigDlgProc,
										  (LONG)(LPSTR)lpsetupdlg));
		}

		else if (lpsetupdlg->aAttr[KEY_DSN].fSupplied)
			fSuccess = SetDSNAttributes(hwnd, lpsetupdlg);
		else
			fSuccess = FALSE;
	}

	GlobalUnlock(hglbAttr);
	GlobalFree(hglbAttr);
	return fSuccess;
}


/* CenterDialog ------------------------------------------------------------
	Description:  Center the dialog over the frame window
	Input	   :  hdlg -- Dialog window handle
	Output	   :  None
--------------------------------------------------------------------------*/
void INTFUNC CenterDialog(HWND hdlg)
{
	HWND	hwndFrame;
	RECT	rcDlg, rcScr, rcFrame;
	int		cx, cy;

	hwndFrame = GetParent(hdlg);

	GetWindowRect(hdlg, &rcDlg);
	cx = rcDlg.right  - rcDlg.left;
	cy = rcDlg.bottom - rcDlg.top;

	GetClientRect(hwndFrame, &rcFrame);
	ClientToScreen(hwndFrame, (LPPOINT)(&rcFrame.left));
	ClientToScreen(hwndFrame, (LPPOINT)(&rcFrame.right));
	rcDlg.top    = rcFrame.top  + (((rcFrame.bottom - rcFrame.top) - cy) >> 1);
	rcDlg.left   = rcFrame.left + (((rcFrame.right - rcFrame.left) - cx) >> 1);
	rcDlg.bottom = rcDlg.top  + cy;
	rcDlg.right  = rcDlg.left + cx;

	GetWindowRect(GetDesktopWindow(), &rcScr);
	if (rcDlg.bottom > rcScr.bottom)
	{
		rcDlg.bottom = rcScr.bottom;
		rcDlg.top    = rcDlg.bottom - cy;
	}
	if (rcDlg.right  > rcScr.right)
	{
		rcDlg.right = rcScr.right;
		rcDlg.left  = rcDlg.right - cx;
	}

	if (rcDlg.left < 0) rcDlg.left = 0;
	if (rcDlg.top  < 0) rcDlg.top  = 0;

	MoveWindow(hdlg, rcDlg.left, rcDlg.top, cx, cy, TRUE);
	return;
}

/* ConfigDlgProc -----------------------------------------------------------
  Description:  Manage add data source name dialog
  Input      :  hdlg --- Dialog window handle
                wMsg --- Message
                wParam - Message parameter
                lParam - Message parameter
  Output     :  TRUE if message processed, FALSE otherwise
--------------------------------------------------------------------------*/
int CALLBACK ConfigDlgProc
						(HWND	hdlg,
						 WORD	wMsg,
						 WPARAM wParam,
						 LPARAM lParam)
{
	switch (wMsg) {
	// Initialize the dialog
	case WM_INITDIALOG:
	{
		LPSETUPDLG lpsetupdlg;
		LPCSTR	   lpszDSN;

#if ! OS2ODBC
		Ctl3dRegister (s_hModule);
#endif
		SetWindowLong(hdlg, DWL_USER, lParam);
#if ! OS2ODBC
#ifdef WIN32
		Ctl3dSubclassDlg(hdlg, CTL3D_ALL);
#else
		Ctl3dSubclassDlgEx(hdlg, CTL3D_ALL);
#endif
#endif
		CenterDialog(hdlg); 				// Center dialog

		lpsetupdlg = (LPSETUPDLG) lParam;
		lpszDSN    = lpsetupdlg->aAttr[KEY_DSN].szAttr;
		// Initialize dialog fields
		// NOTE: Values supplied in the attribute string will always
		//		 override settings in ODBC.INI
		SetDlgItemText(hdlg, IDC_DSNAME, lpszDSN);

		if (!lpsetupdlg->aAttr[KEY_DESC].fSupplied)
		SQLGetPrivateProfileString(lpszDSN, INI_KDESC,
			EMPTYSTR,
			lpsetupdlg->aAttr[KEY_DESC].szAttr,
			sizeof(lpsetupdlg->aAttr[KEY_DESC].szAttr),
			ODBC_INI);
		SetDlgItemText(hdlg, IDC_DESC, lpsetupdlg->aAttr[KEY_DESC].szAttr);

		if (!lpsetupdlg->aAttr[KEY_OPT1].fSupplied)
		SQLGetPrivateProfileString(lpszDSN, INI_KOPT1,
			EMPTYSTR,
			lpsetupdlg->aAttr[KEY_OPT1].szAttr,
			sizeof(lpsetupdlg->aAttr[KEY_OPT1].szAttr),
			ODBC_INI);
		CheckDlgButton(hdlg, IDC_OPTION1,
			!lstrcmpi(lpsetupdlg->aAttr[KEY_OPT1].szAttr, OPTIONON));

		if (!lpsetupdlg->aAttr[KEY_OPT2].fSupplied)
		SQLGetPrivateProfileString(lpszDSN, INI_KOPT2,
			EMPTYSTR,
			lpsetupdlg->aAttr[KEY_OPT2].szAttr,
			sizeof(lpsetupdlg->aAttr[KEY_OPT2].szAttr),
			ODBC_INI);
		CheckDlgButton(hdlg, IDC_OPTION2,
			!lstrcmpi(lpsetupdlg->aAttr[KEY_OPT2].szAttr, OPTIONON));

		// Set Translation fields
		if (!lpsetupdlg->aAttr[KEY_TRANSDLL].fSupplied)
		{
			SQLGetPrivateProfileString(lpsetupdlg->aAttr[KEY_DSN].szAttr,
				szTranslateDLL,
				EMPTYSTR,
				lpsetupdlg->aAttr[KEY_TRANSDLL].szAttr,
				sizeof(lpsetupdlg->aAttr[KEY_TRANSDLL].szAttr),
				ODBC_INI);
		}
		if (!lpsetupdlg->aAttr[KEY_TRANSOPTION].fSupplied)
		{
			SQLGetPrivateProfileString(lpsetupdlg->aAttr[KEY_DSN].szAttr,
				szTranslateOption,
				EMPTYSTR,
				lpsetupdlg->aAttr[KEY_TRANSOPTION].szAttr,
				sizeof(lpsetupdlg->aAttr[KEY_TRANSOPTION].szAttr),
				ODBC_INI);
		}
		if (!lpsetupdlg->aAttr[KEY_TRANSNAME].fSupplied)
		{
			if (!SQLGetPrivateProfileString(lpsetupdlg->aAttr[KEY_DSN].szAttr,
				szTranslateName,
				EMPTYSTR,
				lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr,
				sizeof(lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr),
				ODBC_INI))
			{
				if (*lpsetupdlg->aAttr[KEY_TRANSDLL].szAttr)
				{
					lstrcpy (lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr,
						szUnkTrans);
				}
			}
		}
		SetDlgItemText(hdlg, IDC_TRANS_NAME,
		  lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr);

		if (lpsetupdlg->fDefault)
		{
			EnableWindow(GetDlgItem(hdlg, IDC_DSNAME), FALSE);
			EnableWindow(GetDlgItem(hdlg, IDC_DSNAMETEXT), FALSE);
		}
		else
			SendDlgItemMessage(hdlg, IDC_DSNAME,
				 EM_LIMITTEXT, (WPARAM)(MAXDSNAME-1), 0L);
		SendDlgItemMessage(hdlg, IDC_DESC,
			EM_LIMITTEXT, (WPARAM)(MAXDESC-1), 0L);
		return TRUE;						// Focus was not set
    }

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
#endif

    // Process buttons
    case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam)) {
        // Ensure the OK button is enabled only when a data source name
        // is entered
        case IDC_DSNAME:
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE)
			{
				char	szItem[MAXDSNAME];		// Edit control text

				// Enable/disable the OK button
				EnableWindow(GetDlgItem(hdlg, IDOK),
					GetDlgItemText(hdlg, IDC_DSNAME,
						szItem, sizeof(szItem)));
				return TRUE;
			}
			break;

		//	Translator selection
		case IDC_SELECT:
		{
			WORD cbNameOut, cbPathOut;
			SDWORD vOption;
			LPSETUPDLG lpsetupdlg;

			lpsetupdlg = (LPSETUPDLG)GetWindowLong(hdlg, DWL_USER);
			GetDlgItemText (hdlg, IDC_TRANS_NAME,
				lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr,
				sizeof (lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr));
			vOption = atol (lpsetupdlg->aAttr[KEY_TRANSOPTION].szAttr);
			if (SQLGetTranslator (hdlg,
				lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr,
				sizeof (lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr)-1,
				&cbNameOut,
				lpsetupdlg->aAttr[KEY_TRANSDLL].szAttr,
				sizeof (lpsetupdlg->aAttr[KEY_TRANSDLL].szAttr)-1,
#if OS2ODBC
				&cbPathOut, (DWORD *) &vOption))
#else
				&cbPathOut, &vOption))
#endif
			{
				if (cbNameOut == 0)
				{	//	No translator selected
					*lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr = 0;
					*lpsetupdlg->aAttr[KEY_TRANSDLL].szAttr = 0;
					vOption = 0;
				}
				SetDlgItemText (hdlg, IDC_TRANS_NAME,
					lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr);
				_ltoa (vOption, lpsetupdlg->aAttr[KEY_TRANSOPTION].szAttr,
					10);
			}
			return TRUE;
		}

        // Accept results
		case IDOK:
		{
			LPSETUPDLG lpsetupdlg;

			lpsetupdlg = (LPSETUPDLG)GetWindowLong(hdlg, DWL_USER);
			// Retrieve dialog values
			if (!lpsetupdlg->fDefault)
				GetDlgItemText(hdlg, IDC_DSNAME,
					lpsetupdlg->aAttr[KEY_DSN].szAttr,
					sizeof(lpsetupdlg->aAttr[KEY_DSN].szAttr));
			GetDlgItemText(hdlg, IDC_DESC,
				lpsetupdlg->aAttr[KEY_DESC].szAttr,
				sizeof(lpsetupdlg->aAttr[KEY_DESC].szAttr));
			lstrcpy(lpsetupdlg->aAttr[KEY_OPT1].szAttr,
				(IsDlgButtonChecked(hdlg, IDC_OPTION1) ?
					OPTIONON : OPTIONOFF));
			lstrcpy(lpsetupdlg->aAttr[KEY_OPT2].szAttr,
				(IsDlgButtonChecked(hdlg, IDC_OPTION2) ?
					OPTIONON : OPTIONOFF));

			// Update ODBC.INI
			SetDSNAttributes(hdlg, lpsetupdlg);
        }

        // Return to caller
        case IDCANCEL:
#if ! OS2ODBC
			Ctl3dUnregister (s_hModule);
#endif
			EndDialog(hdlg, wParam);
			return TRUE;
		}
		break;
#if OS2ODBC
		default:
			return (int) WinDefDlgProc(hdlg,wMsg,wParam,lParam);
			break;
#endif
	}

	// Message not processed
	return FALSE;
}


/* ParseAttributes ---------------------------------------------------------
  Description:  Parse attribute string moving values into the aAttr array
  Input      :  lpszAttributes - Pointer to attribute string
  Output     :  None (global aAttr normally updated)
--------------------------------------------------------------------------*/
void INTFUNC ParseAttributes(LPCSTR lpszAttributes, LPSETUPDLG lpsetupdlg)
{
	LPCSTR	lpsz;
	LPCSTR	lpszStart;
	char	aszKey[MAXKEYLEN];
	int		iElement;
	int		cbKey;

	for (lpsz=lpszAttributes; *lpsz; lpsz++)
	{  //  Extract key name (e.g., DSN), it must be terminated by an equals
		lpszStart = lpsz;
		for (;; lpsz++)
		{
			if (!*lpsz)
				return;		// No key was found
			else if (*lpsz == '=')
				break;		// Valid key found
		}
		// Determine the key's index in the key table (-1 if not found)
		iElement = -1;
		cbKey	 = lpsz - lpszStart;
		if (cbKey < sizeof(aszKey))
		{
			register int j;

			_fmemcpy(aszKey, lpszStart, cbKey);
			aszKey[cbKey] = '\0';
			for (j = 0; *s_aLookup[j].szKey; j++)
			{
				if (!lstrcmpi(s_aLookup[j].szKey, aszKey))
				{
					iElement = s_aLookup[j].iKey;
					break;
				}
			}
		}

		// Locate end of key value
		lpszStart = ++lpsz;
		for (; *lpsz; lpsz++);

		// Save value if key is known
		// NOTE: This code assumes the szAttr buffers in aAttr have been
		//	   zero initialized
		if (iElement >= 0)
		{
			lpsetupdlg->aAttr[iElement].fSupplied = TRUE;
			_fmemcpy(lpsetupdlg->aAttr[iElement].szAttr,
				lpszStart,
				MIN(lpsz-lpszStart+1, sizeof(lpsetupdlg->aAttr[0].szAttr)-1));
		}
	}
	return;
}


/* SetDSNAttributes --------------------------------------------------------
  Description:  Write data source attributes to ODBC.INI
  Input      :  hwnd - Parent window handle (plus globals)
  Output     :  TRUE if successful, FALSE otherwise
--------------------------------------------------------------------------*/
BOOL INTFUNC SetDSNAttributes(HWND hwndParent, LPSETUPDLG lpsetupdlg)
{
	LPCSTR	lpszDSN;						// Pointer to data source name

	lpszDSN = lpsetupdlg->aAttr[KEY_DSN].szAttr;

	// Validate arguments
	if (lpsetupdlg->fNewDSN && !*lpsetupdlg->aAttr[KEY_DSN].szAttr)
		return FALSE;

	// Write the data source name
	if (!SQLWriteDSNToIni(lpszDSN, lpsetupdlg->lpszDrvr))
	{
		if (hwndParent)
		{
			char  szBuf[MAXPATHLEN];
			char  szMsg[MAXPATHLEN];

			LoadString(s_hModule, IDS_BADDSN, szBuf, sizeof(szBuf));
			wsprintf(szMsg, szBuf, lpszDSN);
			LoadString(s_hModule, IDS_MSGTITLE, szBuf, sizeof(szBuf));
			MessageBox(hwndParent, szMsg, szBuf, MB_ICONEXCLAMATION | MB_OK);
		}
		return FALSE;
	}

	// Update ODBC.INI
	// Save the value if the data source is new, if it was edited, or if
	// it was explicitly supplied
	if (hwndParent || lpsetupdlg->aAttr[KEY_DESC].fSupplied )
		SQLWritePrivateProfileString(lpszDSN,
			INI_KDESC,
			lpsetupdlg->aAttr[KEY_DESC].szAttr,
			ODBC_INI);

	if (hwndParent || lpsetupdlg->aAttr[KEY_OPT1].fSupplied )
		SQLWritePrivateProfileString(lpszDSN,
			INI_KOPT1,
			(lstrcmpi(lpsetupdlg->aAttr[KEY_OPT1].szAttr, OPTIONON) ?
			OPTIONOFF : OPTIONON),
			ODBC_INI);

	if (hwndParent || lpsetupdlg->aAttr[KEY_OPT2].fSupplied )
		SQLWritePrivateProfileString(lpszDSN,
			INI_KOPT2,
			(lstrcmpi(lpsetupdlg->aAttr[KEY_OPT2].szAttr, OPTIONON)	?
			OPTIONOFF : OPTIONON),
			ODBC_INI);

	if (hwndParent || lpsetupdlg->aAttr[KEY_TRANSNAME].fSupplied )
	{
		SQLWritePrivateProfileString(lpszDSN,
			szTranslateName,
			*lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr ?
			lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr: NULL,
			ODBC_INI);
		SQLWritePrivateProfileString(lpszDSN,
			szTranslateDLL,
			*lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr ?
			lpsetupdlg->aAttr[KEY_TRANSDLL].szAttr: NULL,
			ODBC_INI);
		SQLWritePrivateProfileString(lpszDSN,
			szTranslateOption,
			*lpsetupdlg->aAttr[KEY_TRANSNAME].szAttr ?
			lpsetupdlg->aAttr[KEY_TRANSOPTION].szAttr: NULL,
			ODBC_INI);
	}

	// If the data source name has changed, remove the old name
	if (lpsetupdlg->aAttr[KEY_DSN].fSupplied &&
		lstrcmpi(lpsetupdlg->szDSN, lpsetupdlg->aAttr[KEY_DSN].szAttr))
	{
		SQLRemoveDSNFromIni(lpsetupdlg->szDSN);
	}
	return TRUE;
}
