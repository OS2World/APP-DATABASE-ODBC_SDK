  �         T      �   T      �  T      E  T      I  T   d   �  T   e   -  T   f   �  T   g   �  T   h   �  T   i   �  T   j   �	  T   k   �
  T   l   �  T   m   S  T   n   v  T   o   �  T   p   �  T   q   �  T   r   �  T   s   M  T   t      T   u   �  T   v   �  T   w   A  T   x   �  T   y   �  T   z     T   {   �  T   |   R  T   }   u  T   ~   3  T        T   �   �  T   �   �  T   �   �  T   �   �   T   �   o!  T   �   "  T   �   �"  T   �   �#  T   �   $  T   �   �$  T   �   E%  T   �   �%  T   �   X&  T   �   '  T   �   �'  T   �   D(  T   �   �(  T   �   _)  T   �   �)  T   �   f*  T   �   +  T   �   �+  T   �   0,  T   �   �,  T   �   �-  T   �   o.  T   �   /  T   �   @0  T   �   �0  T   �   �1  T   �   2  T   �   �2  T   �   K3  T   �   �3  T   �   �4  T   �   Z5  T   �   �5  T   �   �6  T   �   8  T   �   �9  T   �   U;  T   �   \<  T   
CLI0001W Disconnect error.  

Explanation:  An error occurred during the disconnect.
However, the disconnect succeeded.  

User Response:  Validate that the communication between the
client and the server are still active.  

 
CLI0002W Data truncated.  

Explanation:  An output buffer specified is not large
enough to contain the data.  

User Response:  Increase the size of the output buffer.  

 
CLI0003W Privilege not revoked.  

Explanation:  The statement was a REVOKE statement and the
user did not have the specified privilege.  

User Response:  No action needed.  

 
CLI0004W Invalid connection string attribute.  

Explanation:  An invalid or unsupported connection string
attribute was specified in the connection string but the
driver was able to connect to the data source anyway.  

User Response:  No action needed.  

 
CLI0005W Option value changed.  

Explanation:  The driver did not support the specified
option value and substituted a similar value.  

User Response:  No action needed.  

 
CLI0100E Wrong number of parameters.  

Explanation:  The number of parameters specified in
SQLSetParam or SQLBindParameter was less than number of
parameters in the SQL statement.  

User Response:  Respecify the SQL statement or provide more
input parameters through SQLSetParam or SQLBindParameter.  

 
CLI0101E The statement did not return a result set.  

Explanation:  The previous statement does not result in a
result set.  

User Response:  Respecify the SQL statement.  

 
CLI0102E Invalid conversion.  

Explanation:  The conversion between the application data
type and SQL data type is not supported by the driver.  

User Response:  Respecify a data conversion which is
supported by the driver.  

 
CLI0103E Too many columns.  

Explanation:  The number of columns specified in SQLBindCol
is greater than the number of columns in the current result
set.  

User Response:  Respecify the SQL statement or reset the
bound column information in SQLBindCol.  

 
CLI0104E Unable to connect to data source.  

Explanation:  The driver was unable to establish a
connection with the data source.  

User Response:  Ensure the server is started and that the
communications between the client and server are correct.  

 
CLI0105E Connection in use.  

Explanation:  The specified connection handle is already
being used and the connection is still opened.  

User Response:  Allocate a new connection using
SQLAllocConnect and retry the connection or terminate the
existing connection.  

 
CLI0106E Connection is closed.  

Explanation:  The connection specified by the connection
handle is no longer active.  

User Response:  Establish a new connection.  

 
CLI0107E Connection failure during transaction.  

Explanation:  The connection failed during the execution of
the function and it cannot be determined whether the COMMIT
or ROLLBACK occurred before the failure.  

User Response:  Establish a new connection.  

 
CLI0108E Communication link failure.  

Explanation:  The connection between the driver and the
data source failed during execution of this function.  

User Response:  Establish a new connection.  

 
CLI0109E String data right truncation.  

