#define GlobalName "/home/mahmostash/Desktop/BP_Project/user/globalname.txt"
#define GlobalEmail "/home/mahmostash/Desktop/BP_Project/user/globalemail.txt"
#define Staging ".dambiz/staging"
#define Addlog ".dambiz/tracks/addlog.txt"
#define MAX_FILE_SIZE 5000
#define MAX_NAME_SIZE 100
#define MAX_ADDRESS_SIZE 500

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
        printf("new configuration added successfully!\n");
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
    if (mkdir(".dambiz/user", 0755) || mkdir(".dambiz/alias", 0755) || mkdir(".dambiz/branches", 0755) ||
        mkdir(".dambiz/staging", 0755) || mkdir(".dambiz/tracks", 0755) || mkdir(".dambiz/branches/master", 0755) ||
        mkdir(".dambiz/branches/master/commits", 0755)) {
        perror("Oops! Something bad happened!");
    }
    FILE *commit;
    FILE *branch;
    FILE *localname;
    FILE *localemail;
    FILE *globalname;
    FILE *globalemail;

    char name[MAX_NAME_SIZE];
    char email[MAX_NAME_SIZE];

    if ((globalname = fopen(GlobalName, "r")) == NULL) {
        perror("oops");
    }
    globalemail = fopen(GlobalEmail, "r");

    fscanf(globalname, "%[^\r]s", name);
    fscanf(globalemail, "%[^\r]s", email);

    fclose(globalname);
    fclose(globalemail);


    if ((localemail = fopen(".dambiz/user/localemail.txt", "w")) == NULL ||
        fopen(".dambiz/tracks/addlog.txt", "w") == NULL ||
        (localname = fopen(".dambiz/user/localname.txt", "w")) == NULL ||
        (commit = fopen(".dambiz/branches/commitcounter.txt", "w")) == NULL ||
        (branch = fopen(".dambiz/branches/currentbranch.txt", "w")) == NULL ||
        (fopen(".dambiz/tracks/commitlog.txt", "w")) == NULL) {
        perror("Oops! Something bad happened!");
    }
    fprintf(commit, "0\n");
    fprintf(branch, "master\n");


    fprintf(localname, "%s", name);
    fprintf(localemail, "%s", email);

    fclose(localname);
    fclose(localemail);

    fclose(commit);
    fclose(branch);
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
            char realfileadd[MAX_ADDRESS_SIZE];
            char stagefileadd[MAX_ADDRESS_SIZE];
            sprintf(realfileadd, "%s/%s", folderaddress, ToAdd->d_name);
            sprintf(stagefileadd, "%s/%s", stageaddress, ToAdd->d_name);
            if (directory_search(staging, ToAdd->d_name) == 0) {
                return 1;
            }
            if (is_identical(realfileadd, stagefileadd) == 0) {
                return 1;
            }
        } else if (ToAdd->d_type == DT_DIR && (strcmp(ToAdd->d_name, ".") != 0) && (strcmp(ToAdd->d_name, "..") != 0)) {
            char newrealfolderadd[MAX_ADDRESS_SIZE];
            char newstagefolderadd[MAX_ADDRESS_SIZE];
            sprintf(newrealfolderadd, "%s/%s", folderaddress, ToAdd->d_name);
            sprintf(newstagefolderadd, "%s/%s", stageaddress, ToAdd->d_name);
            if (check_addfolder(newrealfolderadd, newstagefolderadd) != 0) {
                return 1;
            }
        }
    }
    return 0;
}


