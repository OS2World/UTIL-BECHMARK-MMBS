//===========================================================================
// filetest.h : definitions used by filetest.c
//
// 2004-31-1  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _FILETESTS_H_
   #define _FILETESTS_H_

   #define CB_FILECHUNK           0x80000
   #define CB_LARGEFILE           (CB_FILECHUNK * 256)
   #define CB_SUBDIRCONTENT       278513
   // name of test directory and files
   #define SZ_CLONEDIR            "COPYTREE"
   #define SZ_LARGEFILE           "XLLFILE.TXT"
   #define SZ_SEQZIP              "SEQZIP"
   #define SZ_MULTIZIP            "MULTIZIP"
   #define SZ_LARGEZIP            "LARGEZIP"

#endif // #ifndef _FILETESTS_H_
