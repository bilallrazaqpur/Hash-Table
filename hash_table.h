#ifndef HASH_TABLE_H
#define HASH_TABLE_H

struct HashTable;

struct HashTable* htCreate(unsigned tableSize);

int htDestroy(struct HashTable** ht);

int htCopy(const struct HashTable* ht, struct HashTable** copy);

int htTableSize(const struct HashTable* ht, unsigned* tableSize);

int htNumElements(const struct HashTable* ht, unsigned* numElements);

int htPrint(const struct HashTable* ht, const void (*printValue)(const void*));

int htInsert(struct HashTable* ht, unsigned key, const void* value);

int htGet(const struct HashTable* ht, unsigned key, const void** value);

int htUpdate(struct HashTable* ht, unsigned key, const void* newValue);

int htRemove(struct HashTable* ht, unsigned key);


#endif  // HASH_TABLE_H
