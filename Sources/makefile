#---------------------------------------------------------------------------------------------------------#
# SPDX-License-Identifier: GPL-2.0                                                                        #
# Copyright (c) 2024 by Nuvoton Technology Corporation                                                    #
# All rights reserved                                                                                     #
#---------------------------------------------------------------------------------------------------------#
# File Contents:                                                                                          #
#   makefile                                                                                              #
#            This file contains the arbel_a35_bootblock  makefile                                         #
#  Project:  Arbel                                                                                        #
#---------------------------------------------------------------------------------------------------------#
.SUFFIXES:

THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))

# Edit this to change what make does when invoked without specifying a target.
DEFAULT_TARGET = help

# external flags from build.bar can be combination of:
# OPTIONAL_FLAGS += -D_SORT_ -DBOOTBLOCK_STACK_PROFILER

#
# Source tree roots
#
PROJ_DIR         = .
SWC_DEFS_DIR     = $(PROJ_DIR)/SWC_DEFS
HAL_DIR          = $(PROJ_DIR)/SWC_HAL
FW_DIR           = $(PROJ_DIR)/source
HAL_MODULES_DIR  = $(HAL_DIR)/ModuleDrivers
HAL_HL_DRVS_DIR  = $(HAL_DIR)/LogicalDrivers
HAL_CHIPS_DIR    = $(HAL_DIR)/Chips
HAL_CHIP_DIR     = $(HAL_DIR)/Chips/npcm850
HAL_COMMON_DIR   = $(HAL_DIR)/Common
APPS_DIR         = $(FW_DIR)/apps/serial_printf

#-------------------------------------------------------------------------------
# tools used in makefile
#-------------------------------------------------------------------------------

RM            = rm -rf
MD            ?= mkdir -p
CP            ?= cp -af
MV            ?= mv
RMDIR         ?= rm -rf

# OS detection
ifeq ($(OS),Windows_NT)
   set ENCLAVE=%1
   CC = $(CROSS_COMPILE)aarch64-none-elf-gcc.exe
   LD = $(CROSS_COMPILE)aarch64-none-elf-ld.exe
   AR = $(CROSS_COMPILE)aarch64-none-elf-ar.exe
   OBJCOPY = $(CROSS_COMPILE)\aarch64-none-elf-objcopy.exe
   OBJDUMP = $(CROSS_COMPILE)\aarch64-none-elf-objdump.exe
else
   CC = $(CROSS_COMPILE)gcc
   AR = $(CROSS_COMPILE)ar
   LD = $(CROSS_COMPILE)ld
   OBJCOPY = $(CROSS_COMPILE)objcopy
   OBJDUMP = $(CROSS_COMPILE)objdump
endif  


CFLAGS = -gdwarf-3 -W -mcmodel=large -std=c11 -DCHIP_NAME=npcm850 -D_ARBEL_Z1_ -D__ARM_ARCH_8__ -D_ARM_ -DNO_INTERNAL_IRQ_HANDLER
CFLAGS += -DEXTERNAL_INCLUDE_FILE=../$(FW_DIR)/hal_cfg.h
CFLAGS += -mcpu=Cortex-A35 -march=armv8-a -mcmodel=large -D__AARCH64__ -D__LP64__
CFLAGS += -Wno-empty-body -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -lm  -mstrict-align
CFLAGS += -fno-PIC
CFLAGS += $(OPTIONAL_FLAGS)

#
# make serial_printf compile. Select DEBUG_LOG (really detailed) DEV_LOG (lite logging) or none of them (no logging)
#
DBG = -DDEBUG_LOG
# DBG += -DDEV_LOG
CFLAGS += $(DBG)

export CFLAGS

ASFLAGS = -mcpu=Cortex-A35 -march=armv8-a -gdwarf-3
ASFLAGS += $(OPTIONAL_FLAGS)


DEPEND_FLAGS = -MD -MF $@.d
export $(ASFLAGS)


# The following environment variables are used as source tree roots:
#      RVCT30INC

#
# Suffixes
#
.SUFFIXES:
.SUFFIXES: .a .o .c .S .h


#
# Common make targets
#
Default_Make_Target: $(DEFAULT_TARGET)

all:
	make -f $(THIS_MAKEFILE) clean-all
	make -f $(THIS_MAKEFILE) clean_obj
	make -f $(THIS_MAKEFILE) arbel_a35_bootblock
	make -f $(THIS_MAKEFILE) clean_obj
	make -f $(THIS_MAKEFILE) arbel_a35_bootblock_no_tip
	make -f $(THIS_MAKEFILE) clean_obj

all_bl31:
	make -f $(THIS_MAKEFILE) clean-all
	make -f $(THIS_MAKEFILE) clean_obj
	make -f $(THIS_MAKEFILE) arbel_a35_bootblock_tfa
	make -f $(THIS_MAKEFILE) clean_obj
