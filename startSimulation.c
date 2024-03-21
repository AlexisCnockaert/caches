#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "cache.c"

#define INPUT_FILE "input.txt"
#define NB_CASES_RAM 1024
#define CACHE_SIZE 256
#define CACHE_MISS -1
#define CACHE_HIT 1


int main() {
    srand(time(NULL));

    // Initialisation des paramètres
    int word_size = 4; // Taille d'un mot en octets
    int num_lines_cache = 16; // Nombre de lignes du cache
    int cache_access_time = 1; // Temps d'accès au cache en nombre de cycles
    int ram_access_time = 50; // Temps d'accès à la RAM en nombre de cycles

   if(!fileExists(INPUT_FILE)){
        remove(INPUT_FILE);
   }
    Gen_Input_File(NB_CASES_RAM);
    printf("Fichier d'entree genere avec succes.\n");

    // Initialisation du cache

     // Lecture du fichier d'entrée
    FILE *input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    int address;
    
    int num_hits = 0;
    int num_misses = 0;

     // Simulation du cache selon le type choisi
    int choice;
    printf("Choose the type of cache you want to simulate ^^ :\n");
    printf("1. Direct Mapping\n");
    printf("2. Fully Associative\n");
    printf("3. Set Associative\n");
    printf("Your choice is : ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        // Direct Mapping : aucun paramètre à fixer
        break;
    case 2:
        // Fully Associative : aucun paramètre à fixer
        break;
    case 3:
        // Associatif par Ensembles : demander à l'utilisateur le nombre de lignes par ensemble
        printf("Entrez le nombre de lignes par ensemble : ");
        scanf("%d", &num_lines_cache);
        break;
    default:
        printf("Choix invalide.\n");
        return 1;
    }

    Cache* cache = initializeCache(num_lines_cache);
    
    switch (choice) {
        case 1:
            // Simulation du cache Direct Mapping
            while (fscanf(input_file, "%d", &address) == 1) {
                int result = accessCacheDirectMapping(cache, address, num_lines_cache);
                if (result == CACHE_HIT)
                    num_hits++;
                else {
                    num_misses++;
                    // Mettre à jour le cache en cas de miss
                    int set = address % num_lines_cache;
                    cache->lines[set].tag = address;
                    cache->lines[set].valid = 1;
                    //LRU A IMPLEMENTER
                }
            }
            break;
        case 2:
            // Simulation du cache Fully Associative
            while (fscanf(input_file, "%d", &address) == 1) {
                int result = accessCacheFullyAssociative(cache, address);
                if (result == CACHE_HIT)
                    num_hits++;
                else {
                    num_misses++;
                    // Mettre à jour le cache en cas de miss
                    // ...
                }
            }
            break;
        case 3:
            // Simulation du cache Set Associative
            int num_sets = num_lines_cache; // Pour cette simulation, nous considérons que le nombre d'ensembles est égal au nombre de lignes du cache
            while (fscanf(input_file, "%d", &address) == 1) {
                int result = accessCacheSetAssociative(cache, address, num_sets);
                if (result == CACHE_HIT)
                    num_hits++;
                else {
                    num_misses++;
                    // Mettre à jour le cache en cas de miss
                    // ...
                }
            }
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }

    fclose(input_file);

    // Calcul des métriques
    int total_access_time = num_hits * cache_access_time + num_misses * ram_access_time;
    double success_rate = (double)num_hits / (num_hits + num_misses);
    double average_access_time = (double)total_access_time / (num_hits + num_misses);
    double speedup = (double)total_access_time / (num_hits * ram_access_time);

    // Affichage des résultats
    printf("Taux de succes : %.2lf\n", success_rate);
    printf("Temps global d'acces : %d cycles\n", total_access_time);
    printf("Temps moyen d'acces a une donnee : %.2lf cycles\n", average_access_time);
    printf("Speedup : %.2lf\n", speedup);

    // Libération de la mémoire allouée au cache
    freeCache(cache);

    return 0;
}