/*
    Copyright 2021-     Patrick (skydive241@gmx.de)
    Copyright 2011-2015 Regshot Team
    Copyright 1999-2003,2007,2011 TiANWEi
    Copyright 2004 tulipfan

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

#ifndef REGSHOT_GLOBAL_H
#define REGSHOT_GLOBAL_H


#ifdef __GNUC__
#include <unistd.h>
#endif
#include <windows.h>
#include <stdio.h>
#include <shlobj.h>
#include <stddef.h>  // for "offsetof" macro
#include <string.h>
#include <tchar.h>
#include <assert.h>  // for assert()
#include "resource.h"

#include <shlwapi.h>
#include <windowsx.h>
#include <sddl.h>
#include <LMCons.h>  // for UNLEN define
#include <process.h>

#if defined(_MSC_VER) && (_MSC_VER < 1300)  // before VS 2002 .NET (e.g. VS 6), may depend on PSDK/WSDK too
#ifndef SetClassLongPtr
#define SetClassLongPtr SetClassLong
#endif
#ifndef GCLP_HICON
#define GCLP_HICON GCL_HICON
#endif
#if !defined(LONG_PTR)
typedef long LONG_PTR;
#endif
#ifndef REG_QWORD_LITTLE_ENDIAN
#define REG_QWORD_LITTLE_ENDIAN     ( 11 )  // 64-bit number
#endif
#endif  // _MSC_VER && (_MSC_VER < 1300)

#if defined(_MSC_VER) && (_MSC_VER < 1400)  // before VS 2005 (e.g. VS 6)
#   define REPLACEMENT_STRNLEN
size_t strnlen(const char *lpszText, size_t cchMax);
#endif  // _MSC_VER

#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#   if (defined(_M_IA64) || defined(__ia64__) || defined(_M_AMD64) || defined(_M_X64) || defined(__amd64__) || defined(_M_IX86) || defined(_X86_) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__))
#       define __LITTLE_ENDIAN__ 1
#   else
#       error "Cannot determine Endianness"
#   endif
#endif

//
//  Define QWORD -- not yet defined globally (in WinDef.h)
//
#ifndef QWORD
typedef unsigned __int64 QWORD, NEAR *PQWORD, FAR *LPQWORD;
#endif //QWORD

// added in 1.8.2 to gain a slightly faster speed but it is danger!
#define USEHEAPALLOC_DANGER

#ifdef USEHEAPALLOC_DANGER

extern HANDLE hHeap;

// MSDN doc say use HEAP_NO_SERIALIZE is not good for process heap :( so change fromm 1 to 0 20111216, slower than using 1
#define MYALLOC(x)  HeapAlloc(hHeap,0,x)
#define MYALLOC0(x) HeapAlloc(hHeap,8,x) // (HEAP_NO_SERIALIZE|) HEAP_ZERO_MEMORY ,1|8
#define MYFREE(x)   \
if (x != NULL)\
   HeapFree(hHeap,0,x)
#define MYREALLOC0(x,lpMem) HeapReAlloc(hHeap,8,lpMem,x) // (HEAP_NO_SERIALIZE|) HEAP_ZERO_MEMORY ,1|8

#else

#define MYALLOC(x)  GlobalAlloc(GMEM_FIXED,x) // GMEM_FIXED, 0
#define MYALLOC0(x) GlobalAlloc(GPTR,x) // Combines GMEM_FIXED and GMEM_ZEROINIT GPTR, 0|0x0040
#define MYREALLOC0(x,hMem) GlobalReAlloc(hMem,x,GPTR) // (HEAP_NO_SERIALIZE|) HEAP_ZERO_MEMORY ,1|8
#define MYFREE(x)   GlobalFree(x)

#endif

// Some definitions
#define NOMATCH         0             // Define modification type in comparison results
#define ISMATCH         1
#define ISDEL           2
#define ISADD           3
#define ISMODI          4

#define KEYDEL          1
#define KEYADD          2
#define VALDEL          3
#define VALADD          4
#define VALMODI         5
#define DIRDEL          6
#define DIRADD          7
#define DIRMODI         8
#define FILEDEL         9
#define FILEADD         10
#define FILEMODI        11
#define ALLCHANGES      12

#define ESTIMATE_VALUEDATA_LENGTH 1024*1024 // Define estimated value data in scan
#define REFRESHINTERVAL 110          // Define progress refresh rate
#define MAXPBPOSITION   128          // Define progress bar length
#define COMMENTLENGTH   51           // Define commentfield length on the MainForm in TCHARs incl. NULL
#define HTMLWRAPLENGTH  1000         // Define html output wrap length
#define MAXAMOUNTOFFILE 10000        // Define output file counts
#define EXTDIRLEN       MAX_PATH * 4 // Define searching directory field length in TCHARs (MAX_PATH includes NULL)
#define OLD_COMPUTERNAMELEN 64       // Define COMPUTER name length, do not change

#define PROP_COMMON   0
#define PROP_COMMON_2 1
#define PROP_SCANS    2
#define PROP_WLREGS   3
#define PROP_REGS     4
#define PROP_DIRS     5
#define PROP_FILES    6

#define PROP_TVREGS   0
#define PROP_TVDIRS   1

// Some definitions of MutiLanguage strings [Free space length]
#define SIZEOF_LANGUAGE_SECTIONNAMES_BUFFER 2048
#define SIZEOF_SINGLE_LANGUAGENAME          64
#define MAX_INI_SECTION_CHARS               32767
#define SIZEOF_ABOUTBOX                     4096
#define SIZEOF_INFOBOX                      4096
#define SIZEOF_CHRONOBOX                    4096
#define SIZEOF_SAVINGBOX                    4096
//#define MAX_RESULT_STRINGS                  10
#define SIZEOF_RESULT_DATA                  512


// Struct used for counts of keys, values, dirs and files
struct _COUNTS {
    DWORD cAll;
    DWORD cKeys;
    DWORD cValues;
    DWORD cDirs;
    DWORD cFiles;
};
typedef struct _COUNTS COUNTS, FAR *LPCOUNTS;


// Struct used for Windows Registry Value
struct _VALUECONTENT {
    DWORD  nTypeCode;                       // Type of value [DWORD,STRING...]
    DWORD  cbData;                          // Value data size in bytes
    LPTSTR lpszValueName;                   // Pointer to value's name
    size_t cchValueName;                    // Length of value's name in chars
    LPBYTE lpValueData;                     // Pointer to value's data
    struct _VALUECONTENT FAR *lpBrotherVC;  // Pointer to value's brother
    struct _KEYCONTENT FAR *lpFatherKC;     // Pointer to value's father key
    DWORD  fValueMatch;                     // Flags used when comparing, until 1.8.2 was byte
};
typedef struct _VALUECONTENT VALUECONTENT, FAR *LPVALUECONTENT;


// Struct used for Windows Registry Key
struct _KEYCONTENT {
    LPTSTR lpszKeyName;                     // Pointer to key's name
    size_t cchKeyName;                      // Length of key's name in chars
    LPVALUECONTENT lpFirstVC;               // Pointer to key's first value
    struct _KEYCONTENT FAR *lpFirstSubKC;   // Pointer to key's first sub key
    struct _KEYCONTENT FAR *lpBrotherKC;    // Pointer to key's brother
    struct _KEYCONTENT FAR *lpFatherKC;     // Pointer to key's father
    DWORD  fKeyMatch;                       // Flags used when comparing, until 1.8.2 was byte
};
typedef struct _KEYCONTENT KEYCONTENT, FAR *LPKEYCONTENT;


// Struct used for Windows File System
struct _FILECONTENT {
    LPTSTR lpszFileName;                    // Pointer to file's name
    size_t cchFileName;                     // Length of file's name in chars
    DWORD  nWriteDateTimeLow;               // File write time [LOW  DWORD]
    DWORD  nWriteDateTimeHigh;              // File write time [HIGH DWORD]
    DWORD  nFileSizeLow;                    // File size [LOW  DWORD]
    DWORD  nFileSizeHigh;                   // File size [HIGH DWORD]
    DWORD  nFileAttributes;                 // File attributes (e.g. directory)
    DWORD  nChkSum;                         // File checksum (planned for the future, currently not used)
    struct _FILECONTENT FAR *lpFirstSubFC;  // Pointer to file's first sub file
    struct _FILECONTENT FAR *lpBrotherFC;   // Pointer to file's brother
    struct _FILECONTENT FAR *lpFatherFC;    // Pointer to file's father
    DWORD  fFileMatch;                      // Flags used when comparing, until 1.8.2 it was byte
};
typedef struct _FILECONTENT FILECONTENT, FAR *LPFILECONTENT;


// Adjusted for filecontent saving. 1.8
struct _HEADFILE {
    LPFILECONTENT lpFirstFC;                // Pointer to head file's first file
    struct _HEADFILE FAR *lpBrotherHF;      // Pointer to head file's brother
    DWORD  fHeadFileMatch;                  // Flags used when comparing
};
typedef struct  _HEADFILE HEADFILE, FAR *LPHEADFILE;


// Struct for shot,2012.
struct _REGSHOT {
    LPKEYCONTENT  lpHKLM;                   // Pointer to Shot's HKLM registry keys
    LPKEYCONTENT  lpHKU;                    // Pointer to Shot's HKU registry keys
    LPKEYCONTENT  lpHKCU;                   // Pointer to Shot's HKLM registry keys
    LPHEADFILE    lpHF;                     // Pointer to Shot's head files
    LPTSTR        lpszComputerName;         // Pointer to Shot's computer name
    LPTSTR        lpszUserName;             // Pointer to Shot's user name
    SYSTEMTIME    systemtime;
    COUNTS        stCounts;
    BOOL          fFilled;                  // Flag if Shot was done/loaded (even if result is empty)
    BOOL          fLoaded;                  // Flag if Shot was loaded from a file
    BOOL          fFirst;                   // Flag which shot it is
//    ULONGLONG     duration;
    LPTSTR        lpszFileName;             // Pointer to Shot's file name
    LPTSTR        lpszTitle;                // Pointer to Shot's title
};
typedef struct _REGSHOT REGSHOT, FAR *LPREGSHOT;


// Struct for a single compare result list
struct _COMPRESULT {
    LPVOID lpContentOld;                    // Pointer to old content
    LPVOID lpContentNew;                    // Pointer to new content
    struct _COMPRESULT FAR *lpNextCR;       // Pointer to next _COMPRESULT
};
typedef struct _COMPRESULT COMPRESULT, FAR *LPCOMPRESULT;

// Struct for a single compare result list
struct _COMPRESULTNEW {
    DWORD nActionType;
    LPVOID lpContentOld;                    // Pointer to old content
    LPVOID lpContentNew;                    // Pointer to new content
    struct _COMPRESULTNEW FAR *lpPrevCRNew;       // Pointer to previous _COMPRESULTNEW
    struct _COMPRESULTNEW FAR *lpNextCRNew;       // Pointer to next _COMPRESULTNEW
};
typedef struct _COMPRESULTNEW COMPRESULTNEW, FAR *LPCOMPRESULTNEW;


// Struct for all compare result lists
struct _COMPPOINTERS {
    LPCOMPRESULT  lpCRKeyDeleted;
    LPCOMPRESULT  lpCRKeyAdded;
    LPCOMPRESULT  lpCRValDeleted;
    LPCOMPRESULT  lpCRValAdded;
    LPCOMPRESULT  lpCRValModified;
    LPCOMPRESULT  lpCRDirDeleted;
    LPCOMPRESULT  lpCRDirAdded;
    LPCOMPRESULT  lpCRDirModified;
    LPCOMPRESULT  lpCRFileDeleted;
    LPCOMPRESULT  lpCRFileAdded;
    LPCOMPRESULT  lpCRFileModified;
    LPCOMPRESULTNEW lpCRRegistry;
    LPCOMPRESULTNEW lpCRRegistryLast;
    LPCOMPRESULTNEW lpCRFilesystem;
    LPCOMPRESULTNEW lpCRFilesystemLast;
};
typedef struct _COMPPOINTERS COMPPOINTERS, FAR *LPCOMPPOINTERS;


// Struct for complete compare result
struct _COMPRESULTS {
    LPREGSHOT     lpShot1;
    LPREGSHOT     lpShot2;
    COUNTS        stcCompared;
    COUNTS        stcChanged;
    COUNTS        stcDeleted;
    COUNTS        stcAdded;
    COUNTS        stcModified;
    COMPPOINTERS  stCRHeads;
    COMPPOINTERS  stCRCurrent;
    BOOL          fFilled;                  // Flag if comparison was done (even if result is empty)
};
typedef struct _COMPRESULTS COMPRESULTS, FAR *LPCOMPRESULTS;


// Struct for file header, used in saving and loading
// when accessing fields of this structure always put a version check around them, e.g. "if version >= 2 then use ofsComputerName"
struct _FILEHEADER {
    char signature[12]; // ofs 0 len 12: never convert to Unicode, always use char type and SBCS/MBCS
    DWORD nFHSize;      // (v2) ofs 12 len 4: size of file header incl. signature and header size field

    DWORD ofsHKLM;      // ofs 16 len 4
    DWORD ofsHKU;       // ofs 20 len 4
    DWORD ofsHF;        // ofs 24 len 4: added in 1.8
    DWORD reserved;     // ofs 28 len 4

    // next two fields are kept and filled for <= 1.8.2 compatibility, see substituting fields in file header format version 2
    char computername[OLD_COMPUTERNAMELEN];  // ofs 32 len 64: DO NOT CHANGE, keep as SBCS/MBCS, name maybe truncated or missing NULL char
    char username[OLD_COMPUTERNAMELEN];      // ofs 96 len 64: DO NOT CHANGE, keep as SBCS/MBCS, name maybe truncated or missing NULL char

    SYSTEMTIME systemtime;  // ofs 160 len 16

    // extended values exist only since structure version 2
    // new since file header version 2
    DWORD nFHVersion;        // (v2) ofs 176 len 4: File header structure version
    DWORD nCharSize;         // (v2) ofs 180 len 4: sizeof(TCHAR), allows to separate between SBCS/MBCS (1 for char) and Unicode (2 for UTF-16 WCHAR), may become 4 for UTF-32 in the future

    DWORD nKCVersion;        // (v2) ofs 184 len 4: Key content structure version
    DWORD nKCSize;           // (v2) ofs 188 len 4: Size of key content

    DWORD nVCVersion;        // (v2) ofs 192 len 4: Value content structure version
    DWORD nVCSize;           // (v2) ofs 196 len 4: Size of value content

    DWORD nHFVersion;        // (v2) ofs 200 len 4: Head File structure version
    DWORD nHFSize;           // (v2) ofs 204 len 4: Size of file content

    DWORD nFCVersion;        // (v2) ofs 208 len 4: File content structure version
    DWORD nFCSize;           // (v2) ofs 212 len 4: Size of file content

    DWORD ofsComputerName;   // (v2) ofs 216 len 4
    DWORD nComputerNameLen;  // (v2) ofs 220 len 4: length in chars incl. NULL char

    DWORD ofsUserName;       // (v2) ofs 224 len 4
    DWORD nUserNameLen;      // (v2) ofs 228 len 4: length in chars incl. NULL char

    DWORD nEndianness;       // (v2) ofs 232 len 4: 0x12345678

    DWORD ofsHKCU;           // (v3) ofs 236 len 4
    DWORD ofsTitle;          // (v3) ofs 240 len 4
    DWORD nTitleLen;         // (v2) ofs 244 len 4: length in chars incl. NULL char
    // ^^^ here the file header structure can be extended
    // * increase the version number for the new file header format
    // * place a comment with a reference to the new version before the new fields
    // * check all usages and version checks
    // * remember that older versions cannot use these additional data
};
typedef struct _FILEHEADER FILEHEADER, FAR *LPFILEHEADER;

#define FILEHEADER_VERSION_EMPTY 0
#define FILEHEADER_VERSION_1 1
#define FILEHEADER_VERSION_2 2
#define FILEHEADER_VERSION_3 3
#define FILEHEADER_VERSION_CURRENT FILEHEADER_VERSION_3

#define FILEHEADER_ENDIANNESS_VALUE 0x12345678

struct _FILEEXTRADATA {
    BOOL bSameCharSize;
    BOOL bSameEndianness;
    BOOL bOldKCVersion;
    BOOL bOldVCVersion;
    BOOL bOldFCVersion;

    size_t cbFBuffer;
    DWORD  ofsFBuffer;
    DWORD  ofsFBFile;
    BOOL   bFBStopAlloc;
};
typedef struct _FILEEXTRADATA FILEEXTRADATA, FAR *LPFILEEXTRADATA;

// Struct for reg key, used in saving and loading
// when accessing fields of this structure always put a version check around them, e.g. "if version >= 2 then use nKeyNameLen"
struct _SAVEKEYCONTENT {
    DWORD ofsKeyName;      // Position of key's name
    DWORD ofsFirstValue;   // Position of key's first value
    DWORD ofsFirstSubKey;  // Position of key's first sub key
    DWORD ofsBrotherKey;   // Position of key's brother key
    DWORD ofsFatherKey;    // Position of key's father key

    // extended values exist only since structure version 2
    // new since key content version 2
    DWORD nKeyNameLen;  // (v2) Length of key's name in chars incl. NULL char (up to 1.8.2 there a was single byte for internal fKeyMatch field that was always zero)

    // ^^^ here the key content structure can be extended
    // * increase the version number for the new key content format
    // * place a comment with a reference to the new version before the new fields
    // * check all usages and version checks
    // * remember that older versions cannot use these additional data
};
typedef struct _SAVEKEYCONTENT SAVEKEYCONTENT, FAR *LPSAVEKEYCONTENT;

#define KEYCONTENT_VERSION_EMPTY 0
#define KEYCONTENT_VERSION_1 1
#define KEYCONTENT_VERSION_2 2
#define KEYCONTENT_VERSION_CURRENT KEYCONTENT_VERSION_2


// Struct for reg value, used in saving and loading
// when accessing fields of this structure always put a version check around them, e.g. "if version >= 2 then use nValueNameLen"
struct _SAVEVALUECONTENT {
    DWORD nTypeCode;        // Type of value [DWORD,STRING...]
    DWORD cbData;           // Value data size in bytes
    DWORD ofsValueName;     // Position of value's name
    DWORD ofsValueData;     // Position of value's data
    DWORD ofsBrotherValue;  // Position of value's brother value
    DWORD ofsFatherKey;     // Position of value's father key

    // extended values exist only since structure version 2
    // new since value content version 2
    DWORD nValueNameLen;  // (v2) Length of value's name in chars incl. NULL char (up to 1.8.2 there a was single byte for internal fValueMatch field that was always zero)

    // ^^^ here the value content structure can be extended
    // * increase the version number for the new value content format
    // * place a comment with a reference to the new version before the new fields
    // * check all usages and version checks
    // * remember that older versions cannot use these additional data
};
typedef struct _SAVEVALUECONTENT SAVEVALUECONTENT, FAR *LPSAVEVALUECONTENT;

#define VALUECONTENT_VERSION_EMPTY 0
#define VALUECONTENT_VERSION_1 1
#define VALUECONTENT_VERSION_2 2
#define VALUECONTENT_VERSION_CURRENT KEYCONTENT_VERSION_2


// Struct for dirs and files, used in saving and loading
// when accessing fields of this structure always put a version check around them, e.g. "if version >= 2 then use nFileNameLen"
struct _SAVEFILECONTENT {
    DWORD ofsFileName;         // Position of file name
    DWORD nWriteDateTimeLow;   // File write time [LOW  DWORD]
    DWORD nWriteDateTimeHigh;  // File write time [HIGH DWORD]
    DWORD nFileSizeLow;        // File size [LOW  DWORD]
    DWORD nFileSizeHigh;       // File size [HIGH DWORD]
    DWORD nFileAttributes;     // File attributes
    DWORD nChkSum;             // File checksum (planned for the future, currently not used)
    DWORD ofsFirstSubFile;     // Position of file's first sub file
    DWORD ofsBrotherFile;      // Position of file's brother file
    DWORD ofsFatherFile;       // Position of file's father file

    // extended values exist only since structure version 2
    // new since file content version 2
    DWORD nFileNameLen;  // (v2) Length of file's name in chars incl. NULL char (up to 1.8.2 there a was single byte for internal fFileMatch field that was always zero)

    // ^^^ here the file content structure can be extended
    // * increase the version number for the new file content format
    // * place a comment with a reference to the new version before the new fields
    // * check all usages and version checks
    // * remember that older versions cannot use these additional data
};
typedef struct _SAVEFILECONTENT SAVEFILECONTENT, FAR *LPSAVEFILECONTENT;

#define FILECONTENT_VERSION_EMPTY 0
#define FILECONTENT_VERSION_1 1
#define FILECONTENT_VERSION_2 2
#define FILECONTENT_VERSION_CURRENT KEYCONTENT_VERSION_2


// Adjusted for filecontent saving. 1.8
struct _SAVEHEADFILE {
    DWORD ofsBrotherHeadFile;   // Position of head file's brother head file
    DWORD ofsFirstFileContent;  // Position of head file's first file content

    // extended values exist only since structure version 2

    // ^^^ here the head file structure can be extended
    // * increase the version number for the new head file format
    // * place a comment with a reference to the new version before the new fields
    // * check all usages and version checks
    // * remember that older versions cannot use these additional data
};
typedef struct  _SAVEHEADFILE SAVEHEADFILE, FAR *LPSAVEHEADFILE;

#define HEADFILE_VERSION_EMPTY 0
#define HEADFILE_VERSION_1 1
#define HEADFILE_VERSION_CURRENT HEADFILE_VERSION_1


// Compare result
extern COMPRESULTS CompareResult;

// Variables used for time intervals to update the "status bar"
extern ULONGLONG nStartTime;
extern ULONGLONG nCurrentTime;
extern ULONGLONG nLastTime;

// Variables used to update the progress bar
extern DWORD cCurrent;
extern DWORD cEnd;

extern DWORD NBW;                // that is: NumberOfBytesWritten

extern REGSHOT Shot1;
extern REGSHOT Shot2;
// Pointers to Registry Key
/*
LPHEADFILE      lpHeadFile1;            // Pointer to headfile
LPHEADFILE      lpHeadFile2;
*/

