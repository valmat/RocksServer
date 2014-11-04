CPP             = g++
RM              = rm -f

CPP_FLAGS       = -c -I. -O3 -std=c++11

LD              = g++
#LD_FLAGS        = -Wall -shared -O2 
LD_FLAGS        = -Wall
RESULT          = main

SOURCES		= $(wildcard *.cpp)
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS}

clean:
		${RM} ${OBJECTS} ${RESULT}

${OBJECTS}: 
#		${CPP} ${CPP_FLAGS} -fpic -o $@ ${@:%.o=%.cpp}
		${CPP} ${CPP_FLAGS} -o $@ ${@:%.o=%.cpp}
run:
		 ./${RESULT}

