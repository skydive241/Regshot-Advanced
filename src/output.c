/*
    Copyright 2021-     Patrick (skydive241@gmx.de)
    Copyright 2011-2015 Regshot Team
    Copyright 1999-2003,2007,2011 TiANWEi
    Copyright 2007 Belogorokhov Youri

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

// Some strings used to write to HTML or TEXT file
LPTSTR lpszCRLF = TEXT("\r\n");  // {0x0d,0x0a,0x00};
LPTSTR lpszHTML_BR = TEXT("<br>\r\n");
LPTSTR lpszHTMLBegin = TEXT("<html>\r\n");
LPTSTR lpszHTMLHeadBegin = TEXT("<head>\r\n");
LPTSTR lpszHTML_CType =
#ifdef _UNICODE
TEXT("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-16\">\r\n");
#else
TEXT("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\">\r\n");
#endif
LPTSTR lpszHTMLHeadEnd = TEXT("</head>\r\n");
LPTSTR lpszHTMLTd1Begin = TEXT("<TR><TD BGCOLOR=\"#669999\" ALIGN=\"LEFT\"><FONT COLOR=\"WHITE\"><B>");
LPTSTR lpszHTMLTd1End = TEXT("</B></FONT></TD></TR>\r\n");
LPTSTR lpszHTMLTd2Begin = TEXT("<TR><TD NOWRAP><FONT COLOR=\"BLACK\">");
LPTSTR lpszHTMLTd2End = TEXT("</FONT></TD></TR>\r\n");
// color idea got from HANDLE(Youri) at wgapatcher.ru :) 1.8
LPTSTR lpszHTML_CSS = TEXT("<STYLE TYPE = \"text/css\">td{font-family:\"Tahoma\";font-size:9pt}\
tr{font-size:9pt}body{font-size:9pt}\
.a{background:#FFFFFF}.b{background:#E0F0E0}</STYLE>\r\n");  // 1.8.2 from e0e0e0 to e0f0e0 by Charles
LPTSTR lpszHTMLBodyBegin = TEXT("<BODY BGCOLOR=\"#FFFFFF\" TEXT=\"#000000\" LINK=\"#C8C8C8\">\r\n");
LPTSTR lpszHTMLTableEnd = TEXT("</TABLE>\r\n");
LPTSTR lpszHTMLSpan1 = TEXT("<SPAN CLASS=\"a\">");
LPTSTR lpszHTMLSpan2 = TEXT("<SPAN CLASS=\"b\">");
LPTSTR lpszHTMLSpanEnd = TEXT("</SPAN>");
LPTSTR lpszHTMLWebSiteBegin = TEXT("<FONT COLOR=\"#888888\">Created with <A HREF=\"http://sourceforge.net/projects/regshot/\">");
LPTSTR lpszHTMLWebSiteEnd = TEXT("</A></FONT><BR>\r\n");

LPTSTR lpszHTMLContent = 
#ifdef _UNICODE
TEXT("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\r\n");
#else
TEXT("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\">\r\n");
#endif
LPTSTR lpszHTMLTitle = TEXT("<title>%s</title>\r\n");
LPTSTR lpszHTMLCSSLink = TEXT("<link href=\"report.css\" type=\"text/css\" rel=\"stylesheet\">\r\n");
LPTSTR lpszHTMLScript = TEXT("<script><!--\r\n\
function MoveMenu()\r\n\
{\r\n\
	menu.style.posTop = document.body.scrollTop;\r\n\
}\r\n//--></script>\r\n");
LPTSTR lpszHTMLScript_1 = TEXT("<body onscroll=\"javascript:MoveMenu();\" onresize=\"javascript:MoveMenu();\" style=\"margin: 0\">\r\n");
LPTSTR lpszHTMLMenu = TEXT("<div id=\"menu\" class=\"m\" style=\"position: absolute; top:0\"><nobr>[\r\n");
LPTSTR lpszHTMLDivEnd = TEXT("</div>\r\n");
LPTSTR lpszHTMLClassm = TEXT("<div class=\"m\"><a name=\"a\">&nbsp;</a></div>\r\n");
LPTSTR lpszHTMLdivCenter = TEXT("<div align=\"center\">\r\n");
LPTSTR lpszHTMLTableInfo = TEXT("\t<table id=\"info\" cellspacing=\"1\">\r\n");
LPTSTR lpszHTMLtrClass = TEXT("<tr class=\"");
LPTSTR lpszHTMLtrEnd = TEXT("</tr>\r\n");
LPTSTR lpszHTMLtdClass = TEXT("<td class=\"");
LPTSTR lpszHTMLtdColspan = TEXT("<td colspan=\"");
LPTSTR lpszHTMLtdEnd = TEXT("</td>\r\n");
LPTSTR lpszHTMLtdAName = TEXT("<td><a name=\"b\"></a>&nbsp;</td>\r\n");

// ----------------------------------------------------------------------
// Several routines to write to an output file
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
VOID WritePart(HANDLE hFile, DWORD nActionType, LPCOMPRESULT lpStartCR, LPOUTPUTFILEDESCRIPTION pOutputFileDescription)
{
    size_t nCharsToWrite;
    size_t nCharsToGo;
    LPTSTR lpszResultTemp;
    LPCOMPRESULT lpCR;
    BOOL fColor;  // color flip-flop flag
    DWORD cbHTMLSpan1;
    DWORD cbHTMLSpan2;
    DWORD cbHTMLSpanEnd;
    DWORD cbHTML_BR;
    DWORD cbCRLF;
    size_t i;
    size_t iLinesWrittenOldPart = 0;
    DWORD nBuffersize = 2 * nOutMaxResultLines * sizeof(LPTSTR);
    LPTSTR* rgszResultStrings = MYALLOC0(nBuffersize);
    size_t iResultStringsMac;

    cbHTMLSpan1 = 0;
    cbHTMLSpan2 = 0;
    cbHTMLSpanEnd = 0;
    cbHTML_BR = 0;
    cbCRLF = 0;

    LPTSTR lpszPreviousKeyName = MYALLOC(EXTDIRLEN * sizeof(TCHAR));
    LPTSTR lpszActualKeyName;
    BOOL bSuppressKey = FALSE;

    if (pOutputFileDescription->iOutputType == OUT_HTML) {
        WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTMLTd2Begin, (DWORD)(_tcslen(lpszHTMLTd2Begin) * sizeof(TCHAR)), &NBW, NULL);
        cbHTMLSpan1 = (DWORD)(_tcslen(lpszHTMLSpan1) * sizeof(TCHAR));
        cbHTMLSpan2 = (DWORD)(_tcslen(lpszHTMLSpan2) * sizeof(TCHAR));
        cbHTMLSpanEnd = (DWORD)(_tcslen(lpszHTMLSpanEnd) * sizeof(TCHAR));
        cbHTML_BR = (DWORD)(_tcslen(lpszHTML_BR) * sizeof(TCHAR));
    } else {
        cbCRLF = (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR));
    }

    fColor = FALSE;
    LPTSTR lpszFullName = NULL;
    _tcscpy(lpszPreviousKeyName, lpszEmpty);

    for (lpCR = lpStartCR; NULL != lpCR; lpCR = lpCR->lpNextCR) {
        iResultStringsMac = 0;
        iLinesWrittenOldPart = 0;
        
        if (((FILEMODI != nActionType) && (DIRMODI != nActionType) && (VALMODI != nActionType)) ||
            ((pOutputFileDescription->iOutputType == OUT_ISS_DEINSTALL) || (pOutputFileDescription->iOutputType == OUT_REG_DEINSTALL) || 
             (pOutputFileDescription->iOutputType == OUT_NSI_DEINSTALL) || 
             (pOutputFileDescription->iOutputType == OUT_HTML) || (pOutputFileDescription->iOutputType == OUT_TXT) || (pOutputFileDescription->iOutputType == OUT_UNL))) {
            if (NULL != lpCR->lpContentOld) {
                bSuppressKey = FALSE;
                if ((KEYDEL == nActionType) || (KEYADD == nActionType))
                    lpszFullName = GetWholeKeyName(lpCR->lpContentOld, FALSE);
                else if ((VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType)) {
                    lpszFullName = GetWholeValueName(lpCR->lpContentOld, FALSE);
                }
                if ((KEYDEL == nActionType) || (KEYADD == nActionType) || (VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType)) {
                    if (!IsInWhiteList(lpszFullName, (bRegWhitelistAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                    if (IsInSkipList(lpszFullName, pRegSkipList, (bRegSkipAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                    MYFREE(lpszFullName);
                }

                if (((VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType)) && 
                    pOutputFileDescription->fGroupRegKeys) {
                    lpszActualKeyName = GetWholeKeyName(((LPVALUECONTENT)(lpCR->lpContentOld))->lpFatherKC, FALSE);
                    if (0 == _tcscmp(lpszActualKeyName, lpszPreviousKeyName)) {
                        bSuppressKey = TRUE;
                    }
                    else
                        _tcscpy(lpszPreviousKeyName, lpszActualKeyName);
                    MYFREE(lpszActualKeyName);
                }

                if ((DIRDEL == nActionType) || (DIRADD == nActionType) || (DIRMODI == nActionType) || (FILEDEL == nActionType) || (FILEADD == nActionType) || (FILEMODI == nActionType)) {
                    lpszFullName = GetWholeFileName(lpCR->lpContentOld, 0, NULL);
                    if (IsInSkipList(lpszFullName, pFileSkipList, (bFileSkipAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                    MYFREE(lpszFullName);
                }

                    iResultStringsMac = ResultToString(rgszResultStrings, iResultStringsMac, iLinesWrittenOldPart, nActionType, lpCR->lpContentOld, FALSE, bSuppressKey, pOutputFileDescription);
                    iLinesWrittenOldPart = iResultStringsMac;
            }
        }

        if (((FILEMODI != nActionType) && (DIRMODI != nActionType) && (VALMODI != nActionType)) ||
            ((pOutputFileDescription->iOutputType == OUT_ISS_INSTALL) || (pOutputFileDescription->iOutputType == OUT_REG_INSTALL) || 
             (pOutputFileDescription->iOutputType == OUT_NSI_INSTALL) || 
             (pOutputFileDescription->iOutputType == OUT_HTML) || (pOutputFileDescription->iOutputType == OUT_TXT))) {
            if (NULL != lpCR->lpContentNew) {
                bSuppressKey = FALSE;
                if ((KEYDEL == nActionType) || (KEYADD == nActionType)) 
                    lpszFullName = GetWholeKeyName(lpCR->lpContentNew, FALSE);
                else if ((VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType))
                    lpszFullName = GetWholeValueName(lpCR->lpContentNew, FALSE);
                if ((KEYDEL == nActionType) || (KEYADD == nActionType) || (VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType)) {
                    if (!IsInWhiteList(lpszFullName, (bRegWhitelistAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                    if (IsInSkipList(lpszFullName, pRegSkipList, (bRegSkipAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                    MYFREE(lpszFullName);
                }
                
                if (((VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType)) && 
                    pOutputFileDescription->fGroupRegKeys) {
                    lpszActualKeyName = GetWholeKeyName(((LPVALUECONTENT)(lpCR->lpContentNew))->lpFatherKC, FALSE);
                    if (0 == _tcscmp(lpszActualKeyName, lpszPreviousKeyName)) {
                        bSuppressKey = TRUE;
                    }
                    else
                        _tcscpy(lpszPreviousKeyName, lpszActualKeyName);
                    MYFREE(lpszActualKeyName);
                }
            
                if ((DIRDEL == nActionType) || (DIRADD == nActionType) || (DIRMODI == nActionType) || (FILEDEL == nActionType) || (FILEADD == nActionType) || (FILEMODI == nActionType)) {
                    lpszFullName = GetWholeFileName(lpCR->lpContentNew, 0, NULL);
                    if (IsInSkipList(lpszFullName, pFileSkipList, (bFileSkipAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                    MYFREE(lpszFullName);
                }
            
                iResultStringsMac = ResultToString(rgszResultStrings, iResultStringsMac, iLinesWrittenOldPart, nActionType, lpCR->lpContentNew, TRUE, bSuppressKey, pOutputFileDescription);
            }
        }

        for (i = 0; i < iResultStringsMac; i++) {
            if (NULL == rgszResultStrings[i]) {
                continue;
            }

            if (pOutputFileDescription->iOutputType == OUT_HTML) {
                // 1.8.0: zebra/flip-flop colors
                if (!fColor) {
                    WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTMLSpan1, cbHTMLSpan1, &NBW, NULL);
                } else {
                    WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTMLSpan2, cbHTMLSpan2, &NBW, NULL);
                }
            }

            lpszResultTemp = rgszResultStrings[i];
            for (nCharsToGo = _tcslen(rgszResultStrings[i]); 0 < nCharsToGo;) {
                nCharsToWrite = nCharsToGo;
// TODO: Zeilenumbruch funktioniert nicht (deshalb nur noch bei HTML-Ausgabe aktiv)
                if (pOutputFileDescription->iOutputType == OUT_HTML) {
                    if (HTMLWRAPLENGTH < nCharsToWrite) { 
                        nCharsToWrite = HTMLWRAPLENGTH;
                    }
                }

                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszResultTemp, (DWORD)(nCharsToWrite * sizeof(TCHAR)), &NBW, NULL);
                lpszResultTemp += nCharsToWrite;
                nCharsToGo -= nCharsToWrite;

                if (0 == nCharsToGo) {
                    break;  // skip newline
                }

                if (pOutputFileDescription->iOutputType == OUT_HTML) {
                    WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
                } else {
                    WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
                }
            }
            MYFREE(rgszResultStrings[i]);

            if (pOutputFileDescription->iOutputType == OUT_HTML) {
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTMLSpanEnd, cbHTMLSpanEnd, &NBW, NULL);
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
            } else {
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
            }
        }

        // since 1.9.1: Separate objects in output with empty line
        if (pOutputFileDescription->fOutSeparateObjs) {
            if (pOutputFileDescription->iOutputType == OUT_HTML) {
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
            } else {
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
            }
        }

        // 1.8.0: zebra/flip-flop colors
        fColor = !fColor;

        // Increase count
        cCurrent++;

        // Update progress bar display
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            if (REFRESHINTERVAL < (nCurrentTime - nLastTime)) {
                UI_UpdateProgressBar();
            }
        }
    }
    MYFREE(rgszResultStrings);
    MYFREE(lpszPreviousKeyName);

    if (pOutputFileDescription->iOutputType == OUT_HTML) {
        WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTMLTd2End, (DWORD)(_tcslen(lpszHTMLTd2End) * sizeof(TCHAR)), &NBW, NULL);
    }
}

VOID WritePartNew(HANDLE hFile, DWORD nActionType, LPCOMPRESULTNEW lpStartCR, LPOUTPUTFILEDESCRIPTION pOutputFileDescription)
{
    size_t nCharsToWrite;
    size_t nCharsToGo;
    LPTSTR lpszResultTemp;
    LPCOMPRESULTNEW lpCR;
    BOOL fColor;  // color flip-flop flag
    DWORD cbHTMLSpan1;
    DWORD cbHTMLSpan2;
    DWORD cbHTMLSpanEnd;
    DWORD cbHTML_BR;
    DWORD cbCRLF;
    size_t i;
    size_t iLinesWrittenOldPart = 0;
    //    LPTSTR rgszResultStrings[MAX_RESULT_STRINGS];
    DWORD nBuffersize = 2 * nOutMaxResultLines * sizeof(LPTSTR);
    LPTSTR* rgszResultStrings = MYALLOC0(nBuffersize);
    size_t iResultStringsMac;

    LPTSTR lpszPreviousKeyName = MYALLOC(EXTDIRLEN * sizeof(TCHAR));
    LPTSTR lpszActualKeyName;
    BOOL bSuppressKey = FALSE;

    cbHTMLSpan1 = 0;
    cbHTMLSpan2 = 0;
    cbHTMLSpanEnd = 0;
    cbHTML_BR = 0;
    cbCRLF = 0;

    if (pOutputFileDescription->iOutputType == OUT_HTML) {
        WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTMLTd2Begin, (DWORD)(_tcslen(lpszHTMLTd2Begin) * sizeof(TCHAR)), &NBW, NULL);
        cbHTMLSpan1 = (DWORD)(_tcslen(lpszHTMLSpan1) * sizeof(TCHAR));
        cbHTMLSpan2 = (DWORD)(_tcslen(lpszHTMLSpan2) * sizeof(TCHAR));
        cbHTMLSpanEnd = (DWORD)(_tcslen(lpszHTMLSpanEnd) * sizeof(TCHAR));
        cbHTML_BR = (DWORD)(_tcslen(lpszHTML_BR) * sizeof(TCHAR));
    }
    else {
        cbCRLF = (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR));
    }

    fColor = FALSE;
    LPTSTR lpszFullName = NULL;
    // (DIRDEL == nActionType) || (DIRADD == nActionType) || (DIRMODI == nActionType)
    // (FILEDEL == nActionType) || (FILEADD == nActionType) || (FILEMODI == nActionType)
    for (lpCR = lpStartCR; NULL != lpCR; lpCR = lpCR->lpPrevCRNew) {
        if ((nActionType != ALLCHANGES) && (lpCR->nActionType != nActionType))
            continue;
        if (pOutputFileDescription->fOnlyNewEntries && ((DIRDEL == lpCR->nActionType) || (DIRMODI == lpCR->nActionType) ||
            (FILEDEL == lpCR->nActionType) || (FILEMODI == lpCR->nActionType) ||
            (VALDEL == lpCR->nActionType) || (VALMODI == lpCR->nActionType) || (KEYDEL == lpCR->nActionType)))
            continue;
        if (fNoDeletedEntries && ((DIRDEL == lpCR->nActionType) || (FILEDEL == lpCR->nActionType) || (VALDEL == lpCR->nActionType) || (KEYDEL == lpCR->nActionType)))
            continue;
        iResultStringsMac = 0;
        iLinesWrittenOldPart = 0;
        
        if (((FILEMODI != nActionType) && (DIRMODI != nActionType) && (VALMODI != nActionType)) ||
            ((pOutputFileDescription->iOutputType == OUT_ISS_DEINSTALL) || (pOutputFileDescription->iOutputType == OUT_REG_DEINSTALL) || (pOutputFileDescription->iOutputType == OUT_NSI_DEINSTALL) || 
             (pOutputFileDescription->iOutputType == OUT_HTML) || (pOutputFileDescription->iOutputType == OUT_TXT) || (pOutputFileDescription->iOutputType == OUT_UNL))) {
            if (NULL != lpCR->lpContentOld) {
                bSuppressKey = FALSE;
                if ((KEYDEL == lpCR->nActionType) || (KEYADD == lpCR->nActionType))
                    lpszFullName = GetWholeKeyName(lpCR->lpContentOld, FALSE);
                else if ((VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType) || (VALMODI == lpCR->nActionType)) {
                    lpszFullName = GetWholeValueName(lpCR->lpContentOld, FALSE);
                    if (((VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType)) || ((VALMODI == lpCR->nActionType) && (pOutputFileDescription->iOutputType == OUT_REG_DEINSTALL))) {
                        lpszActualKeyName = GetWholeKeyName(((LPVALUECONTENT)(lpCR->lpContentOld))->lpFatherKC, FALSE);
                        if (0 == _tcscmp(lpszActualKeyName, lpszPreviousKeyName)) {
                            bSuppressKey = TRUE;
                        }
                        else
                            _tcscpy(lpszPreviousKeyName, lpszActualKeyName);
                        MYFREE(lpszActualKeyName);
                    }
                }
                if ((KEYDEL == lpCR->nActionType) || (KEYADD == lpCR->nActionType) ||
                    (VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType) || (VALMODI == lpCR->nActionType)) {
                    if (!IsInWhiteList(lpszFullName, (bRegWhitelistAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                    if (IsInSkipList(lpszFullName, pRegSkipList, (bRegSkipAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                }
                if ((DIRDEL == lpCR->nActionType) || (DIRADD == lpCR->nActionType) ||
                    (DIRMODI == lpCR->nActionType) || (FILEDEL == lpCR->nActionType) ||
                    (FILEADD == lpCR->nActionType) || (FILEMODI == lpCR->nActionType)) {
                    lpszFullName = GetWholeFileName(lpCR->lpContentOld, 0, NULL);
                    if (IsInSkipList(lpszFullName, pFileSkipList, (bFileSkipAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                }
                MYFREE(lpszFullName);
                //if (((lpCR->nActionType != VALMODI) && (lpCR->nActionType != FILEMODI) && (lpCR->nActionType != DIRMODI)) ||
                //    ((iOutputType == OUT_TXT) || (iOutputType == OUT_HTML) || (iOutputType == OUT_UNL) ||
                //     (iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_REG_DEINSTALL))
                //    ) {
                    iResultStringsMac = ResultToString(rgszResultStrings, iResultStringsMac, iLinesWrittenOldPart, lpCR->nActionType, lpCR->lpContentOld, FALSE, bSuppressKey, pOutputFileDescription);
                    iLinesWrittenOldPart = iResultStringsMac;
                //}
            }
        }
        
        if (((FILEMODI != nActionType) && (DIRMODI != nActionType) && (VALMODI != nActionType)) ||
            ((pOutputFileDescription->iOutputType == OUT_ISS_INSTALL) || (pOutputFileDescription->iOutputType == OUT_REG_INSTALL) || (pOutputFileDescription->iOutputType == OUT_NSI_INSTALL) || 
             (pOutputFileDescription->iOutputType == OUT_HTML) || (pOutputFileDescription->iOutputType == OUT_TXT))) {
            if (NULL != lpCR->lpContentNew) {
                bSuppressKey = FALSE;
                if ((KEYDEL == lpCR->nActionType) || (KEYADD == lpCR->nActionType))
                    lpszFullName = GetWholeKeyName(lpCR->lpContentNew, FALSE);
                else if ((VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType) || (VALMODI == lpCR->nActionType)) {
                    lpszFullName = GetWholeValueName(lpCR->lpContentNew, FALSE);
                    if (((VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType)) || ((VALMODI == lpCR->nActionType) && (pOutputFileDescription->iOutputType == OUT_REG_INSTALL))) {
//                    if (iOutputType == OUT_REG_INSTALL) {
                        lpszActualKeyName = GetWholeKeyName(((LPVALUECONTENT)(lpCR->lpContentNew))->lpFatherKC, FALSE);
                        if (0 == _tcscmp(lpszActualKeyName, lpszPreviousKeyName)) {
                            bSuppressKey = TRUE;
                        }
                        else
                            _tcscpy(lpszPreviousKeyName, lpszActualKeyName);
                        MYFREE(lpszActualKeyName);
                    }
                }
                if ((KEYDEL == lpCR->nActionType) || (KEYADD == lpCR->nActionType) ||
                    (VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType) || (VALMODI == lpCR->nActionType)) {
                    if (!IsInWhiteList(lpszFullName, (bRegWhitelistAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                    if (IsInSkipList(lpszFullName, pRegSkipList, (bRegSkipAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                }
                if ((DIRDEL == lpCR->nActionType) || (DIRADD == lpCR->nActionType) || (DIRMODI == lpCR->nActionType) ||
                    (FILEDEL == lpCR->nActionType) || (FILEADD == lpCR->nActionType) || (FILEMODI == lpCR->nActionType)) {
                    lpszFullName = GetWholeFileName(lpCR->lpContentNew, 0, NULL);
                    if (IsInSkipList(lpszFullName, pFileSkipList, (bFileSkipAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                }
                MYFREE(lpszFullName);
                //if (((lpCR->nActionType != VALMODI) && (lpCR->nActionType != FILEMODI) && (lpCR->nActionType != DIRMODI)) ||
                //    ((iOutputType == OUT_TXT) || (iOutputType == OUT_HTML) ||
                //     (iOutputType == OUT_ISS_INSTALL) || (iOutputType == OUT_REG_INSTALL) || (iOutputType == OUT_INF_INSTALL))
                //    ) {
                    iResultStringsMac = ResultToString(rgszResultStrings, iResultStringsMac, iLinesWrittenOldPart, lpCR->nActionType, lpCR->lpContentNew, TRUE, bSuppressKey, pOutputFileDescription);
                //}
            }
        }

        for (i = 0; i < iResultStringsMac; i++) {
            if (NULL == rgszResultStrings[i]) {
                continue;
            }

            if (pOutputFileDescription->iOutputType == OUT_HTML) {
                // 1.8.0: zebra/flip-flop colors
                if (!fColor) {
                    WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTMLSpan1, cbHTMLSpan1, &NBW, NULL);
                }
                else {
                    WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTMLSpan2, cbHTMLSpan2, &NBW, NULL);
                }
            }

            lpszResultTemp = rgszResultStrings[i];
            for (nCharsToGo = _tcslen(rgszResultStrings[i]); 0 < nCharsToGo;) {
                nCharsToWrite = nCharsToGo;
                if (HTMLWRAPLENGTH < nCharsToWrite) {
                    nCharsToWrite = HTMLWRAPLENGTH;
                }

                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszResultTemp, (DWORD)(nCharsToWrite * sizeof(TCHAR)), &NBW, NULL);
                lpszResultTemp += nCharsToWrite;
                nCharsToGo -= nCharsToWrite;

                if (0 == nCharsToGo) {
                    break;  // skip newline
                }

                if (pOutputFileDescription->iOutputType == OUT_HTML) {
                    WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
                }
                else {
                    WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
                }
            }
            MYFREE(rgszResultStrings[i]);

            if (pOutputFileDescription->iOutputType == OUT_HTML) {
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTMLSpanEnd, cbHTMLSpanEnd, &NBW, NULL);
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
            }
            else {
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
            }
        }

        // since 1.9.1: Separate objects in output with empty line
        if (pOutputFileDescription->fOutSeparateObjs) {
            if (pOutputFileDescription->iOutputType == OUT_HTML) {
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
            }
            else {
                WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
            }
        }

        // 1.8.0: zebra/flip-flop colors
        fColor = !fColor;

        // Increase count
        cCurrent++;

        // Update progress bar display
        if (0 != cEnd) {
            nCurrentTime = GetTickCount64();
            if (REFRESHINTERVAL < (nCurrentTime - nLastTime)) {
                UI_UpdateProgressBar();
            }
        }
    }
    MYFREE(rgszResultStrings);
    MYFREE(lpszPreviousKeyName);

    if (pOutputFileDescription->iOutputType == OUT_HTML) {
        WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTMLTd2End, (DWORD)(_tcslen(lpszHTMLTd2End) * sizeof(TCHAR)), &NBW, NULL);
//        WriteFileCP(hFile, pOutputFileDescription->nCodePage, lpszHTMLTableEnd, (DWORD)(_tcslen(lpszHTMLTableEnd) * sizeof(TCHAR)), &NBW, NULL);
    }
}

// ----------------------------------------------------------------------
VOID WriteHTMLHeader(HANDLE hFile, int CodePage)
{
    LPTSTR lpszBuffer, lpszBuffer2;
    DWORD  nBufferSize = 2048+1;
    lpszBuffer = MYALLOC0(nBufferSize * sizeof(TCHAR));
    lpszBuffer2 = MYALLOC0(nBufferSize * sizeof(TCHAR));
    nBufferSize--;

    // HTML head
    if ((lpszBuffer != NULL) && (lpszBuffer2 != NULL)) {
        lpszBuffer[nBufferSize] = _T('\0');
        WriteFileCP(hFile, CodePage, lpszHTMLBegin, (DWORD)(_tcslen(lpszHTMLBegin) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, CodePage, lpszHTMLHeadBegin, (DWORD)(_tcslen(lpszHTMLHeadBegin) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, CodePage, lpszHTMLContent, (DWORD)(_tcslen(lpszHTMLContent) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, lpszHTMLTitle, lpszProgramName);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, CodePage, lpszHTMLCSSLink, (DWORD)(_tcslen(lpszHTMLCSSLink) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, CodePage, lpszHTMLScript, (DWORD)(_tcslen(lpszHTMLScript) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, CodePage, lpszHTMLHeadEnd, (DWORD)(_tcslen(lpszHTMLHeadEnd) * sizeof(TCHAR)), &NBW, NULL);

        WriteFileCP(hFile, CodePage, lpszHTMLScript_1, (DWORD)(_tcslen(lpszHTMLScript_1) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, CodePage, lpszHTMLMenu, (DWORD)(_tcslen(lpszHTMLMenu) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("a"), TEXT("\">"), TEXT("Summary report"), TEXT("</a>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("b"), TEXT("\">"), TEXT("Deleted keys"), TEXT("</a>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("c"), TEXT("\">"), TEXT("New keys"), TEXT("</a>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("d"), TEXT("\">"), TEXT("Deleted values"), TEXT("</a>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("e"), TEXT("\">"), TEXT("New values"), TEXT("</a>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("f"), TEXT("\">"), TEXT("Changed values"), TEXT("</a>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("g"), TEXT("\">"), TEXT("FileShot"), TEXT("</a>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("h"), TEXT("\">"), TEXT("To end of report"), TEXT("</a> ]</nobr>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, CodePage, lpszHTMLDivEnd, (DWORD)(_tcslen(lpszHTMLDivEnd) * sizeof(TCHAR)), &NBW, NULL);

        WriteFileCP(hFile, CodePage, lpszHTMLClassm, (DWORD)(_tcslen(lpszHTMLClassm) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("%s%s%s%s\0"), TEXT("<h2>"), TEXT("Report of the "), lpszProgramName, TEXT("</h2>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, CodePage, lpszHTMLdivCenter, (DWORD)(_tcslen(lpszHTMLdivCenter) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, CodePage, lpszHTMLTableInfo, (DWORD)(_tcslen(lpszHTMLTableInfo) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("h"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s%s%s\0"), lpszHTMLtdColspan, TEXT("3"), TEXT("\">"), TEXT("Summary report"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s\0"), TEXT("<td>&nbsp;</td>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\"><b>Shot&nbsp;1</b></td>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\"><b>Shot&nbsp;2</b></td>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextDateTime].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%04d-%02d-%02d %02d:%02d:%02d%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"),
            CompareResult.lpShot1->systemtime.wYear, CompareResult.lpShot1->systemtime.wMonth, CompareResult.lpShot1->systemtime.wDay,
            CompareResult.lpShot1->systemtime.wHour, CompareResult.lpShot1->systemtime.wMinute, CompareResult.lpShot1->systemtime.wSecond, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%04d-%02d-%02d %02d:%02d:%02d%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"),
            CompareResult.lpShot1->systemtime.wYear, CompareResult.lpShot1->systemtime.wMonth, CompareResult.lpShot2->systemtime.wDay,
            CompareResult.lpShot1->systemtime.wHour, CompareResult.lpShot1->systemtime.wMinute, CompareResult.lpShot2->systemtime.wSecond, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextComputer].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot1->lpszComputerName, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot2->lpszComputerName, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextUsername].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot1->lpszUserName, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot2->lpszUserName, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), TEXT("Shot type"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        // Shot-Time ist bislang nicht protokolliert
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), TEXT("Shot time"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextKey].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot1->stCounts.cKeys, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot2->stCounts.cKeys, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextValue].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot1->stCounts.cValues, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot2->stCounts.cValues, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextDir].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot1->stCounts.cDirs, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot2->stCounts.cDirs, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextFile].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot1->stCounts.cFiles, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot2->stCounts.cFiles, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextKeyDel].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcDeleted.cKeys, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextKeyAdd].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcAdded.cKeys, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextValDel].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcDeleted.cValues, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextValAdd].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcAdded.cValues, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextValModi].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcChanged.cValues, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextDirDel].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcDeleted.cDirs, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextDirAdd].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcAdded.cDirs, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextDirModi].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcModified.cDirs, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextFileDel].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcDeleted.cFiles, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextFileAdd].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcAdded.cFiles, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextFileModi].lpszText, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcModified.cFiles, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\"><b>"), asLangTexts[iszTextTotal].lpszText, TEXT("</b></td>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcModified.cAll, lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), TEXT("File to restoring registry"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);

        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s\0"), lpszHTMLtdClass, TEXT("c"), TEXT("\" colspan=\"3\"><b>Comment</b>:&nbsp;<span class=\"i\"></span></td>\r\n"));
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
        WriteFileCP(hFile, CodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

        WriteFileCP(hFile, CodePage, lpszHTMLTableEnd, (DWORD)(_tcslen(lpszHTMLDivEnd) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, CodePage, lpszHTMLDivEnd, (DWORD)(_tcslen(lpszHTMLDivEnd) * sizeof(TCHAR)), &NBW, NULL);
        
        MYFREE(lpszBuffer2);
        MYFREE(lpszBuffer);
    }
}

// ----------------------------------------------------------------------
