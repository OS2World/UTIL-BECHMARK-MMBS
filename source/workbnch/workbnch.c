//===========================================================================
// workbnch.c :
// WORKBNCH [p:testpath] [l:logfile] [s:speed] [o:options]
// or
// WORKBNCH <t:test_id> [i:iterations] [p:testpath] [l:logfile] [o:options]
// 2004-22-1  - Alessandro Felice Cantatore
//===========================================================================


#include "workbnch.h"

// globals ------------------------------------------------------------------

WORKBENCH wb;


// definitions --------------------------------------------------------------
#define MIN_ID    ITEST_TIMER
#define MAX_ID    ITEST_TEXTFINDS

// default iterations per test
//                 0     1     2     3     4     5     6     7     8     9
#define AITER {    0,    0, 1200,  600,   64,    1,    1,    1,    4,    1, \
                   1,    1,    5,    1,    4,    1,   53,   33,   29,   43, \
                  36,   36,   14,    6}


// prototypes ---------------------------------------------------------------
BOOL appInit(INT argc, PSZ* argv, PTESTPARMS ptp);
VOID appRun(PTESTPARMS ptp);
VOID appTerminate(VOID);
BOOL parseArgs(INT argc, PSZ* argv, PTESTPARMS ptp);
BOOL initRandomWordsArray(VOID);
VOID runTest(ULONG id, ULONG iterations);
BOOL isTestPathValid(VOID);
PSZ randomAlphaWord(PSZ p, ULONG cb);
PSZ randomNumWord(PSZ p, ULONG cb);


/* --------------------------------------------------------------------------
 Main:
 The programs is used to run a set of benchmarks or, for debugging purposes,
 just the specified test.

 To run all the tests:
 WORKBNCH [p:testpath] [l:logfile] [s:speed] [o:options]
 To run a specific test (debug purpose only) :
 WORKBNCH <t:test_id> [i:iterations] [p:testpath] [l:logfile] [o:options]
 where:
 testpath       : is the path to be used for writing the test files
                  (default is 'TEST' in the current path).
 logfile        : output file for the test results (default is results.txt
                  in the current path).
 speed          : [0-9] set the test iterations multiplier. Set to 9 when
                  testing a slow machine, less on faster machines to get
                  more accurate results.
 options        :    u (unattended) to run all tests without prompting
                     q (quiet) do not display test details (only test id and
                       iterations
                     p (progress) to show a progress bar
                     n (no-check) don't check for free space on the test path
 test_id        : [1-12] is the test number
 iterations     : how many times to repeat the test 'test_id'

 ------------------------------------------------------------------------- */
INT main(INT argc, PSZ* argv) {
   TESTPARMS tp;
   if (!appInit(argc, argv, &tp)) return 2;
   appRun(&tp);
   appTerminate();
   return (ULONG)(wb.elapsedTime * 1000);
}


/* --------------------------------------------------------------------------
 - set the screen to 80 cols x 25 rows
 - print the logo
 - check input parameters
 - alloccate and initialize the random text words array
 ------------------------------------------------------------------------- */
BOOL appInit(INT argc, PSZ* argv, PTESTPARMS ptp) {
   scrModeSetDefault();
   scrPrintMsg(SZ_ABOUT, YELLOWONBLACK, FALSE);
   if (!parseArgs(argc, argv, ptp) || !initRandomWordsArray()) return FALSE;
   return TRUE;
}


/* --------------------------------------------------------------------------
 according to the input parameters :
 - run all tests
 - run the specificied test (this is for debugging purpose only)
 ------------------------------------------------------------------------- */
