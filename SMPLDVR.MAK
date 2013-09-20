##########################################################################
#
# makefile.os2 -- Build ODBC Sample Driver
#
#	This code is furnished on an as-is basis as part of the ODBC SDK and is
#	intended for example purposes only.
#
##########################################################################

##########################################################################
#
# Constants
#
##########################################################################
PRODUCT = ODBC Sample Driver

NAME	= SAMPLE

ODBCINC	= ..\..\include
ODBCLIB	= ..\..\lib
DM	= ..\..\dm

INCLUDE	= $(ODBCINC);$(INCLUDE)
LIB	= $(ODBCLIB);$(LIB)

# Compiler, linker, and RC compiler flags.

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

RCFLAGS	= /DOS2ODBC
CFLAGS	= $(CFLAGS) /Wpro+ /Ge- /C /Ms /Ss /Fi+ /Si+ /DOS2ODBC::1 /DEXTENSIONS
CFLAGS	= $(CFLAGS) $(CDEBUG) /DWIN32 /Gm-
LFLAGS	= /NOLOGO /NOE /NOI $(LDEBUG)
CC	= icc
LINK	= link386  

# Generic rules

.c{$(OBJDIR)\}.obj:
	$(CC) $(CFLAGS) -Fo$(OBJDIR)\$(@B).obj $(@B).c

# Targets

all	: $(OBJDIR)\$(NAME).dll 

# Dependencies

$(OBJDIR)\catalog.obj:	$$(@B).c $(ODBCINC)\sql.h $(ODBCINC)\sqltypes.h $(ODBCINC)\sqlext.h sample.h
$(OBJDIR)\connect.obj:	$$(@B).c $(ODBCINC)\sql.h $(ODBCINC)\sqltypes.h $(ODBCINC)\sqlext.h sample.h
$(OBJDIR)\dll.obj:	$$(@B).c $(ODBCINC)\sql.h $(ODBCINC)\sqltypes.h $(ODBCINC)\sqlext.h sample.h
$(OBJDIR)\execute.obj:	$$(@B).c $(ODBCINC)\sql.h $(ODBCINC)\sqltypes.h $(ODBCINC)\sqlext.h sample.h
$(OBJDIR)\info.obj:	$$(@B).c $(ODBCINC)\sql.h $(ODBCINC)\sqltypes.h $(ODBCINC)\sqlext.h sample.h
$(OBJDIR)\options.obj:	$$(@B).c $(ODBCINC)\sql.h $(ODBCINC)\sqltypes.h $(ODBCINC)\sqlext.h sample.h
$(OBJDIR)\prepare.obj:	$$(@B).c $(ODBCINC)\sql.h $(ODBCINC)\sqltypes.h $(ODBCINC)\sqlext.h sample.h
$(OBJDIR)\results.obj:	$$(@B).c $(ODBCINC)\sql.h $(ODBCINC)\sqltypes.h $(ODBCINC)\sqlext.h sample.h
$(OBJDIR)\setup.obj:	$$(@B).c $(ODBCINC)\sql.h $(ODBCINC)\sqltypes.h $(ODBCINC)\sqlext.h sample.h
$(OBJDIR)\transact.obj:	$$(@B).c $(ODBCINC)\sql.h $(ODBCINC)\sqltypes.h $(ODBCINC)\sqlext.h sample.h

$(OBJDIR)\dummy.obj:	$$(@B).c 

# RC compile

$(NAME).res:	sample.rc $(ODBCINC)\odbcver.h sample.h
	rc $(RCFLAGS) $(RDEBUG) -r -DWIN32 sample.rc

$(NAME).def:	smpldvr.mak
	@echo	LIBRARY		SAMPLE					> $@
	@echo	DESCRIPTION	'$(PRODUCT) '			>>$@
	@echo	CODE		MOVEABLE DISCARDABLE	>>$@
	@echo	DATA		MOVEABLE SINGLE			>>$@
	@echo	HEAPSIZE	1024					>>$@
	@echo	EXPORTS								>>$@
	@echo	SQLAllocConnect 	@1				>>$@
	@echo	SQLAllocEnv 		@2				>>$@
	@echo	SQLAllocStmt		@3				>>$@
	@echo	SQLBindCol			@4				>>$@
	@echo	SQLCancel			@5				>>$@
	@echo	SQLColAttributes	@6				>>$@
	@echo	SQLConnect			@7				>>$@
	@echo	SQLDescribeCol		@8				>>$@
	@echo	SQLDisconnect		@9				>>$@
	@echo	SQLError			@10 			>>$@
	@echo	SQLExecDirect		@11 			>>$@
	@echo	SQLExecute			@12 			>>$@
	@echo	SQLFetch			@13 			>>$@
	@echo	SQLFreeConnect		@14 			>>$@
	@echo	SQLFreeEnv			@15 			>>$@
	@echo	SQLFreeStmt 		@16 			>>$@
	@echo	SQLGetCursorName	@17 			>>$@
	@echo	SQLNumResultCols	@18 			>>$@
	@echo	SQLPrepare			@19 			>>$@
	@echo	SQLRowCount 		@20 			>>$@
	@echo	SQLSetCursorName	@21 			>>$@
