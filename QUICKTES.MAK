#%CPW_START%
#****************************************************************************
# COPYRIGHT (c)  1990-95, VISIGENIC SOFTWARE, INC.
#****************************************************************************
# $Header:   /development/workarea1/visiroot/odbc/archives/test/windows/tests/makefile   1.0   Wed Apr 05 12:28:36 1995   ric  $
# 
# $Log:   /development/workarea1/visiroot/odbc/archives/test/windows/tests/makefile  $
## 
##    Rev 1.0   Wed Apr 05 12:28:36 1995   ric
## Initial PVCS Revision
#%CPW_END%

#----------------------------------------------------------------------------
# File   : MAKEFILE.OS2
# Purpose: To compile the 32-bit auto test DLL's for OS/2.
# Author : Prasad
# Note   : This makefile gets the name of the source file from the environment
#          variable TSTNM. It takes only one C source file and produces a DLL.
#----------------------------------------------------------------------------

!IF "$(DEBUG)" == "YES"
CDEBUG	= /Ti+ /D_DEBUG /DWIN32 
LDEBUG	= /CO
RDEBUG	= /d_DEBUG
OBJDIR	= dbgobj
EXEDIR  = dbgexe
!else # $(DEBUG) != YES
CDEBUG	= /DWIN32
LDEBUG	= 
RDEBUG	= 
OBJDIR	= obj
EXEDIR  = exe
!endif # $(DEBUG)

!IF !defined(ODBCROOT)
ODBCROOT = \os2src
!ENDIF

ODBCTEST = .

#----------------------------------------------------------------------------
# Generic defines
#----------------------------------------------------------------------------
TSTNM   = $(TSTNM)              ## Obtained from the environment
SRCDIR  = .
INCDIR  = .
RCDIR  = .
LIBDIR  = $(ODBCTEST)\lib
OS2UTIL = $(ODBCROOT)\os2util
SQLINC  = $(ODBCROOT)\INCLUDE
INCLUDES= $(SQLINC);.;$(INCLUDE);
INCTEST = $(SQLINC)\sql.h \
	  $(SQLINC)\sqlext.h 

#----------------------------------------------------------------------------
# Compiler, linker, and RC compiler specification
#----------------------------------------------------------------------------
#--------
# Defines
#--------

#------
# Flags
#------
RFLAGS	= -r $(RDEBUG) /dOS2ODBC /dIDS_ONLY
CFLAGS	= $(CDEBUG) /Q+ /Wpro+ /Ge- /Gm+ /C /Ms /Ss /Fi+ /Si+ 
CFLAGS	= $(CFLAGS) /DOS2ODBC::1 /DDLL_INVOKED /DDEFINE_PASCAL_TO_NULL
LFLAGS	= $(LDEBUG) /NOLOGO /NOE /NOI /BATCH
#--------------
# Program names
#--------------
CC	= icc
RC	= rc
LINK	= link386
LIBUTL	= implib

#----------------------------------------------------------------------------
# Targets
#----------------------------------------------------------------------------
all     : setenv blddeffile $(EXEDIR)\$(TSTNM).dll $(EXEDIR)\$(TSTNM).lib

setenv  :
        set include=$(INCLUDES)
        set icc=$(ICC) $(CFLAGS)

blddeffile:
        -del $(EXEDIR)\test.def 2>nul;
	@echo LIBRARY          $(TSTNM) INITINSTANCE TERMINSTANCE      > $(EXEDIR)\TEST.DEF
	@echo DESCRIPTION      '$(TSTNM) Test DLL'                     >> $(EXEDIR)\TEST.DEF
	@echo CODE             PRELOAD MOVEABLE LOADONCALL DISCARDABLE >> $(EXEDIR)\TEST.DEF
	@echo DATA             PRELOAD MOVEABLE                        >> $(EXEDIR)\TEST.DEF
	@echo HEAPSIZE         5120                                    >> $(EXEDIR)\TEST.DEF
	@echo PROTMODE                                                 >> $(EXEDIR)\TEST.DEF
	@echo SEGMENTS                                                 >> $(EXEDIR)\TEST.DEF
	@echo EXPORTS                                                  >> $(EXEDIR)\TEST.DEF
	@echo                  AutoTestName                            >> $(EXEDIR)\TEST.DEF
	@echo                  AutoTestDesc                            >> $(EXEDIR)\TEST.DEF
	@echo                  AutoTestFunc                            >> $(EXEDIR)\TEST.DEF

