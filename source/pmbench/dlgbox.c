//===========================================================================
// .c :
// -
// -
// --2004  - Alessandro Felice Cantatore
//===========================================================================

#include "pmbench.h"

// definitions --------------------------------------------------------------
#define SZ_MLETEXT \


// prototypes ---------------------------------------------------------------
VOID initDlg(HWND hwnd);
VOID arrangeItems(HWND hwnd, PSWP pswp);
VOID initItemsSwp(HWND hwnd, PSWP pswp);

// globals ------------------------------------------------------------------

MRESULT EXPENTRY dlgprc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         initDlg(hwnd);
         break;
      case WM_WINDOWPOSCHANGED:
         if (((PSWP)mp1)->fl & SWP_SIZE)
            arrangeItems(hwnd, (PSWP)mp1);
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
         break;
   } /* endswitch */
   return (MRESULT)FALSE;
}


static VOID initDlg(HWND hwnd) {
   CHAR buf[1024];
   PSZ pszText, p;
   INT i, j, k;
   for (i = 0; i < 1022; ++i) {
      buf[i] = i % 64 + '!';
      if (i && !(i % 7)) buf[++i] = ' ';
   } /* endfor */
   buf[i] = 0;
   WinSetDlgItemText(hwnd, RI_DTMLE, buf);
   hwnd = WinWindowFromID(hwnd, RI_DTLBOX);
   for (i = 0; i < 64; ++i) {
      sprintf(buf, "listbox item % 4d", i + 1);
      WinInsertLboxItem(hwnd, i, buf);
   } /* endfor */
}


