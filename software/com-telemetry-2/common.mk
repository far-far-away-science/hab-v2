# Universal C Makefile for MCU targets
# Top-level template file to configure build

-include stlink.mk

# Makefile for STM32L052XX series
DEVICE=STM32L0
# Libraries to include in the link (use -L and -l) e.g. -lm, -lmyLib
LIBRARIES=-lgcc -lm
# Prefix for ARM tools (must be on the path)
MCUPREFIX=arm-none-eabi-
# Flags for the assembler
MCUAFLAGS=-mthumb -mcpu=cortex-m0 -mlittle-endian
# Flags for the compiler
MCUCFLAGS=-mthumb -mcpu=cortex-m0 -mlittle-endian
# Flags for the linker
MCULFLAGS=-nostartfiles -Wl,-static -Bfirmware -Wl,-T,cortex.ld
# Prepares the elf file by converting it to a binary for ST-Link
MCUPREPARE=$(OBJCOPY) $(OUT) -O binary $(BINDIR)/$(OUTBIN)
# Advanced sizing flags
SIZEFLAGS=
# Uploads program using ST-Link
UPLOAD=cd $(BINDIR) && $(STLINK)/ST-LINK_CLI.exe -c SWD -P output.bin 0x08000000 -V -Rst

# Advanced options
ASMEXT=s
CEXT=c
CPPEXT=cpp
HEXT=h
INCLUDE=-I$(ROOT)/include -I$(ROOT)/src
OUTBIN=output.bin
OUTNAME=output.elf

# Flags for programs
AFLAGS:=$(MCUAFLAGS)
ARFLAGS:=$(MCUCFLAGS)
CCFLAGS:=-c -Wall $(MCUCFLAGS) -Os -ffunction-sections -fsigned-char -fomit-frame-pointer \
-Werror=implicit-function-declaration
CFLAGS:=$(CCFLAGS) -std=gnu99
CPPFLAGS:=$(CCFLAGS) -fno-exceptions -fno-rtti -felide-constructors
LDFLAGS:=-Wall $(MCUCFLAGS) $(MCULFLAGS) -Wl,--gc-sections

# Tools used in program
AR:=$(MCUPREFIX)ar
AS:=$(MCUPREFIX)as
CC:=$(MCUPREFIX)gcc
CPPCC:=$(MCUPREFIX)g++
OBJCOPY:=$(MCUPREFIX)objcopy
