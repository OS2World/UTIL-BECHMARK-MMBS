//===========================================================================
// testaux.c : benchmark test utility functions
// 2004-01-22 - Alessandro Felice Cantatore
//===========================================================================


#include "workbnch.h"

// definitions --------------------------------------------------------------

#define SZ_PERCENTSCALE \
"0%    10%     20%     30%     40%     50%     60%     70%     80%     90%   100%"\
"---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+"\
"                                                                                "\
"                                                                                "\
"                                                                                "

// array of word delimiters used by the random text generator
#define ADELIMITERS { \
   {" ", 0}, {".\n", 1}, {". ", 1}, {":\n", 0}, { ": ", 0}, {" ", 0},        \
   {"!\n", 1}, {"! ", 1}, {"\n", 0}, {"; ", 0}, { ", ", 0}, {" ", 0},        \
   {"?\n", 1}, {"? ", 1}, {" ", 0}, {" ", 0}, {" de ", 0}, {" ", 0},        \
   {" et ", 0}, {"\n", 0}, {" et ", 0}, {" sed ", 0}, {" at ", 0}, {" ", 0}, \
   {" ab ", 0}, {" ex ", 0}, {" ", 0}, {" pro ", 0}, {" et ", 0}, {" ", 0} }

typedef struct {
   PSZ delimiter;
   BOOL capitalize;
} WORDDELIM, * PWORDDELIM;

// prototypes ---------------------------------------------------------------


// globals ------------------------------------------------------------------

/* --------------------------------------------------------------------------
 - clean the screen
 - print test no. and iterations
 - print the test description (if required)
 - print the progress scale (if required)
 - prompt the user to go on with the test (when not in unattended mode)
 ------------------------------------------------------------------------- */
BOOL prepareTest(ULONG id, ULONG iterations, PSZ pszMsg, BOOL progress) {
   scrClean();
   sprintf(wb.msgbuf, SZ_TESTDESCR, id, iterations);
   scrPrintMsg(wb.msgbuf, GREENONBLACK, TRUE);
   wb.prevStep = 0;
   // print the test description
   if (!wb.quiet) {
      scrPrintMsg(pszMsg, GREENONBLACK, TRUE);
      if (wb.cbProcessed) {
         sprintf(wb.msgbuf, SZ_TOTBYTES, wb.cbProcessed / 1048576.0);
         scrPrintMsg(wb.msgbuf, GREENONBLACK, TRUE);
      } /* endif */
   } /* endif */
   if (!wb.unattended) {
      ULONG ulpos;
      ulpos = scrCursorPos();
      scrPrintMsg(SZ_TESTSTART, REDONBLACK, FALSE);
      if (userPressedEsc()) return FALSE;
      scrCursorMoveTo(ulpos & 0xffff, ulpos >> 16);
      scrPrintMsg(SZ_DELPROMPT, REDONBLACK, FALSE);
   } /* endif */
   if (wb.progress && progress) {
      scrPrintStringAt(SZ_PERCENTSCALE, YELLOWONBLACK, 10,0);
   } /* endif */
   scrCursorMoveTo(18,0);
   scrPrintMsg(SZ_PLSWAIT, GREENONBLACK, FALSE);
   return TRUE;
}


/* --------------------------------------------------------------------------
 - set priority to the maximum level
 - reset the time counter
 ------------------------------------------------------------------------- */
VOID resetTimer(VOID) {
   DosSetPriority(0, PRTYC_TIMECRITICAL, 31, 0);
   wb.startTime = upTime();
}


/* --------------------------------------------------------------------------
 - calculate the elapsed time
 - reset priority to normal
 - move the progress bar to 100 % if not yet there
 - print the elapsed time
 - print the count of processed bytes (when applicable)
 - print the count of processed operations (when applicable)
 - prompt the user to go on with the next test (when not in unattended mode)
 ------------------------------------------------------------------------- */
VOID endTest(BOOL progress) {
   ULONG endTime;
   endTime = upTime();
   wb.elapsedTime = ((wb.startTime > endTime)
                      ?
                      (endTime + 1 + (0xffffffff - wb.startTime))
                      :
                      (endTime - wb.startTime)
                      ) / 1000.0;
   wb.elapsedTotal += wb.elapsedTime;
   DosSetPriority(0, PRTYC_REGULAR, 0, 0);
   if (wb.progress && progress && (wb.prevStep < 320))
      printProgress(wb.prevStep, 320);
   sprintf(wb.msgbuf, SZ_TIMERRESULT, wb.elapsedTime);
   scrPrintStringAt(wb.msgbuf, SKYONBLACK, 17, 0);
   logWriteMsg(wb.msgbuf);
   if (wb.cbProcessed) {
      sprintf(wb.msgbuf, SZ_MIBSPERSEC, (wb.cbProcessed / 1000000.0) / wb.elapsedTime);
      scrPrintStringAt(wb.msgbuf, SKYONBLACK, 18, 0);
      logWriteMsg(wb.msgbuf);
   } /* endif */
   if (wb.operations) {
      wb.operations /= 1000000 * wb.elapsedTime;
      sprintf(wb.msgbuf, SZ_OPPERSEC, wb.operations);
      scrPrintStringAt(wb.msgbuf, SKYONBLACK, 18, 0);
      logWriteMsg(wb.msgbuf);
   } /* endif */
   if (!wb.unattended) pressAnyKey(19, 0);
}