void addn() {
    DIR *adding;
    struct dirent *ToAdd;
    if ((adding = opendir(".")) == NULL) {
        perror("couldn't open the staging folder");
        return;
    }
    while ((ToAdd = readdir(adding)) != NULL) {
        DIR *staging;
        if ((staging = opendir(Staging)) == NULL) {
            perror("couldn't open the staging folder");
            return;
        }
        if ((strncmp(ToAdd->d_name, ".", 1) != 0)) {
            if (ToAdd->d_type == DT_DIR) {
                printf("%s ", ToAdd->d_name);
                if (directory_search(staging, ToAdd->d_name)) {
                    if (check_addfolder(ToAdd->d_name, Staging)) {
                        printf("(staged!)\n");
                    } else {
                        printf("(not staged!)\n");
                    }
                } else {
                    printf("(not staged!)\n");
                }
            } else {
                printf("%s ", ToAdd->d_name);
                if (directory_search(staging, ToAdd->d_name)) {
                    char filestageadd[MAX_ADDRESS_SIZE] = ".dambiz/staging/";
                    strcat(filestageadd, ToAdd->d_name);
                    if (is_identical(ToAdd->d_name, filestageadd)) {
                        printf("(staged!)\n");
                    } else {
                        printf("(not staged!)\n");
                    }
                } else {
                    printf("(not staged!)\n");
                }
            }
        }
    }
}

void redo() {
    DIR *staging;
    struct dirent *ToRedo;
    if ((staging = opendir(Staging)) == NULL) {
        perror("couldn't open the staging folder");
        return;
    }
    while ((ToRedo = readdir(staging)) != NULL) {
        if ((strcmp(ToRedo->d_name, ".") != 0) && (strcmp(ToRedo->d_name, "..") != 0)) {
            if (opendir(ToRedo->d_name) == NULL && fopen(ToRedo->d_name, "rb") == NULL) {
                char command[MAX_NAME_SIZE];
                sprintf(command, "rm -r .dambiz/staging/%s", ToRedo->d_name);
                system(command);
            } else {
                char command[MAX_NAME_SIZE];
                sprintf(command, "cp -r --parents %s .dambiz/staging/", ToRedo->d_name);
                system(command);
            }
        }
    }
    perror("Redo ran successfully!");
    return;
}

int run_add(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }
    if ((argc == 3) && strcmp(argv[2], "-redo") == 0) {
        redo();
        return 1;
    }
    if ((argc == 3) && strcmp(argv[2], "-n") == 0) {
        addn();
        return 1;
    }
    for (int i = 2; i < argc; i++) {
        int flagforadd = 0;
        if (strcmp(argv[i], "-f") == 0) {
            continue;
        }
        char add_address[MAX_ADDRESS_SIZE];
        char stage_address[MAX_ADDRESS_SIZE] = ".dambiz/staging/";
        char file_name[MAX_NAME_SIZE];
        char opening_folder[MAX_ADDRESS_SIZE];
        char opening_stage[MAX_ADDRESS_SIZE];
        strcpy(add_address, argv[i]);
        strcat(stage_address, add_address);
        DIR *adding;
        DIR *staging;
        struct dirent *ToAdd;
        if (strstr(argv[i], "/")) {
            int lastslash = strrchr(argv[i], '/') - argv[i];
            strcpy(file_name, strrchr(argv[i], '/') + 1);
            strncpy(opening_folder, argv[i], lastslash);
            sprintf(opening_stage, "%s/%s", Staging, opening_folder);
            if ((adding = opendir(opening_folder)) == NULL) {
                perror("Address is not valid!");
                return 1;
            }
            if ((staging = opendir(opening_stage)) == NULL) {
                char command[1000];
                sprintf(command, "cp -r --parents %s %s", add_address, Staging);
                system(command);
                printf("%s had added successfully!\n", argv[i]);
                FILE *addlog = fopen(Addlog, "a");
                fprintf(addlog, "%s/%s ", opening_stage, file_name);
                fclose(addlog);
                continue;

            }
        } else {
            strcpy(file_name, argv[i]);
            strcpy(opening_stage, Staging);
            if ((adding = opendir(".")) == NULL) {
                perror("Something happened!");
                return 1;
            }
            if ((staging = opendir(Staging)) == NULL) {
                perror("couldn't open the staging folder");
                return 1;
            }
        }
        while ((ToAdd = readdir(adding)) != NULL) {
            if ((strcmp(ToAdd->d_name, file_name) == 0) && ToAdd->d_type != DT_DIR) {
                if (directory_search(staging, file_name)) {
                    if (is_identical(add_address, stage_address)) {
                        printf("Can't add %s because files have not changed\n", argv[i]);
                        flagforadd = 2;
                        break;
                    }
                }
                flagforadd = 1;
            } else if ((strcmp(ToAdd->d_name, file_name) == 0) && ToAdd->d_type == DT_DIR) {
                if (check_addfolder(add_address, stage_address) == 0) {
                    printf("Can't add %s because files have not changed\n", argv[i]);
                    flagforadd = 2;
                    break;
                }
                flagforadd = 1;

            }
        }
        if (flagforadd == 1) {
            char command[1000];
            sprintf(command, "cp -r --parents %s %s", add_address, opening_stage);
            system(command);
            printf("%s had added successfully!\n", argv[i]);
            FILE *addlog = fopen(Addlog, "a");
            fprintf(addlog, "%s/%s ", opening_stage, file_name);
            fclose(addlog);
        } else if (flagforadd == 0) {
            perror("No such file or directory!");
            return 1;
        }
    }
}


