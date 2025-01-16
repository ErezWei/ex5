/************************
 * Name:
 * ID:
 * Assignment: ex5
************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Song {
	char* title;
	char* artist;
	int year;
	char* lyrics;
	int streams;
} Song;

typedef struct Playlist {
	char* name;
	Song** songs;
	int songsNum;
} Playlist;

typedef struct AllPlaylists {
	Playlist** playlists;
	int playlistsNum;
} AllPlaylists;

enum {
	FALSE,
	TRUE
};

void playSong(Song* song);
void playSongsInPlaylist(Playlist* playlist);
void addPlaylist(AllPlaylists* allLists);
void deletePlaylist(AllPlaylists* allLists);
void freePlaylist(AllPlaylists* allLists, int index);
void addSong(Playlist* playlist);
void deleteSong(Playlist* playlist);
void freeSong(Playlist* playlist, int index);
char* createString();
void printPlaylistsMenu(AllPlaylists* allLists);
void printPlaylistSelection(AllPlaylists* allLists);
void printPlaylistActions(Playlist* playlist);
void showPlaylist(Playlist* playlist);
void printSongList(Playlist* playlist);
void sortPlaylist(Playlist* playlist);
void swapSongs(Song** songA, Song** songB);
void swapPlaylists(Playlist** playlistA, Playlist** playlistB);
void sortByYear(Playlist* playlist);
void sortByStreamsAscending(Playlist* playlist);
void sortByStreamsDescending(Playlist* playlist);
void sortAlphabetically(Playlist* playlist);



void playSong(Song* song) {
	printf("Now playing %s:\n", song->title);
	printf("$ %s $\n", song->lyrics);
	song->streams++;
}


void playSongsInPlaylist(Playlist* playlist) {
	for (int i=0; i < playlist->songsNum; i++) {
		playSong(playlist->songs[i]);
		printf("\n");
	}
}


void addPlaylist(AllPlaylists* allLists) {
	// Check if this is the first playlist and do malloc/realloc accordingly to add space in the list of playlists
	if (allLists->playlistsNum == 0) {
		allLists->playlists = malloc(sizeof(Playlist*));
	}
	else {
		allLists->playlists = realloc(allLists->playlists, (allLists->playlistsNum + 1) * sizeof(Playlist*));
	}
	// Check if allocation failed
	if (allLists->playlists == NULL) {
		printf("malloc failed\n");
		exit(1);
	}
	// Create the playlist itself and check allocation
	allLists->playlists[allLists->playlistsNum] = malloc(sizeof(Playlist));
	if (allLists->playlists[allLists->playlistsNum] == NULL) {
		printf("malloc failed\n");
		exit(1);
	}
	// Initialize songsNum of new playlist to 0
	allLists->playlists[allLists->playlistsNum]->songsNum = 0;
	// Set playlist name
	printf("Enter playlist's name:\n");
	allLists->playlists[allLists->playlistsNum]->name = createString();
	// Increment playlists counter
	allLists->playlistsNum++;
}


// This function handles the playlist deletion menu
void deletePlaylist(AllPlaylists* allLists) {
	// Print menu and get user selection
	int selection = 0;
	printf("Choose a playlist:\n");
	for (int i=0; i < allLists->playlistsNum; i++) {
		// Use [i+1] for printing to start menu from 1
		printf("\t%d. %s\n", i + 1, allLists->playlists[i]->name);
	}
	printf("\t%d. Back to main menu\n", allLists->playlistsNum + 1);
	scanf("%d", &selection);
	// If user selected a certain playlist, delete it
	if (selection > 0 && selection <= allLists->playlistsNum) {
		// Pass [selection-1] as index since index starts counting from 0
		freePlaylist(allLists, selection - 1);
		printf("Playlist deleted.\n");
	}
}


// This function completes the steps to remove playlist at requested index from the list of playlists
void freePlaylist(AllPlaylists* allLists, int index) {
	/* First - if the playlist we need to delete is not the last one, then move it to the end
	 * (to check we use (playlistsNum-1) because index starts at 0 so need to check if difference is more than 1). */
	if (index < allLists->playlistsNum - 1) {
		// Make swaps until the playlist to delete reaches the end (each time updating the index)
		for ( ; index < allLists->playlistsNum - 1; index++) {
			swapPlaylists(&allLists->playlists[index], &allLists->playlists[index + 1]);
		}
	}
	/* Now call freeSong to free all the songs (and their elements) in the playlist (when freeing the last
	 * song this will also free the array of pointers to the songs). Start the calls from the end of the array
	 * so the function won't need to make swaps.
	 * Need to use [songsNum-1] for initial i value since index starts counting from 0. */
	for (int i = allLists->playlists[index]->songsNum - 1; i >= 0; i--) {
		freeSong(allLists->playlists[index], i);
	}
	// Next - free the playlist's name
	free(allLists->playlists[index]->name);
	// Next - free the playlist itself
	free(allLists->playlists[index]);
	// If this was the only playlist, free the playlists pointers array, else call realloc to reduce the size
	if (allLists->playlistsNum == 1) {
		free(allLists->playlists);
	}
	else {
		allLists->playlists = realloc(allLists->playlists, (allLists->playlistsNum - 1) * sizeof(Playlist*));
		// Check if realloc failed
		if (allLists->playlists == NULL) {
			printf("realloc failed\n");
			exit(1);
		}
	}
	// Process complete - update playListsNum
	allLists->playlistsNum--;
}


