/*
 * chartest.c - Test VT220 down-line loadable characters
 */

#include <stdio.h>
#include <unistd.h>

void twrite(char *s)
{
    write(1, s, strlen(s));
}

/* Test with just one simple character - a checkerboard pattern */
void define_test_char()
{
    /* DCS sequence to define character at position '!' */
    twrite("\033P1;1;1{");  /* Font 1, starting at position 2/1 */
    twrite("41;1;6;10;101010101010/"); /* Character at '!' position (41) */
    twrite("\033\\");  /* ST - String Terminator */
}

int main()
{
    /* Clear screen */
    twrite("\033[2J");

    /* Define our test character */
    define_test_char();

    /* Select the soft character set */
    twrite("\033)1");

    /* Move to center of screen and display our character */
    twrite("\033[12;40H");  /* Move to row 12, column 40 */
    write(1, "!", 1);       /* Display the test character */

    /* Display a normal character next to it for comparison */
    write(1, "X", 1);

    /* Move down and add description */
    twrite("\033[14;35H");
    write(1, "Custom char vs normal char", 25);

    return 0;
}