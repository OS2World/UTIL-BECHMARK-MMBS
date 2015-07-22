# To add new extensions to the inference rules uncomment and redefine this:
#.SUFFIXES:
#
#.SUFFIXES: \
#    .C .obj .rc .res

# compiler, linker, resource compiler, resource binder MACRO

CC = icc.exe
CL = ilink.exe
RC = rc.exe
RB = rc.exe

# compiler and linker flags

# Debug version
!ifdef DEBUG

CFLAGS = /Ss /Ti /G5 /C /Q
LFLAGS = /DE /E:2 /PM:VIO /PACKD /A:4 /OPTF /NOL /M /L

!else
# RELEASE version

CFLAGS = /Ss /O /Oc /Ol /G5 /C /Q
LFLAGS = /E:2 /PM:VIO /PACKD /A:4 /OPTF /NOL /M /L

!endif

RCFLAGS = -r -n -i ..\headers
RBFLAGS = -x2 -n

.rc.res:
   $(RC) $(RCFLAGS) $<

.C.obj:
   $(CC) $(CFLAGS) $<

all: workbnch.exe

workbnch.exe: \
   consoleIO.obj \
   fileTest.obj \
   fileUtil.obj \
   math.obj \
   stringTest.obj \
   stringTestAux.obj \
   testAux.obj \
   fileTestAux.obj \
   workbnch.obj
   $(CL) @<<
      $(LFLAGS)
      /O:workbnch.exe
      consoleIO.obj
      fileTest.obj
      fileUtil.obj
      math.obj
      stringTest.obj
      stringTestAux.obj
      testAux.obj
      fileTestAux.obj
      workbnch.obj
<<

consoleIO.obj: \
   consoleIO.c \
   workbnch.h \
   messages.h

fileTest.obj: \
   fileTest.c \
   filetest.h \
   workbnch.h \
   messages.h

fileTestAux.obj: \
   fileTestAux.c \
   filetest.h \
   workbnch.h \
   fileUtil.h \
   messages.h

fileUtil.obj: \
   fileUtil.c \
   workbnch.h \
   fileUtil.h \
   messages.h

math.obj: \
   math.c \
   workbnch.h \
   messages.h

stringTest.obj: \
   stringTest.c \
   stringTest.h \
   workbnch.h \
   messages.h

stringTestAux.obj: \
   stringTestAux.c \
   stringTest.h \
   workbnch.h \
   messages.h

testAux.obj: \
   testAux.c \
   workbnch.h \
   messages.h

workbnch.obj: \
   workbnch.c \
   workbnch.h \
   fileUtil.h \
   messages.h


