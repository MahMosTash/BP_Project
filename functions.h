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


int check_init();


int run_config(int argc, char **argv);


int run_init(int argc, char **argv);




void create_essentials();


int run_add(int argc, char **argv);


int directory_search(DIR *current, char name[]);


int is_identical(char address1[], char address2[]);


int check_addfolder(char folderaddress[], char stageaddress[]);


int FindHead(DIR *branch);


int run_reset(int argc, char **argv);


void redo();


void addn();


int run_status(int argc, char **argv);


int run_commit(int argc, char **argv);


int EmptyFolderCheck(DIR *checkingfolder);


int run_log(int argc, char **argv);


int run_branch(int argc, char **argv);


void Finding_lastline(FILE *given_file, char *str, char *current_branch);


void Finding_nlastline(FILE *given_file, char *str, char *current_branch, int number);


int run_checkout(int argc, char **argv);


int run_shortcut(int argc, char **argv);


int run_revert(int argc, char**argv);


int run_grep(int argc, char **argv);


int run_tag(int argc, char **argv);

int run_diff(int argc, char **argv);


void readfile(FILE* file, char *content);


int todohook(char path[]);

int blankspacehook(char path[]);

int formatcheckhook(char path[]);

int balancebraceshook(char  path[]);

int staticerror(char path[]);

int sizecheckhook(char path[]);


int run_precommit(int argc, char **argv);
#ifndef BP_PROJECT_FUNCTIONS_H
#define BP_PROJECT_FUNCTIONS_H

#endif //BP_PROJECT_FUNCTIONS_H
