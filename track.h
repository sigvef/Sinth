
#ifndef TRACK_H
#define TRACK_H

typedef struct{
    long long dt;
    unsigned long tracker;
    unsigned long running_status;
    unsigned char *midi;
    unsigned long track_length;
    short active;
} Track;

Track* create_track();

unsigned char track_read_next_byte(Track* tr);
unsigned short track_read_next_word(Track* tr);
unsigned long track_read_next_dword(Track* tr);
unsigned long track_read_next_vlf(Track* tr);
void track_rewind_one_byte(Track* tr);
#endif

