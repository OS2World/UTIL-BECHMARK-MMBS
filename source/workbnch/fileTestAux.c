//===========================================================================
// filetestaux.c : helper functions mainly used for file tests
// 2004-1-28  - Alessandro Felice Cantatore
//===========================================================================


#include "workbnch.h"
#include "filetest.h"

/* --------------------------------------------------------------------------
 create a directory, show (and log) an error message when needed
 ------------------------------------------------------------------------- */
BOOL createDir(PSZ dir) {
   if (!fmPathCreate(dir)) {
      scrPrintErrorMsg(SZERR_CREATEDIR);
      return FALSE;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 the test files have a size ranging from 1KB to 16KB depending on their
 index (0-31). This function returns the size of the file 'idx'.
 ------------------------------------------------------------------------- */
// return the size of the file index
ULONG fileSize(INT idx) {
   return 1024 + (15360 * idx) / 31;
}


/* --------------------------------------------------------------------------
 calculate the offset within the random text data
 ------------------------------------------------------------------------- */
ULONG randomTextOffset(INT idx) {
   ULONG i, cb;
   for (i = 0, cb = 0; i < idx; ++i) {
      cb += 1024 + ((15360 * i) / 31);
   } /* endfor */
   return cb;
}


/* --------------------------------------------------------------------------
 create a file of size ranging from 1KB to 16KB containing random text,
 show (and log) an error message when needed
 ------------------------------------------------------------------------- */
BOOL createFile(PSZ pszName, ULONG idx, PSZ pRandText) {
   ULONG cb;
   // build the file name
   sprintf(pszName + strlen(pszName), "\\%02d", idx);
   // write the file with data from the random text
   if (!fmWriteData(pszName, pRandText + randomTextOffset(idx), fileSize(idx))) {
      sprintf(wb.msgbuf, SZERR_WRITEFILE, pszName);
      scrPrintErrorMsg(wb.msgbuf);
      return FALSE;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 open the a file for writing in multiple steps 128 MB of random text data,
 show (and log) an error message when needed
 ------------------------------------------------------------------------- */
BOOL openLargeFile(PHFILE phf) {
   CHAR achName[CCHMAXPATH];
   sprintf(achName, "%s\\%s", wb.path, SZ_LARGEFILE);
   *phf = fmOpen(achName, FMO_CREATE|FMO_REPLACE|FMO_SHARENONE|FMO_WRITE);
   if (*phf == FMO_ERROR) {
      sprintf(wb.msgbuf, SZERR_OPENLARGEFILE, achName);
      scrPrintErrorMsg(wb.msgbuf);
      return FALSE;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 write a chunk of random text data in a 128 MB file, show (and log) an error
 message when needed
 ------------------------------------------------------------------------- */
BOOL writeLargeFileChunk(HFILE hf, PSZ p, ULONG cb) {
   if (!fmWrite(hf, p, cb)) {
      sprintf(wb.msgbuf, SZERR_WRITELARGEFILE, wb.path);
      scrPrintErrorMsg(wb.msgbuf);
      return FALSE;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 read a file, show (and log) an error message when needed
 ------------------------------------------------------------------------- */
BOOL readFile(ULONG idir, ULONG isub, ULONG ifile, PSZ p) {
   CHAR achName[CCHMAXPATH];
   HFILE hf;
   BOOL rc = FALSE;
   sprintf(achName, "%s\\%04d\\%04d\\%02d", wb.path, idir, isub, ifile);
   hf = fmOpen(achName, FMO_IFEXIST|FMO_SHAREALL|FMO_READ);
   if (FMO_ERROR == hf) goto error;
   if (!fmRead(hf, p, fileSize(ifile))) goto error;
   rc = TRUE;
   goto exit_0;
error:
   sprintf(wb.msgbuf, SZERR_READFILE, achName);
   scrPrintErrorMsg(wb.msgbuf);
exit_0:
   fmClose(hf);
   return rc;
}


/* --------------------------------------------------------------------------
 open or read a chunk of data from the 128 MB text file, show (and log) an
 error message when needed
 ------------------------------------------------------------------------- */
BOOL readLargeFile(PHFILE phf, PSZ p) {
   CHAR achName[CCHMAXPATH];
   // when p is NULL just open the file
   if (!p) {
      sprintf(achName, "%s\\%s", wb.path, SZ_LARGEFILE);
      *phf = fmOpen(achName, FMO_IFEXIST|FMO_SHAREALL|FMO_READ);
      if (*phf == FMO_ERROR) {
         sprintf(wb.msgbuf, SZERR_OPENLARGEFILE, achName);
         scrPrintErrorMsg(wb.msgbuf);
         return FALSE;
      } /* endif */
   } else {
      if (!fmRead(*phf, p, CB_FILECHUNK)) {
         sprintf(wb.msgbuf, SZERR_READFILECHUNK, wb.path);
         scrPrintErrorMsg(wb.msgbuf);
         return FALSE;
      } /* endif */
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 copy a file (overwriting if needed), show (and log) an error message when
 needed
 ------------------------------------------------------------------------- */
BOOL copyFile(PSZ pszSrc, PSZ pszDest) {
   if (DosCopy(pszSrc, pszDest, DCPY_EXISTING)) {
      sprintf(wb.msgbuf, SZERR_COPYFILE, pszSrc);
      scrPrintErrorMsg(wb.msgbuf);
      return FALSE;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 executes a command via the command processor, , show (and log) an error
 message when needed
 ------------------------------------------------------------------------- */
BOOL cmdRun(PSZ pszCommand) {
   if (system(pszCommand)) {
      sprintf(wb.msgbuf, SZERR_SYSTEM, pszCommand);
      scrPrintErrorMsg(wb.msgbuf);
      return FALSE;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 recursively count the number of subdirectories and files contained in a
 tree of directories and calculates the total size of the tree content.
 Show (and log) an error message when needed
 ------------------------------------------------------------------------- */
BOOL treeStats(PSZ pszPath, PULONG pcbTotal, PULONG pcFiles, PULONG pcDirs) {
   CHAR achPath[CCHMAXPATH];
   *pcbTotal = *pcFiles = *pcDirs = 0;
   if (!fmFullPathName(pszPath, achPath)) return FALSE;
   strcat(achPath, "\\*");
   if (!recursiveDirSize(achPath, pcbTotal, pcFiles, pcDirs)) {
      sprintf(wb.msgbuf, SZERR_GETPATHSIZE, wb.path);
      scrPrintErrorMsg(wb.msgbuf);
      return FALSE;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 recursive procedure called by treeStats() to get the count of directories
 and files and the total size of data in a tree of directories.
 ------------------------------------------------------------------------- */
BOOL recursiveDirSize(PSZ pszPath, PULONG pcbTotal, PULONG pcFiles, PULONG pcDirs) {
   HDIR hdir = HDIR_CREATE;
   FILEFINDBUF3 ff;
   ULONG ul;
   PSZ pEndPath;
   BOOL rc = FALSE;
   ul = 1;
   if (DosFindFirst(pszPath, &hdir, FILEATTR_RECUR, &ff, sizeof(ff),
                    &ul, FIL_STANDARD))
      return FALSE;
   pEndPath = strrchr(pszPath, '\\');
   do {
      if (ff.attrFile & FILE_DIRECTORY) {
         if (fmIsCurDir(ff.achName) || fmIsParDir(ff.achName)) continue;
         memcpy(pEndPath + 1, ff.achName, ff.cchName + 1);
         memcpy(pEndPath + 1 + ff.cchName, "\\*", 3);
         if (!recursiveDirSize(pszPath, pcbTotal, pcFiles, pcDirs))
            goto exit_0;
         (*pcDirs)++;
      } else {
         *pcbTotal += ff.cbFile;
         (*pcFiles)++;
      } /* endif */
   } while ( !DosFindNext (hdir, &ff, sizeof(ff), &ul) ); /* enddo */
   memcpy(pEndPath, "\\*", 3);
   rc = TRUE;
exit_0:
   DosFindClose(hdir);
   return rc;
}


/* --------------------------------------------------------------------------
 recursively delete all the files and directories contained in 'pszPath'.
 Show (and log) an error message when needed
 ------------------------------------------------------------------------- */
BOOL treeDelete(PSZ pszPath, ULONG csteps) {
   CHAR achPath[CCHMAXPATH];
   ULONG cdeleted = 0;
   if (!fmFullPathName(pszPath, achPath)) return FALSE;
   strcat(achPath, "\\*");
   if (!recursiveDirDelete(achPath, csteps, &cdeleted)) {
      sprintf(wb.msgbuf, SZERR_DELTREE, pszPath);
      scrPrintErrorMsg(wb.msgbuf);
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 recursive procedure called by treeDelete() to delete all the files and
 directories contained in 'pszPath'.
 If 'csteps' is TRUE updates a progress bar.
 ------------------------------------------------------------------------- */
BOOL recursiveDirDelete(PSZ pszPath, ULONG csteps, PULONG pcdeleted) {
   HDIR hdir = HDIR_CREATE;
   FILEFINDBUF3 ff;
   ULONG ul;
   PSZ pEndPath;
   ULONG curstep, deleted;
   BOOL rc = FALSE;
   ul = 1;
   if (DosFindFirst(pszPath, &hdir, FILEATTR_RECUR, &ff, sizeof(ff),
                    &ul, FIL_STANDARD))
      return -1;
   pEndPath = strrchr(pszPath, '\\');
   deleted = *pcdeleted;
   do {
      if (ff.attrFile & FILE_DIRECTORY) {
         if (fmIsCurDir(ff.achName) || fmIsParDir(ff.achName))
            continue;
         memcpy(pEndPath + 1, ff.achName, ff.cchName + 1);
         memcpy(pEndPath + 1 + ff.cchName, "\\*", 3);
         if (!recursiveDirDelete(pszPath, csteps, pcdeleted)) goto exit_0;
         *(pEndPath + 1 + ff.cchName) = 0;
         if (DosDeleteDir(pszPath)) goto exit_0;
         ++deleted;
      } else {
         memcpy(pEndPath + 1, ff.achName, ff.cchName + 1);
         if (DosForceDelete(pszPath)) goto exit_0;
         ++deleted;
      } /* endif */
      if (csteps && (deleted > *pcdeleted)) {
         curstep = (++*pcdeleted * 320) / csteps;
         if (curstep > wb.prevStep)
            wb.prevStep = printProgress(wb.prevStep, curstep);
      } /* endif */
   } while ( !DosFindNext (hdir, &ff, sizeof(ff), &ul) ); /* enddo */
   memcpy(pEndPath, "\\*", 3);
   rc = TRUE;
exit_0:
   DosFindClose(hdir);
   return rc;
}

