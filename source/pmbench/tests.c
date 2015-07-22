//===========================================================================
// .c :
// -
// -
// --2004  - Alessandro Felice Cantatore
//===========================================================================


#include "pmbench.h"

// definitions --------------------------------------------------------------
#define ACOLORS  { \
CLR_WHITE, CLR_BLUE, CLR_RED, CLR_PINK, CLR_GREEN, CLR_CYAN, CLR_YELLOW, \
CLR_DARKGRAY, CLR_DARKBLUE, CLR_DARKRED, CLR_DARKPINK, CLR_DARKGREEN, \
CLR_DARKCYAN, CLR_BROWN, CLR_PALEGRAY }

#define CCOLORS 15
#define CHARSRANGE ('~' - ' ')

// prototypes ---------------------------------------------------------------


VOID showResults(HWND hwnd, BOOL bShow) {
   INT i;
   PSZ adescr[] = ADESCRIPTIONS;
   HWND hLbox = WinWindowFromID(hwnd, ID_LISTBOX);
   g.showResults = bShow;
   WinShowControl(hwnd, ID_CAPTION, bShow);
   WinShowWindow(hLbox, bShow);
   if (bShow) {
      WinSendMsg(hLbox, LM_DELETEALL, MPVOID, MPVOID);
      for (i = 0; i < IT_COUNT; ++i) {
         if (g.atest[i].done) {
            sprintf(g.buf, "%-37.37s ³ % 14.3f ³ % 14u ³ % 14.3f",
                    adescr[i], g.atest[i].elapsed / 1000.0,
                    g.atest[i].operations,
                    g.atest[i].operations / (g.atest[i].elapsed / 1000.0));
            WinInsertLboxItem(hLbox, LIT_END, g.buf);
         } /* endif */
      } /* endfor */
      g.buf[0] = 0;
   } /* endif */
}


VOID enableMenuBar(HWND hwnd, BOOL bEnable) {
   hwnd = WinWindowFromID(WinQueryWindow(hwnd, QW_PARENT), FID_MENU);
   WinEnableMenuItem(hwnd, MI_FILE, bEnable);
   WinEnableMenuItem(hwnd, MI_TESTBMP, bEnable);
   WinEnableMenuItem(hwnd, MI_GPI, bEnable);
   WinEnableMenuItem(hwnd, MI_TESTPM, bEnable);
}


ULONG upTime(VOID) {
   ULONG ul;
   DosQuerySysInfo(QSV_MS_COUNT, QSV_MS_COUNT, &ul, 4);
   return ul;
}


HPS psInit(HWND hwnd, PRECTL pr) {
   HPS hps;
   RECTL r;
   if (g.showResults) showResults(hwnd, FALSE);
   WinQueryWindowRect(hwnd, &r);
   hps = WinGetPS(hwnd);
   WinFillRect(hps, &r, CLR_BLACK);
   if (pr) *pr = r;
   return hps;
}


ULONG testStart(HWND hwnd, ULONG idTest, ULONG operations) {
   enableMenuBar(hwnd, FALSE);
   g.atest[idTest].totIter = g.atest[idTest].defIter * g.option.factor;
   g.atest[idTest].operations = g.atest[idTest].totIter * operations;
   if (g.option.prompt)
      WinMessageBox(HWND_DESKTOP, hwnd, SZ_PROMPT, "", 200, MB_OK | MB_ICONEXCLAMATION);
   DosSetPriority(0, PRTYC_TIMECRITICAL, 31, 0);
   return upTime();
}


VOID testEnd(HWND hwnd, HPS hps, ULONG startTime, ULONG idTest) {
   ULONG endTime;
   endTime = upTime();
   DosSetPriority(0, PRTYC_REGULAR, 0, 0);
   enableMenuBar(hwnd, TRUE);
   g.atest[idTest].elapsed = (startTime > endTime)
                 ?
                 (endTime + 1 + (0xffffffff - startTime))
                 :
                 (endTime - startTime);
   if (hps) WinReleasePS(hps);
   if (!g.runAllTests) {
      sprintf(g.buf, SZ_SINGLERESULT,
              g.atest[idTest].elapsed / 1000.0,
              g.atest[idTest].operations,
              g.atest[idTest].operations / (g.atest[idTest].elapsed / 1000.0));
      WinInvalidateRect(hwnd, NULL, FALSE);
   } /* endif */
   g.atest[idTest].done = TRUE;
}


