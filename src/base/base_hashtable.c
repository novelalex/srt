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

#define NB_HASHTABLE_INITIAL_CAPACITY 32

// TODO:(Novel) I should make this use the platform allocator if an arena is not provided

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

internal 
const char* 
nb_hashtable_set_entry(
    HashtableEntry* entries, 
    usize capacity, 
    const char* key,
    void* value,
    usize* length_ptr) 
{
    u64 hash = nb_hashtable_hash_key(key);
    usize index = (usize)(hash & (u64)(capacity - 1));

    while (entries[index].key != null) {
        if (strcmp(key, entries[index].key) == 0) {
            entries[index].value = value;
            return entries[index].key;
        }
        index++;
        if (index >= capacity) {
            index = 0;
        }
    }

    if (length_ptr != null) {
        key = strdup(key);
        if (key == null) {
            return null;
        }
        (*length_ptr)++; 
    }
    entries[index].key = (char*)key;
    entries[index].value = value;
    return key;
}

internal 
b32 nb_hashtable_expand(Hashtable* ht, Arena *arena) {
    usize new_capacity = ht->capacity * 2;
    if (new_capacity < ht->capacity) {
        return false;
    }
    HashtableEntry* new_entries = nb_arena_alloc(arena, sizeof(HashtableEntry) * new_capacity);
    if (new_entries == null) {
        return false;
    }

    for (usize i = 0; i < ht->capacity; i++) {
        HashtableEntry entry = ht->entries[i];
        if(entry.key != null) {
            nb_hashtable_set_entry(new_entries, new_capacity, 
                entry.key, entry.value, null);
        }
    }

    // TODO:(Novel) free old entries when not using an arena
    // if(arena == null) {
    //     nb_aligned_free(arena, old_entries, size)
    // }

    ht->entries = new_entries;
    ht->capacity = new_capacity;
    return true;
}

const char* nb_hashtable_set(Hashtable* ht, Arena* arena, const char* key, void* value) {
    NB_ASSERT(value != null);
    if (value == null) {
        return null;
    }

    if (ht->length >= ht->capacity / 2) {
        if (!nb_hashtable_expand(ht, arena)) {
            return null;
        }
    }

    return nb_hashtable_set_entry(ht->entries, ht->capacity, key, value, &ht->length);
}

usize nb_hashtable_length(Hashtable *ht) {
    return ht->length;
}

HashtableIterator nb_hashtable_iterator(Hashtable *ht) {
    HashtableIterator it;
    it._table = ht;
    it._index = 0;
    return it;
}

b8 nb_hashtable_next(HashtableIterator *it) {
    Hashtable* ht = it->_table;
    while(it->_index < ht->capacity) {
        usize i = it->_index;
        it->_index++;
        if (ht->entries[i].key != null) {
            HashtableEntry entry = ht->entries[i];
            it->key = entry.key;
            it->value = entry.value;
            return true;
        }
    }
    return false;
}