VOID appRun(PTESTPARMS ptp) {
   ULONG aiter[] = AITER;
   // no 'test id' was specified : run all tests
   if (!ptp->id) {
      for (ptp->id = MIN_ID + wb.unattended; ptp->id <= MAX_ID; ++ptp->id) {
         runTest(ptp->id, aiter[ptp->id] * ptp->repeat);
      } /* endfor */
   // run a specific test
   } else {
      // no 'iterations' defined : get the default
      if (!ptp->iterations) ptp->iterations = aiter[ptp->id];
      runTest(ptp->id, ptp->iterations);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Application termination:
 - free the array of random words.
 - remove all data created by the tests.
 - close the result file.
 ------------------------------------------------------------------------- */
// close the log file
VOID appTerminate(VOID) {
   scrClean();
   scrCursorMoveTo(5, 0);
   sprintf(wb.msgbuf, SZ_TOTAL, wb.ctests, wb.elapsedTotal);
   scrPrintMsg(wb.msgbuf, SKYONBLACK, TRUE);
   DosBeep(100, 300), DosBeep(800, 400);
   scrPrintMsg(SZ_ANYKEYTOEXIT, REDONBLACK, FALSE);
   kbdGetChar();
   free(wb.prw);
   if (!wb.keepTestPath) {
      treeDelete(wb.path, 0);
      fmDeleteDir(wb.path);
   } /* endif */
   fmClose(wb.hlog);
}


/* --------------------------------------------------------------------------
 Check the input parameters (do not check for duplicates).
 See main() function comment.
 ------------------------------------------------------------------------- */
BOOL parseArgs(INT argc, PSZ* argv, PTESTPARMS ptp) {
   PSZ pszLogFile = "results.txt";
   PSZ pszPath = "TEST";
   ptp->id = 0;
   ptp->repeat = 3;
   ptp->iterations = 0;
   // all arguments are one command letter followed by a colon and further
   // specifiers
   while (--argc > 0) {
      if (argv[argc][1] != ':') goto showhelp;
      switch (argv[argc][0] | 0x20) {
         // iterations
         case 'i':
            if (0 == (ptp->iterations = strtoul(&argv[argc][2], NULL, 10))) {
               scrPrintErrorMsg(SZERR_INVITER);
               goto showhelp;
            } /* endif */
            break;
         // log file
         case 'l':
            pszLogFile = &argv[argc][2];
            break;
         case 'o':
            if (strchr(&argv[argc][2], 'u') || strchr(&argv[argc][2], 'U'))
               wb.unattended = TRUE;
            if (strchr(&argv[argc][2], 'p') || strchr(&argv[argc][2], 'P'))
               wb.progress = TRUE;
            if (strchr(&argv[argc][2], 'q') || strchr(&argv[argc][2], 'Q'))
               wb.quiet = TRUE;
            if (strchr(&argv[argc][2], 'n') || strchr(&argv[argc][2], 'N'))
               wb.nocheck = TRUE;
            break;
         // path name
         case 'p':
            pszPath = &argv[argc][2];
            break;
         // speed
         case 's':
            if ((argv[argc][2] < '0') || (argv[argc][2] > '9')) {
               scrPrintErrorMsg(SZERR_SPPED);
               goto showhelp;
            } /* endif */
            ptp->repeat = 10 - (argv[argc][2] - '0');
            break;
         // test id
         case 't':
            ptp->id = strtoul(&argv[argc][2], NULL, 10);
            if ((ptp->id < MIN_ID) || (ptp->id > MAX_ID)) {
               scrPrintErrorMsg(SZERR_INVID);
               goto showhelp;
            } /* endif */
            wb.keepTestPath = TRUE;
            break;
         default:
            sprintf(wb.msgbuf, "SZ_INVOPTION", argv[argc]);
            scrPrintErrorMsg(wb.msgbuf);
            goto showhelp;
            break;
      } /* endswitch */
   } /* endwhile */
   // get the full path name of the test path
   fmFullPathName(pszPath, wb.path);
   // create the test path
   if (!fmPathCreate(wb.path)) {
      scrPrintErrorMsg(SZERR_MAKEPATH);
      return FALSE;
   } /* endif */
   // check if the test path is valid
   if (!wb.nocheck && !isTestPathValid()) {
      scrPrintErrorMsg(SZERR_INVTESTPATH);
      return FALSE;
   } /* endif */
   wb.hlog = fmOpen(pszLogFile, FMO_CREATE|FMO_IFEXIST|FMO_SHAREREAD|FMO_APPEND);
   if (FMO_ERROR == wb.hlog) {
      wb.hlog = 0;
      scrPrintErrorMsg(SZERR_OPENLOG);
      return FALSE;
   } /* endif */
   return TRUE;

showhelp:
   scrPrintErrorMsg(SZ_HELP);
   return FALSE;
}


/* --------------------------------------------------------------------------
 Create an array of text strings composed by random characters.
 ------------------------------------------------------------------------- */
BOOL initRandomWordsArray(VOID) {
   INT i;
   PSZ p;
   ULONG cb;
   if (NULL == (wb.prw = tryalloc(sizeof(RANDWORDS))))
      return FALSE;
   srand(0xafc);
   for (i = 0, p = wb.prw->buf; i < CRANDWORDS; ++i) {

      wb.prw->apszWords[i] = p;
      p = (rand() % 19)
          ?
          randomAlphaWord(p, (rand() % 12) + 2)
          :
          randomNumWord(p, (rand() % 9) + 1)
          ;
      *p = 0, ++p;
   } /* endfor */
//   {                  // DEBUG //
//      CHAR buf[80];
//      PSZ pfile;
//      pfile = tryalloc(300000);
//      for (i = 0, p = pfile; i < CRANDWORDS; ++i) {
//         p += sprintf(p, "%s\n", wb.prw->apszWords[i]);
//      } /* endfor */
//      fmWriteData("RANDOMARRAY.TXT", pfile, (ULONG)(p - pfile));
//   }
   return TRUE;
}


/* --------------------------------------------------------------------------
 Run the test 'id' for 'iterations' times.
 ------------------------------------------------------------------------- */
// select the test to run
VOID runTest(ULONG id, ULONG iterations) {
   BOOL rc;
   wb.cbProcessed = 0;
   wb.operations = 0;
   switch (id) {
      case ITEST_TIMER:            // check virtual machine timer accuracy
         rc = testTimer();
         break;
      case ITEST_MATHINT:          // integer calculations
         rc = testInt(iterations);
         break;
      case ITEST_MATHFLOAT:        // double math calculations
         rc = testDouble(iterations);
         break;
      case ITEST_MATHTRIG:         // trigonometric calculations
         rc = testTrig(iterations);
         break;
      case ITEST_MULTIFILEWRITE:         // disk write test
         rc = testMultiFileWrite(iterations);
         break;
      case ITEST_LARGEFILEWRITE:       // disk write large file test
         rc = testLargeFileWrite(iterations);
         break;
      case ITEST_MULTIFILEREAD:
         rc = testMultiFileRead(iterations);
         break;
      case ITEST_LARGEFILEREAD:
         rc = testLargeFileRead(iterations);
         break;
      case ITEST_MULTIFILECOPY:
         rc = testMultiFileCopy(iterations);
         break;
      case ITEST_LARGEFILECOPY:
         rc = testLargeFileCopy(iterations);
         break;
      case ITEST_SEQFILEZIP:
         rc = testSeqFileZip(iterations);
         break;
      case ITEST_MULTIFILEZIP:
         rc = testMultiFileZip(iterations);
         break;
      case ITEST_LARGEFILEZIP:
         rc = testLargeFileZip(iterations);
         break;
      case ITEST_UNZIP:
         rc = testUnzip(iterations);
         break;
      case ITEST_MULTIFILEDEL:
         rc = testDeleteAllFiles();
         break;
      case ITEST_TEXTCHARSTATS:
         rc = testTextCharStats(iterations);
         break;
      case ITEST_TEXTWORDPARSE:
         rc = testTextWordParse(iterations);
         break;
      case ITEST_TEXTWORDSORTI:
         rc = testTextWordSort(iterations, ITEST_TEXTWORDSORTI, SZ_TEXTWORDSORTI,
                               (PQSORTCOMPF)compwordsi);
         break;
      case ITEST_TEXTWORDSORTS:
         rc = testTextWordSort(iterations, ITEST_TEXTWORDSORTS, SZ_TEXTWORDSORTS,
                               (PQSORTCOMPF)compwordss);
         break;
      case ITEST_TEXTTOUPPER:
         rc = testTextCaseConvert(iterations, ITEST_TEXTTOUPPER, SZ_TEXTTOUPPER,
                                  (PCASEMAPFN)toupper);
         break;
      case ITEST_TEXTTOLOWER:
         rc = testTextCaseConvert(iterations, ITEST_TEXTTOLOWER, SZ_TEXTTOLOWER,
                                  (PCASEMAPFN)tolower);
         break;
      case ITEST_TEXTFINDI:
         rc = testTextWordFind(iterations, ITEST_TEXTFINDI, SZ_TEXTFINDI,
                               (PFINDCOMPF)strnicmp);
         break;
      case ITEST_TEXTFINDS:
         rc = testTextWordFind(iterations, ITEST_TEXTFINDS, SZ_TEXTFINDS,
                               (PFINDCOMPF)memcmp);
         break;
   } /* endswitch */
   if (rc) wb.ctests++;
}


/* --------------------------------------------------------------------------
 Check if the test path is valid by trying to create a file as large as the
 total size needed to run all the benchmark tests.
 ------------------------------------------------------------------------- */
BOOL isTestPathValid(VOID) {
   ULONG cbpath;
   HFILE hf;
   ULONG ul;
   cbpath = strlen(wb.path);
   strcpy(wb.path + cbpath, "\\CHKPATH.FIL");
   if (DosOpen(wb.path, &hf, &ul, CB_TESTPATH, 0,
               OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_REPLACE_IF_EXISTS,
               OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_WRITEONLY, NULL))
      return FALSE;
   DosClose(hf);
   DosForceDelete(wb.path);
   wb.path[cbpath] = 0;
   return TRUE;
}


/* --------------------------------------------------------------------------
 Create a text word containing random alphabetical characters.
 ------------------------------------------------------------------------- */
PSZ randomAlphaWord(PSZ p, ULONG cb) {
   CHAR consonants[] = "bcdfglmnprstvxz";
   CHAR vowels[] = "aeiou";
   while (cb--) {
      *p = cb % 2 ? vowels[rand() % 5] : consonants[rand() % 15];
      ++p;
   } /* endwhile */
   return p;
}


/* --------------------------------------------------------------------------
 Create a text word containing random numbers.
 ------------------------------------------------------------------------- */
PSZ randomNumWord(PSZ p, ULONG cb) {
   CHAR nums[] = "0123456789";
   while (cb--) {
      *p = nums[rand() % 10];
      ++p;
   } /* endwhile */
   return p;
}


/* --------------------------------------------------------------------------
 ------------------------------------------------------------------------- */