Explanation:  The data specified through SQLSetParam or
SQLBindParameter is larger than the maximum size allowed
for the corresponding use of a parameter marker.  

User Response:  Respecify the parameter using SQLSetParam
or SQLBindParameter.  

 
CLI0110E Invalid output or indicator buffer specified.  

Explanation:  The returned data was NULL but the output or
indicator buffer specified was a NULL buffer.  

User Response:  Respecify the output or indicator buffer
supplying a non-NULL buffer and retry the operation.  

 
CLI0111E Numeric value out or range.  

Explanation:  Returning the numeric data would have caused
the whole part of the number to be truncated.  

SQLPutData was called more than once for a parameter and
the input data was not of type character or binary.  

User Response:  Respecify the output bindings either
through SQLBindCol or SQLGetData to avoid creating a
numeric data truncation.  

Do not call SQLPutData for a parameter if the application
data type specified for that parameter through SQLSetParam
or SQLBindParameter is not SQL_C_CHAR or SQL_C_BINARY.  

 
CLI0112E Error in assignment.  

Explanation:  The data sent for a parameter or column was
incompatible with the data type of the associated table
column.  

User Response:  Respecify the output binding through
SQLBindCol or SQLGetData or the input binding through
SQLSetParam or SQLBindParameter.  

 
CLI0113E Invalid date format.  

Explanation:  The data sent for a date field was invalid.  

User Response:  Respecify the date data.  

 
CLI0114E Datetime field overflow.  

Explanation:  The data sent for a date, time, or timestamp
parameter, or column, was invalid.  

User Response:  Respecify the date, time, or timestamp
data.  

 
CLI0115E Invalid cursor state.  

Explanation:  The statement is not positioned on a row.  

User Response:  Position the statement on a row by calling
SQLFetch or SQLExtendedFetch and retry the operation.  

 
CLI0116E Invalid transaction state.  

Explanation:  There was a transaction in progress when
SQLDisconnect was called.  

User Response:  Call SQLTransact before calling
SQLDisconnect.  

 
CLI0117E Invalid cursor name.  

Explanation:  An invalid or duplicate cursor name was
specified in SQLSetCursorName.  

User Response:  Respecify a valid cursor name through
SQLSetCursorName.  

 
CLI0118E Invalid SQL syntax.  

Explanation:  An invalid or incorrect SQL statement was
specified.  

User Response:  Respecify a valid SQL statement.  

 
CLI0119E Unexpected system failure.  

Explanation:  An unexpected system failure occurred while
processing the function.  

User Response:  Restart the application and try again.  

 
CLI0120E Memory allocation failure.  

Explanation:  The driver was unable to allocate memory
required to support execution or completion of the
function.  

User Response:  Verify that your system has enough memory
to complete the desired operation.  

 
CLI0121E Invalid column number.  

Explanation:  The value specified in iCol is less than
zero, greater than the number of columns in the result set,
or greater than the maximum number of columns allowed in a
result set.  

User Response:  Respecify a valid value for iCol.  

 
CLI0122E Program type out of range.  

Explanation:  The value specified for fCType is not valid.  

User Response:  Respecify a valid value for fCType.  

 
CLI0123E SQL data type out of range.  

Explanation:  The value specified for fSQLType is not
valid.  

User Response:  Respecify a valid value for fSQLType.  

 
CLI0124E Invalid argument value.  

Explanation:  The value specified for an argument was
invalid. Possible reasons may be a null pointer, invalid
length, invalid option, etc.  

User Response:  Re-examine the argument passed in to the
function and determine which argument is invalid.  

 
CLI0125E Function sequence error.  

Explanation:  This function was called in an invalid
sequence.  

User Response:  Correct the sequence in your application
and retry the operation.  

 
CLI0126E Operation invalid at this time.  

Explanation:  The operation the system is trying to perform
is invalid at this time.  

User Response:  Correct the sequence of operation and retry
the operation.  

 
CLI0127E Invalid transaction code.  

