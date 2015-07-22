//===========================================================================
// math.c : simple integer, float and trigonometric operations benchmarks
// 2004-01-22  - Alessandro Felice Cantatore
//===========================================================================


#include "workbnch.h"

// definitions --------------------------------------------------------------


// prototypes ---------------------------------------------------------------


// globals ------------------------------------------------------------------

BOOL testInt(ULONG iterations) {
   INT i, j, k, res, total;
   if (!prepareTest(ITEST_MATHINT, iterations, SZ_TESTMATHINT, TRUE))
      return FALSE;
   resetTimer();
   for (i = 0; i < 320; ++i) {
      for (j = 0, total = 1000 * iterations, k = 1, res = 1; j < total; ++j) {
         res -= k++;
         res += k++;
         res *= k++;
         if (!k) ++k;
         res /= k++;
      } /* endfor */
      if (wb.progress) printProgress(i, i + 1);
   } /* endfor */
   wb.prevStep = i;
   wb.operations = 3200000.0 * iterations;
   endTest(TRUE);
   return TRUE; // just to increase the completed tests counter
}


BOOL testDouble(ULONG iterations) {
   INT i, j, total;
   double k, res;
   if (!prepareTest(ITEST_MATHFLOAT, iterations, SZ_TESTMATHFLOAT, TRUE))
      return FALSE;
   resetTimer();
   for (i = 0; i < 320; ++i) {
      for (j = 0, total = 1000 * iterations, k = 1.0e10, res = 1.0e10; j < total; ++j) {
         res -= k++;
         res += k++;
         res *= k++;
         if (!k) ++k;
         res /= k++;
      } /* endfor */
      if (wb.progress) printProgress(i, i + 1);
   } /* endfor */
   wb.prevStep = i;
   wb.operations = 3200000.0 * iterations;
   endTest(TRUE);
   return TRUE; // just to increase the completed tests counter
}


BOOL testTrig(ULONG iterations) {
   INT i, j, total;
   double k, sine, cosine, tangent, logarithm, squareroot;
   if (!prepareTest(ITEST_MATHTRIG, iterations, SZ_TESTMATHTRIG, TRUE))
      return FALSE;
   resetTimer();
   for (i = 0; i < 320; ++i) {
      for (j = 0, total = 1000 * iterations, k = 0.0; j < total; ++j, ++k) {
         sine = sin(k);
         cosine = cos(k);
         tangent = tan(k);
         logarithm = log10(k);
         squareroot = sqrt(k);
      } /* endfor */
      if (wb.progress) printProgress(i, i + 1);
   } /* endfor */
   wb.prevStep = i;
   wb.operations = 3200000.0 * iterations;
   endTest(TRUE);
   return TRUE; // just to increase the completed tests counter
}
