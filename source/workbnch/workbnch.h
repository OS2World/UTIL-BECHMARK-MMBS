//===========================================================================
// .h :
//
// --2003  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _SERVBNCH_H_
   #define _SERVBNCH_H_
   #define INCL_WIN
   #define INCL_GPI
   #define INCL_DOS
   #define INCL_DOSMISC
   #define INCL_DOSMODULEMGR
   #define INCL_DOSFILEMGR
   #define INCL_DOSPROCESS
   #define INCL_DOSNLS
   #define INCL_KBD
   #define INCL_VIO

   #include <os2.h>
   #include <stdlib.h>
   #include <stdio.h>
   #include <string.h>
   #include <math.h>
   #include <ctype.h>
   #include "messages.h"
   #include "fileUtil.h"

   #ifndef RC_INVOKED
   #endif


// constants ----------------------------------------------------------------

// disk size used for the tests
#define CB_TESTPATH  (1024*1024*512)

// ----------------------------------------------------------------------------


#define REDONBLACK           "[40;31;1m"
#define GREENONBLACK         "[40;32;1m"
#define SKYONBLACK           "[40;36;1m"
#define YELLOWONBLACK        "[40;33;1m"
#define CB_SZCOLOR           10

#define ITEST_TIMER             1
#define ITEST_MATHINT           2
#define ITEST_MATHFLOAT         3
#define ITEST_MATHTRIG          4
#define ITEST_MULTIFILEWRITE    5
#define ITEST_LARGEFILEWRITE    6
#define ITEST_MULTIFILEREAD     7
#define ITEST_LARGEFILEREAD     8
#define ITEST_MULTIFILECOPY     9
#define ITEST_LARGEFILECOPY    10
#define ITEST_SEQFILEZIP       11
#define ITEST_MULTIFILEZIP     12
#define ITEST_LARGEFILEZIP     13
#define ITEST_UNZIP            14
#define ITEST_MULTIFILEDEL     15
#define ITEST_TEXTCHARSTATS    16
#define ITEST_TEXTWORDPARSE    17
#define ITEST_TEXTWORDSORTI    18
#define ITEST_TEXTWORDSORTS    19
#define ITEST_TEXTTOUPPER      20
#define ITEST_TEXTTOLOWER      21
#define ITEST_TEXTFINDI        22
#define ITEST_TEXTFINDS        23

// datatypes ----------------------------------------------------------------

#define CRANDWORDS 20000

typedef struct {
   PSZ apszWords[CRANDWORDS];    // array of random generated words
   CHAR buf[240000];        // storage for the random words
} RANDWORDS, * PRANDWORDS;

typedef struct {
   ULONG id;
   ULONG iterations;
   ULONG repeat;
} TESTPARMS, * PTESTPARMS;

typedef struct {
   CHAR path[CCHMAXPATH];   // test path
   CHAR msgbuf[1024];       // text buffer (used for sprintf-ing text messages)
   HFILE hlog;              // log file handle
   ULONG startTime;         // start time
   ULONG prevStep;          // previous state of the progress bar
   ULONG ctests;            // count of tests succesfully performed
   BOOL unattended;         // do not prompt the user for starting the tests
                            // (and skip the first test)
   BOOL progress;           // show the progress bar
   BOOL quiet;              // do not display test details
   BOOL nocheck;            // do not check for free disk space
   BOOL keepTestPath;       // if TRUE do not delete the test path on termination
   double cbProcessed;      // size of processed data
   double operations;       // operations / seconds
   double elapsedTime;      // elapsed time
   double elapsedTotal;     // total elapsed time
   PRANDWORDS prw;          // array of random generated words
} WORKBENCH, * PWORKBENCH;


typedef int (QSORTCOMPF)(const void*, const void*);
typedef QSORTCOMPF * PQSORTCOMPF;

typedef int (CASEMAPFN)(int);
typedef CASEMAPFN * PCASEMAPFN;

typedef int (FINDCOMPF)(const void*, const void*, size_t);
typedef FINDCOMPF * PFINDCOMPF;

// globals ------------------------------------------------------------------
extern WORKBENCH wb;

// prototypes ---------------------------------------------------------------

