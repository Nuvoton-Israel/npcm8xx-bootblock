#!/usr/bin/env python3
"""
add_fw_header.py - Add CRC32 header to firmware image with sector alignment

This script prepends a 16-byte header to a firmware binary:
  Bytes 0-3:   Image size (UINT32, little-endian) - padded size aligned to 4KB boundary
  Bytes 4-7:   CRC32 checksum (UINT32, little-endian) - CRC32 of padded firmware data
  Bytes 8-15:  Reserved/padding (8 bytes of zeros)

The firmware is padded with 0xFF bytes to 4KB boundary before CRC calculation.
The CRC32 uses polynomial 0xEDB88320 (reflected), matching crctool and Ethernet/ZIP standards.

Usage:
    python add_fw_header.py <input_binary> <output_binary>

Example:
    python add_fw_header.py bootblock.bin bootblock_with_header.bin
"""

import sys
import struct


SECTOR_SIZE = 4096  # 4KB sector size


def crc32_reflected(data):
    """
    Calculate CRC32 with reflected polynomial 0xEDB88320.
    Matches crctool's implementation and standard CRC32 (Ethernet, ZIP).
    
    Args:
        data: bytes to calculate CRC32 over
        
    Returns:
        CRC32 value as integer
    """
    polynomial = 0xEDB88320
    crc_table = [0] * 256
    
    # Build CRC lookup table
    for i in range(256):
        crc = i
        for _ in range(8):
            if crc & 0x00000001:
                crc = (crc >> 1) ^ polynomial
            else:
                crc = crc >> 1
        crc_table[i] = crc
    
    # Calculate CRC32
    crc = 0xffffffff
    for byte in data:
        tmp = crc ^ byte
        crc = (crc >> 8) ^ crc_table[tmp & 0xff]
    
    return crc ^ 0xffffffff


def add_fw_header(input_file, output_file):
    """
    Read firmware binary, pad to sector boundary with 0xFF, calculate CRC32,
    and prepend 16-byte header.
    
    Args:
        input_file: Path to input firmware binary
        output_file: Path to output firmware binary with header
        
    Returns:
        True on success, False on error
    """
    try:
        # Read firmware binary
        with open(input_file, 'rb') as f:
            fw_data = f.read()
        
        fw_size = len(fw_data)
        print(f"Firmware size: {fw_size} bytes (0x{fw_size:x})")
        
        # Calculate aligned size (round up to sector boundary)
        fw_size_aligned = (fw_size + SECTOR_SIZE - 1) & ~(SECTOR_SIZE - 1)
        padding_size = fw_size_aligned - fw_size
        print(f"Aligned size: {fw_size_aligned} bytes (0x{fw_size_aligned:x}) - padding: {padding_size} bytes")
        
        # Pad with 0xFF to sector boundary
        fw_padded = fw_data + (b'\xFF' * padding_size)
        
        # Calculate CRC32 of padded data
        crc32_value = crc32_reflected(fw_padded)
        print(f"CRC32 (padded): 0x{crc32_value:08x}")
        
        # Create header: [size (4 bytes, LE)] [crc32 (4 bytes, LE)] [reserved (8 bytes)]
        header = struct.pack('<II', fw_size_aligned, crc32_value) + b'\x00' * 8
        print(f"Header: {header.hex()}")
        
        # Write output: header + padded firmware
        with open(output_file, 'wb') as f:
            f.write(header)
            f.write(fw_padded)
        
        output_size = fw_size_aligned + 16
        print(f"Output size: {output_size} bytes (0x{output_size:x})")
        print(f"Successfully wrote: {output_file}")
        return True
        
    except FileNotFoundError as e:
        print(f"Error: {e}", file=sys.stderr)
        return False
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        return False


def main():
    if len(sys.argv) != 3:
        print("Usage: python add_fw_header.py <input_binary> <output_binary>")
        print("       Adds 8-byte header (size + CRC32) and pads firmware to 4KB boundary")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    if add_fw_header(input_file, output_file):
        sys.exit(0)
    else:
        sys.exit(1)


if __name__ == '__main__':
    main()

