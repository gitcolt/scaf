#define STR_UTILS_IMPL
#include "str_utils.h"
#include "ansi_colors.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <assert.h>
#include <sys/stat.h>

static const char *prog_name = "scaf";
static const char *scaffolding_dir_root = ".scaf";
static const char *scaf_default_filename = ".scaf_default";

void print_usage() {
    fprintf(stderr, "Usage: %s DIR\n", prog_name);
}

void read_default_dir(char *dst, FILE *fp) {
    if (!fgets(dst, 99, fp)) {
        fprintf(stderr,
                YEL_TXT "No default directory specified in %s. Stopping\n" RST_COL,
                scaf_default_filename);
        return;
    }
    trim(dst);
}

bool valid_default_dir(const char *containing_dir, const char *dir) {
    bool default_dir_found = false;

    DIR *dp = opendir(containing_dir);
    if (!dp) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    struct dirent *dent;
    char buf[99];
    while ((dent = readdir(dp))) {
        if (!strcmp(dent->d_name, ".") ||
            !strcmp(dent->d_name, "..") ||
            dent->d_name[0] == '.')
            continue;
        struct stat ss;
        strcpy(buf, containing_dir);
        strcat(buf, "/");
        strcat(buf, dent->d_name);
        if (stat(buf, &ss) < 0) {
            printf("buf: %s\n", buf);
            perror("stat()");
            exit(EXIT_FAILURE);
        }
        if (!S_ISDIR(ss.st_mode)) {
            printf(MAG_TXT "'%s' is not a directory\n" RST_COL, dent->d_name);
            continue;
        }

        if (!strcmp(dent->d_name, dir))
            default_dir_found = true;
    }
    closedir(dp);

    return default_dir_found;
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 2) {
        print_usage();
        exit(EXIT_FAILURE);
    }

    const char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
        fprintf(stderr, YEL_TXT "HOME environment variable not set\n" RST_COL);
        exit(EXIT_FAILURE);
    }

    char scaffolding_dir[99];
    strcpy(scaffolding_dir, argv[1]);

    char buf[99];
    sprintf(buf, "%s/%s/%s/%s",
            home_dir,
            scaffolding_dir_root,
            scaffolding_dir,
            scaf_default_filename);
    FILE *fp = fopen(buf, "r");
    if (fp) {
        printf("'%s' file found!\n", scaf_default_filename);
        char default_dirname[99];
        read_default_dir(default_dirname, fp);
        fclose(fp);

        if (!*default_dirname) { // empty
            fprintf(stderr,
                    YEL_TXT "No default directory specified in %s. Stopping\n" RST_COL,
                    scaf_default_filename);
            exit(EXIT_FAILURE);
        }

        sprintf(buf, "%s/%s/%s",
                home_dir,
                scaffolding_dir_root,
                scaffolding_dir);
        if (!valid_default_dir(buf, default_dirname)) {
            fprintf(stderr,
                    YEL_TXT "Could not find directory '%s' specified in '%s'."
                    " Stopping\n" RST_COL,
                    default_dirname,
                    scaf_default_filename);
            exit(EXIT_FAILURE);
        }

        strcat(scaffolding_dir, "/");
        strcat(scaffolding_dir, default_dirname);
        printf("Using specified default directory '%s'\n",
                default_dirname);
    }

    sprintf(buf, "cp -rn %s/%s/%s/* .",
            home_dir, scaffolding_dir_root, scaffolding_dir);
    printf("%s\n", buf);
    system(buf);
}