void Finding_lastline(FILE *given_file, char *str) {
    char content[MAX_FILE_SIZE];
    char content_copy[MAX_FILE_SIZE];
    char lines[20][MAX_FILE_SIZE];
    fscanf(given_file, "%[^\r]s", content);
    fclose(given_file);
    strcpy(content_copy, content);
    char *currentline = strtok(content_copy, "\n");
    int countofline = 0;
    while (currentline != NULL) {
        strcpy(lines[countofline++], currentline);
        currentline = strtok(NULL, "\n");
    }
    strcpy(str, lines[countofline - 1]);
}

int run_reset(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }
    if (strcmp(argv[2], "-undo") == 0) {
        if (argc == 3) {
            FILE *addlog = fopen(Addlog, "r+");
            char addlogcontent[MAX_FILE_SIZE];
            char addlogcontent_copy[MAX_FILE_SIZE];
            char rmcommand[20][MAX_FILE_SIZE];
            fscanf(addlog, "%[^\r]s", addlogcontent);
            fclose(addlog);
            strcpy(addlogcontent_copy, addlogcontent);
            char *rmcurrent = strtok(addlogcontent_copy, "\n");
            int countofadd = 0;
            while (rmcurrent != NULL) {
                strcpy(rmcommand[countofadd++], rmcurrent);
                rmcurrent = strtok(NULL, "\n");
            }
            char newaddlog[MAX_FILE_SIZE];
            strncpy(newaddlog, addlogcontent, (strlen(addlogcontent) - 1 - strlen(rmcommand[countofadd - 1])));
            char command[1000];
            sprintf(command, "rm -r %s", rmcommand[countofadd - 1]);
            system(command);
            addlog = fopen(Addlog, "w");
            fprintf(addlog, "%s", newaddlog);
            fclose(addlog);
            printf("undo was ran successfully!\n");
            return 1;
        } else {
            perror("Invalid Command!");
            return 1;
        }
    }

    for (int i = 2; i < argc; i++) {
        int flagforreset = 1;
        if (strcmp(argv[i], "-f") == 0) {
            continue;
        }
        char reset_address[MAX_ADDRESS_SIZE];
        char stage_address[MAX_ADDRESS_SIZE] = ".dambiz/staging/";
        char file_name[MAX_NAME_SIZE];
        char opening_folder[MAX_ADDRESS_SIZE];
        char opening_stage[MAX_ADDRESS_SIZE];
        strcpy(reset_address, argv[i]);
        strcat(stage_address, reset_address);
        struct dirent *ToReset;
        DIR *staging;
        if (strstr(argv[i], "/")) {
            int lastslash = strrchr(argv[i], '/') - argv[i];
            strcpy(file_name, strrchr(argv[i], '/') + 1);
            strncpy(opening_folder, argv[i], lastslash);
            sprintf(opening_stage, "%s/%s", Staging, opening_folder);
        } else {
            strcpy(opening_stage, Staging);
            strcpy(file_name, argv[i]);
        }
        if ((staging = opendir(opening_stage)) == NULL) {
            perror("Invalid address!");
            return 1;
        }
        while ((ToReset = readdir(staging)) != NULL) {
            if ((strcmp(ToReset->d_name, file_name) == 0)) {
                char command[1000];
                sprintf(command, "rm -r %s", stage_address);
                system(command);
                printf("%s had reset successfully!\n", argv[i]);
                flagforreset = 0;
                break;
            }
        }
        if (flagforreset) {
            printf("Can't reset %s because files do not exist!\n", argv[i]);
        }
    }
}


