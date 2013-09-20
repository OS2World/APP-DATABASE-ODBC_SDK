@set TSTNM=%1
@set ADDL1=
@set ADDL2=
@set RCFILE=
@set HFILE=
@IF EXIST %11.c set ADDL1=Y
@IF EXIST %12.c set ADDL2=Y
@IF EXIST %1.rc set RCFILE=Y
@IF EXIST %1.h set HFILE=Y
@echo Building %1 ...
@nmake /NOLOGO -f quicktes.mak |tee %1.er
