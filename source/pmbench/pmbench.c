

#include "pmbench.h"

// prototypes ---------------------------------------------------------------
VOID applInit(VOID);
HAB pmInit(PHMQ phmq);
HWND winMain(HAB hab, ULONG fl, PSZ pszClass,
             PSZ pszTitle, ULONG cx, ULONG cy,
             PFNWP pfnClient);
VOID winMessageLoop(HAB hab);
VOID pmEnd(HWND hwnd, HMQ hmq, HAB hab);
MRESULT EXPENTRY clientProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID setposition(HWND hwnd, LONG cx, LONG cy);
VOID onCommand(HWND hwnd, ULONG id);
VOID createChildren(HWND hwnd);

// --------------------------------------------------------------------------

// globals ------------------------------------------------------------------
GLOBAL g;

INT main(VOID) {
   HWND hwnd = NULLHANDLE;
   HAB hab   = NULLHANDLE;
   HMQ hmq   = NULLHANDLE;

   applInit();
   if (NULLHANDLE != (hab = pmInit(&hmq))) {
      hwnd = winMain(hab, FRAMEFLAGS, SZ_CLASS, SZ_TITLE, 640, 480, clientProc);
      if (hwnd) winMessageLoop(hab);
   } /* endif */
   pmEnd(hwnd, hmq, hab);
   return 0;
}


static VOID applInit(VOID) {
   INT i;
   ULONG aiter[] = AITERATIONS;
   g.option.factor = 1;
   g.option.save = TRUE;
   applWorkDirGet(g.option.file);
   strcat(g.option.file, "\\pmbench.txt");
   // init the default iterations
   for (i = 0; i < IT_COUNT; ++i) {
      g.atest[i].defIter = aiter[i];
   } /* endfor */
}


static HAB pmInit(PHMQ phmq) {
   HAB hab;
   if (NULLHANDLE == (*phmq = WinCreateMsgQueue(hab = WinInitialize(0), 0)))
      return NULLHANDLE;
   return hab;
}


static HWND winMain(HAB hab, ULONG fl, PSZ pszClass,
             PSZ pszTitle, ULONG cx, ULONG cy,
             PFNWP pfnClient) {
   HWND hwndf, hwndc;

   WinRegisterClass(hab, pszClass, pfnClient, CS_SIZEREDRAW, 0);
   hwndf = WinCreateStdWindow(HWND_DESKTOP, 0, &fl, pszClass, pszTitle,
                              0, NULLHANDLE, 1, &hwndc);
   if (hwndf) setposition(hwndf, cx, cy);
   return hwndf;
}


static VOID winMessageLoop(HAB hab) {
   QMSG qmsg;
   while (WinGetMsg(hab, &qmsg, NULLHANDLE, 0, 0 ))
      WinDispatchMsg(hab, &qmsg);
}


static VOID pmEnd(HWND hwnd, HMQ hmq, HAB hab) {
   INT i;
   PSZ p;
   PSZ adescr[] = ADESCRIPTIONS;
   if (hwnd) WinDestroyWindow(hwnd);
   if (hmq)  WinDestroyMsgQueue(hmq);
   if (hab)  WinTerminate(hab);
   if (g.option.save) {
      p = g.buf + sprintf(g.buf, "%s\r\n%s\r\n", SZ_HEADER, SZ_HEADERSEP);
      for (i = 0; i < IT_COUNT; ++i) {
         if (g.atest[i].done) {
            p += sprintf(p, "%-37.37s ³ % 14.3f ³ % 14u ³ % 14.3f\r\n",
                         adescr[i], g.atest[i].elapsed / 1000.0,
                         g.atest[i].operations,
                         g.atest[i].operations / (g.atest[i].elapsed / 1000.0));
         } /* endif */
      } /* endfor */
      fmWriteData(g.option.file, g.buf, (ULONG)(p - g.buf));
   } /* endif */
}



