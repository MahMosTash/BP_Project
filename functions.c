#define GlobalName "/home/mahmostash/Desktop/BP_Project/user/globalname.txt"
#define GlobalEmail "/home/mahmostash/Desktop/BP_Project/user/globalemail.txt"
#define ALIAS "/home/mahmostash/Desktop/BP_Project/alias"
#define Staging ".dambiz/staging"
#define Addlog ".dambiz/tracks/addlog.txt"
#define Branches ".dambiz/branches"
#define CURRENT_BRANCH ".dambiz/branches/currentbranch.txt"
#define Current_Commit ".dambiz/tracks/currentcommit.txt"
#define MAX_FILE_SIZE 5000
#define MAX_NAME_SIZE 100
#define MAX_ADDRESS_SIZE 500
#define RESET \033[1;0



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


int run_config(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }
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
        if (global){
            char path[MAX_ADDRESS_SIZE];
            sprintf(path, "%s/%s", ALIAS, argv[starting_argument] + 6);
            FILE * aliasfile = fopen(path, "w");
            fprintf(aliasfile, "%s\n", argv[starting_argument + 1]);
        }
        char path[MAX_ADDRESS_SIZE];
        sprintf(path, ".dambiz/alias/%s", argv[starting_argument] + 6);
        FILE * aliasfile = fopen(path, "w");
        fprintf(aliasfile, "%s\n", argv[starting_argument + 1]);

        printf("Alias Added Successfully!\n");

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