// consoleIO.c
VOID scrColorSet(PSZ pszClr);
VOID scrPrintMsg(PSZ pszMsg, PSZ pszClr, BOOL blog);
VOID scrPrintErrorMsg(PSZ pszMsg);
VOID scrPrintStringAt(PSZ pszText, PSZ pszColor, UINT row, UINT column);
VOID scrClean(VOID);
VOID scrModeSetDefault(VOID);
VOID scrCursorMoveTo(UINT row, UINT col);
ULONG scrCursorPos(VOID);
ULONG kbdGetChar(VOID);
VOID pressAnyKey(INT row, INT col);
ULONG userPressedEsc(VOID);

// fileTest.c
BOOL testMultiFileWrite(ULONG iterations);
BOOL testLargeFileWrite(ULONG iterations);
BOOL testMultiFileRead(ULONG iterations);
BOOL testLargeFileRead(ULONG iterations);
BOOL testMultiFileCopy(ULONG iterations);
BOOL testLargeFileCopy(ULONG iterations);
BOOL testSeqFileZip(ULONG iterations);
BOOL testMultiFileZip(ULONG iterations);
BOOL testLargeFileZip(ULONG iterations);
BOOL testUnzip(ULONG iterations);
BOOL testDeleteAllFiles(VOID);

// fileTestAux.c
BOOL createDir(PSZ dir);
ULONG fileSize(INT idx);
ULONG randomTextOffset(INT idx);
BOOL createFile(PSZ pszName, ULONG idx, PSZ pRandText);
BOOL openLargeFile(PHFILE phf);
BOOL writeLargeFileChunk(HFILE hf, PSZ p, ULONG cb);
BOOL readFile(ULONG idir, ULONG isub, ULONG ifile, PSZ p);
BOOL readLargeFile(PHFILE phf, PSZ p);
BOOL copyFile(PSZ pszSrc, PSZ pszDest);
BOOL cmdRun(PSZ pszCommand);
BOOL treeStats(PSZ pszPath, PULONG pcbTotal, PULONG pcFiles, PULONG pcDirs);
BOOL recursiveDirSize(PSZ pszPath, PULONG pcbTotal, PULONG pcFiles, PULONG pcDirs);
BOOL treeDelete(PSZ pszPath, ULONG csteps);
BOOL recursiveDirDelete(PSZ pszPath, ULONG csteps, PULONG pcdeleted);

// fileUtil.c
HFILE fmOpen(PSZ pszFile, ULONG flag);
BOOL fmClose(HFILE hf);
BOOL fmRead(HFILE hf, PVOID pBuffer, ULONG cb);
BOOL fmWrite(HFILE hf, PVOID pBuffer, ULONG cb);
LONG fmSize(HFILE hf);
BOOL fmWriteData(PSZ pszFileName, PSZ pszData, ULONG cbData);
VOID logWriteMsg(PSZ pszMsg);
PSZ fmFileExtChange(PSZ pszFileName, PSZ ext);
BOOL fmExists(PSZ pszFile);
BOOL fmPathCreate(PSZ pszPath);
BOOL fmFullPathName(PSZ pszName, PSZ pszFullPath);
BOOL applWorkDirSet(PSZ pszDir);
BOOL applWorkDirGet(PSZ pszDir);
BOOL fmDeleteDir(PSZ pszDir);

// math.c
BOOL testInt(ULONG iterations);
BOOL testDouble(ULONG iterations);
BOOL testTrig(ULONG iterations);

// stringTest.c
BOOL testTextCharStats(ULONG iterations);
BOOL testTextWordParse(ULONG iterations);
BOOL testTextWordSort(ULONG iterations,
                      INT itest, PSZ pszTestDescription, PQSORTCOMPF pSortFn);
BOOL testTextCaseConvert(ULONG iterations,
                      INT itest, PSZ pszTestDescription, PCASEMAPFN pConvFn);
BOOL testTextWordFind(ULONG iterations,
                      INT itest, PSZ pszTestDescription, PFINDCOMPF pFindCmpFn);

// stringTestAux.c
INT compwordss(PSZ* key, PSZ* element);
INT compwordsi(PSZ* key, PSZ* element);

// testAux.c
BOOL prepareTest(ULONG id, ULONG iterations, PSZ pszMsg, BOOL progress);
VOID resetTimer(VOID);
VOID endTest(BOOL progress);
BOOL testTimer(VOID);
ULONG upTime(VOID);
PVOID tryalloc(ULONG cb);
ULONG printProgress(ULONG prevstep, ULONG curstep);
PSZ makeRandomText(ULONG cb);

#endif // #ifndef _SERVBNCH_H_
