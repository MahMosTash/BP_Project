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
            if(run_add(argc, argv) != 5) {
                FILE *addlog = fopen(Addlog, "a");
                fprintf(addlog, "\n");
                fclose(addlog);
            }
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
        } else if ((argc == 3) && strcmp(argv[2], "-m") == 0) {
            printf("You can not commit without a commit message!\n");
        } else {
            printf("Invalid Command!\n");
        }
    } else if (strcmp(argv[1], "log") == 0) {
        if (argc < 5) {
            run_log(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }

    } else if (strcmp(argv[1], "branch") == 0) {
        if (argc < 4) {
            run_branch(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }
    } else if (strcmp(argv[1], "checkout") == 0) {
        if (argc == 3) {
            run_checkout(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }
    } else if ((strcmp(argv[1], "set") == 0) || (strcmp(argv[1], "replace") == 0) || (strcmp(argv[1], "remove") == 0)) {
        if (argc < 7) {
            run_shortcut(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }
    }else if (strcmp(argv[1], "revert") == 0) {
        if (argc < 6) {
            run_revert(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }
    }else if (strcmp(argv[1], "grep") == 0) {
        if (argc < 10) {
            run_grep(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }
    }else if (strcmp(argv[1], "tag") == 0) {
        if (argc < 10) {
            run_tag(argc, argv);
        } else {
            printf("Invalid Command!\n");
        }
    }
}