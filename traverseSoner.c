#include <dirent.h>
#define ABSOLUTE_PATH_LEN 256
int
postOrderApply (char *path) {
    DIR             *dir;
    struct dirent   *entry;
    size_t          len = strlen(path);
    int             totalFileSize = 0;

    dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "Path not found -- \"%s\": %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char const *name = entry->d_name;

        path[len] = '/';
        path[len + 1] = '\0';
        strncat(path + len + 1, name, ABSOLUTE_PATH_LEN - len - 2);
        if (entry->d_type == DT_DIR) {
            if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
                continue;
            }
            
            postOrderApply(path);

        } else {    // regular file
            printf("Regular file \"%s\"\n", name);
        }
    }
    path[len] = '\0';
    printf("%s\n", path);
    closedir(dir);
    return 0;
}