#----------------------------------------------------------------------------
# Dependencies
#----------------------------------------------------------------------------
$(OBJDIR)\$(TSTNM).obj: $(SRCDIR)\$(TSTNM).C \
!IF "$(HFILE)" == "Y"
	     		$(INCDIR)\$(TSTNM).H \
!ENDIF
	     		$(INCTEST)
	$(CC) -Fo$(OBJDIR)\$(@B).obj $(SRCDIR)\$(@B).c

#
#  This dependency is for auto tests with more than one .C file.
#
!IF "$(ADDL1)" == "Y"
$(OBJDIR)\$(TSTNM)1.obj: $(SRCDIR)\$(TSTNM)1.C \
!IF "$(HFILE)" == "Y"
			 $(INCDIR)\$(TSTNM).H \
!ENDIF
			 $(INCTEST)
	$(CC) -DADDL_AUTO_TEST -Fo$(OBJDIR)\$(@B).obj $(SRCDIR)\$(@B).c
!ENDIF

!IF "$(ADDL2)" == "Y"
$(OBJDIR)\$(TSTNM)2.obj: $(SRCDIR)\$(TSTNM)2.C \
!IF "$(HFILE)" == "Y"
			 $(INCDIR)\$(TSTNM).H \
!ENDIF
			 $(INCTEST)
	$(CC) -DADDL_AUTO_TEST -Fo$(OBJDIR)\$(@B).obj $(SRCDIR)\$(@B).c
!ENDIF

#
#  The stub is required to make a full DLL
#
$(OBJDIR)\DLLSTUB.obj: .\DLLSTUB.C 
	$(CC) -Fo$(OBJDIR)\$(@B).obj .\$(@B).c

#-------------------------------------------------------------------------
# Build .res file
#-------------------------------------------------------------------------
!IF "$(RCFILE)" == "Y"
$(RCDIR)\$(TSTNM).res: $(INCDIR)\$(TSTNM).H \
		       $(RCDIR)\$(TSTNM).rc 
	rc $(RFLAGS) $(RCDIR)\$(TSTNM).rc
!ENDIF

#-------------------------------------------------------------------------
# Now do the executable
#-------------------------------------------------------------------------
$(EXEDIR)\$(TSTNM).dll: $(OBJDIR)\$(TSTNM).OBJ \
                        $(OBJDIR)\DLLSTUB.OBJ \
                        $(ODBCROOT)\test20\lib\GATORTST.LIB \
!IF "$(ADDL1)" == "Y"
			$(OBJDIR)\$(TSTNM)1.OBJ \
!ENDIF
!IF "$(ADDL2)" == "Y"
			$(OBJDIR)\$(TSTNM)2.OBJ \
!ENDIF
!IF "$(RCFILE)" == "Y"
			$(RCDIR)\$(TSTNM).res \
!ENDIF
                        $(EXEDIR)\TEST.DEF
!IF "$(ADDL2)" == "Y"
        $(LINK) $(LFLAGS) @<<
$(OBJDIR)\$(TSTNM).OBJ  +
$(OBJDIR)\$(TSTNM)1.OBJ  +
$(OBJDIR)\$(TSTNM)2.OBJ  +
$(OBJDIR)\DLLSTUB.OBJ   
$@
$*.map
$(ODBCROOT)\lib\odbc.lib $(ODBCROOT)\test20\lib\gatortst.lib $(ODBCROOT)\lib\os2util.lib
$(EXEDIR)\test.def;
<<
!ELSE
!IF "$(ADDL1)" == "Y"
        $(LINK) $(LFLAGS) @<<
$(OBJDIR)\$(TSTNM).OBJ  +
$(OBJDIR)\$(TSTNM)1.OBJ  +
$(OBJDIR)\DLLSTUB.OBJ   
$@
$*.map
$(ODBCROOT)\lib\odbc.lib $(ODBCROOT)\test20\lib\gatortst.lib $(ODBCROOT)\lib\os2util.lib
$(EXEDIR)\test.def;
<<
!ELSE
        $(LINK) $(LFLAGS) @<<
$(OBJDIR)\$(TSTNM).OBJ  +
$(OBJDIR)\DLLSTUB.OBJ   
$@
$*.map
$(ODBCROOT)\lib\odbc.lib $(ODBCROOT)\test20\lib\gatortst.lib $(ODBCROOT)\lib\os2util.lib
$(EXEDIR)\test.def;
<<
!ENDIF
!ENDIF
!IF "$(RCFILE)" == "Y"
	rc $(RCDIR)\$(TSTNM).res $(EXEDIR)\$(TSTNM).dll
!ENDIF
	$(LIBUTL) /NOL $(EXEDIR)\$(TSTNM).lib $(EXEDIR)\test.def
