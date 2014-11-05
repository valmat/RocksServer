CC 	 = clang++
#CC 	 = g++

NAME	 = RocksServer

CFLAGS	 = -std=c++11 -O3 -Wall -fPIC
#CFLAGS	 = -std=c++11 -g -Wall -fPIC

#LINKEROPT = -static
LINKEROPT = -rdynamic -O3

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
	$(CC) evhttpsrc/EvLogger.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvLogger.o
	$(CC) evhttpsrc/EvServer.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvServer.o
	$(CC) evhttpsrc/EvResponse.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvResponse.o
	$(CC) evhttpsrc/EvRequest.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o EvRequest.o
	$(CC) extend/Extend.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o Extend.o
	$(CC) extend/PlugContainer.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o PlugContainer.o
	$(CC) *.o $(CFLAGS) -levent -ldl -L$(ROCKSPATH) -L$(LEVENTL) $(ROCKSLIB) $(LINKEROPT) -o $(NAME).bin

#	$(CC) *.o $(CFLAGS) -levent -ldl -L$(ROCKSPATH) -L$(LEVENTL) $(ROCKSLIB) -shared -o $(NAME).so
#	$(CC) *.o $(CFLAGS) -levent -L$(LEVENTL) -shared -o $(NAME).so

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
	rm -f $(NAME).o $(NAME).bin *.o restore.bin human_readable.o human_readable.bin

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
	$(CPR) ${SRC}/rocks /usr/include/rocksserver/
	$(CPR) ${SRC}/iniparse /usr/include/rocksserver/
	$(CPR) ${SRC}/protocols /usr/include/rocksserver/
	$(CPR) ${SRC}/evhttp /usr/include/rocksserver/
	#
	$(CPR) ${SRC}/../deps/rocksdb/include /usr/include/rocksserver/
	#
	$(CP) ${SRC}/extend/Extension.h /usr/include/rocksserver/Extension.h
	$(CP) ${SRC}/extend/api.h /usr/include/rocksserver/api.h
	$(CP) ${SRC}/listeners/RequestBase.h /usr/include/rocksserver/RequestBase.h
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