#define REGSHOT_MESSAGE_LENGTH 256
extern LPTSTR lpszMessage;
extern LPTSTR lpszExtDir;
extern LPTSTR lpszTitle;
extern LPTSTR lpszEditor;
extern LPTSTR lpszOutputPath;
extern LPTSTR lpszLastSaveDir;
extern LPTSTR lpszLastOpenDir;
extern LPTSTR lpszLanguage;
extern LPTSTR lpszLanguageFromIni;
extern LPTSTR lpszWindowsDirName;
extern LPTSTR lpszTempPath;
extern LPTSTR lpszLanguageIni;  // For language.ini
extern LPTSTR lpszCurrentTranslator;
extern LPTSTR lpszRegshotIni;
extern LPTSTR lpszIniSetup;
extern LPTSTR lpszIniOutput;
extern LPTSTR lpszIniStartDefaults;
extern LPTSTR lpszIniOpenEditor;
extern LPTSTR lpszIniLanguage;
extern LPTSTR lpszIniDeleteReadOnly;
extern LPTSTR lpszIniSkipRegKey;
extern LPTSTR lpszIniWhitelistRegKey;
extern LPTSTR lpszIniScanDir;
extern LPTSTR lpszIniSkipDir;
extern LPTSTR lpszIniSkipFile;
extern LPTSTR lpszQuit;
extern LPTSTR lpszStop;
extern LPTSTR lpszISSOutputFolder;
extern LPTSTR lpszISSEditor;
extern LPTSTR lpszNSIOutputFolder;
extern LPTSTR lpszNSIEditor;