int run_status(int argc, char **argv) {

}


int EmptyFolderCheck(DIR *checkingfolder) {
    int counter = -2;
    while ((readdir(checkingfolder)) != NULL) {
        counter++;
    }
    return counter;
}


int run_commit(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }
    int commiting_files;

    if (strcmp(argv[2], "-m") == 0) {
        DIR *staging = opendir(Staging);
        if ((commiting_files = EmptyFolderCheck(staging)) == 0) {
            printf("No staging file to commit :(\n");
            return 1;
        }


        if (strlen(argv[argc - 1]) > 72) {
            printf("Your commit message is too long!\n");
            return 1;
        }


        FILE *currentbranch;
        if ((currentbranch = fopen(".dambiz/branches/currentbranch.txt", "r")) == NULL) {
            printf("WTAF\n");
        }


        FILE *counter = fopen(".dambiz/branches/commitcounter.txt", "r");
        FILE *commitslog = fopen(".dambiz/tracks/commitlog.txt", "a");
        int commitcounter;
        char path[MAX_ADDRESS_SIZE];
        fscanf(counter, "%d", &commitcounter);
        char current[MAX_ADDRESS_SIZE];
        fscanf(currentbranch, "%[^\n]s", current);
        commitcounter++;
        fclose(counter);
        fprintf(commitslog, ".dambiz/branches/%s/commits/%d\n", current, commitcounter);
        counter = fopen(".dambiz/branches/commitcounter.txt", "w");
        fprintf(counter, "%d", commitcounter);
        fclose(currentbranch);
        fclose(counter);
        fclose(commitslog);
        sprintf(path, ".dambiz/branches/%s/commits/%d", current, commitcounter);
        if (mkdir(path, 0755) != 0) {
            perror("oops!");
        }
        char command[MAX_ADDRESS_SIZE];
        sprintf(command, "mv .dambiz/staging/* %s/", path);
        system(command);
        char time_path[MAX_FILE_SIZE];
        char commitlog[MAX_ADDRESS_SIZE];
        sprintf(commitlog, "%s/log.txt", path);
        sprintf(time_path, "%s/committime.txt", path);
        strcat(path, "/comitmessage.txt");
        FILE *commitmessage = fopen(path, "w");
        fprintf(commitmessage, "%s", argv[3]);
        fclose(commitmessage);
        time_t currenttime;
        struct tm *ptr_time;
        char mytime[50];
        time(&currenttime);
        ptr_time = localtime(&currenttime);
        if (strftime(mytime, 50, "%Y.%m.%d %A - %H:%M:%S", ptr_time) == 0) {
            perror("Couldn't prepare time formatted string\n");
        }
        FILE *committime = fopen(time_path, "w");
        fprintf(committime, "%s", mytime);
        fclose(committime);
        FILE *log = fopen(commitlog, "w");
        char author[MAX_NAME_SIZE];
        FILE *user = fopen(".dambiz/user/localname.txt", "r");
        fscanf(user, "%[^\r]s", author);
        fclose(user);
        fprintf(log,
                "************************************************************************************************************************************************************************************************************\nCommited at:     %s\nCommit message:     %s\nCommited by:     %s\nCommit ID:     %d\nBranch:     %s\n%d File(s) were commited!\n************************************************************************************************************************************************************************************************************\n",
                mytime, argv[3], author, commitcounter, current, commiting_files);
        printf("%d Files commited successfully!\n\n\n", commiting_files);
        printf("Commit ID:   %d\nCommit Message:   %s\nCommitting Time:   %s\n", commitcounter, argv[3], mytime);
    } else {
        perror("Invalid Command!");
        return 1;
    }
}

