#
# For making GBM_SCHEDULE program.
#
#CC     = quantify CC
#CC     = purify CC

CC = cc
#CC = gcc

CFLAGS =  -c -g -m64 -I$(INCL_DIR) $(DEFINES) 

BIN_DIR = ../bin

NETLIB = -lsocket -lnsl

.NAMEMAX: 512

ALLIBS =   -lm -ldl -m64 

PROGRAMS = $(BIN_DIR)/TjbParser

OBJECTS =  main.o PropertyParser.o

.SUFFIXES: .cpp .o .h .c


.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all : $(PROGRAMS) 

$(PROGRAMS) : $(OBJECTS) 
	$(CC) -o $@  $(OBJECTS) $(ALLIBS) 



clean:
	rm *o
	rm -rf SunWS_cache
	CCadmin -clean=all -d./