void addSong(Playlist* playlist) {
	// Check if this is the first song and do malloc/realloc accordingly to add space in the list of songs
	if (playlist->songsNum == 0) {
		playlist->songs = malloc(sizeof(Song*));
	}
	else {
		playlist->songs = realloc(playlist->songs, (playlist->songsNum + 1) * sizeof(Song*));
	}
	// Check if allocation failed
	if (playlist->songs == NULL) {
		printf("malloc failed\n");
		exit(1);
	}
	// Create the song itself and check allocation
	playlist->songs[playlist->songsNum] = malloc(sizeof(Song));
	if (playlist->songs[playlist->songsNum] == NULL) {
		printf("malloc failed\n");
		exit(1);
	}
	// Initialize streams number of new song to 0
	playlist->songs[playlist->songsNum]->streams = 0;
	// Populate the rest of the fields with input from the user
	printf("Enter song's details\nTitle:\n");
	playlist->songs[playlist->songsNum]->title = createString();
	printf("Artist:\n");
	playlist->songs[playlist->songsNum]->artist = createString();
	printf("Year of release:\n");
	scanf("%d", &playlist->songs[playlist->songsNum]->year);
	printf("Lyrics:\n");
	playlist->songs[playlist->songsNum]->lyrics = createString();
	// Increment songs counter
	playlist->songsNum++;
}


// This function handles the song deletion menu
void deleteSong(Playlist* playlist) {
	int selection = 0;
	// Print song list and get user's selection
	printSongList(playlist);
	printf("choose a song to delete, or 0 to quit:\n");
	scanf("%d", &selection);
	if (selection < 0) {
		printf("Invalid option\n");
	}
	else if (selection != 0 && selection <= playlist->songsNum) {
		// User chose song - remove it (need to pass [selection-1] as index since index starts at 0)
		freeSong(playlist, selection - 1);
		printf("Song deleted successfully.\n");
	}
}


// This function completes the steps to remove song at requested index from playlist
void freeSong(Playlist* playlist, int index) {
	/* First - if the song we need to delete is not the last one, then move it to the end
	 * (to check we use (songsNum-1) because index starts at 0 so need to check if difference is more than 1). */
	if (index < playlist->songsNum - 1) {
		// Make swaps until the song to delete reaches the end (each time updating the index)
		for ( ; index < playlist->songsNum - 1; index++) {
			swapSongs(&playlist->songs[index], &playlist->songs[index + 1]);
		}
	}
	// Start by freeing the elements inside the song
	free(playlist->songs[index]->title);
	free(playlist->songs[index]->artist);
	free(playlist->songs[index]->lyrics);
	// Then free the song itself
	free(playlist->songs[index]);
	// If this was the only song, free the songs pointers array, else call realloc to reduce the size
	if (playlist->songsNum == 1) {
		free(playlist->songs);
	}
	else {
		playlist->songs = realloc(playlist->songs, (playlist->songsNum - 1) * sizeof(Song*));
		// Check if realloc failed
		if (playlist->songs == NULL) {
			printf("realloc failed\n");
			exit(1);
		}
	}
	// process complete - update songsNum
	playlist->songsNum--;
}


// This function frees all allocated memory when user chooses to exit the program
void freeAll(AllPlaylists* allLists) {
	/* Call freePlaylist to free all playlists (and their elements) one by one. When freeing the last one it will
	 * also free the array that holds the pointers to the playlists. Start the calls from the end of array so that
	 * it won't need to make swaps.
	 * Need to use [playlistsNum-1] for initial i value since index starts counting from 0. */
	for (int i = allLists->playlistsNum - 1; i >= 0; i--) {
		freePlaylist(allLists, i);
	}
}


