# Week 2 — Firmware Development and Implementation

**Project:** Timer-Based LED Controller with Simulated UART Commands  
**Author:** Prajakta Jadhav

## Purpose
This portable C program demonstrates timer-driven LED control, a simple controller state machine, terminal commands representing UART input, and a built-in self-test.

## Important scope note
This is a desktop simulation. The timer tick function is called by a software loop and is not a real hardware interrupt. Terminal input is not a physical UART peripheral. Do not claim physical hardware testing unless you perform it.

## Compile
```bash
gcc -std=c11 -Wall -Wextra -pedantic firmware.c -o firmware
```
Windows MinGW:
```bash
gcc -std=c11 -Wall -Wextra -pedantic firmware.c -o firmware.exe
```

## Run tests
```bash
./firmware --test
```
Windows: `firmware.exe --test`

Expected success message: `PASS: timer interval, LED state, and command tests passed.` Run the test yourself and keep the actual output as evidence.

## Interactive commands
Run `./firmware` and enter:
- `ON` — LED ON, manual mode
- `OFF` — LED OFF, manual mode
- `RUN` — resume periodic blinking
- `STATUS` — display time, LED state, and mode
- `HELP` — list commands
- `QUIT` — exit

## Key functions
- `timer_tick_isr_simulation()` increments simulated time.
- `update_led_controller()` toggles LED every 500 simulated milliseconds in RUN mode.
- `process_command()` validates and handles commands.
- `self_test()` checks timing transitions and ON/OFF/RUN behavior.

## Next hardware step
For ESP32 implementation, replace the simulation layer with the appropriate current ESP32 Arduino or ESP-IDF GPIO, timer, and UART APIs. Confirm APIs against the installed framework and test on real hardware or a suitable simulator.
