/* Include once header guard */
#ifndef ALLOCATOR_HEADER_INCLUDED
#define ALLOCATOR_HEADER_INCLUDED

/********************************** METADATA **********************************/

/*
 * Contributors: roadelou
 * Contacts:
 * Creation Date: 2021-02-19
 * Language: C Header
 */

/********************************** INCLUDES **********************************/

// Used for the CircularList structure.
#include "circular_list.h"

/*********************************** MACROS ***********************************/

/* The macros definitions for your header go here */

/********************************** STRUCTS ***********************************/

// The structure holding the state of the allocated memory.
struct Allocator {
    struct CircularList list; // The circular list with the available segments.
    struct Segment allocated[CIRCULAR_LIST_MAX_LEN]; // The list of currently
                                                     // allocated block.
    char used[CIRCULAR_LIST_MAX_LEN]; // For each segment in the allocated
                                      // array, whether it is used or not.
};

/********************************* PROTOTYPES *********************************/

// Well, malloc, but for blocks...
block_ptr block_malloc(struct Allocator *allocator, unsigned int size);

// free, but for blocks.
void block_free(struct Allocator *allocator, block_ptr allocated);

// Defines a new allocator..
struct Allocator new_allocator(const struct Segment memory);

/* End of include once header guard */
#endif

/************************************ EOF *************************************/
