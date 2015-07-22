//===========================================================================
// filetest.c : file read, write, copy, zip, unzip, delete tests
// 2004-1-28  - Alessandro Felice Cantatore
//===========================================================================


#include "workbnch.h"
#include "filetest.h"


/* --------------------------------------------------------------------------
 This test measures the file write performances.
 During this test 100 directories, 200 subdirectories and 6400 files
 containing random text data will be created. The size of each file varies
 from 1 KB to 16 KB.
 ------------------------------------------------------------------------- */
BOOL testMultiFileWrite(ULONG iterations) {
   CHAR achName[CCHMAXPATH];
   INT iter, i, j, k;
   ULONG curstep, csteps, cbName;
   PSZ pRandomText;
   BOOL rc = FALSE;
   // create a buffer with random ascii content
   if (NULL == (pRandomText = makeRandomText(CB_SUBDIRCONTENT))) goto exit_0;
   wb.cbProcessed = iterations * 200 * CB_SUBDIRCONTENT;
   if (!prepareTest(ITEST_MULTIFILEWRITE, iterations, SZ_TESTMULTIFILEWRITE, TRUE))
      goto exit_1;
   csteps = iterations * 200;
   resetTimer();
   for (iter = 0; iter < iterations; ++iter) {
      for (i = 0; i < 100; ++i) {
         sprintf(achName, "%s\\%04d", wb.path, i);
         if (!createDir(achName)) goto exit_1;
         for (j = 0; j < 2; ++j) {
            cbName = sprintf(achName, "%s\\%04d\\%04d", wb.path, i, j);
            if (!createDir(achName)) goto exit_1;
            for (k = 0; k < 32; ++k, achName[cbName] = 0) {
               if (!createFile(achName, k, pRandomText)) goto exit_1;
            } /* endfor */
            if (wb.progress) {
               curstep = (((iter * 200) + ((i * 2) + j + 1)) * 320) / csteps;
               wb.prevStep = printProgress(wb.prevStep, curstep);
            } /* endif */
         } /* endfor */
      } /* endfor */
   } /* endfor */
   endTest(TRUE);
   rc = TRUE;
exit_1:
   free(pRandomText);
exit_0:
   return rc;
}


/* --------------------------------------------------------------------------
 This test measures the file write performances by writing a large text
 file (128 MB)
 ------------------------------------------------------------------------- */
BOOL testLargeFileWrite(ULONG iterations) {
   INT iter, i;
   ULONG curstep, csteps, cbName;
   PSZ pRandomText;
   BOOL rc = FALSE;
   HFILE hf;
   // create a buffer with random ascii content
   if (NULL == (pRandomText = makeRandomText(CB_FILECHUNK))) goto exit_0;
   wb.cbProcessed = iterations * CB_LARGEFILE;
   if (!prepareTest(ITEST_LARGEFILEWRITE, iterations, SZ_TESTLARGEFILEWRITE, TRUE))
      goto exit_1;
   csteps = iterations * 256;
   resetTimer();
   for (iter = 0; iter < iterations; ++iter) {
      if (!openLargeFile(&hf)) goto exit_1;
      for (i = 0; i < 256; ++i) {
         if (!writeLargeFileChunk(hf, pRandomText, CB_FILECHUNK)) goto exit_2;
         if (wb.progress) {
            curstep = (((iter * 256) + i) * 320) / csteps;
            wb.prevStep = printProgress(wb.prevStep, curstep);
         } /* endif */
      } /* endfor */
      fmClose(hf);
   } /* endfor */
   endTest(TRUE);
   rc = TRUE;
   goto exit_1;
exit_2:
   fmClose(hf);
exit_1:
   free(pRandomText);
exit_0:
   return rc;
}


/* --------------------------------------------------------------------------
 This test measures the file read performances.
 During this test 6400 files will be read
 The size of each file varies from 1 KB to 16 KB
 ------------------------------------------------------------------------- */
