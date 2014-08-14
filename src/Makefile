LIBEVENT = /usr/local/lib/libevent.a
CC 	 = clang++
#CC 	 = g++
NAME	 = RocksServer
CFLAGS	 = -std=c++11 -O3 -Wall 
#CFLAGS	 = -std=c++11 -g -Wall 
BINFILE  = $(NAME).bin
ROCKSP	 = ../../rocksdb-rocksdb-3.3
#ROCKSLIB = -lrocksdb -lpthread -lrt -lsnappy -lgflags -lz -lbz2
ROCKSLIB = -lpthread -lrt -lsnappy -lgflags -lz -lbz2




all: ${BINFILE}

${BINFILE}: 
	$(CC) $(NAME).cpp $(CFLAGS) -I. -I$(ROCKSP)/include -c -o $(NAME).o
	$(CC) $(NAME).o $(CFLAGS) $(LIBEVENT) $(ROCKSP)/librocksdb.a $(ROCKSLIB) -o $(NAME).bin
	@echo
	@echo "Build complete."
	@echo "For start server run ./$(NAME).bin config.ini"

	
run:
	./$(NAME).bin

clean:
	rm -f $(NAME).o $(NAME).bin