int run_branch(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }
    if (argc == 3) {
        DIR *branches;
        branches = opendir(".dambiz/branches");
        if (directory_search(branches, argv[2])) {
            printf("Branch name can not be used because it already exists!\n");
            return 1;
        }
        char path[MAX_ADDRESS_SIZE];
        sprintf(path, ".dambiz/branches/%s", argv[2]);
        mkdir(path, 0755);
        strcat(path, "/commits");
        mkdir(path, 0755);
        char commitpath[MAX_ADDRESS_SIZE];
        FILE *commitlog = fopen(".dambiz/tracks/commitlog.txt", "r");
        Finding_lastline(commitlog, commitpath);
        char command[MAX_ADDRESS_SIZE];
        sprintf(command, "cp -r %s %s/", commitpath, path);
        system(command);
        printf("Branch created successfully!\n");
    } else {
        DIR *branches;
        branches = opendir(".dambiz/branches");
        struct dirent *branch;
        int counter = 1;
        while ((branch = readdir(branches)) != NULL) {
            if ((strcmp(".", branch->d_name) != 0) && (strcmp("..", branch->d_name) != 0) &&
                (strstr(branch->d_name, ".txt") == NULL)) {
                printf("Branch %d: %s\n", counter++, branch->d_name);
            }
        }
    }
}

int run_log(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }
    FILE *commitlog = fopen(".dambiz/tracks/commitlog.txt", "r");
    char commitlogcontent[MAX_FILE_SIZE];
    char commitlogcontent_copy[MAX_FILE_SIZE];
    char logs[20][MAX_FILE_SIZE];
    fscanf(commitlog, "%[^\r]s", commitlogcontent);
    fclose(commitlog);
    strcpy(commitlogcontent_copy, commitlogcontent);
    char *rmcurrent = strtok(commitlogcontent_copy, "\n");
    int countoflog = 0;
    while (rmcurrent != NULL) {
        strcpy(logs[countoflog++], rmcurrent);
        rmcurrent = strtok(NULL, "\n");
    }
    if (argc == 2) {
        printf("\n\n");
        for (int i = (countoflog - 1); i >= 0; i--) {
            char path[MAX_ADDRESS_SIZE];
            sprintf(path, "%s/log.txt", logs[i]);
            FILE *currentlog = fopen(path, "r");
            char logcontent[MAX_FILE_SIZE];
            fscanf(currentlog, "%[^\r]s", logcontent);
            printf("%s\n\n", logcontent);
        }
    } else if((argc == 4) && strcmp(argv[2] , "-n") == 0){
        int logcount = atoi(argv[3]);
        if (logcount > countoflog){
            logcount = countoflog;
        }
        printf("\n\n");
        for (int i = (countoflog - 1); i > (countoflog - 1) - logcount ; i--) {
            char path[MAX_ADDRESS_SIZE];
            sprintf(path, "%s/log.txt", logs[i]);
            FILE *currentlog = fopen(path, "r");
            char logcontent[MAX_FILE_SIZE];
            fscanf(currentlog, "%[^\r]s", logcontent);
            printf("%s\n\n", logcontent);
        }
    }
}