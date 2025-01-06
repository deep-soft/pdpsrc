/* 
 * matrix.c - Draws a matrix-like rain of characters on the screen.
 *            then scroll the screen down (VT100 Reverse Index).
 *            Written in K&R style for 2.11BSD, etc.
 *
 *  Compile example (on 2.11BSD, might need -lm for math library):
 *    cc -o matrix matrix.c -lm
 *
 *  Press Ctrl-C to exit (SIGINT), which restores the cursor and
 *  resets the scrolling region.
 * 
 *  Author: Dave Plummer, 2024. 
 *  License: GPL 2.0
 * 
 */
// 
// set text color green
// change SCREEN_WIDTH and SCREEN_HEIGHT on screen resize
// added: initscr() and handle_winch()
// compiled and tested on raspberry pi 3
// Linux rp3 5.10.103-v7+ #1529 SMP Tue Mar 8 12:21:37 GMT 2022 armv7l GNU/Linux

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define MAX_TRAILS 16
//#define SCREEN_WIDTH 80
//#define SCREEN_HEIGHT 24
int SCREEN_WIDTH  = 80;
int SCREEN_HEIGHT = 24;

/* Structure to represent a trail */
struct Trail {
    int column;      /* Column where the trail is active */
    int rows_drawn;  /* Number of rows drawn so far */
    int length;      /* Length of the trail */
    int active;      /* Whether this trail is active */
};

struct Trail trails[MAX_TRAILS];
int trail_timer = 0;

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

void initialize_trails()
{
    int i;
    for (i = 0; i < MAX_TRAILS; i++) {
        trails[i].active = 0;
    }
}

void start_new_trail(length)
int length;
{
    int i;
    for (i = 0; i < MAX_TRAILS; i++) {
        if (!trails[i].active) {
            trails[i].column = rand() % SCREEN_WIDTH;
            trails[i].rows_drawn = 0; /* Start with no rows drawn */
            trails[i].length = length;
            trails[i].active = 1;
            break;
        }
    }
}

void update_trails()
{
    int i;
    char c;

    for (i = 0; i < MAX_TRAILS; i++) {
        if (trails[i].active) {
            /* Draw only in the first row if the trail is active */
            if (trails[i].rows_drawn < trails[i].length) {
                printf("\033[1;%dH", trails[i].column + 1);
                c = '!' + (rand() % 94);
                putchar(c);

                /* Increment the number of rows drawn */
                trails[i].rows_drawn++;

                /* Deactivate and recycle the trail if fully drawn */
                if (trails[i].rows_drawn >= trails[i].length) {
                    trails[i].active = 0;
                }
            }
        }
    }

    /* Scroll the screen down by one row */
    printf("\033M");
}

int initscr()
{
#include <sys/ioctl.h>
#include <unistd.h>

    struct winsize w;
//    ioctl(0, TIOCGWINSZ, &w);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    SCREEN_WIDTH  = w.ws_col;
    SCREEN_HEIGHT = w.ws_row;

    printf ("%dx%d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
    usleep(500000);

    return 0;
}

// SIGWINCH is called when the window is resized.
void handle_winch(int sig){
  signal(SIGWINCH, SIG_IGN);
  initscr();
  signal(SIGWINCH, handle_winch);
}

int main()
{
//# IGreen='\033[0;92m'       # Green
    printf("\033[0;92m");
    initscr();
    signal(SIGWINCH, handle_winch);

    int trail_length = 5; /* Configurable length of the trail */
    int spawn_rate = 1;   /* Configurable spawn rate */

    /* Seed the random generator */
    srand(time((long *)0));

    /* Install signal handlers */
    signal(SIGINT, restore_on_exit);
    signal(SIGTERM, restore_on_exit);

    /* Hide the cursor */
    printf("\033[?25l");

    /* Set scrolling region to full screen */
    printf("\033[1;24r");

    /* Clear screen */
    printf("\033[2J");

    /* Initialize trails */
    initialize_trails();

    for (;;) {
        /* Start a new trail periodically */
        if (trail_timer % spawn_rate == 0) {
            start_new_trail(trail_length);
        }

        /* Update and draw trails */
        update_trails();

        /* Reset cursor to top-left (optional) */
        printf("\033[1;1H");

        /* Flush output */
        fflush(stdout);

        /* Small delay */
        usleep(50000);

        trail_timer++;
    }

    /* Normally never reached */
    restore_on_exit(0);

//# Color_Off='\033[0m'       # Text Reset
    printf("\033[0m");

    return 0;
}
