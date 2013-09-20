/***************************************************************************

	win2os2.h
	
	Copyright:	(c) 1995 by Visigenic Software, Inc., all rights reserved.
	
 ***************************************************************************/

#ifndef WIN2OS2_H
#define WIN2OS2_H

#define INCL_AVIO
#define INCL_BASE
#define INCL_BITMAPFILEFORMAT
#define INCL_DEV
#define INCL_DEVERRORS
#define INCL_DOS
#define INCL_DOSDATETIME
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_DOSERRORS
#define INCL_DOSFILEMGR
#define INCL_DOSINFOSEG
#define INCL_DOSMEMMGR
#define INCL_DOSMISC
#define INCL_DOSMODULEMGR
#define INCL_DOSMONITORS
#define INCL_DOSNMPIPES
#define INCL_DOSPROCESS
#define INCL_DOSQUEUES
#define INCL_DOSRESOURCES
#define INCL_DOSSEMAPHORES
#define INCL_DOSSESMGR
#define INCL_DOSSIGNALS
#define INCL_ERRORS
#define INCL_FONTFILEFORMAT
#define INCL_GPI
#define INCL_GPIBITMAPS
#define INCL_GPICONTROL
#define INCL_GPICORRELATION
#define INCL_GPIDEFAULTS
#define INCL_GPIERRORS
#define INCL_GPILCIDS
#define INCL_GPILOGCOLORTABLE
#define INCL_GPIMETAFILES
#define INCL_GPIPATHS
#define INCL_GPIPRIMITIVES
#define INCL_GPIREGIONS
#define INCL_GPISEGEDITING
#define INCL_GPISEGMENTS
#define INCL_GPITRANSFORMS
#define INCL_KBD
#define INCL_MOU
#define INCL_NOCOMMON
#define INCL_ORDERS
#define INCL_PIC
#define INCL_PM
#define INCL_SHLERRORS
#define INCL_SUB
#define INCL_VIO
#define INCL_WIN
#define INCL_WINACCELERATORS
#define INCL_WINATOM
#define INCL_WINBUTTONS
#define INCL_WINCATCHTHROW
#define INCL_WINCLIPBOARD
#define INCL_WINCOUNTRY
#define INCL_WINCURSORS
#define INCL_WINDDE
#define INCL_WINDIALOGS
#define INCL_WINENTRYFIELDS
#define INCL_WINERRORS
#define INCL_WINFRAMECTLS
#define INCL_WINFRAMEMGR
#define INCL_WINHEAP
#define INCL_WINHOOKS
#define INCL_WININPUT
#define INCL_WINLISTBOXES
#define INCL_WINLOAD
#define INCL_WINMENUS
#define INCL_WINMESSAGEMGR
#define INCL_WINMLE
#define INCL_WINPOINTERS
#define INCL_WINPROGRAMLIST
#define INCL_WINRECTANGLES
#define INCL_WINSCROLLBARS
#define INCL_WINSHELLDATA
#define INCL_WINSTATICS
#define INCL_WINSTDFILE
#define INCL_WINSWITCHLIST
#define INCL_WINSYS
#define INCL_WINTIMER
#define INCL_WINTRACKRECT
#define INCL_WINWINDOWMGR

#include	<os2.h>
#undef DATA_TYPE  // DATA_TYPE is used in struct.h and others
#include	<stdio.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<share.h>
#include	<limits.h>
#include	<io.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<direct.h>
#include	<process.h>
#include	<sys\stat.h>
#include <sys\types.h>
#include	<time.h>

#define FIELDINFO VFIELDINFO

typedef unsigned long Handle;

typedef Handle		HANDLE;
typedef struct key {
	char	keyname[_MAX_PATH];
	char	keytype;
	int	fhandle;
	int	keyindex;
	int	keylevel;
	int	freeslot;
	char	newline;
} *HKEY;
typedef HANDLE		HMEM;
typedef Handle		HINSTANCE;
typedef Handle		HGLOBAL;
typedef Handle		HICON;
typedef unsigned short	WORD;
typedef unsigned long 	DWORD;
typedef unsigned long 	*LPDWORD;
typedef char *        	LPSTR;
typedef const char *  	LPCSTR;
typedef void *        	LPVOID;
typedef BYTE *        	LPBYTE;
#define LPTSTR          LPCSTR
#define HPALETTE        HPAL