void create_essentials() {
    if (mkdir(".dambiz/user", 0755) || mkdir(".dambiz/shortcuts", 0755) || mkdir(".dambiz/alias", 0755) ||
        mkdir(".dambiz/branches", 0755) || mkdir(".dambiz/hooks", 0755) ||
        mkdir(".dambiz/staging", 0755) || mkdir(".dambiz/tracks", 0755) || mkdir(".dambiz/branches/master", 0755) ||
        mkdir(".dambiz/branches/master/commits", 0755) || mkdir(".dambiz/tags", 0755)) {
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


    if (((localemail = fopen(".dambiz/user/localemail.txt", "w")) == NULL) ||
        (fopen(".dambiz/tracks/addlog.txt", "w") == NULL) ||
        (localname = fopen(".dambiz/user/localname.txt", "w")) == NULL ||
        ((commit = fopen(".dambiz/branches/commitcounter.txt", "w")) == NULL) ||
        (branch = fopen(CURRENT_BRANCH, "w")) == NULL ||
        ((fopen(".dambiz/tracks/commitlog.txt", "w")) == NULL) ||
        (fopen(".dambiz/tracks/currentcommit.txt", "w") == NULL) ||
        (fopen(".dambiz/tags/tags.txt", "w") == NULL)) {
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


int FindHead(DIR *branch) {
    int max = 0;
    struct dirent *current_commit;
    while ((current_commit = readdir(branch)) != NULL) {
        if (strstr(current_commit->d_name, ".") == NULL) {
            if (atoi(current_commit->d_name) > max) {
                max = atoi(current_commit->d_name);
            }
        }
    }
    return max;
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
        } else if (ToAdd->d_type == DT_DIR && (strncmp(ToAdd->d_name, ".", 1) != 0)) {
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
        return 5;
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


void Finding_lastline(FILE *given_file, char *str, char *current_branch) {
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
    for (int i = (countofline - 1); i >= 0; i--) {
        if (strstr(lines[i], current_branch)) {
            strcpy(str, lines[i]);
            return;
        }
    }
}


void Finding_nlastline(FILE *given_file, char *str, char *current_branch, int number) {
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
    for (int i = (countofline - 1); i >= 0; i--) {
        if (strstr(lines[i], current_branch)) {
            if (number == 0) {
                strcpy(str, lines[i]);
                return;
            }
            number--;
        }
    }
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
            printf("undo ran successfully!\n");
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
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }
    DIR *repository = opendir(".");
    DIR *stage = opendir(Staging);

    FILE *curb = fopen(CURRENT_BRANCH, "r");
    char currentbranch[MAX_NAME_SIZE];
    fscanf(curb, "%[^\n]s", currentbranch);

    char branchpath[MAX_ADDRESS_SIZE];

    sprintf(branchpath, ".dambiz/branches/%s/commits", currentbranch);

    DIR *currb = opendir(branchpath);
    int head = FindHead(currb);

    char headpath[MAX_ADDRESS_SIZE];
    sprintf(headpath, ".dambiz/branches/%s/commits/%d", currentbranch, head);
    if (head == 0) {
        printf("No commits available.\n");
        return 1;
    }

    DIR *HEAD;
    char Y;
    struct dirent *eachfile;
    while ((eachfile = readdir(repository)) != NULL) {
        HEAD = opendir(headpath);
        Y = 'N';
        if (strncmp(eachfile->d_name, ".", 1) != 0) {
            if (directory_search(HEAD, eachfile->d_name) == 0) {
                Y = 'A';
            } else if (eachfile->d_type == DT_DIR) {
                char folderinhead[MAX_ADDRESS_SIZE];
                sprintf(folderinhead, "%s/%s", headpath, eachfile->d_name);
                if (check_addfolder(eachfile->d_name, folderinhead) == 1) {
                    Y = 'M';
                }
                struct stat permission1;
                struct stat permission2;
                stat(eachfile->d_name, &permission1);
                stat(folderinhead, &permission2);
                if (permission1.st_mode != permission2.st_mode) {
                    Y = 'T';
                }
            } else {
                char fileinhead[MAX_ADDRESS_SIZE];
                sprintf(fileinhead, "%s/%s", headpath, eachfile->d_name);
                if (is_identical(eachfile->d_name, fileinhead) == 0) {
                    Y = 'M';
                }
                struct stat permission1;
                struct stat permission2;
                stat(eachfile->d_name, &permission1);
                stat(fileinhead, &permission2);
                if (permission1.st_mode != permission2.st_mode) {
                    Y = 'T';
                }
            }
            if (Y != 'N') {
                printf("%s: ", eachfile->d_name);
                if (directory_search(stage, eachfile->d_name) == 1) {
                    printf("+%c\n", Y);
                } else {
                    printf("-%c\n", Y);
                }
            }
        }
    }
    stage = opendir(Staging);
    HEAD = opendir(headpath);
    struct dirent *deletecheck;
    while ((deletecheck = readdir(HEAD)) != NULL) {
        if (strncmp(deletecheck->d_name, ".", 1) != 0) {
            repository = opendir(".");
            if (directory_search(repository, deletecheck->d_name) == 0) {
                if ((strncmp(deletecheck->d_name, ".", 1) != 0) && (strcmp(deletecheck->d_name, "author.txt") != 0) &&
                    (strcmp(deletecheck->d_name, "commitmessage.txt") != 0) &&
                    (strcmp(deletecheck->d_name, "committime.txt") != 0) &&
                    (strcmp(deletecheck->d_name, "log.txt") != 0)) {
                    if (directory_search(stage, deletecheck->d_name) == 1) {
                        printf("%s: +D\n", deletecheck->d_name);
                    } else {
                        printf("%s: -D\n", deletecheck->d_name);
                    }
                }
            }
        }
    }
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

    if ((strcmp(argv[2], "-m") == 0) || (strcmp(argv[2], "-s") == 0)) {
        char finalmessage[MAX_ADDRESS_SIZE];

        DIR *staging = opendir(Staging);
        if ((commiting_files = EmptyFolderCheck(staging)) == 0) {
            printf("No staging file to commit :(\n");
            return 1;
        }

        if (strcmp(argv[2], "-s") == 0) {
            DIR *shortcuts = opendir(".dambiz/shortcuts");
            char txtname[MAX_NAME_SIZE];
            sprintf(txtname, "%s.txt", argv[3]);
            if (directory_search(shortcuts, txtname) == 0) {
                printf("Invalid Shortcut! Check your input.\n");
                return 1;
            }
            char shortcutpath[MAX_ADDRESS_SIZE];
            sprintf(shortcutpath, ".dambiz/shortcuts/%s", txtname);
            FILE *newshort = fopen(shortcutpath, "r");
            fscanf(newshort, "%[^\n]s", finalmessage);
        } else {
            strcpy(finalmessage, argv[3]);
        }


        if (strlen(finalmessage) > 72) {
            printf("Your commit message is too long!\n");
            return 1;
        }


        FILE *currentbranch;
        if ((currentbranch = fopen(CURRENT_BRANCH, "r")) == NULL) {
            printf("WTAF\n");
        }


        char current[MAX_ADDRESS_SIZE];
        fscanf(currentbranch, "%[^\n]s", current);
        fclose(currentbranch);

        FILE *counter = fopen(".dambiz/branches/commitcounter.txt", "r");
        int commitcounter;
        fscanf(counter, "%d", &commitcounter);
        commitcounter++;
        fclose(counter);
        counter = fopen(".dambiz/branches/commitcounter.txt", "w");
        fprintf(counter, "%d", commitcounter);
        fclose(counter);

        char path[MAX_ADDRESS_SIZE];
        char branchpath[MAX_ADDRESS_SIZE];

        sprintf(branchpath, ".dambiz/branches/%s/commits", current);
        sprintf(path, ".dambiz/branches/%s/commits/%d", current, commitcounter);

        DIR *curb = opendir(branchpath);
        int head = FindHead(curb);

        char headpath[MAX_ADDRESS_SIZE];
        sprintf(headpath, ".dambiz/branches/%s/commits/%d", current, head);


        FILE *currentcommit = fopen(Current_Commit, "w");
        fprintf(currentcommit, "%s", path);
        fclose(currentcommit);

        if (mkdir(path, 0755) != 0) {
            perror("oops!\n");
        }


        FILE *commitslog;
        if (commitcounter != 1) {

            char copy_command[MAX_FILE_SIZE];
            sprintf(copy_command, "cp -r %s/* %s/", headpath, path);
            system(copy_command);
        }
        commitslog = fopen(".dambiz/tracks/commitlog.txt", "a");
        fprintf(commitslog, ".dambiz/branches/%s/commits/%d\n", current, commitcounter);
        fclose(commitslog);

        char command[MAX_ADDRESS_SIZE];
        sprintf(command, "cp -r .dambiz/staging/* %s/", path);
        system(command);
        strcpy(command, "rm -r .dambiz/staging/*");
        system(command);


        char time_path[MAX_ADDRESS_SIZE];
        char commitlog[MAX_ADDRESS_SIZE];
        char commitauthor[MAX_ADDRESS_SIZE];
        sprintf(commitlog, "%s/log.txt", path);
        sprintf(time_path, "%s/committime.txt", path);
        sprintf(commitauthor, "%s/author.txt", path);
        strcat(path, "/commitmessage.txt");


        FILE *commitmessage = fopen(path, "w");
        fprintf(commitmessage, "%s\n", finalmessage);
        fclose(commitmessage);


        time_t currenttime;
        struct tm *ptr_time;
        char mytime[50];
        char savetime[50];
        time(&currenttime);
        ptr_time = localtime(&currenttime);
        if (strftime(mytime, 50, "%A %Y.%m.%d - %H:%M:%S", ptr_time) == 0) {
            perror("Couldn't prepare time formatted string\n");
        }
        if (strftime(savetime, 50, "%s", ptr_time) == 0) {
            perror("Couldn't prepare time formatted string\n");
        }


        FILE *comauthor = fopen(commitauthor, "w");
        FILE *committime = fopen(time_path, "w");
        fprintf(committime, "%s", savetime);
        fclose(committime);


        FILE *log = fopen(commitlog, "w");
        char author[MAX_NAME_SIZE];
        FILE *user = fopen(".dambiz/user/localname.txt", "r");
        fscanf(user, "%[^\r]s", author);
        fclose(user);
        fprintf(comauthor, "%s", author);
        fclose(comauthor);
        fprintf(log,
                "************************************************************************************************************************************************************************************************************\nCommited at:     %s\nCommit message:     %s\nCommited by:     %s\nCommit ID:     %d\nBranch:     %s\n%d File(s) were commited!\n************************************************************************************************************************************************************************************************************\n",
                mytime, finalmessage, author, commitcounter, current, commiting_files);
        printf("%d Files commited successfully!\n\n\n", commiting_files);
        printf("Commit ID:   %d\nCommit Message:   %s\nCommitting Time:   %s\n", commitcounter, finalmessage,
               mytime);
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
        char commitpath[MAX_ADDRESS_SIZE] = "";
        char current[MAX_NAME_SIZE];
        FILE *commitlog = fopen(".dambiz/tracks/commitlog.txt", "r");
        FILE *currentbranch = fopen(CURRENT_BRANCH, "r");
        fscanf(currentbranch, "%[^\n]s", current);

        FILE *currentcommit = fopen(Current_Commit, "r");
        fscanf(currentcommit, "%[^\n]s", commitpath);
        if (strlen(commitpath) == 0) {
            printf("Can not create a branch without committing!\n");
            return 1;
        }

        char path[MAX_ADDRESS_SIZE];
        sprintf(path, ".dambiz/branches/%s", argv[2]);
        mkdir(path, 0755);
        strcat(path, "/commits");
        mkdir(path, 0755);
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
            fclose(currentlog);
        }
    } else if ((argc == 4) && strcmp(argv[2], "-n") == 0) {
        int logcount = atoi(argv[3]);
        if (logcount > countoflog) {
            logcount = countoflog;
        }
        printf("\n\n");
        for (int i = (countoflog - 1); i > (countoflog - 1) - logcount; i--) {
            char path[MAX_ADDRESS_SIZE];
            sprintf(path, "%s/log.txt", logs[i]);
            FILE *currentlog = fopen(path, "r");
            char logcontent[MAX_FILE_SIZE];
            fscanf(currentlog, "%[^\r]s", logcontent);
            printf("%s\n\n", logcontent);
            fclose(currentlog);
        }
    } else if ((argc == 4) && strcmp(argv[2], "-branch") == 0) {
        DIR *branches = opendir(".dambiz/branches");
        if (directory_search(branches, argv[3]) == 0) {
            printf("Unknown branch! check your inputs.\n");
            return 1;
        }
        printf("\n\n");
        int counter = 0;
        for (int i = (countoflog - 1); i >= 0; i--) {
            char path[MAX_ADDRESS_SIZE];
            sprintf(path, "%s/log.txt", logs[i]);
            if (strstr(path, argv[3]) == NULL) {
                continue;
            }
            int branchpart = strstr(path, argv[3]) - path;
            if (path[branchpart - 1] != '/' || path[branchpart + strlen(argv[3])] != '/') {
                continue;
            }
            FILE *currentlog = fopen(path, "r");
            char logcontent[MAX_FILE_SIZE];
            fscanf(currentlog, "%[^\r]s", logcontent);
            printf("%s\n\n", logcontent);
            fclose(currentlog);
            counter++;
        }
        if (!counter) {
            printf("No commits available in this branch!\n\n\n");
        }
    } else if ((argc == 4) && strcmp(argv[2], "-author") == 0) {
        printf("\n\n");
        int counter = 0;
        for (int i = (countoflog - 1); i >= 0; i--) {
            char path[MAX_ADDRESS_SIZE];
            char author_path[MAX_ADDRESS_SIZE];
            sprintf(path, "%s/log.txt", logs[i]);
            sprintf(author_path, "%s/author.txt", logs[i]);
            FILE *author = fopen(author_path, "r");
            char name[MAX_NAME_SIZE];
            fscanf(author, "%[^\r]s", name);
            if (strcmp(name, argv[3]) != 0) {
                continue;
            }
            FILE *currentlog = fopen(path, "r");
            char logcontent[MAX_FILE_SIZE];
            fscanf(currentlog, "%[^\r]s", logcontent);
            printf("%s\n\n", logcontent);
            fclose(currentlog);
            counter++;
        }
        if (!counter) {
            printf("No commits available with this author!\n\n\n");
        }
    } else if ((argc == 4) && strcmp(argv[2], "-search") == 0) {
        printf("\n\n");
        int counter = 0;
        char words[20][MAX_NAME_SIZE];
        char *word = strtok(argv[3], " ");
        int countofwords = 0;
        while (word != NULL) {
            strcpy(words[countofwords++], word);
            word = strtok(NULL, " ");
        }
        for (int i = (countoflog - 1); i >= 0; i--) {
            char path[MAX_ADDRESS_SIZE];
            char message_path[MAX_ADDRESS_SIZE];
            sprintf(path, "%s/log.txt", logs[i]);
            sprintf(message_path, "%s/commitmessage.txt", logs[i]);
            FILE *message = fopen(message_path, "r");
            char commitmessage[MAX_NAME_SIZE];
            fscanf(message, "%[^\r]s", commitmessage);
            int found = 0;
            for (int j = 0; j < countofwords; j++) {
                if (strstr(commitmessage, words[j]) != NULL) {
                    found++;
                    break;
                }
            }
            if (!found) {
                continue;
            }
            FILE *currentlog = fopen(path, "r");
            char logcontent[MAX_FILE_SIZE];
            fscanf(currentlog, "%[^\r]s", logcontent);
            printf("%s\n\n", logcontent);
            fclose(currentlog);
            counter++;
        }
        if (!counter) {
            printf("No commits available with this author!\n\n\n");
        }
    } else if ((argc == 4) && (strcmp(argv[2], "-since") == 0 || strcmp(argv[2], "-before") == 0)) {
        printf("\n\n");
        int counter = 0;
        for (int i = (countoflog - 1); i >= 0; i--) {
            char path[MAX_ADDRESS_SIZE];
            char time_path[MAX_ADDRESS_SIZE];
            sprintf(path, "%s/log.txt", logs[i]);
            sprintf(time_path, "%s/committime.txt", logs[i]);
            FILE *time = fopen(time_path, "r");
            char date[MAX_NAME_SIZE];
            fscanf(time, "%[^\r]s", date);
            if ((atoi(argv[3]) < atoi(date)) && (strcmp(argv[2], "-before") == 0)) {
                continue;
            }
            if ((atoi(argv[3]) > atoi(date)) && (strcmp(argv[2], "-since") == 0)) {
                continue;
            }
            FILE *currentlog = fopen(path, "r");
            char logcontent[MAX_FILE_SIZE];
            fscanf(currentlog, "%[^\r]s", logcontent);
            printf("%s\n\n", logcontent);
            fclose(currentlog);
            counter++;
        }
        if (!counter) {
            printf("No commits available with expected commit time!\n\n\n");
        }
    } else {
        printf("Invalid Command!\n");
    }
}


int run_checkout(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }


    DIR *staging = opendir(Staging);
    if (EmptyFolderCheck(staging)) {
        printf("Staging folder is not empty! commit recently staged files\n");
        return 1;
    }

    FILE *counter = fopen(".dambiz/branches/commitcounter.txt", "r");
    int commitcounter;
    fscanf(counter, "%d", &commitcounter);
    bool isbranch = true;

    DIR *branches = opendir(Branches);
    if (atoi(argv[2]) != 0) {
        isbranch = false;
        if (atoi(argv[2]) > commitcounter) {
            printf("Unknown Commit ID! Check your input.\n");
            return 1;
        }
    } else if (strcmp(argv[2], "HEAD") == 0 || strncmp(argv[2], "HEAD-", 5) == 0) {
        isbranch = false;
    } else if (directory_search(branches, argv[2]) == 0) {
        printf("Unknown branch! Check your input.\n");
        return 1;

    }
    FILE *currentb = fopen(CURRENT_BRANCH, "r");
    char currentbranch[MAX_NAME_SIZE];
    fscanf(currentb, "%[^\n]s", currentbranch);
    fclose(currentb);


    char headcommitpath[MAX_ADDRESS_SIZE];
    FILE *currentcommit = fopen(Current_Commit, "r");
    fscanf(currentcommit, "%[^\n]s", headcommitpath);
    fclose(currentcommit);


    if (check_addfolder(".", headcommitpath) == 1) {
        printf("You can not checkout because there are changed files that are not committed!\n");
        return 1;
    }

    DIR *WhereWeAre = opendir(".");
    struct dirent *myf;
    while ((myf = readdir(WhereWeAre)) != NULL) {
        if (strncmp(myf->d_name, ".", 1) != 0) {
            char command[MAX_ADDRESS_SIZE];
            sprintf(command, "rm -r %s", myf->d_name);
            system(command);
        }
    }

    if (isbranch) {


        char newbcommitpath[MAX_ADDRESS_SIZE];
        sprintf(newbcommitpath, ".dambiz/branches/%s/commits", argv[2]);
        DIR *nbranch = opendir(newbcommitpath);
        int HN = FindHead(nbranch);
        char newbranchcommit[MAX_ADDRESS_SIZE];
        sprintf(newbranchcommit, "%s/%d", newbcommitpath, HN);

        currentb = fopen(CURRENT_BRANCH, "w");
        fprintf(currentb, "%s\n", argv[2]);
        fclose(currentb);


        DIR *HeadCommit = opendir(newbranchcommit);
        struct dirent *cf;
        while ((cf = readdir(HeadCommit)) != NULL) {
            if ((strncmp(cf->d_name, ".", 1) != 0) && (strcmp(cf->d_name, "author.txt") != 0) &&
                (strcmp(cf->d_name, "commitmessage.txt") != 0) && (strcmp(cf->d_name, "committime.txt") != 0) &&
                (strcmp(cf->d_name, "log.txt") != 0)) {
                char command[MAX_ADDRESS_SIZE];
                sprintf(command, "cp -r %s/%s .", newbranchcommit, cf->d_name);
                system(command);
            }
        }
        currentcommit = fopen(Current_Commit, "w");
        fprintf(currentcommit, "%s\n", newbranchcommit);
        fclose(currentcommit);

        printf("Checkout ran successfully!\n");
    } else {
        char commiting_add[MAX_ADDRESS_SIZE];
        if (strcmp(argv[2], "HEAD") == 0 || strncmp(argv[2], "HEAD-", 5) == 0) {
            char commitpath[MAX_ADDRESS_SIZE];
            char current[MAX_NAME_SIZE];
            char headaddress[MAX_ADDRESS_SIZE];
            FILE *commitlog = fopen(".dambiz/tracks/commitlog.txt", "r");
            FILE *currentbranch = fopen(CURRENT_BRANCH, "r");
            fscanf(currentbranch, "%[^\n]s", current);
            if (strcmp(argv[2], "HEAD") == 0) {
                Finding_lastline(commitlog, headaddress, current);
                strcpy(commiting_add, headaddress);
            } else {
                char number[5];
                strcpy(number, argv[2] + 5);
                int n = atoi(number);
                Finding_nlastline(commitlog, headaddress, current, n);
                strcpy(commiting_add, headaddress);
            }
        } else {
            FILE *commitlog = fopen(".dambiz/tracks/commitlog.txt", "r");
            char content[MAX_FILE_SIZE];
            fscanf(commitlog, "%[^\r]s", content);
            char *log = strtok(content, "\n");
            while (log != NULL) {
                if (strstr(log, argv[2])) {
                    strcpy(commiting_add, log);
                    break;
                }
                log = strtok(NULL, "\n");
            }
        }


        DIR *HeadCommit = opendir(commiting_add);
        struct dirent *cf;
        while ((cf = readdir(HeadCommit)) != NULL) {
            if ((strncmp(cf->d_name, ".", 1) != 0) && (strcmp(cf->d_name, "author.txt") != 0) &&
                (strcmp(cf->d_name, "commitmessage.txt") != 0) && (strcmp(cf->d_name, "committime.txt") != 0) &&
                (strcmp(cf->d_name, "log.txt") != 0)) {
                char command[MAX_ADDRESS_SIZE];
                sprintf(command, "cp -r %s/%s .", commiting_add, cf->d_name);
                system(command);
            }
        }
        currentcommit = fopen(Current_Commit, "w");
        fprintf(currentcommit, "%s\n", commiting_add);
        fclose(currentcommit);

        printf("Checkout ran successfully!\n");
    }
}


int run_shortcut(int argc, char **argv) {
    if (strcmp(argv[1], "set") == 0) {
        if ((argc != 6) || (strcmp(argv[2], "-m") != 0) || (strcmp(argv[4], "-s") != 0)) {
            printf("Invalid Command!\n");
            return 1;
        }
        char shortcutpath[MAX_ADDRESS_SIZE];
        sprintf(shortcutpath, ".dambiz/shortcuts/%s.txt", argv[5]);
        FILE *newshort = fopen(shortcutpath, "w");
        fprintf(newshort, "%s\n", argv[3]);
        fclose(newshort);
        printf("Shortcut created successfully!\n");
    } else if (strcmp(argv[1], "replace") == 0) {
        if ((argc != 6) || (strcmp(argv[2], "-m") != 0) || (strcmp(argv[4], "-s") != 0)) {
            printf("Invalid Command!\n");
            return 1;
        }

        DIR *shortcuts = opendir(".dambiz/shortcuts");
        char txtname[MAX_NAME_SIZE];
        sprintf(txtname, "%s.txt", argv[5]);

        if (directory_search(shortcuts, txtname) == 0) {
            printf("Invalid Shortcut! Check your input.\n");
            return 1;
        }

        char shortcutpath[MAX_ADDRESS_SIZE];
        sprintf(shortcutpath, ".dambiz/shortcuts/%s.txt", argv[5]);
        FILE *replacedshort = fopen(shortcutpath, "w");
        fprintf(replacedshort, "%s\n", argv[3]);
        fclose(replacedshort);
        printf("Shortcut replaced successfully!\n");
    } else {
        if ((argc != 4) || (strcmp(argv[2], "-s") != 0)) {
            printf("Invalid Command!\n");
            return 1;
        }
        DIR *shortcuts = opendir(".dambiz/shortcuts");
        char txtname[MAX_NAME_SIZE];
        sprintf(txtname, "%s.txt", argv[3]);
        if (directory_search(shortcuts, txtname) == 0) {
            printf("Invalid Shortcut! Check your input.\n");
            return 1;
        }
        char command[MAX_ADDRESS_SIZE];
        sprintf(command, "rm -r .dambiz/shortcuts/%s.txt", argv[3]);
        system(command);
        printf("Short cut has removed successfully!\n");
    }
}


int run_revert(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }


    char headcommitpath[MAX_ADDRESS_SIZE];
    FILE *currentcommit = fopen(Current_Commit, "r");
    fscanf(currentcommit, "%[^\n]s", headcommitpath);
    fclose(currentcommit);


    if (check_addfolder(".", headcommitpath) == 1) {
        printf("You can not revert because there are changed files that are not committed!\n");
        return 1;
    }


    int ID;
    char newmessage[MAX_ADDRESS_SIZE] = "";

    bool flag = false;
    int X = -1;
    if ((argc == 3) || ((argc == 5) && strcmp(argv[2], "-m") == 0)) {
        flag = true;
        ID = atoi(argv[argc - 1]);
        if (strncmp(argv[argc - 1], "HEAD-", 5) == 0) {
            char x[5];
            strcpy(x, argv[argc - 1] + 5);
            X = atoi(x);
            FILE *counter = fopen(".dambiz/branches/commitcounter.txt", "r");
            fscanf(counter, "%d", &ID);
            fclose(counter);
            ID -= X;
            if (ID < 0) {
                printf("WTF Bro you don't have this commit!\n");
                return 1;
            }
        } else if (ID <= 0) {
            printf("Invalid ID! Check your input.\n");
            return 1;
        }
        if (argc == 5) {
            strcpy(newmessage, argv[3]);
        }
    } else if (strcmp(argv[2], "-n") == 0) {
        if (argc == 4) {
            ID = atoi(argv[3]);
            if (ID == 0) {
                printf("Invalid ID! Check your input.\n");
                return 1;
            }
        } else {
            FILE *counter = fopen(".dambiz/branches/commitcounter.txt", "r");
            fscanf(counter, "%d", &ID);
            fclose(counter);
        }
    }
    FILE *commitlog = fopen(".dambiz/tracks/commitlog.txt", "r");
    char content[MAX_FILE_SIZE];
    fscanf(commitlog, "%[^\r]s", content);
    char logs[20][MAX_ADDRESS_SIZE];
    char *log = strtok(content, "\n");
    int logcount = 0;
    while (log != NULL) {
        strcpy(logs[logcount++], log);
        log = strtok(NULL, "\n");
    }
    char commitpath[MAX_ADDRESS_SIZE];
    char currentID[5];
    sprintf(currentID, "%d", ID);
    for (int i = 0; i < logcount; i++) {
        if (strstr(logs[i], currentID)) {
            strcpy(commitpath, logs[ID - 1]);
            break;
        }
    }

    if (strlen(newmessage) == 0) {
        char messagepath[MAX_ADDRESS_SIZE];
        sprintf(messagepath, "%s/commitmessage.txt", commitpath);
        FILE *lastmessage = fopen(messagepath, "r");
        fscanf(lastmessage, "%[^\n]s", newmessage);
        fclose(lastmessage);
    }

    if (flag) {
        FILE *counter = fopen(".dambiz/branches/commitcounter.txt", "r");
        int commitcounter;
        fscanf(counter, "%d", &commitcounter);
        commitcounter++;
        fclose(counter);
        counter = fopen(".dambiz/branches/commitcounter.txt", "w");
        fprintf(counter, "%d", commitcounter);
        fclose(counter);


        FILE *currentbranch;
        if ((currentbranch = fopen(CURRENT_BRANCH, "r")) == NULL) {
            printf("WTAF\n");
        }

        char current[MAX_ADDRESS_SIZE];
        fscanf(currentbranch, "%[^\n]s", current);
        fclose(currentbranch);

        FILE *commitslog;
        commitslog = fopen(".dambiz/tracks/commitlog.txt", "a");
        fprintf(commitslog, ".dambiz/branches/%s/commits/%d\n", current, commitcounter);
        fclose(commitslog);

        char path[MAX_ADDRESS_SIZE];
        sprintf(path, ".dambiz/branches/%s/commits/%d", current, commitcounter);

        if (mkdir(path, 0755) != 0) {
            perror("oops!\n");
        }


        char copy_command[MAX_FILE_SIZE];
        sprintf(copy_command, "cp -r %s/* %s/", commitpath, path);
        system(copy_command);

        char time_path[MAX_ADDRESS_SIZE];
        char ncommitlog[MAX_ADDRESS_SIZE];
        char commitauthor[MAX_ADDRESS_SIZE];
        sprintf(ncommitlog, "%s/log.txt", path);
        sprintf(time_path, "%s/committime.txt", path);
        sprintf(commitauthor, "%s/author.txt", path);
        strcat(path, "/commitmessage.txt");


        FILE *commitmessage = fopen(path, "w");
        fprintf(commitmessage, "%s\n", newmessage);
        fclose(commitmessage);


        time_t currenttime;
        struct tm *ptr_time;
        char mytime[50];
        char savetime[50];
        time(&currenttime);
        ptr_time = localtime(&currenttime);
        if (strftime(mytime, 50, "%A %Y.%m.%d - %H:%M:%S", ptr_time) == 0) {
            perror("Couldn't prepare time formatted string\n");
        }
        if (strftime(savetime, 50, "%s", ptr_time) == 0) {
            perror("Couldn't prepare time formatted string\n");
        }


        FILE *comauthor = fopen(commitauthor, "w");
        FILE *committime = fopen(time_path, "w");
        fprintf(committime, "%s", savetime);
        fclose(committime);


        FILE *nlog = fopen(ncommitlog, "w");
        char author[MAX_NAME_SIZE];
        FILE *user = fopen(".dambiz/user/localname.txt", "r");
        fscanf(user, "%[^\r]s", author);
        fclose(user);
        fprintf(comauthor, "%s", author);
        fclose(comauthor);
        fprintf(nlog,
                "************************************************************************************************************************************************************************************************************\nCommited at:     %s\nCommit message:     %s\nCommited by:     %s\nCommit ID:     %d\nBranch:     %s\n0 File(s) were commited!\n************************************************************************************************************************************************************************************************************\n",
                mytime, newmessage, author, commitcounter, current);
        printf("Commit ID:   %d\nCommit Message:   %s\nCommitting Time:   %s\n", commitcounter, newmessage,
               mytime);
    }

    DIR *WhereWeAre = opendir(".");
    struct dirent *myf;
    while ((myf = readdir(WhereWeAre)) != NULL) {
        if (strncmp(myf->d_name, ".", 1) != 0) {
            char command[MAX_ADDRESS_SIZE];
            sprintf(command, "rm -r %s", myf->d_name);
            system(command);
        }
    }

    DIR *HeadCommit = opendir(commitpath);
    struct dirent *cf;
    while ((cf = readdir(HeadCommit)) != NULL) {
        if ((strncmp(cf->d_name, ".", 1) != 0) && (strcmp(cf->d_name, "author.txt") != 0) &&
            (strcmp(cf->d_name, "commitmessage.txt") != 0) && (strcmp(cf->d_name, "committime.txt") != 0) &&
            (strcmp(cf->d_name, "log.txt") != 0)) {
            char command[MAX_ADDRESS_SIZE];
            sprintf(command, "cp -r %s/%s .", commitpath, cf->d_name);
            system(command);
        }
    }


    currentcommit = fopen(Current_Commit, "w");
    fprintf(currentcommit, "%s", commitpath);
    fclose(currentcommit);


    printf("Revert ran successfully!\n");


}


