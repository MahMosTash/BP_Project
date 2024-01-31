#define GlobalName "/home/mahmostash/Desktop/BP_Project/user/globalname.txt"
#define GlobalEmail "/home/mahmostash/Desktop/BP_Project/user/globalemail.txt"
#define MAX_FILE_SIZE 5000
#define MAX_NAME_SIZE 100

int run_config(int argc, char **argv) {
    bool global = false;
    int starting_argument = 2;
    if (strcmp(argv[2], "-global") == 0) {
        global = true;
        starting_argument = 3;
    }
    if (argc == starting_argument) {
        perror("Invalid Command!");
        return 1;
    }
    if (strncmp(argv[starting_argument], "alias.", 6) == 0) {
        printf("%d\n", global);
    } else if (argc == (starting_argument + 2)) {
        FILE *floc;
        if (strcmp(argv[starting_argument], "user.name") == 0) {
            if (global) {
                if ((floc = fopen(GlobalName, "w")) == NULL) {
                    perror("File could not be opened!");
                    return 1;
                }
                fprintf(floc, "%s", argv[starting_argument + 1]);
            } else {
                if ((floc = fopen(".dambiz/user/localname.txt", "w")) == NULL) {
                    perror("You haven't initialized yet");
                    return 1;
                }
                fprintf(floc, "%s", argv[starting_argument + 1]);
                fclose(floc);
            }
        } else if (strcmp(argv[starting_argument], "user.email") == 0) {
            if (global) {
                if ((floc = fopen(GlobalEmail, "w")) == NULL) {
                    perror("File could not be opened!");
                    return 1;
                }
                fprintf(floc, "%s", argv[starting_argument + 1]);
            } else {
                if ((floc = fopen(".dambiz/user/localemail.txt", "w")) == NULL) {
                    perror("You haven't initialized yet");
                    return 1;
                }
                fprintf(floc, "%s", argv[starting_argument + 1]);
                fclose(floc);
            }
        }
    } else {
        perror("Invalid Command!");
        return 1;
    }
}


int check_init() {
    struct dirent *insearch;
    char temp_directory[MAX_NAME_SIZE];

    do {
        DIR *current_directory;
        if ((current_directory = opendir(".")) == NULL) {
            perror("Could not open a current or parent directory!");
            return 1;
        }
        while ((insearch = readdir(current_directory)) != NULL) {
            if ((strcmp(insearch->d_name, ".dambiz") == 0) && (insearch->d_type == DT_DIR)) {
                perror("Could not initialize .dambiz because it is already created in the current directory or its parents'");
                return 2;
            }
        }
        closedir(current_directory);
        if (getcwd(temp_directory, sizeof(temp_directory)) == NULL) {
            perror("Could not open current working directory!\n");
            return 3;
        }
        if (strcmp(temp_directory, "/") != 0) {
            if (chdir("..") != 0) {
                printf("Could not change directory to the parent dir!\n");
                return 4;
            }
        }
    } while (strcmp(temp_directory, "/") != 0);
    return 0;
}


void create_essentials(){
    if (mkdir(".dambiz/user", 0755) || mkdir(".dambiz/alias", 0755) || mkdir(".dambiz/commits", 0755) || mkdir(".dambiz/staging", 0755) || mkdir(".dambiz/tracks", 0755)){
        perror("Oops! Something bad happened!");
    }
    if (fopen(".dambiz/user/localemail.txt", "w") == NULL || fopen(".dambiz/user/localname.txt", "w") == NULL){
        perror("Oops! Something bad happened!");
    }
}



int run_init(int argc, char **argv) {
    char cwd[MAX_NAME_SIZE];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Could not open current working directory!");
        return 1;
    }
    if (check_init() == 0) {
        chdir(cwd);
        if (mkdir(".dambiz", 0755) != 0) {
            perror("Something went wrong in initializing dambiz!");
        } else {
            create_essentials();
            perror("Dambiz is initialized successfully!");
        }
    }
}
