.globl  _main
        .globl  _puts
        .globl  _putchar
        .globl  _exit
        .globl  _gettimeofday

/---------------------------------------------------------------------
/ Constants (no .eqy; just define them this way)
/
LIMIT = 1000.
BSIZE = 1000.

/---------------------------------------------------------------------
/ Data Section
/---------------------------------------------------------------------
        .data

HELLOMSG:
	<Sieve of Eratosthenes by Davepl 2024>
        .byte   0

DASHESMSG:
	<---------------------------------->
        .byte   0

CBITSMSG:
	<Clearing byte array.  Sieve Size:>
        .byte   0

MSG2:
	<Setup complete>
        .byte   0

RUNMSG:
	<Running sieve>
        .byte   0

DONEMSG:
	<Sieve complete>
        .byte   0

PRIMMSG:
	<Prime numbers found:>
        .byte   0

MARKCMPMSG:
	<Marking composite: >
        .byte   0

CHECKCMPMSG:
	<Checking if composite: >
        .byte   0

EMPTYMSG:
	<>
        .byte   0

/ Optionally, space for gettimeofday()
tv_start:
        .byte   0,0,0,0,0,0,0,0
tv_end:
        .byte   0,0,0,0,0,0,0,0

/---------------------------------------------------------------------
/ BSS section for the sieve array
/---------------------------------------------------------------------
        .bss
BYTEARR:
        .=.+BSIZE               / Reserve 1000 bytes

/---------------------------------------------------------------------
/ Text section for code
/---------------------------------------------------------------------
        .text

/---------------------------------------------------------------------
/ _main
/---------------------------------------------------------------------
_main:
        /-------------------------------------------------------------
        / Optional timing: gettimeofday(&tv_start, NULL)
        /-------------------------------------------------------------
        mov     $tv_start, -(sp)
        mov     $0, -(sp)
        jsr     pc, _gettimeofday
        add     $4., sp

        /-------------------------------------------------------------
        / Print introduction
        /-------------------------------------------------------------
        mov     $HELLOMSG, -(sp)
        jsr     pc, _puts
        add     $2., sp

        mov     $DASHESMSG, -(sp)
        jsr     pc, _puts
        add     $2., sp

        mov     $CBITSMSG, -(sp)
        jsr     pc, _puts
        add     $2., sp

        / Print the limit as a number
        mov     $LIMIT, -(sp)
        jsr     pc, PRNUM
        add     $2., sp

        / Blank line
        mov     $EMPTYMSG, -(sp)
        jsr     pc, _puts
        add     $2., sp

        /-------------------------------------------------------------
        / Clear the byte array - using r3/r4 to avoid r0,r1,r2
        /-------------------------------------------------------------
        mov     $BSIZE, r3
        clr     r4
clb:
        movb    $0, BYTEARR(r4)    / Actually clear the byte to 0
        inc     r4
        cmp     r4, r3
        bne     clb

        /-------------------------------------------------------------
        / Print messages
        /-------------------------------------------------------------
        mov     $MSG2, -(sp)
        jsr     pc, _puts
        add     $2., sp

        mov     $RUNMSG, -(sp)
        jsr     pc, _puts
        add     $2., sp

        /-------------------------------------------------------------
        / Run the sieve - using r3/r4 to avoid conflict with ISCOMP
        /-------------------------------------------------------------
        mov     $3., r3          / Main counter in r3
SIEVE_LOOP:
        cmp     r3, $LIMIT
        bge     DONE_SIEVE

        / Push candidate for ISCOMP
        mov     r3, -(sp)
        jsr     pc, ISCOMP
        add     $2., sp
        tst     r0
        bne     NEXT_ODD     / if r0==1 => composite => skip marking

        / Mark multiples using r3/r4
        / Mark multiples using r3/r4
        mov     r3, r4          / First multiple will be r3 + r3
        add     r3, r4          / Adjust r4 to start at r3 + r3
MARKLOOP:
        mov     r4, -(sp)       / Save current multiple
        jsr     pc, SETCMP
        add     $2., sp
        add     r3, r4          / Add prime to get next multiple
        cmp     r4, $LIMIT     / Check if we've exceeded limit
        blt     MARKLOOP       / If not, continue marking
    / If not, continue marking

