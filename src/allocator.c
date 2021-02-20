/********************************** METADATA **********************************/

/*
 * Contributors: roadelou
 * Contacts:
 * Creation Date: 2021-02-19
 * Language: C Source
 */

/********************************** INCLUDES **********************************/

// The header we are presently implementing.
#include "allocator.h"

// For debugging purposes.
#include <assert.h>

/********************************* PROTOYPES **********************************/

// Returns the index of the memory Segment with information on the allocated
// memory block.
static unsigned int get_segment_index(const struct Allocator *allocator,
                                      block_ptr allocated);

// Returns the index of the first free spot to put an allocated segment within
// the allocator.
static unsigned int first_free(const struct Allocator *allocator);

/************************************ MAIN ************************************/

/* The main function of your code goes here. */

/********************************* FUNCTIONS **********************************/

// Well, malloc, but for blocks...
block_ptr block_malloc(struct Allocator *allocator, unsigned int size) {
    // We look for a free Segment big enough to hold size.
    struct CircularLink *link = get_head(&allocator->list);
    // Used to remove a link if it has exactly the size we want.
    list_index link_address = allocator->list.head;
    for (unsigned int i = 0; i < allocator->list.length; i++) {
        if (link->segment.length > size) {
            // We extract an allocated segment from the big one.
            struct Segment allocated_segment =
                extract_from(&link->segment, size);
            // We add the allocated Segment to the allocated array.
            unsigned int allocated_index = first_free(allocator);
            allocator->allocated[allocated_index] = allocated_segment;
            allocator->used[allocated_index] = 1;
            // We return the expected pointer.
            return allocated_segment.start;
        } else if (link->segment.length == size) {
            // EGDE CASE
            // The allocated segment is the link segment.
            struct Segment allocated_segment = link->segment;
            // We remove the now empty link.
            remove_link(&allocator->list, link_address);
            // We add the allocated Segment to the allocated array.
            unsigned int allocated_index = first_free(allocator);
            allocator->allocated[allocated_index] = allocated_segment;
            allocator->used[allocated_index] = 1;
            // We return the expected pointer.
            return allocated_segment.start;
        } else {
            // We look into the next link.
            link_address = link->next;
            link = next_link(&allocator->list, link);
        }
    }
    // Should never happen in our simplified case.
    assert(0);
}

// free, but for blocks.
void block_free(struct Allocator *allocator, block_ptr allocated) {
    // We first grab the index of the Segment for the allocated block.
    unsigned int allocated_segment_index =
        get_segment_index(allocator, allocated);
    // We grab the associated segment.
    struct Segment allocated_segment =
        allocator->allocated[allocated_segment_index];
    // We clare the presence flag of the Segment from the allocator.
    allocator->used[allocated_segment_index] = 0;
    // Used to handle the case where bothe the preceding and following links can
    // be merged.
    int found_merge = 0;
    struct CircularLink *merged_link;

    // We check if we can insert the Segment in an existing one.
    struct CircularLink *link = get_head(&allocator->list);
    // In case we need it.
    list_index merged_link_index = allocator->list.head;

    for (list_index i = 0; i < CIRCULAR_LIST_MAX_LEN; i++) {
        if ((found_merge == 0) &&
            (are_contiguous(link->segment, allocated_segment))) {
            // We merge the freed segment into an existing one.
            link->segment = merge(link->segment, allocated_segment);
            // We check for an opportunity to merge the now bigger segment with
            // its successor or predecessor. We should only check the remaining
            // links. We have to do this because we don't know who is pointing
            // twords us.
            found_merge = 1;
            merged_link = link;
        } else if ((found_merge == 1) &&
                   (are_contiguous(merged_link->segment, link->segment))) {
            // After merging our allocated segment into an existing link, the
            // link became big enough to be merged with a third one.
            link->segment = merge(link->segment, merged_link->segment);
            // We remove the now unused next link.
            remove_link(&allocator->list, merged_link_index);
            // We cannot have a new merge opportunity, only the neighbours
            // of the freed memory are affected.
            return;
        } else {
            if (found_merge == 0) {
                // We are looking for the first merge opportunity, we must
                // update the merge index in case the next link is the right
                // one.
                merged_link_index = link->next;
            }
            // We haven't found a contiguous segment yet, we keep searching.
            link = next_link(&allocator->list, link);
        }
    }
    // If we reach this line, it means that no fusion was possible. We add a new
    // link to the CircularList. This will fail if the memory fragments beyond
    // CIRCULAR_LIST_MAX_LEN.
    struct CircularLink new_link = {.next = 0, .segment = allocated_segment};
    insert_link(&(allocator->list), new_link);
}

// Defines a new allocator..
struct Allocator new_allocator(const struct Segment memory) {
    // We create a new CircularList from the Segment.
    struct CircularList list = new_list(memory);
    // We then build the new allocator.
    struct Allocator allocator;
    allocator.list = list;
    // We set the presence flags of the allocator to 0. Note that memset is not
    // available.
    for (unsigned int i = 0; i < CIRCULAR_LIST_MAX_LEN; i++) {
        allocator.used[i] = 0;
    }
    // Returning the built allocator.
    return allocator;
}

// Internal functions

// Returns the index of the memory Segment with information on the allocated
// memory block.
static unsigned int get_segment_index(const struct Allocator *allocator,
                                      block_ptr allocated) {
    // Linearly searching the Allocator.
    for (unsigned int i = 0; i < CIRCULAR_LIST_MAX_LEN; i++) {
        if ((allocator->used[i] == 1) &&
            (allocator->allocated[i].start == allocated)) {
            // We have found the right Segment, we may return its index.
            return i;
        }
    }
    // Should never happen.
    assert(0);
}

static unsigned int first_free(const struct Allocator *allocator) {
    for (unsigned int i = 0; i < CIRCULAR_LIST_MAX_LEN; i++) {
        if (allocator->used[i] == 0) {
            // We have found a free spot.
            return i;
        }
    }
    // Should never happen in our simplified case.
    assert(0);
}

/************************************ EOF *************************************/