typedef struct {
     HMTX		semID;
     TID		OwningThread;   
} CRITICAL_SECTION;

// things needed for cpanel

typedef	long		_off_t;	
typedef	PCURSORINFO *HCURSOR;
/* OpenFile() Structure */
typedef struct tagOFSTRUCT
{
    BYTE cBytes;
    BYTE fFixedDisk;
    UINT nErrCode;
    BYTE reserved[4];
    char szPathName[128];
} OFSTRUCT;

typedef struct tagSIZE
{
    int cx;
    int cy;
} SIZE;
typedef SIZE*       PSIZE;
typedef SIZE NEAR* NPSIZE;
typedef SIZE FAR*  LPSIZE;

#define	MSG			QMSG
#define	CPL_DBLCLK		5
#define	IDC_WAIT		0

/* OpenFile() Flags */
#define OF_READ 	    0x0000
#define OF_WRITE	    0x0001
#define OF_READWRITE	    0x0002
#define OF_SHARE_COMPAT	    0x0000
#define OF_SHARE_EXCLUSIVE  0x0010
#define OF_SHARE_DENY_WRITE 0x0020
#define OF_SHARE_DENY_READ  0x0030
#define OF_SHARE_DENY_NONE  0x0040
#define OF_PARSE	    0x0100
#define OF_DELETE	    0x0200
#define OF_VERIFY	    0x0400      /* Used with OF_REOPEN */
#define OF_SEARCH	    0x0400	/* Used without OF_REOPEN */
#define OF_CANCEL	    0x0800
#define OF_CREATE	    0x1000
#define OF_PROMPT	    0x2000
#define OF_EXIST	    0x4000
#define OF_REOPEN	    0x8000

#define	_S_IREAD	S_IREAD
#define	_S_IWRITE	S_IWRITE

#define	FILE_OK_MSG	(WM_USER + 2000)

#define	GetOwner(a)	WinQueryWindow(a, QW_OWNER)
#define	DestroyWindow(hwnd)	WinDestroyWindow(hwnd)
#define  GetActiveWindow()	WinQueryActiveWindow(HWND_DESKTOP)
#define  GetWindowText(hwnd,lpsz,cbMax)	WinQueryWindowText(hwnd,cbMax,lpsz)
#define	PeekMessage(lpmsg,hwnd,uFilterfirst,uFilterLast,fuRemove) \
	WinPeekMsg(hab,lpmsg,hwnd,uFilterfirst,uFilterLast,fuRemove)
#define	GetMessage(lpmsg,hwnd,uFilterfirst,uFilterLast) \
	WinGetMsg(hab,lpmsg,hwnd,uFilterfirst,uFilterLast)
#define	DispatchMessage(lpmsg)	WinDispatchMsg(hab,lpmsg)
#define	PostMessage(hwnd,uMsg,wParam,lParam)	WinPostMsg(hwnd,(ULONG)uMsg,(MPARAM)wParam,(MPARAM)lParam)
#define	SendMessage(hwnd,uMsg,wParam,lParam)	OS2SendMsg(hwnd,(ULONG)uMsg,(MPARAM)wParam,(MPARAM)lParam)
#define  SetFocus(hwnd)	WinSetFocus(HWND_DESKTOP,hwnd)
#define	GetFocus()	WinQueryFocus(HWND_DESKTOP)
#define	InvalidateRect(hwnd,lprc,fErase)	WinInvalidateRect(hwnd,lprc,fErase)
#define  SetWindowText(hwnd,lpsz)	WinSetWindowText(hwnd,lpsz)
#define	GlobalAddAtom(lpszString)	WinAddAtom(WinQuerySystemAtomTable(),lpszString)
#define	GlobalDeleteAtom(lpszString)	WinDeleteAtom(WinQuerySystemAtomTable(),lpszString)
#define	BringWindowToTop(hwnd)	WinSetWindowPos(hwnd,HWND_TOP,0L,0L,0L,0L,SWP_ZORDER)
// WinShowCursor() actually needs HWND. Assuming HWND_DESKTOP. Prasad.
#define ShowCursor(bflag)  WinShowCursor(HWND_DESKTOP,bflag)

