/* Include once header guard */
#ifndef CIRCULAR_LIST_INCLUDED
#define CIRCULAR_LIST_INCLUDED

/********************************** METADATA **********************************/

/*
 * Contributors: roadelou
 * Contacts:
 * Creation Date: 2021-02-19
 * Language: C Header
 */

/********************************** INCLUDES **********************************/

// Holds the definition of a Segment and a block_ptr.
#include "block.h"

/*********************************** MACROS ***********************************/

/* The macros definitions for your header go here */

// The maximum number of elements in the finite-sized linked list.
#ifndef CIRCULAR_LIST_MAX_LEN
#define CIRCULAR_LIST_MAX_LEN 32
#endif

/********************************** STRUCTS ***********************************/

// An index within the CircularList.
typedef unsigned int list_index;

// A link within the Circular List.
struct CircularLink {
    list_index next; // The pointer to the next element in the CircularList.
    struct Segment segment; // The Segment describing a block of free memory.
};

// Implementation of a fixed-size circular linked list.
struct CircularList {
    unsigned int length; // The number of used elements in the list.
    list_index head;     // The index to a valid element in the CircularList.
    struct CircularLink
        links[CIRCULAR_LIST_MAX_LEN]; // The elements within the CircularList.
    char used[CIRCULAR_LIST_MAX_LEN]; // For each index, whether it is free or
                                      // used.
};

/********************************* PROTOTYPES *********************************/

// Used to insert a new CircularLink in the CircularList in a sorted manner, and
// returns its index in the array.
list_index insert_link(struct CircularList *list_ptr, struct CircularLink link);

// Used to remove a block in the CircularList and return it.
struct CircularLink remove_link(struct CircularList *list_ptr,
                                list_index index);

// Gets the next link within the list.
struct CircularLink *next_link(struct CircularList *list,
                               const struct CircularLink *link);

// Creates a new CircularList from a single Segment.
struct CircularList new_list(const struct Segment segment);

// Gets the head of the list.
struct CircularLink *get_head(struct CircularList *list);

/* End of include once header guard */
#endif

/************************************ EOF *************************************/