Explanation:  The transaction option specified in
SQLTransact was not SQL_COMMIT or SQL_ROLLBACK.  

User Response:  Specify either SQL_COMMIT or SQL_ROLLBACK
and retry the operation.  

 
CLI0128E Unexpected memory handling error.  

Explanation:  Memory handling error.  

User Response:  An unexpected error occurred in the driver
while handling some internal memory buffer. Restart your
application.  

 
CLI0129E No more handles.  

Explanation:  A call was made to SQLAllocEnv,
SQLAllocConnect, or SQLAllocStmt but no more handles are
available in the driver.  

User Response:  Call SQLFreeEnv, SQLFreeConnect, or
SQLFreeStmt to free up some handles which are no longer
being used in the application.  

 
CLI0130E No cursor name available.  

Explanation:  There was no open cursor on the statement and
no cursor had been set with SQLSetCursorName.  

User Response:  Specify a cursor name through
SQLSetCursorName.  

 
CLI0131E Invalid string or buffer length.  

Explanation:  The length of the buffer specified is
invalid.  

User Response:  Specify a valid buffer length.  

 
CLI0132E Descriptor type out of range.  

Explanation:  The description type specified is not valid.  

User Response:  Specify a valid descriptor type.  

 
CLI0133E Option type out of range.  

Explanation:  The option type specified is not valid.  

User Response:  Specify a valid option type.  

 
CLI0134E Invalid parameter number.  

Explanation:  The number specified for the parameter number
is less than zero or greater than the maximum parameter
supported by the data source.  

User Response:  Specify a valid parameter number.  

 
CLI0135E Invalid scale value.  

Explanation:  The scale value specified is invalid.  

User Response:  Specify a valid scale value.  

 
CLI0136E Function type out of range.  

Explanation:  The function type is invalid.  

User Response:  Specify a valid function type value.  

 
CLI0137E Information type out of range.  

Explanation:  The information type is invalid.  

User Response:  Specify a valid information type value.  

 
CLI0138E Column type out of range.  

Explanation:  The column type is invalid.  

User Response:  Specify a valid column type value.  

 
CLI0139E Scope type out of range.  

Explanation:  The scope type is invalid.  

User Response:  Specify a valid scope type value.  

 
CLI0140E Uniqueness option type out of range.  

Explanation:  The uniqueness option type is invalid.  

User Response:  Specify a valid uniqueness option type
value.  

 
CLI0141E Accuracy option type out of range.  

Explanation:  The accuracy option type is invalid.  

User Response:  Specify a valid accuracy option type value.  

 
CLI0142E Direction option out of range.  

Explanation:  The direction option is invalid.  

User Response:  Specify a valid direction option value.  

 
CLI0143E Invalid precision value.  

Explanation:  The precision value is invalid.  

User Response:  Specify a valid precision value.  

 
CLI0144E Invalid parameter type.  

Explanation:  The parameter type is invalid.  

User Response:  Specify a valid parameter type value.  

 
CLI0145E Fetch type out of range.  

Explanation:  The fetch type is invalid.  

User Response:  Specify a valid fetch type value.  

 
CLI0146E Row value out of range.  

Explanation:  The row value is invalid.  

User Response:  Specify a valid row value.  

 
CLI0147E Concurrency option out of range.  

Explanation:  The concurrency option is invalid.  

User Response:  Specify a valid concurrency option value.  

 
CLI0148E Invalid cursor position.  

Explanation:  The cursor position is invalid.  

User Response:  Specify a valid cursor position value.  

 
CLI0149E Invalid driver completion.  

Explanation:  The driver completion is invalid.  

User Response:  Specify a valid driver completion value.  

 
CLI0150E Driver not capable.  

Explanation:  The operation is valid but not supported by
either the driver or the data source.  

User Response:  Specify a valid operation.  

 
CLI0151E No data pending.  

