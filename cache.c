#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include "cache.h"
#include "func.c"


#define CACHE_MISS -1
#define CACHE_HIT 1

// Fonction pour initialiser le cache
Cache* initializeCache(int num_lines, int word_size) {
    Cache* cache = (Cache*)malloc(sizeof(Cache));
    cache->num_lines = num_lines;
    cache->lines = (CacheLine*)malloc(cache->num_lines * sizeof(CacheLine));
    cache->word_size = word_size;
    for (int i = 0; i < num_lines; i++) {
        cache->lines[i].tag = -1;
        cache->lines[i].valid = 0;
        cache->lines[i].counter = 0;
    }
    return cache;
}

// Fonction pour libérer la mémoire allouée au cache
void freeCache(Cache* cache) {
    free(cache->lines);
    free(cache);
}

int8_t getTagAddress(Cache *cache){ //only for mapping here
    int offset = countPowersOfTwo(cache->word_size*8);
    return (cache->word_size *8) - countPowersOfTwo(cache->num_lines)- offset;
}

// Fonction pour accéder au cache (Direct Mapping)
int accessCacheDirectMapping(Cache* cache, int address, int num_sets) {
    int set = address % num_sets;
    int8_t tag_size = getTagAddress(cache);
    int address_tag = address >> tag_size;
    if (cache->lines[set].valid && cache->lines[set].tag == address_tag)
        return CACHE_HIT;
    return CACHE_MISS;
}

// Fonction pour accéder au cache (Fully Associative)
int accessCacheFullyAssociative(Cache* cache, int address) {
    for (int i = 0; i < cache->num_lines; i++) {
        if (cache->lines[i].valid && cache->lines[i].tag == address)
            return CACHE_HIT;
    }
    return CACHE_MISS;
}

// Fonction pour accéder au cache (Set Associative)
int accessCacheSetAssociative(Cache* cache, int address, int num_sets) {
    int set = address % num_sets;
    for (int i = 0; i < num_sets; i++) {
        int index = set * num_sets + i;
        if (cache->lines[index].valid && cache->lines[index].tag == address)
            return CACHE_HIT;
    }
    return CACHE_MISS;
}

void updateCacheMissDirectMapping(Cache *cache, int address, int num_sets){
    int set = address % num_sets;
    int8_t tag_size = getTagAddress(cache);
    int address_tag = address >> tag_size;
    cache->lines[set].tag = address_tag;
    cache->lines[set].valid = 1;      
}

int fileExists(const char *filename) {
    return access(filename, F_OK) != -1;
}



void Gen_Input_File(int cases) {
    FILE *output_file = fopen("input.txt", "w");
    if (output_file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    // Boucle pour générer les adresses aléatoires
    for (int i = 0; i < 10000; i++) {
        int address = rand() % cases; // Génération d'une adresse aléatoire entre 0 et NB_cases_RAM - 1
        fprintf(output_file, "%d\n", address); // Écriture de l'adresse dans le fichier
    }

    fclose(output_file);
}

