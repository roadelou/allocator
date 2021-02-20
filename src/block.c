/********************************** METADATA **********************************/

/*
 * Contributors: roadelou
 * Contacts:
 * Creation Date: 2021-02-19
 * Language: C Source
 */

/********************************** INCLUDES **********************************/

// The header we are implementing.
#include "block.h"

// Used for debugging only, would be removed in production.
#include <assert.h>

/********************************* PROTOYPES **********************************/

// Same as are_contiguous, but ordered. Will only work if segmentA comes first.
static int are_contiguous_ordered(const struct Segment segmentA,
                                  const struct Segment segmentB);

/************************************ MAIN ************************************/

/* The main function of your code goes here. */

/********************************* FUNCTIONS **********************************/

// Return the last address in a Segment.
block_ptr end_of(const struct Segment segment) {
    return segment.start + segment.length;
}

// Returns True if two segments are contiguous and can be fused.
int are_contiguous(const struct Segment segmentA,
                   const struct Segment segmentB) {
    // Either the end of the A is the beginning of B, or the other way around.
    if (segmentA.start < segmentB.start) {
        return are_contiguous_ordered(segmentA, segmentB);
    } else {
        return are_contiguous_ordered(segmentB, segmentA);
    }
}

// Merges two segments and returns the bigger Segment.
struct Segment merge(const struct Segment segmentA,
                     const struct Segment segmentB) {
    // Sanity check.
    assert(are_contiguous(segmentA, segmentB));

    // Assuming the two segments can be merged, we just have to find which comes
    // first.
    if (segmentA.start < segmentB.start) {
        // Returning the concatenated segments.
        return (struct Segment){.start = segmentA.start,
                                .length = segmentA.length + segmentB.length};
    } else {
        // Returning the concatenated segments.
        return (struct Segment){.start = segmentB.start,
                                .length = segmentA.length + segmentB.length};
    }
}

// Returns a subsegment of given size from within the provided one. Also resizes
// the original segment.
struct Segment extract_from(struct Segment *segment_ptr, unsigned int size) {
    // Sanity Check.
    assert(segment_ptr->length > size);

    // We will allocate our new pointer at the beginning of the original one.
    block_ptr new_segment_start = segment_ptr->start;

    // Reducing the size of the original Segment.
    segment_ptr->start += size;
    segment_ptr->length -= size;

    // Returning a new Segment which comes after the previous one.
    return (struct Segment){.start = new_segment_start, .length = size};
}

// Internal functions.

// Same as are_contiguous, but ordered. Will only work if segmentA comes first.
static int are_contiguous_ordered(const struct Segment segmentA,
                                  const struct Segment segmentB) {
    return segmentB.start == end_of(segmentA);
}

/************************************ EOF *************************************/
