#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <stdint.h>
#include "murmur3.h"
typedef enum {

    MURMUR3,
    XXHASH,
    FNV1A

} HashType;

typedef struct {

    unsigned char *bit_array;

    size_t m;                    // total bits
    size_t bytes;                // total bytes
    size_t k;                    // number of hash functions

    size_t expected_n;           // expected capacity
    size_t inserted_n;           // actual inserted elements

    size_t set_bits_count;       // currently set bits
    size_t collision_count;      // repeated bit-set attempts

    float p;                  // false positive probability

    uint32_t seed;            // hashing seed

    HashType hash_type;       // hashing algorithm

} BloomFilter;
void bloom_insert(BloomFilter *bf, char *word);

void set_bit(BloomFilter *bf, int index);

int bloom_contains(BloomFilter *bf, char *word);

void bloom_insert_file(BloomFilter *bf,
                       const char *filename);

void bloom_lookup_file(BloomFilter *bf,
                       const char *input_file,
                       const char *output_file);

void bloom_print_stats(BloomFilter *bf);

BloomFilter *bloom_create(int expected_n,
                          float p,
                          uint32_t seed,
                          HashType hash_type) 
{

    /*
        allocate bloom filter structure
    */

    BloomFilter *bf =
        (BloomFilter *)malloc(sizeof(BloomFilter));

    if (bf == NULL) {

        printf("BloomFilter allocation failed\n");

        return NULL;
    }

    /*
        initialize configuration
    */

    bf->expected_n = expected_n;

    bf->inserted_n = 0;

    bf->p = p;

    bf->seed = seed;

    bf->hash_type = hash_type;

    bf->set_bits_count = 0;

    bf->collision_count = 0;

    /*
        calculate total bits
    */

    bf->m =
        (size_t)(-expected_n * log(p) /
              (log(2) * log(2)));

    /*
        calculate total bytes
    */

    bf->bytes = (bf->m + 7) / 8;

    /*
        calculate optimal hash functions
    */

    bf->k =
        round((bf->m / (double)expected_n)
               * log(2));

    /*
        allocate bit array
    */

    bf->bit_array =
        (unsigned char *)
        calloc(bf->bytes,
               sizeof(unsigned char));

    if (bf->bit_array == NULL) {

        printf("Bit array allocation failed\n");

        free(bf);

        return NULL;
    }

    /*
        diagnostics
    */

    printf("Bloom Filter Created\n");

    printf("Bits           : %ld\n", bf->m);

    printf("Bytes          : %ld\n", bf->bytes);

    printf("Hash Functions : %ld\n", bf->k);

    return bf;
}

void bloom_load_dataset(BloomFilter *bf,FILE *fp)
{

    char word[256];

    while (fgets(word,
                 sizeof(word),
                 fp)) {

        /*
            remove newline
        */

        word[strcspn(word, "\n")] = '\0';

        /*
            skip empty lines
        */

        if (word[0] == '\0')
            continue;

        /*
            insert into bloom filter
        */

        bloom_insert(bf, word);
    }

    printf("Dataset Loaded Successfully\n");

    printf("Inserted Elements : %ld\n",
           bf->inserted_n);

    printf("Set Bits          : %ld\n",
           bf->set_bits_count);

    printf("Bit Collisions    : %ld\n",
           bf->collision_count);
}

void bloom_insert(BloomFilter *bf,char *word) 
{

    uint32_t h1;
    uint32_t h2;

    /*
        generate two base hashes
    */

    MurmurHash3_x86_32(word,
                       strlen(word),
                       bf->seed,
                       &h1);

    MurmurHash3_x86_32(word,
                       strlen(word),
                       bf->seed + 1,
                       &h2);

    /*
        generate k hashes using double hashing
    */

    for (int i = 0; i < bf->k; i++) {

        uint32_t combined_hash =
            h1 + i * h2;

        size_t index =
            combined_hash % bf->m;

        set_bit(bf, index);
    }

    bf->inserted_n++;
}
void set_bit(BloomFilter *bf,int index)
{

    size_t byte =
        index / 8;

    size_t position =
        index % 8;

    unsigned char mask =
        (1 << position);

    /*
        bit already set
    */

    if (bf->bit_array[byte] & mask) {

        bf->collision_count++;
    }
    else {

        /*
            new bit set
        */

        bf->bit_array[byte] |= mask;

        bf->set_bits_count++;
    }
}

