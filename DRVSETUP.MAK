##%CPW_START%
##*************************************************************
## COPYRIGHT © 1990-95, VISIGENIC SOFTWARE, INC.
##************************************************************
## $Header:   /development/workarea1/visiroot/odbc/archives/install/windows/drvsetup/makefile.___   1.1   Tue Feb 28 14:57:06 1995   ric  $
## 
## $Log:   /development/workarea1/visiroot/odbc/archives/install/windows/drvsetup/makefile.___  $
## 
##    Rev 1.1   Tue Feb 28 14:57:06 1995   ric
## Include and Lib paths modified to match our source stream
## 
##    Rev 1.0   30 Nov 1994 15:35:14   ric
## Initial PVCS Revision
##%CPW_END%
#---------------------------------------------------------------------------
# Makefile -- Build Driver Setup Kit for 16 and 32 bit
#
#---------------------------------------------------------------------------

APP                      = setup
#DRVSKINC                 = ..\..\..\include
#DRVSKLIB                 = ..\..\..\odbcsdk\lib
#DRVSKLIB32               = ..\..\..\odbcsdk\lib32

# Constants ----------------------------------------------------------------


#!include <ntwin32.mak>

#OBJECTS         = setup32
#INCLUDE         = $(INCLUDE);$(DRVSKINC)
#LIB             = $(LIB);$(DRVSKLIB32)
#!ifdef DEBUG
#CFLAGS = $(CFLAGS) /Zi
#LFLAGS = /DEBUG


LNK             = $(APP).lnk
NEWLINE		= ""

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

!IF !defined(ODBCROOT)
ODBCROOT = \os2src
!ENDIF

INCLUDE = $(ODBCROOT)\include;$(INCLUDE);
CPANEL = $(ODBCROOT)\CPANEL

cc = icc
CFLAGS	= $(CFLAGS) /Wpro+ /Ge+ /Gm+ /C /Ms  /Ss /Fi+ /Si+ /DOS2ODBC::1 /DEXTENSIONS
CFLAGS	= $(CFLAGS)  /DWIN32 /D_WIN32 /DDEDITOR $(CDEBUG)
CFLAGS = $(CFLAGS) /Fo$(OBJDIR)\$(@B).obj
CHFLAGS	=

RFLG	= $(RDEBUG)

LIBUTL = implib /NOI
LINKUTL = link386  /NOI /NOE /NOLOGO /M /PM:PM + $(LDEBUG)

.SUFFIXES : .cpp .obj .c .asm .exe .dll .res

.cpp{$(OBJDIR)\}.obj:
  $(cc) /Fo$(OBJDIR)\$(@B).obj $(@B).cpp

.c{$(OBJDIR)\}.obj:
  $(cc) /Fo$(OBJDIR)\$(@B).obj $(@B).c

# Rules --------------------------------------------------------------------


all :                           setenv $(OBJDIR)\$(APP).exe

setenv :
	set ICC=$(CFLAGS)
	set INCLUDE=$(INCLUDE)
	set LIB=$(LIB)

$(OBJDIR)\$(APP).exe :$(@B).def drvsetup.mak	\
			$(OBJDIR)\regist.obj \
			$(OBJDIR)\drvsetup.obj \
			drvsetup.res
		@echo $(OBJDIR)\drvsetup.obj +   > $(LNK)
		@echo $(OBJDIR)\regist.obj          >> $(LNK)
		@echo $@			>> $(LNK)
		@echo $*.map			>> $(LNK)
		@echo $(NEWLINE)		>> $(LNK)
		@echo $(@B).def;		>> $(LNK)
		$(LINKUTL) @$(LNK)
		@del $(LNK)
	rc -p drvsetup.res $@

$(OBJDIR)\drvsetup.obj :       drvsetup.mak        \
				drvsetup.c      \
				regist.h	\
				drvsetup.h	\
				$(ODBCROOT)\include\win2os2.h

$(OBJDIR)\regist.obj   :       drvsetup.mak        \
				regist.c        \
				drvsetup.h	\
				regist.h	\
				$(ODBCROOT)\include\win2os2.h

drvsetup.res :       drvsetup.mak        \
				drvsetup.rc     \
				drvsetup.dlg    \
				drvsetup.h      \
				$(ODBCROOT)\include\win2os2.h	\
				$(ODBCROOT)\include\odbcver.h	\
				drvsetup.ico    \
				splshos2.bmp	\
				drvsetup.bmp
	copy splshos2.bmp drvsetup.bmp
    	rc -DOS2ODBC=1 $(RFLG) -i . /r drvsetup.rc



.IGNORE:
.SILENT:
clean:	
	erase /N *.res	 2>nul
	erase /N $(OBJDIR)\*.* 2>nul
