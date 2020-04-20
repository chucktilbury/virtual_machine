#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"

typedef struct __array_entry_t
{
    void* data;
    size_t size;
} _array_entry_t;

typedef struct __array_t
{
    comp_func_t func;
    int items;
    size_t capacity;
    _array_entry_t** array;
} _array_t;

/**
 * @brief Get the minimum of the two values given.
 *
 * @param v1 item 1
 * @param v2 item 2
 *
 * @returns The minimum value of the two arguments.
 */
static size_t _min(size_t v1, size_t v2)
{
    return (v1 < v2) ? v1 : v2;
}

/**
 * @brief Grow the array.
 *
 * If another item would not fit into the array then make more room. The size of the array will
 * always be a power of 2.
 *
 * @param ary The array to grow.
 */
static void _grow(_array_t * ary)
{
    if(ary->items + 2 > ary->capacity)
    {
        ary->capacity = ary->capacity << 1;
        ary->array = realloc(ary->array, ary->capacity * sizeof(_array_entry_t));
    }
    // else do nothing
}

/**
 *
 */

/**
 * @brief Partial bubble sort.
 *
 * This takes the last item in the array and places it in a sorted list using a
 * partial bubble sort. Since items will be placed in the array one at a time at
 * the end, this simply moves it toward the front until it's where it belongs.
 *
 * @param ary
 *
 * @returns the index of the item after sorting.
 *
 *
 */
static int _sort(_array_t * ary)
{

    int idx;

    for(idx = ary->items - 1; idx > 0; idx--)
    {
        if(ary->func(ary->array[idx - 1]->data, ary->array[idx]->data) > 0)
        {
            _array_entry_t* temp = ary->array[idx];

            ary->array[idx] = ary->array[idx - 1];
            ary->array[idx - 1] = temp;
        }
    }
    return idx;
}

/**
 * @brief Internal function. Find the item specified and if it's not there, return NULL.
 *
 * @param ary
 * @param key
 *
 * @returns pointer to the internal data structure that was found, else NULL.
 */
static _array_entry_t* _find(_array_t * ary, void* key)
{

    int first = 0;
    int last = ary->items - 1;
    int index = last / 2;
    _array_entry_t* entry = NULL;

    while(first <= last)
    {
        if(ary->func(ary->array[index]->data, key) < 0)
            first = index + 1;
        else if(ary->func(ary->array[index]->data, key) > 0)
            last = index - 1;
        else
        {
            entry = ary->array[index];
            break;
        }

        index = (first + last) / 2;
    }

    return entry;
}

/**
 *
 */
static _array_entry_t* _create_entry(void* data, size_t size)
{
    _array_entry_t* entry = (_array_entry_t *) malloc(sizeof(_array_entry_t));

    if(entry == NULL)
    {
        fprintf(stderr, "ERROR: cannot allocate memory for new array entry\n");
        return NULL;
    }

    entry->data = malloc(size);
    if(entry->data == NULL)
    {
        fprintf(stderr, "ERROR: cannot allocate memory for new array entry data\n");
        free(entry);
        return NULL;
    }

    if(size > 0)
        memcpy(entry->data, data, size);
    entry->size = size;
    return entry;
}

/**
 * @brief Create an array data structure.
 *
 * If the "func" argument is NULL, then the array will not be sorted. If the func arg is given then the function
 * will be called when the array is updated or read.
 *
 * @param func Compare function for the intended data.
 *
 * @returns Opaque handle to use when accessing the array.
 */
array_t array_init(comp_func_t func)
{
    _array_t* ary = (_array_t *) malloc(sizeof(_array_t));

    if(ary == NULL)
    {
        fprintf(stderr, "ERROR: cannot allocate memory for dynamic array data structure.");
        return (array_t) NULL;
    }

    // if func is null, then the array is not sorted
    ary->func = func;
    ary->items = 0;
    // actually capacity is 0, but _grow() knows that this means add memory.
    ary->capacity = 1;
    ary->array = NULL;

    return (array_t) ary;
}

/**
 * @brief Destroy the array and all of the data managed by it.
 *
 * @param array
 */
void array_uninit(array_t array)
{
    _array_t* ary = (_array_t *) array;

    for(int i = 0; i < ary->items; i++)
    {
        if(ary->array[i]->data != NULL)
            free(ary->array[i]->data);
        free(ary->array[i]);
    }
}

/**
 * @brief Add a data elelemt to the array.
 *
 * If the compare function was set specified when the array was created, then the new item will be inserted into
 * the array in sorted order as defined by the compare function. Otherwise, the new item will be placed at the end
 * of the array. Pretty much any data can be added to the array and the array can hold diverse data types.
 *
 * @param array The array to add to.
 * @param data  Pointer to the data to add.
 * @param size  Size of the data to add.
 * @param type  Type of the data. This value will be returned when this item is read from the array.
 *
 * @returns The result of the add.
 */
