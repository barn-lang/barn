CC := gcc
CFLAGS := -I include -O3 -Wall -ansi -Wno-pedantic -fno-common -fno-strict-aliasing -fno-stack-protector -fno-builtin -std=c99 -fomit-frame-pointer -ffast-math -finline-functions -funroll-loops -flto -mtune=native -march=native -Wno-typedef-redefinition

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CFLAGS += -lrt
endif

BUILD_DIR := build
SRC_DIR := ./src
TARGET := barn

SRCS := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/**/*.c $(SRC_DIR)/**/**/*.c)
OBJS := $(SRCS:.c=.o)

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
	sudo cp -r ./libs/ $(HOME)/.barn/
