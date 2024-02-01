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
            FILE *addlog = fopen(Addlog, "a");
            fprintf(addlog, "\n");
            fclose(addlog);
        } else {
            printf("Invalid Command!\n");
        }
    } else if (strcmp(argv[1], "reset") == 0) {
        if (argc > 2) {
            run_reset(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }
    } else if (strcmp(argv[1], "status") == 0) {
        if (argc == 2) {
            run_status(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }
    } else if (strcmp(argv[1], "commit") == 0) {
        if (argc == 4) {
            run_commit(argc, argv);
        } else if ((argc == 3) && strcmp(argv[2], "-m") == 0){
            printf("You can not commit without a commit message!\n");
        }
        else {
            printf("Invalid Command!\n");
        }

    }else if (strcmp(argv[1], "branch") == 0) {
        if (argc < 4) {
            run_branch(argc, argv);
        }
        else {
            printf("Invalid Command!\n");
        }

    }
}