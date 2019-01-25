#include <SDL.h>
#include "utils.h"

/*
 * Fills the given memory location with the given value.
 */
void *utils_memset(void *ptr, int value, size_t num) {
	return SDL_memset(ptr, value, num);
}

 /*
  * Appends src to string dst of size dsize.
  */
size_t utils_strlcat(char *dst, const char *src, size_t dsize) {
	return SDL_strlcat(dst, src, dsize);
}

/*
 * Copy string src to buffer dst of size dsize.
 */
size_t utils_strlcpy(char *dst, const char *src, size_t dsize) {
	return SDL_strlcpy(dst, src, dsize);
}

/*
 * Replaces all characters in the string with another one.
 */
static void utils_replace_char(char *str, char old_char, char new_char) {
	size_t i;
	for (i = 0; i < SDL_strlen(str); ++i) {
		if (str[i] == old_char) {
			str[i] = new_char;
		}
	}
}

void utils_path_to_native(char *path) {
	utils_replace_char(path, PATH_SEP_GENERIC, PATH_SEP_WINDOWS);
}

void utils_path_to_generic(char *path) {
	utils_replace_char(path, PATH_SEP_WINDOWS, PATH_SEP_GENERIC);
}

/*
 * Returns the directory where the assets (scripts, images, etc.)
 * should be loaded from.
 */
const char *utils_asset_dir(void) {
	static char asset_dir[PATH_MAX_LENGTH] = { '\0' };
	const char *base_path = SDL_GetBasePath();
	char *last_slash;
	size_t i;

	if (SDL_strlen(asset_dir)) {
		return asset_dir;
	}

	SDL_memset(asset_dir, 0, sizeof(asset_dir));
	SDL_strlcat(asset_dir, base_path, sizeof(asset_dir));

	/* Normalize path separators to slashes */
	utils_path_to_generic(asset_dir);

	/* Find the last path separator */
	last_slash = SDL_strrchr(asset_dir, PATH_SEP_GENERIC);

	/* Convert back to native path separators */
	utils_path_to_native(asset_dir);

	/* Truncate after the last slash */
	if (last_slash) {
		*(last_slash + 1) = '\0';
	}

	return asset_dir;
}

/*
 * Returns the script path for Lua.
 */
const char *utils_script_path(void) {
	static char script_path[PATH_MAX_LENGTH] = { '\0' };
	const char *asset_dir = utils_asset_dir();

	if (SDL_strlen(script_path)) {
		return script_path;
	}

	SDL_memset(script_path, 0, sizeof(script_path));
	SDL_strlcat(script_path, asset_dir, sizeof(script_path));
	SDL_strlcat(script_path, "?.lua", sizeof(script_path));

	return script_path;
}