int array_add_entry(array_t array, void* data, size_t size)
{
    _array_t* ary = (_array_t *) array;

    _grow(ary);
    _array_entry_t* entry = _create_entry(data, size);

    if(entry == NULL)
    {
        fprintf(stderr, "ERROR: cannot allocate memory for new array entry\n");
        return ARRAY_RET_MEMORY;
    }

    ary->array[ary->items++] = entry;

    if(ary->func)
        _sort(ary);

    return ARRAY_RET_NO_ERROR;
}

/**
 * @brief Retrieve an item from the array, given the index of the item.
 *
 * Returns the data by copying it into the data parameter. The size of the data will be the smaller of the size given
 * and the size that was actually stored for this element.
 *
 * @param array The array to read.
 * @param index The index of the element desired.
 * @param data  Pointer to the location to copy the data to.
 * @param size  Size of the data location.
 *
 * @returns The status of the read if there was an error, or the type of the data element upon success.
 */
int array_get_by_index(array_t array, int index, void* data, size_t size)
{
    _array_t* ary = (_array_t *) array;

    if(ary != NULL)
    {
        if(index < ary->items)
        {
            _array_entry_t* entry = ary->array[index];

            if(entry->data != NULL)
            {
                memcpy(data, entry->data, _min(entry->size, size));
                return ARRAY_RET_NO_ERROR;
            }
            else
                return ARRAY_RET_NO_DATA;
        }
        else
            return ARRAY_RET_NOT_FOUND;
    }
    else
        return ARRAY_RET_INVALID;
}

/**
 * @brief Read the array based upon a key given.
 *
 * The key given must be understandable by the compare function that was given when the array was created. If the
 * compare function was not set, then this simply returns "NOT FOUND" in all cases.
 *
 * @param array The array to read.
 * @param key   Key to use.
 * @param data  Pointer to the location to place the data that was stored.
 * @param size  Size of the data to read.
 *
 * @returns The status of the array is returned in case of an error, or the type of the data item upon success.
 */
int array_get_by_key(array_t array, void* key, void* data, size_t size)
{
    _array_t* ary = (_array_t *) array;

    if(ary != NULL)
    {
        if(ary->func != NULL)
        {
            if(key != NULL)
            {
                _array_entry_t* entry = _find(ary, key);

                if(entry == NULL)
                    return ARRAY_RET_NOT_FOUND;
                else
                {
                    if(entry->data != NULL)
                    {
                        memcpy(data, entry->data, _min(entry->size, size));
                    }
                    return ARRAY_RET_NO_ERROR;
                }
            }
            else
                return ARRAY_RET_INVALID;
        }
        else
            return ARRAY_RET_NOT_FOUND;
    }
    else
        return ARRAY_RET_INVALID;

    return ARRAY_RET_INVALID;   // unreachable
}

/**
 * @brief Return the number of items currently in the array.
 *
 * @param array
 *
 * @returns Number of items.
 */
int array_get_num_items(array_t array)
{
    _array_t* ary = (_array_t *) array;

    return ary->items;
}

/**
 * @brief Add item to the end of the array.
 *
 * This function adds to the end of the array, regardless of whether the compare function was specified
 * when the array was created.
 *
 * @param array
 *
 * @returns Status of the add.
 */
int array_push(array_t array, void* data, size_t size)
{
    _array_t* ary = (_array_t *) array;

    _grow(ary);
    _array_entry_t* entry = _create_entry(data, size);

    if(entry == NULL)
    {
        fprintf(stderr, "ERROR: cannot allocate memory for new array entry\n");
        return ARRAY_RET_MEMORY;
    }

    ary->array[ary->items++] = entry;

    return ARRAY_RET_NO_ERROR;
}

/**
 * @brief Return the last item in the array and destroy it.
 *
 * @param array
 *
 * @returns Status of the read upon failure. The type of the item upon success.
 */
int array_pop(array_t array, void* data, size_t size)
{
    _array_t* ary = (_array_t *) array;

    if(ary != NULL)
    {
        int index = --ary->items;

        if(index >= 0)
        {
            _array_entry_t* entry = ary->array[index];

            if(entry->data != NULL)
                memcpy(data, entry->data, _min(entry->size, size));
            else
                return ARRAY_RET_NO_DATA;

            free(entry->data);
            free(entry);
            return ARRAY_RET_NO_ERROR;
        }
        else
        {
            ary->items = 0;
            return ARRAY_RET_NOT_FOUND;
        }
    }
    else
        return ARRAY_RET_INVALID;
}

void print_array(array_t a)
{
    int items = array_get_num_items(a);

    // char buffer[20];
    _array_entry_t entry;

    for(int i = 0; i < items; i++)
    {
        int ty = array_get_by_index(a, i, (void *)&entry, sizeof(_array_entry_t));

        printf("%4d: %d %s\n", i, ty, (char *)entry.data);
    }

    printf("num_items = %d\n", items);
    printf("capacity = %lu\n\n", ((_array_t *) a)->capacity);
}
