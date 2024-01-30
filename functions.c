int run_config(int argc, char **argv) {
    bool global = false;
    int starting_argument = 2;
    if (strcmp(argv[2], "-global") == 0) {
        global = true;
        starting_argument = 3;
    }
    if (strncmp(argv[starting_argument], "alias.", 6) == 0) {
        printf("%d\n", global);
    } else {
        if (strcmp(argv[starting_argument], "user.name") == 0) {
            FILE *floc;
            char floc_content[200];
            if ((floc = fopen("/home/mahmostash/Desktop/BP_Project", "w")) == NULL) {
                perror("File could not be opened!\n");
                return 1;
            }
            fscanf(floc, "%[^\0]s", floc_content);
            if (strlen(floc_content) == 0)
            {
                printf("yooho");
            }
        } else if (strcmp(argv[starting_argument], "user.email") == 0) {

        } else {
            perror("Invalid Command!\n");
            return 1;
        }
    }
}