NEXT_ODD:
        add     $2., r3
        br      SIEVE_LOOP

/---------------------------------------------------------------------
DONE_SIEVE:
        mov     $DONEMSG, -(sp)
        jsr     pc, _puts
        add     $2., sp

        mov     $PRIMMSG, -(sp)
        jsr     pc, _puts
        add     $2., sp

        /-------------------------------------------------------------
        / Print primes - using r3 for candidate, r5 for counter
        /-------------------------------------------------------------
        mov     $1., r5          / prime count in r5

        / Print 2
        mov     $2., -(sp)
        jsr     pc, PRNUM
        add     $2., sp

        / Print a comma
        mov     $',, -(sp)
        jsr     pc, _putchar
        add     $2., sp

        / Now print odd primes from 3..LIMIT
        mov     $3., r3          / Current prime candidate in r3
PRIME_PRINT_LOOP:
        cmp     r3, $LIMIT
        bge     DONE_PRINT

        mov     r3, -(sp)
        jsr     pc, ISCOMP
        add     $2., sp
        tst     r0
        bne     SKIP_PRN

        / Print the prime
        mov     r3, -(sp)
        jsr     pc, PRNUM
        add     $2., sp

        inc     r5               / Increment count in r5
        mov     $',, -(sp)
        jsr     pc, _putchar
        add     $2., sp

SKIP_PRN:
        add     $2., r3
        br      PRIME_PRINT_LOOP

DONE_PRINT:
        / Print blank line
        mov     $EMPTYMSG, -(sp)
        jsr     pc, _puts
        add     $2., sp

        / Print the number of primes found (from r5)
        mov     r5, -(sp)
        jsr     pc, PRNUM
        add     $2., sp

        /-------------------------------------------------------------
        / Optional timing: gettimeofday(&tv_end, NULL)
        /-------------------------------------------------------------
        mov     $tv_end, -(sp)
        mov     $0, -(sp)
        jsr     pc, _gettimeofday
        add     $4., sp

        / ...compute elapsed if desired...

        / Finally exit
        jsr     pc, _exit

/---------------------------------------------------------------------
/ ISCOMP(arg on stack):
/   Returns r0=1 if "arg" is composite, else 0 if prime
/   Uses: r0, r2
/---------------------------------------------------------------------
ISCOMP:
        mov     2(sp), r0       / Get argument from stack
        movb    BYTEARR(r0), r0 / Check the byte into r0 directly
        rts     pc

SETCMP:
        mov     2(sp), r0       / Get argument from stack
        movb    $1, BYTEARR(r0) / Mark as composite
        rts     pc


/---------------------------------------------------------------------
/ PRNUM(arg on stack):
/   Prints a 16-bit unsigned integer (decimal)
/   Arg: 16-bit integer on stack
/   Returns: None (prints to stdout)
/   Preserves: All caller-saved registers
/---------------------------------------------------------------------
PRNUM:
        mov     r2, -(sp)        / Save r2
        mov     r1, -(sp)        / Save r1 
        mov     r0, -(sp)        / Save r0

        mov     8(sp), r1        / Get number to print in r1
        
        / Divide r1 by 10
        mov     r1, r2           / Save original number
        clr     r0               / Clear high word
        mov     r2, r1           / Put number in low word
        div     $10., r0         / Now divides r0:r1 by 10
                                / r0 = quotient, r1 = remainder

        tst     r0               / Check if quotient is 0
        beq     1f              / If zero, skip recursion

        / Handle quotient recursively
        mov     r1, r2           / Save remainder in r2
        mov     r0, -(sp)        / Push quotient
        jsr     pc, PRNUM        / Recursive call
        add     $2., sp          / Clean stack
        mov     r2, r1           / Restore remainder to r1

1:      / Print current digit
        add     $'0, r1          / Convert to ASCII
        mov     r1, -(sp)        / Push char to print
        jsr     pc, _putchar     / Print it
        add     $2., sp          / Clean stack

        / Restore registers in reverse order
        mov     (sp)+, r0
        mov     (sp)+, r1
        mov     (sp)+, r2
        rts     pc