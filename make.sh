#!/bin/sh

if [ "$1" = "-h" ]
then
	echo "Make for GNU/Linux system"
	echo
	echo "Make syntax : make [-h] [-d] [-r]"
	echo "-h : print this help screen"
	echo "-d : compile with debug activate"
	echo "-r : compile a release version of the source"
elif [ "$1" = "-d" ]
then
	rm debug.txt
	rm pure8
	tcc -Wall -DDEBUG pure8.c -o pure8
elif [ "$1" = "-r" ]
then
	rm debug.txt
	rm pure8
	tcc -Wall pure8.c -o pure8
else
	echo "Error : unknown syntax or no options"
	echo "Type : ./make.sh -h to get the help"
fi
