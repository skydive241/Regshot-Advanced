/*
    Copyright 2021-     Patrick (skydive241@gmx.de)
    
    This file is part of Regshot.

    Regshot is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    Regshot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Regshot.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "global.h"

BOOL WritePartTemplate(HANDLE hFile, LPTSTR lpszTemplate, LPTSTR lpszTask, LPOUTPUTFILEDESCRIPTION pOutputFileDescription)
{
    // TODO: Template-Leserei
    DWORD nSize = 32000;
    LPTSTR lpReturnedString = MYALLOC0(nSize * sizeof(TCHAR));
    DWORD cchSection = 0;

    LPTSTR lpszBlockLine = NULL;
    LPTSTR lpszOutputLine = NULL;
    
    if (lpszMacroFileName != NULL) {
        GetModuleFileNameW(NULL, lpszMacroFileName, MAX_PATH);
        PathRemoveFileSpec(lpszMacroFileName);
        _tcscat(lpszMacroFileName, TEXT("\\Templates\\"));
        _tcscat(lpszMacroFileName, lpszTemplate);
        if (_tcscmp(lpszTemplate, TEXT("COMMON")) == 0) {
            _tcscat(lpszMacroFileName, TEXT("-"));
            _tcscat(lpszMacroFileName, lpszLanguage);
        }
        _tcscat(lpszMacroFileName, TEXT(".tpl"));

        cchSection = GetPrivateProfileSection(
            lpszTask,
            lpReturnedString,
            nSize,
            lpszMacroFileName
            );
    }
    else {
        MYFREE(lpReturnedString);

        return FALSE;
    }

    //FILE* filePointer;
    //int bufferLength = 255;
    //LPTSTR buffer = MYALLOC0(MAX_PATH * sizeof(TCHAR));

    //filePointer = _tfopen(lpszMacroFileName, TEXT("r,ccs=UNICODE"));
    //while (_fgetts(buffer, bufferLength, filePointer)) {
    //    buffer[_tcscspn(buffer, "\n")] = 0;
    //    printf("%s", buffer);
    //}
    //fclose(filePointer);


    if (0 < cchSection) {
        lpszBlockLine = lpReturnedString;
        for (int i = 0; ; i++) {
            if (0 == *lpszBlockLine) {
                break;
            }
            lpszOutputLine = ReplaceHeaderMacros(lpszBlockLine, pOutputFileDescription);
            if (lpszOutputLine != NULL) {
                size_t len = _tcslen(lpszOutputLine);
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszOutputLine, (DWORD)(_tcslen(lpszOutputLine) * sizeof(TCHAR)), &NBW, NULL);
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
                if (lpszOutputLine != lpszBlockLine)
                    MYFREE(lpszOutputLine);
            }
            lpszBlockLine += _tcslen(lpszBlockLine) + 1;
        }
    }
    else {
        MYFREE(lpReturnedString);

        return FALSE;
    }

    MYFREE(lpReturnedString);

    return TRUE;
}

BOOL ReadOutputMacros(LPOUTPUTFILEDESCRIPTION pOutputFileDescription)
{
    DWORD cchDataline;
    DWORD nSize = MAX_PATH;
    
    if (lpszMacroFileName != NULL) {
        GetModuleFileNameW(NULL, lpszMacroFileName, MAX_PATH);
        PathRemoveFileSpec(lpszMacroFileName);
        _tcscat(lpszMacroFileName, TEXT("\\Templates\\"));
        _tcscat(lpszMacroFileName, pOutputFileDescription->lpszFormat);
        _tcscat(lpszMacroFileName, TEXT(".tpl"));

        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("RegDefaultVal"),               NULL, lpszRegDefaultVal           , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("MultiSzStringDivider"), TEXT("\", \""), lpszMultiSzStringDivider    , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("SuppressedKeyPart"),           NULL, lpszSuppressedKeyPart       , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("VALADDNormal"),                NULL, lpszVALADDNormal            , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("VALADDShort"),                 NULL, lpszVALADDShort             , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("VALCHANGENormal"),             NULL, lpszVALCHANGENormal         , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("VALCHANGEShort"),              NULL, lpszVALCHANGEShort          , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("VALDELETENormal"),             NULL, lpszVALDELETENormal         , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("VALDELETEShort"),              NULL, lpszVALDELETEShort          , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("KEYADDNormal"),                NULL, lpszKEYADDNormal            , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("KEYDELETENormal"),             NULL, lpszKEYDELETENormal         , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("FILEADDNormal"),               NULL, lpszFILEADDNormal           , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("FILECHANGENormal"),            NULL, lpszFILECHANGENormal        , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("FILEDELETENormal"),            NULL, lpszFILEDELETENormal        , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("FILEDELETEDeleteReadOnly"),    NULL, lpszFILEDELETEDeleteReadOnly, nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("DIRADDNormal"),                NULL, lpszDIRADDNormal            , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT(";DIRCHANGENormal"),            NULL, lpszDIRCHANGENormal         , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("DIRDELETENormal"),             NULL, lpszDIRDELETENormal         , nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), TEXT("DIRDELETEDeleteRecurse"),      NULL, lpszDIRDELETEDeleteRecurse  , nSize, lpszMacroFileName);
    
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("NameREG_SZ"),                    lpszEmpty, lpszNameREG_SZ,                    nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("NameREG_EXPAND_SZ"),             lpszEmpty, lpszNameREG_EXPAND_SZ,             nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("NameREG_MULTI_SZ"),              lpszEmpty, lpszNameREG_MULTI_SZ,              nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("NameREG_DWORD_LITTLE_ENDIAN"),   lpszEmpty, lpszNameREG_DWORD_LITTLE_ENDIAN,   nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("NameREG_DWORD_BIG_ENDIAN"),      lpszEmpty, lpszNameREG_DWORD_BIG_ENDIAN,      nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("NameREG_QWORD_LITTLE_ENDIAN"),   lpszEmpty, lpszNameREG_QWORD_LITTLE_ENDIAN,   nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("NameREG_NONE"),                  lpszEmpty, lpszNameREG_NONE,                  nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("NameREG_BINARY"),                lpszEmpty, lpszNameREG_BINARY,                nSize, lpszMacroFileName);
    
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("PrefixREG_SZ"),                  lpszEmpty, lpszPrefixREG_SZ,                  nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("PrefixREG_EXPAND_SZ"),           lpszEmpty, lpszPrefixREG_EXPAND_SZ,           nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("PrefixREG_MULTI_SZ"),            lpszEmpty, lpszPrefixREG_MULTI_SZ,            nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("PrefixREG_DWORD_LITTLE_ENDIAN"), lpszEmpty, lpszPrefixREG_DWORD_LITTLE_ENDIAN, nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("PrefixREG_DWORD_BIG_ENDIAN"),    lpszEmpty, lpszPrefixREG_DWORD_BIG_ENDIAN,    nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("PrefixREG_QWORD_LITTLE_ENDIAN"), lpszEmpty, lpszPrefixREG_QWORD_LITTLE_ENDIAN, nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("PrefixREG_NONE"),                lpszEmpty, lpszPrefixREG_NONE,                nSize, lpszMacroFileName);
        cchDataline = GetPrivateProfileString(TEXT("$$REGPREFS$$"), TEXT("PrefixREG_BINARY"),              lpszEmpty, lpszPrefixREG_BINARY,              nSize, lpszMacroFileName);
    
    }

    return TRUE;
}

// ----------------------------------------------------------------------
// Several routines to write to an output file
// ----------------------------------------------------------------------
LPTSTR ReplaceDataMacros(LPTSTR lpszDataLineMacro, LPTSTR lpszText, LPVOID lpContent, BOOL bSuppressKey, LPOUTPUTFILEDESCRIPTION pOutputFileDescription)
{
    size_t i = 0;
    LPTSTR pos = NULL;
    LPTSTR macroStart = TEXT("{{##");
    LPTSTR lpszBuffer = NULL;
    DWORD  nBufferSize = 2048;

    if (bSuppressKey && pOutputFileDescription->fGroupRegKeys &&
        lpszSuppressedKeyPart != NULL && 
        _tcslen(lpszSuppressedKeyPart) > 0 &&
        _tcsncmp(lpszDataLineMacro, lpszSuppressedKeyPart, _tcslen(lpszSuppressedKeyPart)) == 0)
        lpszDataLineMacro += _tcslen(lpszSuppressedKeyPart);
    
    if (lpszDataLineMacro != NULL)
        pos = _tcsstr(lpszDataLineMacro, macroStart);
    while (pos) {
        i++;
        pos = _tcsstr(++pos, macroStart);
    }

    if (i == 0)
        return lpszDataLineMacro;
    else
    {
        LPTSTR pROOTKEYNAME     = TEXT("{{##ROOTKEYNAME}}");
        LPTSTR pWHOLEKEYNAME    = TEXT("{{##WHOLEKEYNAME}}");
        LPTSTR pKEYNAME         = TEXT("{{##KEYNAME}}");
        LPTSTR pVALNAME         = TEXT("{{##VALNAME}}");
        LPTSTR pVALTYPE         = TEXT("{{##VALTYPE}}");
        LPTSTR pDIRNAME         = TEXT("{{##DIRNAME}}");
        LPTSTR pDIROFFILENAME   = TEXT("{{##DIROFFILENAME}}");
        LPTSTR pFILENAME        = TEXT("{{##FILENAME}}");
        LPTSTR pBLANK           = TEXT("{{##BLANK}}");
        LPTSTR pLINEFEED        = TEXT("{{##LINEFEED}}");
        LPTSTR pBS              = TEXT("{{##BS}}");
        LPTSTR pDEL             = TEXT("{{##DEL}}");

        lpszBuffer = MYALLOC0(nBufferSize * sizeof(TCHAR));
        LPTSTR lpszOutputStringCorrected = MYALLOC0(nBufferSize * sizeof(TCHAR));
        if (lpszOutputStringCorrected == NULL)
            return lpszDataLineMacro;

        LPTSTR lpszReplacement = NULL;
        LPTSTR lpszEscapeReplacement = NULL;
        size_t iDest = 0;
        for (size_t iSource = 0; iSource <= _tcslen(lpszDataLineMacro); iSource++) {
            if (lpszOutputStringCorrected + iDest != NULL) {
                if (_tcsncmp(lpszDataLineMacro + iSource, macroStart, _tcslen(macroStart)) == 0) {
                    if (_tcsncmp(lpszDataLineMacro + iSource, pROOTKEYNAME, _tcslen(pROOTKEYNAME)) == 0) {
                        pos = _tcschr(lpszText, _T('\\'));
                        if (pos != NULL)
                            _tcscpy(pos++, TEXT("\0"));
                        lpszReplacement = lpszText;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pROOTKEYNAME) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszDataLineMacro + iSource, pWHOLEKEYNAME, _tcslen(pWHOLEKEYNAME)) == 0) {
                        lpszReplacement = lpszText;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pWHOLEKEYNAME) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszDataLineMacro + iSource, pKEYNAME, _tcslen(pKEYNAME)) == 0) {
                        lpszReplacement = (pos != NULL ? pos : lpszEmpty);
                        lpszEscapeReplacement = ReplaceSpecialCharacters(lpszReplacement, TRUE);
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszEscapeReplacement, _tcslen(lpszEscapeReplacement));
                        iSource = iSource + _tcslen(pKEYNAME) - 1;
                        iDest = iDest + _tcslen(lpszEscapeReplacement);
                        if (lpszEscapeReplacement != lpszReplacement)
                            MYFREE(lpszEscapeReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszDataLineMacro + iSource, pVALNAME, _tcslen(pVALNAME)) == 0) {
                        lpszReplacement = (((LPVALUECONTENT)lpContent)->lpszValueName != NULL ? ((LPVALUECONTENT)lpContent)->lpszValueName : lpszEmpty);
                        lpszReplacement = (((_tcscmp(lpszReplacement, lpszEmpty) == 0) && (lpszRegDefaultVal != NULL)) ? lpszRegDefaultVal : lpszReplacement);
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pVALNAME) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszDataLineMacro + iSource, pVALTYPE, _tcslen(pVALTYPE)) == 0) {
                        switch (((LPVALUECONTENT)lpContent)->nTypeCode) {
                        case REG_NONE:
                            _tcscpy(lpszBuffer, lpszNameREG_NONE);
                            break;
                        case REG_SZ:
                            _tcscpy(lpszBuffer, lpszNameREG_SZ);
                            break;
                        case REG_EXPAND_SZ:
                            _tcscpy(lpszBuffer, lpszNameREG_EXPAND_SZ);
                            break;
                        case REG_BINARY:
                            _tcscpy(lpszBuffer, lpszNameREG_BINARY);
                            break;
                        case REG_DWORD:
//                        case REG_DWORD_LITTLE_ENDIAN:
                            _tcscpy(lpszBuffer, lpszNameREG_DWORD_LITTLE_ENDIAN);
                            break;
                        case REG_DWORD_BIG_ENDIAN:
                            _tcscpy(lpszBuffer, lpszNameREG_DWORD_BIG_ENDIAN);
                            break;
                        case REG_MULTI_SZ:
                            _tcscpy(lpszBuffer, lpszNameREG_MULTI_SZ);
                            break;
//                        case REG_QWORD_LITTLE_ENDIAN:
                        case REG_QWORD:
                            _tcscpy(lpszBuffer, lpszNameREG_QWORD_LITTLE_ENDIAN);
                            break;
                        }
                        lpszReplacement = lpszBuffer;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pVALTYPE) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszDataLineMacro + iSource, pDIRNAME, _tcslen(pDIRNAME)) == 0) {
                        lpszReplacement = GetWholeFileName(lpContent, 0, pOutputFileDescription);
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pDIRNAME) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                        MYFREE(lpszReplacement);
                    }
                    else if (_tcsncmp(lpszDataLineMacro + iSource, pDIROFFILENAME, _tcslen(pDIROFFILENAME)) == 0) {
                        lpszReplacement = GetWholeFileName(lpContent, 0, pOutputFileDescription);
                        pos = _tcsrchr(lpszReplacement, _T('\\'));
                        if (pos != NULL)
                            _tcscpy(pos, TEXT("\0"));
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pDIROFFILENAME) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                        MYFREE(lpszReplacement);
                    }
                    else if (_tcsncmp(lpszDataLineMacro + iSource, pFILENAME, _tcslen(pFILENAME)) == 0) {
                        lpszReplacement = GetWholeFileName(lpContent, 0, pOutputFileDescription);
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pFILENAME) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                        MYFREE(lpszReplacement);
                    }
                    else if (_tcsncmp(lpszDataLineMacro + iSource, pBLANK, _tcslen(pBLANK)) == 0) {
                        lpszReplacement = TEXT(" ");
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pBLANK) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszDataLineMacro + iSource, pBS, _tcslen(pBS)) == 0) {
                        iDest = iDest--;
                        lpszReplacement = TEXT("\0");
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, 1);
                        iSource = iSource + _tcslen(pBS) - 1;
                        i--;
                    }
                    else if (_tcsncmp(lpszDataLineMacro + iSource, pDEL, _tcslen(pDEL)) == 0) {
                        iSource = iSource + _tcslen(pDEL);
                        i--;
                    }
                    else if (_tcsncmp(lpszDataLineMacro + iSource, pLINEFEED, _tcslen(pLINEFEED)) == 0) {
                        lpszReplacement = TEXT("\r\n");
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pLINEFEED) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else {
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszDataLineMacro + iSource, 1);
                        iDest++;
                    }
                }
                else {
                    _tcsncpy(lpszOutputStringCorrected + iDest, lpszDataLineMacro + iSource, 1);
                    iDest++;
                }
                if (i == 0) {
                    _tcsncpy(lpszOutputStringCorrected + iDest, lpszDataLineMacro + ++iSource, _tcslen(lpszDataLineMacro) - iSource);

                    break;
                }
            }
        }
        if ((lpszText != NULL) || (lpContent != NULL))
            lpszOutputStringCorrected = ReplaceDataMacros(lpszOutputStringCorrected, NULL, NULL, FALSE, pOutputFileDescription);
        else
            MYFREE(lpszDataLineMacro);
        
        MYFREE(lpszBuffer);

        return lpszOutputStringCorrected;
    }
}

// ----------------------------------------------------------------------
// Several routines to write to an output file
// ----------------------------------------------------------------------
LPTSTR BuildOutputString(LPTSTR lpszText, LPVOID lpContent, int nActionType, BOOL bSuppressKey, LPOUTPUTFILEDESCRIPTION pOutputFileDescription)
{
    DWORD cchDataline = 0;
    DWORD nSize = MAX_PATH;
    LPTSTR lpszSpecRule = NULL;
    LPTSTR lpszSpecRuleLine = NULL;
    
    LPTSTR lpszOutputString = NULL;
    if (pOutputFileDescription->bDeInstaller) {
        if (nActionType == VALADD) 
            nActionType = VALDEL;
        else if (nActionType == VALDEL) 
            nActionType = VALADD;
        else if (nActionType == KEYADD) 
            nActionType = KEYDEL;
        else if (nActionType == KEYDEL) 
            nActionType = KEYADD;
        else if (nActionType == FILEADD) 
            nActionType = FILEDEL;
        else if (nActionType == FILEDEL) 
            nActionType = FILEADD;
        else if (nActionType == DIRADD) 
            nActionType = DIRDEL;
        else if (nActionType == DIRDEL) 
            nActionType = DIRADD;
    }
    if ((nActionType == VALADD) || (VALMODI == nActionType)) {
        lpszSpecRule = MYALLOC0(2048);
        lpszSpecRuleLine = MYALLOC0(2048);
        _tcscpy(lpszSpecRule, (nActionType == VALADD ? TEXT("VALADD") : TEXT("VALCHANGE")));
        _tcscat(lpszSpecRule, (pOutputFileDescription->fNoVals ? TEXT("Short") : TEXT("Normal")));
        switch (((LPVALUECONTENT)lpContent)->nTypeCode) {
        case REG_NONE:
            _tcscat(lpszSpecRule, TEXT("NONE"));
            break;
        case REG_SZ:
            _tcscat(lpszSpecRule, TEXT("SZ"));
            break;
        case REG_EXPAND_SZ:
            _tcscat(lpszSpecRule, TEXT("EXPANDSZ"));
            break;
        case REG_BINARY:
            _tcscat(lpszSpecRule, TEXT("BINARY"));
            break;
        case REG_DWORD:
            _tcscat(lpszSpecRule, TEXT("DWORD"));
            break;
        case REG_DWORD_BIG_ENDIAN:
            _tcscat(lpszSpecRule, TEXT("DWORD"));
            break;
        case REG_MULTI_SZ:
            _tcscat(lpszSpecRule, TEXT("MULTISZ"));
            break;
        case REG_QWORD:
            _tcscat(lpszSpecRule, TEXT("QWORD"));
            break;
        }
        if (lpszMacroFileName != NULL) {
            GetModuleFileNameW(NULL, lpszMacroFileName, MAX_PATH);
            PathRemoveFileSpec(lpszMacroFileName);
            _tcscat(lpszMacroFileName, TEXT("\\Templates\\"));
            _tcscat(lpszMacroFileName, pOutputFileDescription->lpszFormat);
            _tcscat(lpszMacroFileName, TEXT(".tpl"));

            cchDataline = GetPrivateProfileString(TEXT("$$DATALINES$$"), lpszSpecRule, lpszEmpty, lpszSpecRuleLine, nSize, lpszMacroFileName);
        }
    }
    if (nActionType == VALADD) {
        if (cchDataline != 0)
            lpszOutputString = ReplaceDataMacros(lpszSpecRuleLine, lpszText, lpContent, bSuppressKey, pOutputFileDescription);
        else
            lpszOutputString = (pOutputFileDescription->fNoVals ? ReplaceDataMacros(lpszVALADDShort, lpszText, lpContent, bSuppressKey, pOutputFileDescription) : ReplaceDataMacros(lpszVALADDNormal, lpszText, lpContent, bSuppressKey, pOutputFileDescription));
        
        MYFREE(lpszSpecRule);
        MYFREE(lpszSpecRuleLine);
    }
    else if (VALMODI == nActionType) {
        if (cchDataline != 0)
            lpszOutputString = ReplaceDataMacros(lpszSpecRuleLine, lpszText, lpContent, bSuppressKey, pOutputFileDescription);
        else
            lpszOutputString = (pOutputFileDescription->fNoVals ? ReplaceDataMacros(lpszVALCHANGEShort, lpszText, lpContent, bSuppressKey, pOutputFileDescription) : ReplaceDataMacros(lpszVALCHANGENormal, lpszText, lpContent, bSuppressKey, pOutputFileDescription));

        MYFREE(lpszSpecRule);
        MYFREE(lpszSpecRuleLine);
    }
    else if (VALDEL == nActionType) {
        lpszOutputString = (pOutputFileDescription->fNoVals ? ReplaceDataMacros(lpszVALDELETEShort, lpszText, lpContent, bSuppressKey, pOutputFileDescription) : ReplaceDataMacros(lpszVALDELETENormal, lpszText, lpContent, bSuppressKey, pOutputFileDescription));
    }
    else if (KEYADD == nActionType) {
        lpszOutputString = ReplaceDataMacros(lpszKEYADDNormal, lpszText, lpContent, FALSE, pOutputFileDescription);
    }
    else if (KEYDEL == nActionType) {
        lpszOutputString = ReplaceDataMacros(lpszKEYDELETENormal, lpszText, lpContent, FALSE, pOutputFileDescription);
    }
    else if (FILEADD == nActionType) {
        lpszOutputString = ReplaceDataMacros(lpszFILEADDNormal, lpszText, lpContent, FALSE, pOutputFileDescription);
    }
    else if (FILEMODI == nActionType) {
        lpszOutputString = ReplaceDataMacros(lpszFILECHANGENormal, lpszText, lpContent, FALSE, pOutputFileDescription);
    }
    else if (FILEDEL == nActionType) {
        lpszOutputString = (fDeleteReadOnly ? ReplaceDataMacros(lpszFILEDELETEDeleteReadOnly, lpszText, lpContent, FALSE, pOutputFileDescription) : ReplaceDataMacros(lpszFILEDELETENormal, lpszText, lpContent, FALSE, pOutputFileDescription));
    }
    else if (DIRADD == nActionType) {
        lpszOutputString = ReplaceDataMacros(lpszDIRADDNormal, lpszText, lpContent, FALSE, pOutputFileDescription);
    }
    else if (DIRMODI == nActionType) {
        lpszOutputString = ReplaceDataMacros(lpszDIRCHANGENormal, lpszText, lpContent, FALSE, pOutputFileDescription);
    }
    else if (DIRDEL == nActionType) {
        lpszOutputString = (fDeleteDirNotEmpty ? ReplaceDataMacros(lpszDIRDELETEDeleteRecurse, lpszText, lpContent, FALSE, pOutputFileDescription) : ReplaceDataMacros(lpszDIRDELETENormal, lpszText, lpContent, FALSE, pOutputFileDescription));
    }

    //    EscapeSpecialCharacters(lpszOutputString, _T('"'), TEXT("\""));

    return lpszOutputString;
}

LPTSTR ReplaceHeaderMacros(LPTSTR lpszText, LPOUTPUTFILEDESCRIPTION pOutputFileDescription)
{
    size_t i = 0;
    LPTSTR pos = NULL;
    LPTSTR macroStart = TEXT("{{##");
    LPTSTR lpszBuffer = NULL;
    DWORD  nBufferSize = _MACROLINESIZE_;

    if (lpszText != NULL)
        pos = _tcsstr(lpszText, macroStart);
    while (pos) {
        i++;
        pos = _tcsstr(++pos, macroStart);
    }

    if (i == 0) 
        return lpszText;
    else
    {
        LPTSTR pCOMMENT         = TEXT("{{##COMMENT}}");
        LPTSTR pBLANK           = TEXT("{{##BLANK}}");
        LPTSTR pLINEFEED        = TEXT("{{##LINEFEED}}");
        LPTSTR pLEFTBRACKET     = TEXT("{{##LEFTBRACKET}}");
        LPTSTR pTITLE           = TEXT("{{##TITLE}}");
        LPTSTR pSCRIPTFILE      = TEXT("{{##SCRIPTFILE}}");
        LPTSTR pSCRIPTNAME      = TEXT("{{##SCRIPTNAME}}");
        LPTSTR pOUTDIR          = TEXT("{{##OUTDIR}}");
        LPTSTR pOUTFILE         = TEXT("{{##OUTFILE}}");
        LPTSTR pVERSION         = TEXT("{{##VERSION}}");
        LPTSTR pDATE1           = TEXT("{{##DATE1}}");
        LPTSTR pDATE2           = TEXT("{{##DATE2}}");
        LPTSTR pCOMPUTER1       = TEXT("{{##COMPUTER1}}");
        LPTSTR pCOMPUTER2       = TEXT("{{##COMPUTER2}}");
        LPTSTR pUSER1           = TEXT("{{##USER1}}");
        LPTSTR pUSER2           = TEXT("{{##USER1}}");
        LPTSTR pCOMMENTTEXT     = TEXT("{{##COMMENTTEXT}}");
        LPTSTR pPARTCOUNTTEXT   = TEXT("{{##PARTCOUNTTEXT}}");
        LPTSTR pPARTCOUNT       = TEXT("{{##PARTCOUNT}}");
        LPTSTR pDELETERECURSE   = TEXT("{{##DELETERECURSE}}");
        LPTSTR pDELETEREADONLY  = TEXT("{{##DELETEREADONLY}}");

        lpszBuffer = MYALLOC0(nBufferSize * sizeof(TCHAR));
        LPTSTR lpszOutputStringCorrected = MYALLOC0(2048 * sizeof(TCHAR));
        if (lpszOutputStringCorrected == NULL)
            return lpszText;

        TCHAR szCount[17];
        LPTSTR lpszReplacement = NULL;

        size_t iDest = 0;
        for (size_t iSource = 0; iSource <= _tcslen(lpszText); iSource++) {
            if (lpszOutputStringCorrected + iDest != NULL) {
                if (_tcsncmp(lpszText + iSource, macroStart, _tcslen(macroStart)) == 0) {
                    if (_tcsncmp(lpszText + iSource, pCOMMENT, _tcslen(pCOMMENT)) == 0) {
                        _tcsncpy(lpszOutputStringCorrected + iDest, pOutputFileDescription->lpszComment, _tcslen(pOutputFileDescription->lpszComment));
                        iSource = iSource + _tcslen(pCOMMENT) - 1;
                        iDest = iDest + _tcslen(pOutputFileDescription->lpszComment);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pLEFTBRACKET, _tcslen(pLEFTBRACKET)) == 0) {
                        lpszReplacement = TEXT("[");
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pLEFTBRACKET) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pBLANK, _tcslen(pBLANK)) == 0) {
                        lpszReplacement = TEXT(" ");
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pBLANK) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pLINEFEED, _tcslen(pLINEFEED)) == 0) {
                        lpszReplacement = lpszEmpty;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pLINEFEED) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pTITLE, _tcslen(pTITLE)) == 0) {
                        lpszReplacement = lpszTitle;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pTITLE) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pSCRIPTFILE, _tcslen(pSCRIPTFILE)) == 0) {
                        lpszReplacement = pOutputFileDescription->lpszScriptFile;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pSCRIPTFILE) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pOUTFILE, _tcslen(pOUTFILE)) == 0) {
                        lpszReplacement = pOutputFileDescription->lpszOutFile;
                        if ((lpszReplacement != NULL) && (_tcslen(lpszReplacement) > 0)) {
                            _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                            iSource = iSource + _tcslen(pOUTFILE) - 1;
                            iDest = iDest + _tcslen(lpszReplacement);
                        }
                        else {
                            MYFREE(lpszOutputStringCorrected);
                            lpszOutputStringCorrected = NULL;
                            break;
                        }
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pOUTDIR, _tcslen(pOUTDIR)) == 0) {
                        lpszReplacement = pOutputFileDescription->lpszOutDir;
                        if ((lpszReplacement != NULL) && (_tcslen(lpszReplacement) > 0)) {
                            _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                            iSource = iSource + _tcslen(pOUTDIR) - 1;
                            iDest = iDest + _tcslen(lpszReplacement);
                        }
                        else {
                            MYFREE(lpszOutputStringCorrected);
                            lpszOutputStringCorrected = NULL;
                            break;
                        }
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pSCRIPTNAME, _tcslen(pSCRIPTNAME)) == 0) {
                        _tcscpy(lpszBuffer, lpszTitle);
                        _tcscat(lpszBuffer, (pOutputFileDescription->bDeInstaller ? TEXT("-uninstaller") : TEXT("-installer")));
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszBuffer, _tcslen(lpszBuffer));
                        iSource = iSource + _tcslen(pSCRIPTNAME) - 1;
                        iDest = iDest + _tcslen(lpszBuffer);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pVERSION, _tcslen(pVERSION)) == 0) {
                        lpszReplacement = lpszProgramName;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pVERSION) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pDATE1, _tcslen(pDATE1)) == 0) {
                        _sntprintf(lpszBuffer, nBufferSize, TEXT("%04d-%02d-%02d %02d:%02d:%02d\0"),
                            CompareResult.lpShot1->systemtime.wYear, CompareResult.lpShot1->systemtime.wMonth, CompareResult.lpShot1->systemtime.wDay,
                            CompareResult.lpShot1->systemtime.wHour, CompareResult.lpShot1->systemtime.wMinute, CompareResult.lpShot1->systemtime.wSecond);
                        lpszBuffer[nBufferSize - 1] = (TCHAR)'\0';
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszBuffer, _tcslen(lpszBuffer));
                        iSource = iSource + _tcslen(pDATE1) - 1;
                        iDest = iDest + _tcslen(lpszBuffer);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pDATE2, _tcslen(pDATE2)) == 0) {
                        _sntprintf(lpszBuffer, nBufferSize, TEXT("%04d-%02d-%02d %02d:%02d:%02d\0"),
                            CompareResult.lpShot2->systemtime.wYear, CompareResult.lpShot2->systemtime.wMonth, CompareResult.lpShot2->systemtime.wDay,
                            CompareResult.lpShot2->systemtime.wHour, CompareResult.lpShot2->systemtime.wMinute, CompareResult.lpShot2->systemtime.wSecond);
                        lpszBuffer[nBufferSize - 1] = (TCHAR)'\0';
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszBuffer, _tcslen(lpszBuffer));
                        iSource = iSource + _tcslen(pDATE2) - 1;
                        iDest = iDest + _tcslen(lpszBuffer);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pCOMPUTER1, _tcslen(pCOMPUTER1)) == 0) {
                        lpszReplacement = CompareResult.lpShot1->lpszComputerName;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pCOMPUTER1) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pCOMPUTER2, _tcslen(pCOMPUTER2)) == 0) {
                        lpszReplacement = CompareResult.lpShot2->lpszComputerName;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pCOMPUTER2) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pUSER1, _tcslen(pUSER1)) == 0) {
                        lpszReplacement = CompareResult.lpShot1->lpszUserName;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pUSER1) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pUSER2, _tcslen(pUSER2)) == 0) {
                        lpszReplacement = CompareResult.lpShot2->lpszUserName;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pUSER2) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pCOMMENTTEXT, _tcslen(pCOMMENTTEXT)) == 0) {
                        GetDlgItemText(hMainWnd, IDC_EDITCOMMENT, lpszBuffer, COMMENTLENGTH);  // length incl. NULL character
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszBuffer, _tcslen(lpszBuffer));
                        iSource = iSource + _tcslen(pCOMMENTTEXT) - 1;
                        iDest = iDest + _tcslen(lpszBuffer);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pPARTCOUNTTEXT, _tcslen(pPARTCOUNTTEXT)) == 0) {
                        lpszReplacement = pOutputFileDescription->lpszPartCountText;
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pPARTCOUNTTEXT) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pPARTCOUNT, _tcslen(pPARTCOUNT)) == 0) {
                        _sntprintf(szCount, 17, TEXT("%u\0"), pOutputFileDescription->nPartCount);
                        szCount[16] = (TCHAR)'\0';
                        _tcsncpy(lpszOutputStringCorrected + iDest, szCount, _tcslen(szCount));
                        iSource = iSource + _tcslen(pPARTCOUNT) - 1;
                        iDest = iDest + _tcslen(szCount);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pDELETERECURSE, _tcslen(pDELETERECURSE)) == 0) {
                        lpszReplacement = (fDeleteDirNotEmpty ? TEXT("1") : TEXT("0"));
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pDELETERECURSE) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else if (_tcsncmp(lpszText + iSource, pDELETEREADONLY, _tcslen(pDELETEREADONLY)) == 0) {
                        lpszReplacement = (fDeleteReadOnly ? TEXT("1") : TEXT("0"));
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszReplacement, _tcslen(lpszReplacement));
                        iSource = iSource + _tcslen(pDELETEREADONLY) - 1;
                        iDest = iDest + _tcslen(lpszReplacement);
                        i--;
                    }
                    else {
                        _tcsncpy(lpszOutputStringCorrected + iDest, lpszText + iSource, 1);
                        iDest++;
                    }
                }
                else {
                    _tcsncpy(lpszOutputStringCorrected + iDest, lpszText + iSource, 1);
                    iDest++;
                }
                if (i == 0) {
                    _tcsncpy(lpszOutputStringCorrected + iDest, lpszText + ++iSource, _tcslen(lpszText) - iSource);

                    break;
                }
            }
        }
        MYFREE(lpszBuffer);
        
        return lpszOutputStringCorrected;
    }
}

// ----------------------------------------------------------------------
VOID WriteTableHead(HANDLE hFile, LPTSTR lpszText, DWORD nCount, LPOUTPUTFILEDESCRIPTION pOutputFileDescription)
{
    pOutputFileDescription->lpszPartCountText = lpszText;
    pOutputFileDescription->nPartCount = nCount;
    if (!WritePartTemplate(hFile, pOutputFileDescription->lpszFormat, TEXT("$$TABLEHEAD$$"), pOutputFileDescription))
        WritePartTemplate(hFile, TEXT("COMMON"), TEXT("$$TABLEHEAD$$"), pOutputFileDescription);
}

// ----------------------------------------------------------------------
// Several routines to write to an output file
// ----------------------------------------------------------------------
LPTSTR ReplaceSpecialCharacters(LPTSTR lpszText, BOOL bNames)
{
    LPTSTR pos = NULL;
    int iNewStringCounter = 0;

    size_t i = 0;
    LPTSTR a[3][2] = {
        {TEXT("\""), TEXT("\\")},
        {TEXT("{"), TEXT("{")},
        {NULL, NULL}
    };

    for (int count = 0; count < 3; count++) {
        if (a[count][0] == NULL)
            break;

        if (lpszText != NULL)
            pos = _tcsstr(lpszText, a[count][0]);
        while (pos) {
            i++;
            pos = _tcsstr(++pos, a[count][0]);
        }

        if (i > 0) {
            LPTSTR lpszOutputStringCorrected = MYALLOC0((_tcslen(lpszText) + i * _tcslen(a[count][1]) + 1) * sizeof(TCHAR));
            iNewStringCounter++;

            i = 0;
            for (int j = 0; j <= _tcslen(lpszText); j++) {
                if (lpszOutputStringCorrected + j + i != NULL) {
                    if (_tcsncmp(lpszText + j, a[count][0], _tcslen(a[count][0])) == 0) {
                        _tcsncpy(lpszOutputStringCorrected + j + i, a[count][1], _tcslen(a[count][1]));
                        i = i + _tcslen(a[count][1]);
                    }
                    if (lpszOutputStringCorrected + j + i != NULL)
                        _tcsncpy(lpszOutputStringCorrected + j + i, lpszText + j, 1);
                }
            }
            if (iNewStringCounter > 1)
                MYFREE(lpszText);
            lpszText = lpszOutputStringCorrected;
        }
    }

    return lpszText;
}