#define	ODBCCtl3dRegister(x)
#define	Ctl3dRegister(x)
#define  ODBCCtl3dUnregister(x)
#define  Ctl3dUnregister(x)
#define  ODBCCtl3dSubclassDlg(p1,p2)
#define  Ctl3dSubclassDlg(p1,p2)
#define	ODBCCtl3dCtlColor(p1,p2,p3)
#define	Ctl3dCtlColor(p1,p2,p3)
#define	ODBCCtl3dDlgFramePaint(p1,p2,p3,p4)
#define	Ctl3dDlgFramePaint(p1,p2,p3,p4)
#define  ODBCCtl3dColorChange()
#define  Ctl3dColorChange()
#define  ODBCCtl3dCtlColorEx(p1,p2,p3)
#define  Ctl3dCtlColorEx(p1,p2,p3)

#define  LOWORD(l)           ((WORD)(DWORD)(l))
#define  HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xFFFF))

#define  LBN_SELCHANGE  LN_SELECT

#define  LB_ADDSTRING 	LM_INSERTITEM
#define  LB_DELETESTRING LM_DELETEITEM
#define  LB_ERR 		   LIT_ERROR
#define  LB_FINDSTRING 	LM_SEARCHSTRING
#define  LB_FINDSTRINGEXACT   0x0008
#define  LB_GETCOUNT 	 LM_QUERYITEMCOUNT
#define  LB_GETCURSEL 	 LM_QUERYSELECTION
#define  LB_GETTEXT 	    LM_QUERYITEMTEXT
#define  LB_INSERTSTRING LM_INSERTITEM
#define  LB_RESETCONTENT LM_DELETEALL
#define  LB_GETTEXTLEN	LM_QUERYITEMTEXTLENGTH
#define  LB_SETSEL	    (WM_USER+6)
#define  LB_SETCURSEL	 (WM_USER+7)
#define  LB_GETITEMDATA   LM_QUERYITEMHANDLE
#define  LB_SETITEMDATA   LM_SETITEMHANDLE
#define  LB_DIR		     (WM_USER+14)

#define  CB_ADDSTRING 	LB_ADDSTRING
#define  CB_SETCURSEL 	LB_SETCURSEL
#define  CB_GETCURSEL 	LM_QUERYSELECTION
#define  CB_GETCOUNT    LM_QUERYITEMCOUNT
#define  CB_DELETESTRING LM_DELETEITEM
#define  CB_ERRSPACE	   LIT_MEMERROR
#define  CB_GETLBTEXT 	LM_QUERYITEMTEXT
#define  CB_GETLBTEXTLEN 	LM_QUERYITEMTEXTLENGTH
#define  CB_FINDSTRING 	LB_FINDSTRING
#define  CB_FINDSTRINGEXACT   LB_FINDSTRINGEXACT
#define  CBN_DBLCLK	   CN_ENTER
#define  CBN_DROPDOWN	CBN_LBSCROLL
#define  CBN_EDITCHANGE	CBN_EFCHANGE
#define  CB_SHOWDROPDOWN CBM_SHOWLIST
#define  CB_RESETCONTENT LM_DELETEALL
#define  CBN_SELCHANGE CBN_LBSELECT
#define  CB_LIMITTEXT   EM_SETTEXTLIMIT
#define  CB_DIR		     LB_DIR
// Not sure if the following two are supported for combo box
#define  CB_GETITEMDATA   LM_QUERYITEMHANDLE
#define  CB_SETITEMDATA   LM_SETITEMHANDLE

#define  EM_SETTABSTOPS   MLM_SETTABSTOP

#define HELP_CONTEXT		0
#define HELP_QUIT		   0
#define WM_SETREDRAW		0xffff
#define MB_ICONINFORMATION	MB_INFORMATION
#define MB_ICONSTOP        MB_ICONHAND

#define	_stat	stat