/* --------------------------------------------------------------------------
 Test accuracy of the virtual machine timer by allowing the user compare
 the reported elapsed time with what reported by a real world timer.
------------------------------------------------------------------------- */
BOOL testTimer(VOID) {
   ULONG startTime, endTime, elapsedTime;
   BOOL terminate = FALSE;
   if (!prepareTest(ITEST_TIMER, 0, SZ_TIMERTEST, FALSE)) return FALSE;
   for (;;) {
      scrPrintStringAt(SZ_DELLINE, YELLOWONBLACK, 8, 0);
      scrPrintStringAt(SZ_DELLINE, YELLOWONBLACK, 9, 0);
      scrPrintStringAt(SZ_TIMERSTART, YELLOWONBLACK, 8, 0);
      if (userPressedEsc()) break;
      startTime = upTime();
      scrPrintStringAt(SZ_DELLINE, REDONBLACK, 8, 0);
      scrPrintStringAt(SZ_DELLINE, REDONBLACK, 9, 0);
      scrPrintStringAt(SZ_TIMERSTOP, REDONBLACK, 8, 0);
      terminate = userPressedEsc();
      endTime = upTime();
      elapsedTime = (startTime > endTime)
                    ?
                    (endTime + 1 + (0xffffffff - startTime))
                    :
                    (endTime - startTime);
      sprintf(wb.msgbuf, SZ_TIMERRESULT, elapsedTime / 1000.0);
      scrPrintStringAt(SZ_DELLINE, SKYONBLACK, 12, 0);
      scrPrintStringAt(wb.msgbuf, SKYONBLACK, 12, 0);
      if (terminate) break;
   } /* endfor */
   return TRUE; // just to increase the completed tests counter
}


/* --------------------------------------------------------------------------
 Return time in milliseconds from the system boot.
 ------------------------------------------------------------------------- */
ULONG upTime(VOID) {
   ULONG ul;
   DosQuerySysInfo(QSV_MS_COUNT, QSV_MS_COUNT, &ul, 4);
   return ul;
}


/* --------------------------------------------------------------------------
 Allocate memory printing an error message in case of failure.
 ------------------------------------------------------------------------- */
PVOID tryalloc(ULONG cb) {
   PVOID p;
   if (NULL == (p = malloc(cb)))
      scrPrintErrorMsg(SZERR_ALLOC);
   return p;
}


/* --------------------------------------------------------------------------
 Update the progress bar displaying the state of the current test.
 ------------------------------------------------------------------------- */
ULONG printProgress(ULONG prevstep, ULONG curstep) {
   CHAR progress[32];
   CHAR bar[] = "Û°±²";
   INT len;
   while (prevstep < curstep) {
      prevstep++;
      VioWrtCharStr(bar + (prevstep % 4), 1, 12, (prevstep - 1) / 4, 0);
   } /* endwhile */
   len = sprintf(progress, SZ_PROGRESSSTATE, ((curstep * 5.0) / 16.0));
   VioWrtCharStr(progress, len, 14, 1, 0);
   return curstep;
}


/* --------------------------------------------------------------------------
 Allocate a buffer of 'cb' bytes randomly filling it with words taken from
 the array of random text words. The words are separated by a delimiter
 randomly choosed from an array of string delimiters. The words are also
 uppercased when follow '.', '?', or '!'.
 ------------------------------------------------------------------------- */
PSZ makeRandomText(ULONG cb) {
   PSZ pText, p;
   ULONG i, cbWord, cbWritten, irand, cdelimiters;
   WORDDELIM aDelim[] = ADELIMITERS;
   BOOL capitalize;

   if (NULL == (pText = tryalloc(cb + 28)))
      return NULL;
   cdelimiters = sizeof(aDelim) / sizeof(WORDDELIM);
   srand(0xcfa);
   for (i = 0, p = pText, cbWritten = 0, capitalize = TRUE; cbWritten < cb; ++i) {
      irand = rand() % CRANDWORDS;                  // random word index
      cbWord = strlen(wb.prw->apszWords[irand]);
      memcpy(p, wb.prw->apszWords[irand], cbWord);
      if (capitalize && (*p >= 'a') && (*p <= 'z')) *p &= ~0x20;
      p += cbWord, cbWritten += cbWord;
      irand = rand() % cdelimiters;
      cbWord = strlen(aDelim[irand].delimiter);
      memcpy(p, aDelim[irand].delimiter, cbWord);
      capitalize = aDelim[irand].capitalize;
      p += cbWord, cbWritten += cbWord;
   } /* endfor */
   pText[cb] = 0;
   return pText;
}

