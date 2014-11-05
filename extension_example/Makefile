CPP             = g++
LD              = g++
#CPP             = clang++
#LD              = clang++


#CPP_FLAGS       = -I. -I"$(PWD)/../deps/rocksdb/include" -g -std=c++11 -fPIC -c
CPP_FLAGS       = -I. -I"$(PWD)/../deps/rocksdb/include" -O3 -std=c++11 -fPIC -c

#LD_FLAGS        = -Wall -std=c++11 -g -shared
LD_FLAGS        = -Wall -std=c++11 -O3 -shared

NAME            = smpl_plug

RESULT          = $(NAME).so

SOURCES		= $(wildcard *.cpp)
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS}

clean:
		rm -f ${OBJECTS} ${RESULT}

${OBJECTS}: 
#		${CPP} ${CPP_FLAGS} -fpic -o $@ ${@:%.o=%.cpp}
		${CPP} ${CPP_FLAGS} -o $@ ${@:%.o=%.cpp}



#// g++ -std=c++11 -shared -fPIC -o smpl_plug.so smpl_plug.cpp

#// g++ -std=c++11 -fPIC -I../deps/rocksdb -c smpl_plug.cpp
#// g++ -std=c++11 -shared smpl_plug.o -o smpl_plug.so

#// g++ -std=c++11 -fPIC -I../deps/rocksdb/include -c smpl_plug.cpp
