/**************************************************************
COPYRIGHT © 1990-95, VISIGENIC SOFTWARE, INC., 
*************************************************************/
// -----------------------------------------------------------------------------
//      regist.c
//
//         This module implements a custom dialog box that allows the user
//         to enter their name, company and serial number. The name, company
//         and serial number is then confirmed in a confirmation dialog box.
//         Finally, the serial number is validated.
//

// Includes --------------------------------------------------------------------
#ifdef  OS2ODBC
#include	"win2os2.h"
#else
#include	<windows.h>
#include	<windowsx.h>

#ifndef WIN32
#include	<w16macro.h>
#endif

#include	<ctl3d.h>

#include	<stdlib.h>
#include    <stdio.h>
#include    <ctype.h>
#endif

#include	<odbcinst.h>

#ifdef WIN32
#if OS2ODBC
#define EXPFUNC
#define INTFUNC
#define	LRESULT	LONG
#else
#define EXPFUNC	__stdcall
#define INTFUNC	__stdcall
#endif
#else
#ifndef EXPORT
#define EXPORT		_export
#endif

#define EXPFUNC	EXPORT CALLBACK
#define INTFUNC	PASCAL
#endif

#include    "drvsetup.h"
#include	"regist.h"

// Constants -------------------------------------------------------------------

// Globals ---------------------------------------------------------------------
extern HINSTANCE	hinst;                 // Instance handle
extern BOOL		    fAuto;                 // /AUTO requested
extern BOOL		    fAutoCtl3d;            // Ctl3d in auto-subclass mode
/*
char		szINF[_MAX_PATH];              // INF path
char		szSrc[_MAX_PATH];              // Source path
char		szODBC_INI[_MAX_PATH];         // ODBC.INI path
char		szODBCINST_INI[_MAX_PATH];     // ODBCINST.INI path
char		szTitle[STRLEN];               // Window title
*/

//------------------------------------------------------------------------------
//  RegistrationUpdate
//
//      This function displays a Registration Information dialog box. The user
//      can enter their name, company name and serial number and press the
//      CONTINUE button.
//
//      A Registration Confirmation dialog box is then displayed which allows
//      the user to confirm the information entered.  If the information
//      entered is not correct, the original Registration Information dialog
//      is displayed again.
//
//      Next the serial number is validated.
//
//  PARAMETERS:
//      hwnd    The owning window
//
BOOL INTFUNC RegistrationUpdate(HWND hwnd)
{
    int result;

    // Until the user Confirms Registration by saying YES
    // keep displaying the Registration dialog box.
    // The use may, however, use the ESC key to back out
    // of registration and, ultimately, the setup.

    do
    {
        do
        {
            result = DialogBoxParam(hinst, (fAuto ? MAKEINTRESOURCE(GETREGINFO)
        									      : MAKEINTRESOURCE(GETREGINFO)),
        								    hwnd,
            								RegDlgProc,
            								CANCELNOTOK);
        }
        while (result == IDOK &&
               DialogBoxParam(hinst, (fAuto ? MAKEINTRESOURCE(REGINFO_CONFIRM)
        							        : MAKEINTRESOURCE(REGINFO_CONFIRM)),
        					          hwnd,
            				          RegDlgProc,
            				          CANCELOK) != IDYES);

        if (result == IDOK)
            if ((result = validate_psn((LPSTR) szSerialNum, (LPSTR) "ds")) != IDOK)
    			DialogBoxParam(hinst, MAKEINTRESOURCE(REGINFO_INVALID), hwnd,
    			               DlgProc, CANCELOK);

    }
    while (result != IDOK && result != IDCANCEL);

    return (result == IDOK ? FALSE : TRUE);
}

