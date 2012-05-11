#include <stdlib.h>
#include <stdio.h>
#include "track.h"

Track* create_track(long track_length){
    Track* track = (Track*) malloc(sizeof(Track));
    track->tracker = 0;
    track->dt = 0;
    track->running_status = 0xF;
    track->midi = (unsigned char*)malloc(track_length);
    track->track_length = track_length;
    track->active = 1;
    return track;
}


void track_rewind_one_byte(Track* tr){
    tr->tracker--;
}

unsigned long track_read_next_vlf(Track* tr){
    // 81 fd 10
    if(tr->tracker >= tr->track_length){
        printf("GENERAL FAILURE! track_read_next_vlf\n");
    }
    unsigned long vlf = 0;
    unsigned long next = (unsigned char) tr->midi[tr->tracker++];
    while(next&0x80){
        next &= 0x7F;
        vlf <<= 7;
        vlf |= next;
        next = (unsigned char) tr->midi[tr->tracker++];
    }
    return (vlf<<7) | next;
    /*
    long next = (unsigned char)tr->midi[tr->tracker++];
    long n = next;
    while(n&0x80){
        next &= 0x7F;
        next <<= 7;
        if(tr->tracker >= tr->track_length){
            printf("GENERAL FAILURE! track_read_next_vlf (inside)\n");
        }
        n = (unsigned char)tr->midi[tr->tracker++];
        next |= (n&0x7F);

    }
    */
}


unsigned long track_read_next_dword(Track* tr){
    if(tr->tracker+3 >= tr->track_length){
        printf("GENERAL FAILURE! track_read_next_dword\n");
    }
    long next = (unsigned char)tr->midi[tr->tracker++];
    next <<= 8; next |= (unsigned char)tr->midi[tr->tracker++];
    next <<= 8; next |= (unsigned char)tr->midi[tr->tracker++];
    next <<= 8; next |= (unsigned char)tr->midi[tr->tracker++];
    return next;
}

unsigned short track_read_next_word(Track* tr){
    if(tr->tracker+1 >= tr->track_length){
        printf("GENERAL FAILURE! track_read_next_word\n");
    }
    long next = (unsigned char)tr->midi[tr->tracker++];
    next <<= 8; next |= (unsigned char)tr->midi[tr->tracker++];
    return next;
}


unsigned char track_read_next_byte(Track* tr){
    if(tr->tracker >= tr->track_length){
        printf("GENERAL FAILURE! track_read_next_byte\n");
    }
    return (unsigned char)tr->midi[tr->tracker++];
}
