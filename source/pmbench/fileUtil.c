//===========================================================================
// fileUtil.c : file i/o utility routines
// 2004-01-22  - Alessandro Felice Cantatore
//===========================================================================


#include "pmbench.h"


/* --------------------------------------------------------------------------
 Open 'pszFile' according to the specified mode 'flag'.
 ------------------------------------------------------------------------- */
HFILE fmOpen(PSZ pszFile, ULONG flag) {
   HFILE hf;
   ULONG ul;
   if (DosOpen(pszFile, &hf, &ul, 0, 0, MKOPENFLAG(flag), MKOPENMODE(flag), NULL))
      return FMO_ERROR;
   if ((flag & FMO_APPEND) && DosSetFilePtr(hf, 0, FILE_END, &ul)) {
      DosClose(hf);
      return FMO_ERROR;
   } /* endif */
   return hf;
}


/* --------------------------------------------------------------------------
 Close a file.
 ------------------------------------------------------------------------- */
BOOL fmClose(HFILE hf) {
   return !DosClose(hf);
}


/* --------------------------------------------------------------------------
 Read 'cb' bytes from the file 'hf' into the buffer 'pBuffer'.
 ------------------------------------------------------------------------- */
BOOL fmRead(HFILE hf, PVOID pBuffer, ULONG cb) {
   return !DosRead(hf, pBuffer, cb, &cb);
}


/* --------------------------------------------------------------------------
 Write 'cb' bytes from the buffer 'pBuffer' to the file 'hf'.
 ------------------------------------------------------------------------- */
BOOL fmWrite(HFILE hf, PVOID pBuffer, ULONG cb) {
   return !DosWrite(hf, pBuffer, cb, &cb);
}


/* --------------------------------------------------------------------------
 Return the size of the file 'hf'.
 ------------------------------------------------------------------------- */
LONG fmSize(HFILE hf) {
   APIRET rc;
   FILESTATUS3 fs;
   if (0 != (rc = DosQueryFileInfo(hf, 1, &fs, sizeof(fs))))
      return -rc;
   return fs.cbFile;
}


/* --------------------------------------------------------------------------
 Create (or replace if already exists) the file 'pszFileName' writing 'cbData'
 bytes from the buffer 'pszData'.
 ------------------------------------------------------------------------- */
BOOL fmWriteData(PSZ pszFileName, PSZ pszData, ULONG cbData) {
   HFILE hf;
   hf = fmOpen(pszFileName, FMO_CREATE|FMO_REPLACE|FMO_SHARENONE|FMO_WRITE);
   if (hf == FMO_ERROR) return FALSE;
   if (!fmWrite(hf, pszData, cbData)) {
      fmClose(hf);
      return FALSE;
   } /* endif */
   fmClose(hf);
   return TRUE;
}


/* --------------------------------------------------------------------------
 Change the extension of 'pszFileName' with 'ext'.
 ------------------------------------------------------------------------- */
PSZ fmFileExtChange(PSZ pszFileName, PSZ ext) {
   PSZ p;
   p = strrchr(pszFileName, '\\'); // find the file name
   if (!p) p = pszFileName;
   p = strrchr(p, '.');            // find the extension
   if (!p) p = pszFileName + strlen(pszFileName);
   strcpy(((*ext == '.') ? p : p + 1), ext);
   return pszFileName;
}


/* --------------------------------------------------------------------------
 Return TRUE if 'pszFile' exists.
 ------------------------------------------------------------------------- */
BOOL fmExists(PSZ pszFile) {
   HDIR hdir = HDIR_CREATE;
   FILEFINDBUF3 ffb;
   ULONG ul = 1;
   if (!DosFindFirst(pszFile, &hdir, FILEATTR_ANY | FILE_DIRECTORY, &ffb,
                     sizeof(ffb), &ul, FIL_STANDARD)) {
      DosFindClose(hdir);
      return TRUE;
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 If pszPath doesn't already exist creates all the needed directories.
 Note: pszPath can be a path relative to the current path.
 ------------------------------------------------------------------------- */
BOOL fmPathCreate(PSZ pszPath) {
   CHAR buf[CCHMAXPATH];
   PSZ p;
   // if pszPath is just a drive name
   if (!pszPath[2] && pszPath[1] == ':') {
      memcpy(buf, " :\\", 4);
      buf[0] = pszPath[0];
      return fmExists(buf);
   } /* endif */
   if (!fmFullPathName(pszPath, buf))
      return FALSE;
   for (p = buf + 3; ; *p++ = '\\') {
      if (NULL != (p = strchr(p, '\\'))) *p = 0;
      if (!fmExists(buf) && DosCreateDir(buf, NULL)) return FALSE;
      if (!p) break;
   } /* endfor */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Check the full path name of 'pszName' writing it to 'pszFullPath'.
 ------------------------------------------------------------------------- */
// If pszPath doesn't already exist creates all the needed directories.
// Note: pszPath can be a path relative to the current path.
BOOL fmFullPathName(PSZ pszName, PSZ pszFullPath) {
   return !DosQueryPathInfo(pszName, FIL_QUERYFULLNAME, pszFullPath, CCHMAXPATH);
}


/* --------------------------------------------------------------------------
 Set the current work directory and drive.
 ------------------------------------------------------------------------- */
BOOL applWorkDirSet(PSZ pszDir) {
   BOOL rc = FALSE;
   // check if the drive letter was specified
   DosError(FERR_DISABLEHARDERR);
   if ((pszDir[1] == ':') && DosSetDefaultDisk((*pszDir & ~0x20) - 'A' + 1))
      goto end;
   rc = !DosSetCurrentDir(pszDir);
end:
   DosError(FERR_ENABLEHARDERR);
   return rc;
}


/* --------------------------------------------------------------------------
 Get the current work directory and drive.
 ------------------------------------------------------------------------- */
BOOL applWorkDirGet(PSZ pszDir) {
   return fmFullPathName(".", pszDir);
}
