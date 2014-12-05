#CC 	 = clang++
CC 	 = g++

NAME	 = RocksServer

OPT_FLAG=-O4
#OPT_FLAG=-g -O0

CFLAGS	 = -std=c++11 $(OPT_FLAG) -Wall

#LINKEROPT = -static
LINKEROPT = -rdynamic $(OPT_FLAG)

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

${BINFILE}: 
	$(CC) $(NAME).cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o $(NAME).o
	$(CC) evhttp/EvLogger.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvLogger.o
	$(CC) evhttp/EvServer.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvServer.o
	$(CC) evhttp/EvResponse.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvResponse.o
	$(CC) evhttp/EvRequest.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvRequest.o
	$(CC) rocks/RocksDBWrapper.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o RocksDBWrapper.o
	$(CC) extend/Extend.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o Extend.o
	$(CC) extend/PlugContainer.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o PlugContainer.o
	$(CC) extend/Extension.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o Extension.o
	$(CC) *.o $(CFLAGS) -levent -ldl -L$(ROCKSPATH) -L$(LEVENTL) $(ROCKSLIB) $(LINKEROPT) -o $(NAME).bin

	$(CC) restore.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -c -o restore.o
	$(CC) restore.o $(CFLAGS) -L$(ROCKSPATH) $(ROCKSLIB) $(LINKEROPT) -o restore.bin
	$(CC) human_readable.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -c -o human_readable.o
	$(CC) human_readable.o $(CFLAGS) -L$(ROCKSPATH) $(ROCKSLIB) $(LINKEROPT) -o human_readable.bin
	@echo
	@echo "Build complete."
#	@echo "For start server run ./$(NAME).bin config.ini"

	
run:
	./$(NAME).bin

clean:
	rm -f *.o *.bin

install:
	mkdir -p /var/rocksserver
	chown nobody /var/rocksserver
	$(CP) ${SRC}/RocksServer.bin /usr/local/bin/rocksserver
	$(CP) ${SRC}/restore.bin /usr/local/bin/rocksrestore
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
	rm -f /usr/local/bin/rocks_human_readable
	rm -f /etc/init.d/rocksserver