HWND dlgTestInit(HWND hwnd, PHWND phframe, PSWP pswp, PINT pcx, PINT pcy) {
   HWND hdlg;
   RECTL rframe, rdesk, rdlg;
   SWP swp;
   HPS hps;
   if (g.showResults) showResults(hwnd, FALSE);
   WinQueryWindowRect(HWND_DESKTOP, &rdesk);
   *phframe = WinQueryWindow(hwnd, QW_PARENT);
   WinQueryWindowPos(*phframe, pswp);
   rframe = rdesk;
   WinCalcFrameRect(*phframe, &rframe, FALSE);
   WinSetWindowPos(*phframe, 0, rframe.xLeft, rframe.yBottom,
                   rframe.xRight - rframe.xLeft, rframe.yTop - rframe.yBottom,
                   SWP_SIZE | SWP_MOVE);
   hps = WinGetPS(hwnd);
   rframe.xRight -= rframe.xLeft, rframe.yTop -= rframe.yBottom;
   rframe.xLeft = rframe.yBottom = 0;
   WinFillRect(hps, &rframe, CLR_BLACK);
   WinReleasePS(hps);
   hdlg = WinLoadDlg(HWND_DESKTOP, hwnd, dlgprc, 0, RI_DLGTEST, NULL);
   *pcx = rdesk.xRight, *pcy = rdesk.yTop;
   return hdlg;
}


BOOL EXPENTRY gMoveTo(HPS hps, INT x, INT y) {
   return GpiMove(hps, (PPOINTL)&x);
}

BOOL EXPENTRY gLineTo(HPS hps, INT x, INT y) {
   return GpiLine(hps, (PPOINTL)&x);
}

INT gColor(HPS hps, INT index) {
   INT color[] = ACOLORS;
   GpiSetColor(hps, color[index]);
   ++index;
   if (index == CCOLORS) index = 0;
   return index;
}

VOID testDrawBitmapFluidity(HWND hwnd) {
   HPS hps;
   HBITMAP hbmp;
   INT i;
   RECTL r;
   POINTL pt, ptinc;
   ULONG startTime;

   hps = psInit(hwnd, &r);
   hbmp = GpiLoadBitmap(hps, NULLHANDLE, IBMP_SFERAM, 0, 0);
   startTime = testStart(hwnd, IT_BMPSMOOTH, 1);
   for (i = 0, pt.x = 0, pt.y = 0, ptinc.x = 2, ptinc.y = 2;
        i < g.atest[IT_BMPSMOOTH].totIter;
        ++i) {
      WinDrawBitmap(hps, hbmp, NULL, &pt, 0, 0, DBM_NORMAL);
      pt.x += ptinc.x, pt.y += ptinc.y;
      if (pt.x > (r.xRight - 56)) {
         ptinc.x = -2, pt.x -= 2;
      } else if (pt.x < 0) {
         ptinc.x = 2, pt.x += 2;
      } /* endif */
      if (pt.y > (r.yTop - 56)) {
         ptinc.y = -2, pt.y -= 2;
      } else if (pt.y < 0) {
         ptinc.y = 2, pt.y += 2;
      } /* endif */
      DosSleep(1);
   } /* endfor */
   testEnd(hwnd, hps, startTime, IT_BMPSMOOTH);
   GpiDeleteBitmap(hbmp);
}


VOID testDrawBitmapSpeed(HWND hwnd) {
   HPS hps;
   HBITMAP hbmp;
   INT i;
   RECTL r;
   POINTL pt, ptinc;
   ULONG startTime;

   hps = psInit(hwnd, &r);
   hbmp = GpiLoadBitmap(hps, NULLHANDLE, IBMP_SFERAS, 0, 0);
   startTime = testStart(hwnd, IT_BMPDRAW, 1);
   for (i = 0, pt.x = 0, pt.y = 0, ptinc.x = 1, ptinc.y = 1;
        i < g.atest[IT_BMPDRAW].totIter;
        ++i) {
      WinDrawBitmap(hps, hbmp, NULL, &pt, 0, 0, DBM_NORMAL);
      pt.x += ptinc.x, pt.y += ptinc.y;
      if (pt.x > (r.xRight - 50)) {
         ptinc.x = -1, pt.x -= 2;
      } else if (pt.x < 0) {
         ptinc.x = 1, pt.x += 2;
      } /* endif */
      if (pt.y > (r.yTop - 50)) {
         ptinc.y = -1, pt.y -= 2;
      } else if (pt.y < 0) {
         ptinc.y = 1, pt.y += 2;
      } /* endif */
   } /* endfor */
   testEnd(hwnd, hps, startTime, IT_BMPDRAW);
   GpiDeleteBitmap(hbmp);
}


