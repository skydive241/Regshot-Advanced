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

// ----------------------------------------------------------------------
// Several routines to write to an output file
// ----------------------------------------------------------------------
LPTSTR EscapeSpecialCharacters(LPTSTR lpszText, TCHAR c, LPTSTR lpEscape)
{
    LPTSTR pos = NULL;
    size_t i = 0;

    if (lpszText != NULL)
        pos = _tcschr(lpszText, c);
    while (pos) {
        i++;
        pos = _tcschr(++pos, c);
    }

    if (i > 0) {
        LPTSTR lpszOutputStringCorrected = MYALLOC0((_tcslen(lpszText) + i * _tcslen(lpEscape) + 1) * sizeof(TCHAR));
        
        i = 0;
        for (int j = 0; j <= _tcslen(lpszText); j++) {
            if (lpszOutputStringCorrected + j + i != NULL) {
                if (_tcsncmp(lpszText + j, &c, 1) == 0) {
                    _tcsncpy(lpszOutputStringCorrected + j + i, lpEscape, _tcslen(lpEscape));
                    i = i + _tcslen(lpEscape);
                }
                if (lpszOutputStringCorrected + j + i != NULL) 
                    _tcsncpy(lpszOutputStringCorrected + j + i, lpszText + j, 1);
            }
        }
        MYFREE(lpszText);
        return lpszOutputStringCorrected;
    }

    return lpszText;
}

