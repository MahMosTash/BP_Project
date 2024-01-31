#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "functions.c"




int run_config(int argc, char **argv);

int run_init(int argc, char **argv);

int check_init();

void create_essentials();

int run_add(int argc, char **argv);

int directory_search(DIR* current, char name[]);

int is_identical(char address1[], char address2[]);

int check_addfolder(char folderaddress[], char stageaddress[]);

int run_reset(int argc, char **argv);


#ifndef BP_PROJECT_FUNCTIONS_H
#define BP_PROJECT_FUNCTIONS_H

#endif //BP_PROJECT_FUNCTIONS_H
