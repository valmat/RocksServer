CPP             = g++
LD              = g++
#CPP             = clang++
#LD              = clang++

OPT_FLAG=-O3
#OPT_FLAG=-g

NAME            = smpl_plug

CPP_FLAGS       = -I. -I"/usr/include/rocksserver/include" -Wall ${OPT_FLAG} -std=c++11 -fPIC -c
LD_FLAGS        = -Wall -std=c++11 ${OPT_FLAG} -shared

RESULT          = $(NAME).so

SOURCES		= $(wildcard *.cpp)
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS}

clean:
		rm -f ${OBJECTS} ${RESULT}

${OBJECTS}: 
		${CPP} ${CPP_FLAGS} -o $@ ${@:%.o=%.cpp}

