/*%CPW_START%*/
/*****************************************************************************
COPYRIGHT (c)  1990-95, VISIGENIC SOFTWARE, INC., 
*****************************************************************************/
/*
$Header:   /development/workarea1/visiroot/odbc/archives/test/windows/tests/dllstub.c__   1.0   Wed Apr 05 12:01:32 1995   ric  $

$Log:   /development/workarea1/visiroot/odbc/archives/test/windows/tests/dllstub.c__  $
 * 
 *    Rev 1.0   Wed Apr 05 12:01:32 1995   ric
 * Initial PVCS Revision
*/
/*%CPW_END%*/
//*------------------------------------------------------------------------
//|	File:			DLLSTUB.C
//|
//|	Purpose:		This is a generic module which contains the entry points
//|						required to compile a DLL.
//*------------------------------------------------------------------------
#include "win2os2.h"
int _CRT_init(void);
void _CRT_term(void);

HINSTANCE	hLoadedInst;

unsigned long _System _DLL_InitTerm (unsigned long ModHandle, 
	unsigned long flag)
{
//   int fd;
	switch (flag)
	{
		case 0:	// DLL environment is initialized
			hLoadedInst = ModHandle;
         if (!hLoadedInst || (_CRT_init() == -1))
            return 0UL;

//         fd = open("c:\\dll.log",O_CREAT|O_APPEND|O_RDWR, S_IWRITE);
//         write(fd,"load one of the TEST DLLs\n",sizeof("load one of the TEST DLLs"));
//         close(fd);
			break;

    	case 1: // DLL environment is ended
//         fd = open("c:\\dll.log",O_CREAT|O_APPEND|O_RDWR, S_IWRITE);
//         write(fd,"unload the TEST DLL\n",sizeof("unload the TEST DLL"));
//         close(fd);
         _CRT_term();
			break;

		default:
	      return 0UL;
	}
	return 1UL;
}
