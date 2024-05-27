#include <stdio.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include "../lib/raylib.h"
#include "../lib/futilities.h"
#include "../config/config.h"

int main(void) {
	// Progress bar dimensions
	const float width_progress_bar = 900.00f;
	const int height_progress_bar = 15;
	float x_progress_bar = (float) WIDTH/2 - width_progress_bar/2;
	const int y_progress_bar = 450;

	// Volume bar dimensions
	const float width_volume_bar = 100.00f;
	const int height_volume_bar = 3;
	float x_volume_bar = WIDTH - (WIDTH - width_progress_bar)/2 - width_volume_bar;
	int y_volume_bar = 80;

	// Standard margin for elements
	const int x_margin = 60;

	// Starting volume
	float volume = 0.5f;


	// Get music from dir
	int change_dir_flag = 0; // If we change directory, we will want to update this boolean variable to 1
	char music_dir[BUFFER_SIZE];
	strncpy(music_dir, PATH, BUFFER_SIZE);
	strncat(music_dir, STARTING_DIR, BUFFER_SIZE);
	puts(music_dir);
	FilePathList song_list =  LoadDirectoryFilesEx(music_dir, ".mp3", true); // Load music

	// Initial pitch
	float pitch = 1.0f;

	int song_index = 0; // The program starts by the first song of the dir
	Music current_song = LoadMusicStream(song_list.paths[song_index]);
	current_song.looping = 0;
	PlayMusicStream(current_song); // Play song
	SetMusicVolume(current_song, volume);

	// Open the directory where the music is playing for future use
	DIR *directory = opendir(PATH);
	int len = getNumberOfDirs(directory); 
	struct MDIR dirs_array[len];

	// The program will return here each time the user changes the directory from which our songs are playing
	init:
	// Initialize the window
	InitWindow(WIDTH, HEIGHT, "ADHD Music Player");
	SetTargetFPS(60);
	SetWindowFocused();

	// Load volume icon
	Image volume_icon = LoadImage("/Users/guillermomarcoslara/Developer/MusicPlayer/img/vol.png");
	ImageResize(&volume_icon, volume_icon.width/100, volume_icon.height/100);
	Texture2D volume_texture = LoadTextureFromImage(volume_icon); // Image converted to texture, uploaded to GPU memory (VRAM)
	UnloadImage(volume_icon);

	// Load "Go to"
	Image go_to_icon = LoadImage("/Users/guillermomarcoslara/Developer/MusicPlayer/img/go_to_icon.png");
	ImageResize(&go_to_icon, go_to_icon.width/5, go_to_icon.height/5);
	Texture2D go_to_texture = LoadTextureFromImage(go_to_icon); // Image converted to texture, uploaded to GPU memory (VRAM)
	UnloadImage(go_to_icon);

	// If the user changes dir, the program needs to load the new directory songs and update the playing one
	char new_dir[BUFFER_SIZE];
	if (change_dir_flag == 1) {
		song_list =  LoadDirectoryFilesEx(new_dir, ".mp3", true);
		current_song = LoadMusicStream(song_list.paths[song_index]);
		current_song.looping = 0;
		PlayMusicStream(current_song);
		SetMusicVolume(current_song, volume);
		 change_dir_flag = 0;
	}
	//Event loop











		if (0) {
		fexplorer:
			change_dir_flag = 1;
			strcpy(new_dir, PATH);
			int quit = launchExplorer(new_dir, BUFFER_SIZE, dirs_array, len);
			if (quit == 10)
				change_dir_flag = 0;
			else
				song_index = 0;
			CloseWindow();
			goto init;
	}

	return 0;
}