VOID testDrawBitmapStretch(HWND hwnd) {
   HPS hps;
   HBITMAP hbmp;
   INT i;
   RECTL r, rd;
   POINTL ptinc;
   ULONG startTime;

   hps = psInit(hwnd, &r);
   hbmp = GpiLoadBitmap(hps, NULLHANDLE, IBMP_SFERAL, 0, 0);
   startTime = testStart(hwnd, IT_BMPSTRETCH, 1);
   rd.xLeft   = r.xRight / 2 - 4;
   rd.yBottom = r.yTop / 2 - 4;
   rd.xRight  = rd.xLeft + 8;
   rd.yTop    = rd.yBottom + 8;
   for (i = 0, ptinc.x = 1, ptinc.y = 1; i < g.atest[IT_BMPSTRETCH].totIter; ++i) {
      WinDrawBitmap(hps, hbmp, NULL, (PPOINTL)&rd, 0, 0, DBM_STRETCH);
      rd.xLeft   -= ptinc.x;
      rd.yBottom -= ptinc.y;
      rd.xRight  += ptinc.x;
      rd.yTop    += ptinc.y;
      if (rd.yBottom < -220) {
         rd.xLeft += 2, rd.yBottom += 2, rd.xRight -= 2, rd.yTop -= 2;
         ptinc.x = -1, ptinc.y = -1;
      } else if ((rd.yTop - rd.yBottom) < 8) {
         rd.xLeft -= 2, rd.yBottom -= 2, rd.xRight += 2, rd.yTop += 2;
         ptinc.x = 1, ptinc.y = 1;
      } /* endif */
   } /* endfor */
   testEnd(hwnd, hps, startTime, IT_BMPSTRETCH);
   GpiDeleteBitmap(hbmp);
}


VOID testDrawLines(HWND hwnd) {
   HPS hps;
   INT i, icolor, xl, xr, yb, yt;
   RECTL r;
   ULONG startTime;

   hps = psInit(hwnd, &r);
   startTime = testStart(hwnd, IT_LINES, r.yTop / 2);
   for (i = 0; i < g.atest[IT_LINES].totIter; ++i) {
      icolor = i % 15;
      xl = 0, yb = 0;
      gMoveTo(hps, xl, yb);
      for (xr = r.xRight - 1, yt = r.yTop - 1; yb < yt; ++xl, ++yb, --xr, --yt) {
         icolor = gColor(hps, icolor);
         gLineTo(hps, xr, yb);
         gLineTo(hps, xr, yt);
         gLineTo(hps, xl, yt);
         gLineTo(hps, xl, yb + 1);
      } /* endfor */
   } /* endfor */
   testEnd(hwnd, hps, startTime, IT_LINES);
}


VOID testFillRectanges(HWND hwnd) {
   HPS hps;
   INT i, icolor;
   RECTL r, rd;
   ULONG startTime;
   INT color[] = ACOLORS;

   hps = psInit(hwnd, &r);
   startTime = testStart(hwnd, IT_DRAWRECT, (r.yTop / 2) - 2);
   for (i = 0, icolor = 0; i < g.atest[IT_DRAWRECT].totIter; ++i) {
      rd = r;
      while ((rd.yBottom - 2) < rd.yTop) {
         WinFillRect(hps, &rd, color[icolor]);
         if (++icolor == 15) icolor = 0;
         rd.xLeft++, rd.yBottom++, rd.xRight--, rd.yTop--;
      } /* endwhile */
   } /* endfor */
   testEnd(hwnd, hps, startTime, IT_DRAWRECT);
}