int run_grep(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }

    char currentcommit[MAX_ADDRESS_SIZE];
    bool n = false;

    if (argc != 6) {
        if (strcmp(argv[6], "-c") == 0) {
            if (argc >= 8) {
                int ID = atoi(argv[7]);
                FILE *commitlog = fopen(".dambiz/tracks/commitlog.txt", "r");
                char content[MAX_FILE_SIZE];
                fscanf(commitlog, "%[^\r]s", content);
                char logs[20][MAX_ADDRESS_SIZE];
                char *log = strtok(content, "\n");
                int logcount = 0;
                while (log != NULL) {
                    strcpy(logs[logcount++], log);
                    log = strtok(NULL, "\n");
                }
                strcpy(currentcommit, logs[ID - 1]);
            } else {
                printf("Invalid Inputs!\n");
                return 1;
            }
        }
    } else {
        FILE *curcommit = fopen(Current_Commit, "r");
        fscanf(curcommit, "%[^\n]s", currentcommit);
        fclose(curcommit);
    }
    if (strcmp(argv[argc - 1], "-n") == 0) {
        n = true;
    }
    char filepath[MAX_ADDRESS_SIZE];
    sprintf(filepath, "%s/%s", currentcommit, argv[3]);
    char word[MAX_NAME_SIZE];
    strcpy(word, argv[5]);
    FILE *content = fopen(filepath, "r");
    char fcontent[MAX_FILE_SIZE];
    fscanf(content, "%[^\r]s", fcontent);
    char *line = strtok(fcontent, "\n");
    int linecounter = 1;
    while (line != NULL) {
        long int location;
        if (strstr(line, word)) {
            location = strstr(line, word) - line;
            if (n) {
                printf("Line %d: ", linecounter);
            }
            for (unsigned long int i = 0; i < strlen(line); i++) {
                if (i == location) {
                    for (unsigned long int j = i; j < i + strlen(word); j++) {
                        printf("\033[1;31m%c", line[j]);
                    }
                    i += (strlen(word) - 1);
                    printf("\033[0m");
                    if (strstr(line + i, word)) {
                        location = strstr(line + i, word) - line;
                    }
                } else {
                    printf("%c", line[i]);

                }
            }
            printf("\n");
        }
        line = strtok(NULL, "\n");
        linecounter++;
    }
    printf("\n\n\nGrep Completed!\n");
}

