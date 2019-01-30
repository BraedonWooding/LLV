#ifndef LLV_ARRAY_H
#define LLV_ARRAY_H

#include <stdbool.h>
#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

typedef struct _array_t *Array;
typedef struct _array_data_t *ArrayNode;

struct _array_data_t {
   char *ptr;
   Data data; // the data type
   TypeTag data_tag; // the corresponding tag;
};

struct _array_t {
   struct _collection_t parent;
   ArrayNode data;
   int len;
};

/* Create a new array with a given name and size */
Array array_new(char *name, int size);

/* Free the array */
void array_free(Array array);

/* Get the array node at the given index */
ArrayNode array_at(Array array, int index);

/*
   Create a new array node for use in the other functions 
   Could use NEW_NODE(array, data) instead! 
*/
struct _array_data_t array_new_node(Data data, TypeTag type);

/*
   Resize the array to the new size keeping the old elements.
*/
void array_resize(Array array, int new_size);

/*
   Set a specific index to a node value.
*/
void array_set(Array array, int index, struct _array_data_t node);

/*
   Get the array length.
*/
int array_length(Array array);

#endif /* LLV_ARRAY_H */