#define	LZClose(hf)			_close(hf)
#define	LZRead(hf,lpvBuf,cb)		_read(hf,lpvBuf,cb)
#define	LZSeek(hf,lOffset,nOrigin)	_lseek(hf,lOffset,nOrigin)
#define GetModuleFileName(hinst,lpszFileName,cbFileName)	\
	DosQueryModuleName(hinst,cbFileName,lpszFileName)
#define InflateRect(lprc,xAmt,yAmt)	WinInflateRect(hab,lprc,xAmt,yAmt)
#define	LoadString(hinst,idResource,lpszBuffer,cbBuffer)	\
	WinLoadString(hab,hinst,(ULONG)idResource,(LONG)cbBuffer,(PSZ)lpszBuffer)
#define FILEOKSTRING   "commdlg_FileNameOK"
#define	DRIVE_REMOVABLE	2

#ifdef __cplusplus
extern "C" {
#endif

extern HAB hab;
extern InitializeCriticalSection(CRITICAL_SECTION*);  
HFILE	LZOpenFile(LPCSTR,OFSTRUCT *,UINT);

int	_CRT_init(void);
void	_CRT_term(void);
void	__ctordtorInit(void);

// end of cpanel
#ifdef __cplusplus
}
#endif

#define far
#define FAR
#define NEAR
#ifndef TRUE
#define TRUE		true
#endif
#ifndef FALSE
#define FALSE		false
#endif

typedef signed short    (* FARPROC)(void);

#define	lstrlen		strlen
#define	lstrcpy		strcpy
#define _strnicmp 	strnicmp
#define _stricmp     stricmp
#define  strcasecmp  stricmp
#define	SetErrorMode(m) 0
#define	MAKEINTRESOURCE(i) i
#define	FreeLibrary(a)	_freemod(a)
#define	OpenFile(n,s,m)	open(n,m,S_IWRITE)
#define  VGFreeLibrary(a)  _freemod((ULONG)a)
#define  VGGetProcAddress(voidptr, lpcstr) OS2GetProcAddressString((HANDLE)voidptr, lpcstr)
#define	_lopen(n,m)	open(n,m,S_IWRITE)
#define	_lclose(f)	close(f)
#define	_llseek(f,o,w)	lseek(f,o,w)
#define	_lwrite(f,b,c)	write(f,b,c)
#define	_lread(f,b,c)	read(f,b,c)
#define	HFILE_ERROR	-1
#define	__cdecl		
#define	_cdecl		
#define	wvsprintf	vsprintf
#define	wsprintf	sprintf
#define	wsscanf  sscanf

#define GET_WM_COMMAND_ID(wp,lp)	LOWORD(wp)	
#define GET_WM_COMMAND_CMD(wp,lp)	HIWORD(wp)
#define GET_WM_COMMAND_CODE(wp,lp)      GET_WM_COMMAND_CMD(wp,lp)
#define GET_WM_COMMAND_HWND(wp, lp)	((HWND)lp)

// Dialogbox related stuff
// Max listbox string. 255 is random. Make it more meaningful later.
#define MAX_TITLE   255
#define WPARAM		MPARAM
#define LPARAM		MPARAM
#define CB_ERR		LIT_ERROR
#define EndDialog(a,b)	WinDismissDlg(a,(ULONG)b)
#define LBN_DBLCLK	LN_ENTER
#define IDOK		DID_OK
#define IDCANCEL	DID_CANCEL
#define IDYES		MBID_YES
#define IDNO		MBID_NO
#define IDRETRY		MBID_RETRY
#define IDIGNORE	MBID_IGNORE
#define IDABORT		MBID_ABORT
#define GetDlgItemText(a,b,c,d)	WinQueryDlgItemText(a,b,d,c)
#define POINT		POINTL
#define RECT		RECTL
#define LPRECT		PRECTL
#define left		xLeft
#define right		xRight
#define top		yBottom
#define bottom		yTop
#define GetParent(a)	WinQueryWindow(a, QW_PARENT)
#define SM_CXSCREEN	SV_CXSCREEN
#define SM_CYSCREEN	SV_CYSCREEN
#define SM_CXBORDER  SV_CXBORDER
#define SM_CYBORDER  SV_CYBORDER
#define SM_CXFULLSCREEN	SV_CXFULLSCREEN
#define SM_CYFULLSCREEN	SV_CYFULLSCREEN
#define SM_CXHSCROLL	SV_CXHSCROLLARROW
#define SM_CYHSCROLL	SV_CYHSCROLL
#define SM_CXVSCROLL	SV_CXVSCROLL
#define SM_CYVSCROLL	SV_CYVSCROLLARROW
#define GetSystemMetrics(a)	WinQuerySysValue(HWND_DESKTOP, a)
#define GetWindowRect(a,b)	WinQueryWindowRect(a,b)
#define MoveWindow(hwnd,x,y,dx,dy,flag) WinSetWindowPos(hwnd,HWND_TOP,x,y,dx,dy,SWP_ACTIVATE | SWP_MOVE | SWP_SIZE | SWP_SHOW)
#define GlobalAlloc(flags,Size)	(HANDLE) calloc(1,Size)
#define GlobalFree(hMem)	free((LPSTR) hMem)
#define WM_INITDIALOG	WM_INITDLG