void bloom_interactive_mode(BloomFilter *bf)
{

    int choice;

    char word[256];

    while (1) {

        printf("\n1. Insert\n");
        printf("2. Lookup\n");
        printf("3. Stats\n");
        printf("4. Insert File\n");
        printf("5. Lookup File\n");
        printf("6. Exit\n");

        printf("Enter choice: ");

        scanf("%d", &choice);

        switch(choice) {

            case 1:

                printf("Enter word: ");

                scanf("%255s", word);

                bloom_insert(bf, word);

                printf("Inserted\n");

                break;

            case 2:

                printf("Enter word: ");

                scanf("%255s", word);

                if (bloom_contains(bf, word))
                    printf("Possibly Present\n");
                else
                    printf("Definitely Not Present\n");

                break;

            case 3:
                bloom_print_stats(bf);
                

                break;
            case 4:
                
                    char filename[256];

                    printf("Enter insert file: ");

                    scanf("%255s", filename);

                    bloom_insert_file(bf, filename);

                    break;
            case 5:
                
                    char input[256];

                    char output[256];

                    printf("Enter lookup input file: ");

                    scanf("%255s", input);

                    printf("Enter output result file: ");

                    scanf("%255s", output);

                    bloom_lookup_file(bf,
                                    input,
                                    output);

                    break;
                
            case 6:
                
                return;
                

            default:

                printf("Invalid Choice\n");
        }
    }
}

void bloom_destroy(BloomFilter *bf) 
{

    if (bf == NULL)
        return;

    free(bf->bit_array);

    free(bf);
}

int bloom_save(BloomFilter *bf,const char *filename) 
{

    FILE *fp = fopen(filename, "wb");

    if (fp == NULL) {

        printf("Unable to open file for writing\n");

        return 0;
    }

    /*
        save metadata
    */

    fwrite(&bf->m,
           sizeof(size_t),
           1,
           fp);

    fwrite(&bf->bytes,
           sizeof(size_t),
           1,
           fp);

    fwrite(&bf->k,
           sizeof(size_t),
           1,
           fp);

    fwrite(&bf->expected_n,
           sizeof(size_t),
           1,
           fp);

    fwrite(&bf->inserted_n,
           sizeof(size_t),
           1,
           fp);

    fwrite(&bf->set_bits_count,
           sizeof(size_t),
           1,
           fp);

    fwrite(&bf->collision_count,
           sizeof(size_t),
           1,
           fp);

    fwrite(&bf->p,
           sizeof(float),
           1,
           fp);

    fwrite(&bf->seed,
           sizeof(uint32_t),
           1,
           fp);

    fwrite(&bf->hash_type,
           sizeof(HashType),
           1,
           fp);

    /*
        save bit array
    */

    fwrite(bf->bit_array,
           sizeof(unsigned char),
           bf->bytes,
           fp);

    fclose(fp);

    return 1;
}

BloomFilter *bloom_load(const char *filename) 
{

    FILE *fp = fopen(filename, "rb");

    if (fp == NULL) {

        printf("Unable to open bloom file\n");

        return NULL;
    }

    /*
        allocate bloom filter structure
    */

    BloomFilter *bf =
        (BloomFilter *)malloc(sizeof(BloomFilter));

    if (bf == NULL) {

        printf("BloomFilter allocation failed\n");

        fclose(fp);

        return NULL;
    }

    /*
        load metadata
    */

    fread(&bf->m,
          sizeof(size_t),
          1,
          fp);

    fread(&bf->bytes,
          sizeof(size_t),
          1,
          fp);

    fread(&bf->k,
          sizeof(size_t),
          1,
          fp);

    fread(&bf->expected_n,
          sizeof(size_t),
          1,
          fp);

    fread(&bf->inserted_n,
          sizeof(size_t),
          1,
          fp);

    fread(&bf->set_bits_count,
          sizeof(size_t),
          1,
          fp);

    fread(&bf->collision_count,
          sizeof(size_t),
          1,
          fp);

    fread(&bf->p,
          sizeof(float),
          1,
          fp);

    fread(&bf->seed,
          sizeof(uint32_t),
          1,
          fp);

    fread(&bf->hash_type,
          sizeof(HashType),
          1,
          fp);

    /*
        allocate bit array
    */

    bf->bit_array =
        (unsigned char *)
        malloc(bf->bytes);

    if (bf->bit_array == NULL) {

        printf("Bit array allocation failed\n");

        free(bf);

        fclose(fp);

        return NULL;
    }

    /*
        load bit array
    */

    fread(bf->bit_array,
          sizeof(unsigned char),
          bf->bytes,
          fp);

    fclose(fp);

    printf("Bloom Filter Loaded Successfully\n");

    printf("Bits           : %ld\n", bf->m);

    printf("Bytes          : %ld\n", bf->bytes);

    printf("Hash Functions : %ld\n", bf->k);

    return bf;
}
int bloom_contains(BloomFilter *bf,
                   char *word)
{

    uint32_t h1;
    uint32_t h2;

    MurmurHash3_x86_32(word,
                       strlen(word),
                       bf->seed,
                       &h1);

    MurmurHash3_x86_32(word,
                       strlen(word),
                       bf->seed + 1,
                       &h2);

    for (int i = 0; i < bf->k; i++) {

        uint32_t combined_hash =
            h1 + i * h2;

        size_t index =
            combined_hash % bf->m;

        size_t byte = index / 8;

        size_t position = index % 8;

        unsigned char mask =
            (1 << position);

        if (!(bf->bit_array[byte] & mask))
            return 0;
    }

    return 1;
}
float bloom_load_factor(BloomFilter *bf)
{
    return (float)bf->set_bits_count / bf->m;
}

