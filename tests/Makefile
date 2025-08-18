# tests/Makefile
CC      := clang
CFLAGS  := -std=c11 -O0 -g -Wall -Wextra -I../include -DUNIT_TEST
LDFLAGS := 

# 只測 list 邏輯（用主機 malloc）
SRCS    := test_list.c
BIN     := test_list

all: $(BIN)

$(BIN): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN) *.o
