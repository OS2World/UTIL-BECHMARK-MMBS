#define INCL_GPI
#define INCL_WIN
#define INCL_DOSFILEMGR
#define INCL_DOSPROCESS
#define INCL_DOSMISC
#include <os2.h>
#include <winutil.h>
#include <string.h>
#include <stdio.h>
#include "resources.h"
#include "textmsgs.h"
#include "fileUtil.h"

#define SZ_CLASS    "benchclient"

#define FRAMEFLAGS   FCF_TITLEBAR       \
                   | FCF_SYSMENU        \
                   | FCF_MENU           \
                   | FCF_DLGBORDER      \
                   | FCF_TASKLIST       \
                   | FCF_ICON




typedef struct {
   CHAR file[CCHMAXPATH];
   ULONG factor;
   BOOL prompt;
   BOOL save;
} OPTIONS, * POPTIONS;

typedef struct {
   ULONG elapsed;
   ULONG defIter;         // default number of iterations
   ULONG totIter;         // total iterations
   ULONG operations;      // count of performed operations
   BOOL done;
} TEST, * PTEST;

enum {IT_BMPSMOOTH, IT_BMPDRAW, IT_BMPSTRETCH,
      IT_LINES, IT_DRAWRECT, IT_DRAWTEXT,
      IT_LOADDLG, IT_MOVEWIN, IT_SIZEWIN,
      IT_COUNT};

// default number of iterations per test
#define AITERATIONS \
/*    BMP            | lines/rect/text | dialogs       */ \
{3000, 37000, 1200,     600, 72, 3700,   60, 6000, 500}

typedef struct {
   CHAR buf[0x1000];
   BOOL runAllTests;
   BOOL showResults;
   HWND hwndText, hwndLbox;
   OPTIONS option;
   TEST atest[IT_COUNT];
} GLOBAL, * PGLOBAL;


extern GLOBAL g;



VOID showResults(HWND hwnd, BOOL bShow);
VOID testDrawBitmapFluidity(HWND hwnd);
VOID testDrawBitmapSpeed(HWND hwnd);
VOID testDrawBitmapStretch(HWND hwnd);
VOID testDrawLines(HWND hwnd);
VOID testFillRectanges(HWND hwnd);
VOID testDrawText(HWND hwnd);
VOID testLoadDialog(HWND hwnd);
VOID testMoveWindow(HWND hwnd);
VOID testSizeWindow(HWND hwnd);
MRESULT EXPENTRY optionProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY dlgprc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);


