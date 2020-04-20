#ifndef __ARRAY_MANAGER_H__
#  define __ARRAY_MANAGER_H__

enum
{
    ARRAY_INVALID = -1,
    ARRAY_NOT_FOUND = -2,
    ARRAY_BAD_INDEX = -3,
    ARRAY_NO_ERROR = 0,
};

typedef void* array_t;

void init_array(array_t array, size_t size);
void destroy_array(array_t array);
void append_array(array_t array, void* item);
int get_array(array_t array, int index, void* buffer);
void push_array(array_t array, void* value);
int pop_array(array_t arr, void* value);

#endif
