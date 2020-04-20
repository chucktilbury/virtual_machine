#ifndef __DYNAMIC_ARRAY_H__
#  define __DYNAMIC_ARRAY_H__

typedef enum array_ret_type_t
{
    ARRAY_RET_INVALID = -1,
    ARRAY_RET_MEMORY = -2,
    ARRAY_RET_NOT_FOUND = -3,
    ARRAY_RET_DELETED = -4,
    ARRAY_RET_NO_DATA = -5,
    ARRAY_RET_NO_ERROR = 0,
} array_ret_type_t;

typedef void* array_t;
typedef int (*comp_func_t)(const void* , const void* );

array_t array_init(comp_func_t func);
void array_uninit(array_t ary);
int array_add_entry(array_t ary, void* data, size_t size);
int array_get_by_index(array_t ary, int index, void* data, size_t size);
int array_get_by_key(array_t ary, void* key, void* data, size_t size);
int array_get_num_items(array_t ary);
int array_push(array_t array, void* data, size_t size);
int array_pop(array_t array, void* data, size_t size);

#endif