//------------------------------------------------------------------------------
//  RegDlgProc
//
//      This is the registration dialog handler.
//
BOOL EXPFUNC RegDlgProc(HWND hdlg, UINT msg, WPARAM wparam, LPARAM lparam)
{
	DWORD	fCancelOK;
    HWND    hwndMyCtl, hwndPushBut;

	if (msg > WM_USER)
#if OS2ODBC
		WinUpdateWindow(hdlg);
#else
		UpdateWindow(hdlg);
#endif

	fCancelOK = (DWORD)GetWindowLong(hdlg, DWL_USER);

    hwndPushBut = GetDlgItem(hdlg, IDOK);

	switch (msg) {
		case WM_INITDIALOG:
#if OS2ODBC
            SetDlgItemText(hdlg,ID_GETREGINFO_NAME, szName);
            SetDlgItemText(hdlg,ID_GETREGINFO_CO, szCompany);
            SetDlgItemText(hdlg,ID_GETREGINFO_SERNUM, szSerialNum);
#else
            hwndMyCtl = GetDlgItem(hdlg, ID_GETREGINFO_NAME);
            SendMessage(hwndMyCtl, WM_SETTEXT, (WPARAM) 0,
                        (LPARAM) ((LPSTR) szName));
            hwndMyCtl = GetDlgItem(hdlg, ID_GETREGINFO_CO);
            SendMessage(hwndMyCtl, WM_SETTEXT, (WPARAM) 0,
                        (LPARAM) ((LPSTR) szCompany));
            hwndMyCtl = GetDlgItem(hdlg, ID_GETREGINFO_SERNUM);
            SendMessage(hwndMyCtl, WM_SETTEXT, (WPARAM) 0,
                        (LPARAM) ((LPSTR) szSerialNum));
#endif

            // disable the continue button if all the fields are empty
            if (hwndPushBut &&
                (!lstrlen((LPSTR) szName) || !lstrlen((LPSTR) szCompany) ||
                 !lstrlen((LPSTR) szSerialNum)))
                EnableWindow(hwndPushBut,FALSE);

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
			WinSetFocus(HWND_DESKTOP,WinWindowFromID(hdlg,ID_GETREGINFO_NAME));
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
#endif	// ! OS2ODBC

		case WM_COMMAND:
			switch (GET_WM_COMMAND_ID(wparam, lparam))
			{
				case IDCANCEL:
					if (!fCancelOK)
					{
						if (DialogBoxParam(hinst, MAKEINTRESOURCE(ASKQUIT),
						  				   hdlg, DlgProc, CANCELOK) != IDOK)
        					EndDialog(hdlg, GET_WM_COMMAND_ID(wparam ,lparam));

						return TRUE;
					}

                    break;

                case IDYES:
                case IDNO:

					EndDialog(hdlg, GET_WM_COMMAND_ID(wparam ,lparam));

					return TRUE;

                    break;

				case IDX:
				case IDOK:

					EndDialog(hdlg, GET_WM_COMMAND_ID(wparam ,lparam));

					return TRUE;

                    break;

#if ! OS2ODBC
                case ID_GETREGINFO_NAME:

                    if (GET_WM_COMMAND_CMD(wparam ,lparam) == EN_CHANGE)
                    {
                        hwndMyCtl = GetDlgItem(hdlg, ID_GETREGINFO_NAME);
                        SendMessage(hwndMyCtl, WM_GETTEXT, _MAX_PATH,
                                    (LPARAM) ((LPSTR) szName));

                        if (!lstrlen((LPSTR) szName))
                            EnableWindow(hwndPushBut,FALSE);
                        else if (lstrlen((LPSTR) szCompany) &&
                                 lstrlen((LPSTR) szSerialNum))
                            EnableWindow(hwndPushBut,TRUE);

                        return TRUE;
                    }

                    break;

                case ID_GETREGINFO_CO:

                    if (GET_WM_COMMAND_CMD(wparam ,lparam) == EN_CHANGE)
                    {
                        hwndMyCtl = GetDlgItem(hdlg, ID_GETREGINFO_CO);
                        SendMessage(hwndMyCtl, WM_GETTEXT, _MAX_PATH,
                                    (LPARAM) ((LPSTR) szCompany));

                        if (!lstrlen((LPSTR) szCompany))
                            EnableWindow(hwndPushBut,FALSE);
                        else if (lstrlen((LPSTR) szName) &&
                                 lstrlen((LPSTR) szSerialNum))
                            EnableWindow(hwndPushBut,TRUE);

                        return TRUE;
                    }

                    break;
            
                case ID_GETREGINFO_SERNUM:

                    if (GET_WM_COMMAND_CMD(wparam ,lparam) == EN_CHANGE)
                    {
                        hwndMyCtl = GetDlgItem(hdlg, ID_GETREGINFO_SERNUM);
                        SendMessage(hwndMyCtl, WM_GETTEXT, _MAX_PATH,
                                    (LPARAM) ((LPSTR) szSerialNum));

                        if (!lstrlen((LPSTR) szSerialNum))
                            EnableWindow(hwndPushBut,FALSE);
                        else if (lstrlen((LPSTR) szName) &&
                                 lstrlen((LPSTR) szCompany))
                            EnableWindow(hwndPushBut,TRUE);

                        return TRUE;
                    }

                    break;
#endif
			}

			break;
#if OS2ODBC
		case WM_CONTROL:
			switch(SHORT1FROMMP(wparam)){
                		case ID_GETREGINFO_NAME:

                    		if (SHORT2FROMMP(wparam) == EN_CHANGE)
                    		{
					GetDlgItemText(hdlg,ID_GETREGINFO_NAME,szName,_MAX_PATH);
                        		if (!lstrlen((LPSTR) szName))
                            		EnableWindow(hwndPushBut,FALSE);
                        		else if (lstrlen((LPSTR) szCompany) &&
                                 		lstrlen((LPSTR) szSerialNum))
                            		EnableWindow(hwndPushBut,TRUE);
		
                    		}
		
                    		break;
		
                		case ID_GETREGINFO_CO:
		
                    		if (SHORT2FROMMP(wparam) == EN_CHANGE)
                    		{
					GetDlgItemText(hdlg,ID_GETREGINFO_CO,szCompany,_MAX_PATH);
                        		if (!lstrlen((LPSTR) szCompany))
                            		EnableWindow(hwndPushBut,FALSE);
                        		else if (lstrlen((LPSTR) szName) &&
                                 		lstrlen((LPSTR) szSerialNum))
                            		EnableWindow(hwndPushBut,TRUE);
		
                    		}
		
                    		break;
           		 
                		case ID_GETREGINFO_SERNUM:
		
                    		if (SHORT2FROMMP(wparam) == EN_CHANGE)
                    		{
					GetDlgItemText(hdlg,ID_GETREGINFO_SERNUM,szSerialNum,_MAX_PATH);
		
                        		if (!lstrlen((LPSTR) szSerialNum))
                            		EnableWindow(hwndPushBut,FALSE);
                        		else if (lstrlen((LPSTR) szName) &&
                                 		lstrlen((LPSTR) szCompany))
                            		EnableWindow(hwndPushBut,TRUE);
		
                    		}
		
			}	// end of WM_CONTROL switch
			return WinDefDlgProc(hdlg,msg,wparam,lparam);
			break;
#endif

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

//------------------------------------------------------------------------------
//
//  validate_psn
//                                        
//      This procedure validates the product serial number. There must be a 100%
//      match otherwise it will call the error handling procedure and abort the
//      process.
//
//      Verifies that the serial # has a valid format 00-000-0000000, matching
//      the dashes. The middle 3 digits should be 001 for the Driver Manager
//      and 002 for the Driver Set.
//
int INTFUNC validate_psn(LPSTR prod_ser_nbr, LPSTR prod_type)
{
    int result;

	if (lstrlen(prod_ser_nbr) != 14)
	    result = ID_BAD_SERNUM_LEN;
    else if (lstrcmp(prod_type,"dm") && lstrcmp(prod_type,"ds"))
        result = ID_INVALID_PRODUCT;
    else
    {
        result = ID_INVALID_SERNUM;
        
        if (isdigit(prod_ser_nbr[0]) && isdigit(prod_ser_nbr[1]) &&
            prod_ser_nbr[2] == '-' && prod_ser_nbr[6] == '-' &&
            isdigit(prod_ser_nbr[7]) && isdigit(prod_ser_nbr[8]) &&
            isdigit(prod_ser_nbr[9]) && isdigit(prod_ser_nbr[10]) &&
            isdigit(prod_ser_nbr[11]) && isdigit(prod_ser_nbr[12]) &&
            isdigit(prod_ser_nbr[13]))
        {
        	if (lstrcmp(prod_type,"dm") == 0 &&
        	    prod_ser_nbr[3] == '0' && prod_ser_nbr[4] == '0' &&
        		prod_ser_nbr[5] == '1')
        		    result = IDOK;
        	else if (lstrcmp(prod_type,"ds") == 0 &&
        	    prod_ser_nbr[3] == '0' && prod_ser_nbr[4] == '0' &&
        		prod_ser_nbr[5] == '2')
        		    result = IDOK;
        }
    }

    return (result);
}


//------------------------------------------------------------------------------
//
//  stamp_psn
//                                        
//      This procedure stamps the file by the specified path (pathin). If the
//      file cannot be opened, or if no stamp has been done then a value other
//      than 0 is returned.
//
int INTFUNC stamp_psn(LPSTR prod_ser_nbr, LPSTR pathin)
{
	FILE *fp;
	long pos;
	int c, cnt, result = 0;
	char path[_MAX_PATH], ser_num[_MAX_PATH];
    lstrcpy((LPSTR) path,pathin);

	if ((fp = fopen(path,"rb+")) != NULL)
    {
        result = ID_FILEERROR;
    	pos = -1;
    	cnt = 0;

    	while (result && (c = fgetc(fp)) != EOF)
    	{
    		if (c == '@')
    		{
    			if (pos == -1)
    			    pos = ftell(fp) - 1;

    			cnt++;
    		}
    		else if (cnt)
    		{
                	if (cnt == 50)          // Stamp not present, stamp it
                	{
        			fseek(fp,pos,0);
        			fprintf(fp,"%14s",prod_ser_nbr);
        			result = cnt = 0;
        		}
                	else if (cnt == 36)     // Stamp already exists, validate it
                	{
                    		fseek(fp,pos - 14,0);
                    		fscanf(fp,"%14s",ser_num);

                    		if (validate_psn(ser_num, (LPSTR) "ds") == IDOK)
                        	result = 0;
                	}
           		else
           		{
           			pos = -1;
           			cnt = 0;
           		}
            	}
    	}

        if (cnt == 50)
        {
   			fseek(fp,pos,0);
   			fprintf(fp,"%14s",prod_ser_nbr);
   			result = 0;
   	}

    	fclose(fp);
    }

#ifdef OS2ODBC
	return (0);	// always return success.
#else
	return (result);
#endif
}

#if ( defined(WIN32) && (! OS2ODBC) )
// Use the registry Luke...

#define MAX_KEY_LEN 256

const char szODBC_REG_KEY[] = "SOFTWARE\\ODBC\\";
const char szODBCIni[] = "ODBC.INI";
const char szODBCInstIni[] = "ODBCINST.INI";

    
int INTFUNC DMGetPrivateProfileString(LPCSTR lpszSection,
									  LPCSTR lpszEntry,
									  LPCSTR lpszDefault,
									  LPSTR  lpszRetBuffer,
									  int    cbRetBuffer,
									  LPCSTR lpszFilename)
{
	LPSTR  lpszKeyName;
	char   szBuffer[MAX_KEY_LEN];
	HKEY   hKey;
	HKEY   hKeyRoot;
	int    cchName;
	LONG   lRetCode;
	DWORD  cb, cbT;
	LPTSTR lpsz;
	DWORD  iValue;


	/* Does not make sense for NULL section or INI file. */
	if(lpszSection  == NULL || *lpszSection == '\0'
	|| lpszFilename == NULL || *lpszFilename== '\0'
	|| lpszRetBuffer== NULL || cbRetBuffer  <=   0)
		return 0;

	/* Find out if the key name is not longer than the buffer that we have.*/
	cchName = lstrlen(lpszSection) + lstrlen(lpszFilename) +3;
	if(cchName < sizeof(szBuffer))
		lpszKeyName = &szBuffer[0];
	else
		return 0;

	/* If the file name is ODBCINST.INI use HKEY_LOCAL_MACHINE as root
	   otherwise use HKEY_CURRENT_USER (we want each user to see installed
	   drivers.                                                            */
	hKeyRoot = (_strnicmp(lpszFilename, szODBCInstIni, strlen(szODBCInstIni)))
				? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE;
				
	/* Construct the key name.*/
	lstrcpy(lpszKeyName, szODBC_REG_KEY);
	lstrcat(lpszKeyName, lpszFilename);
	cchName = lstrlen(lpszKeyName);
	*(lpszKeyName+cchName)   = '\\';
	*(lpszKeyName+cchName+1) = '\0';
	lstrcat(lpszKeyName, lpszSection);

	/* Open the key corresponding to the section. */
	lRetCode = RegOpenKeyEx(hKeyRoot, 
							(LPTSTR)lpszKeyName,
							0,
							KEY_READ,
							&hKey);
	if(lRetCode != ERROR_SUCCESS)
	{
		cb = min((int)strlen(lpszDefault)+1, cbRetBuffer) -1;
		if(cb > 0)
			memcpy(lpszRetBuffer, lpszDefault, cb);
		*(lpszRetBuffer+cb) = '\0';
		return (int)cb;
	}

	cb = cbRetBuffer;

	/* Here we have to get a list of all entries which means geting a list of
	** value names for a key representing the section.
	*/
	if(lpszEntry == NULL)
	{
		lpsz = lpszRetBuffer;
		iValue = 0;
		do
		{
			cbT = cb;
			lRetCode =RegEnumValue(hKey,iValue++,lpsz,&cbT,NULL,NULL,NULL,NULL);
			if(lRetCode == ERROR_MORE_DATA)
			{
				CHAR rgchT[128];

				iValue--;
				cbT = sizeof(rgchT);
				lRetCode =RegEnumValue(hKey,iValue++,rgchT,&cbT,NULL,NULL,NULL,NULL);
				if(lRetCode == ERROR_SUCCESS)
				{
					cbT = (cb > 0) ? (cb-1) : 0;
					if(cbT > 0)
						memcpy(lpsz, &rgchT[0], cbT);
					if(cb > 0)
						*(lpsz+cbT) = '\0';
				}
				else
					cbT = cb;
			}
			if(lRetCode == ERROR_SUCCESS)
			{
				cb = ((cb-cbT) > 0) ? (cb-cbT-1) : 0;
				if(cb <= 1)
				{
					if(cbRetBuffer >= 2)
						*(lpszRetBuffer+cbRetBuffer-2) = '\0';
					cb = 1;
					break;					
				}
				lpsz += cbT+1;
			}
		} while(lRetCode == ERROR_SUCCESS);
		*(lpszRetBuffer+cbRetBuffer-cb) = '\0';
		cb = cbRetBuffer-cb;
	}

	/* Just get the value for a given entry. */
	else
	{
		lRetCode = RegQueryValueEx( hKey,
									(LPTSTR)lpszEntry,
									NULL,
									NULL,
									(LPBYTE)lpszRetBuffer,
									&cb);	
		if(lRetCode == ERROR_MORE_DATA)
		{
			cb = cbRetBuffer-1;
			*(lpszRetBuffer+cb) = '\0';
		}
		else if(lRetCode != ERROR_SUCCESS)
		{
			cb = min((int)strlen(lpszDefault)+1, cbRetBuffer) -1;
			if(cb > 0)
				memcpy(lpszRetBuffer, lpszDefault, cb);
			*(lpszRetBuffer+cb) = '\0';
		}
		else
			cb--;	// REG_SZ counts the zero byte
	}

	RegCloseKey(hKey); 
	return (int)cb;
}
#endif

#if 0

This puts a foreign file into the windows directory.  First the Windows directory may not
exist and second we shouldn't be copying strange files there anyway.  Rethink and redo.

//------------------------------------------------------------------------------
//
//  SaveLicenseData
//                                        
//      This procedure creates the VSODBC.LIC file in \windows which currently
//      contains the user's Name and Company Name.
//
void INTFUNC SaveLicenseData()
{
	FILE *fp;

	if ((fp = fopen("c:\\windows\\vsodbc.lic","w+")) != NULL)
    {
    	fprintf(fp,"%s\n",szName);
        fprintf(fp,"%s",szCompany);
        fclose(fp);
    }
}

#endif