int run_tag(int argc, char **argv) {

    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }

    if (argc > 3) {
        char tagpath[MAX_ADDRESS_SIZE];
        sprintf(tagpath, ".dambiz/tags/%s", argv[3]);

        if (strcmp(argv[2], "-a") == 0) {
            char tagname[MAX_NAME_SIZE];
            strcpy(tagname, argv[3]);

            if (strcmp(argv[argc - 1], "-f") != 0) {
                DIR *tag = opendir(".dambiz/tags");
                if (directory_search(tag, argv[3])) {
                    printf("Tag already exist!\n");
                    return 1;
                }
                FILE *tags = fopen(".dambiz/tags/tags.txt", "a");
                fprintf(tags, "%s\n", tagname);
            }

            char tag_message[MAX_NAME_SIZE] = "";
            int ID = 0;
            if (argc > 4) {
                int flag = 4;
                if (strcmp(argv[4], "-m") == 0) {
                    flag = 6;
                    strcpy(tag_message, argv[5]);
                }
                if (argc > flag) {
                    if (strcmp(argv[flag], "-c") == 0) {
                        ID = atoi(argv[flag + 1]);
                    }
                }
            }


            mkdir(tagpath, 0755);


            strcat(tagpath, "/taglog.txt");
            FILE *taglog = fopen(tagpath, "w");

            if (ID == 0) {
                FILE *curID = fopen(Current_Commit, "r");
                char content[MAX_FILE_SIZE];
                fscanf(curID, "%[^\r]", content);
                ID = atoi(strrchr(content, '/') + 1);
            }

            char author[MAX_NAME_SIZE];
            FILE *user = fopen(".dambiz/user/localname.txt", "r");
            fscanf(user, "%[^\r]s", author);
            fclose(user);

            time_t currenttime;
            struct tm *ptr_time;
            char mytime[50];
            time(&currenttime);
            ptr_time = localtime(&currenttime);
            if (strftime(mytime, 50, "%A %Y.%m.%d - %H:%M:%S", ptr_time) == 0) {
                perror("Couldn't prepare time formatted string\n");
            }


            fprintf(taglog, "Tag Name:     %s\nCommit ID:     %d\nUser:     %s\nTagging Date:     %s\n", tagname, ID,
                    author, mytime);

            if (strlen(tag_message) != 0) {
                fprintf(taglog, "Tag Message:     %s\n", tag_message);
            }
            fclose(taglog);


            printf("Tag added successfully!\n");
        } else if (strcmp(argv[2], "show") == 0) {
            strcat(tagpath, "/taglog.txt");
            FILE *taglog = fopen(tagpath, "r");
            char logcontent[MAX_FILE_SIZE];
            fscanf(taglog, "%[^\r]s", logcontent);
            printf("\n%s\n", logcontent);
        } else {
            printf("Invalid inputs! try again.\n");
            return 1;
        }

    } else {
        FILE *alltags = fopen(".dambiz/tags/tags.txt", "r");

        char content[MAX_ADDRESS_SIZE];
        fscanf(alltags, "%[^\r]s", content);
        char tags[50][MAX_NAME_SIZE];
        char *tag = strtok(content, "\n");
        int tagcounter = 0;
        while (tag != NULL) {
            strcpy(tags[tagcounter++], tag);
            tag = strtok(NULL, "\n");
        }
        for (int i = 0; i < tagcounter; i++) {
            char min[] = "zzzzzzzzzzzzzzzzzzzzzzzzzzzz";
            int Max = 0;
            for (int j = 0; j < tagcounter; j++) {
                if (strcmp(min, tags[j]) > 0 && strcmp("", tags[j]) != 0) {
                    strcpy(min, tags[j]);
                    Max = j;
                }
            }
            printf("%s\n", min);
            strcpy(tags[Max], "");
        }
    }
}


