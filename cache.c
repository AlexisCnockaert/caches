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
    int8_t tag_size = getTagAddress(cache);
    int address_tag = address >> tag_size;
     for (int i = 0; i < cache->num_lines; i++) {
        if (cache->lines[i].tag == address_tag) {
            // L'adresse est déjà présente dans le cache (hit)
            // Mettre à jour le compteur de toutes les lignes du cache pour indiquer leur utilisation la plus récente
            for (int j = 0; j < cache->num_lines; j++) {
                if (j != i) {
                    cache->lines[j].counter++;
                }
            }
            return CACHE_HIT;
        }
    }
    return CACHE_MISS;
}

// Fonction pour accéder au cache (Set Associative)
int accessCacheSetAssociative(Cache* cache, int address, int num_sets) {
    int set = address % num_sets;
    int8_t tag_size = getTagAddress(cache);
    int address_tag = address >> tag_size;
    for (int i = 0; i < num_sets; i++) {
        int index = set * num_sets + i;
        if (cache->lines[index].valid && cache->lines[index].tag == address_tag)
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

void updateCacheMissFullyAssociative(Cache *cache, int address){
    int least_recently_used_index = 0;
    int least_recently_used_counter = cache->lines[0].counter;
    int8_t tag_size = getTagAddress(cache);
    int address_tag = address >> tag_size;
    for (int i = 1; i < cache->num_lines; i++) {
        if (cache->lines[i].counter > least_recently_used_counter) {
            least_recently_used_index = i;
            least_recently_used_counter = cache->lines[i].counter;
        }
    }
    cache->lines[least_recently_used_index].tag = address_tag;
    cache->lines[least_recently_used_index].counter = 0;
    for (int i = 0; i < cache->num_lines; i++) {
    if (i != least_recently_used_index) {
        cache->lines[i].counter++;
    }
}
   
}

void updateCacheMissSetAssociative(Cache *cache, int address, int num_sets){
    int set_index = address % num_sets;
    int least_recently_used_index = set_index * num_sets;
    int least_recently_used_counter = cache->lines[least_recently_used_index].counter;
    for (int i = set_index * num_sets + 1; i < (set_index + 1) * num_sets; i++) {
        if (cache->lines[i].counter > least_recently_used_counter) {
            least_recently_used_index = i;
            least_recently_used_counter = cache->lines[i].counter;
        }
    }
    int8_t tag_size = getTagAddress(cache);
    int address_tag = address >> tag_size;
    cache->lines[least_recently_used_index].tag = address_tag;
    cache->lines[least_recently_used_index].valid = 1;
    cache->lines[least_recently_used_index].counter = 0;
    for (int i = set_index * num_sets; i < (set_index + 1) * num_sets; i++) {
    if (i != least_recently_used_index) {
        cache->lines[i].counter++;
    }
    }
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

