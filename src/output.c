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

// Some strings used to write to UNL file
LPTSTR lpszCommentCommon = TEXT("; ");
LPTSTR lpszCommentBAT = TEXT(":: ");

// Some strings used to write to HTML or TEXT file
LPTSTR lpszCRLF = TEXT("\r\n");  // {0x0d,0x0a,0x00};
LPTSTR lpszTextLine = TEXT("\r\n----------------------------------\r\n");
LPTSTR lpszTextLineUNL = TEXT("\r\n; --------------------------------------------------------------------------------------------------------------\r\n");
LPTSTR lpszTextLineBAT = TEXT("\r\n:: --------------------------------------------------------------------------------------------------------------\r\n");
LPTSTR lpszHTML_BR = TEXT("<br>\r\n");
LPTSTR lpszHTMLBegin = TEXT("<html>\r\n");
LPTSTR lpszHTMLEnd = TEXT("</html>\r\n");
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
LPTSTR lpszHTMLBodyEnd = TEXT("</BODY>\r\n");
LPTSTR lpszHTMLTableBegin = TEXT("<table id=\"%s\" cellspacing=\"1\">\r\n");
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
LPTSTR lpszHTMLtd = TEXT("<td>%s %i</td>\r\n");
LPTSTR lpszHTMLtdColspan = TEXT("<td colspan=\"");
LPTSTR lpszHTMLtdEnd = TEXT("</td>\r\n");
LPTSTR lpszHTMLtdAName = TEXT("<td><a name=\"b\"></a>&nbsp;</td>\r\n");
LPTSTR lpszHTMLtdANameC = TEXT("<td><a name=\"c\"></a>&nbsp;</td>\r\n");

// CSS-content (ASCII, NOT UNICODE!!!!!)
LPTSTR lpszHTML_CSS2 = TEXT("body, table  { background-color: #FFFFFF; font-family: Tahoma; font-size: 10pt }\r\n\
div.m        { background-color: #FFFFE0; width: 100%; text-align: center; border: 1px solid #808080; padding-top: 1px; padding-bottom: 3px }\r\n\
h2           { font-size: 11pt; text-align: center }\r\n\
td           { vertical-align: top; padding-left: 8px; padding-right: 8px; padding-top: 2px; padding-bottom: 2px }\r\n\
#info        { border: 1px solid #E0E0E0; background-color: #F8F8F4 }\r\n\
#info tr.a   { background-color: #FFFFFF }\r\n\
#info tr.b   { background-color: #F0F0EC }\r\n\
#info tr.h   { background-color: #DBE8F0; text-align: center; font-weight: bold }\r\n\
#info td.a   { text-align: left }\r\n\
#info td.b   { text-align: right }\r\n\
#info td.c   { text-align: justify }\r\n\
#info td.h   { text-align: center }\r\n\
#info span.i { font-style: italic; color: #800000 }\r\n\
#hive        { width: 100% }\r\n\
#hive tr.a   { background-color: #FFFFFF }\r\n\
#hive tr.b   { background-color: #F4F4F4 }\r\n\
#hive tr.c   { background-color: #FFFFFF; font-family: Courier New }\r\n\
#hive tr.d   { background-color: #F0F4F4; font-family: Courier New }\r\n\
#hive tr.e   { background-color: #DAEAD5; font-weight: bold }\r\n\
#hive tr.f   { background-color: #FFFFFF; font-family: Courier New }\r\n\
#hive tr.g   { background-color: #F4F8F4; font-family: Courier New }\r\n\
#hive tr.h   { background-color: #DBE8F0; font-weight: bold }\r\n\
#hive span.r { color: #FF0000 }\r\n\
#file        { width: 100% }\r\n\
#file tr.a   { background-color: #FFFFFF }\r\n\
#file tr.b   { background-color: #F4F8FC }\r\n\
#file tr.h   { background-color: #DBE8F0; font-weight: bold }\r\n\
.hash td     { font-family: Courier New; font-size: 9pt; padding-top: 0px; padding-bottom: 0px; text-align:right }\r\n\
a            { color: #0000FF }\r\n\
a:active     { color: #FF0000 }\r\n\
a:hover      { color: #FF0000 }\r\n\
a:link       { color: #0000FF }\r\n\
a:visited    { color: #800080 }\r\n");

LPTSTR lpszISSSectionMessages = TEXT("[Messages]");
LPTSTR lpszISSReadyLabel1 = TEXT("ReadyLabel1=[name].");
LPTSTR lpszISSReadyLabel2b = TEXT("ReadyLabel2b=Click (Un)install to continue.");
LPTSTR lpszISSButtonInstall = TEXT("ButtonInstall=(Un)install");

LPTSTR lpszNSISectionEnd = TEXT("SectionEnd");

LPTSTR lpszREGVersion4 = TEXT("REGEDIT4\r\n\r\n");
LPTSTR lpszREGVersion5 = TEXT("Windows Registry Editor Version 5.00\r\n\r\n");

