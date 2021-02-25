/*
    Copyright 2021-     Patrick (skydive241@gmx.de)
    Copyright 1999-2003,2007 TiANWEi
    Copyright 2011-2012 Regshot Team

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

// ----------------------------------------------------------------------
// Write BOM, especially for ISS output files
// ----------------------------------------------------------------------
VOID WriteBOM(HANDLE hFile)
{
//    FF FE 57
    unsigned char bUTF8[] = { 0xef, 0xbb, 0xbf };
    unsigned char bUTF16[] = { 0xff, 0xfe };
    if (nCodePage == CP_UTF8) {
        WriteFile(hFile, &bUTF8, sizeof(bUTF8), &NBW, NULL);
    }
    else if (nCodePage == -1) {
        WriteFile(hFile, &bUTF16, sizeof(bUTF16), &NBW, NULL);
    }

}

// ----------------------------------------------------------------------
// Write to output files with the correct codepage
// ----------------------------------------------------------------------
BOOL WriteFileCP(
    _In_ HANDLE hFile,
    _In_ int CodePage,
    _In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,
    _In_ DWORD nNumberOfBytesToWrite,
    _Out_opt_ LPDWORD lpNumberOfBytesWritten,
    _Inout_opt_ LPOVERLAPPED lpOverlapped
)
{
    LPSTR lpTransformedBuffer;
    int nTransformedBufferSize = 4 * nNumberOfBytesToWrite + 1;
    int nSize = 0;

    if (lpNumberOfBytesWritten != NULL)
        *lpNumberOfBytesWritten = 0;

    if (CodePage == -1) {
        return WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
    }
    lpTransformedBuffer = MYALLOC0(nTransformedBufferSize);
    if (lpTransformedBuffer == NULL)
        return FALSE;

// CodePage = CP_UTF8;
    if (lpBuffer != NULL) {
#ifdef _UNICODE
        nSize = WideCharToMultiByte((UINT)CodePage, 0, (LPCWSTR)lpBuffer, -1, lpTransformedBuffer, nTransformedBufferSize, NULL, NULL);
#else
        nSize = MultiByteToWideChar(CodePage, MB_PRECOMPOSED, (LPCCH)lpBuffer, -1, lpTransformedBuffer, nTransformedBufferSize);
#endif
    }
    BOOL bReturn = WriteFile(hFile, lpTransformedBuffer, nSize - 1, lpNumberOfBytesWritten, lpOverlapped);

    MYFREE(lpTransformedBuffer);
//    HeapFree(hHeap, 0, lpTransformedBuffer);


    return bReturn;
}

// ----------------------------------------------------------------------
// Write file content to file buffer to speed up saving onto network drives
// as a lot of SetFilePointer() calls will extremely decrease save speed
// ----------------------------------------------------------------------
VOID WriteFileBuffer(HANDLE hFile, long ofsFile, LPCVOID lpData, DWORD cbData)
{
    DWORD ofsFileDW;
    long  ofsFileCurrent;

    if (0 >= cbData) {  // Ignore, but still Aaarrrggghhh!!!
        return;
    }

    ofsFileCurrent = (long)fileextradata.ofsFBFile + (long)fileextradata.ofsFBuffer;

    if (0 > ofsFile) {  // special case: negative value means append
        ofsFile = ofsFileCurrent;
    }
    else {
        // Error case: Writing only allowed before or at current file offset (replace or append)
        assert(ofsFileCurrent >= ofsFile);
    }

    ofsFileDW = ofsFile;

    if ((long)fileextradata.ofsFBFile > ofsFile) {
        // Error case: Writing before buffer only allowed if data is not reaching into the buffer / no buffer: behind current file end
        assert((ofsFile + (long)cbData) <= (long)fileextradata.ofsFBFile);

        SetFilePointer(hFile, ofsFileDW, NULL, FILE_BEGIN);
        WriteFile(hFile, lpData, cbData, &NBW, NULL);
        SetFilePointer(hFile, fileextradata.ofsFBFile, NULL, FILE_BEGIN);
        return;
    }

    if ((NULL == lpFileBuffer) && (!fileextradata.bFBStopAlloc)) {
        DWORD cbNewSize;

        cbNewSize = 1024 * 1024;  // 1 MiB
        lpFileBuffer = MYALLOC(cbNewSize);
        if (NULL == lpFileBuffer) {
            cbNewSize = 128 * 1024;  // 128 KiB
            lpFileBuffer = MYALLOC(cbNewSize);
        }
        if (NULL == lpFileBuffer) {
            cbNewSize = 10 * 1024;  // 10 KiB
            lpFileBuffer = MYALLOC(cbNewSize);
        }
        if (NULL == lpFileBuffer) {
            cbNewSize = 1024;  // 1 KiB
            lpFileBuffer = MYALLOC(cbNewSize);
        }
        if (NULL == lpFileBuffer) {
            fileextradata.bFBStopAlloc = TRUE;
        }
        else {
            fileextradata.cbFBuffer = cbNewSize;
        }
    }

    if (NULL != lpFileBuffer) {
        DWORD cbWantedSize;

        // Error case: Not appending but still writing over current buffer offset
        if (ofsFileCurrent > ofsFile) {
            assert((ofsFile + (long)cbData) <= ofsFileCurrent);
        }

        cbWantedSize = ofsFileDW - fileextradata.ofsFBFile + cbData;

        if (cbWantedSize > fileextradata.cbFBuffer) {
            EmptyFileBuffer(hFile);
            cbWantedSize = ofsFileDW - fileextradata.ofsFBFile + cbData;
        }

        if (cbWantedSize <= fileextradata.cbFBuffer) {
            ofsFileDW -= fileextradata.ofsFBFile;
            CopyMemory((lpFileBuffer + ofsFileDW), lpData, cbData);
            if (ofsFileDW == fileextradata.ofsFBuffer) {
                fileextradata.ofsFBuffer += cbData;
            }
            return;
        }
    }

    // Fallback: direct append (e.g. no buffer or buffer too small)
    WriteFile(hFile, lpData, cbData, &NBW, NULL);
    fileextradata.ofsFBFile += cbData;
}

// ----------------------------------------------------------------------
// Write file buffer content to file with a single write
// ----------------------------------------------------------------------
VOID EmptyFileBuffer(HANDLE hFile)
{
    if ((NULL != lpFileBuffer) && (0 < fileextradata.ofsFBuffer)) {
        WriteFile(hFile, lpFileBuffer, fileextradata.ofsFBuffer, &NBW, NULL);
        fileextradata.ofsFBFile += fileextradata.ofsFBuffer;
        fileextradata.ofsFBuffer = 0;
    }
}

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------
size_t AdjustBuffer(LPVOID* lpBuffer, size_t nCurrentSize, size_t nWantedSize, size_t nAlign)
{
    if (NULL == *lpBuffer) {
        nCurrentSize = 0;
    }

    if (nWantedSize > nCurrentSize) {
        if (NULL != *lpBuffer) {
            MYFREE(*lpBuffer);
            *lpBuffer = NULL;
        }

        if (1 >= nAlign) {
            nCurrentSize = nWantedSize;
        }
        else {
            nCurrentSize = nWantedSize / nAlign;
            nCurrentSize *= nAlign;
            if (nWantedSize > nCurrentSize) {
                nCurrentSize += nAlign;
            }
        }

        *lpBuffer = MYALLOC(nCurrentSize);
    }

    return nCurrentSize;
}

// VS 6 fix, may depend on PSDK/WSDK too
#ifndef _tcsnlen
size_t _tcsnlen(const TCHAR* str, size_t numberOfElements)
{
    size_t i;

    for (i = 0; i < numberOfElements; i++, str++) {
        if ((TCHAR)'\0' == *str) {
            break;
        }
    }

    return i;
}
#endif

// ----------------------------------------------------------------------
// Show error message
// ----------------------------------------------------------------------
VOID ErrMsg(LPTSTR lpszErrMsg)
{
    MessageBox(hMainWnd, lpszErrMsg, asLangTexts[iszTextError].lpszText, MB_ICONHAND);
}

#ifdef DEBUGLOG
// debug log files
LPTSTR lpszDebugTryToGetValueLog = TEXT("debug_trytogetvalue.log");
LPTSTR lpszDebugValueNameDataLog = TEXT("debug_valuenamedata.log");
LPTSTR lpszDebugKeyLog = TEXT("debug_key.log");

// ----------------------------------------------------------------------
// Write message to debug log file
// ----------------------------------------------------------------------
VOID DebugLog(LPTSTR lpszFileName, LPTSTR lpszDbgMsg, BOOL fAddCRLF)
{
    size_t nLen;
    DWORD nPos;

    hFile = CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) {
        ErrMsg(asLangTexts[iszTextErrorCreateFile].lpszText);
        return;
    }

    nPos = SetFilePointer(hFile, 0, NULL, FILE_END);
    if (0xFFFFFFFF == nPos) {
        ErrMsg(asLangTexts[iszTextErrorMoveFP].lpszText);
    }
    else {
        nLen = _tcslen(lpszDbgMsg) * sizeof(TCHAR);
        WriteFile(hFile, lpszDbgMsg, (DWORD)nLen, &NBW, NULL);
        if (NBW != nLen) {
            //ErrMsg(asLangTexts[iszTextErrorWriteFile].lpszText);
        }
        if (fAddCRLF) {
            WriteFile(hFile, lpszCRLF, (DWORD)(_tcslen(lpszCRLF) * sizeof(TCHAR)), &NBW, NULL);
        }
    }

    CloseHandle(hFile);
}
#endif

// ----------------------------------------------------------------------
// Replace invalid chars for a file name
// ----------------------------------------------------------------------
LPTSTR lpszInvalid = TEXT("\\/:*?\"<>|");  // 1.8.2

// ----------------------------------------------------------------------
// Replace invalid chars for a file name
// ----------------------------------------------------------------------
BOOL ReplaceInvalidFileNameChars(LPTSTR lpszFileName)
{
    size_t nInvalidLen;
    size_t nFileNameLen;
    size_t i, j;
    BOOL fFileNameIsLegal;
    BOOL fCharIsValid;

    nInvalidLen = _tcslen(lpszInvalid);
    nFileNameLen = _tcslen(lpszFileName);

    fFileNameIsLegal = FALSE;
    for (i = 0; i < nFileNameLen; i++) {
        fCharIsValid = TRUE;  // valid chars expected

        if ((TCHAR)'\t' == lpszFileName[i]) {  // replace tab with space
            lpszFileName[i] = (TCHAR)' ';
        }
        else {  // replace invalid char with underscore
            for (j = 0; j < nInvalidLen; j++) {
                if (lpszFileName[i] == lpszInvalid[j]) {
                    lpszFileName[i] = (TCHAR)'_';
                    fCharIsValid = FALSE;
                    break;
                }
            }
        }

        if ((fCharIsValid) && ((TCHAR)' ' != lpszFileName[i])) {  // At least one valid non-space char needed
            fFileNameIsLegal = TRUE;
        }
    }
    return fFileNameIsLegal;
}

// this new function added by Youri in 1.8.2, for expanding path in browse dialog
int CALLBACK SelectBrowseFolder(HWND hWndBrowse, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    UNREFERENCED_PARAMETER(lParam);
    if (BFFM_INITIALIZED == uMsg) {
        SendMessage(hWndBrowse, BFFM_SETSELECTION, 1, lpData);
    }
    return 0;
}

