include version.mk

APPNAME := emu68_bbb

CC := gcc
CXX := g++
OBJCOPY := objcopy

OBJDIR := build
PLATFORM := bbb

BBB_SOURCES := $(wildcard *.c $(PLATFORM)/*.c)
BBB_OBJECTS := $(patsubst %.c,$(OBJDIR)/%.o, $(patsubst $(PLATFORM)/%.c,$(OBJDIR)/%.o, $(BBB_SOURCES)))

DEFINES := -DVERSION_STRING_DATE='$(VERSION_STRING_DATE)'

COMMON_FLAGS := $(EXTRA_FLAGS) -Iinclude -mbig-endian -marm -mcpu=cortex-a8 -Os \
	-mfpu=neon-vfpv4 -pedantic -pedantic-errors -ffixed-r11 -fomit-frame-pointer \
	-Wall -Wextra -Werror -nostdlib -ffreestanding

#-fno-builtin

CFLAGS := $(COMMON_FLAGS) $(DEFINES) -std=gnu11
CXXFLAGS:= $(COMMON_FLAGS) $(DEFINES) -std=c++11
LDFLAGS := -static -lrt -s \
	-nostdlib -nostartfiles \
	-T ldscript-be.lds \
	-Wl,--Map=$@.map \
	-Wl,--build-id \
	-Wl,--be8 \
	-Wl,--format=elf32-bigarm

.PHONY: all clean examples

all: premake $(BBB_OBJECTS)
	$(CC) -s $(BBB_OBJECTS) -o $(APPNAME).elf $(LDFLAGS)
	$(OBJCOPY) -O binary $(APPNAME).elf $(APPNAME).bin
	
premake:
	@mkdir -p build
	
examples:

clean:
	rm -rf $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(PLATFORM)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

