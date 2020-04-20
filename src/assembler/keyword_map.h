#ifndef __KEYWORD_MAP_H__
#  define __KEYWORD_MAP_H__

typedef struct
{
    token_t token;
    const char* str;         // this algorithm depends on the extra bytes being filled in with 0's
} keyword_map_t;

extern keyword_map_t keyword_map[]; // located in the generated file keyword_map.c
extern const size_t num_keywords;

#endif