// General function to allocate memory and receive unlimited strings, returns pointer to string.
char* createString() {
	// Create allocation for string and check if worked
	char* string = malloc(sizeof(char));
	if (string == NULL) {
		printf("malloc failed\n");
		exit(1);
	}
	char temp = 0;
	int i = 0;
	while (temp != '\n') {
		// If this is 1st character, use " %c" (with space)
		if (i == 0) {
			scanf(" %c", &temp);
		}
		else {
			scanf("%c", &temp);
		}
		// If this is '\r' - skip iteration
		if (temp == '\r') {
			continue;
		}
		if (temp != '\n') {
			// Add allocation (increase by 2 in order to leave room for '\0' at the end)
			string = realloc(string, (i+2) * sizeof(char));
			if (string == NULL) {
				printf("realloc failed\n");
				exit(1);
			}
			string[i] = temp;
			i++;
		}
	}
	// If we got no characters for some buffer issue, try again
	if (i == 0) {
		free(string);
		return createString();
	}
	// Add '\0' at the end
	string[i] = '\0';
	return string;
}


// This function handles the main menu
void printPlaylistsMenu(AllPlaylists* allLists) {
	int selection = 0;
	do {
		printf("Please Choose:\n");
		printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
		scanf("%d", &selection);
		switch (selection) {
		case 1:
			printPlaylistSelection(allLists);
			break;
		case 2:
			addPlaylist(allLists);
			break;
		case 3:
			deletePlaylist(allLists);
			break;
		case 4:
			freeAll(allLists);
			break;
		default:
			printf("Invalid option\n");
			break;
		}
	}
	while (selection != 4);
}


// This functions handles the playlist selection menu
void printPlaylistSelection(AllPlaylists* allLists) {
	int selection = 0;
	int exitOption = allLists->playlistsNum + 1;
	do {
		printf("Choose a playlist:\n");
		for (int i = 1; i < exitOption; i++) {
			// Need to use [i-1] for printing because index starts from 0, but i starts counting from 1.
			printf("\t%d. %s\n", i, allLists->playlists[i - 1]->name);
		}
		printf("\t%d. Back to main menu\n", exitOption);
		scanf("%d", &selection);
		if (selection <= 0 || selection > exitOption) {
			printf("Invalid option\n");
		}
		else if (selection != exitOption) {
			// Again - need to use [selection-1] because index starts at 0, and menu starts at 1.
			printPlaylistActions(allLists->playlists[selection - 1]);
		}
	}
	while (selection != exitOption);
}


// This function handles the playlist actions menu
void printPlaylistActions(Playlist* playlist) {
	int selection = 0;
	printf("playlist %s:\n", playlist->name);
	do {
		printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
		scanf("%d", &selection);
		switch (selection) {
		case 1:
			showPlaylist(playlist);
			break;
		case 2:
			addSong(playlist);
			break;
		case 3:
			deleteSong(playlist);
			break;
		case 4:
			sortPlaylist(playlist);
			break;
		case 5:
			playSongsInPlaylist(playlist);
			break;
		case 6:
			break;
		default:
			printf("Invalid option\n");
			break;
		}
	}
	while (selection != 6);
}


// Prints list of songs in playlist and lets user choose a song to play
void showPlaylist(Playlist* playlist) {
	printSongList(playlist);
	int selection = 0;
	do {
		printf("choose a song to play, or 0 to quit:\n");
		scanf("%d", &selection);
		// If playlist is empty, any input should lead back to previous menu
		if (playlist->songsNum == 0) {
			break;
		} else if (selection > playlist->songsNum || selection < 0) {
			printf("Invalid option\n");
			break;
		} else if (selection != 0) {
			// Use [selection-1] since index starts at 0 and menu starts at 1
			playSong(playlist->songs[selection - 1]);
		}
	} while (selection != 0);
}


// This function prints the list of songs for a given playlist
void printSongList(Playlist* playlist) {
	for (int i = 0; i < playlist->songsNum; i++) {
		// Use [i+1] for list items in order to start from 1
		printf("%d. Title: %s\n", i + 1, playlist->songs[i]->title);
		printf("   Artist: %s\n", playlist->songs[i]->artist);
		printf("   Released: %d\n", playlist->songs[i]->year);
		printf("   Streams: %d\n\n", playlist->songs[i]->streams);
	}
}


