[$$SETUP$$]
Comment=;
Extension=.au3
OnlyNewEntries=1
LogUNLOrder=0
OutSeparateObjs=0
NoVals=1

[$$HEADER$$] 
{{##LINEFEED}}
{{##COMMENT}} ==============================================================================================================
{{##COMMENT}}                        !!! USE AUTOIT3_64.EXE TO HIT THE CORRECT REGISTRY KEYS !!!
{{##COMMENT}} ==============================================================================================================
{{##LINEFEED}}
#cs
{{##COMMENT}} Maintence.SKT:
{{##LEFTBRACKET}}] 
Transaction={{##TITLE}}
TEXT=Regshot's uninstaller for "{{##TITLE}}"
SAP=0
DeleteReadOnly = {{##DELETEREADONLY}}
DeleteDirRecurse = {{##DELETERECURSE}}
{{##LINEFEED}}
{{##COMMENT}} I18N\SAPGui-Automation.de.lng, SAPGui-Automation.en.lng:
{{##LEFTBRACKET}}{{##TITLE}}]
NAME=Unlock {{##TITLE}}
#ce
{{##LINEFEED}}
AutoItSetOption("MustDeclareVars", 1)
#RequireAdmin
{{##LINEFEED}}
Dim $bSimulation = 0
Dim $strTitle = "Regshot's uninstaller for ""{{##TITLE}}""" & ($bSimulation ? " (SIMULATION)" : "")
Dim $bLog = 1
Dim $hOpenFile = -1
Dim $bAppendLog = 1 ; 1 = Append, 2 = Overwrite
Dim $bReadyMessage = 1
Dim $LogFileName = @ScriptDir & "\" & "{{##SCRIPTNAME}}.log"
Dim $ret = 1
If $bReadyMessage = 1 Then MsgBox (65, $strTitle, "Start script """ & @ScriptFullPath & """?", 3)
if $ret = 2 Then exit
{{##LINEFEED}}
{{##TITLE}}()
{{##LINEFEED}}
Local $strLogLine = "Script """ & @ScriptFullPath & """ finished."
_FileWriteLog($hOpenFile, $strLogLine)
If $hOpenFile <> -1 Then FileClose($hOpenFile)
If $bReadyMessage = 1 Then MsgBox (64, $strTitle, $strLogLine)
{{##LINEFEED}}
Func {{##TITLE}}()
{{##BLANK}} Dim $bDeleteReadOnly = {{##DELETEREADONLY}}
{{##BLANK}} Dim $bDeleteDirRecurse = {{##DELETERECURSE}}
{{##BLANK}} If $bLog = 1 Then $hOpenFile = FileOpen($LogFileName, $bAppendLog)
{{##BLANK}} _FileWriteLog($hOpenFile, "")
{{##BLANK}} _FileWriteLog($hOpenFile, "Script """ & @ScriptFullPath & """ started.")
{{##BLANK}} _FileWriteLog($hOpenFile, "--------------------------------------------------------------------------------------------------------------")
{{##BLANK}} _FileWriteLog($hOpenFile, "DeleteReadOnly: " & ($bDeleteReadOnly ? "1" : "0"))
{{##BLANK}} _FileWriteLog($hOpenFile, "DeleteDirRecurse: " & ($bDeleteDirRecurse ? "1" : "0"))
{{##BLANK}} _FileWriteLog($hOpenFile, "Simulation: " & ($bSimulation ? "1" : "0"))
{{##BLANK}} _FileWriteLog($hOpenFile, "--------------------------------------------------------------------------------------------------------------")

[$$DATALINES$$]
RegDefaultVal=""
VALDELETENormal=         {{##BLANK}} _REGVALDEL("{{##WHOLEKEYNAME}}", "{{##VALNAME}}")
VALDELETEShort =         {{##BLANK}} _REGVALDEL("{{##WHOLEKEYNAME}}", "{{##VALNAME}}")
KEYDELETENormal=         {{##BLANK}} _REGKEYDEL("{{##WHOLEKEYNAME}}")
FILEDELETENormal=        {{##BLANK}} _DEL("{{##FILENAME}}", $bDeleteReadOnly)
FILEDELETEDeleteReadOnly={{##BLANK}} _DEL("{{##FILENAME}}", $bDeleteReadOnly)
DIRDELETENormal=         {{##BLANK}} _RMDIR("{{##DIRNAME}}", $bDeleteDirRecurse)
DIRDELETEDeleteRecurse=  {{##BLANK}} _RMDIR("{{##DIRNAME}}", $bDeleteDirRecurse)

[$$FOOTER$$]
Endfunc
{{##LINEFEED}}
{{##COMMENT}} --------------------------------------------------------------------------------------------------------------
{{##COMMENT}} Coding, Routines
{{##COMMENT}} --------------------------------------------------------------------------------------------------------------
Func _RMDIR($DIR, $RECURSE)
{{##BLANK}} Local $strLogLine = "Directory deleted : "
{{##BLANK}} If Not FileExists($DIR) Then
{{##BLANK}}   $strLogLine = "=> Directory doesn't exist: "
{{##BLANK}} Elseif Not $bSimulation = 1 Then
{{##BLANK}}   If Not DirRemove($DIR, $RECURSE) Then $strLogLine = "=> Directory couldn't be deleted: "
{{##BLANK}} Endif
{{##BLANK}} $strLogLine = $strLogLine &  """" & $DIR & """"
{{##BLANK}} _FileWriteLog($hOpenFile, $strLogLine)
EndFunc
{{##LINEFEED}}
Func _DEL($FILE, $READONLY)
{{##BLANK}} Local $strLogLine = "File(s) deleted : "
{{##BLANK}} If Not FileExists($FILE) Then
{{##BLANK}}   $strLogLine = "=> File doesn't exist: "
{{##BLANK}} Elseif Not $bSimulation = 1 Then
{{##BLANK}}   If $READONLY = 1 Then FileSetAttrib($FILE, "-R")
{{##BLANK}} 	If Not FileDelete($FILE) Then $strLogLine = "=> File couldn't be deleted: "
{{##BLANK}} Endif
{{##BLANK}} $strLogLine = $strLogLine &  """" & $FILE & """"
{{##BLANK}} _FileWriteLog($hOpenFile, $strLogLine)
EndFunc
{{##LINEFEED}}
Func _REGKEYDEL($KEYNAME)
{{##BLANK}} Local $strLogLine = "Registry key deleted: "
{{##BLANK}} RegEnumKey($KEYNAME, 0)
{{##BLANK}} If @error = 1 Then 
{{##BLANK}}   $strLogLine="=> Registry key doesn't exist: "
{{##BLANK}} ElseIf Not $bSimulation = 1 Then
{{##BLANK}}   If RegDelete($KEYNAME) = 2 Then $strLogLine = "=> Registry key couldn't be deleted: "
{{##BLANK}} Endif
{{##BLANK}} $strLogLine = $strLogLine &  """" & $KEYNAME & """"
{{##BLANK}} _FileWriteLog($hOpenFile, $strLogLine)
EndFunc
{{##LINEFEED}}
Func _REGVALDEL($KEYNAME, $VALNAME)
{{##BLANK}} Local $strLogLine = "Registry value deleted: "
{{##BLANK}} RegRead($KEYNAME, $VALNAME)
{{##BLANK}} If @error = -1 or @error = 1 Then 
{{##BLANK}}   $strLogLine="=> Registry value doesn't exist: "
{{##BLANK}} ElseIf $bSimulation = 0 Then
{{##BLANK}}   If RegDelete($KEYNAME, $VALNAME) = 2 Then $strLogLine = "=> Registry value couldn't be deleted: "
{{##BLANK}} Endif
{{##BLANK}} $strLogLine = $strLogLine &  """" & $KEYNAME & """, """ & $VALNAME & """"
{{##BLANK}} _FileWriteLog($hOpenFile, $strLogLine)
EndFunc
{{##LINEFEED}}
Func _FileWriteLog($hOpenFile, $sLogMsg )
{{##BLANK}} Local $sDateNow
{{##BLANK}} Local $sTimeNow
{{##BLANK}} Local $sMsg
{{##LINEFEED}}
{{##BLANK}} $sDateNow = @MDAY & "." & @MON & "." & @YEAR
{{##BLANK}} $sTimeNow = @Hour & ":" & @MIN & ":" & @SEC & "." & @MSEC
{{##BLANK}} $sMsg = $sDateNow & " " & $sTimeNow & "   " & $sLogMsg
{{##BLANK}}
{{##BLANK}} If $bLog = 1 Then FileWriteLine($hOpenFile, $sMsg)
EndFunc
