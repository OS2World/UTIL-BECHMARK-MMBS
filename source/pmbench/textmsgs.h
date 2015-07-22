
// main window title
#define SZ_TITLE    "PM benchmarks"

// test result header
#define SZ_HEADER \
"Test description                      ³  Elapsed time  ³   "\
"Operations   ³  Operations/sec"

#define SZ_HEADERSEP \
"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄ"\
"ÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ"

#define SZ_PROMPT \
"Press \'Enter\' to start the test..."

#define SZ_SINGLERESULT \
"[Elapsed time : % 10.3f]       [Operations : % 10u]       [Operations/sec : % 10.3f]"

// test descriptions
#define SZ_BMPSMOOTH        "Draw bitmap (smoothness test)"
#define SZ_BMPDRAW          "Draw bitmap (speed)"
#define SZ_BMPSTRETCH       "Stretch bitmap"
#define SZ_LINES            "Draw horizontal/vertical lines"
#define SZ_FILLRECT         "Fill rectangles"
#define SZ_DRAWTEXT         "Draw text"
#define SZ_LOADDLG          "Load dialog"
#define SZ_MOVEWIN          "Move dialog"
#define SZ_SIZEWIN          "Resize dialog"

#define ADESCRIPTIONS \
{SZ_BMPSMOOTH, SZ_BMPDRAW, SZ_BMPSTRETCH, SZ_LINES, \
 SZ_FILLRECT, SZ_DRAWTEXT, SZ_LOADDLG, SZ_MOVEWIN, SZ_SIZEWIN}

// options dialog
#define SZ_DOGRP \
"Benchmark test iteration factor :"
#define SZ_DOTEXT \
"With an iteration factor of 10 each test takes about one minute on a P4 "\
"3GHz machine.\x0d\x0a"\
"Choose smaller iteration factors to complete the tests in a shorter"\
" time."
#define SZ_DOPROMPT \
"~Prompt the user for the next test"
#define SZ_DOSAVE \
"Save the results as:"
#define SZ_DOOK \
"~OK"
#define SZ_DOCANCEL \
"~Cancel"


