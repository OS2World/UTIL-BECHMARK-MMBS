//===========================================================================
// stringTest.c : text string manipulation benchmarks
// 2004-01-22  - Alessandro Felice Cantatore
//===========================================================================


#include "workbnch.h"
#include "stringTest.h"

// definitions --------------------------------------------------------------


// prototypes ---------------------------------------------------------------


// globals ------------------------------------------------------------------

/* --------------------------------------------------------------------------
 This test measures the performances in counting the characters contained
 in a text file.
 ------------------------------------------------------------------------- */
#undef CSUBITER
#define CSUBITER 40
BOOL testTextCharStats(ULONG iterations) {
   ULONG charOccurrences[256];
   INT i, j;
   ULONG curstep;
   PSZ pRandomText, p;
   BOOL rc = FALSE;

   wb.cbProcessed = iterations * CSUBITER * CB_STRTESTFILE;
   if (!prepareTest(ITEST_TEXTCHARSTATS, iterations, SZ_TEXTCHARSTATS, TRUE))
      goto exit_0;
   // create a 2 MB buffer containing randomly generated text words
   if (NULL == (pRandomText = makeRandomText(CB_STRTESTFILE))) goto exit_0;
   fmWriteData("test\\random.txt", pRandomText, CB_STRTESTFILE); /* DEBUG */
   resetTimer();
   for (i = 0, wb.prevStep = 0; i < iterations; ++i) {
      for (j = 0; j < CSUBITER; ++j) {
         memset(charOccurrences, 0, sizeof(charOccurrences));
         for (p = pRandomText; *p; ++p) {
            charOccurrences[*p]++;
         } /* endfor */
      } /* endfor */
      if (wb.progress) {
         curstep = ((i + 1) * 320) / iterations;
         wb.prevStep = printProgress(wb.prevStep, curstep);
      } /* endif */
   } /* endfor */
   endTest(TRUE);
   rc = TRUE;
exit_1:
   free(pRandomText);
exit_0:
   return rc;
}


/* --------------------------------------------------------------------------
 This test measures the performances in extracting words from a text file.
 ------------------------------------------------------------------------- */
BOOL testTextWordParse(ULONG iterations) {
   PWORDLIST pwl;
   INT i;
   ULONG curstep;
   PSZ pRandomText, p;
   BOOL rc = FALSE;

   wb.cbProcessed = iterations * CB_STRTESTFILE;
   if (!prepareTest(ITEST_TEXTWORDPARSE, iterations, SZ_TEXTWORDPARSE, TRUE))
      goto exit_0;
   // create a 2 MB buffer containing randomly generated text words
   if (NULL == (pRandomText = makeRandomText(CB_STRTESTFILE))) goto exit_0;
   // allocate memory for storing the array of parsed words
   if (NULL == (pwl = (PWORDLIST)tryalloc(CB_STRTESTFILE))) goto exit_1;
   resetTimer();
   for (i = 0, wb.prevStep = 0; i < iterations; ++i) {
      pwl->count = 0;
      pwl->cb = 0;
      // loop through the random text buffer content
      for (p = pRandomText; *p;) {
         // strip leading delimiters
         p = stripDelimiters(p);
         p = addWord(p, pwl);
      } /* endfor */
      if (wb.progress) {
         curstep = ((i + 1) * 320) / iterations;
         wb.prevStep = printProgress(wb.prevStep, curstep);
      } /* endif */
   } /* endfor */
   endTest(TRUE);
   if (!saveWordList(pwl)) goto exit_2;
   rc = TRUE;
exit_2:
   free(pwl);
exit_1:
   free(pRandomText);
exit_0:
   return rc;
}


/* --------------------------------------------------------------------------
 This test measures the performances in sorting * text words contained in
 words array. Sorting is case insensitive or sensitive.
 Sort an array of words case insensitively or sensitively.
 - read the list of words from a binary file
 - convert the list of words in a words array
 - sort the array
 ------------------------------------------------------------------------- */