// This function handles the sorting menu
void sortPlaylist(Playlist* playlist) {
	int selection = 0;
	printf("choose:\n"
		"1. sort by year\n"
		"2. sort by streams - ascending order\n"
		"3. sort by streams - descending order\n"
		"4. sort alphabetically\n");
	scanf("%d", &selection);
	if (selection == 1) {
		sortByYear(playlist);
	} else if (selection == 2) {
		sortByStreamsAscending(playlist);
	} else if (selection == 3) {
		sortByStreamsDescending(playlist);
	} else {
		// if selection was 4 or any other value - sort alphabetically
		sortAlphabetically(playlist);
	}
	printf("sorted\n");
}


// Function for swapping song positions (by changing pointers)
void swapSongs(Song** songA, Song** songB) {
	Song* temp = *songA;
	*songA = *songB;
	*songB = temp;
}


// Function for swapping playlist positions (by changing pointers)
void swapPlaylists(Playlist** playlistA, Playlist** playlistB) {
	Playlist* temp = *playlistA;
	*playlistA = *playlistB;
	*playlistB = temp;
}


void sortByYear(Playlist* playlist) {
	// Use bubble sort - outer loop runs until (size-1) since last item is naturally sorted
	for (int i = 0; i < playlist->songsNum - 1; i++) {
		// For each pass check if swap was made
		int swapped = FALSE;
		// Inner loop limit: -i since each time one more item already sorted, -1 since 2 items are checked at once
		for (int j = 0; j < playlist->songsNum - i - 1; j++) {
			if (playlist->songs[j]->year > playlist->songs[j + 1]->year) {
				swapSongs(&playlist->songs[j], &playlist->songs[j + 1]);
				swapped = TRUE;
			}
		}
		// If no swap was made, sorting is complete and we can break
		if (!swapped) {
			break;
		}
	}
}


void sortByStreamsAscending(Playlist* playlist) {
	// Use bubble sort - outer loop runs until (size-1) since last item is naturally sorted
	for (int i = 0; i < playlist->songsNum - 1; i++) {
		// For each pass check if swap was made
		int swapped = FALSE;
		// Inner loop limit: -i since each time one more item already sorted, -1 since 2 items are checked at once
		for (int j = 0; j < playlist->songsNum - i - 1; j++) {
			if (playlist->songs[j]->streams > playlist->songs[j + 1]->streams) {
				swapSongs(&playlist->songs[j], &playlist->songs[j + 1]);
				swapped = TRUE;
			}
		}
		// If no swap was made, sorting is complete and we can break
		if (!swapped) {
			break;
		}
	}
}


void sortByStreamsDescending(Playlist* playlist) {
	// Use bubble sort - outer loop runs until (size-1) since last item is naturally sorted
	for (int i = 0; i < playlist->songsNum - 1; i++) {
		// For each pass check if swap was made
		int swapped = FALSE;
		// Inner loop limit: -i since each time one more item already sorted, -1 since 2 items are checked at once
		for (int j = 0; j < playlist->songsNum - i - 1; j++) {
			if (playlist->songs[j]->streams < playlist->songs[j + 1]->streams) {
				swapSongs(&playlist->songs[j], &playlist->songs[j + 1]);
				swapped = TRUE;
			}
		}
		// If no swap was made, sorting is complete and we can break
		if (!swapped) {
			break;
		}
	}
}


void sortAlphabetically(Playlist* playlist) {
	// Use bubble sort - outer loop runs until (size-1) since last item is naturally sorted
	for (int i = 0; i < playlist->songsNum - 1; i++) {
		// For each pass check if swap was made
		int swapped = FALSE;
		// Inner loop limit: -i since each time one more item already sorted, -1 since 2 items are checked at once
		for (int j = 0; j < playlist->songsNum - i - 1; j++) {
			if ( strcmp(playlist->songs[j]->title, playlist->songs[j + 1]->title) > 0 ) {
				swapSongs(&playlist->songs[j], &playlist->songs[j + 1]);
				swapped = TRUE;
			}
		}
		// If no swap was made, sorting is complete and we can break
		if (!swapped) {
			break;
		}
	}
}


int main() {
	// Create the list of playlists, initialize number of playlists to 0, then print menu
	AllPlaylists allLists;
	allLists.playlistsNum = 0;
	printPlaylistsMenu(&allLists);
	// User chose to exit - print goodbye
	printf("Goodbye!\n");
}
