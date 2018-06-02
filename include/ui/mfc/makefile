
include ..\msvc6.mak

NEEDLIB=comctl32.lib kernel32.lib user32.lib gdi32.lib advapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib 

all: test_mfc.exe

test_mfc.exe: test_mfc.obj
	-$(LINK) test_mfc.obj $(NEEDLIB) /out:$@
