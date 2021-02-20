/********************************** METADATA **********************************/

/*
 * Contributors: roadelou
 * Contacts:
 * Creation Date: 2021-02-19
 * Language: C Source
 */

/********************************** INCLUDES **********************************/

// The header we are currently implementing.
#include "circular_list.h"

// Used for Segment manipulation in insertion.
#include "block.h"

// Used for debugging, would be removed in production.
#include <assert.h>

/********************************* PROTOYPES **********************************/

// Returns the first free spot to add a new link in the CircularList.
static list_index first_free(const struct CircularList *list);

/************************************ MAIN ************************************/

/* The main function of your code goes here. */

/********************************* FUNCTIONS **********************************/

// Used to insert a new CircularLink in the CircularList in a sorted manner, and
// returns its index in the array.
list_index insert_link(struct CircularList *list, struct CircularLink link) {
    // Sanity Check.
    assert(list->length < CIRCULAR_LIST_MAX_LEN);

    // First of all, we store the link in the links array.
    list_index link_index = first_free(list);
    list->links[link_index] = link;
    // Marking the spot as taken.
    list->used[link_index] = 1;
    // Increasing the length of the list.
    list->length++;
    // From now on, we only care about the copy of the link which is stored in
    // the links array.
    struct CircularLink *link_ptr = &list->links[link_index];

    // We first get the head link.
    struct CircularLink *head_link = get_head(list);

    // We have to find where to insert the link. Three cases appear:
    //  1- The link contains the first address and should come before any
    //  	other.
    //  2- The link comes between two existing links.
    //  3- The link comes after any existing links.
    //
    // Case 2 is easy to spot. For cases 1 and 3 we keep track of the links
    // holding the lowest and highest addresses.

    list_index highest_link = list->head;
    block_ptr highest_address = end_of(head_link->segment);

    list_index lowest_link = list->head;
    block_ptr lowest_address = head_link->segment.start;

    // We don't iterate beyong the length of the linked list (-1 because we
    // already inspected the head link.
    for (unsigned int i = 0; i < list->length; i++) {
        // We get the next link.
        struct CircularLink *adjacent_link = next_link(list, head_link);
        // First case (easy case), link comes between the head_link and
        // next_link.
        if ((adjacent_link->segment.start < link_ptr->segment.start) &&
            (link_ptr->segment.start < head_link->segment.start)) {
            // Changing the "next" pointer of the two links.
            link_ptr->next = head_link->next; // Pointer to next_link.
            head_link->next = link_index;
            // Early return.
            return link_index;
        } else {
            // We update the min and max addresses if necessary.
            if (adjacent_link->segment.start < lowest_address) {
                lowest_address = adjacent_link->segment.start;
                lowest_link = head_link->next;
            } else if (end_of(adjacent_link->segment) > highest_address) {
                highest_address = end_of(adjacent_link->segment);
                highest_link = head_link->next;
            }
        }
    }
    // If we reach this line, it means that our link is either the highest or
    // lowest link in the CircularList.
    if (link_ptr->segment.start < lowest_address) {
        // Our link is going to become the lowest link. Because we don't know
        // who pointed to the current lowest link, we are going to take his spot
        // in the links array. We first have to move the current lowest link to
        // the link_index.
        list->links[link_index] = list->links[lowest_link];
        // Copy is done, we can overwrite the original lowest link. We reuse the
        // stack copy of the insterted link, since it has not been modified yet.
        // We should point to the former lowest link, but it should keep the
        // same destination.
        link.next = link_index;
        list->links[lowest_link] = link;
        // We return the expected value.
        return lowest_link;
    } else {
        // Our link is the new highest link. We should insert ourselves after
        // the current highest link and before its destination.
        struct CircularLink *former_highest_link = &list->links[highest_link];
        link_ptr->next = former_highest_link->next;
        former_highest_link->next = link_index;
        // We return the expected value.
        return link_index;
    }
}

// Used to remove a block in the CircularList and return it.
struct CircularLink remove_link(struct CircularList *list, list_index index) {
    // Sanity Check;
    assert(list->length > 1);

    // We don't know who points to the current link, so we are going to the
    // "next" of the current link to take its place. We grab (and copy) the link
    // we will remove.
    struct CircularLink removed_link = list->links[index];
    list_index next_link_index = removed_link.next;
    // We copy the link that comes after us.
    list->links[index] = list->links[next_link_index];
    // We free the space that was previously held by the other link.
    list->used[next_link_index] = 0;
    // Decreasing the length of the list.
    list->length--;

    // Note that even if we were the head of the linked list, someone took our
    // spot hence the head is still valid. So that means head could be 0 and
    // just not exist...
    return removed_link;
}

// Gets the next link within the list.
struct CircularLink *next_link(struct CircularList *list,
                               const struct CircularLink *link) {
    // Using the list_index of the head link.
    return &(list->links[link->next]);
}

// Creates a new CircularList from a single Segment.
struct CircularList new_list(const struct Segment segment) {
    // We first create a segment for the link, with list_index 0.
    struct CircularLink first_link =
        (struct CircularLink){.next = 0, .segment = segment};
    // We then create a CircularList with only this link.
    struct CircularList list;
    list.length = 1;
    list.head = 0; // The link at address 0 is first_link and thus valid.
    list.links[0] = first_link;
    // Only the first list_index is used.
    list.used[0] = 1;
    for (list_index i = 1; i < sizeof(list.used); i++) {
        list.used[i] = 0;
    }
    // Returning the CircularList.
    return list;
}

// Gets the head of the list.
struct CircularLink *get_head(struct CircularList *list) {
    return &(list->links[list->head]);
}

// Internal functions.

// Returns the first free spot to add a new link in the CircularList.
static list_index first_free(const struct CircularList *list) {
    // Sanity check.
    assert(list->length < CIRCULAR_LIST_MAX_LEN);

    // Searching free spot.
    list_index free_index = 0;
    while (free_index < CIRCULAR_LIST_MAX_LEN) {
        if (list->used[free_index] == 0) {
            // We have found an empty spot.
            return free_index;
        } else {
            // Checking the next spot.
            free_index++;
        }
    }
    // Will never happen.
    assert(0);
}

/************************************ EOF *************************************/
