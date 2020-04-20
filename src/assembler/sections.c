/*
 * As the assembler churns through the source, it creates these data structures to hold the results.
 *
 * When symbols are encountered, they are stored in a separate hash table with the index into the section
 * that they are a part of. The section data is stored as a simple byte stream, including the larger sized
 * data objects. They are stored in such a way as to be able to cast the data given the index. The data in
 * the section data structure is kept in a dynamic array that grows as data is added to it. When data is
 * defined with an initializer, that is stored in the section as well. If the data is given as
 * uninitialized, the zeros are stored.
 *
 * When the program is serialized, the sections are concatenated and the indexes are fixed up to point to
 * the correct location. The names of objects are saved to a debug section, which will be implemented in
 * the future. Otherwise, they are discarded.
 *
 * There are two main types of sections, data and code. All data is read/write and the code is read-only
 * from the point of view of the VM. A third section, the debug section, is used to store the symbols that
 * were used in the program, along with the section offsets.
 *
 * A section must have a name. References to a section are defined using simple "dot" notation. For example,
 * a symbol named "foo" which was defined in a section named "bar" is referenced as "bar.foo". In the global
 * symbol table, that is the name that is stored. In the section, the undotted symbol is stored. Information
 * about the type of the name is stored in the symbol table.
 */

#include "common.h"

#include "scanner.h"
#include "errors.h"
#include "sections.h"
//#include "dynamic_array.h"


typedef struct
{
    const char* name;        // simple name connected to the data object.
    int type;                // type of each element in the buffer.
    size_t size;             // this is the number of bytes given by the buffer.
    size_t capacity;         // how much data this buffer will hold
    uint8_t* data;           // This is a buffer of the type given.
} _section_entry_t;

typedef struct sec_t
{
    const char* name;        // simple name connected to the data object.
    int type;                // type of each element in the buffer.
    array_t table;           // section data.
} _section_t;

static array_t section_table;

// TODO: Fix strdup()'s

static void grow_array(_section_entry_t * entry, size_t size)
{

    size_t new_cap = entry->capacity;

    do
    {
        new_cap = new_cap << 1; // grow by powers of 2
    } while(new_cap < (entry->size + size + 1));

    entry->capacity = new_cap;
    entry->data = realloc(entry->data, new_cap);
}

static int compare_entry_func(const void* p1, const void* p2)
{
    return strcmp(((_section_entry_t *) (p1))->name, ((_section_entry_t *) (p2))->name);
}

static int compare_func(const void* p1, const void* p2)
{
    return strcmp(((_section_t *) (p1))->name, ((_section_t *) (p2))->name);
}

/************************
 * public interface
 */
void init_sections(void)
{
    section_table = array_init(compare_func);
}

// section_t
void add_section(const char* name, int type)
{
    _section_t sec;

    sec.name = strdup(name);
    sec.type = type;
    sec.table = array_init(compare_entry_func);
    array_add_entry(section_table, (void *)&sec, sizeof(_section_t));
    // return (section_t) sec;
}

void destroy_all_sections(void)
{
    _section_t sec;

    for(int i = 0; i < array_get_num_items(section_table); i++)
    {
        array_get_by_index(section_table, i, (void *)&sec, sizeof(_section_t));
        for(int j = 0; j < array_get_num_items(sec.table); j++)
        {
            _section_entry_t entry;

            array_get_by_index(sec.table, j, (void *)&entry, sizeof(_section_entry_t));
            if(entry.name != NULL)
                free((void *)entry.name);
            if(entry.data != NULL)
                free(entry.data);
        }
        array_uninit(sec.table);
        if(sec.name != NULL)
            free((void *)sec.name);
    }
    array_uninit(section_table);
}

void add_section_entry(const char* sec_name, const char* name)
{
    _section_entry_t entry;

    entry.capacity = 1;
    entry.size = 0;
    entry.name = strdup(name);
    entry.data = NULL;

    _section_t sec;

    sec.name = sec_name;
    array_get_by_key(section_table, (void *)&sec, (void *)&sec, sizeof(_section_t));
    array_add_entry(sec.table, (void *)&entry, sizeof(_section_entry_t));
}

void add_entry_bytes(const char* sec_name, const char* ent_name, void* bytes, size_t size)
{
    _section_t sec;
    _section_entry_t entry;

    sec.name = sec_name;
    array_get_by_key(section_table, (void *)&sec, (void *)&sec, sizeof(_section_t));
    entry.name = ent_name;
    array_get_by_key(sec.table, (void *)&entry, (void *)&entry, sizeof(_section_entry_t));

    if(entry.size + size + 1 > entry.capacity)
    {
        grow_array(&entry, size);
    }

    memcpy((void *)&((uint8_t *) entry.data)[entry.size], bytes, size);
    entry.size += size;
}
