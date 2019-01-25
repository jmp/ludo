#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "script.h"
#include "error.h"
#include "utils.h"

/* Lua state */
static lua_State *L;

static int dummy(lua_State *L) {
	printf("dummy here\n");
	return 1;
}

static const luaL_Reg lib1[] = {
	{"func1", dummy},
	{"func2", dummy},
	{0, 0}
};

static const luaL_Reg lib2[] = {
	{"func3", dummy},
	{"func4", dummy},
	{0, 0}
};

/*
 * Load all the C functions in a Lua table.
 */
static int load_functions(lua_State *L) {
	lua_newtable(L);
	luaL_newlib(L, lib1);
	lua_setfield(L, -2, "lib1");
	luaL_newlib(L, lib2);
	lua_setfield(L, -2, "lib2");
	return 1;
}

/*
 * Set "package.path" for the Lua interpreter.
 */
 static void set_package_path(lua_State *L) {
 	static char stored_path[PATH_MAX_LENGTH] = { '\0' };
	static const char path_sep[] = { PATH_SEP, '\0' };
	const char *asset_dir = utils_asset_dir();

	/* Package paths relative to the binary directory */
	const char *paths[] = {
		"?.lua;",
		"?/init.lua;",
		"data/?.lua;",
		"data/?/init.lua;",
		"data/scripts/?.lua;",
		"data/scripts/?/init.lua;",
		NULL
	};

	/* Append binary-relative directories to the path */
	int i;
	for (i = 0; paths[i]; ++i) {
		utils_strlcat(stored_path, asset_dir, sizeof(stored_path));
		utils_strlcat(stored_path, paths[i], sizeof(stored_path));
	}

#ifdef ASSET_DIRECTORY
	/* Optional directory defined at compile-time */
	for (i = 0; paths[i]; ++i) {
		utils_strlcat(stored_path, asset_dir, sizeof(stored_path));
		utils_strlcat(
			stored_path,
			ASSET_DIRECTORY "?.lua;" ASSET_DIRECTORY "?/init.lua;",
			sizeof(stored_path)
		);
	}
#endif

 	/* Fetch package.path */
 	lua_getglobal(L, "package");
 	lua_getfield(L, -1, "path");

 	/* Replace the path in the stack */
 	lua_pop(L, 1);
 	lua_pushstring(L, stored_path);
 	lua_setfield(L, -2, "path");
 	lua_pop(L, 1);
 }


/*
 * Initializes the Lua state.
 */
void script_init(void) {
	static const luaL_Reg bindings[] = {
		{"ludo", load_functions},
		{0, 0}
	};

	if (L) {
		error("Lua state has already been initialized!\n");
	}

	/* Create the Lua state */
	if (!(L = luaL_newstate())) {
		error("Failed to initialize Lua state!\n");
	}

	/* Open standard libraries */
	luaL_openlibs(L);

	/* Set the path from which modules will be loaded */
	set_package_path(L);

	/* Load native bindings */
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	luaL_setfuncs(L, bindings, 0);
	lua_pop(L, 2);
}

/*
 * Start executing Lua scripts. This works by requiring the "init"
 * module which should be a file called "init.lua" somewhere in the
 * search path.
 */
void script_run(void) {
	if (!L) {
		error("Lua state has not been initialized!\n");
	}

	/* Run and print an error in case something went wrong */
	if (luaL_dostring(L, "require \"init\"")) {
		error("%s\n", lua_tostring(L, -1));
	}
}

/*
 * Destroy the Lua state.
 */
void script_quit(void) {
	if (!L) {
		error("Lua state has already been destroyed!\n");
	}

	/* Close Lua */
	lua_close(L);
}
