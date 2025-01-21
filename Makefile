CC ?=
CFLAGS = 
LDFLAGS = -lcrypt # crypt
BUILD_DIR := build
SRC := $(wildcard *.c)
OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))
TARGET := $(BUILD_DIR)/program

.PHONY: all run clean $(shell run reset)

all: $(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
