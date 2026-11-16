# npcm8xx-bootblock
Arbel NPCM8XX A35 Bootblock

## Info
The Arbel NPCM8XX requires a bootblock image that will initiate the DRAM and basic HW settings.

## Notes
The `Sources` directory and the prebuilt images in `Images` are from the
0.6.0 customer release (November 16, 2026), supporting TIP and NO_TIP builds.

## Usage
Build instructions are in `Sources\HowToBuild.txt`. Rebuilt files are written
under `Sources\Images`; package-level `Images` contains the prebuilt release.

Bootblock full image build requires the use of IGPS:
   https://github.com/Nuvoton-Israel/igps-npcm8xx

