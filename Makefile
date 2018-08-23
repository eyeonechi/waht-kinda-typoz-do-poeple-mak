EXECS=src/approximate_string_search.cpp
NAME=src/approximate_string_search
CC=g++
DICT=data/dict.txt
CORRECT=data/wiki_correct.txt
CORRECT2=data/birkbeck_correct.txt
MISPELL=data/wiki_misspell.txt
MISPELL2=data/birkbeck_misspell.txt

all:
	${CC} -o ${NAME} ${EXECS}
	./${NAME} ${DICT} ${CORRECT} ${MISPELL}

compile:
	${CC} -o ${NAME} ${EXECS}

run:
	./${NAME} ${DICT} ${CORRECT} ${MISPELL}

run2:
	./${NAME} ${DICT} ${CORRECT2} ${MISPELL2}

wiki:
	${CC} -o ${NAME} ${EXECS}
	./${NAME} ${DICT} ${CORRECT} ${MISPELL}

birkbeck:
	${CC} -o ${NAME} ${EXECS}
	./${NAME} ${DICT} ${CORRECT2} ${MISPELL2}

clean:
	rm ${NAME}
