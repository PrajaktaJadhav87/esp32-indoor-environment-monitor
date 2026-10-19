/*
 WEEK 2: Firmware Development and Implementation
 Project: Timer-Based LED Controller with Simulated UART Commands
 Author: Prajakta Jadhav

 README
 Purpose: Portable C simulation of a timer-driven LED controller and UART-style
 terminal commands. This is host-side simulation, NOT a real hardware ISR/UART.

 Features:
 - Simulated 1 ms timer tick; LED toggles every 500 ms in RUN mode.
 - Commands: ON, OFF, RUN, STATUS, HELP, QUIT.
 - Automated test mode: firmware --test

 Compile (GCC):
   gcc -std=c11 -Wall -Wextra -pedantic firmware.c -o firmware
 Run:
   ./firmware --test
   ./firmware
 Windows MinGW:
   gcc -std=c11 -Wall -Wextra -pedantic firmware.c -o firmware.exe
   firmware.exe --test

 For actual ESP32 hardware, replace the simulation layer with verified ESP32
 GPIO/timer/UART APIs and test on hardware. Do not claim hardware testing unless
 performed.
*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BLINK_INTERVAL_MS 500U

typedef enum { MODE_RUN = 0, MODE_MANUAL } controller_mode_t;

typedef struct {
    volatile uint32_t tick_ms;
    uint32_t last_toggle_ms;
    bool led_on;
    controller_mode_t mode;
} firmware_state_t;

static firmware_state_t state = {0U, 0U, false, MODE_RUN};

/* Simulates a 1 ms timer ISR; it is not a real hardware interrupt. */
static void timer_tick_isr_simulation(void) { state.tick_ms++; }

/* Application logic: only blink when the controller is in RUN mode. */
static void update_led_controller(void)
{
    if (state.mode != MODE_RUN) return;
    if ((uint32_t)(state.tick_ms - state.last_toggle_ms) >= BLINK_INTERVAL_MS) {
        state.led_on = !state.led_on;
        state.last_toggle_ms = state.tick_ms;
        printf("[TIME %u ms] LED %s\n", (unsigned)state.tick_ms,
               state.led_on ? "ON" : "OFF");
    }
}

static void simulate_ticks(uint32_t count)
{
    for (uint32_t i = 0; i < count; i++) {
        timer_tick_isr_simulation();
        update_led_controller();
    }
}

static void print_status(void)
{
    printf("STATUS | time=%u ms | LED=%s | mode=%s\n",
           (unsigned)state.tick_ms, state.led_on ? "ON" : "OFF",
           state.mode == MODE_RUN ? "RUN" : "MANUAL");
}

static void help(void)
{
    puts("Commands: ON, OFF, RUN, STATUS, HELP, QUIT");
    puts("ON/OFF set a fixed LED state; RUN resumes periodic blinking.");
}

static void normalize(char *s)
{
    size_t len;
    char *start = s;
    while (isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1U);
    len = strlen(s);
    while (len > 0U && isspace((unsigned char)s[len - 1U])) s[--len] = '\0';
    for (size_t i = 0; s[i] != '\0'; i++) s[i] = (char)toupper((unsigned char)s[i]);
}

/* Terminal input stands in for commands received through a UART terminal. */
static bool process_command(char *cmd)
{
    normalize(cmd);
    if (strcmp(cmd, "ON") == 0) {
        state.mode = MODE_MANUAL; state.led_on = true;
        puts("Command accepted: LED ON (manual mode).");
    } else if (strcmp(cmd, "OFF") == 0) {
        state.mode = MODE_MANUAL; state.led_on = false;
        puts("Command accepted: LED OFF (manual mode).");
    } else if (strcmp(cmd, "RUN") == 0) {
        state.mode = MODE_RUN; state.last_toggle_ms = state.tick_ms;
        puts("Command accepted: periodic blinking resumed.");
    } else if (strcmp(cmd, "STATUS") == 0) {
        print_status();
    } else if (strcmp(cmd, "HELP") == 0) {
        help();
    } else if (strcmp(cmd, "QUIT") == 0) {
        return false;
    } else {
        printf("Unknown command '%s'. Type HELP.\n", cmd);
    }
    return true;
}

static int self_test(void)
{
    char cmd[32];
    puts("=== Firmware simulation self-test ===");
    state = (firmware_state_t){0U, 0U, false, MODE_RUN};
    simulate_ticks(500U);
    if (!state.led_on) { puts("FAIL: LED should be ON at 500 ms."); return 1; }
    simulate_ticks(500U);
    if (state.led_on) { puts("FAIL: LED should be OFF at 1000 ms."); return 1; }
    strcpy(cmd, "ON"); (void)process_command(cmd);
    if (!state.led_on || state.mode != MODE_MANUAL) { puts("FAIL: ON."); return 1; }
    strcpy(cmd, "OFF"); (void)process_command(cmd);
    if (state.led_on || state.mode != MODE_MANUAL) { puts("FAIL: OFF."); return 1; }
    strcpy(cmd, "RUN"); (void)process_command(cmd);
    if (state.mode != MODE_RUN) { puts("FAIL: RUN."); return 1; }
    puts("PASS: timer interval, LED state, and command tests passed.");
    print_status();
    return 0;
}

int main(int argc, char **argv)
{
    char cmd[64];
    if (argc > 1 && strcmp(argv[1], "--test") == 0) return self_test();

    puts("Timer-Based LED Controller (desktop simulation)");
    help();
    puts("Time advances by 100 simulated ms after each command.");
    for (;;) {
        printf("> ");
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) break;
        if (!process_command(cmd)) { puts("Exiting."); break; }
        simulate_ticks(100U);
        print_status();
    }
    return 0;
}
