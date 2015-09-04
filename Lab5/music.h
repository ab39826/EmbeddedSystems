#define noteChanges 10

typedef struct Song{
	int melodyLength;
	int harmonyLength;
	
	int* melodyFrequencies;
	int* harmonyFrequencies;	
	
	int * melodyDurations;
	int * harmonyDurations;
} Song;

typedef struct NoteInfo{	
	int sineIndex;
	int sinceLastTick;
	int sinceLastNote;
	int isPlaying;
	int recordTrack[noteChanges];
	int * tickLengths;
	
}NoteInfo;



typedef short (*Envelope) (short amplitude, int time, int length, int next);

void musicInit(void);
void playSong(Song* song, Envelope envelope, int tempo);
void pauseResumeSong(void);
void pauseSong(void);
void rewindSong(void);
void resumeSong(void);
