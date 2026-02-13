// Implementation from https://benhoyt.com/writings/hash-table-in-c/

#include "base.h"
#include <string.h>

#define NB_HASHTABLE_FNV_OFFSET 14695981039346656037UL
#define NB_HASHTABLE_FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
internal u64 nb_hashtable_hash_key(const char* key) {
    u64 hash = NB_HASHTABLE_FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (u64)(unsigned char)(*p);
        hash *= NB_HASHTABLE_FNV_PRIME;
    }
    return hash;
}

typedef struct HashtableEntry {
    const char* key;
    void* value;
} HashtableEntry;

struct Hashtable {
    HashtableEntry* entries;
    usize capacity;
    usize length;
};

#define NB_HASHTABLE_INITIAL_CAPACITY 16

Hashtable* nb_hashtable_create(Arena* arena) {
    Hashtable* ht = nb_arena_alloc(arena, sizeof(Hashtable));
    if (ht == null) {
        return null;
    }
    ht->length = 0;
    ht->capacity = NB_HASHTABLE_INITIAL_CAPACITY;

    ht->entries = nb_arena_alloc(arena, sizeof(HashtableEntry) * ht->capacity);
    memset(ht->entries, 0, sizeof(HashtableEntry) * ht->capacity);
    if (ht->entries == null) {
        return null;
    }
    return ht;
}

void* nb_hashtable_get(Hashtable* ht, const char* key) {
    u64 hash = nb_hashtable_hash_key(key);
    usize index = (usize)(hash & (u64)(ht->capacity - 1));

    while (ht->entries[index].key != null) {
        if(strcmp(key, ht->entries[index].key) == 0) {
            return ht->entries[index].value;
        }
        index++;
        if (index >= ht->capacity) {
            index = 0;
        }
    }
    return null;
}

const char* nb_hashtable_set(Hashtable* ht, Arena* arena, const char* key, void* value) {
    NB_ASSERT(value != null);
    if (value == null) {
        return null;
    }

    if (ht->length >= ht->capacity / 2.0) {
        
    }
}

usize       nb_hashtable_length(Hashtable *ht);