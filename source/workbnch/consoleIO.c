//===========================================================================
// consoleIO.c : utility funcitions to write to the screen or get keyboard
//               input.
// 2004-01-22  - Alessandro Felice Cantatore
//===========================================================================


#include "workbnch.h"

// definitions --------------------------------------------------------------


// prototypes ---------------------------------------------------------------


// globals ------------------------------------------------------------------

// set a color via the ANSI sequences
VOID scrColorSet(PSZ pszClr) {
   fmWrite(1, pszClr, strlen(pszClr));
}

// write an ordinary message
VOID scrPrintMsg(PSZ pszMsg, PSZ pszClr, BOOL blog) {
   ULONG cb;
   if (pszMsg) {
      cb = strlen(pszMsg);
      if (pszClr) scrColorSet(pszClr);
      fmWrite(1, pszMsg, cb);
      if (blog & wb.hlog) fmWrite(wb.hlog, pszMsg, cb);
   } /* endif */
}

// write an error message
VOID scrPrintErrorMsg(PSZ pszMsg) {
   ULONG cb;
   DosBeep(80,100),DosBeep(440,180),DosBeep(80,100);
   if (pszMsg) {
      scrColorSet(REDONBLACK);
      cb = strlen(pszMsg);
      fmWrite(2, pszMsg, cb);
      if (wb.hlog) fmWrite(wb.hlog, pszMsg, cb);
      if (!wb.unattended) pressAnyKey(-1, -1);
   } /* endif */
}

// write a string at a given row,column position
VOID scrPrintStringAt(PSZ pszText, PSZ pszColor, UINT row, UINT column) {
   scrCursorMoveTo(row, column);
   scrPrintMsg(pszText, pszColor, FALSE);
}

// clear the screen
VOID scrClean(VOID) {
   CHAR buf[80*25];
   memset(buf, ' ', sizeof(buf));
   fmWrite(1, buf, sizeof(buf));
   scrCursorMoveTo(0,0);
}

VOID scrModeSetDefault(VOID) {
   system("mode co80,25");
}

// set the cursor position
VOID scrCursorMoveTo(UINT row, UINT col) {
   VioSetCurPos(row, col, 0);
}

// return the current cursor position
ULONG scrCursorPos(VOID) {
   ULONG ulrow, ulcol;
   if (VioGetCurPos((PUSHORT)&ulrow, (PUSHORT)&ulcol, 0)) return 0;
   return ulrow | (ulcol << 16);
}

// return the pressed key
ULONG kbdGetChar(VOID) {
   KBDKEYINFO kki;
   if (KbdCharIn(&kki, 0, 0)) return 0;
   return (ULONG)(kki.chChar | (kki.chScan << 8));
}

VOID pressAnyKey(INT row, INT col) {
   ULONG ulpos;
   if (row < 0) {
      ulpos = scrCursorPos();
      row = ulpos & 0xffff;
      col = ulpos >> 16;
   } /* endif */
   scrPrintStringAt(SZ_PAUSE, REDONBLACK, row, col);
   kbdGetChar();
}

// Wait for a key to be pressed, return TRUE if the user pressed ESC
ULONG userPressedEsc(VOID) {
   return (1 == (kbdGetChar() >> 8));
}
