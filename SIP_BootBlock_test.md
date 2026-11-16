# SIP BootBlock Test Procedure

## Overview

This document provides instructions for performing SIP (Self-In-Package) tests for the Arbel A35 BootBlock. It describes the process of loading test images into Flash memory.

---

## Flash Memory Layout

Flash SPI 0 CS 0 contains two key images at the following offsets:

| Offset     | Size     | Image                                   | Source Path                                                                         |
| ---------- | -------- | --------------------------------------- | ----------------------------------------------------------------------------------- |
| 0x00000000 | ~256 KB  | BootBlock SIP Image                     | `deliverables/sip/arbel_a35_bootblock_sip.bin`                                    |
| 0x01C00000 | Variable | Firmware Header (ATF, BL31, TEE, uBoot) | `deliverables/tools/Kmt_TipFwL0_Skmt_TipFwL1_BootBlock_BL31_Tee_uboot_header.bin` |

---

## Loading Images into Flash

### Standard Test Images

To perform SIP tests, load the following pre-built images into Flash SPI 0 CS 0 using DediProg on the SVB:

#### 1. BootBlock SIP Image

**Step 1:** Run the IGPS generation script:
```
\\tanap1\proj_sw\Projects\Arbel\releases\Internal\IGPS\IGPS_4.4.3_Internal_SIP_TEST\Generate_SIP.bat
```

**Step 2:** Load the generated image into Flash SPI 0 CS 0 at offset `0x00000000`:
```
\\tanap1\proj_sw\Projects\Arbel\releases\Internal\IGPS\IGPS_4.4.3_Internal_SIP_TEST\py_scripts\ImageGeneration\output_binaries\image_no_tip_SA.bin
```

**Purpose:** Main BootBlock image for SIP testing (wraps `deliverables/sip/arbel_a35_bootblock_sip.bin`)
**Size:** Approximately 256 KB

#### 2. Firmware with Header Image

**Source:** `deliverables/tools/Kmt_TipFwL0_Skmt_TipFwL1_BootBlock_BL31_Tee_uboot_header.bin`
**Destination:** Flash SPI 0 CS 0, Offset `0x01C00000`
**Purpose:** Firmware header and boot components (ATF, BL31, TEE, uBoot)
**Size:** Variable (up to 4 MB recommended)

---

## Using Custom Customer Images

If you want to use a custom customer image in place of the pre-built firmware header, you must first add a firmware header using the provided Python script.

### Header Generation Procedure

The firmware header is a 16-byte structure that precedes the image binary:

| Offset | Size (bytes) | Field     | Description                                                |
| ------ | ------------ | --------- | ---------------------------------------------------------- |
| 0x00   | 4            | File Size | Total size of the image in bytes (32-bit, little-endian)   |
| 0x04   | 4            | CRC32     | CRC32 checksum of the entire image (32-bit, little-endian) |
| 0x08   | 8            | Reserved  | Reserved for future use (set to 0x00)                      |

**Usage:**

- **File Size** — Specifies the exact number of bytes to program to the SiP flash. This allows efficient erasing (only the required sectors) and avoids writing unnecessary data.
- **CRC32 Checksum** — Calculated over the entire image (after the header). Used by the BootBlock to verify image integrity before programming and after verification read.

These fields ensure the image is correctly transferred and can be validated for integrity.

### Step 1: Prepare Your Customer Image

Ensure your custom image binary is ready (e.g., `customer_image.bin`).

### Step 2: Add Firmware Header

Use the `add_fw_header.py` script to prepend the header:

**Script Location:** `deliverables/tools/add_fw_header.py`

**Command:**

```bash
python deliverables/tools/add_fw_header.py <input_image> <output_image>
```

**Example:**

```bash
python deliverables/tools/add_fw_header.py customer_image.bin customer_image_with_header.bin
```

### Step 3: Load into Flash

After header generation, load the output image into Flash SPI 0 CS 0 at the appropriate offset:

```bash
# Load into Flash at offset 0x01C00000
flash_write customer_image_with_header.bin 0x01C00000
```

---

## Image Loading Sequence