#
# Help
#
help:
	@echo
	@echo "usage: make -f $(THIS_MAKEFILE) <target> <vendor> <security>, where <target> is one of:"
	@echo
	@echo "    palladium, flash or arbel_a35_bootblock"
	@echo
	@echo "    all:           Builds all of the above targets"
	@echo "    clean:         Removes objects and output from all of the above targets"
	@echo "    clean-all:     Removes deliverables as well"
	@echo "    help:          This message"
	@echo


#
# Common source
#
# Sources common to targets:  Release
SourceGroup_1 = ''
export SourceGroup_1
SourceGroup_arbel_a35_bootblock_no_tip = \
			$(FW_DIR)/startup.S  \
			$(FW_DIR)/vectors.S  \
			$(FW_DIR)/MP_GIC.S  \
			$(FW_DIR)/v8_aarch64.S  \
			$(FW_DIR)/v8_utils.S  \
			$(HAL_DIR)/hal.c     \
			$(FW_DIR)/cfg.c \
			$(FW_DIR)/images.c \
			$(FW_DIR)/bootblock_boot.c \
			$(FW_DIR)/bootblock_version.c \
			$(FW_DIR)/bootblock_main.c \
			$(FW_DIR)/apps/serial_printf/serial_printf.c \
			$(FW_DIR)/mailbox.c
export SourceGroup_arbel_a35_bootblock_no_tip

SourceGroup_arbel_a35_bootblock = \
			$(FW_DIR)/startup.S  \
			$(FW_DIR)/vectors.S  \
			$(FW_DIR)/MP_GIC.S  \
			$(FW_DIR)/v8_aarch64.S  \
			$(FW_DIR)/v8_utils.S  \
			$(HAL_DIR)/hal.c     \
			$(FW_DIR)/cfg.c \
			$(FW_DIR)/bootblock_boot.c \
			$(FW_DIR)/bootblock_version.c \
			$(FW_DIR)/bootblock_main.c \
			$(FW_DIR)/apps/serial_printf/serial_printf.c \
			$(FW_DIR)/mailbox.c
export SourceGroup_arbel_a35_bootblock

###
### Begin target "Release"
###

TARGET = arbel
TargetType = a35_bootblock_$(ENCLAVE)

#
# Output directories and files
#
OUTDIR_Release = $(PROJ_DIR)/Images/$(ENCLAVE)
OUTFILE_Release = $(OUTDIR_Release)/$(TARGET)_$(TargetType).elf
Map_File =  $(OUTDIR_Release)/$(TARGET)_$(TargetType).map
BUILD_DIR ?= $(OUTDIR_Release)/objects


#
# Build
#
Release: $(OUTFILE_Release)

arbel_a35_bootblock:
	$(MAKE) -f $(THIS_MAKEFILE) Release TargetType=a35_bootblock  SourceGroup_1+="$(SourceGroup_arbel_a35_bootblock)"
arbel_a35_bootblock_no_tip:
	$(MAKE) -f $(THIS_MAKEFILE) Release TargetType=a35_bootblock_no_tip  SourceGroup_1+="$(SourceGroup_arbel_a35_bootblock_no_tip)" CFLAGS+="-D_NOTIP_ -D_BOOTBLOCK_"
arbel_a35_bootblock_tfa:
	$(MAKE) -f $(THIS_MAKEFILE) Release TargetType=a35_bootblock  SourceGroup_1+="$(SourceGroup_arbel_a35_bootblock)" ASFLAGS+="-DTFA_BL31_RESET" CFLAGS+="-DTFA_BL31_RESET -D_BOOTBLOCK_"

#
# Clean
#
clean: clean_Release
	@echo .
	@echo clean
	$(RMDIR) $(PROJ_DIR)/Images

clean-all: clean
	@echo .
	@echo clean-all
	$(RMDIR) $(OUTDIR_Release)
	$(RM) *.o
	$(RM) *.d

