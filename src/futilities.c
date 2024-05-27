#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "../lib/raylib.h"

#define MAX_SIZE 500
#define BUFFER_SIZE 200

struct MDIR {
	char name[MAX_SIZE];
	char genre[MAX_SIZE];
};

int checkIfDir(char *name) {
	for (int i = 0; i < strlen(name); i++) {
		if (name[i] == '.') {
			return 0;
		}
	}
	return 1;
}

int getNumberOfDirs(DIR* dir) {
	int count = 0;
	struct dirent *ent;

	if (dir != NULL) {
		while ((ent=readdir(dir)) != NULL) {
			if (checkIfDir(ent->d_name)) {
				count++;
			}
		}
	} else {
		printf("ERROR: directory not found\n");
	}

	return count;
}

void assignName(struct MDIR* dest, char* path) {
	struct dirent *ent;
	int i = 0;
	DIR* dir = opendir(path);
	while ((ent=readdir(dir)) != NULL) {
		if (checkIfDir(ent->d_name)) {
			strcpy(dest[i].name, ent->d_name);
			i++;
		}
	}
	closedir(dir);
}

void assignGenre(struct MDIR* array, int len) {
	for (int i = 0; i < len; i++) {
		if (!strcmp(array[i].name, "CAS")) {
			strcpy(array[i].genre, "Dreampop");
		}
		else if (!strcmp(array[i].name, "Rap")) {
			strcpy(array[i].genre, "Rap");
		}
	}
}

void loadDirArray(char *path) {
	DIR* dir = opendir(path);
	int n = getNumberOfDirs(dir);

	struct MDIR array[n];
	assignName(array, path);
	assignGenre(array, n);
	for (int i = 0; i < n; i++) {
		printf("%s, %s\n", array[i].name, array[i].genre);
	}
	closedir(dir);
}

int getIndex() {
	
	if (IsKeyPressed(KEY_ZERO)) {
		return 0;
	}
	else if (IsKeyPressed(KEY_ONE)) {
		return 1;
	}
	else if (IsKeyPressed(KEY_TWO)) {
		return 2;
	}
	else if (IsKeyPressed(KEY_THREE)) {
		return 3;
	}
	else if (IsKeyPressed(KEY_FOUR)) {
		return 4;
	}
	else if (IsKeyPressed(KEY_FIVE)) {
		return 5;
	}
	else if (IsKeyPressed(KEY_SIX)) {
		return 6;
	}
	else if (IsKeyPressed(KEY_SEVEN)) {
		return 7;
	}
	else if (IsKeyPressed(KEY_EIGHT)) {
		return 8;
	}
	else if (IsKeyPressed(KEY_NINE)) {
		return 9;
	}
	return 10;
}

int launchExplorer(char* music_dir, int buffer_size, struct MDIR array[], int len) {

	int margin_x = 20;
	int margin_y = 20;
	int font_size = 20;

	int height = margin_y*2 + font_size*len + margin_y*len*3;
	int width = height * (7/5);
	if (width < 500) {
		width = 500;
	}

	InitWindow(width, height, "File Explorer");
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(BLACK);
				for (int i = 0; i < len; i++) {
					DrawText(TextFormat("(%i)", i),
							margin_x, margin_y + margin_y*i*3 + font_size, font_size, RED);
					DrawText(array[i].name,
							margin_x + 50, margin_y + margin_y*i*3 + font_size, font_size, RED);
					DrawText(array[i].genre,
							margin_x + 330, margin_y + margin_y*i*3 + font_size, font_size, RED);
				}
			SetWindowOpacity(0.75);

			int sel = getIndex();
			if (sel != 10) {
				strncat(music_dir, array[sel].name, buffer_size);
				return sel;
			}
		EndDrawing();
	}
	return 10;
}

void GetSongName(char *path, int path_len, char *destination) {
	int song_name_len = 0;
	int slash_count = 0;

	for (int i = 0; i < path_len; i++) {
		if (path[i] == '/') {
			slash_count++;
			song_name_len = 0;
		}
		song_name_len++;
	}

	int counter = 0;
	for (int i = 0; i < path_len; i++) {
		if (path[i] == '/') {
			slash_count--;
			i++;
		}
		if (slash_count == 0) {
			destination[counter] = path[i];
			counter++;
		}
	}
	destination[song_name_len] = '\0';
}

void GetDirName(char *path, int path_len, char *destination) {
	int song_name_len = 0;
	int slash_count = 0;

	for (int i = 0; i < path_len; i++) {
		if (path[i] == '/') {
			slash_count++;
			song_name_len = 0;
		}
		song_name_len++;
	}

	int new_len = strlen(path) - song_name_len - 1;

	int counter = 0;
	for (int i = 0; i < path_len; i++) {
		if (path[i] == '/') {
			slash_count--;
			i++;
		}
		if (slash_count == 1) {
			if (path[i] == '/')
				break;
			destination[counter] = path[i];
			counter++;
		}
	}
	destination[counter] = '\0';
}

