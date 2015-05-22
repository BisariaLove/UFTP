CC=gcc
SHELL:=$(shell which bash) -e -o pipefail
UNAME_S:=$(shell uname -s)
_CFLAGS=-Wall -Wextra -Werror -Wshadow -Winline -D_GNU_SOURCE -std=c99 -g

BIN_DIR=$(CURDIR)/bin
SRC_DIR=$(CURDIR)/src
DATA_DIR=$(CURDIR)/etc/data
SCRIPT_DIR=$(CURDIR)/scripts
NODE_A_DIR=$(SRC_DIR)/a
NODE_B_DIR=$(SRC_DIR)/b
LIB_DIR=$(SRC_DIR)/lib

# All header files in libraries
INCLUDES_A=-I$(SRC_DIR) -I$(CURDIR) \
	   -I$(CURDIR)/deps/uthash \
	   -I$(NODE_A_DIR) \
	   -I$(LIB_DIR) \
	   -lpthread \

INCLUDES_B=-I$(SRC_DIR) -I$(CURDIR) \
	   -I$(CURDIR)/deps/uthash \
	   -I$(NODE_B_DIR) \
	   -I$(LIB_DIR) \
	   -lpthread \

# All .c files in src folder
_SOURCES=$(shell find $(SRC_DIR) -type f -iname '*.[c]')
_SOURCES_A=$(shell find $(NODE_A_DIR) -type f -iname '*.[c]')
_SOURCES_B=$(shell find $(NODE_B_DIR) -type f -iname '*.[c]')
_SOURCES_LIB=$(shell find $(LIB_DIR) -type f -iname '*.[c]')

# Create 1GB file
create:
	@echo "Creating 1GB file :"
	$(SCRIPT_DIR)/create1GB.sh

# Build
build_a:
	-mkdir -p $(BIN_DIR)
	touch $(DATA_DIR)/recv.bin
	rm $(DATA_DIR)/recv.bin
	$(CC) -o $(BIN_DIR)/a $(_SOURCES_LIB) $(_SOURCES_A) $(INCLUDES_A)

build_b:
	-mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/b $(_SOURCES_LIB) $(_SOURCES_B) $(INCLUDES_B)

clean:
	@echo "Deleting bin folder"
	rm -rf $(BIN_DIR)/*
