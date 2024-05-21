#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
# Copyright (c) 2024 by Nuvoton Technology Corporation
# All rights reserved

# File Contents:
#   build.sh
#   Shell script for building code on Linux

# Usage:
# CC_PATH=xxx build.sh [ENCLAVE]
# ex: CC_PATH=/home/brian/bin/arm-gnu-toolchain build.sh tip
set -e

TARGET="arbel"
BOOTBLOCK="arbel_a35_bootblock"

# ENCLAVE can be tip \ no_tip.  no_tip shoud be used for users who have an external RoT.
ENCLAVE=${1:-"no_tip"}

find ./ -type f -name "*.o" -exec rm -vf {} +
find ./ -type f -name "*.o.d" -exec rm -vf {} +
find ./ -type f -name "*.d" -exec rm -vf {} +

rm -rf ./Images


# Create the output directory if ENCLAVE is not empty
if [ -n "$ENCLAVE" ]; then
    DELIV_DIR="./Images/$ENCLAVE"
    mkdir -p "$DELIV_DIR"
fi
# now we only support tip or no_tip
if [ "$ENCLAVE" != "tip" ]; then
    BOOTBLOCK="${BOOTBLOCK}_no_tip"
    ENCLAVE="no_tip"
fi


echo "========================================"
echo "= build: Building $TARGET $ENCLAVE"
echo "========================================"

# Preserve the original PATH
PATH_BACKUP="$PATH"
CROSS_COMPILER_PATH=${CC_PATH:-"/work_sw/tperry/tools/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf"}
CROSS_COMPILER_BIN="${CROSS_COMPILER_PATH}/bin"
CROSS_COMPILER_LIB="${CROSS_COMPILER_PATH}/lib"
MAKE_PATH="/usr/bin/"

if [ ! -d "$CROSS_COMPILER_BIN" ] || [ ! -d "$MAKE_PATH" ]; then
    echo "Required tools not found. Make sure the paths are correct."
    exit 1
fi

echo "=============================="
echo "=== Using GNU tools locally ==="
echo "=============================="

# Prepend paths to the PATH environment variable
export PATH="$CROSS_COMPILER_BIN:$CROSS_COMPILER_LIB:$MAKE_PATH:$PATH"

echo "========================================"
echo "= Building Arbel $TARGET $ENCLAVE code"
echo "========================================"

make "$BOOTBLOCK" ENCLAVE="$ENCLAVE" CROSS_COMPILE="${CROSS_COMPILER_BIN}/aarch64-none-elf-"

echo "Compile done"

# Make sure outputs are ready
if [ ! -e "${DELIV_DIR}/${BOOTBLOCK}.bin" ]; then
    exit 1
fi

# If we reach this point, the build was successful
echo "======================================="
echo "build.sh: Arbel $TARGET $ENCLAVE build ended successfully"
echo "======================================="

# Restore the original PATH
export PATH="$PATH_BACKUP"

exit 0
