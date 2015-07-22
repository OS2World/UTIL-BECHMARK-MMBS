//===========================================================================
// messages.h :
// translatable text messages used for WORKBNCH.EXE
// 22-01-2004  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _MESSAGES_H_
   #define _MESSAGES_H_

// translatable messages ----------------------------------------------------

#define SZ_ABOUT \
"WORKBNCH.EXE : (c) 2004 - Alessandro Felice Cantatore\r\n"

#define SZ_HELP \
"Synopsys:\r\n"\
"[40;32;1m"\
"To run all the tests:\r\n"\
" [40;36;1m"\
"WORKBNCH [p:testpath] [l:logfile] [s:speed] [o:options]\r\n"\
"[40;32;1m"\
"To run a specific test (debug purpose only) :\r\n"\
" [40;36;1m"\
"WORKBNCH <t:test_id> [i:iterations] [p:testpath] [l:logfile] [o:options]\r\n"\
"[40;32;1m"\
"where:\r\n"\
" [40;36;1m"\
"testpath       "\
"[40;32;1m"\
": is the path to be used for writing the test files\r\n"\
"                  (default is \'TEST\' in the current path).\r\n"\
" [40;36;1m"\
"logfile        "\
"[40;32;1m"\
": output file for the test results (default is results.txt\r\n"\
"                  in the current path).\r\n"\
" [40;36;1m"\
"speed          "\
"[40;32;1m"\
": [0-9] set the test iterations multiplier. Set to 9 when\r\n"\
"                  testing a slow machine, less on faster machines to get\r\n"\
"                  more accurate results (the default speed is 7).\r\n"\
" [40;36;1m"\
"options        "\
"[40;32;1m"\
":    u (unattended) to run all tests without prompting\r\n"\
"                     q (quiet) do not display test details (only test id and\r\n"\
"                       iterations\r\n"\
"                     p (progress) to show a progress bar\r\n"\
"                     n (no-check) don't check for free space on the test path\r\n"\
" [40;36;1m"\
"test_id        "\
"[40;32;1m"\
": [1-23] is the test number\r\n"\
" [40;36;1m"\
"iterations     "\
"[40;32;1m"\
": how many times to repeat the test \'test_id\'\r\n"

#define SZ_DELPROMPT \
"                                \r\n"\
"                                   \r\n"

#define SZ_DELLINE \
"                                        "\
"                                        "

#define SZ_TESTDESCR \
"\r\nTest no. %d (iterations : %u)\r\n"

#define SZ_TIMERTEST \
"The purpose of this test is to check the accuracy of the virtual machine\r\n"\
"timers.\r\n"

#define SZ_TIMERSTART \
"Press any key to start the timer.\r\n"\
"Press the ESC key to terminate the test.\r\n"

#define SZ_TIMERSTOP \
"Press any key to stop the timer.\r\n"\
"Press the ESC key to terminate the test.\r\n"

#define SZ_TESTSTART \
"Press any key to start the test.\r\n"\
"Press the ESC key to skip the test.\r\n"

#define SZ_PAUSE \
"Press any key to continue.\r\n"

#define SZ_TIMERRESULT \
"Elapsed time      : % 8.3f seconds.\r\n"

#define SZ_MIBSPERSEC \
"Elaboration speed : % 8.3f MiBs per second.\r\n"

#define SZ_OPPERSEC \
"Elaboration speed : % 8.3f millions of operations per second.\r\n"

#define SZ_PLSWAIT \
"Please wait ...\r\n"

#define SZ_PROGRESSSTATE \
"current state : % 6.2f %%"

#define SZ_TOTBYTES \
" % .3f MBs.\r\n"

#define SZ_TESTMATHINT \
"This test measures the performances of integer number operations.\r\n"

#define SZ_TESTMATHFLOAT \
"This test measures the performances of float number operations.\r\n"

#define SZ_TESTMATHTRIG \
"This test measures the performances of trigonometric operations.\r\n"

#define SZ_TESTMULTIFILEWRITE \
"This test measures the file write performances.\r\n"\
"During this test 100 directories, 200 subdirectories and 6400 files\r\n"\
"will be created.\r\n"\
"The size of each file will range from 1 KB to 16 KB.\r\n"\
"The total size of data written to disk files will be : "

#define SZ_TESTLARGEFILEWRITE \
"This test measures the file write performances by writing a large\r\n"\
"text file (128 MB)\r\n" \
"The total size of data written to disk will be : "

#define SZ_TESTMULTIFILEREAD \
"This test measures the file read performances.\r\n"\
"During this test 6400 files will be read.\r\n"\
"The size of each file varies from 1 KB to 16 KB.\r\n"\
"The total size of data read from the disk will be : "

#define SZ_TESTLARGEFILEREAD \
"This test measures the file read performances by reading a large\r\n"\
"text file (128 MB) in 256 steps.\r\n"\
"The total size of data read from the disk will be : "

#define SZ_TESTMULTIFILECOPY \
"This test measures the file copy performances by copying a tree containing\r\n"\
"25 directories, 50 subdirectories and 1600 files.\r\n"\
"The size of each file varies from 1 KB to 16 KB.\r\n"\
"The total size of copied data will be : "

#define SZ_TESTLARGEFILECOPY \
"This test measures the file copy performances by copying a large\r\n"\
"text file (128 MB) in a unique step via DosCopy().\r\n"\
"The total size of copied data will be : "

