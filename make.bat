@echo off

if exist "debug.txt" del debug.txt

if /i "%~1"=="-d" (
	tcc -Wall -DDEBUG pure8.c -o pure8.exe
	:: launch pure8 for test, only when using the -d(ebug) option ::
	cls
	pure8 rom.bin
) else if /i "%~1"=="-h" (
	echo MAKE in batch
	echo -------------
	echo.
	echo make [-h] [-d]
	echo -h : print the help about make.
	echo -d : build pure8 with debug active.
	echo -r : build pure8 as a release executable.
) else if /i "%~1"=="-r" (
	tcc -Wall pure8.c -o pure8.exe
) else (
	echo Error : no options specified or bad syntax !
	echo Write make -h to get the full help.
)