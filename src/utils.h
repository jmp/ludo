#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#define PATH_MAX_LENGTH 4096  /* This ought to be enough for anybody! */
#define PATH_SEP_WINDOWS '\\'
#define PATH_SEP_GENERIC '/'

#ifdef _WIN32
#define PATH_SEP PATH_SEP_WINDOWS
#else
#define PATH_SEP PATH_SEP_GENERIC
#endif

extern void *utils_memset(void *ptr, int value, size_t num);
extern size_t utils_strlcat(char *dst, const char *src, size_t dsize);
extern size_t utils_strlcpy(char *dst, const char *src, size_t dsize);
extern void utils_path_to_native(char *path);
extern void utils_path_to_generic(char *path);
extern const char *utils_asset_dir(void);
extern const char *utils_script_path(void);

#endif