BOOL testTextWordSort(ULONG iterations,
                      INT itest, PSZ pszTestDescription, PQSORTCOMPF pSortFn) {
   PWORDLIST pwl;
   INT i;
   PSZ* ppszUnsortedWords;
   PSZ* ppszWords;
   ULONG curstep, cbarray;
   BOOL rc = FALSE;
   wb.cbProcessed = iterations * CB_WORDARRAY;
   if (!prepareTest(itest, iterations, pszTestDescription, TRUE))
      goto exit_0;
   // read the file containing the text words sequence
   if (NULL == (pwl = readWordArrayFile())) goto exit_0;
   // convert the text words sequence in an array of C string pointers
   if (NULL == (ppszUnsortedWords = makeWordArray(pwl))) goto exit_1;
   // allocate memory for copying the array
   cbarray = sizeof(PSZ*) * pwl->count;
   if (NULL == (ppszWords = tryalloc(cbarray))) goto exit_2;
   resetTimer();
   for (i = 0; i < iterations; ++i) {
      // copy the unsorted array to the array to be sorted
      memcpy(ppszWords, ppszUnsortedWords, cbarray);
      // sort the array of words
      qsort(ppszWords, pwl->count, sizeof(PSZ*), pSortFn);
      if (wb.progress) {
         curstep = ((i + 1) * 320) / iterations;
         wb.prevStep = printProgress(wb.prevStep, curstep);
      } /* endfor */
   } /* endfor */
   endTest(TRUE);
   dumpSortedList(ppszWords, itest, pwl); /* DEBUG */
   rc = TRUE;
   free(ppszWords);
exit_2:
   free(ppszUnsortedWords);
exit_1:
   free(pwl);
exit_0:
   return rc;
}


/* --------------------------------------------------------------------------
 This test measures the performances of upper or lower case text conversion.
 Create a memory buffer filling it with randomly generated pseudo text words.
 Convert the buffer content to upper case or lower case according to the
 value of the 'uppercase' parameter.
 ------------------------------------------------------------------------- */
#undef CSUBITER
#define CSUBITER 20
BOOL testTextCaseConvert(ULONG iterations,
                      INT itest, PSZ pszTestDescription, PCASEMAPFN pConvFn) {
   INT i, j;
   ULONG curstep;
   PSZ pRandomText, pRandomDup, p;
   BOOL rc = FALSE;
   wb.cbProcessed = iterations * CSUBITER * CB_STRTESTFILE;
   if (!prepareTest(itest, iterations, pszTestDescription, TRUE))
      goto exit_0;
   // create a 2 MB buffer containing randomly generated text words
   if (NULL == (pRandomText = makeRandomText(CB_STRTESTFILE))) goto exit_0;
   if (NULL == (pRandomDup = tryalloc(CB_STRTESTFILE))) goto exit_1;
   resetTimer();
   for (i = 0; i < iterations; ++i) {
      for (j = 0; j < CSUBITER; ++j) {
         memcpy(pRandomDup, pRandomText, CB_STRTESTFILE);
         for (p = pRandomDup; *p; ++p) {
            *p = pConvFn(*p);
         } /* endfor */
      } /* endfor */
      if (wb.progress) {
         curstep = ((i + 1) * 320) / iterations;
         wb.prevStep = printProgress(wb.prevStep, curstep);
      } /* endif */
   } /* endfor */
   endTest(TRUE);
   dumpTestCaseConvert(pRandomDup, itest);    /* DEBUG */
   rc = TRUE;
   free(pRandomDup);
exit_1:
   free(pRandomText);
exit_0:
   return rc;
}


/* --------------------------------------------------------------------------
 This test measures the performances of case insensitive or sensitive text
 word search.
 Create a memory buffer filling it with randomly generated pseudo text words.
 Perform a linear search of a word counting all the occurrences.
 ------------------------------------------------------------------------- */
#undef CSUBITER
#define CSUBITER 25
BOOL testTextWordFind(ULONG iterations,
                      INT itest, PSZ pszTestDescription, PFINDCOMPF pFindCmpFn) {
   INT i, j;
   ULONG curstep, cbRefWord, cFound;
   PSZ pRandomText, p, pRefWord;
   BOOL rc = FALSE;
   wb.cbProcessed = iterations * CSUBITER * CB_STRTESTFILE;
   if (!prepareTest(itest, iterations, pszTestDescription, TRUE))
      goto exit_0;
   // create a 2 MB buffer containing randomly generated text words
   if (NULL == (pRandomText = makeRandomText(CB_STRTESTFILE))) goto exit_0;
   // get the word to be searched
   srand(0xcfa);
   pRefWord = wb.prw->apszWords[rand() % CRANDWORDS];
   cbRefWord = strlen(pRefWord);
   resetTimer();
   for (i = 0; i < iterations; ++i) {
      for (j = 0; j < CSUBITER; ++j) {
         for (p = pRandomText; *p; ++p) {
            if (!pFindCmpFn(p, pRefWord, cbRefWord)) {
               cFound++;
               p += cbRefWord;
            } /* endif */
         } /* endfor */
      } /* endfor */
      if (wb.progress) {
         curstep = ((i + 1) * 320) / iterations;
         wb.prevStep = printProgress(wb.prevStep, curstep);
      } /* endif */
   } /* endfor */
   endTest(TRUE);
   rc = TRUE;
   free(pRandomText);
exit_0:
   return rc;
}


/* --------------------------------------------------------------------------
 ------------------------------------------------------------------------- */