int run_diff(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }

    if (strcmp(argv[2], "-f") == 0) {
        char pathfile1[MAX_ADDRESS_SIZE];
        char pathfile2[MAX_ADDRESS_SIZE];
        strcpy(pathfile1, argv[3]);
        strcpy(pathfile2, argv[4]);
        int start1 = 1, start2 = 1, end1 = 0, end2 = 0;

        if (argc > 5) {
            int flag = 5;
            if (strcmp(argv[flag], "-line1") == 0) {
                sscanf(argv[flag + 1], "%d-%d", &start1, &end1);
                flag += 2;
            }
            if (argc > flag) {
                if (strcmp(argv[flag], "-line2") == 0) {
                    sscanf(argv[flag + 1], "%d-%d", &start2, &end2);
                }
            }
        }

        FILE *file1 = fopen(pathfile1, "r");
        FILE *file2 = fopen(pathfile2, "r");

        char filecontent1[1000][1000];
        char filecontent2[1000][1000];

        int line1counter = 0;
        int line2counter = 0;

        while (!feof(file1)) {
            fgets(filecontent1[line1counter++], 1000, file1);
        }

        while (!feof(file2)) {
            fgets(filecontent2[line2counter++], 1000, file2);
        }
        if (end1 == 0 || end1 > line1counter) {
            end1 = line1counter;
        }
        if (end2 == 0 || end2 > line2counter) {
            end2 = line2counter;
        }
        for (int i = start1 - 1, j = start2 - 1; i < end1 && j < end2; i++, j++) {
            if (i >= end1) {
                strcpy(filecontent1[i], "");
            }
            if (j >= end2) {
                strcpy(filecontent2[j], "");
            }
            if (strcmp(filecontent1[i], filecontent2[j]) == 0) {
                continue;
            }
            printf("\n\n************************************************************************************************************************************************************************************************************\n%s-%d\n\033[1;31m%s\033[0m\n%s-%d\n\n\033[1;36m%s\033[0m\n************************************************************************************************************************************************************************************************************\n\n",
                   pathfile1, i + 1, filecontent1[i], pathfile2, j + 1, filecontent2[j]);
        }
    } else if (strcmp(argv[2], "-c") == 0) {
        int commitID1, commitID2;
        commitID1 = atoi(argv[3]);
        commitID2 = atoi(argv[4]);

        FILE *commitlog = fopen(".dambiz/tracks/commitlog.txt", "r");
        char content[MAX_FILE_SIZE];
        fscanf(commitlog, "%[^\r]s", content);
        char logs[20][MAX_ADDRESS_SIZE];
        char *log = strtok(content, "\n");
        int logcount = 0;
        while (log != NULL) {
            strcpy(logs[logcount++], log);
            log = strtok(NULL, "\n");
        }
        char commitpath1[MAX_ADDRESS_SIZE];
        char commitpath2[MAX_ADDRESS_SIZE];
        strcpy(commitpath1, logs[commitID1 - 1]);
        strcpy(commitpath2, logs[commitID2 - 1]);

        DIR *commit1 = opendir(commitpath1);

        struct dirent *dir1;
        while ((dir1 = readdir(commit1)) != NULL) {
            DIR *commit2 = opendir(commitpath2);
            if ((strncmp(dir1->d_name, ".", 1) != 0) &&
                (strcmp(dir1->d_name, "author.txt") != 0) &&
                (strcmp(dir1->d_name, "commitmessage.txt") != 0) &&
                (strcmp(dir1->d_name, "committime.txt") != 0) &&
                (strcmp(dir1->d_name, "log.txt") != 0) && dir1->d_type != DT_DIR) {
                if (directory_search(commit2, dir1->d_name)) {
                    char command[MAX_ADDRESS_SIZE];
                    char commitfile1[MAX_ADDRESS_SIZE];
                    char commitfile2[MAX_ADDRESS_SIZE];

                    sprintf(commitfile1, "%s/%s", commitpath1, dir1->d_name);
                    sprintf(commitfile2, "%s/%s", commitpath2, dir1->d_name);

                    sprintf(command, "dambiz diff -f %s %s", commitfile1, commitfile2);
                    system(command);
                } else {
                    printf("File %s only exists in commit ID: %d\n", dir1->d_name, commitID1);
                }
            }
        }
        DIR *commit2 = opendir(commitpath2);
        struct dirent *dir2;
        while ((dir2 = readdir(commit2)) != NULL) {
            commit1 = opendir(commitpath1);
            if ((strncmp(dir2->d_name, ".", 1) != 0) &&
                (strcmp(dir2->d_name, "author.txt") != 0) &&
                (strcmp(dir2->d_name, "commitmessage.txt") != 0) &&
                (strcmp(dir2->d_name, "committime.txt") != 0) &&
                (strcmp(dir2->d_name, "log.txt") != 0) && dir2->d_type != DT_DIR) {
                if (directory_search(commit1, dir2->d_name) == 0) {
                    printf("File %s only exists in commit ID: %d\n", dir2->d_name, commitID2);
                }
            }
        }
    } else {
        printf("Invalid Command!\n");
    }
}


