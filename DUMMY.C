#include "sample.h"

#define INSTAPI 
#define UNALIGNED 

typedef struct tagTRANS {                 // Translator structure
	LPSTR		lpszName;                  //   Pointer to name
	WORD		cbNameMax;                 //   Max name size
	UNALIGNED WORD FAR	*pcbNameOut;               //   Size of returned name
	LPSTR		lpszPath;                  //   Pointer to path
	WORD		cbPathMax;                 //   Max path size
	UNALIGNED WORD FAR	*pcbPathOut;               //   Size of returned path
	UNALIGNED DWORD FAR	*pvOption;                 //   Pointer to option word
} TRANS, FAR *LPTRANS;

/* SQLGetTranslator --------------------------------------------------------
	Description:
	Input      :
	Output     :
--------------------------------------------------------------------------*/
BOOL INSTAPI SQLGetTranslator(HWND			hwnd,
								LPSTR		lpszName,
								WORD		cbNameMax,
								UNALIGNED WORD FAR	*pcbNameOut,
								LPSTR		lpszPath,
								WORD		cbPathMax,
								UNALIGNED WORD FAR	*pcbPathOut,
								UNALIGNED DWORD FAR	*pvOption)
{
	TRANS	trans;
	BOOL	fSuccess;
	WORD cbOut;

#if ! OS2ODBC
	// Validate input arguments
	if (!hwnd       ||
		!lpszName   ||
		!lpszPath   ||
		!pvOption    )
		return FALSE;

	if (pcbPathOut == NULL)
		pcbPathOut = &cbOut;

	if (pcbNameOut == NULL)
		pcbNameOut = &cbOut;

	// Move user arguments into local structure
	trans.lpszName   = lpszName;
	trans.cbNameMax  = cbNameMax;
	trans.pcbNameOut = pcbNameOut;
	trans.lpszPath   = lpszPath;
	trans.cbPathMax  = cbPathMax;
	trans.pcbPathOut = pcbPathOut;
	trans.pvOption   = pvOption;

	Ctl3dRegister(hinst);

	// Invoke dialog, return FALSE if Ok not selected
	// NOTE: The cast to LPSTR is strange, but gets around an invalid
	//       compiler warning
	fSuccess = (IDOK == DialogBoxParam(hinst,
									MAKEINTRESOURCE(SELECTTRANS),
									hwnd == GetDesktopWindow() ? NULL : hwnd,
			  						SelectTransDlg,
									(LPARAM)((LPSTR)&trans)));

	Ctl3dUnregister(hinst);

#endif
	return fSuccess;
}


/* SQLWriteDSNToIni --------------------------------------------------------
  Description:	Write a datasource name to ODBC.INI
  Input      :
  Output     :
--------------------------------------------------------------------------*/
BOOL INSTAPI SQLWriteDSNToIni(LPCSTR lpszDSN,
			      LPCSTR lpszDriver)
{
#if ! OS2ODBC
	char	szDriver[_MAX_PATH];
#ifdef WIN32
	char	szDriver32[_MAX_PATH];
	char	szSuffix32[_MAX_PATH];
	LPSTR	lpszTemp;
#endif

	// Validity check parameters
	if (lpszDSN	    == NULL ||
		*lpszDSN	== '\0' ||
		lpszDriver  == NULL ||
		*lpszDriver == '\0' ||
		lstrlen(lpszDSN) > SQL_MAX_DSN_LENGTH ||
		!lstrcmpi(lpszDSN, INI_SDSOURCES))
		return FALSE;

	// If the data source already exists for a different driver,
	// first remove the data source
	if (lstrcmpi(lpszDSN, INI_SDEFAULT))
		SQLGetPrivateProfileString(INI_SDSOURCES,
								lpszDSN,
								EMPTYSTR,
								szDriver,
								sizeof(szDriver),
								ODBC_INI);
	else
		SQLGetPrivateProfileString(INI_SDEFAULT,
								INI_KDRIVER,
								EMPTYSTR,
								szDriver,
								sizeof(szDriver),
								ODBCINST_INI);
	if (*szDriver && lstrcmpi(lpszDriver, szDriver))
		SQLRemoveDSNFromIni(lpszDSN);

#ifdef WIN32

		//
		//	Build 'driver name (32 bit)' string for writing to INI files
		//

	lstrcpy(szDriver32, lpszDriver);
	lpszTemp = (LPSTR)szDriver32;
	while( *lpszTemp )
		lpszTemp = AnsiNext(lpszTemp);
	SetString(szSuffix32, sizeof(szSuffix32), IDS_SUFFIX32);
	lstrcpy(lpszTemp, szSuffix32);
#endif

	// Add non-default data sources to the list of data sources
	if (lstrcmpi(lpszDSN, INI_SDEFAULT)) {
		SQLWritePrivateProfileString(INI_SDSOURCES,
								lpszDSN,
								lpszDriver,
								ODBC_INI);
#ifdef WIN32

			//
			//	Add the same data source to the 16 bit INI file
			//

		WritePrivateProfileString(INI_S32DSOURCES,
								lpszDSN,
								szDriver32,
								ODBC_INI);
#endif
	}

	// Add data source name section
	SQLGetPrivateProfileString(lpszDriver,
							INI_KDRIVER,
							EMPTYSTR,
							szDriver, sizeof(szDriver),
							ODBCINST_INI);
	SQLWritePrivateProfileString(lpszDSN,
							INI_KDRIVER,
							szDriver,
							ODBC_INI);
#ifdef WIN32

		//
		//	Add the data source name section to the 16 bit INI file
		//

	WritePrivateProfileString(lpszDSN,
							INI_KDRIVER32,
							szDriver,
							ODBC_INI);
#endif

	if (!lstrcmpi(lpszDSN, INI_SDEFAULT))
	{
		SQLWritePrivateProfileString(lpszDSN,
								INI_KDRIVER,
								lpszDriver,
								ODBCINST_INI);
	}

	FlushIniFiles();

#endif
	return TRUE;
}


