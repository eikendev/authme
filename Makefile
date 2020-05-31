CC = gcc
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
CFLAGS = -std=c99 -Wall -pedantic -lrt -pthread -g $(DEFS)

DIR_OUT = out
DIR_SRC = src
DIR_DOC = doc

BINS = $(DIR_OUT)/auth-server $(DIR_OUT)/auth-client

.PHONY: all
all: build

.PHONY: build
build: directories $(BINS) documentation

.PHONY: directories
directories:
	mkdir -p $(DIR_OUT)/client
	mkdir -p $(DIR_OUT)/server
	mkdir -p $(DIR_OUT)/share

.PHONY: documentation
documentation:
	doxygen

.PHONY: clean
clean:
	rm -rf $(DIR_OUT) $(DIR_DOC)

$(DIR_OUT)/client/%.o: $(DIR_SRC)/client/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(DIR_OUT)/server/%.o: $(DIR_SRC)/server/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(DIR_OUT)/share/%.o: $(DIR_SRC)/share/%.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(DIR_OUT)/auth-client: $(DIR_OUT)/client/auth-client.o $(DIR_OUT)/client/options.o $(DIR_OUT)/client/user.o $(DIR_OUT)/client/instruction.o $(DIR_OUT)/client/utils.o $(DIR_OUT)/share/utils.o $(DIR_OUT)/share/shmem.o
	$(CC) $(CFLAGS) -o $@ $^

$(DIR_OUT)/auth-server: $(DIR_OUT)/server/auth-server.o $(DIR_OUT)/server/options.o $(DIR_OUT)/server/utils.o $(DIR_OUT)/server/database.o $(DIR_OUT)/server/ipc.o $(DIR_OUT)/server/list.o $(DIR_OUT)/server/user.o $(DIR_OUT)/share/shmem.o $(DIR_OUT)/share/utils.o
	$(CC) $(CFLAGS) -o $@ $^