void readfile(FILE *file, char *content) {
    while (!feof(file)) {
        char line[1000];
        fgets(line, 1000, file);
        strcat(content, line);
    }
}


int todohook(char path[]) {
    if (strstr(path, ".txt") || strstr(path, ".c") || strstr(path, ".cpp")) {
        FILE *file = fopen(path, "r");
        char content[MAX_FILE_SIZE];
        readfile(file, content);
        if (strstr(path, ".txt")) {
            if (strstr(content, "TODO")) {
                int place = strstr(content, "TODO") - content;
                if (place + 4 < strlen(content)) {
                    if (content[place + 4] == ' ') {
                        return 0;
                    }
                    return 1;
                }
                return 0;
            }
            return 0;
        } else {
            char *line = strtok(content, "\n");
            while (line != NULL) {
                if (strstr(line, "//") && strstr(line, "TODO")) {
                    if (strstr(line, "//") < strstr(line, "TODO")) {
                        return 0;
                    }
                }
                line = strtok(NULL, "\n");
            }
            return 1;
        }
    }
    return 2;
}


int blankspacehook(char path[]) {
    if (strstr(path, ".txt") || strstr(path, ".c") || strstr(path, ".cpp")) {
        FILE *file = fopen(path, "r");
        char lines[200][1000];
        int linecounter = 0;
        while (!feof(file)) {
            fgets(lines[linecounter++], 1000, file);
        }
        if (strtok(lines[linecounter - 2], " \n") == NULL) {
            return 0;
        }
        return 1;
    }
    return 2;
}

