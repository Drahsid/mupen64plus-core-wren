#include <wren.h>
#include <stdio.h>
#include <direct.h>
#include "dirent.h"
#include "api/m64p_memaccess.h"

#define MAX_FILE_SIZE 134217728

void rdramRead8(WrenVM* vm) {
	uint8_t result = read_rdram_8(wrenGetSlotDouble(vm, 1));
	wrenSetSlotDouble(vm, 0, result);
}

void rdramRead16(WrenVM* vm) {
	uint16_t result = read_rdram_16(wrenGetSlotDouble(vm, 1));
	wrenSetSlotDouble(vm, 0, result);
}

void rdramRead32(WrenVM* vm) {
	uint32_t result = read_rdram_32(wrenGetSlotDouble(vm, 1));
	wrenSetSlotDouble(vm, 0, result);
}

void rdramReadBuffer(WrenVM* vm) {
	uint32_t result = read_rdram_buffer(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
	wrenSetSlotDouble(vm, 0, result);
}

void rdramWrite8(WrenVM* vm) {
	write_rdram_8(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
}

void rdramWrite16(WrenVM* vm) {
	write_rdram_16(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
}

void rdramWrite32(WrenVM* vm) {
	write_rdram_32(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
}

void romRead8(WrenVM* vm) {
	uint32_t result = read_rom_8(wrenGetSlotDouble(vm, 1));
	wrenSetSlotDouble(vm, 0, result);
}

void romRead16(WrenVM* vm) {
	uint32_t result = read_rom_16(wrenGetSlotDouble(vm, 1));
	wrenSetSlotDouble(vm, 0, result);
}

void romRead32(WrenVM* vm) {
	uint32_t result = read_rom_32(wrenGetSlotDouble(vm, 1));
	wrenSetSlotDouble(vm, 0, result);
}

void romReadBuffer(WrenVM* vm) {
	uint32_t result = read_rom_buffer(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
	wrenSetSlotDouble(vm, 0, result);
}

void romWrite8(WrenVM* vm) {
	write_rom_8(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
}

void romWrite16(WrenVM* vm) {
	write_rom_16(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
}

void romWrite32(WrenVM* vm) {
	write_rom_32(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2));
}

void wPrint(WrenVM* vm, const char* str) {
	printf("%s", str);
}

void wError(WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message)
{
	if (type == WREN_ERROR_COMPILE)
		printf("ERROR In module %s on line %d: %s\n", module, line, message);
	else if (type == WREN_ERROR_RUNTIME)
		printf("Runtime error: %s\n", message);
	else
		printf("ERROR: %s:%d %s\n", module, line, message);
}

int loadModule(const char* path, char* buffer) {
	unsigned int len = 0;
	unsigned int size = MAX_FILE_SIZE;

	char* source = malloc(MAX_FILE_SIZE);
	if (!source)
	{
		printf("Could not allocate memory!\n");
		return 0;
	}

	FILE* file = fopen(path, "r");
	while (!feof(file))
	{
		int c = fgetc(file);
		if (c != EOF)
		{
			source[len++] = (char)c;
			if (len == size)
			{
				char* t = realloc(source, 2 * size);
				if (!t)
				{
					free(source);
					return;
				}
				source = t;
				size *= 2;
			}
		}
	}

	strcpy(buffer, source);
	fclose(file);
	free(source);
}

char* wLoadModule(WrenVM* vm, const char* name) //Untested; got from elsewhere. Not sure what calls this, internal imports maybe?
{
	char* path = 0;
	unsigned int pathLen = strlen("scripts/") + strlen(name) + strlen(".wren") + 1;
	unsigned int len = 0;
	unsigned int size = MAX_FILE_SIZE;

	path = malloc(pathLen);
	if (!path)
	{
		printf("Could not allocate memory for module path.\n");
		return 0;
	}

	strcpy(path, "scripts/");
	strcat(path, name);
	strcat(path, ".wren");

	char* source = malloc(MAX_FILE_SIZE);
	if (!source)
	{
		free(path);
		printf("Could not allocate memory for module source.\n");
		return 0;
	}

	FILE* file = fopen(path, "r");
	while (!feof(file))
	{
		int c = fgetc(file);
		if (c != EOF)
		{
			source[len++] = (char)c;
			if (len == size)
			{
				char* t = realloc(source, 2 * size);
				if (!t)
				{
					free(source);
					free(path);
					return 0;
				}
				source = t;
				size *= 2;
			}
		}
	}

	fclose(file);
	free(path);
	return source; // We don't free this... dangerous? 
}

WrenForeignMethodFn wBindForeignMethod(WrenVM* vm, const char* module, const char* className, bool isStatic, const char* signature)
{
	if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("rdramRead8(_)", signature) == 0)
		return rdramRead8;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("rdramRead16(_)", signature) == 0)
		return rdramRead16;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("rdramRead32(_)", signature) == 0)
		return rdramRead32;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("rdramReadBuffer(_,_)", signature) == 0)
		return rdramReadBuffer;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("rdramWrite8(_,_)", signature) == 0)
		return rdramWrite8;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("rdramWrite16(_,_)", signature) == 0)
		return rdramWrite16;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("rdramWrite32(_,_)", signature) == 0)
		return rdramWrite32;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("romRead8(_)", signature) == 0)
		return romRead8;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("romRead16(_)", signature) == 0)
		return romRead16;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("romRead32(_)", signature) == 0)
		return romRead32;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("romReadBuffer(_,_)", signature) == 0)
		return romReadBuffer;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("romWrite8(_,_)", signature) == 0)
		return romWrite8;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("romWrite16(_,_)", signature) == 0)
		return romWrite16;
	else if (strcmp("emulator", module) == 0 && strcmp("mupen", className) == 0 && strcmp("romWrite32(_,_)", signature) == 0)
		return romWrite32;

	return 0;
}

