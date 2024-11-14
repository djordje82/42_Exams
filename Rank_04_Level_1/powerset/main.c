#include <stdio.h>
#include <stdlib.h>

void print_subset(int *subset, int size) {
    printf("{");
    for (int i = 0; i < size; i++) {
        printf("%d", subset[i]);
        if (i < size - 1) printf(", ");
    }
    printf("}\n");
}

void find_subsets(int *set, int set_size, int target_sum, int *subset, int subset_size, int index, int *found) {
    int sum = 0;
    for (int i = 0; i < subset_size; i++) sum += subset[i];
    if (sum == target_sum) {
        print_subset(subset, subset_size);
        *found = 1;
    }

    for (int i = index; i < set_size; i++) {
        subset[subset_size] = set[i];
        find_subsets(set, set_size, target_sum, subset, subset_size + 1, i + 1, found);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: ./powerset n [s]\n");
        return 1;
    }

    int target_sum = atoi(argv[1]);
    int set_size = argc - 2;
    int *set = malloc(set_size * sizeof(int));
    if (!set) {
        exit(1);
    }

    for (int i = 0; i < set_size; i++) {
        set[i] = atoi(argv[i + 2]);
    }

    int *subset = malloc(set_size * sizeof(int));
    if (!subset) {
        free(set);
        exit(1);
    }

    int found = 0;
    find_subsets(set, set_size, target_sum, subset, 0, 0, &found);

    if (!found) {
        printf("\n");
    }

    free(set);
    free(subset);

    return 0;
}