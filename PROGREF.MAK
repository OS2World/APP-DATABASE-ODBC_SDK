#--------------------------------------------------------------------#
#  makefile for example ODBC programs                                #
#  Set $ODBC to your "ODBC Root Directory"                           #
#--------------------------------------------------------------------#

# ODBCDIR should be set to where your odbc package is installed
#  This example assumes that the odbc package is installed on
#  /usr/odbc.  You must change it to the appropriate path.

ODBCDIR=$(OS2SRC)

!IF "$(DEBUG)" == "YES"
CDEBUG	= /Ti+
LDEBUG	= /CO
RDEBUG	= /dDEBUG
OBJDIR	= dbgobj
!else # $(DEBUG) != YES
CDEBUG	= 
LDEBUG	= 
RDEBUG	= 
OBJDIR	= obj
!endif # $(DEBUG)

CC 	= icc /I..\..\include
LINK    = link386 
CFLAGS	= $(CDEBUG) /Wpro+ /Ge+ /Gm- /C- /Ms /Ss+ /Fi+ /Si+ /Q+ 
CFLAGS	= $(CFLAGS) /DOS2ODBC::1 /DWIN32 /DEXTENSIONS 
LFLAGS  = /B $(LDEBUG) ..\..\lib\odbc.lib
LIBPATH = $(LIB); c:\toolkt21\os2lib

#.c{$(OBJDIR)\}.obj      :
#	$(CC) $(CFLAGS) -Fo$(OBJDIR)\$(@B).obj $(@B).c

all:    error.obj static.exe adhoc.exe


error.obj:  error.c progref.mak
    $(CC) $(CFLAGS) /C+ -Fo$(OBJDIR)\error.obj error.c

static.exe:	static.c error.c progref.mak 
    $(CC) $(CFLAGS) -Fo$(OBJDIR)\static.obj static.c $(LFLAGS) /B $(OBJDIR)\error.obj 

adhoc.exe:	adhoc.c error.c progref.mak 
    $(CC) $(CFLAGS) -Fo$(OBJDIR)\adhoc.obj adhoc.c $(LFLAGS) /B $(OBJDIR)\error.obj 


clean:
	rm *.obj 

               
