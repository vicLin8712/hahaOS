CC = clang
CFLAGS = -std=c11 -O0 -g3 -Wall -Wextra --target=riscv32-unknown-elf \
         -fno-stack-protector -ffreestanding -nostdlib \
         -Ilib -I. -Iinclude -Ikernel -Iarch
LDFLAGS = -Wl,-Tarch/kernel.ld -Wl,-Map=build/kernel.map

BUILD_DIR = build

SRCS = kernel/kernel.c kernel/task.c kernel/error.c lib/mm.c lib/stdio.c  lib/malloc.c arch/hal.c
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

TARGET = $(BUILD_DIR)/kernel.elf

QEMU = qemu-system-riscv32
QEMU_FLAGS = -machine virt -bios none -nographic -serial mon:stdio --no-reboot
QEMU_GDB = -machine virt -bios none -nographic -serial mon:stdio --no-reboot -s -S

FMT_SRCS := $(shell find . \( -name "*.c" -o -name "*.h" \))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET)

gdb: $(TARGET)
	$(QEMU) $(QEMU_GDB) -kernel $(TARGET)

clean:
	rm -rf $(BUILD_DIR)

format:
	@clang-format -i $(FMT_SRCS)