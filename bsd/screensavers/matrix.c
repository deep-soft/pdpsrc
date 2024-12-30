/*
 * matrix.c - Simple "Matrix" screen effect for a VT100/VT220-like terminal
 *            under 2.11BSD (K&R style), with signal handling.
 *
 * Compile example (on 2.11BSD):
 *    cc -o matrix matrix.c
 *
 * Run it, then press Ctrl-C to exit (SIGINT).
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>   /* might need to replace usleep() if 2.11BSD lacks it */

/* Signal handler to restore the cursor and reset scrolling region */
void restore_on_exit(signum)
int signum;
{
    /* Show the cursor again */
    printf("\033[?25h");
    /* Reset scrolling region to the entire screen (1..24 or as needed) */
    printf("\033[1;24r");
    /* Optionally clear screen or any other cleanup */
    fflush(stdout);

    /* Exit gracefully */
    exit(0);
}

int main()
{
    int col;
    char c;

    /* Seed the random generator */
    srand(time((long *)0));

    /* Install signal handlers */
    (void) signal(SIGINT, restore_on_exit);  /* For Ctrl-C */
    (void) signal(SIGTERM, restore_on_exit); /* If you want to handle SIGTERM too */

    /* Hide the cursor */
    printf("\033[?25l");

    /* Set scrolling region to full screen (1..24).
       Adjust if your terminal has more (or fewer) lines. */
    printf("\033[1;24r");

    /* Clear screen once at the beginning (optional) */
    printf("\033[2J");

    for (;;) {
        /* Pick a random column in [0..79] */
        col = rand() % 80;

        /* Pick a random printable ASCII char from '!'..'~' (94 chars) */
        c = '!' + (rand() % 94);

        /* Move cursor to top row, (col+1) in 1-based column indexing */
        printf("\033[1;%dH", col + 1);

        /* Print the random character */
        putchar(c);

        /* Move cursor back to top-left (just to be sure) */
        printf("\033[1;1H");

        /* Issue Reverse Index to scroll entire screen down by 1 line */
        printf("\033M");

        /* Flush output so it shows up now */
        fflush(stdout);

        /* Small delay: 50ms. Adjust to taste. */
        usleep(50000);
    }

    /* Normally never reached, but just in case */
    restore_on_exit(0);

    return 0;
}