MRESULT EXPENTRY clientProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_CREATE:
         createChildren(hwnd);
         break;
      case WM_COMMAND:
         onCommand(hwnd, (ULONG)mp1);
         break;
      case WM_PAINT: {
         RECTL rcl;
         HPS hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);
         WinQueryWindowRect(hwnd, &rcl);
         if (g.buf[0])
            WinDrawText(hps, -1, g.buf, &rcl, CLR_BLUE, CLR_PALEGRAY,
                        DT_CENTER | DT_VCENTER | DT_ERASERECT);
         else
            WinFillRect(hps, &rcl, CLR_PALEGRAY);
         WinEndPaint(hps);
      }  break;
      default:
         return WinDefWindowProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return (MRESULT)FALSE;
}


static VOID setposition(HWND hwnd, LONG cx, LONG cy) {
   RECTL rd, rw;
   WinQueryWindowRect(HWND_DESKTOP, &rd);
   rw.xLeft = 0, rw.yBottom = 0, rw.xRight = cx, rw.yTop = cy;
   if (rd.yTop == cy) rw.yTop -= 40;
   WinCalcFrameRect(hwnd, &rw, FALSE);
   rw.xRight -= rw.xLeft, rw.yTop -= rw.yBottom;
   rw.xLeft = (rd.xRight - rw.xRight) / 2;
   rw.yBottom = (rd.yTop - rw.yTop) / 2;
   WinSetPresParam(WinWindowFromID(hwnd, FID_CLIENT), PP_FONTNAMESIZE,
                   13, "5.System VIO");
   WinSetWindowPos(hwnd, HWND_TOP, rw.xLeft, rw.yBottom, rw.xRight, rw.yTop,
                   SWP_SIZE | SWP_MOVE | SWP_SHOW | SWP_ACTIVATE | SWP_ZORDER);

}


static VOID onCommand(HWND hwnd, ULONG id) {
   switch (id) {
      case MI_RUNALL:
         g.runAllTests = TRUE;
         testDrawBitmapFluidity(hwnd);
         testDrawBitmapSpeed(hwnd);
         testDrawBitmapStretch(hwnd);
         testDrawLines(hwnd);
         testFillRectanges(hwnd);
         testDrawText(hwnd);
         testLoadDialog(hwnd);
         testMoveWindow(hwnd);
         testSizeWindow(hwnd);
         g.runAllTests = FALSE;
         showResults(hwnd, TRUE);
         break;
      case MI_RESULTS:
         if (!g.showResults) showResults(hwnd, TRUE);
         break;
      case MI_OPTIONS:
         WinDlgBox(HWND_DESKTOP, hwnd, optionProc, 0, RI_DLGOPTIONS, NULL);
         break;
      case MI_FLUIDITY:
         testDrawBitmapFluidity(hwnd);
         break;
      case MI_DRAWBITMAP:
         testDrawBitmapSpeed(hwnd);
         break;
      case MI_BMPSTRETCH:
         testDrawBitmapStretch(hwnd);
         break;
      case MI_DRAWLINES:
         testDrawLines(hwnd);
         break;
      case MI_FILLRECTANGLES:
         testFillRectanges(hwnd);
         break;
      case MI_DRAWTEXT:
         testDrawText(hwnd);
         break;
      case MI_LOADDLG:
         testLoadDialog(hwnd);
         break;
      case MI_MOVEWINDOW:
         testMoveWindow(hwnd);
         break;
      case MI_SIZEWINDOW:
         testSizeWindow(hwnd);
         break;
   } /* endswitch */
}


static VOID createChildren(HWND hwnd) {
   ULONG pp[4];
   RECTL r;
   WinQueryWindowRect(HWND_DESKTOP, &r);
   r.yTop = ((r.xRight == 640) ? 440 : 480), r.xRight = 640;
   pp[0] = sizeof(ULONG) * 3;
   pp[1] = PP_BACKGROUNDCOLOR;
   pp[2] = sizeof(ULONG);
   pp[3] = 0xcccccc;
   WinCreateWindow(hwnd, WC_STATIC, SZ_HEADER, SS_TEXT | DT_BOTTOM | DT_LEFT,
                   4, r.yTop - 20, r.xRight - 8, 20, hwnd, HWND_TOP, ID_CAPTION,
                   NULL, &pp);
   WinCreateWindow(hwnd, WC_LISTBOX, "", LS_NOADJUSTPOS,
                   0, 0, r.xRight, r.yTop - 20, hwnd, HWND_TOP, ID_LISTBOX,
                   NULL, NULL);
}