static VOID arrangeItems(HWND hwnd, PSWP pswp) {
   SWP aswp[23];
   RECTL r = {0, 0, pswp->cx, pswp->cy};
   WinCalcFrameRect(hwnd, &r, TRUE);
   initItemsSwp(hwnd, aswp);
   aswp[RI_DTGRP3].x = r.xLeft + 2;
   aswp[RI_DTGRP3].y = r.yBottom + 2;
   aswp[RI_DTGRP3].cx = (r.xRight - aswp[RI_DTGRP3].x) / 2 - 1;
   aswp[RI_DTGRP3].cy = (r.yTop - aswp[RI_DTGRP3].y) / 2 - 5;
   aswp[RI_DTBTN9].x = aswp[RI_DTGRP3].x + 6;
   aswp[RI_DTBTN9].y = aswp[RI_DTGRP3].y + 6;
   aswp[RI_DTBTN9].cx = aswp[RI_DTGRP3].cx / 2 - 15;
   aswp[RI_DTBTN9].cy = (aswp[RI_DTGRP3].cy - 40) / 5;
   aswp[RI_DTBTN10].x = aswp[RI_DTBTN9].x + aswp[RI_DTBTN9].cx + 3;
   aswp[RI_DTBTN10].y = aswp[RI_DTBTN9].y;
   aswp[RI_DTBTN10].cx = aswp[RI_DTBTN9].cx;
   aswp[RI_DTBTN10].cy = aswp[RI_DTBTN9].cy;
   aswp[RI_DTBTN7].x = aswp[RI_DTBTN9].x;
   aswp[RI_DTBTN7].y = aswp[RI_DTBTN9].y + aswp[RI_DTBTN9].cy + 4;
   aswp[RI_DTBTN7].cx = aswp[RI_DTBTN9].cx;
   aswp[RI_DTBTN7].cy = aswp[RI_DTBTN9].cy;
   aswp[RI_DTBTN8].x = aswp[RI_DTBTN10].x;
   aswp[RI_DTBTN8].y = aswp[RI_DTBTN7].y;
   aswp[RI_DTBTN8].cx = aswp[RI_DTBTN9].cx;
   aswp[RI_DTBTN8].cy = aswp[RI_DTBTN9].cy;
   aswp[RI_DTBTN5].x = aswp[RI_DTBTN9].x;
   aswp[RI_DTBTN5].y = aswp[RI_DTBTN7].y + aswp[RI_DTBTN7].cy + 4;
   aswp[RI_DTBTN5].cx = aswp[RI_DTBTN9].cx;
   aswp[RI_DTBTN5].cy = aswp[RI_DTBTN9].cy;
   aswp[RI_DTBTN6].x = aswp[RI_DTBTN10].x;
   aswp[RI_DTBTN6].y = aswp[RI_DTBTN5].y;
   aswp[RI_DTBTN6].cx = aswp[RI_DTBTN9].cx;
   aswp[RI_DTBTN6].cy = aswp[RI_DTBTN9].cy;
   aswp[RI_DTBTN3].x = aswp[RI_DTBTN9].x;
   aswp[RI_DTBTN3].y = aswp[RI_DTBTN5].y + aswp[RI_DTBTN5].cy + 4;
   aswp[RI_DTBTN3].cx = aswp[RI_DTBTN9].cx;
   aswp[RI_DTBTN3].cy = aswp[RI_DTBTN9].cy;
   aswp[RI_DTBTN4].x = aswp[RI_DTBTN10].x;
   aswp[RI_DTBTN4].y = aswp[RI_DTBTN3].y;
   aswp[RI_DTBTN4].cx = aswp[RI_DTBTN9].cx;
   aswp[RI_DTBTN4].cy = aswp[RI_DTBTN9].cy;
   aswp[RI_DTBTN1].x = aswp[RI_DTBTN9].x;
   aswp[RI_DTBTN1].y = aswp[RI_DTBTN3].y + aswp[RI_DTBTN3].cy + 4;
   aswp[RI_DTBTN1].cx = aswp[RI_DTBTN9].cx;
   aswp[RI_DTBTN1].cy = aswp[RI_DTBTN9].cy;
   aswp[RI_DTBTN2].x = aswp[RI_DTBTN10].x;
   aswp[RI_DTBTN2].y = aswp[RI_DTBTN1].y;
   aswp[RI_DTBTN2].cx = aswp[RI_DTBTN9].cx;
   aswp[RI_DTBTN2].cy = aswp[RI_DTBTN9].cy;
   aswp[RI_DTGRP4].x = aswp[RI_DTGRP3].x + aswp[RI_DTGRP3].cx + 3;
   aswp[RI_DTGRP4].y = aswp[RI_DTGRP3].y;
   aswp[RI_DTGRP4].cx = aswp[RI_DTGRP3].cx;
   aswp[RI_DTGRP4].cy = aswp[RI_DTGRP3].cy;
   aswp[RI_DTLBOX].x = aswp[RI_DTGRP4].x + 6;
   aswp[RI_DTLBOX].y = aswp[RI_DTGRP4].y + 6;
   aswp[RI_DTLBOX].cx = aswp[RI_DTGRP4].cx - 12;
   aswp[RI_DTLBOX].cy = aswp[RI_DTGRP4].cy - 28;
   aswp[RI_DTGRP1].x = aswp[RI_DTGRP3].x;
   aswp[RI_DTGRP1].y = aswp[RI_DTGRP3].y + aswp[RI_DTGRP3].cy + 10;
   aswp[RI_DTGRP1].cx = aswp[RI_DTGRP3].cx;
   aswp[RI_DTGRP1].cy = aswp[RI_DTGRP3].cy;
   aswp[RI_DTMLE].x = aswp[RI_DTGRP1].x + 6;
   aswp[RI_DTMLE].y = aswp[RI_DTGRP1].y + 6;
   aswp[RI_DTMLE].cx = aswp[RI_DTGRP1].cx - 12;
   aswp[RI_DTMLE].cy = aswp[RI_DTGRP1].cy - 28;
   aswp[RI_DTDTGRP2].x = aswp[RI_DTGRP4].x;
   aswp[RI_DTDTGRP2].y = aswp[RI_DTGRP1].y;
   aswp[RI_DTDTGRP2].cx = aswp[RI_DTGRP3].cx;
   aswp[RI_DTDTGRP2].cy = aswp[RI_DTGRP3].cy;
   aswp[RI_DTRDBTN7].x = aswp[RI_DTDTGRP2].x + 8;
   aswp[RI_DTRDBTN7].y = aswp[RI_DTDTGRP2].y + 8;
   aswp[RI_DTRDBTN7].cx = aswp[RI_DTDTGRP2].cx - 12;
   aswp[RI_DTRDBTN7].cy = (aswp[RI_DTDTGRP2].cy - 24) / 7;
   aswp[RI_DTRDBTN6].x = aswp[RI_DTRDBTN7].x;
   aswp[RI_DTRDBTN6].y = aswp[RI_DTRDBTN7].y + aswp[RI_DTRDBTN7].cy;
   aswp[RI_DTRDBTN6].cx = aswp[RI_DTRDBTN7].cx;
   aswp[RI_DTRDBTN6].cy = aswp[RI_DTRDBTN7].cy;
   aswp[RI_DTRDBTN5].x = aswp[RI_DTRDBTN7].x;
   aswp[RI_DTRDBTN5].y = aswp[RI_DTRDBTN6].y + aswp[RI_DTRDBTN6].cy;
   aswp[RI_DTRDBTN5].cx = aswp[RI_DTRDBTN7].cx;
   aswp[RI_DTRDBTN5].cy = aswp[RI_DTRDBTN7].cy;
   aswp[RI_DTRDBTN4].x = aswp[RI_DTRDBTN7].x;
   aswp[RI_DTRDBTN4].y = aswp[RI_DTRDBTN5].y + aswp[RI_DTRDBTN5].cy;
   aswp[RI_DTRDBTN4].cx = aswp[RI_DTRDBTN7].cx;
   aswp[RI_DTRDBTN4].cy = aswp[RI_DTRDBTN7].cy;
   aswp[RI_DTRDBTN3].x = aswp[RI_DTRDBTN7].x;
   aswp[RI_DTRDBTN3].y = aswp[RI_DTRDBTN4].y + aswp[RI_DTRDBTN4].cy;
   aswp[RI_DTRDBTN3].cx = aswp[RI_DTRDBTN7].cx;
   aswp[RI_DTRDBTN3].cy = aswp[RI_DTRDBTN7].cy;
   aswp[RI_DTRDBTN2].x = aswp[RI_DTRDBTN7].x;
   aswp[RI_DTRDBTN2].y = aswp[RI_DTRDBTN3].y + aswp[RI_DTRDBTN3].cy;
   aswp[RI_DTRDBTN2].cx = aswp[RI_DTRDBTN7].cx;
   aswp[RI_DTRDBTN2].cy = aswp[RI_DTRDBTN7].cy;
   aswp[RI_DTRDBTN1].x = aswp[RI_DTRDBTN7].x;
   aswp[RI_DTRDBTN1].y = aswp[RI_DTRDBTN2].y + aswp[RI_DTRDBTN2].cy;
   aswp[RI_DTRDBTN1].cx = aswp[RI_DTRDBTN7].cx;
   aswp[RI_DTRDBTN1].cy = aswp[RI_DTRDBTN7].cy;
   WinSetMultWindowPos(WinQueryAnchorBlock(hwnd), aswp, 23);
}


static VOID initItemsSwp(HWND hwnd, PSWP pswp) {
   HWND ahwnd[23];
   INT i;
   WinMultWindowFromIDs(hwnd, ahwnd, RI_DTGRP1, RI_DTLBOX);
   for (i = RI_DTGRP1; i <= RI_DTLBOX; ++i, pswp++) {
      pswp->hwnd = ahwnd[i];
      pswp->hwndInsertBehind = 0;
      pswp->fl = SWP_SIZE | SWP_MOVE;
   } /* endfor */
}