1. **Power on** the Arbel A35 SiP device
2. **Connect to ATE** (Automatic Test Equipment) or programmer
3. **Erase Flash SPI 0 CS 0** (if required)
4. **Load the BootBlock SIP image** at offset `0x00000000`
5. **Load the Firmware Header image** (or custom image with header) at offset `0x01C00000`
6. **Verify** both images are correctly written to flash
7. **Reset** the device to begin SIP test execution

---

## Test Execution and Results

After loading the images and resetting the device, the BootBlock will automatically execute the SIP test program:

1. DDR memory training and size detection
2. SPI flash identification on the SiP internal flash (CS1)
3. Image copy from load board flash (CS0) to SiP internal flash (CS1)
4. CRC32 validation of the copied image
5. Test result reporting via GPIO[135:128] signals

### Reading Test Results

Test results are reported on **GPIO[135:128]** (active-low logic):

| GPIO | Signal            | Result         |
| ---- | ----------------- | -------------- |
| 128  | DDR Training Pass | LOW = pass     |
| 129  | DDR Size Bit 0    | LOW = bit set  |
| 130  | DDR Size Bit 1    | LOW = bit set  |
| 131  | Flash Size Bit 0  | LOW = bit set  |
| 132  | Flash Size Bit 1  | LOW = bit set  |
| 133  | Flash Size Bit 2  | LOW = bit set  |
| 134  | Quad Read Pass    | LOW = pass     |
| 135  | Test Finished     | LOW = complete |

### DDR Size Encoding

When DDR training completes, the BootBlock reports the detected DDR size using GPIO[130:129]:

| GPIO[130:129] | DDR Size | GPIO Values |
|---|----------|------------|
| 11 | Illegal  | GPIO[130]=HIGH, GPIO[129]=HIGH |
| 10 | 1 GB     | GPIO[130]=HIGH, GPIO[129]=LOW |
| 01 | 2 GB     | GPIO[130]=LOW, GPIO[129]=HIGH |
| 00 | 4 GB     | GPIO[130]=LOW, GPIO[129]=LOW |

### Flash Size Encoding

The BootBlock reports the detected SiP internal flash (CS1) size using GPIO[133:131]:

| GPIO[133:131] | Flash Size | GPIO Values |
|---|----------|------------|
| 111 | Illegal  | [133]=HIGH, [132]=HIGH, [131]=HIGH |
| 110 | 8 MB     | [133]=HIGH, [132]=HIGH, [131]=LOW |
| 101 | 16 MB    | [133]=HIGH, [132]=LOW, [131]=HIGH |
| 100 | 32 MB    | [133]=HIGH, [132]=LOW, [131]=LOW |
| 011 | 64 MB    | [133]=LOW, [132]=HIGH, [131]=HIGH |
| 010 | 128 MB   | [133]=LOW, [132]=HIGH, [131]=LOW |
| 001 | 256 MB   | [133]=LOW, [132]=LOW, [131]=HIGH |
| 000 | 512 MB   | [133]=LOW, [132]=LOW, [131]=LOW |

---

## Troubleshooting

### Image Load Verification

Verify that images were written correctly:

```bash
# Read back BootBlock SIP image (first 256 KB)
flash_read 0x00000000 0x40000 bootblock_sip_readback.bin

# Read back Firmware image (at offset 0x01C00000)
flash_read 0x01C00000 0x100000 firmware_readback.bin

# Compare with original
diff bootblock_sip_readback.bin deliverables/sip/arbel_a35_bootblock_sip.bin
```

### Custom Image Header Verification

Verify the header was correctly added to your custom image:

```bash
# Display the first 8 bytes (header)
hexdump -C customer_image_with_header.bin | head -1
# Output should show: 00000000  <file_size_bytes_0-3>  <crc32_bytes_4-7>
```

### CRC32 Mismatch Troubleshooting

If GPIO[134] (Quad Read Pass) shows FAIL:

1. Verify the header was generated correctly with `add_fw_header.py`
2. Check that the image was not corrupted during programming
3. Ensure the image was loaded at the correct offset (0x01C00000)
4. Re-generate the header and reload the image

---

## Additional Resources

- [add_fw_header.py](tools/add_fw_header.py) — Firmware header generation script
- `deliverables/sip/` — Pre-built BootBlock SIP images
- `deliverables/tools/` — Test utilities and firmware images
