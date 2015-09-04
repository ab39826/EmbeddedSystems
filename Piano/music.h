typedef struct Song{
	int melodyLength;
	int harmonyLength;
	
	int* melodyFrequencies;
	int* harmonyFrequencies;	
	
	int * melodyDurations;
	int * harmonyDurations;
} Song;

typedef short (*Envelope) (short amplitude, int time, int length, int next);


void musicInit(void);
void playSong(Song* song, Envelope envelope, int tempo);
void pauseResumeSong(void);
void pauseSong(void);
void rewindSong(void);
void resumeSong(void);
