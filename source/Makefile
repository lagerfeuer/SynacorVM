SRCS := $(wildcard *.c)
OBJS := $(patsubst %,%,${SRCS:.c=.o})

all: $(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN): cpu disasm stack $(OBJS)
	$(CC) -fuse-ld=lld -o $@ $^

cpu:
	make -C cpu

disasm:
	make -C disassembler

stack:
	make -C stack
	

clean:
	make -C cpu clean
	make -C disassembler clean
	make -C stack clean

.PHONY: cpu disasm stack clean
