#include "functions.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        perror("Invalid Command!\n");
        return 1;
    } else if (strcmp(argv[1], "config") == 0) {
        if (argc > 2) {
            run_config(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }
    } else if (strcmp(argv[1], "init") == 0) {
        if (argc == 2) {
            run_init(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }
    } else if (strcmp(argv[1], "add") == 0) {
        if (argc > 2) {
            run_add(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }
    }
}