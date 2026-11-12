
#include "repository.h"
#include <errno.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/types.h>
#endif

int repository_exists(const char *path)
{
    struct stat info;

    if (path == NULL || path[0] == '\0')
        return 0;

    return stat(path, &info) == 0 &&
           (info.st_mode & S_IFDIR) != 0;
}

int initialize_repository(const char *path)
{
    if (path == NULL || path[0] == '\0')
        return 0;

    if (repository_exists(path))
        return 1;

#ifdef _WIN32
    if (_mkdir(path) == 0)
        return 1;
#else
    if (mkdir(path, 0777) == 0)
        return 1;
#endif

    return errno == EEXIST && repository_exists(path);
}
