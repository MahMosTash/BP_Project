#include "functions.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        perror("Invalid Command!\n");
        return 1;
    } else if (strcmp(argv[1], "config") == 0) {
        if (argc > 2) {
            run_config(argc, argv);
            printf("done\n");
        } else {
            printf("Invalid Command!\n");
        }
    }
}