void bloom_print_stats(BloomFilter *bf)
{
    printf("\n************** BLOOM FILTER STATS ************\n");

    printf("Expected Elements : %zu\n", bf->expected_n);
    printf("Inserted Elements : %zu\n", bf->inserted_n);

    printf("Total Bits        : %zu\n", bf->m);
    printf("Total Bytes       : %zu\n", bf->bytes);

    printf("Hash Functions    : %zu\n", bf->k);

    printf("Set Bits          : %zu\n", bf->set_bits_count);
    printf("Collision Count   : %zu\n", bf->collision_count);

    printf("Load Factor       : %.6f\n",
           bloom_load_factor(bf));

    printf("False Positive p  : %.6f\n", bf->p);

    printf("************************************************\n\n");
}

void bloom_insert_file(BloomFilter *bf,
                       const char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {

        printf("Unable to open insert file\n");

        return;
    }

    char word[256];

    size_t inserted = 0;

    while (fgets(word,
                 sizeof(word),
                 fp)) {

        /*
            remove newline
        */

        word[strcspn(word, "\n")] = '\0';

        /*
            skip empty
        */

        if (word[0] == '\0')
            continue;

        bloom_insert(bf, word);

        inserted++;
    }

    fclose(fp);

    printf("Batch Insert Completed\n");

    printf("Inserted Words : %ld\n",
           inserted);
}

void bloom_lookup_file(BloomFilter *bf,
                       const char *input_file,
                       const char *output_file)
{
    FILE *fin = fopen(input_file, "r");

    if (fin == NULL) {

        printf("Unable to open lookup file\n");

        return;
    }

    FILE *fout = fopen(output_file, "w");

    if (fout == NULL) {

        printf("Unable to open output file\n");

        fclose(fin);

        return;
    }

    char word[256];

    size_t present = 0;

    size_t absent = 0;

    while (fgets(word,
                 sizeof(word),
                 fin)) {

        /*
            remove newline
        */

        word[strcspn(word, "\n")] = '\0';

        if (word[0] == '\0')
            continue;

        if (bloom_contains(bf, word)) {

            fprintf(fout,
                    "%s Possibly Present\n",
                    word);

            present++;
        }
        else {

            fprintf(fout,
                    "%s Definitely Not Present\n",
                    word);

            absent++;
        }
    }

    fclose(fin);

    fclose(fout);

    printf("Lookup Completed\n");

    printf("Present     : %ld\n",
           present);

    printf("Not Present : %ld\n",
           absent);
}

int main(int argc, char *argv[]) {

    BloomFilter *bf = NULL;

    /*
        CREATE MODE
    */

    if (strcmp(argv[1], "create") == 0) {

        if (argc < 6) {

            printf("Usage:\n");

            printf("./a.out create <dataset> <expected_n> <fp_rate> <output.bloom>\n");

            return 1;
        }

        char *filename = argv[2];

        size_t expected_n = atoi(argv[3]);

        float p = atof(argv[4]);

        char *bloom_file = argv[5];

        /*
            validation
        */

        if (expected_n <= 0) {

            printf("Invalid expected_n\n");

            return 1;
        }

        if (p <= 0 || p >= 1) {

            printf("False positive rate must be between 0 and 1\n");

            return 1;
        }

        FILE *fp = fopen(filename, "r");

        if (fp == NULL) {

            printf("Unable to open dataset file\n");

            return 1;
        }

        /*
            create bloom filter
        */

        bf = bloom_create(expected_n,
                          p,
                          42,
                          MURMUR3);

        if (bf == NULL) {

            fclose(fp);

            return 1;
        }

        /*
            load dataset
        */

        bloom_load_dataset(bf, fp);

        fclose(fp);

        /*
            save bloom filter
        */

        if (bloom_save(bf, bloom_file))
            printf("Bloom Filter Saved Successfully\n");
        else
            printf("Bloom Filter Save Failed\n");
    }

    /*
        LOAD MODE
    */

    else if (strcmp(argv[1], "load") == 0) {

        if (argc < 3) {

            printf("Usage:\n");

            printf("./a.out load <filter.bloom>\n");

            return 1;
        }

        char *bloom_file = argv[2];

        bf = bloom_load(bloom_file);

        if (bf == NULL)
            return 1;
    }

    else {

        printf("Invalid mode\n");

        return 1;
    }

    /*
        interactive mode
    */

    bloom_interactive_mode(bf);

    /*
        cleanup
    */

    bloom_destroy(bf);

    return 0;
}