BOOL testMultiFileRead(ULONG iterations) {
   INT iter, i, j, k;
   ULONG curstep, csteps;
   PSZ pReadBuf;
   BOOL rc = FALSE;
   // preallocate a buffer to read the files
   if (NULL == (pReadBuf = tryalloc(0x4000))) goto exit_0;
   wb.cbProcessed = iterations * 200 * CB_SUBDIRCONTENT;
   if (!prepareTest(ITEST_MULTIFILEREAD, iterations, SZ_TESTMULTIFILEREAD, TRUE))
      goto exit_1;
   csteps = iterations * 200;
   resetTimer();
   for (iter = 0; iter < iterations; ++iter) {
      for (i = 0; i < 100; ++i) {
         for (j = 0; j < 2; ++j) {
            for (k = 0; k < 32; ++k) {
               if (!readFile(i, j, k, pReadBuf)) goto exit_1;
            } /* endfor */
            if (wb.progress) {
               curstep = (((iter * 200) + ((i * 2) + j + 1)) * 320) / csteps;
               wb.prevStep = printProgress(wb.prevStep, curstep);
            } /* endif */
         } /* endfor */
      } /* endfor */
   } /* endfor */
   endTest(TRUE);
   rc = TRUE;
exit_1:
   free(pReadBuf);
exit_0:
   return rc;
}


/* --------------------------------------------------------------------------
 This test measures the file read performances by reading a large
 text file (128 MB) in 256 steps.
 ------------------------------------------------------------------------- */
BOOL testLargeFileRead(ULONG iterations) {
   INT iter, i;
   ULONG curstep, csteps, cbName;
   PSZ pReadBuf;
   BOOL rc = FALSE;
   HFILE hf;
   // create a buffer with random ascii content
   if (NULL == (pReadBuf = tryalloc(CB_FILECHUNK))) goto exit_0;
   wb.cbProcessed = iterations * CB_LARGEFILE;
   if (!prepareTest(ITEST_LARGEFILEREAD, iterations, SZ_TESTLARGEFILEREAD, TRUE))
      goto exit_1;
   csteps = iterations * 256;
   resetTimer();
   for (iter = 0; iter < iterations; ++iter) {
      if (!readLargeFile(&hf, NULL)) goto exit_1;
      for (i = 0; i < 256; ++i) {
         if (!readLargeFile(&hf, pReadBuf)) goto exit_2;
         if (wb.progress) {
            curstep = (((iter * 256) + i) * 320) / csteps;
            wb.prevStep = printProgress(wb.prevStep, curstep);
         } /* endif */
      } /* endfor */
      fmClose(hf);
   } /* endfor */
   endTest(TRUE);
   rc = TRUE;
   goto exit_1;
exit_2:
   fmClose(hf);
exit_1:
   free(pReadBuf);
exit_0:
   return rc;
}


/* --------------------------------------------------------------------------
 This test measures the file copy performances by copying a tree containing
 25 directories, 50 subdirectories and 1600 files.
 The size of each file varies from 1 KB to 16 KB.
 The first 25 directories created during the previous tests are copied to
 the 'COPYTREE' directory.
 ------------------------------------------------------------------------- */
BOOL testMultiFileCopy(ULONG iterations) {
   CHAR achSrc[CCHMAXPATH];
   CHAR achDest[CCHMAXPATH];
   INT iter, i, j, k;
   ULONG curstep, csteps, cbpath;
   wb.cbProcessed = iterations * 50 * CB_SUBDIRCONTENT;
   if (!prepareTest(ITEST_MULTIFILECOPY, iterations, SZ_TESTMULTIFILECOPY, TRUE))
      return FALSE;
   csteps = iterations * 50;
   cbpath = strlen(wb.path);
   sprintf(achDest, "%s\\%s", wb.path, SZ_CLONEDIR);
   if (!createDir(achDest)) return FALSE;
   resetTimer();
   for (iter = 0; iter < iterations; ++iter) {
      for (i = 0; i < 25; ++i) {
         // copy the directories 0000-0025
         sprintf(achSrc, "%s\\%04d", wb.path, i);
         sprintf(achDest + cbpath + 9, "\\%04d", i);
         if (!copyFile(achSrc, achDest)) return FALSE;
         for (j = 0; j < 2; ++j) {
            // copy the subdirectories 0000-0001
            sprintf(achSrc + cbpath + 5, "\\%04d", j);
            sprintf(achDest + cbpath + 14, "\\%04d", j);
            if (!copyFile(achSrc, achDest)) return FALSE;
            for (k = 0; k < 32; ++k) {
               sprintf(achSrc + cbpath + 10, "\\%02d", k);
               if (!copyFile(achSrc, achDest)) return FALSE;
            } /* endfor */
            if (wb.progress) {
               curstep = (((iter * 50) + ((i * 2) + j + 1)) * 320) / csteps;
               wb.prevStep = printProgress(wb.prevStep, curstep);
            } /* endif */
         } /* endfor */
      } /* endfor */
   } /* endfor */
   endTest(TRUE);
   return TRUE;
}


