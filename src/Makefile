#CC 	 = clang++
CC 	 = g++

NAME	 = RocksServer

OPT_FLAGS=-O4
#OPT_FLAGS=-g -O0

#compiler flags
CFLAGS	 = -std=c++11 $(OPT_FLAGS) -pedantic -Wall -Wextra -Wno-unused-parameter -fno-rtti -fno-exceptions



BINFILE  = bin/$(NAME).bin

# path to RocksDB source
ROCKSPATH	 = $(PWD)/../deps/rocksdb

# ROCKSLIB = -lrocksdb -lpthread -lrt -lsnappy -lgflags -lz -lbz2 -lzstd -lbz2 -llz4
ROCKSLIB = -lrocksdb -lpthread -lsnappy -lz -lzstd -lbz2 -llz4


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


RocksServer.h.gch: RocksServer.h
	g++ $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c RocksServer.h -o RocksServer.h.gch

bin/obj/$(NAME).rs.o: $(NAME).cpp
	$(CC) $(NAME).cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o bin/obj/$(NAME).rs.o

bin/obj/EvLogger.rs.o: evhttp/EvLogger.cpp
	$(CC) evhttp/EvLogger.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o bin/obj/EvLogger.rs.o

bin/obj/EvServer.rs.o: evhttp/EvServer.cpp
	$(CC) evhttp/EvServer.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o bin/obj/EvServer.rs.o

bin/obj/EvResponse.rs.o: evhttp/EvResponse.cpp
	$(CC) evhttp/EvResponse.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o bin/obj/EvResponse.rs.o

bin/obj/EvRequest.rs.o: evhttp/EvRequest.cpp
	$(CC) evhttp/EvRequest.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o bin/obj/EvRequest.rs.o

bin/obj/RocksDBWrapper.rs.o: rocks/RocksDBWrapper.cpp
	$(CC) rocks/RocksDBWrapper.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o bin/obj/RocksDBWrapper.rs.o

bin/obj/Extend.rs.o: extend/Extend.cpp
	$(CC) extend/Extend.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o bin/obj/Extend.rs.o

bin/obj/PlugContainer.rs.o: extend/PlugContainer.cpp
	$(CC) extend/PlugContainer.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o bin/obj/PlugContainer.rs.o

bin/obj/Extension.rs.o: extend/Extension.cpp
	$(CC) extend/Extension.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o bin/obj/Extension.rs.o

bin/obj/IniConfigs.rs.o: iniparse/IniConfigs.cpp
	$(CC) iniparse/IniConfigs.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o bin/obj/IniConfigs.rs.o

bin/obj/IniValue.rs.o: iniparse/IniValue.cpp
	$(CC) iniparse/IniValue.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c -o bin/obj/IniValue.rs.o

bin/obj/restore.o: utils/restore.cpp
	$(CC) utils/restore.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -c -o bin/obj/restore.o

bin/obj/restore_hr.o: utils/restore_hr.cpp
	$(CC) utils/restore_hr.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -c -o bin/obj/restore_hr.o

bin/obj/human_readable.o: utils/human_readable.cpp
	$(CC) utils/human_readable.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -c -o bin/obj/human_readable.o

.PHONY: obj
obj: RocksServer.h.gch bin/obj/$(NAME).rs.o bin/obj/EvLogger.rs.o bin/obj/EvServer.rs.o bin/obj/EvResponse.rs.o bin/obj/EvRequest.rs.o bin/obj/RocksDBWrapper.rs.o bin/obj/Extend.rs.o bin/obj/PlugContainer.rs.o bin/obj/Extension.rs.o bin/obj/IniConfigs.rs.o bin/obj/IniValue.rs.o bin/obj/restore.o bin/obj/restore_hr.o bin/obj/human_readable.o


bin/restore.bin: bin/obj/restore.o
	$(CC) bin/obj/restore.o $(CFLAGS) -L$(ROCKSPATH) $(ROCKSLIB) -o bin/restore.bin

bin/restore_hr.bin: bin/obj/restore_hr.o
	$(CC) bin/obj/restore_hr.o $(CFLAGS) -L$(ROCKSPATH) $(ROCKSLIB) -o bin/restore_hr.bin

bin/human_readable.bin: bin/obj/human_readable.o
	$(CC) bin/obj/human_readable.o $(CFLAGS) -L$(ROCKSPATH) $(ROCKSLIB) -o bin/human_readable.bin


#dynamic:
${BINFILE}: obj bin/restore.bin bin/restore_hr.bin bin/human_readable.bin
	$(CC) bin/obj/*.rs.o $(CFLAGS) -levent -ldl -L$(ROCKSPATH) -L$(LEVENTL) $(ROCKSLIB) -rdynamic -o bin/$(NAME).bin
	@echo
	@echo "\033[1;36mBuild complite \033[0m"
#	@echo "To start the server run ./bin/$(NAME).bin config.ini"


.PHONY: static
static: obj bin/restore.bin bin/restore_hr.bin bin/human_readable.bin
	$(CC) bin/obj/*.rs.o $(CFLAGS) $(LEVENTL)/libevent.a -ldl -L$(ROCKSPATH) $(ROCKSLIB) -rdynamic -o bin/$(NAME).bin
	@echo
	@echo "\033[1;34mBuild complite \033[0m"


.PHONY: clean install uninstall uninstall-clean


clean:
	rm -f bin/obj/*.o bin/*.bin RocksServer.h.gch

gch:
	g++ $(CFLAGS) -I. -I$(ROCKSPATH)/include -I$(LEVENTI) -c RocksServer.h -o RocksServer.h.gch


install:
	mkdir -p /var/rocksserver
	chown nobody /var/rocksserver
	$(CP) ${SRC}/bin/RocksServer.bin /usr/local/bin/rocksserver
	$(CP) ${SRC}/bin/restore.bin /usr/local/bin/rocksrestore
	$(CP) ${SRC}/bin/restore_hr.bin /usr/local/bin/rocksrestore_hr
	$(CP) ${SRC}/bin/human_readable.bin /usr/local/bin/rocks_human_readable
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