// ----------------------------------------------------------------------
// Several routines to write to an output file
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
VOID WriteTableHead(HANDLE hFile, LPTSTR lpszText, DWORD nCount, int iOutputType)
{
    TCHAR szCount[17];

    _sntprintf(szCount, 17, TEXT("%u\0"), nCount);
    szCount[16] = (TCHAR)'\0';  // saftey NULL char

    if ((iOutputType == OUT_UNL) || (iOutputType == OUT_ISS_DEINSTALL)|| (iOutputType == OUT_ISS_INSTALL) || 
        (iOutputType == OUT_NSI_DEINSTALL) || (iOutputType == OUT_NSI_INSTALL) ||
        (iOutputType == OUT_REG_DEINSTALL)|| (iOutputType == OUT_REG_INSTALL)) {
        WriteFileCP(hFile, nCodePage, lpszTextLineUNL, (DWORD)(_tcslen(lpszTextLineUNL) * sizeof(TCHAR)), &NBW, NULL);
    } 
    else if (iOutputType == OUT_BAT) {
        WriteFileCP(hFile, nCodePage, lpszTextLineBAT, (DWORD)(_tcslen(lpszTextLineBAT) * sizeof(TCHAR)), &NBW, NULL);
    } 
    else if (iOutputType != OUT_HTML) {
        WriteFileCP(hFile, nCodePage, lpszTextLine, (DWORD)(_tcslen(lpszTextLine) * sizeof(TCHAR)), &NBW, NULL);
    }

    if ((iOutputType == OUT_UNL) || (iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_ISS_INSTALL) ||
        (iOutputType == OUT_NSI_DEINSTALL) || (iOutputType == OUT_NSI_INSTALL) ||
        (iOutputType == OUT_REG_DEINSTALL) || (iOutputType == OUT_REG_INSTALL)) {
        WriteFileCP(hFile, nCodePage, lpszCommentCommon, (DWORD)(_tcslen(lpszCommentCommon) * sizeof(TCHAR)), &NBW, NULL);
    }
    else if (iOutputType == OUT_BAT)
        WriteFileCP(hFile, nCodePage, lpszCommentBAT, (DWORD)(_tcslen(lpszCommentBAT) * sizeof(TCHAR)), &NBW, NULL);
    
    if (iOutputType != OUT_HTML) {
        WriteFileCP(hFile, nCodePage, lpszText, (DWORD)(_tcslen(lpszText) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, nCodePage, TEXT(" "), (DWORD)(1 * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, nCodePage, szCount, (DWORD)(_tcslen(szCount) * sizeof(TCHAR)), &NBW, NULL);
    }
    else 
    {
        WriteHTMLPartStart(hFile, lpszText, nCount);
    }

    if (iOutputType == OUT_HTML) {
//        WriteFileCP(hFile, nCodePage, lpszHTMLtrEnd, (DWORD)(_tcslen(lpszHTMLtrEnd) * sizeof(TCHAR)), &NBW, NULL);
//        WriteFileCP(hFile, nCodePage, lpszHTMLTableEnd, (DWORD)(_tcslen(lpszHTMLTableEnd) * sizeof(TCHAR)), &NBW, NULL);
    } else if ((iOutputType == OUT_UNL) || (iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_ISS_INSTALL) ||
        (iOutputType == OUT_NSI_DEINSTALL) || (iOutputType == OUT_NSI_INSTALL) ||
        (iOutputType == OUT_REG_DEINSTALL) || (iOutputType == OUT_REG_INSTALL)) {
        WriteFileCP(hFile, nCodePage, lpszTextLineUNL, (DWORD)(_tcslen(lpszTextLineUNL) * sizeof(TCHAR)), &NBW, NULL);
    }
    else if (iOutputType == OUT_BAT) {
        WriteFileCP(hFile, nCodePage, lpszTextLineBAT, (DWORD)(_tcslen(lpszTextLineBAT) * sizeof(TCHAR)), &NBW, NULL);
    }
    else {
        WriteFileCP(hFile, nCodePage, lpszTextLine, (DWORD)(_tcslen(lpszTextLine) * sizeof(TCHAR)), &NBW, NULL);
    }
}

// ----------------------------------------------------------------------
VOID WritePart(HANDLE hFile, DWORD nActionType, LPCOMPRESULT lpStartCR, int iOutputType)
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
    LPTSTR* rgszResultStrings = MYALLOC0(2 * nOutMaxResultLines * sizeof(LPTSTR));
    size_t iResultStringsMac;

    cbHTMLSpan1 = 0;
    cbHTMLSpan2 = 0;
    cbHTMLSpanEnd = 0;
    cbHTML_BR = 0;
    cbCRLF = 0;

    LPTSTR lpszPreviousKeyName = MYALLOC(EXTDIRLEN * sizeof(TCHAR));
    LPTSTR lpszActualKeyName;
    BOOL bSuppressKey = FALSE;

    if (iOutputType == OUT_HTML) {
//        WriteFileCP(hFile, nCodePage, lpszHTMLTableBegin, (DWORD)(_tcslen(lpszHTMLTableBegin) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, nCodePage, lpszHTMLTd2Begin, (DWORD)(_tcslen(lpszHTMLTd2Begin) * sizeof(TCHAR)), &NBW, NULL);
        cbHTMLSpan1 = (DWORD)(_tcslen(lpszHTMLSpan1) * sizeof(TCHAR));
        cbHTMLSpan2 = (DWORD)(_tcslen(lpszHTMLSpan2) * sizeof(TCHAR));
        cbHTMLSpanEnd = (DWORD)(_tcslen(lpszHTMLSpanEnd) * sizeof(TCHAR));
        cbHTML_BR = (DWORD)(_tcslen(lpszHTML_BR) * sizeof(TCHAR));
    } else {
        cbCRLF = (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR));
    }

    fColor = FALSE;
    LPTSTR lpszFullName = NULL;
    _tcscpy(lpszPreviousKeyName, _TEXT(""));

// (DIRDEL == nActionType) || (DIRADD == nActionType) || (DIRMODI == nActionType)
// (FILEDEL == nActionType) || (FILEADD == nActionType) || (FILEMODI == nActionType)
    for (lpCR = lpStartCR; NULL != lpCR; lpCR = lpCR->lpNextCR) {
        iResultStringsMac = 0;
        iLinesWrittenOldPart = 0;
        
        if (((FILEMODI != nActionType) && (DIRMODI != nActionType) && (VALMODI != nActionType)) ||
            ((iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_REG_DEINSTALL) || (iOutputType == OUT_NSI_DEINSTALL) || (iOutputType == OUT_INF_DEINSTALL) ||
            (iOutputType == OUT_HTML) || (iOutputType == OUT_TXT) || (iOutputType == OUT_UNL))) {
            if (NULL != lpCR->lpContentOld) {
                bSuppressKey = FALSE;
                if ((KEYDEL == nActionType) || (KEYADD == nActionType))
                    lpszFullName = GetWholeKeyName(lpCR->lpContentOld, FALSE);
                else if ((VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType)) {
//                    lpszActualKeyName = GetWholeKeyName(((LPVALUECONTENT)(lpCR->lpContentOld))->lpFatherKC, FALSE);
                    lpszFullName = GetWholeValueName(lpCR->lpContentOld, FALSE);
//                    lpszFullName = GetWholeKeyName(((LPVALUECONTENT)(lpCR->lpContentOld))->lpFatherKC, FALSE);
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
                    ((iOutputType == OUT_REG_DEINSTALL) || (iOutputType == OUT_REG_INSTALL))) {
                    lpszActualKeyName = GetWholeKeyName(((LPVALUECONTENT)(lpCR->lpContentOld))->lpFatherKC, FALSE);
                    if (0 == _tcscmp(lpszActualKeyName, lpszPreviousKeyName)) {
                        bSuppressKey = TRUE;
                    }
                    else
                        _tcscpy(lpszPreviousKeyName, lpszActualKeyName);
                    MYFREE(lpszActualKeyName);
                }

                if ((DIRDEL == nActionType) || (DIRADD == nActionType) || (DIRMODI == nActionType) || (FILEDEL == nActionType) || (FILEADD == nActionType) || (FILEMODI == nActionType)) {
                    lpszFullName = GetWholeFileName(lpCR->lpContentOld, 0, FALSE);
                    if (IsInSkipList(lpszFullName, pFileSkipList, (bFileSkipAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                    MYFREE(lpszFullName);
                }

                //if (((nActionType != VALMODI) && (nActionType != FILEMODI) && (nActionType != DIRMODI)) ||
                //    ((iOutputType == OUT_TXT) || (iOutputType == OUT_HTML) || (iOutputType == OUT_UNL) ||
                //     (iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_REG_DEINSTALL) || (iOutputType == OUT_INF_DEINSTALL))
                //    ) {
                    iResultStringsMac = ResultToString(rgszResultStrings, iResultStringsMac, iLinesWrittenOldPart, nActionType, lpCR->lpContentOld, FALSE, bSuppressKey, iOutputType);
                    iLinesWrittenOldPart = iResultStringsMac;
                //}
            }
        }

        if (((FILEMODI != nActionType) && (DIRMODI != nActionType) && (VALMODI != nActionType)) ||
            ((iOutputType == OUT_ISS_INSTALL) || (iOutputType == OUT_REG_INSTALL) || (iOutputType == OUT_NSI_INSTALL) || (iOutputType == OUT_INF_INSTALL) ||
            (iOutputType == OUT_HTML) || (iOutputType == OUT_TXT))) {
            if (NULL != lpCR->lpContentNew) {
//        if ((VALMODI == nActionType) && (NULL != lpCR->lpContentNew)) {
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
                
                if (((VALDEL == nActionType) || (VALADD == nActionType) || (VALMODI == nActionType)) && ((iOutputType == OUT_REG_DEINSTALL) || (iOutputType == OUT_REG_INSTALL))) {
                    lpszActualKeyName = GetWholeKeyName(((LPVALUECONTENT)(lpCR->lpContentNew))->lpFatherKC, FALSE);
                    if (0 == _tcscmp(lpszActualKeyName, lpszPreviousKeyName)) {
                        bSuppressKey = TRUE;
                    }
                    else
                        _tcscpy(lpszPreviousKeyName, lpszActualKeyName);
                    MYFREE(lpszActualKeyName);
                }
            
                if ((DIRDEL == nActionType) || (DIRADD == nActionType) || (DIRMODI == nActionType) || (FILEDEL == nActionType) || (FILEADD == nActionType) || (FILEMODI == nActionType)) {
                    lpszFullName = GetWholeFileName(lpCR->lpContentNew, 0, FALSE);
                    if (IsInSkipList(lpszFullName, pFileSkipList, (bFileSkipAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                    MYFREE(lpszFullName);
                }
            
            //if (((nActionType != VALMODI) && (nActionType != FILEMODI) && (nActionType != DIRMODI)) ||
            //    ((iOutputType == OUT_TXT) || (iOutputType == OUT_HTML) ||
            //    (iOutputType == OUT_ISS_INSTALL) || (iOutputType == OUT_REG_INSTALL) || (iOutputType == OUT_INF_INSTALL))
            //   ) {
                iResultStringsMac = ResultToString(rgszResultStrings, iResultStringsMac, iLinesWrittenOldPart, nActionType, lpCR->lpContentNew, TRUE, bSuppressKey, iOutputType);
            //}
            }
        }

        for (i = 0; i < iResultStringsMac; i++) {
            if (NULL == rgszResultStrings[i]) {
                continue;
            }

            if (iOutputType == OUT_HTML) {
                // 1.8.0: zebra/flip-flop colors
                if (!fColor) {
                    WriteFileCP(hFile, nCodePage, lpszHTMLSpan1, cbHTMLSpan1, &NBW, NULL);
                } else {
                    WriteFileCP(hFile, nCodePage, lpszHTMLSpan2, cbHTMLSpan2, &NBW, NULL);
                }
            }

            lpszResultTemp = rgszResultStrings[i];
            for (nCharsToGo = _tcslen(rgszResultStrings[i]); 0 < nCharsToGo;) {
                nCharsToWrite = nCharsToGo;
// TODO: Zeilenumbruch funktioniert nicht (deshalb nur noch bei HTML-Ausgabe aktiv)
                if (iOutputType == OUT_HTML) {
                    if (HTMLWRAPLENGTH < nCharsToWrite) { 
                        nCharsToWrite = HTMLWRAPLENGTH;
                    }
                }

                WriteFileCP(hFile, nCodePage, lpszResultTemp, (DWORD)(nCharsToWrite * sizeof(TCHAR)), &NBW, NULL);
                lpszResultTemp += nCharsToWrite;
                nCharsToGo -= nCharsToWrite;

                if (0 == nCharsToGo) {
                    break;  // skip newline
                }

                if (iOutputType == OUT_HTML) {
                    WriteFileCP(hFile, nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
                } else {
                    WriteFileCP(hFile, nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
                }
            }
            MYFREE(rgszResultStrings[i]);

            if (iOutputType == OUT_HTML) {
                WriteFileCP(hFile, nCodePage, lpszHTMLSpanEnd, cbHTMLSpanEnd, &NBW, NULL);
                WriteFileCP(hFile, nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
            } else {
                WriteFileCP(hFile, nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
            }
        }

        // since 1.9.1: Separate objects in output with empty line
        if (fOutSeparateObjs) {
            if (iOutputType == OUT_HTML) {
                WriteFileCP(hFile, nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
            } else {
                WriteFileCP(hFile, nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
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

    if (iOutputType == OUT_HTML) {
        WriteFileCP(hFile, nCodePage, lpszHTMLTd2End, (DWORD)(_tcslen(lpszHTMLTd2End) * sizeof(TCHAR)), &NBW, NULL);
//        WriteFileCP(hFile, nCodePage, lpszHTMLTableEnd, (DWORD)(_tcslen(lpszHTMLTableEnd) * sizeof(TCHAR)), &NBW, NULL);
        WriteHTMLPartEnd(hFile);
    }
}

VOID WritePartNew(HANDLE hFile, DWORD nActionType, LPCOMPRESULTNEW lpStartCR, int iOutputType)
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
    LPTSTR* rgszResultStrings = MYALLOC0(2 * nOutMaxResultLines * sizeof(LPTSTR));
    size_t iResultStringsMac;

    LPTSTR lpszPreviousKeyName = MYALLOC(EXTDIRLEN * sizeof(TCHAR));
    LPTSTR lpszActualKeyName;
    BOOL bSuppressKey = FALSE;

    cbHTMLSpan1 = 0;
    cbHTMLSpan2 = 0;
    cbHTMLSpanEnd = 0;
    cbHTML_BR = 0;
    cbCRLF = 0;

    if (iOutputType == OUT_HTML) {
//        WriteFileCP(hFile, nCodePage, lpszHTMLTableBegin, (DWORD)(_tcslen(lpszHTMLTableBegin) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, nCodePage, lpszHTMLTd2Begin, (DWORD)(_tcslen(lpszHTMLTd2Begin) * sizeof(TCHAR)), &NBW, NULL);
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
        if (fOnlyNewEntries && ((DIRDEL == lpCR->nActionType) || (DIRMODI == lpCR->nActionType) || 
            (FILEDEL == lpCR->nActionType) || (FILEMODI == lpCR->nActionType) ||
            (VALDEL == lpCR->nActionType) || (VALMODI == lpCR->nActionType) || (KEYDEL == lpCR->nActionType)))
            continue;
        iResultStringsMac = 0;
        iLinesWrittenOldPart = 0;
        
        if (((FILEMODI != nActionType) && (DIRMODI != nActionType) && (VALMODI != nActionType)) ||
            ((iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_REG_DEINSTALL) || (iOutputType == OUT_NSI_DEINSTALL) || (iOutputType == OUT_INF_DEINSTALL) ||
             (iOutputType == OUT_HTML) || (iOutputType == OUT_TXT) || (iOutputType == OUT_UNL))) {
            if (NULL != lpCR->lpContentOld) {
                bSuppressKey = FALSE;
                if ((KEYDEL == lpCR->nActionType) || (KEYADD == lpCR->nActionType))
                    lpszFullName = GetWholeKeyName(lpCR->lpContentOld, FALSE);
                else if ((VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType) || (VALMODI == lpCR->nActionType)) {
                    lpszFullName = GetWholeValueName(lpCR->lpContentOld, FALSE);
                    if (((VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType)) || ((VALMODI == lpCR->nActionType) && (iOutputType == OUT_REG_DEINSTALL))) {
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
                    lpszFullName = GetWholeFileName(lpCR->lpContentOld, 0, FALSE);
                    if (IsInSkipList(lpszFullName, pFileSkipList, (bFileSkipAdded ? TRUE : FALSE))) {
                        MYFREE(lpszFullName);
                        continue;
                    }
                }
                MYFREE(lpszFullName);
                //if (((lpCR->nActionType != VALMODI) && (lpCR->nActionType != FILEMODI) && (lpCR->nActionType != DIRMODI)) ||
                //    ((iOutputType == OUT_TXT) || (iOutputType == OUT_HTML) || (iOutputType == OUT_UNL) ||
                //     (iOutputType == OUT_ISS_DEINSTALL) || (iOutputType == OUT_REG_DEINSTALL) || (iOutputType == OUT_INF_DEINSTALL))
                //    ) {
                    iResultStringsMac = ResultToString(rgszResultStrings, iResultStringsMac, iLinesWrittenOldPart, lpCR->nActionType, lpCR->lpContentOld, FALSE, bSuppressKey, iOutputType);
                    iLinesWrittenOldPart = iResultStringsMac;
                //}
            }
        }
        
        if (((FILEMODI != nActionType) && (DIRMODI != nActionType) && (VALMODI != nActionType)) ||
            ((iOutputType == OUT_ISS_INSTALL) || (iOutputType == OUT_REG_INSTALL) || (iOutputType == OUT_NSI_INSTALL) || (iOutputType == OUT_INF_INSTALL) ||
             (iOutputType == OUT_HTML) || (iOutputType == OUT_TXT))) {
            if (NULL != lpCR->lpContentNew) {
                bSuppressKey = FALSE;
                if ((KEYDEL == lpCR->nActionType) || (KEYADD == lpCR->nActionType))
                    lpszFullName = GetWholeKeyName(lpCR->lpContentNew, FALSE);
                else if ((VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType) || (VALMODI == lpCR->nActionType)) {
                    lpszFullName = GetWholeValueName(lpCR->lpContentNew, FALSE);
                    if (((VALDEL == lpCR->nActionType) || (VALADD == lpCR->nActionType)) || ((VALMODI == lpCR->nActionType) && (iOutputType == OUT_REG_INSTALL))) {
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
                    lpszFullName = GetWholeFileName(lpCR->lpContentNew, 0, FALSE);
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
                    iResultStringsMac = ResultToString(rgszResultStrings, iResultStringsMac, iLinesWrittenOldPart, lpCR->nActionType, lpCR->lpContentNew, TRUE, bSuppressKey, iOutputType);
                //}
            }
        }

        for (i = 0; i < iResultStringsMac; i++) {
            if (NULL == rgszResultStrings[i]) {
                continue;
            }

            if (iOutputType == OUT_HTML) {
                // 1.8.0: zebra/flip-flop colors
                if (!fColor) {
                    WriteFileCP(hFile, nCodePage, lpszHTMLSpan1, cbHTMLSpan1, &NBW, NULL);
                }
                else {
                    WriteFileCP(hFile, nCodePage, lpszHTMLSpan2, cbHTMLSpan2, &NBW, NULL);
                }
            }

            lpszResultTemp = rgszResultStrings[i];
            for (nCharsToGo = _tcslen(rgszResultStrings[i]); 0 < nCharsToGo;) {
                nCharsToWrite = nCharsToGo;
                if (HTMLWRAPLENGTH < nCharsToWrite) {
                    nCharsToWrite = HTMLWRAPLENGTH;
                }

                WriteFileCP(hFile, nCodePage, lpszResultTemp, (DWORD)(nCharsToWrite * sizeof(TCHAR)), &NBW, NULL);
                lpszResultTemp += nCharsToWrite;
                nCharsToGo -= nCharsToWrite;

                if (0 == nCharsToGo) {
                    break;  // skip newline
                }

                if (iOutputType == OUT_HTML) {
                    WriteFileCP(hFile, nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
                }
                else {
                    WriteFileCP(hFile, nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
                }
            }
            MYFREE(rgszResultStrings[i]);

            if (iOutputType == OUT_HTML) {
                WriteFileCP(hFile, nCodePage, lpszHTMLSpanEnd, cbHTMLSpanEnd, &NBW, NULL);
                WriteFileCP(hFile, nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
            }
            else {
                WriteFileCP(hFile, nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
            }
        }

        // since 1.9.1: Separate objects in output with empty line
        if (fOutSeparateObjs) {
            if (iOutputType == OUT_HTML) {
                WriteFileCP(hFile, nCodePage, lpszHTML_BR, cbHTML_BR, &NBW, NULL);
            }
            else {
                WriteFileCP(hFile, nCodePage, lpszCRLF, cbCRLF, &NBW, NULL);
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

    if (iOutputType == OUT_HTML) {
        WriteFileCP(hFile, nCodePage, lpszHTMLTd2End, (DWORD)(_tcslen(lpszHTMLTd2End) * sizeof(TCHAR)), &NBW, NULL);
//        WriteFileCP(hFile, nCodePage, lpszHTMLTableEnd, (DWORD)(_tcslen(lpszHTMLTableEnd) * sizeof(TCHAR)), &NBW, NULL);
        WriteHTMLPartEnd(hFile);
    }
}

// ----------------------------------------------------------------------
VOID WriteTitle(HANDLE hFile, LPTSTR lpszSectionTitle, LPTSTR lpszValue, int iOutputType)
{
    //if (fAsHTML) {
    //    WriteFileCP(hFile, nCodePage, lpszHTMLTableBegin, (DWORD)(_tcslen(lpszHTMLTableBegin) * sizeof(TCHAR)), &NBW, NULL);
    //    WriteFileCP(hFile, nCodePage, lpszHTMLTd1Begin, (DWORD)(_tcslen(lpszHTMLTd1Begin) * sizeof(TCHAR)), &NBW, NULL);
    //}
    if (iOutputType == OUT_HTML)
        return;

    if (iOutputType == OUT_UNL)
        WriteFileCP(hFile, nCodePage, lpszCommentCommon, (DWORD)(_tcslen(lpszCommentCommon) * sizeof(TCHAR)), &NBW, NULL);
    else if (iOutputType == OUT_BAT)
        WriteFileCP(hFile, nCodePage, lpszCommentBAT, (DWORD)(_tcslen(lpszCommentCommon) * sizeof(TCHAR)), &NBW, NULL);
    else if (iOutputType == OUT_ISS_DEINSTALL)
        WriteFileCP(hFile, nCodePage, lpszCommentCommon, (DWORD)(_tcslen(lpszCommentCommon) * sizeof(TCHAR)), &NBW, NULL);
    else if (iOutputType == OUT_ISS_INSTALL)
        WriteFileCP(hFile, nCodePage, lpszCommentCommon, (DWORD)(_tcslen(lpszCommentCommon) * sizeof(TCHAR)), &NBW, NULL);
    else if (iOutputType == OUT_NSI_DEINSTALL)
        WriteFileCP(hFile, nCodePage, lpszCommentCommon, (DWORD)(_tcslen(lpszCommentCommon) * sizeof(TCHAR)), &NBW, NULL);
    else if (iOutputType == OUT_NSI_INSTALL)
        WriteFileCP(hFile, nCodePage, lpszCommentCommon, (DWORD)(_tcslen(lpszCommentCommon) * sizeof(TCHAR)), &NBW, NULL);
    else if (iOutputType == OUT_REG_DEINSTALL)
        WriteFileCP(hFile, nCodePage, lpszCommentCommon, (DWORD)(_tcslen(lpszCommentCommon) * sizeof(TCHAR)), &NBW, NULL);
    else if (iOutputType == OUT_REG_INSTALL)
        WriteFileCP(hFile, nCodePage, lpszCommentCommon, (DWORD)(_tcslen(lpszCommentCommon) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszSectionTitle, (DWORD)(_tcslen(lpszSectionTitle) * sizeof(TCHAR)), &NBW, NULL);
    if (lpszValue != NULL) {
        WriteFileCP(hFile, nCodePage, TEXT(" "), (DWORD)(1 * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, nCodePage, lpszValue, (DWORD)(_tcslen(lpszValue) * sizeof(TCHAR)), &NBW, NULL);
    }

    //if (fAsHTML) {
    //    WriteFileCP(hFile, nCodePage, lpszHTMLTd1End, (DWORD)(_tcslen(lpszHTMLTd1End) * sizeof(TCHAR)), &NBW, NULL);
    //    WriteFileCP(hFile, nCodePage, lpszHTMLTableEnd, (DWORD)(_tcslen(lpszHTMLTableEnd) * sizeof(TCHAR)), &NBW, NULL);
    //} else {
        WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    //}
}

// ----------------------------------------------------------------------
VOID WriteUNLINIKeys(HANDLE hFile)
{
    LPTSTR lpszBuffer;
    DWORD  nBufferSize = 2048;

    lpszBuffer = MYALLOC0(nBufferSize * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
    
    _tcscpy(_Notnull_ lpszBuffer, lpszCommentCommon);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    
    //Title = Sandboxie - Plus
    //Log = 1
    //; NewFileLog = 1
    //; LogOnlyErrors = 1
    //; Simulation = 1
    //DeleteReadOnly = 1
    //;
    //    if (GetDlgItemText(hWnd, IDC_EDITTITLE, lpszBuffer, EXTDIRLEN) != 0) {  // length incl. NULL character
    _tcscpy(lpszBuffer, TEXT("Title="));
    _tcscat(_Notnull_ lpszBuffer, lpszTitle);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("Log=1"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("; NewFileLog=1"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("; LogOnlyErrors=1"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("; Simulation=1"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("DeleteReadOnly="));
    _tcscat(lpszBuffer, (fDeleteReadOnly ? TEXT("1") : TEXT("0")));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);

    MYFREE(lpszBuffer);
}

VOID WriteISSSetupKeys(HANDLE hFile, BOOL bInstall)
{
    LPTSTR lpszBuffer;
    DWORD  nBufferSize = 2048;

    lpszBuffer = MYALLOC0(nBufferSize * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
    _tcscpy(_Notnull_ lpszBuffer, lpszCommentCommon);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);

    //; #define MyAppPublisher "skydive241@gmx.de"
    //; #define MyAppURL "http://www.example.com/"
    //; #define MyAppExeName "MyProg.exe"
    //; NOTE: The value of AppId uniquely identifies this application.
    //; Do not use the same AppId value in installers for other applications.
    //; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
    //; AppId = { {1A0386AC - 5147 - 4C46 - BEC7 - 720C729B21C2}
    //; AppVerName = {#MyAppName} {#MyAppVersion}
    //; AppPublisher = {#MyAppPublisher}
    //; InfoBeforeFile = regshot uninstaller - handle with care
    //; InfoAfterFile = '"Title" uninstalled


    //    if (GetDlgItemText(hWnd, IDC_EDITTITLE, lpszBuffer, EXTDIRLEN) != 0) {  // length incl. NULL character
        //#define MyAppName "Regshot's uninstaller for ""Title""
    _tcscpy(lpszBuffer, TEXT("#define MyAppName \"Regshot's "));
    if (bInstall)
        _tcscat(lpszBuffer, TEXT("installer"));
    else
        _tcscat(lpszBuffer, TEXT("uninstaller"));
    _tcscat(lpszBuffer, TEXT(" for \"\""));
    _tcscat(_Notnull_ lpszBuffer, lpszTitle);
    _tcscat(_Notnull_ lpszBuffer, TEXT("\"\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //#define MyAppVersion "1.0"
    _tcscpy(lpszBuffer, TEXT("#define MyAppVersion \"1.0\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _tcscpy(lpszBuffer, TEXT("[Setup]"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //AppName = {#MyAppName}
    _tcscpy(lpszBuffer, TEXT("AppName={#MyAppName}"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //AppVersion = {#MyAppVersion}
    _tcscpy(lpszBuffer, TEXT("AppVersion={#MyAppVersion}"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //CreateAppDir = no
    _tcscpy(lpszBuffer, TEXT("CreateAppDir=no"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //_tcscat(lpszBuffer, (fDeleteReadOnly ? TEXT("1") : TEXT("0")));
    //WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //OutputBaseFilename = Regshot - Installer
    _tcscpy(lpszBuffer, TEXT("OutputBaseFilename="));
    _tcscat(lpszBuffer, lpszTitle);
    if (bInstall)
        _tcscat(lpszBuffer, TEXT("-Installer"));
    else
        _tcscat(lpszBuffer, TEXT("-Uninstaller"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //Compression = lzma
    _tcscpy(lpszBuffer, TEXT("Compression=lzma"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //SolidCompression = yes
    _tcscpy(lpszBuffer, TEXT("SolidCompression=yes"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //Uninstallable = no
    _tcscpy(lpszBuffer, TEXT("Uninstallable=no"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //SetupLogging = True
    _tcscpy(lpszBuffer, TEXT("SetupLogging=True"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    if (fUseDifferentISSOutputFolder) {
        //TODO: OutputDir = c:\temp\Output
        _tcscpy(lpszBuffer, TEXT("OutputDir="));
        _tcscat(lpszBuffer, lpszISSOutputFolder);
        _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
        WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    }
    _tcscpy(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    //[Languages]
    _tcscpy(lpszBuffer, TEXT("[Languages]"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //Name: "english"; MessagesFile: "compiler:Default.isl"
    _tcscpy(lpszBuffer, TEXT("Name: \"english\"; MessagesFile: \"compiler:Default.isl\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);

//[Types]
    _tcscpy(lpszBuffer, TEXT("[Types]"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
//Name: "full"; Description: "Full maintenance"; Flags: iscustom
    _tcscpy(lpszBuffer, TEXT("Name: \"full\"; Description: \"Full maintenance\"; Flags: iscustom"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
//Name: "filesystem"; Description: "Filesystem maintenance"
    _tcscpy(lpszBuffer, TEXT("Name: \"filesystem\"; Description: \"Filesystem maintenance\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
//Name: "registry"; Description: "Registry maintenance"
    _tcscpy(lpszBuffer, TEXT("Name: \"registry\"; Description: \"Registry maintenance\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
//
//[Components]
    _tcscpy(lpszBuffer, TEXT("[Components]"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
//Name: Filesystem; Types: full filesystem; Description: Filesystem maintenance
    _tcscpy(lpszBuffer, TEXT("Name: \"Filesystem\"; Types: full filesystem; Description: \"Filesystem maintenance\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
//Name: Registry; Types: full registry; Description: Registry maintenance
    _tcscpy(lpszBuffer, TEXT("Name: \"Registry\"; Types: full registry; Description: \"Registry maintenance\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    
    _tcscpy(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    MYFREE(lpszBuffer);
}

VOID WriteNSISetupKeys(HANDLE hFile, BOOL bInstall)
{
    LPTSTR lpszBuffer;
    DWORD  nBufferSize = 2048;

    lpszBuffer = MYALLOC0(nBufferSize * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
    _tcscpy(_Notnull_ lpszBuffer, lpszCommentCommon);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    
//!include "MUI2.nsh"
//
//Name "Example2"
//OutFile "C:\Temp\Example2.exe"
//RequestExecutionLevel admin
//Unicode True
//ShowInstDetails Show
//
//;--------------------------------
//; Pages
//;--------------------------------
//!insertmacro MUI_PAGE_COMPONENTS
//!insertmacro MUI_PAGE_INSTFILES
//
//;--------------------------------
//;Languages
//;--------------------------------
//!insertmacro MUI_LANGUAGE "English"
//!insertmacro MUI_LANGUAGE "German"
//
//;--------------------------------
//; The stuff to install
//;--------------------------------
//Section "Example2"
//
//  SectionIn RO
    
//    if (GetDlgItemText(hWnd, IDC_EDITTITLE, lpszBuffer, EXTDIRLEN) != 0) {  // length incl. NULL character
//!include "MUI2.nsh"
    _tcscpy(lpszBuffer, TEXT("!include \"MUI2.nsh\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    
    //Name "Example2"
    _tcscpy(lpszBuffer, TEXT("Name \""));
    _tcscat(_Notnull_ lpszBuffer, lpszTitle);
    if (bInstall)
        _tcscat(lpszBuffer, TEXT("-installer"));
    else
        _tcscat(lpszBuffer, TEXT("-uninstaller"));
    _tcscat(_Notnull_ lpszBuffer, TEXT("\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    
    _tcscpy(lpszBuffer, TEXT("OutFile \""));
    if (fUseDifferentNSIOutputFolder) {
        _tcscat(lpszBuffer, lpszNSIOutputFolder);
        if (_tcsrchr(lpszBuffer, _T('\\')) != lpszBuffer + _tcslen(lpszBuffer) - 1)
            _tcscat(_Notnull_ lpszBuffer, TEXT("\\"));
    }
    _tcscat(_Notnull_ lpszBuffer, lpszTitle);
    if (bInstall)
        _tcscat(lpszBuffer, TEXT("-installer.exe"));
    else
        _tcscat(lpszBuffer, TEXT("-uninstaller.exe"));
    _tcscat(_Notnull_ lpszBuffer, TEXT("\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    
    //RequestExecutionLevel admin
    _tcscpy(lpszBuffer, TEXT("RequestExecutionLevel admin"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //Unicode True
    _tcscpy(lpszBuffer, TEXT("Unicode True"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //ShowInstDetails Show
    _tcscpy(lpszBuffer, TEXT("ShowInstDetails Show"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //InstallDir "$EXEDIR"
    _tcscpy(lpszBuffer, TEXT("InstallDir \"$EXEDIR\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    //;--------------------------------
    //; Pages
    //;--------------------------------
    //!insertmacro MUI_PAGE_COMPONENTS
    //!insertmacro MUI_PAGE_INSTFILES
    _tcscpy(lpszBuffer, TEXT(";--------------------------------"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("; Pages"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT(";--------------------------------"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("!insertmacro MUI_PAGE_COMPONENTS"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("!insertmacro MUI_PAGE_INSTFILES"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //;--------------------------------
    //;Languages
    //;--------------------------------
    //!insertmacro MUI_LANGUAGE "English"
    //!insertmacro MUI_LANGUAGE "German"
    _tcscpy(lpszBuffer, TEXT(";--------------------------------"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("; Languages"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT(";--------------------------------"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("!insertmacro MUI_LANGUAGE \"English\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("!insertmacro MUI_LANGUAGE \"German\""));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //;--------------------------------
    //; Maintenance actions
    //;--------------------------------
    //Section "Example2"
    //
    //  SectionIn RO
    _tcscpy(lpszBuffer, TEXT(";--------------------------------"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT("; Maintenance actions"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, TEXT(";--------------------------------"));
    _tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //_tcscpy(lpszBuffer, TEXT("Section \"Maintenance\""));
    //_tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    //_tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    //WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //_tcscpy(lpszBuffer, TEXT("  SectionIn RO"));
    //_tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    //_tcscat(_Notnull_ lpszBuffer, lpszCRLF);
    //WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    MYFREE(lpszBuffer);
}

VOID WriteISSEnd(HANDLE hFile)
{
    LPTSTR lpszBuffer;
    DWORD  nBufferSize = 2048;

    lpszBuffer = MYALLOC0(nBufferSize * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
    if (lpszBuffer != NULL) {
        _tcscpy(lpszBuffer, lpszISSSectionMessages);
        WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
        _tcscpy(lpszBuffer, lpszISSReadyLabel1);
        WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
        _tcscpy(lpszBuffer, lpszISSReadyLabel2b);
        WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
        _tcscpy(lpszBuffer, lpszISSButtonInstall);
        WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);

        MYFREE(lpszBuffer);
    }
}

VOID WriteNSISectionEnd(HANDLE hFile)
{
    LPTSTR lpszBuffer;
    DWORD  nBufferSize = 2048;

    lpszBuffer = MYALLOC0(nBufferSize * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
    if (lpszBuffer != NULL) {
        WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
        _tcscpy(lpszBuffer, lpszNSISectionEnd);
        WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
        WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);

        MYFREE(lpszBuffer);
    }
}

// ----------------------------------------------------------------------
VOID WriteCommonBegin(HANDLE hFile, LPTSTR lpszDestFileName, LPTSTR lpszComment)
{
    LPTSTR lpszBuffer;
    DWORD  nBufferSize = 2048;

    lpszBuffer = MYALLOC0(nBufferSize * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
    //;
    //; Maintenance - File D : \Eigene Dateien\Snapshots\Sandboxie - Plus\Sandboxie - Plus.UNL
    //; @ Patrick Eder(Patrick.Eder@gmx.de)
    //; !!!!!!!! H A N D L E   W I T H   C A R E !!!!!!!!
    //;
    //; Regshot x64 ANSI Dbg(trunk rUnknown)
    _tcscpy(_Notnull_ lpszBuffer, lpszComment);
    WriteFileCP(hFile, nCodePage, _Notnull_ lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    _tcscat(_Notnull_ lpszBuffer, TEXT("Maintenance - File "));
    _tcscat(_Notnull_ lpszBuffer, lpszDestFileName);
    WriteFileCP(hFile, nCodePage, _Notnull_ lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, lpszComment);
    _tcscat(lpszBuffer, TEXT("@ Patrick (skydive241@gmx.de)"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, lpszComment);
    _tcscat(lpszBuffer, TEXT("!!!!!!!! H A N D L E   W I T H   C A R E !!!!!!!!"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
    _tcscpy(lpszBuffer, lpszComment);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);

    _tcscat(lpszBuffer, lpszProgramName);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);

    MYFREE(lpszBuffer);
}

VOID WriteHTMLPartStart(HANDLE hFile, LPTSTR lpszPart, int nCount)
{
    LPTSTR lpszBuffer, lpszBuffer2;
    DWORD  nBufferSize = 2048;
//    lpszBuffer = MYALLOC0((nBufferSize + 1) * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
//    lpszBuffer2 = MYALLOC0((nBufferSize + 1) * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
    
    //<tr class="c">
	//	<td><a name="c"></a>&nbsp;</td>
	//</tr>
    WriteFileCP(hFile, nCodePage, lpszHTMLtrClass, (DWORD)(_tcslen(lpszHTMLtrClass) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, TEXT("c\">\r\n"), (DWORD)(5 * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLtdANameC, (DWORD)(_tcslen(lpszHTMLtdANameC) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLtrEnd, (DWORD)(_tcslen(lpszHTMLtrEnd) * sizeof(TCHAR)), &NBW, NULL);
                                                               //<tr class = "h">
    //    <td>Deleted keys(0) for shot A</td>
    //< /tr>
    WriteFileCP(hFile, nCodePage, lpszHTMLtrClass, (DWORD)(_tcslen(lpszHTMLtrClass) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, TEXT("h\">\r\n"), (DWORD)(5 * sizeof(TCHAR)), &NBW, NULL);
    
    lpszBuffer = MYALLOC0((nBufferSize + 1) * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
    lpszBuffer2 = MYALLOC0((nBufferSize + 1) * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
    _tcscpy(lpszBuffer2, lpszHTMLtd);
    _sntprintf(lpszBuffer, nBufferSize, lpszBuffer2, lpszPart, nCount);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLtrEnd, (DWORD)(_tcslen(lpszHTMLtrEnd) * sizeof(TCHAR)), &NBW, NULL);

    MYFREE(lpszBuffer2);
    MYFREE(lpszBuffer);
}

VOID WriteHTMLPartEnd(HANDLE hFile)
{
    UNREFERENCED_PARAMETER(hFile);

}

VOID WriteHTMLSectionStart(HANDLE hFile, LPTSTR lpszSection)
{
    LPTSTR lpszBuffer, lpszBuffer2;
    DWORD  nBufferSize = 2048;
    lpszBuffer = MYALLOC0((nBufferSize + 1) * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
    lpszBuffer2 = MYALLOC0((nBufferSize + 1) * sizeof(TCHAR)); // nBufferSize must > commentlength + 10 .txt 0000
    
//<table id="hive" cellspacing="1">
    _tcscpy(lpszBuffer2, lpszHTMLTableBegin);
    _sntprintf(lpszBuffer, nBufferSize, lpszBuffer2, lpszSection);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    MYFREE(lpszBuffer2);
    MYFREE(lpszBuffer);
}

// ----------------------------------------------------------------------
VOID WriteHTMLBegin(HANDLE hFile)
{
    LPTSTR lpszBuffer, lpszBuffer2;
    DWORD  nBufferSize = 2048;
    lpszBuffer = MYALLOC0((nBufferSize + 1) * sizeof(TCHAR));
    lpszBuffer2 = MYALLOC0((nBufferSize + 1) * sizeof(TCHAR));

    // HTML head
    WriteFileCP(hFile, nCodePage, lpszHTMLBegin, (DWORD)(_tcslen(lpszHTMLBegin) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLHeadBegin, (DWORD)(_tcslen(lpszHTMLHeadBegin) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLContent, (DWORD)(_tcslen(lpszHTMLContent) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, lpszHTMLTitle, lpszProgramName);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLCSSLink, (DWORD)(_tcslen(lpszHTMLCSSLink) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLScript, (DWORD)(_tcslen(lpszHTMLScript) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLHeadEnd, (DWORD)(_tcslen(lpszHTMLHeadEnd) * sizeof(TCHAR)), &NBW, NULL);
    
    WriteFileCP(hFile, nCodePage, lpszHTMLScript_1, (DWORD)(_tcslen(lpszHTMLScript_1) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLMenu, (DWORD)(_tcslen(lpszHTMLMenu) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("a"), TEXT("\">"), TEXT("Summary report"), TEXT("</a>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("b"), TEXT("\">"), TEXT("Deleted keys"), TEXT("</a>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("c"), TEXT("\">"), TEXT("New keys"), TEXT("</a>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("d"), TEXT("\">"), TEXT("Deleted values"), TEXT("</a>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("e"), TEXT("\">"), TEXT("New values"), TEXT("</a>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("f"), TEXT("\">"), TEXT("Changed values"), TEXT("</a>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("g"), TEXT("\">"), TEXT("FileShot"), TEXT("</a>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t%s%s%s%s%s\0"), TEXT("\t<a href=\"#"), TEXT("h"), TEXT("\">"), TEXT("To end of report"), TEXT("</a> ]</nobr>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLDivEnd, (DWORD)(_tcslen(lpszHTMLDivEnd) * sizeof(TCHAR)), &NBW, NULL);

    WriteFileCP(hFile, nCodePage, lpszHTMLClassm, (DWORD)(_tcslen(lpszHTMLClassm) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("%s%s%s%s\0"), TEXT("<h2>"), TEXT("Report of the "), lpszProgramName, TEXT("</h2>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLdivCenter, (DWORD)(_tcslen(lpszHTMLdivCenter) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLTableInfo, (DWORD)(_tcslen(lpszHTMLTableInfo) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("h"), TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s%s%s\0"), lpszHTMLtdColspan, TEXT("3"),  TEXT("\">"), TEXT("Summary report"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s\0"), TEXT("<td>&nbsp;</td>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s\0"), lpszHTMLtdClass, TEXT("b"),  TEXT("\"><b>Shot&nbsp;1</b></td>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s\0"), lpszHTMLtdClass, TEXT("b"),  TEXT("\"><b>Shot&nbsp;2</b></td>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextDateTime].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%04d-%02d-%02d %02d:%02d:%02d%s\0"), lpszHTMLtdClass, TEXT("b"),  TEXT("\">"), 
        CompareResult.lpShot1->systemtime.wYear, CompareResult.lpShot1->systemtime.wMonth, CompareResult.lpShot1->systemtime.wDay,
        CompareResult.lpShot1->systemtime.wHour, CompareResult.lpShot1->systemtime.wMinute, CompareResult.lpShot1->systemtime.wSecond, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%04d-%02d-%02d %02d:%02d:%02d%s\0"), lpszHTMLtdClass, TEXT("b"),  TEXT("\">"), 
        CompareResult.lpShot1->systemtime.wYear, CompareResult.lpShot1->systemtime.wMonth, CompareResult.lpShot2->systemtime.wDay,
        CompareResult.lpShot1->systemtime.wHour, CompareResult.lpShot1->systemtime.wMinute, CompareResult.lpShot2->systemtime.wSecond, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextComputer].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    //if (NULL != CompareResult.lpShot1->lpszComputerName) {
    //    _tcscpy(lpszBuffer, CompareResult.lpShot1->lpszComputerName);
    //}
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"),  TEXT("\">"), CompareResult.lpShot1->lpszComputerName, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot2->lpszComputerName, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextUsername].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"),  TEXT("\">"), CompareResult.lpShot1->lpszUserName, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"),  TEXT("\">"), CompareResult.lpShot2->lpszUserName, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), TEXT("Shot type"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

// Shot-Time ist bislang nicht protokolliert
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), TEXT("Shot time"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextKey].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot1->stCounts.cKeys, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot2->stCounts.cKeys, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextValue].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot1->stCounts.cValues, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot2->stCounts.cValues, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextDir].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot1->stCounts.cDirs, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot2->stCounts.cDirs, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextFile].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot1->stCounts.cFiles, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.lpShot2->stCounts.cFiles, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextKeyDel].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcDeleted.cKeys, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"),  TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextKeyAdd].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcAdded.cKeys, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextValDel].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcDeleted.cValues, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextValAdd].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcAdded.cValues, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextValModi].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcChanged.cValues, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextDirDel].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcDeleted.cDirs, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextDirAdd].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcAdded.cDirs, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextDirModi].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcModified.cDirs, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextFileDel].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcDeleted.cFiles, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"), TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextFileAdd].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcAdded.cFiles, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"), TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), asLangTexts[iszTextFileModi].lpszText, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcModified.cFiles, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\"><b>"), asLangTexts[iszTextTotal].lpszText, TEXT("</b></td>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%u%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), CompareResult.stcModified.cAll, lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("b"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("a"), TEXT("\">"), TEXT("File to restoring registry"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s%s%s\0"), lpszHTMLtdClass, TEXT("b"), TEXT("\">"), TEXT("-"), lpszHTMLtdEnd);
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);

    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s%s%s\0"), lpszHTMLtrClass, TEXT("a"),  TEXT("\">\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t\t%s%s%s\0"), lpszHTMLtdClass, TEXT("c"), TEXT("\" colspan=\"3\"><b>Comment</b>:&nbsp;<span class=\"i\"></span></td>\r\n"));
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);
    _sntprintf(lpszBuffer, nBufferSize, TEXT("\t\t%s\0"), lpszHTMLtrEnd);
    lpszBuffer[nBufferSize] = _T('\0');
    WriteFileCP(hFile, nCodePage, lpszBuffer, (DWORD)(_tcslen(lpszBuffer) * sizeof(TCHAR)), &NBW, NULL);

    WriteFileCP(hFile, nCodePage, lpszHTMLTableEnd, (DWORD)(_tcslen(lpszHTMLDivEnd) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLDivEnd, (DWORD)(_tcslen(lpszHTMLDivEnd) * sizeof(TCHAR)), &NBW, NULL);

    MYFREE(lpszBuffer2);
    MYFREE(lpszBuffer);
}

// ----------------------------------------------------------------------
VOID WriteHTMLEnd(HANDLE hFile)
{
    WriteFileCP(hFile, nCodePage, lpszHTMLBodyEnd, (DWORD)(_tcslen(lpszHTMLBodyEnd) * sizeof(TCHAR)), &NBW, NULL);
    WriteFileCP(hFile, nCodePage, lpszHTMLEnd, (DWORD)(_tcslen(lpszHTMLEnd) * sizeof(TCHAR)), &NBW, NULL);
}

// ----------------------------------------------------------------------
VOID WriteHTML_BR(HANDLE hFile)
{
    WriteFileCP(hFile, nCodePage, lpszHTML_BR, (DWORD)(_tcslen(lpszHTML_BR) * sizeof(TCHAR)), &NBW, NULL);
}

// ----------------------------------------------------------------------
VOID WriteHTML_CSS(HANDLE hFile)
{
    WriteFileCP(hFile, nCodePage, lpszHTML_CSS2, (DWORD)(_tcslen(lpszHTML_CSS2) * sizeof(CHAR)), &NBW, NULL);
}
