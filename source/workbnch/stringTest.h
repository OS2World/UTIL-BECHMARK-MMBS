//===========================================================================
// .h :
//
// --2004  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _STRINGTEST_H_
   #define _STRINGTEST_H_

#define CB_STRTESTFILE        (1024*1024*2)
#define SZ_WORDSLIST          "WORDLIST.BIN"
#define CB_WORDARRAY          2024301


typedef struct {
   ULONG cb;                // real size of the achWords array
   ULONG count;             // count of words
   CHAR achWords[1];        // words storage
} WORDLIST, * PWORDLIST;


BOOL isCharValid(INT ch);
PSZ stripDelimiters(PSZ p);
PSZ addWord(PSZ psz, PWORDLIST pwl);
BOOL saveWordList(PWORDLIST pwl);
PWORDLIST readWordArrayFile(VOID);
PSZ* makeWordArray(PWORDLIST pwl);
VOID dumpSortedList(PSZ* ppszWords, BOOL insensitive, PWORDLIST pwl);
VOID dumpTestCaseConvert(PSZ pText, ULONG itest);

#endif // #ifndef _STRINGTEST_H_
