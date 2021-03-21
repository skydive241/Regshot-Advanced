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

LANGUAGETEXT asLangTexts[cLangStrings];

LPTSTR lpszDefaultLanguage = TEXT("English");
LPTSTR lpszItemTranslator  = TEXT("Translator=");
LPTSTR lpszSectionCurrent  = TEXT("CURRENT");
LPTSTR lpszOriginal        = TEXT("[Original]");

LPTSTR lpszLanguage;
size_t cchMaxLanguageNameLen;

LPTSTR lpgrszLangSection;
LPTSTR lpszQuit            = TEXT("Quit");
LPTSTR lpszStop            = TEXT("Stop");


// ----------------------------------------------------------------------
// Get available languages from language ini and calculate maximal
// name length
// An English section in language ini will be ignored
// ----------------------------------------------------------------------
VOID CalculateMaxLanguageNameLength(VOID)
{
    //LRESULT nResult;
    LPTSTR lpgrszSectionNames;
    DWORD cchSectionNames;
    size_t i;
    size_t nLanguageNameLen;

    cchMaxLanguageNameLen = _tcslen(lpszDefaultLanguage) + 1;  // incl. NULL character

    // Get sections (=language names) from language ini
    lpgrszSectionNames = MYALLOC0(MAX_INI_SECTION_CHARS * sizeof(TCHAR));
    if (lpgrszSectionNames == NULL)
        return;
    cchSectionNames = GetPrivateProfileSectionNames(lpgrszSectionNames, MAX_INI_SECTION_CHARS, lpszLanguageIni);  // length incl. double NULL character
    if (1 < cchSectionNames) {
        for (i = 0; i < cchSectionNames;) {
            if (0 == lpgrszSectionNames[i]) {  // reached the end of the section names buffer
                break;
            }

            nLanguageNameLen = _tcslen(&lpgrszSectionNames[i]) + 1;  // incl. NULL character

            if ((0 != _tcsicmp(&lpgrszSectionNames[i], lpszSectionCurrent)) && (0 != _tcsicmp(&lpgrszSectionNames[i], lpszDefaultLanguage))) {
                if (nLanguageNameLen > cchMaxLanguageNameLen) {
                    cchMaxLanguageNameLen = nLanguageNameLen;
                }
            }

            i += nLanguageNameLen;  // skip to next string
        }
    }
    MYFREE(lpgrszSectionNames);

    // Allocate memory for longest language name, and copy default language name to it
    lpszLanguage = MYALLOC0((cchMaxLanguageNameLen) * sizeof(TCHAR));

    return;
}