/* --------------------------------------------------------------------------
 This test measures the file copy performances by copying a large
 text file (128 MB) in a unique step via DosCopy().
 ------------------------------------------------------------------------- */
BOOL testLargeFileCopy(ULONG iterations) {
   CHAR achSrc[CCHMAXPATH];
   CHAR achDest[CCHMAXPATH];
   INT iter;
   ULONG curstep, csteps;
   wb.cbProcessed = iterations * CB_LARGEFILE;
   if (!prepareTest(ITEST_LARGEFILECOPY, iterations, SZ_TESTLARGEFILECOPY, TRUE))
      return FALSE;
   csteps = iterations;
   // source and destination file name
   sprintf(achSrc, "%s\\%s", wb.path, SZ_LARGEFILE);
   sprintf(achDest, "%s\\%s", wb.path, SZ_CLONEDIR);
   resetTimer();
   for (iter = 0; iter < iterations; ++iter) {
      if (!copyFile(achSrc, achDest)) return FALSE;
      if (wb.progress) {
         curstep = ((iter + 1) * 320) / csteps;
         wb.prevStep = printProgress(wb.prevStep, curstep);
      } /* endif */
   } /* endfor */
   endTest(TRUE);
   return TRUE;
}


/* --------------------------------------------------------------------------
 This test measures the file ZIP performances by sequentially zipping the
 files contained in a tree of 2 directories, 4 subdirectories (128 files).
 The compression is the highest (-9) and ZIP.EXE is executed for each file.
 The size of each file varies from 1 KB to 16 KB.
 ------------------------------------------------------------------------- */
BOOL testSeqFileZip(ULONG iterations) {
   CHAR achCmd[CCHMAXPATH * 2];
   INT iter, i, j, k;
   ULONG curstep, csteps, cbcmd;
   wb.cbProcessed = iterations * 4 * CB_SUBDIRCONTENT;
   if (!prepareTest(ITEST_SEQFILEZIP, iterations, SZ_TESTSEQFILEZIP, TRUE))
      return FALSE;
   csteps = iterations * 4;
   cbcmd = sprintf(achCmd, "ZIP.EXE -q9 %s\\%s ", wb.path, SZ_SEQZIP);
   resetTimer();
   for (iter = 0; iter < iterations; ++iter) {
      for (i = 0; i < 2; ++i) {
         for (j = 0; j < 2; ++j) {
            for (k = 0; k < 32; ++k) {
               sprintf(achCmd + cbcmd, "%s\\%04d\\%04d\\%02d", wb.path, i, j, k);
               if (!cmdRun(achCmd)) return FALSE;
            } /* endfor */
            if (wb.progress) {
               curstep = (((iter * 4) + ((i * 2) + j + 1)) * 320) / csteps;
               wb.prevStep = printProgress(wb.prevStep, curstep);
            } /* endif */
         } /* endfor */
      } /* endfor */
   } /* endfor */
   endTest(TRUE);
   return TRUE;
}


/* --------------------------------------------------------------------------
 This test measures the file ZIP performances by zipping a tree of files
 containing 16 directories, 32 subdirectories and 1024 files.
 The compression is the highest (-9).
 The size of each file varies from 1 KB to 16 KB.
 ------------------------------------------------------------------------- */
BOOL testMultiFileZip(ULONG iterations) {
   CHAR achCmd[CCHMAXPATH * 17];
   INT iter, i;
   ULONG curstep, csteps, cbcmd;
   wb.cbProcessed = iterations * 32 * CB_SUBDIRCONTENT;
   if (!prepareTest(ITEST_MULTIFILEZIP, iterations, SZ_TESTMULTIFILEZIP, TRUE))
      return FALSE;
   csteps = iterations;
   cbcmd = sprintf(achCmd, "ZIP.EXE -rq9 %s\\%s", wb.path, SZ_MULTIZIP);
   for (i = 0; i < 16; ++i) {
      cbcmd += sprintf(achCmd + cbcmd, " %s\\%04d", wb.path, i);
   } /* endfor */
   resetTimer();
   for (iter = 0; iter < iterations; ++iter) {
      if (!cmdRun(achCmd)) return FALSE;
      if (wb.progress) {
         curstep = ((iter + 1) * 320) / csteps;
         wb.prevStep = printProgress(wb.prevStep, curstep);
      } /* endif */
   } /* endfor */
   endTest(TRUE);
   return TRUE;
}


