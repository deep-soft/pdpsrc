/*
 * splash.c - DEC PDP-11/83 Splash Screen for VT220 Terminal on 2.11BSD
 *            Written in K&R C style.
 *
 *  Compile example:
 *    cc -o splash splash.c
 *
 *  Run:
 *    ./splash
 *
 *  Press any key to exit.
 * 
 *  Author: [Your Name], 2024.
 *  License: GPL 2.0
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sgtty.h>
#include <sys/ioctl.h>


/* Constants */
#define SCREEN_ROWS 24
#define SCREEN_COLS 80
#define BORDER_CHAR_H '-'
#define BORDER_CHAR_V '|'
#define BORDER_CHAR_ULCORNER '+'
#define BORDER_CHAR_URCORNER '+'
#define BORDER_CHAR_LLCORNER '+'
#define BORDER_CHAR_LRCORNER '+'
#define TITLE_TEXT "DEC PDP-11/83"

/* Function Prototypes */
void cleanup();
void handle_signal();
void draw_border();
void display_title();
void set_terminal_raw();
void reset_terminal();

/* Global variable to store original terminal settings */
struct sgttyb orig_tty;

/* Cleanup function to reset terminal before exiting */
void cleanup()
{
    /* Clear screen and reset cursor */
    printf("\033[2J\033[H");

    /* Reset terminal to original settings */
    reset_terminal();

    /* Show cursor */
    printf("\033[?25h");

    fflush(stdout);
}

/* Signal handler to ensure cleanup on termination */
void handle_signal(sig)
int sig;
{
    cleanup();
    exit(0);
}

/* Set terminal to raw mode to capture key press */
void set_terminal_raw()
{
    struct sgttyb new_tty;

    /* Get current terminal settings */
    ioctl(1, TIOCGETP, &orig_tty);  /* Using file descriptor 1 for stdout */
    new_tty = orig_tty;

    /* Disable echo and enable raw mode */
    new_tty.sg_flags &= ~(ECHO | CRMOD | CBREAK);
    new_tty.sg_flags |= RAW;

    /* Apply new terminal settings */
    ioctl(1, TIOCSETP, &new_tty);
}

/* Reset terminal to original settings */
void reset_terminal()
{
    ioctl(1, TIOCSETP, &orig_tty);  /* Using file descriptor 1 for stdout */
}

/* Function to draw an 80x24 border around the screen */
void draw_border()
{
    int row, col;

    /* Move cursor to home position */
    printf("\033[H");

    /* Draw top border */
    printf("%c", BORDER_CHAR_ULCORNER);
    for (col = 1; col < SCREEN_COLS - 1; col++) {
        printf("%c", BORDER_CHAR_H);
    }
    printf("%c\n", BORDER_CHAR_URCORNER);

    /* Draw side borders */
    for (row = 2; row < SCREEN_ROWS; row++) {
        printf("%c", BORDER_CHAR_V);
        for (col = 1; col < SCREEN_COLS - 1; col++) {
            printf(" ");
        }
        printf("%c\n", BORDER_CHAR_V);
    }

    /* Draw bottom border */
    printf("%c", BORDER_CHAR_LLCORNER);
    for (col = 1; col < SCREEN_COLS - 1; col++) {
        printf("%c", BORDER_CHAR_H);
    }
    printf("%c\n", BORDER_CHAR_LRCORNER);
}

/* Function to display the title in double-height characters */
void display_title()
{
    int title_length;
    int pos_row, pos_col;

    title_length = 14; /* Length of "DEC PDP-11/83" */
    pos_row = SCREEN_ROWS / 2 - 1; /* Approximate center row */
    pos_col = (SCREEN_COLS - title_length) / 2;

    /* Move cursor to the title row */
    printf("\033[%d;%dH", pos_row, pos_col);

    /* Enable double-height characters (top half) */
    printf("\033#3");

    /* Print the title */
    printf("%s", TITLE_TEXT);

    /* Disable double-height characters (return to normal) */
    printf("\033#5");

    fflush(stdout);
}

int main()
{
    int c;

    /* Setup signal handlers */
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    /* Clear the screen and hide cursor */
    printf("\033[2J");        /* Clear entire screen */
    printf("\033[?25l");      /* Hide cursor */
    fflush(stdout);

    /* Draw the border */
    draw_border();

    /* Display the title */
    display_title();

    /* Set terminal to raw mode to capture key press */
    set_terminal_raw();

    /* Wait for any key press to exit */
    while (1) {
        c = getchar();
        if (c != EOF) {
            break;
        }
    }

    /* Cleanup and exit */
    cleanup();
    return 0;
}
