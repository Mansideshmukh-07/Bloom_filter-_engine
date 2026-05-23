#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned int hash1(char *str) {

    unsigned int hash = 0;

    while (*str) {
        hash = hash * 31 + *str;
        str++;
    }

    return hash;
}

unsigned int hash2(char *str) {

    unsigned int hash = 7;

    while (*str) {
        hash = hash * 37 + *str;
        str++;
    }

    return hash;
}

void set_bit(unsigned char *bit_array, int index) {

    int byte = index / 8;
    int position = index % 8;

    bit_array[byte] |= (1 << position);
}

int check_bit(unsigned char *bit_array, int index) {

    int byte = index / 8;
    int position = index % 8;

    return bit_array[byte] & (1 << position);
}

void insert(unsigned char *bit_array,
            char *str,
            int m) {

    int h1 = hash1(str) % m;
    int h2 = hash2(str) % m;

    set_bit(bit_array, h1);
    set_bit(bit_array, h2);
}

int contains(unsigned char *bit_array,
             char *str,
             int m) {

    int h1 = hash1(str) % m;
    int h2 = hash2(str) % m;

    if (!check_bit(bit_array, h1))
        return 0;

    if (!check_bit(bit_array, h2))
        return 0;

    return 1;
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Usage: %s <n> <false_positive_rate>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);      // expected number of elements
    float p = atof(argv[2]);    // false positive rate

    // size of bit array in bits
    int m = (int)(-n * log(p) / (log(2) * log(2)));

    // convert bits -> bytes
    int bytes = (m + 7) / 8;

    printf("Bits required  = %d\n", m);
    printf("Bytes required = %d\n", bytes);

    // allocate bit array
    unsigned char *bit_array;

    bit_array = calloc(bytes, sizeof(unsigned char));

    if (bit_array == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // optimal number of hash functions
    int k = round((m / (double)n) * log(2));

    printf("Hash functions = %d\n", k);

    while (1) {

        int choice;

        printf("\n1. Insert\n");
        printf("2. Lookup\n");
        printf("3. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1: {

                char str[100];

                printf("Enter string: ");
                scanf("%s", str);

                insert(bit_array, str, m);

                printf("Inserted successfully\n");

                break;
            }

            case 2: {

                char str[100];

                printf("Enter string: ");
                scanf("%s", str);

                if (contains(bit_array, str, m))
                    printf("Possibly Present\n");
                else
                    printf("Definitely Not Present\n");

                break;
            }

            case 3:

                free(bit_array);

                printf("Exiting...\n");

                return 0;

            default:

                printf("Invalid choice\n");
        }
    }

    return 0;
}