struct _SKIPLIST {
    LPTSTR lpSkipString;
    BOOL bSkipKeyActive;
    BOOL bAutomaticallyAdded;
    BOOL bAdhoc;
    BOOL bFile;
    BOOL bDeleted;
};
typedef struct _SKIPLIST SKIPLIST;

extern SKIPLIST* pRegSkipList;
extern SKIPLIST* pRegWhiteList;
extern SKIPLIST* pFileSkipList;
extern SKIPLIST* pDirScanList;

extern LPTSTR lpgrszLangSection;

BOOL bSuppressNotifications;
LPTSTR lpszUIBuffer;

// Former definitions used at Dynamic Monitor Engine. Not Used NOW
//extern BOOL  bWinNTDetected;

extern MSG       msg;              // Windows MSG struct
extern HINSTANCE hInst;
extern HWND      hMainWnd;         // The handle of REGSHOT main window
extern HMENU     hMainMenu;       // The handle of main menu
extern HMENU     hButtonsMenu;            // The handles of shortcut menus
extern LPREGSHOT lpMenuShot;       // Pointer to current Shot for popup menus and alike
extern BOOL      fUseLongRegHead;  // Flag for compatibility with Regshot 1.61e5 and undoreg 1.46
extern BOOL      fOutSeparateObjs; 
extern int       nOutMaxResultLines;
extern BOOL      fDontDisplayInfoAfterShot;
extern BOOL      fDontDisplayInfoAfterComparison;
extern BOOL      bRunning;
extern BOOL      fNoVals;
extern BOOL      fOnlyNewEntries;
extern BOOL      fNoDeletedEntries;
extern BOOL      fLogEnvironmentStrings;
extern BOOL      fSuppressLogs;
extern BOOL      fLogUNLOrder;
extern BOOL      fStoreOnQuit;
extern BOOL      fStoreOnlyUncompared;
extern BOOL      fSaveSettingsOnExit;
extern BOOL      fNoFiltersWhenLoading;
extern BOOL      fShowSIDFilterRules;
extern UINT      nExpandLevels;
extern UINT      nMaxNodes;
extern UINT      nMaxLines;
extern BOOL      fOpenEditor;
extern BOOL      fOutputTXTfile;
extern BOOL      fOutputAU3file;
extern BOOL      fOutputUNLfile;
extern BOOL      fOutputBATfile;
extern BOOL      fOutputHTMfile;
extern BOOL      fISSInstallFile;
extern BOOL      fISSDeinstallFile;
extern BOOL      fNSIInstallFile;
extern BOOL      fNSIDeinstallFile;
extern BOOL      fDeleteReadOnly;
extern BOOL      fDeleteDirNotEmpty;
extern BOOL      fCheckResult;
extern BOOL      bRegSkipAdded;
extern BOOL      bRegWhitelistAdded;
extern BOOL      bFileSkipAdded;
extern BOOL      bDirScanAdded;
extern BROWSEINFO BrowseInfo1;  // BrowseINFO struct
extern BOOL      bNoGui;
extern BOOL      fRegDel;
extern BOOL      fRegIns;
extern BOOL      fREG5;
extern BOOL      fUseDifferentISSOutputFolder;
extern BOOL      fUseDifferentISSEditor;
extern BOOL      fUseDifferentNSIOutputFolder;
extern BOOL      fUseDifferentNSIEditor;
extern LPTSTR    szSID;

