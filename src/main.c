#include <stdio.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include "../lib/raylib.h"
#include "../lib/futilities.h"
#include "../config/config.h"

int main(void) {
	InitAudioDevice();
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

	const char *pwd = GetWorkingDirectory();
	char img_dir[BUFFER_SIZE];
	strncpy(img_dir, pwd, BUFFER_SIZE);
	strncat(img_dir, "/img/", BUFFER_SIZE);

	// Load volume icon
	char volume_icon_path[BUFFER_SIZE];
	strncpy(volume_icon_path, img_dir, BUFFER_SIZE);
	strncat(volume_icon_path, "vol.png", BUFFER_SIZE);
	Image volume_icon = LoadImage(volume_icon_path);
	ImageResize(&volume_icon, volume_icon.width/100, volume_icon.height/100);
	Texture2D volume_texture = LoadTextureFromImage(volume_icon); // Image converted to texture, uploaded to GPU memory (VRAM)
	UnloadImage(volume_icon);

	// Load "Go to"
	char go_to_icon_path[BUFFER_SIZE];
	strncpy(go_to_icon_path, img_dir, BUFFER_SIZE);
	strncat(go_to_icon_path, "go_to_icon.png", BUFFER_SIZE);
	Image go_to_icon = LoadImage(go_to_icon_path);
	ImageResize(&go_to_icon, go_to_icon.width/6, go_to_icon.height/6);
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
	while (!WindowShouldClose()) {
		UpdateMusicStream(current_song);

		if (IsKeyPressed(KEY_O)) {
			assignName(dirs_array, PATH);
			assignGenre(dirs_array, len);
			goto fexplorer;
		}

		if (IsKeyPressed(KEY_SPACE)) {
			if (IsMusicStreamPlaying(current_song)) {
				PauseMusicStream(current_song);
			} else {
				ResumeMusicStream(current_song);
			}
		}

		if (IsKeyPressed(KEY_RIGHT)) {
			song_index++;
			if (song_index == song_list.count-1 || song_index < 0) {
				song_index = 0;
			}
			current_song = LoadMusicStream(song_list.paths[song_index]);
			current_song.looping = 0;
			PlayMusicStream(current_song);
		} else if (IsKeyPressed(KEY_LEFT)) {
			song_index--;
			if (song_index == song_list.count-1 || song_index < 0) {
				song_index = 0;
			}
			current_song = LoadMusicStream(song_list.paths[song_index]);
			current_song.looping = 0;
			PlayMusicStream(current_song);
		}

		if (IsKeyPressed(KEY_DOWN)) pitch -= 0.25f;
		else if (IsKeyPressed(KEY_UP)) pitch += 0.25f;
		SetMusicPitch(current_song, pitch);

		float song_len = GetMusicTimeLength(current_song);
		float secs = GetMusicTimePlayed(current_song);
		float time = secs / song_len;
		int mins = (int) secs / 60;

		while (GetMusicTimePlayed(current_song) > song_len - 0.9) {
			secs = 0;
			if (song_index == song_list.count-1) {
				song_index = 0;
			} else {
				song_index++;
			}
			current_song = LoadMusicStream(song_list.paths[song_index]);
			current_song.looping = 0;
			PlayMusicStream(current_song);
			break;
		}




		Vector2 mouse_pos = GetMousePosition();
		int current_volume_bar_width = (int) (volume * width_volume_bar);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			//Progress bar
			if (mouse_pos.y > y_progress_bar && mouse_pos.y < (height_progress_bar + y_progress_bar)
					&& mouse_pos.x > x_progress_bar && mouse_pos.x < (width_progress_bar + x_progress_bar)) {
				float new_time = ((mouse_pos.x - x_progress_bar) / width_progress_bar) * song_len;
				SeekMusicStream(current_song, new_time);
			}
		}

		/* DRAWING */
		BeginDrawing();

			SetWindowOpacity(0.75);
			ClearBackground(BLACK);

			DrawText(TextFormat("SPEED: x%.2f (Arrows)", pitch), x_margin, 80, 20, RED);

			// Volume
			if (mouse_pos.y > y_volume_bar - 5 && mouse_pos.y < (height_volume_bar + y_volume_bar + 5)
						&& mouse_pos.x > x_volume_bar && mouse_pos.x < (width_volume_bar + x_volume_bar)) {
				DrawRectangle(x_volume_bar, y_volume_bar, width_volume_bar, height_volume_bar, DARKGRAY);
				DrawRectangle(x_volume_bar, y_volume_bar, current_volume_bar_width, height_volume_bar, RED);
				DrawCircle(x_volume_bar + current_volume_bar_width, (int) (y_volume_bar + height_volume_bar/2), 5, RED);
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					volume = ((mouse_pos.x  - x_volume_bar) / width_volume_bar);
					SetMusicVolume(current_song, volume);
				}
			}
			else {
				DrawRectangle(x_volume_bar, y_volume_bar, width_volume_bar, height_volume_bar, DARKGRAY);
				DrawRectangle(x_volume_bar, y_volume_bar, current_volume_bar_width, height_volume_bar, RED);
			}

			DrawTexture(volume_texture, x_volume_bar - volume_texture.width - 10, y_volume_bar - (float) volume_texture.height/2, LIGHTGRAY);

			// Go to dir button
			int x_go_to_dir = x_volume_bar - 50;
			int y_go_to_dir = HEIGHT - 80;

			int width_go_to_dir = width_volume_bar + 50;
			int height_go_to_dir = 60;

			DrawRectangle(x_go_to_dir, y_go_to_dir, width_go_to_dir, height_go_to_dir, RED);
			DrawText("Go to main", x_go_to_dir + 10, y_go_to_dir + (HEIGHT - y_go_to_dir - height_go_to_dir)/2, FONT_SIZE, WHITE);
			DrawText("dir", x_go_to_dir + 10, y_go_to_dir + (HEIGHT - y_go_to_dir - height_go_to_dir)/2 + 20, FONT_SIZE, WHITE);
			DrawTexture(go_to_texture, x_go_to_dir + width_go_to_dir - go_to_texture.width - 10,
					y_go_to_dir + height_go_to_dir - go_to_texture.height - 10, WHITE);

			if (mouse_pos.y > y_go_to_dir && mouse_pos.y < (y_go_to_dir + height_go_to_dir)
						&& mouse_pos.x > x_go_to_dir && mouse_pos.x < (x_go_to_dir + width_go_to_dir)) {
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					system(TextFormat("open %s", PATH));
				}
			} 
			if (IsKeyPressed(KEY_G)) {
				system(TextFormat("open %s", music_dir));
			}


			// Porgress bar
			if (mouse_pos.y > y_progress_bar && mouse_pos.y < (height_progress_bar + y_progress_bar)
					&& mouse_pos.x > x_progress_bar && mouse_pos.x < (width_progress_bar + x_progress_bar)) {
				DrawRectangle(x_progress_bar, y_progress_bar,
							width_progress_bar, height_progress_bar, DARKGRAY);
				DrawRectangle(x_progress_bar, y_progress_bar, mouse_pos.x - x_progress_bar, height_progress_bar, DARKRED);
				DrawRectangle(x_progress_bar, y_progress_bar,
						(int) (time * width_progress_bar), height_progress_bar, RED);
			}
			else {
				DrawRectangle(x_progress_bar, y_progress_bar,
							width_progress_bar, height_progress_bar, DARKGRAY);
				DrawRectangle(x_progress_bar, y_progress_bar,
							(int) (time * width_progress_bar), height_progress_bar, RED);
			}

			// Time text
			const char *time_text = TextFormat("%02i:%02i", mins, (int) secs - mins * 60);
			int time_text_len = MeasureText(time_text, 20);
			DrawText(time_text,
				    (int) (time * width_progress_bar + x_progress_bar - (float) time_text_len/2),
					y_progress_bar + height_progress_bar + 10, 
					20, LIGHTGRAY);
			
			//Dir name
			char dir_name[100];
			GetDirName(song_list.paths[song_index], strlen(song_list.paths[song_index]), dir_name);
			int dir_name_len = MeasureText(dir_name, FONT_SIZE);
			DrawText(dir_name, WIDTH/2 - dir_name_len/2, HEIGHT/3 - 30, FONT_SIZE, LIGHTGRAY);

			//Song name
			char song_name[100];
			GetSongName(song_list.paths[song_index], strlen(song_list.paths[song_index]), song_name);
			int song_name_font_size = FONT_SIZE * 2;
			int song_name_len = MeasureText(song_name, song_name_font_size);
			while (song_name_len > WIDTH - 20) {
				song_name_font_size -= 1;
				song_name_len = MeasureText(song_name, song_name_font_size);
			}
			DrawText(song_name, WIDTH/2 - song_name_len/2, HEIGHT/3, song_name_font_size, LIGHTGRAY);


			char menu_text[100] = "Press O to open directory menu";
			int menu_text_len = MeasureText(menu_text, FONT_SIZE);
			DrawText(menu_text, WIDTH/2 - menu_text_len/2, HEIGHT/3 + 60, FONT_SIZE, RED);

			
		EndDrawing();



	}












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