/* --------------------------------------------------------------------------
 This test measures the file ZIP performances by zipping a 128 MBs file.
 The compression is the highest (-9).
 ------------------------------------------------------------------------- */
BOOL testLargeFileZip(ULONG iterations) {
   CHAR achCmd[CCHMAXPATH * 2];
   INT iter, i;
   ULONG curstep, csteps;
   wb.cbProcessed = iterations * CB_LARGEFILE;
   if (!prepareTest(ITEST_LARGEFILEZIP, iterations, SZ_TESTLARGEFILEZIP, TRUE))
      return FALSE;
   csteps = iterations;
   sprintf(achCmd, "ZIP.EXE -q9 %s\\%s %s\\%s",
           wb.path, SZ_LARGEZIP, wb.path, SZ_LARGEFILE);
   resetTimer();
   for (iter = 0; iter < iterations; ++iter) {
      if (!cmdRun(achCmd)) return FALSE;
      if (wb.progress) {
         curstep = ((iter + 1) * 320) / csteps;
         wb.prevStep = printProgress(wb.prevStep, curstep);
      } /* endif */
   } /* endfor */
   endTest(TRUE);
   return TRUE;
}

/* --------------------------------------------------------------------------
 This test measures the file UNZIP performances by unzipping a tree of files
 containing 16 directories, 32 subdirectories and 1024 files.
 The compression is the highest (-9).
 The size of each file varies from 1 KB to 16 KB.
 ------------------------------------------------------------------------- */
BOOL testUnzip(ULONG iterations) {
   CHAR achPrevDir[CCHMAXPATH];
   CHAR achNewDir[CCHMAXPATH];
   CHAR achCmd[CCHMAXPATH * 2];
   INT iter, i;
   ULONG curstep, csteps, cbcmd;
   wb.cbProcessed = iterations * 32 * CB_SUBDIRCONTENT;
   if (!prepareTest(ITEST_UNZIP, iterations, SZ_TESTUNZIP, TRUE))
      return FALSE;
   // save current directory
   if (!applWorkDirGet(achPrevDir)) {
      scrPrintErrorMsg(SZERR_GETWORKDIR);
      return FALSE;
   } /* endif */
   // move to the root of the drive of the test path
   memcpy(achNewDir, wb.path, 3), achNewDir[3] = 0;
   if (!applWorkDirSet(achNewDir)) {
      scrPrintErrorMsg(SZERR_SETWORKDIR);
      return FALSE;
   } /* endif */
   csteps = iterations;
   cbcmd = sprintf(achCmd, "UNZIP.EXE -o -qq %s\\%s", wb.path, SZ_MULTIZIP);
   resetTimer();
   for (iter = 0; iter < iterations; ++iter) {
      if (!cmdRun(achCmd)) return FALSE;
      if (wb.progress) {
         curstep = ((iter + 1) * 320) / csteps;
         wb.prevStep = printProgress(wb.prevStep, curstep);
      } /* endif */
   } /* endfor */
   endTest(TRUE);
   // restore working directory
   if (!applWorkDirSet(achPrevDir)) {
      scrPrintErrorMsg(SZERR_SETWORKDIR);
      return FALSE;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 This test measures the performances in deleting all the files created
 during the previous tests.
 ------------------------------------------------------------------------- */
BOOL testDeleteAllFiles(VOID) {
   CHAR achPath[CCHMAXPATH];
   ULONG cbTotal, cFiles, cDirs;
   if (!treeStats(wb.path, &cbTotal, &cFiles, &cDirs)) return FALSE;
//   printf("files: %u, directories: %u\r\n", cFiles, cDirs);   /* DEBUG */
//   pressAnyKey(-1, -1);
   wb.cbProcessed = cbTotal;
   if (!prepareTest(ITEST_MULTIFILEDEL, 1, SZ_MULTIFILEDEL, TRUE))
      return FALSE;
   if (!wb.progress) cFiles = cDirs = 0;
   resetTimer();
   if (!treeDelete(wb.path, cFiles + cDirs))
      return FALSE;
   endTest(TRUE);
   return TRUE;
}

