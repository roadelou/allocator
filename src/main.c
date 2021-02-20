/********************************** METADATA **********************************/

/*
 * Contributors: roadelou
 * Contacts:
 * Creation Date: 2021-02-19
 * Language: C Source
 */

/********************************** INCLUDES **********************************/

// The code we want to test.
#include "allocator.h"

// Used for printf.
#include <stdio.h>

/********************************* PROTOYPES **********************************/

// Prints debug information on the Segment.
int debug_segment(const struct Segment segment);

// Prints debug information on the CircularList.
int debug_list(struct CircularList *list);

// Prints debug information on the Allocator.
int debug_allocator(struct Allocator *allocator);

/************************************ MAIN ************************************/

int main(int argc, char **argv) {
    // We create a segment for the whole memory.
    struct Segment whole_memory = {.start = 0, .length = 512};
    // We create an allocator for this memory.
    struct Allocator allocator = new_allocator(whole_memory);
    debug_allocator(&allocator);

    // We try some allocation and deallocation to see if the code works.
    block_ptr blocks_10 = block_malloc(&allocator, 10);
    // Printing the allocated address.
    debug_allocator(&allocator);

    // We try a new allocation.
    block_ptr blocks_20 = block_malloc(&allocator, 20);
    debug_allocator(&allocator);

    // Deallocating the first buffer.
    block_free(&allocator, blocks_10);
    debug_allocator(&allocator);

    // Allocating 3 buffers of 5 blocks.
    block_ptr first_blocks_5 = block_malloc(&allocator, 5);
    block_ptr second_blocks_5 = block_malloc(&allocator, 5);
    block_ptr third_blocks_5 = block_malloc(&allocator, 5);
    // Printing the three pointers.
    debug_allocator(&allocator);

    // Freing the big pointer.
    block_free(&allocator, blocks_20);
    debug_allocator(&allocator);

    // Freeing all the pointers.
    block_free(&allocator, first_blocks_5);
    block_free(&allocator, second_blocks_5);
    block_free(&allocator, third_blocks_5);
    // Printing the three pointers.
    debug_allocator(&allocator);

    return 0;
}

/********************************* FUNCTIONS **********************************/

// Prints debug information on the Segment.
int debug_segment(const struct Segment segment) {
    return printf("{start: %d, length %d}\n", segment.start, segment.length);
}

// Prints debug information on the CircularList.
int debug_list(struct CircularList *list) {
    printf("List length: %d\n", list->length);
    printf("List Links:\n");
    struct CircularLink *link = get_head(list);
    for (unsigned int i = 0; i < list->length; i++) {
        printf("Segment %d:\t", i);
        debug_segment(link->segment);
        link = next_link(list, link);
    }
    return 0;
}

// Prints debug information on the Allocator.
int debug_allocator(struct Allocator *allocator) {
    puts("");
    puts("Allocator information\n---------------------\n");
    debug_list(&(allocator->list));
    puts("\n");
    // Printing infor on the allocated memory.
    for (unsigned int i = 0; i < CIRCULAR_LIST_MAX_LEN; i++) {
        printf("Pointer %d: ", i);
        if (allocator->used[i]) {
            debug_segment(allocator->allocated[i]);
        } else {
            puts("<unused>");
        }
    }
    puts("");
    return 0;
}

/************************************ EOF *************************************/