// ----------------------------------------------------------------------
// Initialize text strings with English defaults
// ----------------------------------------------------------------------
VOID SetTextsToDefaultLanguage(VOID)
{
    // Clear all structures (pointers, IDs, etc.)
    ZeroMemory(asLangTexts, sizeof(asLangTexts));

    // Set English default language strings
    asLangTexts[iszTextKey].lpszText                    = TEXT("Keys:");
    asLangTexts[iszTextValue].lpszText                  = TEXT("Values:");
    asLangTexts[iszTextDir].lpszText                    = TEXT("Dirs:");
    asLangTexts[iszTextFile].lpszText                   = TEXT("Files:");
    asLangTexts[iszTextTime].lpszText                   = TEXT("Time:");
    asLangTexts[iszTextKeyAdd].lpszText                 = TEXT("Keys added:");
    asLangTexts[iszTextKeyDel].lpszText                 = TEXT("Keys deleted:");
    asLangTexts[iszTextValAdd].lpszText                 = TEXT("Values added:");
    asLangTexts[iszTextValDel].lpszText                 = TEXT("Values deleted:");
    asLangTexts[iszTextValModi].lpszText                = TEXT("Values modified:");
    asLangTexts[iszTextRegistryModi].lpszText           = TEXT("Registry objects changed:");
    asLangTexts[iszTextFileAdd].lpszText                = TEXT("Files added:");
    asLangTexts[iszTextFileDel].lpszText                = TEXT("Files deleted:");
    asLangTexts[iszTextFileModi].lpszText               = TEXT("Files [attributes?] modified:");
    asLangTexts[iszTextDirAdd].lpszText                 = TEXT("Folders added:");
    asLangTexts[iszTextDirDel].lpszText                 = TEXT("Folders deleted:");
    asLangTexts[iszTextDirModi].lpszText                = TEXT("Folders attributes changed:");
    asLangTexts[iszTextFilesystemModi].lpszText         = TEXT("Filesystem objects changed:");
    asLangTexts[iszTextTotal].lpszText                  = TEXT("Total changes:");
    asLangTexts[iszTextComments].lpszText               = TEXT("Comments:");
    asLangTexts[iszTextDateTime].lpszText               = TEXT("Datetime:");
    asLangTexts[iszTextComputer].lpszText               = TEXT("Computer:");
    asLangTexts[iszTextUsername].lpszText               = TEXT("Username:");
    asLangTexts[iszTextAbout].lpszText                  = TEXT("About");
    asLangTexts[iszTextError].lpszText                  = TEXT("Error");
    asLangTexts[iszTextErrorExecViewer].lpszText        = TEXT("Error call External Viewer!");
    asLangTexts[iszTextErrorCreateFile].lpszText        = TEXT("Error creating file!");
    asLangTexts[iszTextErrorOpenFile].lpszText          = TEXT("Error open file!");
    asLangTexts[iszTextErrorMoveFP].lpszText            = TEXT("Error move file pointer!");

    asLangTexts[iszTextButtonShot1].lpszText            = TEXT("&1st shot");
    asLangTexts[iszTextButtonShot1].nIDDialog           = DLG_MAIN;
    asLangTexts[iszTextButtonShot1].nIDDlgItem          = IDC_1STSHOT;

    asLangTexts[iszTextButtonShot2].lpszText            = TEXT("&2nd shot");
    asLangTexts[iszTextButtonShot2].nIDDialog           = DLG_MAIN;
    asLangTexts[iszTextButtonShot2].nIDDlgItem          = IDC_2NDSHOT;

    asLangTexts[iszTextButtonCompare].lpszText          = TEXT("C&ompare");
    asLangTexts[iszTextButtonCompare].nIDDialog         = DLG_MAIN;
    asLangTexts[iszTextButtonCompare].nIDDlgItem        = IDC_COMPARE;

    asLangTexts[iszTextMenuClear].lpszText              = TEXT("&Clear");

    asLangTexts[iszTextButtonQuit].lpszText             = TEXT("&Quit");
    asLangTexts[iszTextButtonQuit].nIDDialog = DLG_MAIN;
    asLangTexts[iszTextButtonQuit].nIDDlgItem           = IDC_QUIT;
    lpszQuit = asLangTexts[iszTextButtonQuit].lpszText;

    asLangTexts[iszTextButtonAbout].lpszText            = TEXT("&About...");
    asLangTexts[iszTextButtonAbout].nIDDialog = DLG_MAIN;
    asLangTexts[iszTextButtonAbout].nIDDlgItem          = IDC_ABOUT;

    asLangTexts[iszTextTextMonitor].lpszText            = TEXT("&Monitor...");
    //asLangTexts[iszTextTextMonitor].nIDDlgItem          = IDC_MONITOR;

    asLangTexts[iszTextTextCompare].lpszText            = TEXT("Compare logs save as:");
    asLangTexts[iszTextTextCompare].nIDDialog = DLG_MAIN;
    asLangTexts[iszTextTextCompare].nIDDlgItem          = IDC_STATICSAVEFORMAT;

    asLangTexts[iszTextTextOutput].lpszText             = TEXT("Output path:");
    asLangTexts[iszTextTextOutput].nIDDialog = DLG_MAIN;
    asLangTexts[iszTextTextOutput].nIDDlgItem           = IDC_STATICOUTPUTPATH;

    asLangTexts[iszTextTextComment].lpszText            = TEXT("Add comment into the log:");
    asLangTexts[iszTextTextComment].nIDDialog = DLG_MAIN;
    asLangTexts[iszTextTextComment].nIDDlgItem          = IDC_STATICADDCOMMENT;

    asLangTexts[iszTextRadioPlain].lpszText             = TEXT("CMD file");
    asLangTexts[iszTextRadioPlain].nIDDialog = DLG_MAIN;
    asLangTexts[iszTextRadioPlain].nIDDlgItem           = IDC_CHECK_BAT;

    asLangTexts[iszTextRadioHTML].lpszText              = TEXT("&HTML document");
    asLangTexts[iszTextRadioHTML].nIDDialog = DLG_MAIN;
    asLangTexts[iszTextRadioHTML].nIDDlgItem            = IDC_CHECK_HTML;

    asLangTexts[iszTextTextScan].lpszText               = TEXT("&Scan dir1[;dir2;dir3;...;dir nn]:");
    asLangTexts[iszTextTextScan].nIDDialog = DLG_MAIN;
    asLangTexts[iszTextTextScan].nIDDlgItem             = IDC_CHECKDIR;

    asLangTexts[iszTextMenuShotSave].lpszText           = TEXT("Shot and Sa&ve...");
    asLangTexts[iszTextMenuLoad].lpszText               = TEXT("Loa&d...");

    asLangTexts[iszTextButtonClearAll].lpszText         = TEXT("&Clear all");
    asLangTexts[iszTextButtonClearAll].nIDDialog = DLG_MAIN;
    asLangTexts[iszTextButtonClearAll].nIDDlgItem       = IDC_CLEARALL;

    asLangTexts[iszTextMenuClearShot1].lpszText         = TEXT("Clear &1st shot");
    asLangTexts[iszTextMenuClearShot2].lpszText         = TEXT("Clear &2nd shot");

    asLangTexts[iszTextMenuSave].lpszText               = TEXT("S&ave...");
    asLangTexts[iszTextMenuInfo].lpszText               = TEXT("&Info");
    asLangTexts[iszTextMenuSwap].lpszText               = TEXT("S&wap");

    asLangTexts[iszTextMenuCompareOutput].lpszText      = TEXT("Com&pare and Output");
    asLangTexts[iszTextMenuCompareCompareOutputFS].lpszText   = TEXT("Compare a&nd Output (only filesystem)");
    asLangTexts[iszTextMenuCompareCompareOutputReg].lpszText  = TEXT("Compare a&nd Output (only registry)");
    asLangTexts[iszTextMenuShotCompareOutput].lpszText  = TEXT("Shot, Compare a&nd Output");
    asLangTexts[iszTextMenuOutput].lpszText             = TEXT("Ou&tput");
    asLangTexts[iszTextMenuOutputFS].lpszText           = TEXT("Ou&tput (only filesystem)");
    asLangTexts[iszTextMenuOutputReg].lpszText          = TEXT("Ou&tput (only registry)");

    asLangTexts[iszTextLoadedFromFile].lpszText         = TEXT("Loaded from file");

    asLangTexts[iszTextWarning].lpszText                = TEXT("Warning");
    asLangTexts[iszTextChronology].lpszText             = TEXT("Chronology");
    asLangTexts[iszTextShotsNotChronological].lpszText  = TEXT("Shots not in chronological order.");
    asLangTexts[iszTextQuestionSwapShots].lpszText      = TEXT("Do you want to swap?");

    asLangTexts[iszTextGroupboxTitle].lpszText          = TEXT("Title/Subdirectory");
    asLangTexts[iszTextGroupboxTitle].nIDDialog         = DLG_MAIN;
    asLangTexts[iszTextGroupboxTitle].nIDDlgItem        = IDC_STATIC_TITLE;
    asLangTexts[iszTextGroupboxRegs].lpszText           = TEXT("Registry-Rootkeys");
    asLangTexts[iszTextGroupboxRegs].nIDDialog          = DLG_MAIN;
    asLangTexts[iszTextGroupboxRegs].nIDDlgItem         = IDC_STATIC_REGS;
    asLangTexts[iszTextGroupboxDirs].lpszText           = TEXT("Directories");
    asLangTexts[iszTextGroupboxDirs].nIDDialog          = DLG_MAIN;
    asLangTexts[iszTextGroupboxDirs].nIDDlgItem         = IDC_STATIC_DIRS;
    asLangTexts[iszTextRadioUNL].lpszText               = TEXT("UNL");
    asLangTexts[iszTextRadioUNL].nIDDialog              = DLG_MAIN;
    asLangTexts[iszTextRadioUNL].nIDDlgItem             = IDC_CHECK_UNL;
    asLangTexts[iszTextCheckboxRegIns].lpszText         = TEXT("REG-File(Install)");
    asLangTexts[iszTextCheckboxRegIns].nIDDialog        = DLG_MAIN;
    asLangTexts[iszTextCheckboxRegIns].nIDDlgItem       = IDC_CHECK_REGINS;
    asLangTexts[iszTextCheckboxRegDel].lpszText         = TEXT("REG-File(Delete)");
    asLangTexts[iszTextCheckboxRegDel].nIDDialog        = DLG_MAIN;
    asLangTexts[iszTextCheckboxRegDel].nIDDlgItem       = IDC_CHECK_REGDEL;
    asLangTexts[iszTextCheckOnlyAddedParts].lpszText    = TEXT("Log only added parts");
    asLangTexts[iszTextCheckOnlyAddedParts].nIDDialog   = DLG_MAIN;
    asLangTexts[iszTextCheckOnlyAddedParts].nIDDlgItem  = IDC_CHECK_ONLYADDED;
    asLangTexts[iszTextCheckNoDeletedEntries].lpszText    = TEXT("Don't log deleted parts");
    asLangTexts[iszTextCheckNoDeletedEntries].nIDDialog   = DLG_MAIN;
    asLangTexts[iszTextCheckNoDeletedEntries].nIDDlgItem  = IDC_CHECK_NOTDELETED;
    asLangTexts[iszTextCheckNoFilters].lpszText         = TEXT("(*) Next comparison w/o filter");
    asLangTexts[iszTextCheckNoFilters].nIDDialog        = DLG_MAIN;
    asLangTexts[iszTextCheckNoFilters].nIDDlgItem       = IDC_CHECK_NOFILTERS;
    asLangTexts[iszTextCheckResult].lpszText            = TEXT("Check result before output");
    asLangTexts[iszTextCheckResult].nIDDialog           = DLG_MAIN;
    asLangTexts[iszTextCheckResult].nIDDlgItem          = IDC_CHECK_RESULT;
    asLangTexts[iszTextCheckResetFilters].lpszText      = TEXT("(*) Reset adhoc filters");
    asLangTexts[iszTextCheckResetFilters].nIDDialog     = DLG_MAIN;
    asLangTexts[iszTextCheckResetFilters].nIDDlgItem    = IDC_CHECK_RESETADHOC;
    asLangTexts[iszTextCheckDeleteShots].lpszText       = TEXT("(*) Delete shot files");
    asLangTexts[iszTextCheckDeleteShots].nIDDialog      = DLG_MAIN;
    asLangTexts[iszTextCheckDeleteShots].nIDDlgItem     = IDC_CHECK_DELSHOTS;
    asLangTexts[iszTextCheckDefaultActions].lpszText    = TEXT("Default actions");
    asLangTexts[iszTextCheckDefaultActions].nIDDialog   = DLG_MAIN;
    asLangTexts[iszTextCheckDefaultActions].nIDDlgItem  = IDC_CHECK_DEFAULTS;
    asLangTexts[iszTextCheckAutocompare].lpszText       = TEXT("Automatic compares");
    asLangTexts[iszTextCheckAutocompare].nIDDialog      = DLG_MAIN;
    asLangTexts[iszTextCheckAutocompare].nIDDlgItem     = IDC_CHECK_AUTOCOMPARE;
    asLangTexts[iszTextCheckNoVals].lpszText            = TEXT("Don't log registry data");
    asLangTexts[iszTextCheckNoVals].nIDDialog           = DLG_MAIN;
    asLangTexts[iszTextCheckNoVals].nIDDlgItem          = IDC_CHECK_NOVALS;
    asLangTexts[iszTextCheckUNLOrder].lpszText          = TEXT("Log with old \"unl order\"");
    asLangTexts[iszTextCheckUNLOrder].nIDDialog         = DLG_MAIN;
    asLangTexts[iszTextCheckUNLOrder].nIDDlgItem        = IDC_CHECK_OPT_UNLORDER;
    asLangTexts[iszTextCheckOpenEditor].lpszText        = TEXT("Open result in default editor");
    asLangTexts[iszTextCheckOpenEditor].nIDDialog       = DLG_MAIN;
    asLangTexts[iszTextCheckOpenEditor].nIDDlgItem      = IDC_CHECK_OPENEDITOR;
    asLangTexts[iszTextNotSaved].lpszText               = TEXT("(*) Not saved to ini");
    asLangTexts[iszTextNotSaved].nIDDialog              = DLG_MAIN;
    asLangTexts[iszTextNotSaved].nIDDlgItem             = IDC_TEXT_REMARK;
    asLangTexts[iszTextMaxOutputLines].lpszText         = TEXT("Maximum output lines per value");
    asLangTexts[iszTextMaxOutputLines].nIDDialog        = DLG_MAIN;
    asLangTexts[iszTextMaxOutputLines].nIDDlgItem       = IDC_STATIC_OUTPUTLINES;
    
    asLangTexts[iszTextButtonLogs].lpszText             = TEXT("Logs");
    asLangTexts[iszTextButtonLogs].nIDDialog            = DLG_MAIN;
    asLangTexts[iszTextButtonLogs].nIDDlgItem           = IDC_PROP_LOGS;
    asLangTexts[iszTextButtonDirs].lpszText             = TEXT("Dirs");
    asLangTexts[iszTextButtonDirs].nIDDialog            = DLG_MAIN;
    asLangTexts[iszTextButtonDirs].nIDDlgItem           = IDC_PROP_DIRS;
    asLangTexts[iszTextButtonRegs].lpszText             = TEXT("Regs");
    asLangTexts[iszTextButtonRegs].nIDDialog            = DLG_MAIN;
    asLangTexts[iszTextButtonRegs].nIDDlgItem           = IDC_PROP_REGS;
    asLangTexts[iszTextButtonScans].lpszText            = TEXT("Scans");
    asLangTexts[iszTextButtonScans].nIDDialog           = DLG_MAIN;
    asLangTexts[iszTextButtonScans].nIDDlgItem          = IDC_PROP_SCANS;
    asLangTexts[iszTextButtonLoadIni].lpszText          = TEXT("Load");
    asLangTexts[iszTextButtonLoadIni].nIDDialog         = DLG_MAIN;
    asLangTexts[iszTextButtonLoadIni].nIDDlgItem        = IDC_LOAD_INI;
    asLangTexts[iszTextButtonSaveIni].lpszText          = TEXT("Save");
    asLangTexts[iszTextButtonSaveIni].nIDDialog         = DLG_MAIN;
    asLangTexts[iszTextButtonSaveIni].nIDDlgItem        = IDC_SAVE_INI;
    asLangTexts[iszTextButtonClear1].lpszText           = TEXT("Clear1");
    asLangTexts[iszTextButtonClear1].nIDDialog          = DLG_MAIN;
    asLangTexts[iszTextButtonClear1].nIDDlgItem         = IDC_PROP_CLEAR1;
    asLangTexts[iszTextButtonClear2].lpszText           = TEXT("Clear2");
    asLangTexts[iszTextButtonClear2].nIDDialog          = DLG_MAIN;
    asLangTexts[iszTextButtonClear2].nIDDlgItem         = IDC_PROP_CLEAR2;
    
    asLangTexts[iszTextMenuFile].lpszText               = TEXT("File");
    asLangTexts[iszTextMenuOpen].lpszText               = TEXT("Open");
    asLangTexts[iszTextMenuShot].lpszText               = TEXT("&Shot");
    asLangTexts[iszTextMenuOptions].lpszText            = TEXT("Options");
    asLangTexts[iszTextMenuHelp].lpszText               = TEXT("Help");
    asLangTexts[iszTextButtonStop].lpszText             = TEXT("Stop");
    asLangTexts[iszTextMenuOptions].lpszText            = TEXT("Options");
    
    // Set language and "translator"
    _tcscpy(lpszLanguage, lpszDefaultLanguage);
    lpszCurrentTranslator = lpszOriginal;
}

