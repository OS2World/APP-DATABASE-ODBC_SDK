REM Place the name of all tests inside the parenthesis and they will be built in order.
REM Example:
REM     @for %%1 in (ALLOCCON ALLOCENV ALLOCST) do @CALL build2.cmd %%1
REM

@for %%1 in (QUIKTEST) do @CALL build2.cmd %%1