extern BOOL      bCompareFS;
extern BOOL      bCompareReg;

extern BOOL  fHKLM;
extern BOOL  fHKU;
extern BOOL  fHKCU;
extern BOOL  fFileshot;

extern int nCodePage;
extern BOOL bNoOutput;

// ----------------------------------------------------------------------
// Routines defined in setup.c
// ----------------------------------------------------------------------
BOOL    LoadSettingsFromIni(HWND hDlg);
BOOL    SaveSettingsToIni(HWND hDlg);
SKIPLIST* FillSkipList(SKIPLIST* pList, LPTSTR lpszIniSection, LPTSTR lpszSectionContent, int iList);
BOOL    IsInSkipList(LPTSTR lpszString, SKIPLIST * lpSkipList, BOOL bForOutput);
BOOL    IsInWhiteList(LPTSTR lpszString, BOOL bForOutput);
LPTSTR  FindKeyInIniSection(LPTSTR lpgrszSection, LPTSTR lpszSearch, size_t cchSectionLen, size_t cchSearchLen);
SKIPLIST* EvaluateAndSkipSID(SKIPLIST* pList);
BOOL    ResetOutputOptions(VOID);
BOOL    CleanUpGlobalMemory(VOID);

// ----------------------------------------------------------------------
// Routines defined in ui.c
// ----------------------------------------------------------------------
VOID    UI_ShowHideCounters(int nCmdShow);
VOID    UI_ShowHideProgressBar(int nCmdShow);
VOID    UI_InitCounters(VOID);
VOID    UI_InitProgressBar(VOID);
VOID    UI_UpdateCounters(LPTSTR lpszTitle1, LPTSTR lpszTitle2, DWORD nCount1, DWORD nCount2, LPTSTR lpszTitle3, LPTSTR lpszKeyname);
VOID    UI_UpdateProgressBar(VOID);
VOID    UI_SetHourGlassCursor(VOID);
VOID    UI_GetShotDialogSettings(VOID);
VOID    UI_RemoveHourGlassCursor(VOID);
VOID    UI_EnableMainButtons(VOID);
VOID    UI_DisableSelectionButtons(VOID);
VOID    UI_CreateShotPopupMenu(VOID);
VOID    UI_CreateComparePopupMenu(VOID);
int     UI_Refresh(VOID);