// ----------------------------------------------------------------------
// Get selected language name
// ----------------------------------------------------------------------
VOID LoadLanguageFromIni()
{
    LPTSTR lpszSelectedLanguage;
    DWORD cchLanguageName;
    //LRESULT nResult;

    lpszSelectedLanguage = MYALLOC0((cchMaxLanguageNameLen) * sizeof(TCHAR));
    cchLanguageName = GetPrivateProfileString(lpszIniSetup, lpszIniLanguage, NULL, lpszSelectedLanguage, (DWORD)(cchMaxLanguageNameLen), lpszRegshotIni);  // length incl. NULL character
    if (0 >= cchLanguageName) {  // not found or empty in regshot ini, therefore try old (<1.9.0) setting in language ini
        cchLanguageName = GetPrivateProfileString(lpszSectionCurrent, lpszSectionCurrent, NULL, lpszSelectedLanguage, (DWORD)(cchMaxLanguageNameLen), lpszLanguageIni);  // length incl. NULL character
    }

    if (lpszSelectedLanguage != NULL) {
        _tcscpy(lpszLanguage, lpszSelectedLanguage);
        MYFREE(lpszSelectedLanguage);
    }

    return;
}

// ----------------------------------------------------------------------
// Set text strings to selected language
// ----------------------------------------------------------------------
VOID SetTextsToSelectedLanguage(HWND hDlg, BOOL bGetComboboxSelection)
{
    LRESULT nResult, nResult2;
    DWORD cchSection;
    int i;
    LPTSTR lpszMatchValue;
    TCHAR  szIniKey[17];
    BOOL fUseIni;

// Use language index ini
    if (bGetComboboxSelection) {
// Get language index from combo box
        nResult = SendDlgItemMessage(hDlg, IDC_COMBOLANGUAGE, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
        if (CB_ERR == nResult) {
            return;
        }

        // Get language name of language index from combo box
        nResult2 = SendDlgItemMessage(hDlg, IDC_COMBOLANGUAGE, CB_GETLBTEXTLEN, (WPARAM)nResult, (LPARAM)NULL);
        if ((CB_ERR == nResult2) || ((size_t)nResult2 >= cchMaxLanguageNameLen)) {  // cchMaxLanguageNameLen incl. NULL character
            return;
        }
        nResult = SendDlgItemMessage(hDlg, IDC_COMBOLANGUAGE, CB_GETLBTEXT, (WPARAM)nResult, (LPARAM)lpszLanguage);
        if (CB_ERR == nResult) {
            return;
        }
    }

    // Get ini section of language
    if (NULL == lpgrszLangSection) {
        lpgrszLangSection = MYALLOC0(MAX_INI_SECTION_CHARS * sizeof(TCHAR));  // holds the selected translation until the end of the program
    }
    cchSection = GetPrivateProfileSection(lpszLanguage, lpgrszLangSection, MAX_INI_SECTION_CHARS, lpszLanguageIni);  // length incl. double NULL character

    // Ignore translation for default language, but continue to update GUI
    fUseIni = TRUE;
    if (0 == _tcsicmp(lpszLanguage, lpszDefaultLanguage)) {
        fUseIni = FALSE;
    }

    // Find language strings and assign if not empty
    for (i = 0; i < cLangStrings; i++) {
        if (fUseIni) {
            _sntprintf(szIniKey, 17, TEXT("%u%s\0"), (i + 1), TEXT("="));
            szIniKey[16] = (TCHAR)'\0';  // saftey NULL char
            lpszMatchValue = FindKeyInIniSection(lpgrszLangSection, szIniKey, cchSection, _tcslen(szIniKey));
            if (NULL != lpszMatchValue) {
                // pointer returned points to char directly after equal char ("="), and is not empty
                asLangTexts[i].lpszText = lpszMatchValue;
            }
        }

        // Update gui text with language string if id provided
        if ((0 != asLangTexts[i].nIDDlgItem)) {
            if (asLangTexts[i].nIDDialog == DLG_MAIN)
                SetDlgItemText(hMainWnd, asLangTexts[i].nIDDlgItem, asLangTexts[i].lpszText);
        }
    }
    lpszStop = asLangTexts[iszTextButtonStop].lpszText;
    lpszQuit = asLangTexts[iszTextButtonQuit].lpszText;

    // Get translator's name
    if (fUseIni) {
        lpszMatchValue = FindKeyInIniSection(lpgrszLangSection, lpszItemTranslator, cchSection, _tcslen(lpszItemTranslator));
        if (NULL != lpszMatchValue) {
            lpszCurrentTranslator = lpszMatchValue;
        } else {
            lpszCurrentTranslator = lpszOriginal;
        }
    }


    ModifyMenu(hMainMenu, 0, MF_STRING | MF_BYPOSITION, 0, asLangTexts[iszTextMenuFile].lpszText);
    ModifyMenu(hMainMenu, 1, MF_STRING | MF_BYPOSITION, 1, asLangTexts[iszTextButtonShot1].lpszText);
    ModifyMenu(hMainMenu, 2, MF_STRING | MF_BYPOSITION, 2, asLangTexts[iszTextButtonShot2].lpszText);
    ModifyMenu(hMainMenu, 3, MF_STRING | MF_BYPOSITION, 3, asLangTexts[iszTextButtonCompare].lpszText);
    ModifyMenu(hMainMenu, ID_SETTINGS, MF_STRING | MF_BYCOMMAND, ID_SETTINGS, asLangTexts[iszTextMenuOptions].lpszText);
    MENUITEMINFO mii;
    mii.cbSize = sizeof(MENUITEMINFO);
    GetMenuItemInfo(hMainMenu, 5, TRUE, &mii);
    mii.fMask = MIIM_TYPE;
    mii.dwTypeData = asLangTexts[iszTextMenuHelp].lpszText;
    mii.fType = MFT_STRING | MFT_RIGHTJUSTIFY;
    SetMenuItemInfo(hMainMenu, 5, TRUE, &mii);


    ModifyMenu(hMainMenu, ID_FILE_OPEN, MF_STRING | MF_BYCOMMAND, ID_FILE_OPEN, asLangTexts[iszTextMenuOpen].lpszText);
    ModifyMenu(hMainMenu, ID_FILE_OPTIONS, MF_STRING | MF_BYCOMMAND, ID_FILE_OPTIONS, asLangTexts[iszTextMenuOptions].lpszText);
    ModifyMenu(hMainMenu, ID_FILE_CLEARALL, MF_STRING | MF_BYCOMMAND, ID_FILE_CLEARALL, asLangTexts[iszTextButtonClearAll].lpszText);
    ModifyMenu(hMainMenu, ID_FILE_QUIT, MF_STRING | MF_BYCOMMAND, ID_FILE_QUIT, asLangTexts[iszTextButtonQuit].lpszText);
    
    ModifyMenu(hMainMenu, ID_1_SHOT, MF_STRING | MF_BYCOMMAND, ID_1_SHOT, asLangTexts[iszTextMenuShot].lpszText);
    ModifyMenu(hMainMenu, ID_1_SHOTANDSAVE, MF_STRING | MF_BYCOMMAND, ID_1_SHOTANDSAVE, asLangTexts[iszTextMenuShotSave].lpszText);
    ModifyMenu(hMainMenu, ID_1_LOAD, MF_STRING | MF_BYCOMMAND, ID_1_LOAD, asLangTexts[iszTextMenuLoad].lpszText);
    ModifyMenu(hMainMenu, ID_1_SAVE, MF_STRING | MF_BYCOMMAND, ID_1_SAVE, asLangTexts[iszTextMenuSave].lpszText);
    ModifyMenu(hMainMenu, ID_1_CLEAR, MF_STRING | MF_BYCOMMAND, ID_1_CLEAR, asLangTexts[iszTextMenuClear].lpszText);
    ModifyMenu(hMainMenu, ID_1_INFO, MF_STRING | MF_BYCOMMAND, ID_1_INFO, asLangTexts[iszTextMenuInfo].lpszText);
    ModifyMenu(hMainMenu, ID_1_SWAP, MF_STRING | MF_BYCOMMAND, ID_1_SWAP, asLangTexts[iszTextMenuSwap].lpszText);
    
    ModifyMenu(hMainMenu, ID_2_SHOTCOMPAREOUTPUT, MF_STRING | MF_BYCOMMAND, ID_2_SHOTCOMPAREOUTPUT, asLangTexts[iszTextMenuShotCompareOutput].lpszText);
    ModifyMenu(hMainMenu, ID_2_SHOTCOMPAREOUTPUT, MF_STRING | MF_BYCOMMAND, ID_2_SHOTCOMPAREOUTPUT, asLangTexts[iszTextMenuShotCompareOutput].lpszText);
    ModifyMenu(hMainMenu, ID_2_SHOT, MF_STRING | MF_BYCOMMAND, ID_2_SHOT, asLangTexts[iszTextMenuShot].lpszText);
    ModifyMenu(hMainMenu, ID_2_SHOTANDSAVE, MF_STRING | MF_BYCOMMAND, ID_2_SHOTANDSAVE, asLangTexts[iszTextMenuShotSave].lpszText);
    ModifyMenu(hMainMenu, ID_2_LOAD, MF_STRING | MF_BYCOMMAND, ID_2_LOAD, asLangTexts[iszTextMenuLoad].lpszText);
    ModifyMenu(hMainMenu, ID_2_SAVE, MF_STRING | MF_BYCOMMAND, ID_2_SAVE, asLangTexts[iszTextMenuSave].lpszText);
    ModifyMenu(hMainMenu, ID_2_CLEAR, MF_STRING | MF_BYCOMMAND, ID_2_CLEAR, asLangTexts[iszTextMenuClear].lpszText);
    ModifyMenu(hMainMenu, ID_2_INFO, MF_STRING | MF_BYCOMMAND, ID_2_INFO, asLangTexts[iszTextMenuInfo].lpszText);
    ModifyMenu(hMainMenu, ID_2_SWAP, MF_STRING | MF_BYCOMMAND, ID_2_SWAP, asLangTexts[iszTextMenuSwap].lpszText);

    ModifyMenu(hMainMenu, ID_COMPARE_COMPARE, MF_STRING | MF_BYCOMMAND, ID_COMPARE_COMPARE, asLangTexts[iszTextButtonCompare].lpszText);
    ModifyMenu(hMainMenu, ID_COMPARE_COMPAREANDOUTPUT, MF_STRING | MF_BYCOMMAND, ID_COMPARE_COMPAREANDOUTPUT, asLangTexts[iszTextMenuCompareOutput].lpszText);
    ModifyMenu(hMainMenu, ID_COMPARE_COMPAREANDOUTPUT_FS, MF_STRING | MF_BYCOMMAND, ID_COMPARE_COMPAREANDOUTPUT_FS, asLangTexts[iszTextMenuCompareCompareOutputFS].lpszText);
    ModifyMenu(hMainMenu, ID_COMPARE_COMPAREANDOUTPUT_REG, MF_STRING | MF_BYCOMMAND, ID_COMPARE_COMPAREANDOUTPUT_REG, asLangTexts[iszTextMenuCompareCompareOutputReg].lpszText);
    ModifyMenu(hMainMenu, ID_COMPARE_OUTPUT, MF_STRING | MF_BYCOMMAND, ID_COMPARE_OUTPUT, asLangTexts[iszTextMenuOutput].lpszText);
    ModifyMenu(hMainMenu, ID_COMPARE_OUTPUT_FS, MF_STRING | MF_BYCOMMAND, ID_COMPARE_OUTPUT_FS, asLangTexts[iszTextMenuOutputFS].lpszText);
    ModifyMenu(hMainMenu, ID_COMPARE_OUTPUT_REG, MF_STRING | MF_BYCOMMAND, ID_COMPARE_OUTPUT_REG, asLangTexts[iszTextMenuOutputReg].lpszText);
    ModifyMenu(hMainMenu, ID_COMPARE_CLEAR, MF_STRING | MF_BYCOMMAND, ID_COMPARE_CLEAR, asLangTexts[iszTextMenuClear].lpszText);
    ModifyMenu(hMainMenu, ID_COMPARE_INFO, MF_STRING | MF_BYCOMMAND, ID_COMPARE_INFO, asLangTexts[iszTextMenuInfo].lpszText);

    ModifyMenu(hMainMenu, ID_HELP_ABOUT, MF_STRING | MF_BYCOMMAND, ID_HELP_ABOUT, asLangTexts[iszTextButtonAbout].lpszText);

    DrawMenuBar(hMainWnd);
    UpdateWindow(hMainWnd);
}

// ----------------------------------------------------------------------
// Get available languages from language ini and add to combo box
// An English section in language ini will be ignored
// ----------------------------------------------------------------------
VOID FillComboboxWithLanguagesFromIni(HWND hDlg)
{
    LRESULT nResult;
    LPTSTR lpgrszSectionNames;
    DWORD cchSectionNames;
    size_t i;
    size_t nLanguageNameLen;

    // Always add default language to combo box and select it as default
    nResult = SendDlgItemMessage(hDlg, IDC_COMBOLANGUAGE, CB_ADDSTRING, (WPARAM)0, (LPARAM)lpszDefaultLanguage);  // TODO: handle CB_ERR and CB_ERRSPACE
    SendDlgItemMessage(hDlg, IDC_COMBOLANGUAGE, CB_SETCURSEL, (WPARAM)nResult, (LPARAM)0);

    // Get sections (=language names) from language ini
    lpgrszSectionNames = MYALLOC0(MAX_INI_SECTION_CHARS * sizeof(TCHAR));
    if (lpgrszSectionNames == NULL)
        return;
    cchSectionNames = GetPrivateProfileSectionNames(lpgrszSectionNames, MAX_INI_SECTION_CHARS, lpszLanguageIni);  // length incl. double NULL character
    if (1 < cchSectionNames) {
        for (i = 0; i < cchSectionNames;) {
            if (0 == lpgrszSectionNames[i]) {  // reached the end of the section names buffer
                break;
            }

            nLanguageNameLen = _tcslen(&lpgrszSectionNames[i]) + 1;  // incl. NULL character
            if ((0 != _tcsicmp(&lpgrszSectionNames[i], lpszSectionCurrent)) && (0 != _tcsicmp(&lpgrszSectionNames[i], lpszDefaultLanguage))) {
                nResult = SendDlgItemMessage(hDlg, IDC_COMBOLANGUAGE, CB_ADDSTRING, (WPARAM)0, (LPARAM)&lpgrszSectionNames[i]);  // TODO: handle CB_ERR and CB_ERRSPACE
            }
            i += nLanguageNameLen;  // skip to next string
        }
    }
    MYFREE(lpgrszSectionNames);
    
    nResult = SendDlgItemMessage(hDlg, IDC_COMBOLANGUAGE, CB_FINDSTRINGEXACT, (WPARAM)0, (LPARAM)lpszLanguage);
    if (CB_ERR != nResult) {
        SendDlgItemMessage(hDlg, IDC_COMBOLANGUAGE, CB_SETCURSEL, (WPARAM)nResult, (LPARAM)0);
    }
}