#define GET_WM_COMMAND_MPS(id,hwnd,cmd)	(id),MAKELONG(hwnd,cmd)
#ifdef DEFINE_PASCAL_TO_NULL 
// Required while building the test DLLs
#define PASCAL 
#else
// Required while building sample driver
#define PASCAL _System
#endif
#define GlobalPtrHandle(x) ((x)->savedHandle)
#define CALLBACK
#define GLOBALHANDLE	HMEM
#define DialogBoxParam(hinst,iddlg,hwnd,dlgproc,param) \
        WinDlgBox(HWND_DESKTOP,hwnd,(PFNWP)dlgproc,hinst,iddlg,(PVOID)param)
#define DialogBox(hinst,iddlg,hwnd,dlgproc) \
        WinDlgBox(HWND_DESKTOP,hwnd,(PFNWP)dlgproc,hinst,iddlg,(PVOID)NULL)
#define DialogBoxIndirectParam(hinst,hglbDlgTemp,hwnd,dlgproc,param)	\
        WinCreateDlg(HWND_DESKTOP,hwnd,(PFNWP)dlgproc,\
        (PDLGTEMPLATE)hglbDlgTemp,(PVOID)param)
#define GetClientRect(hwnd,rect)	WinQueryWindowRect(hwnd,rect)
#define ClientToScreen(a,b)
#define GetDesktopWindow()	WinQueryDesktopWindow(0,NULLHANDLE)
#define GetWindowLong(x,a)	WinQueryWindowULong(x, QWL_USER)
#define SetWindowLong(x,a,y)	WinSetWindowULong(x, QWL_USER, (LONG)y)
#define SetDlgItemText(a,b,c)	WinSetDlgItemText(a,b,(PSZ)c)
#define GetDlgItem(a,b)		WinWindowFromID(a,b)
#define SendDlgItemMessage(hwndDlg,idDlgItem,uMsg,wParam,lParam) \
        OS2SendMsg(WinWindowFromID(hwndDlg,(ULONG)idDlgItem),(ULONG)uMsg,(MPARAM)wParam,(MPARAM)lParam)
#define CheckDlgButton(a,b,c)	WinCheckButton(a,b,c)
#define EnableWindow(a,b)	WinEnableWindow(a,b)
#define MessageBox(hwnd,text,title,style) WinMessageBox(HWND_DESKTOP,hwnd,text,title,0,style)	
#define IsDlgButtonChecked(a,b)	WinQueryButtonCheckstate(a,b)
#define	SetCapture(hwnd)	WinSetCapture(HWND_DESKTOP,hwnd)
#define ReleaseCapture()	WinSetCapture(HWND_DESKTOP,NULLHANDLE)
#define	GetLastError()		WinGetLastError(hab)
#define EM_LIMITTEXT		EM_SETTEXTLIMIT
#define EM_GETSEL       EM_QUERYSEL
#define EM_GETMODIFY    EM_QUERYCHANGED

#pragma map(DMGetPrivateProfileString , "GetPrivateProfileString")
#pragma map(DMWritePrivateProfileString ,"WritePrivateProfileString")

