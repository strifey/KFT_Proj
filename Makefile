CC = gcc
CFLAGS = -std=gnu99 -pedantic -Wall -Wextra
OUT_DIR_SERVER = ./bin/server
OUT_DIR_CLIENT = ./bin/client
SRC_DIR_SERVER = ./src/server
SRC_DIR_CLIENT = ./src/client
SERVER_FILES = server.c s_main.c
CLIENT_FILES = client.c c_main.c
CLIENT_O_FILES = $(addprefix $(OUT_DIR_CLIENT)/,$(CLIENT_FILES:.c=.o))
SERVER_O_FILES =$(addprefix $(OUT_DIR_SERVER)/,$(SERVER_FILES:.c=.o))
CLIENT_C_FILES =$(addprefix $(SRC_DIR_CLIENT)/,$(CLIENT_FILES))
SERVER_C_FILES =$(addprefix $(SRC_DIR_SERVER)/,$(SERVER_FILES))


build: build-server build-client

build-client : $(CLIENT_O_FILES) ./bin/dropper.o
	$(CC) $(CFLAGS) $(CLIENT_O_FILES) ./bin/dropper.o -o ./kftclient

build-server : $(SERVER_O_FILES) ./bin/dropper.o
	$(CC) $(CFLAGS) $(SERVER_O_FILES) ./bin/dropper.o -o ./kftserver

$(OUT_DIR_SERVER)/%.o : $(SRC_DIR_SERVER)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OUT_DIR_CLIENT)/%.o : $(SRC_DIR_CLIENT)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

./bin/%.o : ./src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean : 
	rm -f ./bin/client/* ./bin/server/* kftclient kftserver