Explanation:  SQLParamData or SQLPutData was called but
there is no data at execute pending on this statement.  

User Response:  Respecify the parameters through
SQLSetParam or SQLBindParameter.  

 
CLI0152E Not a string value.  

Explanation:  The function expected a string argument to be
supplied.  

User Response:  Respecify the argument to the function.  

 
CLI0153E Invalid file name length.  

Explanation:  The length specified for the file name is
invalid.  

User Response:  Specify a valid file name length.  

 
CLI0154E Invalid connection state.  

Explanation:  An attempt to change the connection type was
made but the connection or another connection (in the case
of Distributed Unit of Work) is already opened.  

User Response:  Do not attempt to change the connection
type once the connection is opened.  

 
CLI0155E File name length is too long.  

Explanation:  The length supplied for the file name is
greater than supported.  

User Response:  Specify a valid file name length.  

 
CLI0156E Error closing a file.  

Explanation:  An unexpected condition occurred while
closing a file.  

User Response:  Retry the operation.  

 
CLI0157E Error opening a file.  

Explanation:  An unexpected condition occurred while
opening a file.  

User Response:  Retry the operation.  

 
CLI0158E Error writing to a file.  

Explanation:  An unexpected condition occurred while
writing to a file.  

User Response:  Retry the operation.  

 
CLI0159E Error deleting a file.  

Explanation:  An unexpected condition occurred while
deleting a file.  

User Response:  Retry the operation.  

 
CLI0164E Nullable type out of range.  

Explanation:  An invalid nullable value was specified.  

User Response:  Respecify the nullable value.  

 
CLI0165E Error in row.  

Explanation:  An error occurred while fetching one or more
rows. (Function returns SQL_SUCCESS_WITH_INFO.) 

User Response:  Retry the operation.  

 
CLI0166E PARMLIST syntax error.  

Explanation:  The PARMLIST value in the stored procedures
catalog table contains a syntax error.  

User Response:  Respecify the row for this stored
procedure.  

 
CLI0167E Operation was cancelled.  

Explanation:  SQLCancel was called on the statement.  

User Response:  Resubmit the operation.  

 
CLI0171E The database was unable to connect.  Reason: 

Explanation:  The database connection failed. The 'Reason:'
will explain why the database was unable to connect.  

User Response:  Fix the problem, and try the connect again.  

 
CLI0172E The database was unable to connect.  Would you
          like to continue with the configuration? Reason: 

Explanation:  The database connection failed. The 'Reason:'
will explain why the database was unable to connect.  

User Response:  Fix the problem, and try the connect again
or continue with the configuration.  

 
CLI0175E No ODBC Driver Manager has been found.
          ODBCINST.INI file cannot be opened.  

Explanation:  Neither one of the Visigenic or the Intersolv
ODBC Driver Managers have been installed. An ODBC Driver
Manager must be present before the DB2 ODBC Driver can be
registered into the associated ODBCINST.INI file.  

User Response:  Install an ODBC Driver Manager, and try
running this command again.  

 
CLI0176E Unable to write to the ODBCINST.INI file.  

Explanation:  An error has occurred while writing to an
ODBCINST.INI file to be used with an ODBC Driver Manager.
This may be due to invalid format of the original
ODBCINST.INI file.  

User Response:  Rename the existing ODBCINST.INI file to
something else and try running this command again. If this
error persists, contact your technical support.  

 
CLI0177E Cannot determine where the DB2 OS/2 Client is
          installed.  

Explanation:  An error has occurred while trying to locate
where the DB2 OS/2 client is installed.  

User Response:  Check that the DB2 OS/2 Client has been
correctly installed.  

 
CLI0178I The DB2 ODBC Driver has been successfully
          registered.  

Explanation:  The DB2 ODBC Driver has been registered with
the installed ODBC Driver Manager(s).  

User Response:  The user can now use the appropriate ODBC
Administrator tool from the ODBC Driver Manager vendor to
configure the ODBC data sources.  
 