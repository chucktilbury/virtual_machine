/*
 * Generic hash table to support the symbol table and symbol attributes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "errors.h"
#include "hash_table.h"

typedef struct __hte__
{
    char* key;
    void* data;
    struct __hte__* next;
} hash_table_entry_t;

typedef struct __htt__
{
    hash_table_entry_t** table;
    int slots;
} hash_table_t;

hash_tab_t create_hash_table(int slots)
{
    hash_table_t* ht;

    if(NULL == (ht = (hash_table_t *) calloc(1, sizeof(hash_table_t))))
        fatal_error("cannot allocate hash table struct");

    if(NULL == (ht->table = (hash_table_entry_t **) calloc(slots, sizeof(hash_table_entry_t))))
        fatal_error("cannot allocate hash table entry");

    ht->slots = slots;
    return ((hash_tab_t) ht);
}

void destroy_hash_table(hash_tab_t ht)
{
    hash_table_entry_t* next,* hte;
    hash_table_t* table = (hash_table_t *) ht;

    if(NULL != table)
    {
        int i;

        for(i = 0; i < table->slots; i++)
        {
            if(NULL != table->table[i])
            {
                for(hte = table->table[i]; hte != NULL; hte = next)
                {
                    next = hte->next;
                    if(NULL != hte->key)
                        free(hte->key);
                    if(NULL != hte->data)
                        free(hte->data);

                    free(hte);
                }
            }
        }
    }
}

uint32_t make_hash(const char* str)
{
    uint32_t hash = 2166136261u;

    for(int i = 0; i < str[i] != 0; i++)
    {
        hash ^= str[i];
        hash *= 16777619;
    }

    return hash;
    // return (uint32_t) XXH32(str, strlen(str), -1);
}

/*
 * This is called when there was a collision storing the entry. It simply
 * searches a trivial linked list.
 */
static inline hash_table_entry_t* find_local(hash_table_t * table, const char* key, int hash)
{

    hash_table_entry_t* hte;

    for(hte = table->table[hash]; NULL != hte; hte = hte->next)
    {
        if(!strcmp(key, hte->key))
        {
            return hte; // found
        }
    }
    return NULL;    // not found
}

int hash_save(hash_tab_t ht, const char* key, void* data, size_t size)
{
    hash_table_t* table = (hash_table_t *) ht;
    hash_table_entry_t* hte;
    int hash;

    if(NULL != table)
    {
        hash = make_hash(key) % table->slots;
        hte = find_local(table, key, hash);
        if(NULL == hte)
        {
            if(NULL == (hte = (hash_table_entry_t *) calloc(1, sizeof(hash_table_entry_t))))
                fatal_error("cannot allocate hash table entry");

            if(NULL == (hte->key = strdup(key)))
                fatal_error("cannot allocate hash table entry name");

            if(data != NULL)
            {
                if(NULL == (hte->data = calloc(size + 2, 1)))
                    fatal_error("cannot allocate hash table data (%d)", size);
                memcpy(hte->data, data, size);
            }
            else
            {
                if(NULL == (hte->data = calloc(0, 0)))
                    fatal_error("cannot allocate hash table data (%d)", 0);
            }

            hte->next = NULL;

            if(NULL != table->table[hash])
                hte->next = table->table[hash];
            table->table[hash] = hte;

            return 0;   // success
        }
        else
        {
            // symbol already in the table
            if(data != NULL)
            {
                free(hte->data);

                if(NULL == (hte->data = calloc(size + 2, 1)))
                    fatal_error("cannot allocate hash table data (%d)", size);
                memcpy(hte->data, data, size);
            }
            else
            {
                if(NULL == (hte->data = calloc(0, 0)))
                    fatal_error("cannot allocate hash table data (%d)", 0);
            }

            return 0;
        }
    }
    else
    {
        return 1;   // table is invalid
    }
}

void* hash_find(hash_tab_t ht, const char* key)
{
    hash_table_t* table = (hash_table_t *) ht;
    hash_table_entry_t* hte;

    if(NULL != table)
    {
        hte = find_local(table, key, make_hash(key) % table->slots);
        if(NULL != hte)
            return hte->data;   // success
        else
            return NULL;    // not found
    }
    else
        return NULL;    // invalid table
}
