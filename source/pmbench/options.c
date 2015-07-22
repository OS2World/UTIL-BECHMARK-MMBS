//===========================================================================
// .c :
// -
// -
// --2004  - Alessandro Felice Cantatore
//===========================================================================


#include "pmbench.h"


// prototypes ---------------------------------------------------------------
VOID initDlg(HWND hwnd);
static ULONG idBtnFromFactor(ULONG factor);
static VOID onCommand(HWND hwnd, ULONG id);


// --------------------------------------------------------------------------

MRESULT EXPENTRY optionProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         initDlg(hwnd);
         return (MRESULT)TRUE;
      case WM_COMMAND:
         onCommand(hwnd, (ULONG)mp1);
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return (MRESULT)FALSE;
}


VOID initDlg(HWND hwnd) {
   WinCheckButton(hwnd, idBtnFromFactor(g.option.factor), 1);
   WinCheckButton(hwnd, RI_DOCHKPROMPT, g.option.prompt);
   WinCheckButton(hwnd, RI_DOCHKSAVE, g.option.save);
   WinSetDlgItemText(hwnd, RI_DOEFSAVE, g.option.file);
}


static ULONG idBtnFromFactor(ULONG factor) {
   switch (factor) {
      case 1:
      case 2:
      case 3:  return factor + RI_DORD_1 - 1;
      case 5:  return RI_DORD_5;
      case 10: return RI_DORD_10;
      case 20: return RI_DORD_20;
      case 50: return RI_DORD_50;
   } /* endswitch */
   return 0;
}


static VOID onCommand(HWND hwnd, ULONG id) {
   CHAR buf[CCHMAXPATH];
   BYTE afactors[] = {1, 2, 3, 5, 10, 20, 50};
   INT idxRadioBtn;
   if (DID_OK == id) {
      idxRadioBtn = (INT)WinSendDlgItemMsg(hwnd, RI_DORD_1, BM_QUERYCHECKINDEX,
                                           MPVOID, MPVOID);
      if ((idxRadioBtn < 0) || (idxRadioBtn > 6)) idxRadioBtn = 3;
      g.option.factor = afactors[idxRadioBtn];
      g.option.prompt = WinQueryButtonCheckstate(hwnd, RI_DOCHKPROMPT);
      g.option.save = WinQueryButtonCheckstate(hwnd, RI_DOCHKSAVE);
      WinQueryDlgItemText(hwnd, RI_DOEFSAVE, CCHMAXPATH, buf);
      if (buf[0]) strcpy(g.option.file, buf);
   } /* endif */
   WinDismissDlg(hwnd, id);
}
