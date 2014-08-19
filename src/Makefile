CC 	 = clang++
#CC 	 = g++

NAME	 = RocksServer

CFLAGS	 = -std=c++11 -O3 -Wall
#CFLAGS	 = -std=c++11 -g -Wall


#LINKEROPT = -static
LINKEROPT = 

BINFILE  = $(NAME).bin

# path to RocksDB source
ROCKSPATH	 = ../../rocksdb-rocksdb-3.3

ROCKSLIB = -lrocksdb -lpthread -lrt -lsnappy -lgflags -lz -lbz2

all: ${BINFILE}

${BINFILE}: 
	$(CC) $(NAME).cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -c -o $(NAME).o
	$(CC) $(NAME).o $(CFLAGS) -levent -L$(ROCKSPATH) $(ROCKSLIB) $(LINKEROPT) -o $(NAME).bin
	$(CC) restore.cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -c -o restore.o
	$(CC) restore.o $(CFLAGS) -L$(ROCKSPATH) $(ROCKSLIB) $(LINKEROPT) -o restore.bin
	@echo
	@echo "Build complete."
	@echo "For start server run ./$(NAME).bin config.ini"

	
run:
	./$(NAME).bin

clean:
	rm -f $(NAME).o $(NAME).bin restore.o restore.bin

install:
	mkdir -p /var/rocksserver
	chown nobody /var/rocksserver
	mkdir -p /etc/rocksserver
	cp -f config.ini /etc/rocksserver/config.ini
	cp -f RocksServer.bin /usr/local/bin/rocksserver
	cp -f restore.bin /usr/local/bin/rocksrestore
	cp -f ../init.d/rocksserver /etc/init.d/rocksserver
	update-rc.d rocksserver defaults

uninstall:
	update-rc.d -f rocksserver remove
	rm -fR /etc/rocksserver
	rm -f /usr/local/bin/rocksrestore
	rm -f /etc/init.d/rocksserver

uninstall-clean:
	update-rc.d -f rocksserver remove
	rm -fR /var/rocksserver
	rm -fR /etc/rocksserver
	rm -f /usr/local/bin/rocksrestore
	rm -f /etc/init.d/rocksserver


