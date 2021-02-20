/* Include once header guard */
#ifndef BLOCK_HEADER_INCLUDED
#define BLOCK_HEADER_INCLUDED

/********************************** METADATA **********************************/

/*
 * Contributors: roadelou
 * Contacts:
 * Creation Date: 2021-02-19
 * Language: C Header
 */

/********************************** INCLUDES **********************************/

/* The includes for your header go here */

/*********************************** MACROS ***********************************/

/* The macros definitions for your header go here */

/********************************** STRUCTS ***********************************/

// We define the type of block_ptr to talk about the address of a block within
// the memory.
typedef unsigned int block_ptr;

// A Segment is a continous area inside the memory.
struct Segment {
    block_ptr start;     // The start address of the Segment.
    unsigned int length; // The number of blocks in the Segment.
};

/********************************* PROTOTYPES *********************************/

// Return the last address in a Segment.
block_ptr end_of(const struct Segment segment);

// Returns True if two segments are contiguous and can be fused.
int are_contiguous(const struct Segment segmentA,
                   const struct Segment segmentB);

// Merges two segments and returns the bigger Segment.
struct Segment merge(const struct Segment segmentA,
                     const struct Segment segmentB);

// Returns a subsegment of given size from within the provided one. Also resizes
// the original segment.
struct Segment extract_from(struct Segment *segment_ptr, unsigned int size);

/* End of include once header guard */
#endif

/************************************ EOF *************************************/