// ----------------------------------------------------------------------
// Routines defined in misc.c
// ----------------------------------------------------------------------
VOID    WriteBOM(HANDLE hFile, int CodePage);
BOOL    WriteFileCP(_In_ HANDLE hFile, _In_ int CodePage, _In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer, _In_ DWORD nNumberOfBytesToWrite, _Out_opt_ LPDWORD lpNumberOfBytesWritten, _Inout_opt_ LPOVERLAPPED lpOverlapped);
VOID    WriteFileBuffer(HANDLE hFile, long ofsFile, LPCVOID lpData, DWORD cbData);
VOID    EmptyFileBuffer(HANDLE hFile);
size_t  AdjustBuffer(LPVOID* lpBuffer, size_t nCurrentSize, size_t nWantedSize, size_t nAlign);
VOID    ErrMsg(LPTSTR lpszErrMsg);
BOOL    ReplaceInvalidFileNameChars(LPTSTR lpszFileName);
int     CALLBACK SelectBrowseFolder(HWND hWndBrowse, UINT uMsg, LPARAM lParam, LPARAM lpData);
// ----------------------------------------------------------------------
// Routines defined in language.c
// ----------------------------------------------------------------------
VOID    CalculateMaxLanguageNameLength(VOID);
VOID    LoadLanguageFromIni(VOID);
VOID    SetTextsToDefaultLanguage(VOID);
VOID    SetTextsToSelectedLanguage(HWND hDlg, BOOL bGetComboboxSelection);
VOID    FillComboboxWithLanguagesFromIni(HWND hDlg);
// ----------------------------------------------------------------------
// Routines defined in resultcontrol.c
// ----------------------------------------------------------------------
VOID    DoTVPropertySheet(HWND hwndOwner, int iSelectedPage);
BOOL    CALLBACK DlgSkipTVProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL    SearchTVDown(HWND hwndTV, HTREEITEM hItem, int iPropertyPage);
BOOL    BuildSkipString(HWND hwndTV, HTREEITEM hItem, int iPropertyPage);
BOOL    ResetAdhocFilters(VOID);
BOOL    InsertRootItems(HWND hwndTV, int iPropertyPage);
HTREEITEM FindOrCreateTreeItem(HWND hwndTV, HTREEITEM hParent, HTREEITEM hItem, UINT nLevel, LPTSTR lpszNodeText, int iPropertyPage, BOOL bKey, LPCOMPRESULTNEW lpCR);
BOOL    InitTreeViewItems(HWND hwndTV, int iPropertyPage);
BOOL    InitTreeViewImageLists(HWND hwndTV);
BOOL    CheckFilters(LPVOID lpContent, LPTSTR lpszKeyName, LPTSTR* lpszValueName, DWORD nActionType, BOOL* pbKey);


