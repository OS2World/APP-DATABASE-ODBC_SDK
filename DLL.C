/*
** DLL.C - This is the ODBC sample driver code for
** LIBMAIN processing.
**
**	This code is furnished on an as-is basis as part of the ODBC SDK and is
**	intended for example purposes only.
**
*/

//	-	-	-	-	-	-	-	-	-

#include "sample.h"

HINSTANCE NEAR s_hModule;		// Saved module handle.
HAB hab;

#if OS2ODBC
unsigned long _System _DLL_InitTerm (unsigned long ModHandle, 
	unsigned long flag)
{
   int fd;
	switch (flag)
	{
		case 0:	// DLL environment is initialized
         if (_CRT_init() == -1)
            return 0UL;

         fd = open("c:\\dll.log",O_CREAT|O_APPEND|O_RDWR, S_IWRITE);
         write(fd,"load SAMPLE DRIVER DLL\n",sizeof("load SAMPLE DRIVER DLL"));
         close(fd);

	     	s_hModule = ModHandle; 
			break;

    	case 1: // DLL environment is ended
         fd = open("c:\\dll.log",O_CREAT|O_APPEND|O_RDWR, S_IWRITE);
         write(fd,"unload SAMPLE DRIVER DLL\n",sizeof("unload SAMPLE DRIVER DLL"));
         close(fd);

         _CRT_term();
			break;

		default:
	      return 0UL;
	}
	return 1UL;
}

#else // ! OS2ODBC

#ifdef _WIN32

int __stdcall LibMain(HANDLE hInst,DWORD ul_reason_being_called,LPVOID lpReserved) 
{
    switch (ul_reason_being_called) 
	{
		case DLL_PROCESS_ATTACH:	// case of libentry call in win 3.x
        	s_hModule = hInst; 
			break;
		case DLL_THREAD_ATTACH:
			break;
    	case DLL_PROCESS_DETACH:	// case of wep call in win 3.x
			break;
    	case DLL_THREAD_DETACH:
			break;
		default:
			break;
	} /* switch */

    return TRUE;                                                                
                                                                                
    UNREFERENCED_PARAMETER(lpReserved);                                         
} /* LibMain */

#else	//	_WIN32

//	-	-	-	-	-	-	-	-	-

//	This routine is called by LIBSTART.ASM at module load time.  All it
//	does in this sample is remember the DLL module handle.	The module
//	handle is needed if you want to do things like load stuff from the
//	resource file (for instance string resources).

int _export FAR PASCAL libmain(
	HANDLE	   hModule,
	short	   cbHeapSize,
	UCHAR FAR *lszCmdLine)
{
	s_hModule = hModule;
	return TRUE;
}

#endif	//	_WIN32

#endif	//	OS2ODBC

void EXPFUNC FAR PASCAL LoadByOrdinal(void);
//	Entry point to cause DM to load using ordinals
void EXPFUNC FAR PASCAL LoadByOrdinal(void)
{
}