clean_Release:
	@echo .
	@echo clean_Release
	$(RM) -v $(BUILD_DIR)/$(FW_DIR)/*.o $(BUILD_DIR)/$(FW_DIR)/*.d

BUILD_OBJ_DIRS := $(BUILD_DIR)/$(FW_DIR) $(BUILD_DIR)/$(HAL_DIR)
clean_obj:
	@echo $(BUILD_OBJ_DIRS)
	$(RM) -v $(addsuffix /*.o,$(BUILD_OBJ_DIRS))

#
# Source files
#
SRCS_Release := $(SourceGroup_1)


#
# Object Code
#
OBJS_Release_c := $(subst .c,.o,$(filter %.c,$(SourceGroup_1)))
OBJS_Release_s := $(subst .S,.o,$(filter %.S,$(SourceGroup_1)))
OBJS_Release := $(addprefix $(BUILD_DIR)/, $(OBJS_Release_s) $(OBJS_Release_c))

#
# Dependency Files
#
DEPS_Release_c := $(subst .c,.d,$(filter %.c,$(SourceGroup_1)))
DEPS_Release_s := $(subst .S,.d,$(filter %.S,$(SourceGroup_1)))
DEPS_Release := $(DEPS_Release_s) $(DEPS_Release_c) 
-include $(DEPS_Release_s) $(DEPS_Release_c)

LINKER_FILE := $(PROJ_DIR)/$(FW_DIR)/bootblock.ld
ifeq ($(ENCLAVE),no_tip)
	LINKER_FILE := $(PROJ_DIR)/$(FW_DIR)/bootblock_NOTIP.ld
endif
LINKER_TMP = $(BUILD_DIR)/bootblock.ld

LINKER_FLAGS := --entry=start64 --gc-sections --build-id=none --print-memory-usage -Map=$(Map_File)

#
# Include search paths
#
USRINCS_Release := -I. -I$(HAL_COMMON_DIR) -I$(HAL_CHIP_DIR) -I$(SWC_DEFS_DIR) -I$(HAL_DIR)
USRINCS_Release += -I$(HAL_MODULES_DIR) -I$(HAL_HL_DRVS_DIR) -I$(HAL_CHIPS_DIR) -I$(FW_DIR)
USRINCS_Release += -I$(CROSS_COMPILER_INC) -I$(APPS_DIR)

# Define the library path
LDFLAGS := -L$(CROSS_COMPILER)/lib

#
# Output directory
#
$(OUTDIR_Release):
	$(RMDIR) $@
	$(MD) $@


#
# Build rules for linking
#
$(OUTFILE_Release): $(OUTDIR_Release) $(OBJS_Release)
	@echo .
	@echo Linking: Create elf file
	sed 's,./source,$(BUILD_DIR)/./source,g' $(LINKER_FILE) > $(LINKER_TMP)
	$(LD) $(OBJS_Release) -T$(LINKER_TMP) $(LINKER_FLAGS) $(USRINCS_Release) $(LDFLAGS) -o $(OUTFILE_Release)
	@echo .
	@echo Linking: Create bin file
	@echo $(OBJCOPY) $(ONLY_SECTIONS) -O binary $(OUTFILE_Release)  $(subst .elf,.bin,$(OUTFILE_Release)) 
	 $(OBJCOPY) $(ONLY_SECTIONS) -O binary $(OUTFILE_Release)       $(subst .elf,.bin,$(OUTFILE_Release)) 
	@echo .
	@echo Linking: Create dat file
	@echo $(OBJCOPY) $(ONLY_SECTIONS) -O verilog $(OUTFILE_Release) $(subst .elf,.dat,$(OUTFILE_Release)) 
	$(OBJCOPY) $(ONLY_SECTIONS) -O verilog $(OUTFILE_Release)       $(subst .elf,.dat,$(OUTFILE_Release)) 
	@echo .
	@echo Linking: Create asm file
	@echo $(OBJDUMP)  -S  $(OUTFILE_Release)   >                    $(subst .elf,.asm,$(OUTFILE_Release))  2>&1
	$(OBJDUMP)    -h   -S    $(OUTFILE_Release)      >              $(subst .elf,.asm,$(OUTFILE_Release))  2>&1
	@echo Linking done


#
# Build rules for .c files
#
BUILD_Release_c = $(CC) -static -ffunction-sections -fdata-sections $(USRINCS_Release) -c $(CFLAGS) -O2 -o $@ $<

$(BUILD_DIR)/%.o: %.c
	@echo .
	@$(MD) $(dir $@)
	@echo Compiling C $*.o
	$(BUILD_Release_c)



#
# Build rules for .s files
#
BUILD_Release_s = $(CC) -c $(ASFLAGS) $(DEPEND_FLAGS) -DCORTEXA $(USRINCS_Release) -O2 -o $@ $<

$(BUILD_DIR)/%.o: %.S
	@echo .
	@$(MD) $(dir $@)
	@echo Compiling S $*.o
	@echo $(BUILD_Release_s)
	@$(BUILD_Release_s)


.s.o:
	@echo .
	@echo Compiling s $*.o
	@echo $(BUILD_Release_s)
	@$(BUILD_Release_s)
	@echo Copy $*.o
	@$(MD) $(OUTDIR_Release)/objects/$(@D)
	@$(CP) $*.o $(OUTDIR_Release)/objects/$(@D)/

###
### End target "Release"
###


#
# PHONY target declarations
#    (Informs make that these targets do not produce output)
#
.PHONY: all flash clean clean-all Release clean_Release

#
# End Makefile
#