#define _MACROLINESIZE_ 2048
#define _MACROREPLACEMENTS_ 100
#define _TREEITEMTEXT_ 100

struct _OUTPUTFILEDESCRIPTION {
    int    iOutputType;
    BOOL   bDeInstaller;
    BOOL   bLogger;
    LPTSTR lpszFormat;
    LPTSTR lpszComment;
    LPTSTR lpszScriptFile;
    LPTSTR lpszOutFile;
    LPTSTR lpszOutDir;
    int    nCodePage;
    BOOL   bBOM;
    LPTSTR lpszPartCountText;
    DWORD  nPartCount;
// Flags
    LPTSTR lpszFilenameSuffix;
    LPTSTR lpszExtension;

    BOOL   fOnlyNewEntries;
    BOOL   fLogUNLOrder;
    BOOL   fUseLongRegHead;
    BOOL   fLogEnvironmentStrings;
    BOOL   fOutSeparateObjs;
    BOOL   fNoVals;
    BOOL   fGroupRegKeys;

    BOOL   fFilesystem;
    BOOL   fRegistry;
};
typedef struct _OUTPUTFILEDESCRIPTION OUTPUTFILEDESCRIPTION, FAR* LPOUTPUTFILEDESCRIPTION;
//OUTPUTFILEDESCRIPTION OutputFileDescription;

LPTSTR lpszMacroFileName;
LPTSTR lpszSuppressedKeyPart;
LPTSTR lpszRegDefaultVal;
LPTSTR lpszMultiSzStringDivider;
LPTSTR lpszVALADDNormal;
LPTSTR lpszVALADDShort;
LPTSTR lpszVALCHANGENormal;
LPTSTR lpszVALCHANGEShort;
LPTSTR lpszVALDELETENormal;
LPTSTR lpszVALDELETEShort;
LPTSTR lpszKEYADDNormal;
LPTSTR lpszKEYDELETENormal;
LPTSTR lpszFILEADDNormal;
LPTSTR lpszFILECHANGENormal;
LPTSTR lpszFILEDELETENormal;
LPTSTR lpszFILEDELETEDeleteReadOnly;
LPTSTR lpszDIRADDNormal;
LPTSTR lpszDIRCHANGENormal;
LPTSTR lpszDIRDELETENormal;
LPTSTR lpszDIRDELETEDeleteRecurse;

