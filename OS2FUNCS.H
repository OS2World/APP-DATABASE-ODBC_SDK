
#ifdef GLOBAL
#undef GLOBAL
#endif
#ifdef CONSTSTR
#undef CONSTSTR
#endif
#ifdef INCL_GLOBAL
#define	GLOBAL
#define	CONSTSTR(x,y)	const char  x[]=y;
#else
#define	GLOBAL	extern
#define	CONSTSTR(x,y)	extern const char x[];
#endif

#ifdef __cplusplus
extern "C" {
#endif
GLOBAL int (*EnterCriticalSection)	(CRITICAL_SECTION*);
GLOBAL int (*LeaveCriticalSection)	(CRITICAL_SECTION*);
GLOBAL int (*DeleteCriticalSection)	(CRITICAL_SECTION*);
GLOBAL TID (*GetCurrentThreadId)	(void);
GLOBAL FARPROC (*GetProcAddress)	(HINSTANCE, int);
GLOBAL FARPROC (*OS2GetProcAddressString)	(HANDLE , LPCSTR );
GLOBAL HINSTANCE	(*LoadLibrary)	(LPCSTR);
GLOBAL HANDLE (*VGLoadLibrary)	(LPCSTR nm,char **errstr);
GLOBAL void *(*GlobalAllocPtr)	(long, int);
GLOBAL void *(*GlobalReAllocPtr)	(void *,long, int);
GLOBAL void (*GlobalFreePtr)	(void *);
GLOBAL void *(*GlobalLock)	(long);
GLOBAL void (*GlobalUnlock)	(long);
GLOBAL void (*GetTempFileName)	(const char [],const char [],int,char[]);
GLOBAL void (*VirtualFree)	(void*,int,int);
GLOBAL void* (*VirtualAlloc)	(void*,int,int,int);
GLOBAL int (*GetPrivateProfileString)	( LPCSTR lpszSection, LPCSTR lpszEntry, LPCSTR lpszDefault, LPSTR  lpszRetBuffer, int cbRetBuffer, LPCSTR lpszFilename);
GLOBAL BOOL (*WritePrivateProfileString)	(LPCSTR lpszSection, LPCSTR lpszEntry, LPCSTR lpszString, LPCSTR lpszFilename);
GLOBAL BOOL (*DMWritePrivateProfileString)	(LPCSTR lpszSection, LPCSTR lpszEntry, LPCSTR lpszString, LPCSTR lpszFilename);
GLOBAL WORD (*GetProfileString)	( LPCSTR lpszSection, LPCSTR lpszEntry, LPCSTR lpszDefault, LPSTR  lpszRetBuffer, WORD   cbRetBuffer);
GLOBAL int  (*GetPrivateProfileInt)	(LPCSTR pszApp, LPCSTR pszKey, int lDefault, LPCSTR pszFileName);
GLOBAL UINT (*GetProfileInt)	(LPCSTR,LPCSTR,int);
GLOBAL BOOL (*fullpathspecified)	(const char *);
GLOBAL BOOL	(*commentline)	(char *);
GLOBAL char	*(*returnenvinline)	(char *line,char *variable);
GLOBAL BOOL	(*pathinline)	(char *line,char *path);
GLOBAL BOOL (*GetEnvFromConfigFile)	(char *envname,char *envdata,int envlen);
GLOBAL UINT (*GetBootDrive)	(char *syspath,UINT cbsyspath);
GLOBAL void (*GetINIfileDirectory)	(char *szpath,int pathlen);
GLOBAL BOOL (*DeleteFile)	(LPCSTR lpsz);
GLOBAL LPSTR	(*AnsiNext)	(LPCSTR);
GLOBAL LPSTR	(*AnsiLower)	(LPSTR);
GLOBAL LPSTR	(*AnsiUpper)	(LPSTR);
GLOBAL LPSTR	(*AnsiPrev)	(LPCSTR,LPCSTR);
GLOBAL UINT	(*AnsiUpperBuff)	(LPSTR,UINT);
GLOBAL void	(*AnsiToOem)	(char *hpszOS2,char *hpszOem);
GLOBAL void	(*OemToAnsi)	(char *hpszOS2,char *hpszOem);
GLOBAL void	(*AnsiToOemBuff)	(char *hpszOS2,char *hpszOem,UINT);
GLOBAL void	(*OemToAnsiBuff)	(char *hpszOS2,char *hpszOem,UINT);
GLOBAL HCURSOR	(*SetCursor)	(HCURSOR);
GLOBAL HCURSOR (*LoadCursor)	(HWND,LPCSTR);
GLOBAL UINT	(*GetSystemDirectory)	(LPSTR,UINT);
GLOBAL UINT	(*GetWindowsDirectory)	(LPSTR,UINT);
GLOBAL UINT	(*GetDriveType)	(char*);
GLOBAL BOOL	(*WinHelp)	(HWND,LPCSTR,UINT,DWORD);
GLOBAL HMODULE	(*GetModuleHandle)	(LPCSTR);
GLOBAL long	(*OS2SendMsg)	(HWND,ULONG,MPARAM,MPARAM);
GLOBAL void	(*OS2GetTextExtent)	(HPS,PRECTL,LPCSTR,int,PSIZE );
GLOBAL int	(*GetSelCount)	(HWND,ULONG);
GLOBAL int	(*GetSelItems)	(HWND,ULONG,LONG,int	[]);
GLOBAL void	(*CheckRadioButton)	(HWND,int,int,int);
GLOBAL UINT	(*RegisterOS2Message)	(LPCSTR);
GLOBAL void	*(*searchstring)	(char *buf,size_t count, char *string);
GLOBAL int  (*GetScrollPos)	(HWND, int);
GLOBAL int  (*SetScrollPos)	(HWND, int, int, BOOL);
GLOBAL int  (*SetScrollRange)	(HWND, int, int, int, BOOL);
GLOBAL int  (*GetScrollRange)	(HWND, int, int *, int *);
GLOBAL UINT (*OS2GetDlgItemInt)	(HWND, int, BOOL);
GLOBAL BOOL (*IsZoomed)	(HWND);

GLOBAL UINT WINAPI (*VerFindFile)	(UINT uFlags, LPCSTR szFileName, LPCSTR szWinDir, LPCSTR szAppDir, LPSTR szCurDir, UINT FAR* lpuCurDirLen, LPSTR szDestDir, UINT FAR* lpuDestDirLen);

GLOBAL DWORD WINAPI (*VerInstallFile)	(UINT uFlags,
      LPCSTR szSrcFileName, LPCSTR szDestFileName, LPCSTR szSrcDir,
      LPCSTR szDestDir, LPCSTR szCurDir, LPSTR szTmpFile, UINT FAR* lpuTmpFileLen);

/* Returns size of version info in bytes */
GLOBAL DWORD WINAPI (*GetFileVersionInfoSize)	(
      LPCSTR lpstrFilename,     /* Filename of version stamped file */
      DWORD FAR *lpdwHandle);   /* Information for use by GetFileVersionInfo */

/* Read version info into buffer */
GLOBAL BOOL WINAPI (*GetFileVersionInfo)	(
      LPCSTR lpstrFilename,     /* Filename of version stamped file */
      DWORD dwHandle,           /* Information from GetFileVersionSize */
      DWORD dwLen,              /* Length of buffer for info */
      void FAR* lpData);        /* Buffer to place the data structure */

GLOBAL UINT WINAPI (*VerLanguageName)	(UINT wLang, LPSTR szLang, UINT nSize);

GLOBAL BOOL WINAPI (*OS2VerQueryValue)	(const void FAR* pBlock,UINT pBlockSize, LPCSTR lpSubBlock,
      void FAR* FAR* lplpBuffer, UINT FAR* lpuLen);

#ifdef __cplusplus
}
#endif
#undef GLOBAL
#undef CONSTSTR
