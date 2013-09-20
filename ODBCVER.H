/*--------------------------------------------------------------------------
  ODBCVer.h -- ODBC Version info
--------------------------------------------------------------------------*/


// Includes ----------------------------------------------------------------
#ifndef WIN32
#include  <ver.h>                         // Version defines
#endif
#if OS2ODBC
#include <ver.h>
#endif

// Constants ---------------------------------------------------------------
#if OS2ODBC
	/*
	* Minor,major,day,month
	*/
#ifndef VER_FILEVERSION
#define VER_FILEVERSION 10,02,01,24
#endif
#ifndef VER_PRODUCTVERSION
#define VER_PRODUCTVERSION 10,02,01,24
#endif
#else
#define VER_FILEVERSION 02,10,24,01
#define VER_PRODUCTVERSION 02,10,24,01
#endif
#define VER_FILEVERSION_STR " 2.10.2401\0"
#define VER_PRODUCTVERSION_STR " 2.10.2401\0"

#ifdef OS2ODBC
#define VER_FILEFLAGSMASK (VS_FF_PRERELEASE)
#define VER_DATE	00,00,00,00
#else
#define VER_FILEFLAGSMASK	(VS_FF_DEBUG | VS_FF_PRERELEASE)
#endif
#ifdef DEBUG
#define VER_FILEFLAGS (VS_FF_DEBUG)
#else
#define VER_FILEFLAGS (0L)
#endif

#ifdef WIN32
#define	VER_FILEOS	VOS_NT_WINDOWS32
#else
#define VER_FILEOS  VOS_DOS_WINDOWS16
#endif

#define VER_COMPANYNAME_STR     "Visigenic Software\0"
#define VER_PRODUCTNAME_STR     "Microsoft Open Database Connectivity\0"
#define VER_LEGALCOPYRIGHT_STR	"Copyright \251 Microsoft Corporation 1990-1994\0"
