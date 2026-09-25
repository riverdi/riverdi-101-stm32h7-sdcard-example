# SD Card Write Example — Riverdi 10.1" (STM32H747)

This is a test/demo application showing SD card write functionality on the Riverdi 10" display module (STM32H747-based).

## What it does

A colored square in the corner of the screen indicates the SD card status:

| Color | Meaning |
|-------|---------|
| Gray  | No SD card detected |
| Green | Card detected and TEST.TXT written successfully |

When an SD card is present at boot (or inserted while the indicator is gray), the firmware:

1. Waits 500 ms for the card to settle
2. Mounts the FAT32 filesystem
3. Writes `TEST.TXT` to the root of the card
4. Unmounts the filesystem and shows green

## TEST.TXT contents

```
SD card write test - OK
To verify the write works: delete this file and power-cycle the board.
If the file reappears, SD card write is working correctly.
```

The file creation date visible in Windows Explorer (file Properties) reflects the RTC time at the moment of writing. The RTC is set once on the first boot from the build-time macros (`__DATE__` / `__TIME__`) and stored in a backup register. If a backup battery is present, the timestamp advances across resets.

## How to re-trigger the write

This is a one-shot demo — one write per power cycle:

- **Card in before/during boot:** the module writes immediately after startup and shows green.
- **Card inserted after boot:** the module detects it and writes once, then shows green.
- **Removing and reinserting the card** does not trigger a new write — the indicator stays gray.

**To write again:** insert the card and reset (or power-cycle) the module. To verify: delete `TEST.TXT` from the card on a PC, insert the card, reset the module, and confirm the file reappears.

## Technical notes

- MCU: STM32H747 (CM7 core)
- SD interface: SDMMC1, 4-bit bus, ClockDiv = 4
- Card detect pin: PE3, active HIGH (card present = HIGH)
- FatFS + FreeRTOS CMSIS-RTOS v2
- TouchGFX MVP: `Model` polls `sd_write_status` every tick → `Presenter` → `View` updates box color
- QSPI external flash loader required for programming: `QSPI_STM32H747I-Riverdi-MICRON_V2.stldr`