VOID testDrawText(HWND hwnd) {
   HPS hps;
   INT i, icolor;
   RECTL r, rt;
   ULONG startTime;
   CHAR testtext[CHARSRANGE + 1];
   INT color[] = ACOLORS;
   for (i = 0; i < CHARSRANGE; ++i) {
      testtext[i] = ' ' + i + 1;
   } /* endfor */
   testtext[CHARSRANGE] = 0;
   hps = psInit(hwnd, &r);
   startTime = testStart(hwnd, IT_DRAWTEXT, CHARSRANGE);
   for (i = 0, icolor = 0; i < g.atest[IT_DRAWTEXT].totIter; ++i) {
      rt = r;
      rt.yBottom = r.yTop - 13;
      while (rt.yTop > 0) {
         WinDrawText(hps, CHARSRANGE, testtext, &rt, CLR_BLACK, color[icolor],
                     DT_LEFT | DT_VCENTER | DT_ERASERECT);
         rt.yBottom -= 13, rt.yTop -= 13;
         if (++icolor == 15) icolor = 0;
      } /* endwhile */
   } /* endfor */
   testEnd(hwnd, hps, startTime, IT_DRAWTEXT);
}


VOID testLoadDialog(HWND hwnd) {
   INT i;
   ULONG startTime;
   HWND hdlg;
   WinReleasePS(psInit(hwnd, NULL));
   startTime = testStart(hwnd, IT_LOADDLG, 1);
//   WinDlgBox(HWND_DESKTOP, hwnd, dlgprc, 0, RI_DLGTEST, NULL);
   for (i = 0; i < g.atest[IT_LOADDLG].totIter; ++i) {
      hdlg = WinLoadDlg(HWND_DESKTOP, hwnd, dlgprc, 0, RI_DLGTEST, NULL);
      WinDestroyWindow(hdlg);
   } /* endfor */
   testEnd(hwnd, NULLHANDLE, startTime, IT_LOADDLG);
}


VOID testMoveWindow(HWND hwnd) {
   INT i, x, y, dx, dy, cxDesk, cyDesk;
   ULONG startTime;
   HWND hdlg, hframe;
   RECTL rdlg;
   SWP swp;
   HPS hps;
   hdlg = dlgTestInit(hwnd, &hframe, &swp, &cxDesk, &cyDesk);
   WinQueryWindowRect(hdlg, &rdlg);
   startTime = testStart(hwnd, IT_MOVEWIN, 1);
   for (i = 0, x = 0, y = 0, dx = 1, dy = 1; i < g.atest[IT_MOVEWIN].totIter; ++i) {
      WinSetWindowPos(hdlg, 0, x, y, 0, 0, SWP_MOVE);
      x += dx, y += dy;
      if ((x + rdlg.xRight) > cxDesk) {
         dx = -1, x -= 2;
      } else if (x < 0) {
         dx = 1, x = 1;
      } /* endif */
      if ((y + rdlg.yTop) > cyDesk) {
         dy = -1, y -= 2;
      } else if (y < 0) {
         dy = 1, y = 1;
      } /* endif */
   } /* endfor */
   testEnd(hwnd, NULLHANDLE, startTime, IT_MOVEWIN);
   WinDestroyWindow(hdlg);
   WinSetWindowPos(hframe, 0, swp.x, swp.y, swp.cx, swp.cy, SWP_SIZE | SWP_MOVE);
}


VOID testSizeWindow(HWND hwnd) {
   INT i, cx, cy, dcx, dcy, cxDesk, cyDesk;
   ULONG startTime;
   HWND hdlg, hframe;
   SWP swp;

   hdlg = dlgTestInit(hwnd, &hframe, &swp, &cxDesk, &cyDesk);
   WinSetWindowPos(hdlg, 0, 0, 0, 159, 159, SWP_SIZE | SWP_MOVE);
   startTime = testStart(hwnd, IT_SIZEWIN, 1);
   for (i = 0, cx = 160, cy = 160, dcx = 1, dcy = 1; i < g.atest[IT_SIZEWIN].totIter; ++i) {
      WinSetWindowPos(hdlg, 0, 0, 0, cx, cy, SWP_SIZE);
      cx += dcx, cy += dcy;
      if (cy > cyDesk) {
         dcx = -1, dcy = -1, cx -= 2, cy -= 2;
      } else if (cy < 159) {
         dcx = 1, dcy = 1, cx += 2, cy += 2;
      } /* endif */
   } /* endfor */
   testEnd(hwnd, NULLHANDLE, startTime, IT_SIZEWIN);
   WinDestroyWindow(hdlg);
   WinSetWindowPos(hframe, 0, swp.x, swp.y, swp.cx, swp.cy, SWP_SIZE | SWP_MOVE);
}



