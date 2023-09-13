CC := gcc
CFLAGS := -I include -O3 -Wall -ansi -Wno-pedantic -fno-common -fno-strict-aliasing -fno-stack-protector -fno-builtin -std=c99 -fomit-frame-pointer -ffast-math -finline-functions -funroll-loops -flto -mtune=native -march=native

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CFLAGS += -lrt
endif

BUILD_DIR := build
SRC_DIR := .
TARGET := barn

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

.PHONY: all clean

all: $(TARGET) clean

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

install:
	mkdir -p $(HOME)/.barn/
	mkdir -p $(HOME)/.barn/libs/
	sudo cp ./barn /bin/
	sudo cp ./barn /usr/bin/
	sudo cp -r ./libs/ $(HOME)/.barn/
