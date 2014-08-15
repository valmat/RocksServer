CC 	 = clang++
#CC 	 = g++

NAME	 = RocksServer

#CFLAGS	 = -std=c++11 -O3 -Wall 
CFLAGS	 = -std=c++11 -g -Wall 

BINFILE  = $(NAME).bin

# path to RocksDB source
ROCKSPATH	 = ../../rocksdb-rocksdb-3.3

ROCKSLIB = -lrocksdb -lpthread -lrt -lsnappy -lgflags -lz -lbz2

all: ${BINFILE}

${BINFILE}: 
	$(CC) $(NAME).cpp $(CFLAGS) -I. -I$(ROCKSPATH)/include -c -o $(NAME).o
	$(CC) $(NAME).o $(CFLAGS) -levent -L$(ROCKSPATH) $(ROCKSLIB) -o $(NAME).bin
	@echo
	@echo "Build complete."
	@echo "For start server run ./$(NAME).bin config.ini"

	
run:
	./$(NAME).bin

clean:
	rm -f $(NAME).o $(NAME).bin
