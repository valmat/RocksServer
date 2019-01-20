CPP             = g++
LD              = g++
#CPP             = clang++
#LD              = clang++

OPT_FLAG=-O3
#OPT_FLAG=-g -O0

NAME       = simple_plugin

CPP_FLAGS  = -I. -I"/usr/include/rocksserver/rocksdb" -Wall ${OPT_FLAG} -std=c++11 -fPIC -c
LD_FLAGS   = -Wall -std=c++11 ${OPT_FLAG} -shared

RESULT     = $(NAME).so

SOURCES    = $(wildcard *.cpp)
OBJECTS    = $(patsubst %.cpp,obj/%.o,$(SOURCES))

all: ${RESULT}

${RESULT}: ${OBJECTS}
	${LD} ${LD_FLAGS} -o $@ ${OBJECTS}

${OBJECTS} : obj/%.o : %.cpp
	${CPP} ${CPP_FLAGS} $< -o $@


.PHONY: clean install uninstall

clean:
	rm -f ${OBJECTS} ${RESULT}

install:
	cp -f ${RESULT} /usr/lib/rocksserver/plugins/${RESULT}

uninstall:
	rm -f /usr/lib/rocksserver/plugins/${RESULT}

