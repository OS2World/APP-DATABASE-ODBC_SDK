/*
** CATALOG.C - This is the ODBC sample driver code for
** executing Data Dictionary functions.
**
**	This code is furnished on an as-is basis as part of the ODBC SDK and is
**	intended for example purposes only.
**
*/

#include "sample.h"

//	-	-	-	-	-	-	-	-	-

//	Have DBMS set up result set of Tables.

RETCODE SQL_API SQLTables(
	HSTMT	  hstmt,
	UCHAR FAR *szTableQualifier,
	SWORD	  cbTableQualifier,
	UCHAR FAR *szTableOwner,
	SWORD	  cbTableOwner,
	UCHAR FAR *szTableName,
	SWORD	  cbTableName,
	UCHAR FAR *szTableType,
	SWORD	  cbTableType)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Have DBMS set up result set of Columns.

RETCODE SQL_API SQLColumns(
	HSTMT	  hstmt,
	UCHAR FAR *szTableQualifier,
	SWORD	  cbTableQualifier,
	UCHAR FAR *szTableOwner,
	SWORD	  cbTableOwner,
	UCHAR FAR *szTableName,
	SWORD	  cbTableName,
	UCHAR FAR *szColumnName,
	SWORD	  cbColumnName)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Have DBMS set up result set of Statistics.

RETCODE SQL_API SQLStatistics(
	HSTMT	  hstmt,
	UCHAR FAR *szTableQualifier,
	SWORD	  cbTableQualifier,
	UCHAR FAR *szTableOwner,
	SWORD	  cbTableOwner,
	UCHAR FAR *szTableName,
	SWORD	  cbTableName,
	UWORD	  fUnique,
	UWORD	  fAccuracy)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Have DBMS set up result set of TablePrivileges.

RETCODE SQL_API SQLTablePrivileges(
	HSTMT	  hstmt,
	UCHAR FAR *szTableQualifier,
	SWORD	  cbTableQualifier,
	UCHAR FAR *szTableOwner,
	SWORD	  cbTableOwner,
	UCHAR FAR *szTableName,
	SWORD	  cbTableName)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Have DBMS set up result set of ColumnPrivileges.

RETCODE SQL_API SQLColumnPrivileges(
	HSTMT	  hstmt,
	UCHAR FAR *szTableQualifier,
	SWORD	  cbTableQualifier,
	UCHAR FAR *szTableOwner,
	SWORD	  cbTableOwner,
	UCHAR FAR *szTableName,
	SWORD	  cbTableName,
	UCHAR FAR *szColumnName,
	SWORD	  cbColumnName)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Have DBMS set up result set of SpecialColumns.

RETCODE SQL_API SQLSpecialColumns(
	HSTMT	  hstmt,
	UWORD	  fColType,
	UCHAR FAR *szTableQualifier,
	SWORD	  cbTableQualifier,
	UCHAR FAR *szTableOwner,
	SWORD	  cbTableOwner,
	UCHAR FAR *szTableName,
	SWORD	  cbTableName,
	UWORD	  fScope,
	UWORD	  fNullable)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Have DBMS set up result set of PrimaryKeys.

RETCODE SQL_API SQLPrimaryKeys(
	HSTMT	  hstmt,
	UCHAR FAR *szTableQualifier,
	SWORD	  cbTableQualifier,
	UCHAR FAR *szTableOwner,
	SWORD	  cbTableOwner,
	UCHAR FAR *szTableName,
	SWORD	  cbTableName)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Have DBMS set up result set of ForeignKeys.

RETCODE SQL_API SQLForeignKeys(
	HSTMT	  hstmt,
	UCHAR FAR *szPkTableQualifier,
	SWORD	  cbPkTableQualifier,
	UCHAR FAR *szPkTableOwner,
	SWORD	  cbPkTableOwner,
	UCHAR FAR *szPkTableName,
	SWORD	  cbPkTableName,
	UCHAR FAR *szFkTableQualifier,
	SWORD	  cbFkTableQualifier,
	UCHAR FAR *szFkTableOwner,
	SWORD	  cbFkTableOwner,
	UCHAR FAR *szFkTableName,
	SWORD	  cbFkTableName)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Have DBMS set up result set of Procedures.

RETCODE SQL_API SQLProcedures(
	HSTMT	  hstmt,
	UCHAR FAR *szProcQualifier,
	SWORD	  cbProcQualifier,
	UCHAR FAR *szProcOwner,
	SWORD	  cbProcOwner,
	UCHAR FAR *szProcName,
	SWORD	  cbProcName)
{
	return SQL_SUCCESS;
}

//	-	-	-	-	-	-	-	-	-

//	Have DBMS set up result set of ProcedureColumns.

RETCODE SQL_API SQLProcedureColumns(
	HSTMT	  hstmt,
	UCHAR FAR *szProcQualifier,
	SWORD	  cbProcQualifier,
	UCHAR FAR *szProcOwner,
	SWORD	  cbProcOwner,
	UCHAR FAR *szProcName,
	SWORD	  cbProcName,
	UCHAR FAR *szColumnName,
	SWORD	  cbColumnName)
{
	return SQL_SUCCESS;
}
