#ifndef __SECTIONS_H__
#  define __SECTIONS_H__

enum
{
    TYPE_INT8,
    TYPE_INT16,
    TYPE_INT32,
    TYPE_INT64,
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT32,
    TYPE_UINT64,
    TYPE_FLOAT,
    SEC_TYPE_DATA,
    SEC_TYPE_CODE,
};

/*
 * Opaque handle for sections
 */
typedef void* section_t;

void add_section(const char* name, int type);
void destroy_all_sections(void);
void add_section_entry(const char* sec_name, const char* name);
void add_entry_bytes(const char* sec_name, const char* ent_name, void* bytes, size_t size);

#endif
