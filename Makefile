PROGRAM_A=main_a
PROGRAM_B=main_b
SOURCES_A=main_a.c morse_code.c 
SOURCES_B=main_b.c morse_code.c

TESTER=as1_testbenc


CFLAGS=-O2 -g -Wall -pedantic


all: ${PROGRAM_A} ${PROGRAM_B} ${TESTER}

${PROGRAM_A}: ${SOURCES_A} morse_code.h
	gcc ${CFLAGS} -o ${PROGRAM_A} ${SOURCES_A}

${PROGRAM_B}: ${SOURCES_B} morse_code.h
	gcc ${CFLAGS} -o ${PROGRAM_B} ${SOURCES_B}

${TESTER}:
	gcc ${CFLAGS} -o ${TESTER} as1_testbench.c

.PHONY: test
test: all
	./${TESTER} ${PROGRAM_A} ${PROGRAM_B}



.PHONY: clean
clean:
	rm -rf *.o *~ ${PROGRAM_A} ${PROGRAM_B} ${TESTER}