WrenForeignClassMethods wBindForeignClass(WrenVM* vm, const char* module, const char* className)
{
	WrenForeignClassMethods methods;
	memset(&methods, 0, sizeof(methods));
	return methods;
}

inline const char* getExt(const char *filename) {
	const char* p = strrchr(filename, '.');
	if (!p || p == filename) return "";
	return p + 1;
}

void initWrenConfig(WrenConfiguration* config) 
{
	config->loadModuleFn = wLoadModule;
	config->writeFn = wPrint;
	config->errorFn = wError;
	config->bindForeignClassFn = wBindForeignClass;
	config->bindForeignMethodFn = wBindForeignMethod;
}

void loadGameScripts(WrenVM* vm) {
	DIR* dir = opendir("scripts");
	if (dir == NULL) {
		printf("Creating scripts directory\n");
		mkdir("scripts");
	}
	else closedir(dir);

	const char gameScriptsDir[MAX_PATH] = "scripts/";
	strcat(gameScriptsDir, ROM_SETTINGS.goodname);

	dir = opendir(gameScriptsDir);
	if (dir == NULL) {
		printf("Creating game script dir for %s\n", ROM_SETTINGS.goodname);
		mkdir(gameScriptsDir);
		dir = opendir(gameScriptsDir);
	}

	dirent* dp;
	while (true)
	{
		dp = readdir(dir);
		if (dp == NULL) break;

		size_t len = strlen(dp->d_name);
		const char* fileExt = getExt(dp->d_name);

		if (strcmp(fileExt, "wren") == 0) {
			printf("Extension is .wren!\n");
			char* thisScript = malloc(MAX_FILE_SIZE);
			const char* thisScriptDir = malloc(MAX_PATH);
			memset(thisScript, 0, MAX_FILE_SIZE);
			memset(thisScriptDir, 0, MAX_PATH);
			
			strcat(thisScriptDir, gameScriptsDir);
			strcat(thisScriptDir, "/");
			strcat(thisScriptDir, dp->d_name);

			printf("Loading script at dir: %s\n", thisScriptDir);

			loadModule(thisScriptDir, thisScript);
			wrenInterpret(vm, "emulator", thisScript);

			free(thisScript);
			free(thisScriptDir);
		}
	}

	closedir(dir);
}