int formatcheckhook(char path[]) {
    if (strstr(path + 1, ".") == NULL) {
        return 2;
    } else if (strstr(path, ".txt") || strstr(path, ".c") || strstr(path, ".cpp") || strstr(path, ".mp4") ||
               strstr(path, ".wav") || strstr(path, ".mp3") || strstr(path, ".jpg")) {
        return 1;
    }
    return 0;
}


int balancebraceshook(char path[]) {
    if (strstr(path, ".txt") || strstr(path, ".c") || strstr(path, ".cpp")) {
        FILE *file = fopen(path, "r");
        char content[MAX_FILE_SIZE];
        readfile(file, content);
        int bo = 0, po = 0, ko = 0, be = 0, pe = 0, ke = 0;
        for (int i = 0; i < strlen(content); i++) {
            if (content[i] == '(') {
                po++;
            } else if (content[i] == ')') {
                pe++;
            } else if (content[i] == '[') {
                bo++;
            } else if (content[i] == ']') {
                be++;
            } else if (content[i] == '{') {
                ko++;
            } else if (content[i] == '}') {
                ke++;
            }
        }
        if (be == bo && pe == po && ke == ko) {
            return 1;
        }
        return 0;
    }
    return 2;
}

int staticerror(char path[]) {
    struct stat x;
    if (strstr(path, ".c") || strstr(path, ".cpp")) {
        char command[MAX_ADDRESS_SIZE];
        sprintf(command, "gcc %s -o test 2>/dev/null", path);
        if (system(command) == 0) {
            return 1;
        } else {
            return 0;
        }
    }
    return 2;
}

int sizecheckhook(char path[]) {
    FILE *file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    unsigned long long int size = ftell(file);
    if (size > 5000000) {
        return 0;
    }
    return 1;
}


