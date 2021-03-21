[$$SETUP$$]
Comment=::
Extension=.cmd
OnlyNewEntries=1
LogUNLOrder=0
OutSeparateObjs=0
NoVals=1
UseLongRegHead=0

[$$DATALINES$$]
RegDefaultVal={{##BS}}{{##BS}}e{{##DEL}}
VALDELETENormal=         REG DELETE "{{##WHOLEKEYNAME}}" /f /reg:64 /v "{{##VALNAME}}"
VALDELETEShort =         REG DELETE "{{##WHOLEKEYNAME}}" /f /reg:64 /v "{{##VALNAME}}"
KEYDELETENormal=         REG DELETE "{{##WHOLEKEYNAME}}" /f	/reg:64 
FILEDELETENormal=        DEL "{{##FILENAME}}"
FILEDELETEDeleteReadOnly=DEL /F "{{##FILENAME}}"
DIRDELETENormal=         RMDIR "{{##DIRNAME}}"
DIRDELETEDeleteRecurse=  RMDIR /S /Q "{{##DIRNAME}}"