LPTSTR lpszNameREG_SZ;
LPTSTR lpszNameREG_EXPAND_SZ;
LPTSTR lpszNameREG_MULTI_SZ;
LPTSTR lpszNameREG_DWORD_LITTLE_ENDIAN;
LPTSTR lpszNameREG_DWORD_BIG_ENDIAN;
LPTSTR lpszNameREG_QWORD_LITTLE_ENDIAN;
LPTSTR lpszNameREG_NONE;
LPTSTR lpszNameREG_BINARY;
LPTSTR lpszPrefixREG_SZ;
LPTSTR lpszPrefixREG_EXPAND_SZ;
LPTSTR lpszPrefixREG_MULTI_SZ;
LPTSTR lpszPrefixREG_DWORD_LITTLE_ENDIAN;
LPTSTR lpszPrefixREG_DWORD_BIG_ENDIAN;
LPTSTR lpszPrefixREG_QWORD_LITTLE_ENDIAN;
LPTSTR lpszPrefixREG_NONE;
LPTSTR lpszPrefixREG_BINARY;


LPTSTR  ReplaceSpecialCharacters(LPTSTR lpszText, BOOL bNames);
LPTSTR  EscapeSpecialCharacters(LPTSTR lpszText, TCHAR c, LPTSTR lpEscape);
LPTSTR  ReplaceHeaderMacros(LPTSTR lpszText, LPOUTPUTFILEDESCRIPTION pOutputFileDescription);
LPTSTR  ReplaceDataMacros(LPTSTR lpszDataLineMacro, LPTSTR lpszText, LPVOID lpContent, BOOL bSuppressKey, LPOUTPUTFILEDESCRIPTION pOutputFileDescription);
BOOL    ReadOutputMacros(LPOUTPUTFILEDESCRIPTION pOutputFileDescription);
BOOL    WritePartTemplate(HANDLE hFile, LPTSTR lpszTemplate, LPTSTR lpszTask, LPOUTPUTFILEDESCRIPTION pOutputFileDescription);
LPTSTR  BuildOutputString(LPTSTR lpszText, LPVOID lpContent, int nActionType, BOOL bSuppressKey, LPOUTPUTFILEDESCRIPTION pOutputFileDescription);

VOID    WriteTableHead(HANDLE hFile, LPTSTR lpszText, DWORD nCount, LPOUTPUTFILEDESCRIPTION pOutputFileDescription);
VOID    CreateNewResult(DWORD nActionType, LPVOID lpContentOld, LPVOID lpContentNew);
size_t  ResultToString(LPTSTR rgszResultStrings[], size_t iResultStringsMac, size_t iLinesWrittenOldPart, DWORD nActionType, LPVOID lpContent, BOOL fNewContent, BOOL bSuppressKey, LPOUTPUTFILEDESCRIPTION pOutputFileDescription);

VOID    Shot(LPREGSHOT lpShot);
VOID    CompareShots(VOID);
BOOL    OutputComparisonResult(VOID);
VOID    SaveShot(LPREGSHOT lpShot);
BOOL    LoadShot(LPREGSHOT lpShot);
VOID    FreeCompareResult(void);
VOID    FreeShot(LPREGSHOT lpShot);
VOID    FreeAllHeadFiles(LPHEADFILE lpHF);
VOID    ClearRegKeyMatchFlags(LPKEYCONTENT lpKC);
VOID    FileShot(LPREGSHOT lpShot);
LPTSTR  GetWholeFileName(LPFILECONTENT lpStartFC, size_t cchExtra, LPOUTPUTFILEDESCRIPTION pOutputFileDescription);
VOID    WritePart(HANDLE hFile, DWORD nActionType, LPCOMPRESULT lpStartCR, LPOUTPUTFILEDESCRIPTION pOutputFileDescription);
VOID    WritePartNew(HANDLE hFile, DWORD nActionType, LPCOMPRESULTNEW lpStartCR, LPOUTPUTFILEDESCRIPTION pOutputFileDescription);
VOID    WriteHTMLHeader(HANDLE hFile, int CodePage);
LPTSTR  GetWholeKeyName(LPKEYCONTENT lpVC, BOOL fUseLongNames);
LPTSTR  GetWholeValueName(LPVALUECONTENT lpVC, BOOL fUseLongNames);
VOID    ClearHeadFileMatchFlags(LPHEADFILE lpHF);
BOOL    FindDirChain(LPHEADFILE lpHF, LPTSTR lpszDir, size_t nBufferLen);
BOOL    DirChainMatch(LPHEADFILE lpHF1, LPHEADFILE lpHF2);
VOID    CompareHeadFiles(LPHEADFILE lpStartHF1, LPHEADFILE lpStartHF2);
VOID    DisplayShotInfo(HWND hDlg, LPREGSHOT lpShot);
VOID    DisplayResultInfo(HWND hDlg);
VOID    SwapShots(VOID);
BOOL    CheckShotsChronology(HWND hDlg);
BOOL    CheckSavingShots(HWND hDlg, LPREGSHOT lpShot);
VOID    DoPropertySheet(HWND hwndOwner, int iSelectedPage);
BOOL CALLBACK DlgOptionsProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgSkipRegProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#define REGSHOT_BUFFER_BLOCK_BYTES 1024

