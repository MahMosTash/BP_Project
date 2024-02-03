#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>

#include "functions.c"


int run_config(int argc, char **argv);

int run_init(int argc, char **argv);

int check_init();

void create_essentials();

int run_add(int argc, char **argv);

int directory_search(DIR *current, char name[]);

int is_identical(char address1[], char address2[]);

int check_addfolder(char folderaddress[], char stageaddress[]);

int run_reset(int argc, char **argv);

void redo();

void addn();

int run_status(int argc, char **argv);

int run_commit(int argc, char **argv);

int EmptyFolderCheck(DIR *checkingfolder);

int run_log(int argc, char **argv);

int run_branch(int argc, char **argv);

void Finding_lastline(FILE *given_file, char *str, char* current_branch);

void Finding_nlastline(FILE *given_file, char *str, char* current_branch, int number);


int FindHead(DIR * branch);


int run_checkout(int argc, char **argv);





#ifndef BP_PROJECT_FUNCTIONS_H
#define BP_PROJECT_FUNCTIONS_H

#endif //BP_PROJECT_FUNCTIONS_H