#define SZ_TESTSEQFILEZIP \
"This test measures the file ZIP performances by sequentially zipping the files\r\n"\
"contained in a tree of 2 directories, 4 subdirectories (128 files).\r\n"\
"The compression is the highest (-9) and ZIP.EXE is executed for each file.\r\n"\
"The size of each file varies from 1 KB to 16 KB.\r\n"\
"The total size of the data to be ZIPPED is : "

#define SZ_TESTMULTIFILEZIP \
"This test measures the file ZIP performances by zipping a tree of files\r\n"\
"containing 16 directories, 32 subdirectories and 1024 files.\r\n"\
"The compression is the highest (-9) and ZIP.EXE is executed once.\r\n"\
"The size of each file varies from 1 KB to 16 KB.\r\n"\
"The total size of the data to be ZIPPED is : "

#define SZ_TESTLARGEFILEZIP \
"This test measures the file ZIP performances by zipping a 128 MBs file.\r\n"\
"The compression is the highest (-9).\r\n"\
"The total size of data to be ZIPPED is : "

#define SZ_TESTUNZIP \
"This test measures the file UNZIP performances by unzipping a tree of files\r\n"\
"containing 16 directories, 32 subdirectories and 1024 files.\r\n"\
"The compression is the highest (-9).\r\n"\
"The size of each file varies from 1 KB to 16 KB.\r\n"\
"The total size of UNZIPPED data will be : "

#define SZ_MULTIFILEDEL \
"This test measures the performances in deleting all the files created\r\n"\
"during the previous tests.\r\n"\
"376 directories and 8005 files will be deleted.\r\n"\
"The total size of deleted data will be : "

#define SZ_TEXTCHARSTATS \
"This test measures the performances in counting the characters contained\r\n"\
"in a text file.\r\n"\
"The total size of processed data will be : "

#define SZ_TEXTWORDPARSE \
"This test measures the performances in extracting words from a text file.\r\n"\
"The total size of processed data will be : "

#define SZ_TEXTWORDSORTI \
"This test measures the performances in sorting * text words contained in\r\n"\
"words array. Sorting is case insensitive.\r\n"\
"The total size of processed data will be : "

#define SZ_TEXTWORDSORTS \
"This test measures the performances in sorting * text words contained in\r\n"\
"words array. Sorting is case sensitive.\r\n"\
"The total size of processed data will be : "

#define SZ_TEXTTOUPPER \
"This test measures the performances of upper case text conversion.\r\n"\
"The total size of processed data will be : "

#define SZ_TEXTTOLOWER \
"This test measures the performances of lower case text conversion.\r\n"\
"The total size of processed data will be : "

#define SZ_TEXTFINDI \
"This test measures the performances of case insensitive text word search.\r\n"\
"The total size of processed data will be : "

#define SZ_TEXTFINDS \
"This test measures the performances of case sensitive text word search.\r\n"\
"The total size of processed data will be : "


#define SZ_TOTAL \
"WorkBench terminated!\r\n"\
"Tests succesfully completed : % 5d\r\n"\
"Total elapsed time          : % 9.3f seconds\r\n"

#define SZ_ANYKEYTOEXIT \
"\r\n\r\nPress any key to exit ...\r\n"

// error messages -----------------------------------------------------------

#define SZERR_INVALIDOPTION \
"Invalid option : %s\r\n"

#define SZERR_INVITER \
"Invalid \'iterations\' parameter\r\n"

#define SZERR_INVID \
"Invalid test id.\r\n"

#define SZERR_INVTESTPATH \
"The test path is not valid (there is no write access or not enough free\r\n"\
"space).\r\n"

#define SZERR_SPPED \
"Invalid speed argument.\r\n"

#define SZERR_MAKEPATH \
"Failed to create the test path.\r\n"

#define SZERR_OPENLOG \
"Failed to open the log file.\r\n"

#define SZERR_ALLOC \
"Allocation error.\r\n"

#define SZERR_CREATEDIR \
"Directory creation error.\r\n"

#define SZERR_WRITEFILE \
"Failed to write the file: %s.\r\n"

#define SZERR_OPENLARGEFILE \
"Failed to open: %s.\r\n"

#define SZERR_WRITELARGEFILE \
"Failed to write data to %s\\xllfile.txt.\r\n"

#define SZERR_READFILE \
"Failed to read the file: %s.\r\n"

#define SZERR_READFILECHUNK \
"Failed to read a part of the file: %s\\xllfile.txt.\r\n"

#define SZERR_COPYFILE \
"Failed to copy file: %s.\r\n"

#define SZERR_FILEDEL \
"Failed to delete the file: %s.\r\n"

#define SZERR_DIRDEL \
"Failed to delete the directory: %s.\r\n"

#define SZERR_SYSTEM \
"The C call system(%s) returned a non-zero value.\r\n"

#define SZERR_GETWORKDIR \
"Failed to get the current work directory.\r\n"

#define SZERR_SETWORKDIR \
"Failed to change the work directory.\r\n"

#define SZERR_GETPATHSIZE \
"Failed to get the total size of data contained in %s.\r\n"

#define SZERR_DELTREE \
"Failed to delete the content of %s.\r\n"

#define SZERR_READWORDARRAYFILE \
"Failed to read text words data from %s\\%s\r\n"

#define SZERR_MAKEWORDARRAY \
"Failed to create the text words array\r\n"

#endif // #ifndef _MESSAGES_H_
