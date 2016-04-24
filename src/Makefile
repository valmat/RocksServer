#CC 	 = clang++
CC 	 = g++

NAME	 = RocksServer

OPT_FLAGS=-O4
#OPT_FLAGS=-g -O0

#compiler flags
CFLAGS	 = -std=c++11 $(OPT_FLAGS) -Wall -pedantic



BINFILE  = $(NAME).bin

# path to RocksDB source
ROCKSPATH	 = $(PWD)/../deps/rocksdb

ROCKSLIB = -lrocksdb -lpthread -lrt -lsnappy -lgflags -lz -lbz2

# path to Libevent source
LEVENTPATH	 = $(PWD)/../deps/libevent

LEVENTI	 = $(LEVENTPATH)/include
LEVENTL	 = $(LEVENTPATH)/.libs

CP = cp -f
CPR = cp -fR
#CPR = ln -sf
#CP = ln -sf
SRC=`pwd`

all: ${BINFILE}



$(NAME).rs.o: $(NAME).cpp
	$(CC) $(NAME).cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o $(NAME).rs.o

EvLogger.rs.o: evhttp/EvLogger.cpp
	$(CC) evhttp/EvLogger.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvLogger.rs.o

EvServer.rs.o: evhttp/EvServer.cpp
	$(CC) evhttp/EvServer.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvServer.rs.o

EvResponse.rs.o: evhttp/EvResponse.cpp
	$(CC) evhttp/EvResponse.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvResponse.rs.o

EvRequest.rs.o: evhttp/EvRequest.cpp
	$(CC) evhttp/EvRequest.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvRequest.rs.o

RocksDBWrapper.rs.o: rocks/RocksDBWrapper.cpp
	$(CC) rocks/RocksDBWrapper.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o RocksDBWrapper.rs.o

Extend.rs.o: extend/Extend.cpp
	$(CC) extend/Extend.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o Extend.rs.o

PlugContainer.rs.o: extend/PlugContainer.cpp
	$(CC) extend/PlugContainer.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o PlugContainer.rs.o

Extension.rs.o: extend/Extension.cpp
	$(CC) extend/Extension.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o Extension.rs.o

restore.o: restore.cpp
	$(CC) restore.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -c -o restore.o

restore_hr.o: restore_hr.cpp
	$(CC) restore_hr.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -c -o restore_hr.o

human_readable.o: human_readable.cpp
	$(CC) human_readable.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -c -o human_readable.o

.PHONY: obj
obj: $(NAME).rs.o EvLogger.rs.o EvServer.rs.o EvResponse.rs.o EvRequest.rs.o RocksDBWrapper.rs.o Extend.rs.o PlugContainer.rs.o Extension.rs.o restore.o restore_hr.o human_readable.o


restore.bin: restore.o
	$(CC) restore.o $(CFLAGS) -L$(ROCKSPATH) $(ROCKSLIB) -o restore.bin

restore_hr.bin: restore_hr.o
	$(CC) restore_hr.o $(CFLAGS) -L$(ROCKSPATH) $(ROCKSLIB) -o restore_hr.bin

human_readable.bin: human_readable.o
	$(CC) human_readable.o $(CFLAGS) -L$(ROCKSPATH) $(ROCKSLIB) -o human_readable.bin


#dynamic:
${BINFILE}: obj restore.bin restore_hr.bin human_readable.bin
	$(CC) *.rs.o $(CFLAGS) -levent -ldl -L$(ROCKSPATH) -L$(LEVENTL) $(ROCKSLIB) -rdynamic -o $(NAME).bin
	@echo
	@echo "Build complete."
#	@echo "To start the server run ./$(NAME).bin config.ini"


.PHONY: static
static: obj restore.bin restore_hr.bin human_readable.bin
	$(CC) *.rs.o $(CFLAGS) $(LEVENTL)/libevent.a -ldl -L$(ROCKSPATH) $(ROCKSLIB) -rdynamic -o $(NAME).bin
	@echo
	@echo "Build complete."


.PHONY: clean install uninstall uninstall-clean


clean:
	rm -f *.o *.bin


install:
	mkdir -p /var/rocksserver
	chown nobody /var/rocksserver
	$(CP) ${SRC}/RocksServer.bin /usr/local/bin/rocksserver
	$(CP) ${SRC}/restore.bin /usr/local/bin/rocksrestore
	$(CP) ${SRC}/restore_hr.bin /usr/local/bin/rocksrestore_hr
	$(CP) ${SRC}/human_readable.bin /usr/local/bin/rocks_human_readable
	$(CP) ${SRC}/../init.d/rocksserver /etc/init.d/rocksserver
	mkdir -p /etc/rocksserver
	$(CP) ${SRC}/config.ini /etc/rocksserver/config.ini
	mkdir -p /usr/lib/rocksserver/plugins
	#
	mkdir -p /usr/include/rocksserver
	$(CPR) ${SRC}/include/* /usr/include/rocksserver/
	#
	mkdir -p /usr/include/rocksserver/rocksdb
	$(CPR) ${SRC}/../deps/rocksdb/include/* /usr/include/rocksserver/rocksdb/
	#
	update-rc.d rocksserver defaults

uninstall:
	update-rc.d -f rocksserver remove
	rm -f /usr/local/bin/rocksrestore
	rm -f /usr/local/bin/rocksrestore_hr
	rm -f /usr/local/bin/rocksserver
	rm -f /usr/local/bin/rocks_human_readable
	rm -f /etc/init.d/rocksserver

uninstall-clean:
	update-rc.d -f rocksserver remove
	rm -fR /var/rocksserver
	rm -fR /etc/rocksserver
	rm -fR /usr/lib/rocksserver/plugins
	rm -fR /usr/include/rocksserver
	rm -f /usr/local/bin/rocksrestore
	rm -f /usr/local/bin/rocksrestore_hr
	rm -f /usr/local/bin/rocks_human_readable
	rm -f /etc/init.d/rocksserver


