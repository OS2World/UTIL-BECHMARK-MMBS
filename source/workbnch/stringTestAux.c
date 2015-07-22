//===========================================================================
// stringTestAux.c : text string manipulation benchmarks - utility functions
// 2004-01-22  - Alessandro Felice Cantatore
//===========================================================================


#include "workbnch.h"
#include "stringTest.h"

// definitions --------------------------------------------------------------

#define SZ_DELIMITERS \
"\r\n\t !\"%&\'()*+,-./:;<=>?@[\\]^{|}~"


// prototypes ---------------------------------------------------------------


// globals ------------------------------------------------------------------


/* --------------------------------------------------------------------------
 Check if a character is a valid word character.
 ------------------------------------------------------------------------- */
BOOL isCharValid(INT ch) {
   return ((ch > ' ') && (ch < '\x80') && !strchr(SZ_DELIMITERS, ch));
}



/* --------------------------------------------------------------------------
 Find the beginning of the next word.
 ------------------------------------------------------------------------- */
PSZ stripDelimiters(PSZ p) {
   for ( ; *p; ++p) {
      if (isCharValid(*p)) break;
   } /* endfor */
   return p;
}


/* --------------------------------------------------------------------------
 Add the current word to the array of words.
 ------------------------------------------------------------------------- */
PSZ addWord(PSZ psz, PWORDLIST pwl) {
   PSZ pin, pout;
   for (pin = psz, pout = pwl->achWords + pwl->cb;
        isCharValid(*pin);
        ++pin, ++pout, ++pwl->cb) {
      *pout = *pin;
   } /* endfor */
   // terminate the word, update the data length, update the word count
   *pout = 0, ++pwl->cb, ++pwl->count;
   return pin + 1;
}


/* --------------------------------------------------------------------------
 Save the list of words in a binary file.
 ------------------------------------------------------------------------- */
BOOL saveWordList(PWORDLIST pwl) {
   CHAR achFile[CCHMAXPATH];
   sprintf(achFile, "%s\\%s", wb.path, SZ_WORDSLIST);
   return fmWriteData(achFile, (PSZ)pwl, pwl->cb + sizeof(WORDLIST) - 4);
}


/* --------------------------------------------------------------------------
 Allocate memory and read the content of the word array file WORDLIST.BIN.
 ------------------------------------------------------------------------- */
PWORDLIST readWordArrayFile(VOID) {
   HFILE hf;
   PWORDLIST pwl = NULL;
   CHAR achFile[CCHMAXPATH];
   LONG cbFile;
   // open the file
   sprintf(achFile, "%s\\%s", wb.path, SZ_WORDSLIST);
   if (FMO_ERROR == (hf = fmOpen(achFile, FMO_IFEXIST|FMO_SHARENONE|FMO_READ)))
      goto exit_0;
   // get the file size
   if (0 > (cbFile = fmSize(hf))) goto exit_1;
   // allocate memory for reading the file
   if (NULL == (pwl = (PWORDLIST)tryalloc(cbFile))) goto exit_1;
   // read the file into the memory buffer
   if (!fmRead(hf, pwl, cbFile)) {
      free(pwl);
      pwl = NULL;
      cbFile = -1;
   } /* endif */
exit_1:
   fmClose(hf);
exit_0:
   if ((cbFile < 0) || (hf == FMO_ERROR)) {
      sprintf(wb.msgbuf, SZERR_READWORDARRAYFILE, achFile);
      scrPrintErrorMsg(wb.msgbuf);
   } /* endif */
   return pwl;
}


/* --------------------------------------------------------------------------
 Create an array of text strings from the content of the file WORDLIST.BIN
 ------------------------------------------------------------------------- */
PSZ* makeWordArray(PWORDLIST pwl) {
   PSZ* ppszWords;
   PSZ* ppsz;
   PSZ pszword;
   INT i;
   // allocate memory for storing the array of words
   if (NULL == (ppszWords = malloc(pwl->count * sizeof(PSZ)))) {
      scrPrintErrorMsg(SZERR_MAKEWORDARRAY);
      return NULL;
   } /* endif */
   for (i = 0, pszword = pwl->achWords, ppsz = ppszWords; i < pwl->count; ++i) {
      *ppsz = pszword;
      pszword += strlen(pszword) + 1;
      ppsz++;
   } /* endfor */
   return ppszWords;
}


/* --------------------------------------------------------------------------
 strcmp() wrapper.
 ------------------------------------------------------------------------- */
INT compwordss(PSZ* key, PSZ* element) {
   return strcmp(*key, *element);
}


/* --------------------------------------------------------------------------
 stricmp() wrapper.
 ------------------------------------------------------------------------- */
INT compwordsi(PSZ* key, PSZ* element) {
   return stricmp(*key, *element);
}


/* --------------------------------------------------------------------------
 Write the sorted text words array to a file (debugging purpose only).
 ------------------------------------------------------------------------- */
VOID dumpSortedList(PSZ* ppszWords, ULONG itest, PWORDLIST pwl) {
   CHAR achFile[CCHMAXPATH];
   PSZ pFile, p;
   ULONG i;
   // if allocation is succesful
   if (NULL != (pFile = tryalloc(pwl->cb))) {
      // file the buffer with the sorted words separating them by a space
      // or a new line character (every six words)
      for (i = 0, p = pFile; i < pwl->count; ++i) {
         p += sprintf(p, ((i && (i % 6)) ? "%s " : "%s\n"), *ppszWords);
         ppszWords++;
      } /* endfor */
      // save the buffer to a file name IWSORTED.TXT or SWSORTED.TXT
      sprintf(achFile, "%s\\%cWSORTED.TXT",
              wb.path, ((itest == ITEST_TEXTWORDSORTI) ? 'I' : 'S'));
      fmWriteData(achFile, pFile, pwl->cb);
      free(pFile);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Write the uppercased/lowercased text to a file (debugging purpose only).
 ------------------------------------------------------------------------- */
VOID dumpTestCaseConvert(PSZ pText, ULONG itest) {
   CHAR achFile[CCHMAXPATH];
   sprintf(achFile, "%s\\%cCASED.TXT",
           wb.path, ((itest == ITEST_TEXTTOUPPER)? 'U': 'L'));
   fmWriteData(achFile, pText, CB_STRTESTFILE);
}


/* --------------------------------------------------------------------------
 ------------------------------------------------------------------------- */


/* --------------------------------------------------------------------------
 ------------------------------------------------------------------------- */
