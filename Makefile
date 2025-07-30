# 編譯器與參數
CC = clang
CFLAGS = -std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf \
         -fno-stack-protector -ffreestanding -nostdlib
LDFLAGS = -Wl,-Tkernel.ld -Wl,-Map=kernel.map

# 檔案列表
SRCS = kernel.c mm.c
OBJS = $(SRCS:.c=.o)
TARGET = kernel.elf

# QEMU SETTING
QEMU = qemu-system-riscv32
QEMU_FLAGS = -machine virt -bios default -nographic -serial mon:stdio --no-reboot


# 預設目標
all: $(TARGET)

# 編譯 elf
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# 每個 .o 的規則
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# 執行 QEMU
run: $(TARGET)
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET)

# 清除目標
clean:
	rm -f $(OBJS) $(TARGET) kernel.map
