CC := clang
CFLAGS := -Wall -g -c
BIN := vm
SRC_DIR := source
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst %,%,${SRCS:.c=.o})

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -fuse-ld=lld -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm $(OBJS)
	rm $(BIN)