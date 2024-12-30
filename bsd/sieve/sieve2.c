/* Paged Sieve of Eratosthenes

   by Dave Plummer 12/28/2024
   for the PDP-11 running 211-BSD

   This program calculates prime numbers using the Sieve of Eratosthenes
   algorithm, managing memory with paged overlays. The sieve is limited
   only by the backing storage available for swapping.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LIMIT 1000L
#define BITSPERBYTE 8
#define PAGE_SIZE 8192  // 8KB per page (standard PDP-11 MMU page size)

/* Macros for bit manipulation */
#define GET_BIT(array, n) ((array[(n) / BITSPERBYTE] >> ((n) % BITSPERBYTE)) & 1)
#define SET_BIT(array, n) (array[(n) / BITSPERBYTE] |= (1 << ((n) % BITSPERBYTE)))

char *sieve_backing_store;  // Full sieve in backing memory
size_t total_size;          // Total size of the sieve in bytes

char overlay_buffer[PAGE_SIZE];  // Overlay buffer (8KB page size)

/* Function to load a page from backing store into overlay buffer */
void load_page(size_t page_number) {
    size_t offset = page_number * PAGE_SIZE;
    memset(overlay_buffer, 0, PAGE_SIZE);  // Clear the overlay buffer
    memcpy(overlay_buffer, sieve_backing_store + offset, PAGE_SIZE);
}

/* Function to store a page from overlay buffer back to backing store */
void store_page(size_t page_number) {
    size_t offset = page_number * PAGE_SIZE;
    memcpy(sieve_backing_store + offset, overlay_buffer, PAGE_SIZE);
}

/* Sieve of Eratosthenes with paged memory management */
void paged_sieve_of_eratosthenes(long limit, long *count_ptr) {
    long i, j, count = 1;  // Count starts at 1 because "2" is prime
    size_t num_pages = (total_size + PAGE_SIZE - 1) / PAGE_SIZE;

    for (i = 3; i * i <= limit; i += 2) {
        size_t page_number = (i / 2) / PAGE_SIZE;
        load_page(page_number);  // Load the page containing "i"

        if (!GET_BIT(overlay_buffer, (i / 2) % (PAGE_SIZE * BITSPERBYTE))) {
            for (j = i * i; j <= limit; j += 2 * i) {
                size_t bit_index = j / 2;
                size_t target_page = bit_index / (PAGE_SIZE * BITSPERBYTE);

                load_page(target_page);  // Load the page for "j"
                SET_BIT(overlay_buffer, bit_index % (PAGE_SIZE * BITSPERBYTE));
                store_page(target_page);  // Save the modified page
            }
        }

        store_page(page_number);  // Save the modified page for "i"
    }

    // Count primes in the entire sieve
    for (i = 3; i <= limit; i += 2) {
        size_t bit_index = i / 2;
        size_t target_page = bit_index / (PAGE_SIZE * BITSPERBYTE);

        load_page(target_page);  // Load the page for "i"
        if (!GET_BIT(overlay_buffer, bit_index % (PAGE_SIZE * BITSPERBYTE))) {
            count++;
        }
    }

    *count_ptr = count;
}

int main(int argc, char *argv[]) {
    long limit = DEFAULT_LIMIT;
    long prime_count = 0;

    printf("Paged Sieve of Eratosthenes\n");
    printf("------------------------------------\n");

    printf("Enter the upper limit for prime calculation: ");
    scanf("%ld", &limit);

    total_size = (limit / 2) / BITSPERBYTE + 1;
    sieve_backing_store = malloc(total_size);
    if (!sieve_backing_store) {
        perror("Failed to allocate backing store");
        return 1;
    }
    memset(sieve_backing_store, 0, total_size);

    paged_sieve_of_eratosthenes(limit, &prime_count);

    printf("Count of primes found: %ld\n", prime_count);

    free(sieve_backing_store);
    return 0;
}

