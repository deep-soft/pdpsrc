/*
 * matrix_sine.c - Draw a '*' in a sine-wave pattern at the top row,
 *                 then scroll the screen down (VT100 Reverse Index).
 *                 Written in K&R style for 2.11BSD, etc.
 *
 *  Compile example (on 2.11BSD, might need -lm for math library):
 *    cc -o matrix_sine matrix_sine.c -lm
 *
 *  Press Ctrl-C to exit (SIGINT), which restores the cursor and
 *  resets the scrolling region.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <unistd.h>  /* Might need to replace usleep() if unavailable */

static void restore_on_exit();

int main()
{
    double a = 0.0;  /* Angle for sine wave */
    int col;

    /* Install signal handler for Ctrl-C (SIGINT) and SIGTERM. */
    (void) signal(SIGINT,  restore_on_exit);
    (void) signal(SIGTERM, restore_on_exit);

    /* Hide the cursor */
    printf("\033[?25l");

    /* Set scrolling region to full screen (1..24).
       Adjust if your terminal has more (or fewer) lines. */
    printf("\033[1;24r");

    /* Clear screen once at the beginning (optional) */
    printf("\033[2J");

    fflush(stdout);

    /* Main loop: increment 'a' and place '*' at sine-based columns */
    while (1) {
        /* Compute column: around screen center (40), plus amplitude (35 * sin a).
           Make sure it's within 1..80 for 1-based column indexing. */
        col = 40 + (int)(35.0 * sin(a));
        if (col < 1)   col = 1;
        if (col > 80)  col = 80;

        /* Move cursor to top row, (col) in 1-based indexing */
        printf("\033[1;%dH", col);

        /* Print the '*' */
        putchar('*');

        /* Move cursor back to top-left (just to be sure) */
        printf("\033[1;1H");

        /* Issue Reverse Index to scroll entire screen down by 1 line */
        printf("\033M");

        /* Flush output so it shows up now */
        fflush(stdout);

        /* Increment angle by a small step for the wave motion */
        a += 0.2;

        /* Small delay: 50ms. Adjust to taste. */
        usleep(50000);
    }

    /* Normally never reached, but just in case */
    restore_on_exit(0);

    return 0;
}

/* Signal handler to restore the terminal when exiting */
static void restore_on_exit(signum)
int signum;
{
    /* Show the cursor again */
    printf("\033[?25h");

    /* Reset scrolling region (assuming 24 lines) */
    printf("\033[1;24r");

    /* Flush out any pending output */
    fflush(stdout);

    /* Exit gracefully */
    exit(0);
}