enum eOutputFiles
{
    OUT_UNL = 0,
    OUT_TXT,
    OUT_HTML,
    OUT_BAT,
    OUT_REG_DEINSTALL,
    OUT_REG_INSTALL,
    OUT_ISS_DEINSTALL,
    OUT_ISS_INSTALL,
    OUT_NSI_DEINSTALL,
    OUT_NSI_INSTALL,
    OUT_AU3,
    // ATTENTION: add new output types before this line, the last line is used to determine the count
    OUTPUTTYPES
};

enum eDialogs {
    DLG_MAIN = 0,
    DLG_COMMON_1,
    DLG_COMMON_2
};

// List of all language strings
// NEVER CHANGE THE ORDER, LEAVE OLD ENTRIES UNTOUCHED
enum eLangTexts {
    iszTextKey = 0,
    iszTextValue,
    iszTextDir,
    iszTextFile,
    iszTextTime,
    iszTextKeyAdd,
    iszTextKeyDel,
    iszTextValAdd,
    iszTextValDel,
    iszTextValModi,
    iszTextFileAdd,
    iszTextFileDel,
    iszTextFileModi,
    iszTextDirAdd,
    iszTextDirDel,
    iszTextDirModi,
    iszTextTotal,
    iszTextComments,
    iszTextDateTime,
    iszTextComputer,
    iszTextUsername,
    iszTextAbout,
    iszTextError,
    iszTextErrorExecViewer,
    iszTextErrorCreateFile,
    iszTextErrorOpenFile,
    iszTextErrorMoveFP,
    //
    iszTextButtonShot1,
    iszTextButtonShot2,
    iszTextButtonCompare,
    iszTextMenuClear,
    iszTextButtonQuit,
    iszTextButtonAbout,
    iszTextTextMonitor,
    iszTextTextCompare,
    iszTextTextOutput,
    iszTextTextComment,
    iszTextRadioPlain,
    iszTextRadioHTML,
    iszTextTextScan,
    //
    iszTextMenuShot,
    iszTextMenuShotSave,
    iszTextMenuLoad,
    iszTextButtonClearAll,
    iszTextMenuClearShot1,
    iszTextMenuClearShot2,
    //
    iszTextMenuSave,
    iszTextMenuInfo,
    iszTextMenuSwap,
    //
    iszTextMenuCompareOutput,
    iszTextMenuOutput,
    //
    iszTextLoadedFromFile,
    //
    iszTextWarning,
    iszTextChronology,
    iszTextShotsNotChronological,
    iszTextQuestionSwapShots,
    iszTextMenuShotCompareOutput,
    iszTextGroupboxTitle,
    iszTextGroupboxRegs,
    iszTextGroupboxDirs,
    iszTextRadioUNL,
    iszTextCheckboxRegIns,
    iszTextCheckboxRegDel,
    iszTextMenuFile,
    iszTextMenuOptions,
    iszTextMenuHelp,
    iszTextMenuOpen,
    iszTextButtonStop,
    iszTextRegistryModi,
    iszTextFilesystemModi,
    iszTextMenuOutputFS,
    iszTextMenuOutputReg,
    iszTextCheckOnlyAddedParts,
    iszTextCheckNoFilters,
    iszTextCheckResult,
    iszTextCheckResetFilters,
    iszTextCheckDeleteShots,
    iszTextCheckDefaultActions,
    iszTextCheckAutocompare,
    iszTextCheckNoVals,
    iszTextCheckUNLOrder,
    iszTextCheckOpenEditor,
    iszTextNotSaved,
    iszTextMaxOutputLines,
    iszTextMenuCompareCompareOutputFS,
    iszTextMenuCompareCompareOutputReg,
    iszTextButtonLogs,
    iszTextButtonDirs,
    iszTextButtonRegs,
    iszTextButtonLoadIni,
    iszTextButtonSaveIni,
    iszTextButtonClear1,
    iszTextButtonClear2,
    iszTextButtonScans,
    iszTextCheckNoDeletedEntries,
    // ATTENTION: add new language strings before this line, the last line is used to determine the count
    cLangStrings
};

struct _LANGUAGETEXT {
    LPTSTR lpszText;
    int nIDDialog;
    int nIDDlgItem;
};
typedef struct _LANGUAGETEXT LANGUAGETEXT, FAR *LPLANGUAGETEXT;

extern LANGUAGETEXT asLangTexts[];

extern FILEHEADER fileheader;
extern FILEEXTRADATA fileextradata;
extern LPBYTE lpFileBuffer;
extern LPTSTR lpStringBuffer;
extern size_t nStringBufferSize;
extern size_t nSourceSize;

extern LPTSTR lpszCRLF;

extern LPTSTR lpszProgramName;
extern LPTSTR lpszREGVersion4;
extern LPTSTR lpszREGVersion5;

extern LPTSTR lpszCommentCommon;
extern LPTSTR lpszCommentBAT;
extern HICON ico_green, ico_red;

//#ifndef _UNICODE
extern LPTSTR lpszEmpty;
//#endif

VOID SaveHeadFiles(HANDLE hFile, LPREGSHOT lpShot, LPHEADFILE lpHF, DWORD nFPCaller);
VOID LoadHeadFiles(LPREGSHOT lpShot, DWORD ofsHeadFile, LPHEADFILE *lplpCaller);

#ifdef DEBUGLOG
#define REGSHOT_DEBUG_MESSAGE_LENGTH 101

extern LPTSTR lpszDebugTryToGetValueLog;
extern LPTSTR lpszDebugValueNameDataLog;
extern LPTSTR lpszDebugKeyLog;

VOID DebugLog(LPTSTR lpszFileName, LPTSTR lpszDbgMsg, BOOL fAddCRLF);
#endif

#endif  // REGSHOT_GLOBAL_H
