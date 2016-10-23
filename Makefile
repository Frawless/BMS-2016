# Makefile for Cross Interleaved Reed Solomon encoder/decoder
#
# (c) Henry Minsky, Universal Access 1991-1996
#

RANLIB = ranlib
AR = ar
LIB = lib/


VERSION = 1.0
DIRNAME= SornaEncode


CC = gcc
# OPTIMIZE_FLAGS = -O69
DEBUG_FLAGS = -g
CFLAGS = -Wall -Wstrict-prototypes  $(OPTIMIZE_FLAGS) $(DEBUG_FLAGS) -I..
LDFLAGS = $(OPTIMIZE_FLAGS) $(DEBUG_FLAGS)

LIB_CSRC = $(LIB)rs.c $(LIB)galois.c $(LIB)berlekamp.c $(LIB)crcgen.c 
LIB_HSRC = $(LIB)ecc.h
LIB_OBJS = $(LIB)rs.o $(LIB)galois.o $(LIB)berlekamp.o $(LIB)crcgen.o 

TARGET_LIB = libecc.a
TEST_PROGS = bms1A bms1B

TARGETS = $(TARGET_LIB) $(TEST_PROGS)

all: $(TARGETS)

$(TARGET_LIB): $(LIB_OBJS)
	$(RM) $@
	$(AR) cq $@ $(LIB_OBJS)
	if [ "$(RANLIB)" ]; then $(RANLIB) $@; fi

bms1A: bms1A.o $(LIB)galois.o $(LIB)berlekamp.o $(LIB)crcgen.o $(LIB)rs.o
	gcc -o bms1A bms1A.o -L. -lecc
	
bms1B: bms1B.o $(LIB)galois.o $(LIB)berlekamp.o $(LIB)crcgen.o $(LIB)rs.o
	gcc -o bms1B bms1B.o -L. -lecc	

clean:
	rm -f *.o $(LIB)*.o bms1A bms1B libecc.a
	rm -f *~

dist:
	(cd ..; tar -cvf rscode-$(VERSION).tar $(DIRNAME))

depend:
	makedepend $(SRCS)
	
encode:
	./bms1A

decode:
	./bms1B

# DO NOT DELETE THIS LINE -- make depend depends on it.

