#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkPrime(unsigned tableSize) 
{
	for (int i = 2; i <= tableSize/2; i++) {
		if (tableSize % i == 0)
			return 0;
	}
	return 1;
}

struct HashTable
{
	unsigned tableSize; 
	unsigned numElements;
	unsigned* key;
	void** value;
};

struct HashTable* htCreate(unsigned tableSize)
{
    if (tableSize == 0 || checkPrime(tableSize) == 0)
		return NULL;
	struct HashTable* ht = malloc(sizeof(struct HashTable)); 
	ht->tableSize = tableSize;
	ht->numElements = 0;
	ht->key = malloc(sizeof(unsigned) * tableSize);
	for (int i = 0; i < tableSize; i++) 
		ht->key[i] = 0;
	ht->value = malloc(sizeof(void*) * tableSize);
	for (int i = 0; i < tableSize; i++) 
		ht->value[i] = '\0';
    return ht;
}

int htRehash(struct HashTable** ht)
{
	unsigned m = (*ht)->tableSize * 2 + 1;
	while (checkPrime(m) == 0)
		m++;
	struct HashTable* rehash = htCreate(m);
	for (int i = 0; i < (*ht)->tableSize; i++) {
		htInsert(rehash, (*ht)->key[i], (*ht)->value[i]);
	}
	free((*ht)->key);
	free((*ht)->value);
	(*ht)->key = rehash->key;
	(*ht)->value = rehash->value;
	(*ht)->tableSize = m;
	free(rehash);
	return 1;
}

int htFindKey(const struct HashTable* ht, unsigned key)
{
	int index = key % ht->tableSize;
	for (int i = 1; i < ht->tableSize; i++) {
		if (ht->key[index] == key) {
			return 1; //Key is in table
		}
		index = (key + i * i) % ht->tableSize; //Quadratic probe to search faster
	}
	return 0;
}

int htDestroy(struct HashTable** ht)
{
    if (!ht)
		return 0;
	free((*ht)->key);
	(*ht)->key = NULL;
	free((*ht)->value);
	(*ht)->value = NULL;
	free(*ht); 
	*ht = NULL;
    return 1;
}

int htCopy(const struct HashTable* ht, struct HashTable** copy)
{
    if (!ht) 
		return 0;
	*copy = htCreate(ht->tableSize);
	(*copy)->tableSize = ht->tableSize;
	(*copy)->numElements = ht->numElements;
	for (int i = 0; i < ht->tableSize; i++) 
		(*copy)->key[i] = ht->key[i];
	for (int i = 0; i < ht->tableSize; i++) 
		(*copy)->value[i] = ht->value[i]; 
    return 1;
}

int htTableSize(const struct HashTable* ht, unsigned* tableSize)
{
    if (!ht || !tableSize)
		return 0;
	*tableSize = ht->tableSize;
    return 1;
}

int htNumElements(const struct HashTable* ht, unsigned* numElements)
{
    if (!ht || !numElements)
		return 0;
	*numElements = ht->numElements;
    return 1;
}

int htPrint(const struct HashTable* ht, const void (*printValue)(const void*))
{
    if (!ht || !printValue)
		return 0;
	for (int i = 0; i < ht->tableSize; i++) {
		printf("Bucket %d: ", i);
		if (ht->key[i] == 0) {
			printf("(empty)\n");
			continue;
		}
		printf("%d -> ", ht->key[i]);
		printValue(ht->value[i]); 
		printf("\n");
	}
    return 1;
}

int htInsert(struct HashTable* ht, unsigned key, const void* value)
{
    if (!ht || !value || htFindKey(ht, key) == 1) 
		return 0;
	float load = ((float) ht->numElements + 1) / ((float) ht->tableSize);
	if (load >= 0.5) 
		htRehash(&ht);
	int index = key % ht->tableSize;
	for (int i = 1; i < ht->tableSize; i++) {
		if (ht->key[index] == 0) {
			break; //Found empty spot in table
		}
		index = (key + i * i) % ht->tableSize; //Quadratic probe to try to find empty spot.
	}
	ht->key[index] = key;
	ht->value[index] = (void*)value;
	ht->numElements++;
    return 1;
}

int htGet(const struct HashTable* ht, unsigned key, const void** value)
{
    if (!ht || htFindKey(ht, key) == 0) 
		return 0;
	int index = key % ht->tableSize;
	for (int i = 1; i < ht->tableSize; i++) {
		if (ht->key[index] == key) 
			break; //Key index found.
		index = (key + i * i) % ht->tableSize; //Quadratic probe to find key index.
	}
	*value = ht->value[index];
    return 1;
}

int htUpdate(struct HashTable* ht, unsigned key, const void* newValue)
{
    if (!ht || htFindKey(ht, key) == 0)  
		return 0;
	int index = key % ht->tableSize;
	for (int i = 1; i < ht->tableSize; i++) {
		if (ht->key[index] == key) 
			break; //Key index found.
		index = (key + i * i) % ht->tableSize; //Quadratic probe to find key index.
	}
	ht->value[index] = (void*)newValue;
    return 1;
}

int htRemove(struct HashTable* ht, unsigned key)
{
    if (!ht || htFindKey(ht, key) == 0)  
		return 0;
	int index = key % ht->tableSize;
	for (int i = 1; i < ht->tableSize; i++) {
		if (ht->key[index] == key) 
			break; //Key index found.
		index = (key + i * i) % ht->tableSize; //Quadratic probe to find key index.
	}
	ht->key[index] = 0;
	ht->value[index] = NULL;
	ht->numElements--;
    return 1;
}