#define lstrcmp 	      strcmp
#define lstrcmpi 	      strcmpi
#define lstrcat		   strcat
#define GHND	         0
#define GPTR	         0
#define GMEM_NODISCARD  0
#define GMEM_SHARE	   0
#define GMEM_FIXED	   0
#define GMEM_ZEROINIT   1
#define MEM_RESERVE	   0
#define MEM_RELEASE	   0
#define PAGE_NOACCESS	0
#define PAGE_READWRITE	0
#define MEM_COMMIT	   0

// things needed for Oracle driver
#define	WINAPI
#undef   NULL
#define	NULL  0
// Supposed to come from serialno.h which has empty parentheses. But, OS/2
// needs a 'void' to recognize the statement as a prototype
extern int VSCheckSerialNumber(void);
#ifndef HINSTANCE_ERROR
#define HINSTANCE_ERROR NULL
#endif
// end of things needed for Oracle driver

// things needed for TEST20\GATORTST module
#define  INTFUN
#define  EXTFUN
#define  EXTFUNCDECL
#define  CDECL
#define  _stdcall
#define  dCSEG(type) const type
// end of things needed for TEST20\GATORTST module

typedef struct tagOFN
{
    DWORD   lStructSize;
    HWND    hwndOwner;
    HINSTANCE hInstance;
    LPCSTR  lpstrFilter;
    LPSTR   lpstrCustomFilter;
    DWORD   nMaxCustFilter;
    DWORD   nFilterIndex;
    LPSTR   lpstrFile;
    DWORD   nMaxFile;
    LPSTR   lpstrFileTitle;
    DWORD   nMaxFileTitle;
    LPCSTR  lpstrInitialDir;
    LPCSTR  lpstrTitle;
    DWORD   Flags;
    UINT    nFileOffset;
    UINT    nFileExtension;
    LPCSTR  lpstrDefExt;
    ULONG  lCustData;
    UINT    (CALLBACK *lpfnHook)(HWND, UINT, WPARAM, LPARAM);
    LPCSTR  lpTemplateName;
}   OPENFILENAME;
typedef OPENFILENAME FAR * LPOPENFILENAME;

//BOOL    GetOpenFileName(OPENFILENAME FAR *);
//BOOL    GetSaveFileName(OPENFILENAME FAR *);
//int     GetFileTitle(LPCSTR, LPSTR, UINT);

#define OFN_READONLY                 0x00000001
#define OFN_OVERWRITEPROMPT          0x00000002
#define OFN_HIDEREADONLY             0x00000004
#define OFN_NOCHANGEDIR              0x00000008
#define OFN_SHOWHELP                 0x00000010
#define OFN_ENABLEHOOK               0x00000020
#define OFN_ENABLETEMPLATE           0x00000040
#define OFN_ENABLETEMPLATEHANDLE     0x00000080
#define OFN_NOVALIDATE               0x00000100
#define OFN_ALLOWMULTISELECT         0x00000200
#define OFN_EXTENSIONDIFFERENT       0x00000400
#define OFN_PATHMUSTEXIST            0x00000800
#define OFN_FILEMUSTEXIST            0x00001000
#define OFN_CREATEPROMPT             0x00002000
#define OFN_SHAREAWARE               0x00004000
#define OFN_NOREADONLYRETURN         0x00008000
#define OFN_NOTESTFILECREATE         0x00010000

#define FORWARD_WM_SETREDRAW(hwnd, fRedraw, fn) \
    (void)(fn)((hwnd), WM_SETREDRAW, (WPARAM)(fRedraw), 0L)

typedef DWORD COLORREF;

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)(g)<<8))|(((DWORD)(BYTE)(b))<<16)))

// Used in test dll modules
#define GetTempPath(size,Path)
#define GetTickCount()  WinGetCurrentTime(hab)

// Used in C version of ODBCTEST
typedef Handle		HFONT;
typedef Handle		HMENU;
typedef Handle		HBRUSH;
#undef  CONTROL
#define MF_CHECKED   TRUE
#define MF_UNCHECKED FALSE
#define CheckMenuItem(hmenu,idCheckItem,uCheck) \
        WinCheckMenuItem(hmenu,idCheckItem,uCheck)