int run_precommit(int argc, char **argv) {
    if (check_init() == 0) {
        perror("You have not initialized in this folder or its parents yet.");
        return 1;
    }
    if (argc == 2 || (argc > 2 && strcmp(argv[2], "-f") == 0)) {

        int failed = 0;

        DIR *hookdir = opendir(".dambiz/hooks");
        char hooks[8][MAX_NAME_SIZE];
        int hookcount = 0;
        struct dirent *hook;
        while ((hook = readdir(hookdir)) != NULL) {
            if (strncmp(hook->d_name, ".", 1) == 0) {
                continue;
            }
            strcpy(hooks[hookcount++], hook->d_name);
        }


        if (argc == 2) {


            DIR *stage = opendir(Staging);
            char stagedfiles[20][MAX_ADDRESS_SIZE];
            char names[20][MAX_NAME_SIZE];
            int stagedcount = 0;
            struct dirent *staged;
            while ((staged = readdir(stage)) != NULL) {
                struct stat x;
                if (strncmp(staged->d_name, ".", 1) == 0) {
                    continue;
                }
                sprintf(stagedfiles[stagedcount], Staging);
                strcpy(names[stagedcount], staged->d_name);
                strcat(stagedfiles[stagedcount], "/");
                strcat(stagedfiles[stagedcount++], staged->d_name);
            }




            for (int i = 0; i < stagedcount; i++) {
                printf("**************************************************************************************\n%s:\n\n",
                       names[i]);
                for (int j = 0; j < hookcount; j++) {
                    int flag = 2;
                    if (strstr(hooks[j], "todo-check")) {
                        printf("todo-check ....................................................................");
                        flag = todohook(stagedfiles[i]);
                    } else if (strstr(hooks[j], "eof-blank-space")) {
                        printf("eof-blank-space................................................................");
                        flag = blankspacehook(stagedfiles[i]);
                    } else if (strstr(hooks[j], "format-check")) {
                        printf("format-check...................................................................");
                        flag = formatcheckhook(stagedfiles[i]);
                    } else if (strstr(hooks[j], "balance-braces")) {
                        printf("balance-braces.................................................................");
                        flag = balancebraceshook(stagedfiles[i]);
                    } else if (strstr(hooks[j], "static-error-check")) {
                        printf("static-error-check.............................................................");
                        flag = staticerror(stagedfiles[i]);
                    } else if (strstr(hooks[j], "file-size-check")) {
                        printf("file-size-check................................................................");
                        flag = sizecheckhook(stagedfiles[i]);
                    } else if (strstr(hooks[j], "character-limit")) {
                        printf("character-limit................................................................");
                        if (strstr(stagedfiles[i], ".txt") || strstr(stagedfiles[i], ".c") || strstr(stagedfiles[i], ".cpp")) {
                            flag = sizecheckhook(stagedfiles[i]);
                        } else{
                            flag = 2;
                        }
                    }else {
                        printf("Invalid hook!\n");
                        continue;
                    }
                    if (flag == 0) {
                        printf("\033[0;31mFAILED\033[0;0m\n\n");
                    } else if (flag == 1) {
                        failed++;
                        printf("\033[0;32mPASSED\033[0;0m\n\n");
                    } else {
                        printf("\033[0;36mSKIPPED\033[0;0m\n\n");
                    }
                }
            }
            if (failed){
                printf("DO NOT COMMIT UNLESS YOU ARE SURE ABOUT IT!\n");
            }
        } else{
            for (int i = 3; i < argc; i++) {
                char newpath[MAX_ADDRESS_SIZE];
                printf("**************************************************************************************\n%s:\n\n",
                       argv[i]);
                sprintf(newpath, ".dambiz/staging/%s", argv[i]);
                for (int j = 0; j < hookcount; j++) {
                    int flag = 2;
                    if (strstr(hooks[j], "todo-check")) {
                        printf("todo-check ....................................................................");
                        flag = todohook(newpath);
                    } else if (strstr(hooks[j], "eof-blank-space")) {
                        printf("eof-blank-space................................................................");
                        flag = blankspacehook(newpath);
                    } else if (strstr(hooks[j], "format-check")) {
                        printf("format-check...................................................................");
                        flag = formatcheckhook(newpath);
                    } else if (strstr(hooks[j], "balance-braces")) {
                        printf("balance-braces.................................................................");
                        flag = balancebraceshook(newpath);
                    } else if (strstr(hooks[j], "static-error-check")) {
                        printf("static-error-check.............................................................");
                        flag = staticerror(newpath);
                    } else if (strstr(hooks[j], "file-size-check")) {
                        printf("file-size-check................................................................");
                        flag = sizecheckhook(newpath);
                    } else if (strstr(hooks[j], "character-limit")) {
                        printf("character-limit................................................................");
                        if (strstr(newpath, ".txt") || strstr(newpath, ".c") || strstr(newpath, ".cpp")) {
                            flag = sizecheckhook(newpath);
                        } else{
                            flag = 2;
                        }
                    }else {
                        printf("Invalid hook!\n");
                        continue;
                    }
                    if (flag == 0) {
                        printf("\033[0;31mFAILED\033[0;0m\n\n");
                    } else if (flag == 1) {
                        failed++;
                        printf("\033[0;32mPASSED\033[0;0m\n\n");
                    } else {
                        printf("\033[0;36mSKIPPED\033[0;0m\n\n");
                    }
                }
            }
            if (failed){
                printf("DO NOT COMMIT UNLESS YOU ARE SURE ABOUT IT!\n");
            }
        }
    }
    if (argc == 4) {
        if ((strcmp(argv[2], "hooks") == 0) && (strcmp(argv[3], "list") == 0)) {
            printf("\n\n************************************************************************************************************************************************************************************************************\n"
                   "                                                                                             HOOKS LIST\n\n"
                   "\033[1;32mtodo-check\033[1;0m for .txt, .c and .cpp formats\n\n"
                   "\033[1;32meof-blank-space\033[1;0m for .txt, .c and .cpp formats\n\n"
                   "\033[1;32mformat-check\033[1;0m for valid formats\n\n"
                   "\033[1;32mbalance-braces\033[1;0m for .txt, .c and .cpp formats\n\n"
                   "\033[1;32mindentation-check\033[1;0m for .cpp and .c formats\n\n"
                   "\033[1;32mstatic-error-check\033[1;0m for .cpp and c formats\n\n"
                   "\033[1;32mfile-size-check\033[1;0m for all formats\n\n"
                   "\033[1;32mcharacter-limit\033[1;0m for .txt, .c and .cpp formats\n\n"
                   "\033[1;32mtime-limit\033[1;0m for .mp4, .mp3 and .wav formats\n\n"
                   "************************************************************************************************************************************************************************************************************\n\n");
        } else if ((strcmp(argv[2], "applied") == 0) && (strcmp(argv[3], "hooks") == 0)) {
            printf("\n\n************************************************************************************************************************************************************************************************************\n"
                   "                                                                                            APPLIED HOOKS\n\n");
            DIR *hooks = opendir(".dambiz/hooks");
            struct dirent *hook;
            while ((hook = readdir(hooks)) != NULL) {
                if (hook->d_type != DT_DIR) {
                    printf("\033[1;32m%s\033[1;0m\n\n", hook->d_name);
                }
            }
            printf("************************************************************************************************************************************************************************************************************\n\n");
        }
    } else if (argc == 5) {
        if ((strcmp(argv[2], "add") == 0) && (strcmp(argv[3], "hook") == 0)) {
            char path[MAX_ADDRESS_SIZE];
            sprintf(path, ".dambiz/hooks/%s", argv[4]);
            FILE *hook = fopen(path, "w");
            fclose(hook);
            printf("Hook added successfully!\n");
        } else if ((strcmp(argv[2], "remove") == 0) && (strcmp(argv[3], "hook") == 0)) {
            char path[MAX_ADDRESS_SIZE];
            sprintf(path, ".dambiz/hooks/%s", argv[4]);
            char command[MAX_ADDRESS_SIZE];
            sprintf(command, "rm -r %s", path);
            system(command);
            printf("Hook removed successfully!\n");
        } else {
            printf("Invalid Command!\n");
        }
    } else {

    }
}