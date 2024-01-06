H = $(L3X)\src\h
L = $(L3X)\lib

CC = bcc -mh 
CFLAGS = $(L3XCOPTS) -I$(H) -w- -DTURBOC
COUTFLAG = -e
ASM = tasm /MX /I$(H)
LINK = tlink
LIB = tlib
LIBFLAGS = /c