#	@echo	SQLSetParam 		@22 			>>$@ // 1.0 drivers only
	@echo	SQLTransact 		@23 			>>$@
	@echo	SQLColumns			@40 			>>$@
	@echo	SQLDriverConnect	@41 			>>$@
	@echo	SQLGetConnectOption @42 			>>$@
	@echo	SQLGetData			@43 			>>$@
	@echo	SQLGetFunctions 	@44 			>>$@
	@echo	SQLGetInfo			@45 			>>$@
	@echo	SQLGetStmtOption	@46 			>>$@
	@echo	SQLGetTypeInfo		@47 			>>$@
	@echo	SQLParamData		@48 			>>$@
	@echo	SQLPutData			@49 			>>$@
	@echo	SQLSetConnectOption @50 			>>$@
	@echo	SQLSetStmtOption	@51 			>>$@
	@echo	SQLSpecialColumns	@52 			>>$@
	@echo	SQLStatistics		@53 			>>$@
	@echo	SQLTables			@54 			>>$@
	@echo	SQLBrowseConnect	@55 			>>$@
	@echo	SQLColumnPrivileges @56 			>>$@
#	@echo	SQLDataSources		@57 			>>$@ // Driver Manager only
	@echo	SQLDescribeParam	@58 			>>$@
	@echo	SQLExtendedFetch	@59 			>>$@
	@echo	SQLForeignKeys		@60 			>>$@
	@echo	SQLMoreResults		@61 			>>$@
	@echo	SQLNativeSql		@62 			>>$@
	@echo	SQLNumParams		@63 			>>$@
	@echo	SQLParamOptions 	@64 			>>$@
	@echo	SQLPrimaryKeys		@65 			>>$@
	@echo	SQLProcedureColumns @66 			>>$@
	@echo	SQLProcedures		@67 			>>$@
	@echo	SQLSetPos			@68 			>>$@
	@echo	SQLSetScrollOptions @69 			>>$@
	@echo	SQLTablePrivileges	@70 			>>$@
#	@echo	SQLDrivers			@71 			>>$@ //	Driver Manager only
	@echo	SQLBindParameter	@72 			>>$@
	@echo	LoadByOrdinal		@199			>>$@
	@echo	FDriverConnectProc	@333			>>$@
	@echo	ConfigDlgProc		@334			>>$@
	@echo	ConfigDSN			@335			>>$@



NAMEOBJ	=	$(OBJDIR)\catalog.obj \
				$(OBJDIR)\dll.obj \
				$(OBJDIR)\connect.obj \
				$(OBJDIR)\execute.obj \
				$(OBJDIR)\info.obj \
				$(OBJDIR)\options.obj \
				$(OBJDIR)\prepare.obj \
				$(OBJDIR)\results.obj \
				$(OBJDIR)\setup.obj \
				$(OBJDIR)\transact.obj \
				$(OBJDIR)\dummy.obj

$(OBJDIR)\$(NAME).dll:	$(NAMEOBJ)	\
		$(NAME).def		\
		$(NAME).res
  $(LINK) $(LFLAGS) @<<
$(OBJDIR)\catalog.obj+
$(OBJDIR)\dll.obj+
$(OBJDIR)\connect.obj+
$(OBJDIR)\execute.obj+
$(OBJDIR)\info.obj+
$(OBJDIR)\options.obj+
$(OBJDIR)\prepare.obj+
$(OBJDIR)\results.obj+
$(OBJDIR)\setup.obj+
$(OBJDIR)\transact.obj+
$(OBJDIR)\dummy.obj,
$(OBJDIR)\$(NAME).dll,
$(OBJDIR)\$(NAME).map,
$(ODBCLIB)\odbc.lib $(ODBCLIB)\odbccp.lib $(ODBCLIB)\os2util.lib,
$(NAME).def;
<<
	rc $(NAME).res $(OBJDIR)\$(NAME).dll

#	-	-	-	-	-	-	-	-	-

clean:
	-del $(OBJDIR)\*.obj
	-del $(OBJDIR)\$(NAME).dll
	-del $(NAME).res
	-del $(OBJDIR)\$(NAME).map

#	-	-	-	-	-	-	-	-	-
