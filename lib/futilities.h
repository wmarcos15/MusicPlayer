#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define MAX_SIZE 500
#define BUFFER_SIZE 200

struct MDIR {
	char name[MAX_SIZE];
	char genre[MAX_SIZE];
};

int checkIfDir(char *name);
int getNumberOfDirs(DIR* dir);
void assignName(struct MDIR* dest, char* path);
void assignGenre(struct MDIR* array, int len);
void loadDirArray(char *path);
int getIndex();
int launchExplorer(char* music_dir, int buffer_size, struct MDIR array[], int len);
void GetSongName(char *path, int path_len, char *destination);
void GetDirName(char *path, int path_len, char *destination);
