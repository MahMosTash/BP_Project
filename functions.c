#define GlobalName "/home/mahmostash/Desktop/BP_Project/user/globalname.txt"
#define GlobalEmail "/home/mahmostash/Desktop/BP_Project/user/globalemail.txt"
#define Staging ".dambiz/staging"
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


void create_essentials() {
    if (mkdir(".dambiz/user", 0755) || mkdir(".dambiz/alias", 0755) || mkdir(".dambiz/commits", 0755) ||
        mkdir(".dambiz/staging", 0755) || mkdir(".dambiz/tracks", 0755)) {
        perror("Oops! Something bad happened!");
    }
    if (fopen(".dambiz/user/localemail.txt", "w") == NULL || fopen(".dambiz/user/localname.txt", "w") == NULL) {
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
    } else if (check_init() == 2) {
        perror("Could not initialize .dambiz because it is already created in the current directory or its parents'");
    }
}

int directory_search(DIR *current, char name[]) {
    struct dirent *insearch;
    while ((insearch = readdir(current)) != NULL) {
        if ((strcmp(insearch->d_name, name) == 0)) {
            return 1;
        }
    }
    return 0;
}


int is_identical(char address1[], char address2[]) {
    FILE *f1 = fopen(address1, "rb");
    FILE *f2 = fopen(address2, "rb");
    char bf1, bf2;
    while (1) {
        fread(&bf1, 1, 1, f1) != 0;
        fread(&bf2, 1, 1, f2) != 0;

        if (bf1 != bf2) {
            return 0;
        }
        if ((feof(f1) && !feof(f2)) || (!feof(f1) && feof(f2))) {
            return 0;
        }
        if (feof(f1) && feof(f2)) {
            break;
        }

    }
    return 1;
}


int check_addfolder(char folderaddress[], char stageaddress[]) {
    DIR *adding;
    DIR *staging;
    struct dirent *ToAdd;

    if ((adding = opendir(folderaddress)) == NULL) {
        perror("Something happened!");
        return 0;
    }
    if ((staging = opendir(stageaddress)) == NULL) {
        return 1;
    }
    while ((ToAdd = readdir(adding)) != NULL) {
        if (ToAdd->d_type != DT_DIR) {
            char realfileadd[MAX_FILE_SIZE];
            char stagefileadd[MAX_FILE_SIZE];
            sprintf(realfileadd, "%s/%s", folderaddress, ToAdd->d_name);
            sprintf(stagefileadd, "%s/%s", stageaddress, ToAdd->d_name);
            if (directory_search(staging, ToAdd->d_name) == 0) {
                return 1;
            }
            if (is_identical(realfileadd, stagefileadd) == 0) {
                return 1;
            }
        } else if (ToAdd->d_type == DT_DIR && (strcmp(ToAdd->d_name, ".") != 0) && (strcmp(ToAdd->d_name, "..") != 0)) {
            char newrealfolderadd[MAX_FILE_SIZE];
            char newstagefolderadd[MAX_FILE_SIZE];
            sprintf(newrealfolderadd, "%s/%s", folderaddress, ToAdd->d_name);
            sprintf(newstagefolderadd, "%s/%s", stageaddress, ToAdd->d_name);
            if (check_addfolder(newrealfolderadd, newstagefolderadd) != 0) {
                return 1;
            }
        }
    }
    return 0;
}


int run_add(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
    }
    for (int i = 2; i < argc; i++) {
        int flagforadd = 1;
        if (strcmp(argv[i], "-f") == 0) {
            continue;
        }
        char add_address[MAX_FILE_SIZE];
        char stage_address[MAX_FILE_SIZE] = ".dambiz/staging/";
        strcpy(add_address, argv[i]);
        strcat(stage_address, add_address);
        DIR *adding;
        DIR *staging;
        struct dirent *ToAdd;
        if ((adding = opendir(".")) == NULL) {
            perror("Something happened!");
            return 1;
        }
        if ((staging = opendir(Staging)) == NULL) {
            perror("couldn't open the staging folder");
            return 1;
        }
        while ((ToAdd = readdir(adding)) != NULL) {
            if ((strcmp(ToAdd->d_name, add_address) == 0) && ToAdd->d_type != DT_DIR) {
                if (directory_search(staging, add_address)) {
                    if (is_identical(add_address, stage_address)) {
                        printf("Can't add %s because files have not changed\n", argv[i]);
                        flagforadd = 0;
                        break;
                    }
                }
            } else if ((strcmp(ToAdd->d_name, add_address) == 0) && ToAdd->d_type == DT_DIR) {
                if (check_addfolder(add_address, stage_address) == 0) {
                    printf("Can't add %s because files have not changed\n", argv[i]);
                    flagforadd = 0;
                    break;
                }

            }
        }
        if (flagforadd) {
            char command[1000];
            sprintf(command, "rsync -r %s .dambiz/staging", add_address);
            system(command);
            printf("%s had added successfully!\n", argv[i]);
        }
    }
}