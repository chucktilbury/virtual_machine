/*
 * This module manages a dynamic array by controling the size of it. It is intended
 * for use by the dynamic LIFO and FIFO modules.
 */
#include "common.h"
#ifdef __TESTING_ARRAY_MANAGER_C__
#  define fatal_error(...) do {fprintf(stderr, __VA_ARGS__); exit(1);}while(0)
#endif

typedef struct
{
    size_t nitems;           // number of items currently in the array
    size_t capacity;         // capacity in items
    size_t item_size;        // size of each item
    unsigned char* buffer;   // raw buffer where the items are kept
} managed_array_t;

/*
 * This resizes the array to grow it. This should always be called before
 * adding aything to it.
 *
 * Aborts the program upon failure.
 */
static void resize_array(managed_array_t * array)
{
    if(array->nitems + 2 > array->capacity)
    {
        array->capacity = array->capacity << 1;
        array->buffer = realloc(array->buffer, array->capacity * array->item_size);

        if(array->buffer == NULL)
            fatal_error("cannot allocate %d bytes for managed buffer", array->capacity * array->item_size);
    }
}

/*
 * Internal function that calculates a pointer into the buffer, given the index of
 * the intended item. This returns a pointer to the first byte of the item that was
 * stored. It's up to the caller to convert it to a pointer to the data object that
 * it knows about.
 */
static unsigned char* array_at_index(managed_array_t * array, int index)
{
    unsigned char* ptr = &array->buffer[array->item_size * index];

    return ptr;
}

/*
 * Free the array buffer. This is only used when the array will not longer
 * be used, such as when the program ends.
 */
void destroy_array(array_t arr)
{
    managed_array_t* array = (managed_array_t *) arr;

    if(array->buffer != NULL && array->nitems != 0)
        free(array->buffer);
}

/*
 * Initially create the array and initialize the contents to initial values.
 * If the array was in use before this, the buffer will be freed.
 */
void init_array(array_t arr, size_t size)
{
    managed_array_t* array = (managed_array_t *) arr;

    destroy_array(array);

    array->item_size = size;
    array->capacity = 1;
    array->nitems = 0;
    array->buffer = NULL;
}

/*
 * Store the given item in the given array at the end of the array.
 */
void append_array(array_t arr, void* item)
{
    managed_array_t* array = (managed_array_t *) arr;

    resize_array(array);
    memcpy(array_at_index(array, array->nitems), item, array->item_size);
    array->nitems++;
}

/*
 * If the index is within the bounds of the array, then return a raw pointer to the
 * element specified. If it is outside the array, or if there is nothing in the array,
 * then return NULL.
 */
int get_array(array_t arr, int index, void* buffer)
{
    managed_array_t* array = (managed_array_t *) arr;

    if(array != NULL) {
        if(buffer != NULL)
        {
            if(index >= 0 && index <= array->nitems)
            {
                memcpy(buffer, array_at_index(array, index), array->item_size);
                return ARRAY_NO_ERROR;   // success
            }
            else
                return ARRAY_BAD_INDEX;   // failed
        }
    }
    else
        return ARRAY_INVALID;

    return ARRAY_BAD_INDEX;   // failed
}

/*
 * Push a value to the end of the array as if it were a stack.
 */
void push_array(array_t array, void* value)
{
    append_array(array, value);
}

/*
 * Pop a value from the end of the array as if it were a stack. This also
 * shrinks the number of items in the stack by one element. Since the items
 * are memcpy()'ed to the stack there is no memory management required.
 */
int pop_array(array_t arr, void* value)
{
    managed_array_t* array = (managed_array_t *) arr;

    array->nitems--;
    return get_array(arr, array->nitems, value);
}

#ifdef __TESTING_ARRAY_MANAGER_C__
/*
 * Simple test using an array of strings.
 */

int main(void)
{
    char* strs[] = { "foo", "bar", "baz", "bacon", "eggs", "potatoes", "onions", NULL };
    managed_array_t array;
    char* ptr;

    init_array(&array, sizeof(char *));
    for(int i = 0; strs[i] != NULL; i++)
        append_array(&array, (void *)&strs[i]);

    for(int i = 0; strs[i] != NULL; i++)
    {
        get_array(&array, i, &ptr);
        printf("value: %s\n", ptr);
    }

    printf("nitems: %lu\n", array.nitems);
    printf("item_size: %lu\n", array.item_size);
    printf("capacity: %lu\n", array.capacity);

    printf("null: %d\n", get_array(&array, -1, &ptr));
    printf("null: %d\n", get_array(&array, 100, &ptr));
    printf("null: %d\n", get_array(&array, 8, &ptr));

    for(int i = 0; i < 3; i++)
    {
        pop_array(&array, &ptr);
        printf("value: %s\n", ptr);
    }
}

#endif
