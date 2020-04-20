/*
 * Hash table uses the
 *
 */

#include "common.h"

#ifdef __TESTING_HASH_TABLE_C__
#  define fatal_error(...) do {fprintf(stderr, __VA_ARGS__); exit(1);}while(0)
#endif

#define TABLE_MAX_LOAD 0.75

typedef struct {
    const char* key;
    size_t size;
    void* data;
} _table_entry_t;

typedef struct {
    size_t count;
    size_t capacity;
    _table_entry_t* entries;
} _hash_table_t;

/*
 * Return the smaller of the two parameters.
 */
static size_t _min(size_t v1, size_t v2)
{
    return (v1 < v2) ? v1 : v2;
}

/*
 * This is a “FNV-1a” hash function. Do not mess with the constants.
 */
static uint32_t make_hash(const char* key)
{
    uint32_t hash = 2166136261u;

    for (int i = 0; i < strlen(key); i++)
    {
        hash ^= key[i];
        hash *= 16777619;
    }

    return hash;
}

/*
 * If the entry is found, return the slot, if the entry is not found, then the slot
 * returned is where to put the entry. Check the slot's key to tell the difference.
 */
static _table_entry_t* find_slot(_table_entry_t* ent, size_t cap, const char* key)
{
    uint32_t index = make_hash(key) & (cap - 1);

    while(1) {
        _table_entry_t* entry = &ent[index];

        // depends on left evaluate before right
        if((entry->key == NULL)||(!strcmp(key, entry->key))) {
#ifdef __TESTING_HASH_TABLE_C__
    if(entry->key != NULL)
        printf("found: index: %-2u key: %-12s value: %s\n", index, key, (char*)entry->data);
    else
        printf("insert: index: %-2u key: %-12s\n", index, key);
#endif
            return entry;
        }

        index = (index + 1) & (cap - 1);
    }
}

/*
 * Grow the table if it needs it. Since the hash values change when the table size
 * changes, this function simply re-adds them to the new table, then updates the
 * data structure.
 */
static void grow_table(_hash_table_t* tab)
{
    if(tab->count + 2 > tab->capacity * TABLE_MAX_LOAD) {
#ifdef __TESTING_HASH_TABLE_C__
    printf("\ngrowing table\n");
    printf("  table capacity: %lu\n", tab->capacity);
    printf("  table count: %lu\n", tab->count);
#endif
        // table must always be an even power of 2 for this to work.
        size_t capacity = tab->capacity << 1;

        _table_entry_t* entries = (_table_entry_t*)calloc(capacity, sizeof(_table_entry_t));
        if(entries == NULL)
            fatal_error("cannot allocate %lu bytes for hash table", capacity * sizeof(_table_entry_t));

        // re-add the table entries to the new table.
        if(tab->entries != NULL) {
            for(int i = 0; i < tab->capacity; i++) {
                if(tab->entries[i].key != NULL) {
                    _table_entry_t* ent = find_slot(entries, capacity, tab->entries[i].key);
                    // if the key is the same, (i.e. not NULL) the replace the data. There can
                    // be no duplicate entries. No need to check it.
                    ent->key = tab->entries[i].key;
                    ent->size = tab->entries[i].size;
                    ent->data = tab->entries[i].data;
                }
            }
            // free the old table
            free(tab->entries);
        }

        tab->entries = entries;
        tab->capacity = capacity;
#ifdef __TESTING_HASH_TABLE_C__
    printf("\nfinished growing table\n");
    printf("  table capacity: %lu\n", tab->capacity);
    printf("  table count: %lu\n", tab->count);
#endif
    }
}

hash_table_t create_hash_table(void)
{
    _hash_table_t* tab;

    tab = calloc(1, sizeof(_hash_table_t));
    if(tab == NULL) {
        fatal_error("cannot allocate %lu bytes for hash table structure", sizeof(_hash_table_t));
    }

    tab->capacity = 0x01 << 3;
    tab->entries = (_table_entry_t*)calloc(tab->capacity, sizeof(_table_entry_t));
    return (hash_table_t)tab;
}

void destroy_hash_table(hash_table_t table)
{
    _hash_table_t* tab  = (_hash_table_t*)table;

    if(tab != NULL) {
        if(tab->entries != NULL) {
            for(int i = 0; i < tab->capacity; i++) {
                if(tab->entries[i].data != NULL)
                    free(tab->entries[i].data);
                if(tab->entries[i].key != NULL)
                    free((void*)tab->entries[i].key);
            }
            free(tab->entries);
        }
        free(tab);
    }
}

int insert_hash_table(hash_table_t table, const char* key, void* data, size_t size)
{
    _hash_table_t* tab  = (_hash_table_t*)table;
    grow_table(tab);

    _table_entry_t* entry = find_slot(tab->entries, tab->capacity, key);
    int retv = (entry->key == NULL)? HASH_NO_ERROR: HASH_REPLACE;

    entry->key = strdup(key);
    if(entry->key == NULL)
        fatal_error("cannot allocate %lu bytes for hash table key", strlen(key));

    entry->data = malloc(size);
    if(entry->data == NULL)
        fatal_error("cannot allocate %lu bytes for hash table data", size);

    memcpy(entry->data, data, size);
    entry->size = size;
    tab->count++;

    return retv;
}

int find_hash_table(hash_table_t table, const char* key, void* data, size_t size)
{
    _hash_table_t* tab  = (_hash_table_t*)table;
    _table_entry_t* entry = find_slot(tab->entries, tab->capacity, key);
    int retv = HASH_NO_ERROR;

    if(entry->key != NULL) {
        if(entry->data != NULL) {
            if(entry->size != size)
                retv = HASH_DATA_SIZE;
            memcpy(data, entry->data, _min(size, entry->size));
        }
        else
            retv = HASH_NO_DATA;
    }
    else
        retv = HASH_NOT_FOUND;

    return retv;
}

#ifdef __TESTING_HASH_TABLE_C__

int main(void)
{
    char* strs[] = { "foo", "bar", "baz", "bacon", "eggs", "potatoes", "onions", "knuckles",
                    "are", "dragging", "hoops", "of", "chocolate", "almonds", "with", "sprinkles",
                    "and", "cyanide", "log", "balls", "eaten", "by", "unicorns", "as", "pink", "stripes",
                    "given", "to", "nuclear", "pound", "cake", "candles", "snards", "snipes", NULL };
    hash_table_t table = create_hash_table();
    _hash_table_t* tab  = (_hash_table_t*)table;
    char buffer[128];


    printf("\ninsert entries\n");
    printf("  table capacity: %lu\n", tab->capacity);
    printf("  table count: %lu\n", tab->count);
    for(int i = 0; strs[i] != NULL; i++)
        insert_hash_table(table, strs[i], strs[i], strlen(strs[i])+1);

    printf("\nfind entries\n");
    printf("  table capacity: %lu\n", tab->capacity);
    printf("  table count: %lu\n", tab->count);
    for(int i = 0; strs[i] != NULL; i++)
    {
        find_hash_table(table, strs[i], &buffer, sizeof(buffer)); //strlen(strs[i])+1);
        //printf("value: %s\n", buffer);
    }
}


#endif
