@ECHO OFF
@ECHO Begin geneate VC project files
qmake -tp vc -r
@ECHO End geneate VC project files
@ECHO --------------------------------------
set choice=n
set /p choice=Do you want to open QtDemo.sln(y/n,default to n)?
IF %choice% == y (QtDemo.sln) ELSE (ECHO Do nothing!)