// From windows.h
#define LB_GETSEL	          (WM_USER+8)
#define LB_GETSELCOUNT	    (WM_USER+17)
#define LB_GETSELITEMS	    (WM_USER+18)
#define WM_SETTEXT	       0x000C
#define WM_GETTEXT	       0x000D
#define WM_GETTEXTLENGTH    0x000E
#define IsWindowEnabled(hwnd) WinIsWindowEnabled(hwnd)
#define GetClassName(hwnd,buf,length) WinQueryClassName(hwnd,length,buf)
typedef FARPROC DLGPROC;
#define GetDlgItemInt(a, b, c, d) OS2GetDlgItemInt(a, b, d)
#define DefWindowProc(hwnd, msg, wParam, lParam) \
        (LONG) WinDefWindowProc(hwnd, (ULONG) msg, (MPARAM) wParam, (MPARAM) lParam)
#define MAKELPARAM         MPFROM2SHORT
#define WM_KEYFIRST  WM_CHAR
#define WM_KEYLAST   WM_CHAR
#define VK_ESCAPE    VK_ESC
#define VK_PRIOR     VK_PAGEUP
#define VK_NEXT      VK_PAGEDOWN

//*** Last argument should be SIZEL. OS/2 has another SIZE defined. *DO*
//*** in-line changes to change SIZE to SIZEL. Do *NOT* ignore warning.
BOOL GetTextExtentPoint(HPS, LPCSTR, int, SIZEL *);

//*** Following 4 funcs are implemented for SB_CTL *ONLY*. Compiler generates
//*** error for other values.
// From windows.h
#define SB_CTL		    2
// Arbitrary numbers
#define SB_TOP		    666
#define SB_BOTTOM	    777

#define SB_THUMBPOSITION   SB_SLIDERPOSITION
#define SB_THUMBTRACK      SB_SLIDERTRACK

// Valid *ONLY* for SW_HIDE & SW_SHOW of MSWin. Rest are *NOT* handled.
// From windows.h
#define SW_HIDE		    0
#define SW_SHOW		    5
#define ShowWindow(hwnd, nCmdShow) WinShowWindow(hwnd, ((nCmdShow) == SW_HIDE) ? FALSE : TRUE);
#define ScrollWindow(hwnd, dx, dy, lprcScroll, lprcClip) \
        WinScrollWindow(hwnd, dx, dy, lprcScroll, lprcClip, (HRGN)NULLHANDLE,\
        (PRECTL) NULL, 0)
#define UpdateWindow(hwnd) WinUpdateWindow(hwnd)

#define WA_INACTIVE  FALSE
#define WM_INITMENUPOPUP WM_INITMENU

// not sure of the following UNDO ...
#define WM_UNDO      MLM_UNDO

#define WM_CUT       EM_CUT
#define WM_COPY      EM_COPY
#define WM_PASTE     EM_PASTE
#define WM_CLEAR     EM_CLEAR

#define MF_SEPARATOR MIS_SEPARATOR
#define MF_MENUBREAK MIS_BREAK
#define MF_MENUBARBREAK MIS_BREAKSEPARATOR
#define MF_POPUP     MIS_SUBMENU
#define MF_SYSMENU   MIS_SYSCOMMAND
#define MF_ENABLED   TRUE
#define MF_DISABLED  FALSE
#define MF_GRAYED    FALSE
#define EnableMenuItem(hwndMenu, usId, fEnable) \
        WinEnableMenuItem(hwndMenu, usId, fEnable)
#define IsClipboardFormatAvailable(fmt) WinQueryClipbrdData(hab,fmt)

// ODBC 2.5 requirements
#define	MAX_COMPUTERNAME_LENGTH		80

// Used in LB_DIR's equivalent - from windows.h
#define DDL_READWRITE       0x0000

// Dummy value to use in WinCreateWindow().
#define CW_USEDEFAULT   50
#define WS_MAXIMIZE WS_MAXIMIZED

#ifdef INCL_OS2UTIL_H
#include "os2util.h"
#endif
                               
#endif // WIN2OS2_H
