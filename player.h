#ifndef PLAYER_H
#define PLAYER_H

#include "mixer.h"
#include "track.h"

typedef struct{
    unsigned char* midi;
    short number_of_tracks;
    Mixer* m;
    long tracker;
    Track** tracks;
    long ticks_per_second;
    long ticks_per_beat;
} Player;

Player* create_player();
void player_forward(Player* p);
void player_load(Player* p, const char* midi_filename);
void _player_load_instruments(Player* p, const char* midi_filename);
char player_read_next_byte(Player* p);
short player_read_next_word(Player* p);
long player_read_next_dword(Player* p);
long player_read_next_vlf(Player* p);
void player_set_microseconds_per_quaver(Player* p,long microseconds_per_quaver);
#endif
