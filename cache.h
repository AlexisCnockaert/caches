// Structure pour une ligne du cache
typedef struct {
    int tag;
    int valid;
    int counter; // Compteur LRU
} CacheLine;

// Structure pour le cache
typedef struct {
    CacheLine *lines;
    int num_lines;
} Cache;

Cache* initializeCache(int num_lines);
void freeCache(Cache* cache);
int accessCacheDirectMapping(Cache* cache, int address, int num_sets);
int accessCacheFullyAssociative(Cache* cache, int address);
int accessCacheSetAssociative(Cache* cache, int address, int num_sets);
int fileExists(const char *filename);
void Gen_Input_File(int cases);