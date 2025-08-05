CC = clang
CFLAGS = -std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf \
         -fno-stack-protector -ffreestanding -nostdlib \
         -Ilib -I. -Iinclude -Ikernel
LDFLAGS = -Wl,-Tarch/kernel.ld -Wl,-Map=build/kernel.map

BUILD_DIR = build

SRCS = kernel/kernel.c kernel/task.c lib/mm.c lib/stdio.c lib/sbi.c
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

TARGET = $(BUILD_DIR)/kernel.elf

QEMU = qemu-system-riscv32
QEMU_FLAGS = -machine virt -bios default -nographic -serial mon:stdio --no-reboot

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET)

clean:
	rm -rf $(BUILD_DIR)