/* SQLRemoveDSNFromIni -----------------------------------------------------
  Description:	Remove a datasource name to ODBC.INI
  Input      :
  Output     :
--------------------------------------------------------------------------*/
BOOL INSTAPI SQLRemoveDSNFromIni(LPCSTR lpszDSN)
{
#if ! OS2ODBC
	char	szTemp[_MAX_PATH];

	// Validity check parameter
	if (lpszDSN == NULL ||
		*lpszDSN== '\0' ||
		!lstrcmpi(lpszDSN, INI_SDSOURCES))
		return FALSE;

	// Remove data source key and section
	SQLWritePrivateProfileString(INI_SDSOURCES,
							lpszDSN,
							NULL,
							ODBC_INI);
#ifndef WIN32

		//
		//	If there is a Driver32= keyword in this section, delete the
		//	section, but re-create it with just this keyword (the
		//	section is deleted first to get rid of any extraneous 16 bit
		//	parameters)
		//

	if( SQLGetPrivateProfileString(lpszDSN, INI_KDRIVER32, EMPTYSTR,
								   szTemp, sizeof(szTemp), ODBC_INI) )
	{
		SQLWritePrivateProfileString(lpszDSN, NULL, NULL, ODBC_INI);
		SQLWritePrivateProfileString(lpszDSN, INI_KDRIVER32,
									 szTemp, ODBC_INI);
	}
	else
		SQLWritePrivateProfileString(lpszDSN, NULL, NULL, ODBC_INI);
#else
	SQLWritePrivateProfileString(lpszDSN, NULL, NULL, ODBC_INI);
#endif
	if (!lstrcmpi(lpszDSN, INI_SDEFAULT))
	{
		SQLWritePrivateProfileString(lpszDSN, NULL, NULL, ODBCINST_INI);
	}

#ifdef WIN32

		//
		//	Remove the necessary info from the 16 bit INI files, too
		//

	WritePrivateProfileString(INI_S32DSOURCES, lpszDSN, NULL, ODBC_INI);

		//
		//	If there is a Driver= keyword in this section, only remove
		//	the Driver32 keyword-value pair; otherwise remove the whole
		//	section
		//

	if( GetPrivateProfileString(lpszDSN, INI_KDRIVER, EMPTYSTR,
								szTemp, 2, ODBC_INI) )
		WritePrivateProfileString(lpszDSN, INI_KDRIVER32, NULL, ODBC_INI);
	else
		WritePrivateProfileString(lpszDSN, NULL, NULL, ODBC_INI);
#endif	

	FlushIniFiles();

#endif
	return TRUE;
}
