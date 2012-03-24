#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "track.h"

Player* create_player(){
    Player* player = (Player*) malloc(sizeof(Player));
    player->tracker = 0;
    player->ticks_per_second = 0;
    player->ticks_per_beat = 0;
    return player;
}

void _player_load_instruments(Player* p, const char* midi_filename){
    FILE *f;
    char instruments_filename[strlen(midi_filename)+6];
    strcpy(instruments_filename, midi_filename);
    strcat(instruments_filename, ".instr");

    f = fopen(instruments_filename,"rt");
    if(!f) return _player_load_instruments(p,"res/default");
    char line[80];
    while(fgets(line,80,f)){
      printf("%s\n",line);  
    }
    fclose(f);
    f=NULL;

}

void player_load(Player* p, const char* midi_filename){
    _player_load_instruments(p,midi_filename);
    FILE *f;
    f = fopen(midi_filename,"rb");
    fseek(f, 0L, SEEK_END);
    size_t s = ftell(f);
    rewind(f);
    p->midi = malloc(sizeof(char)*s);
     if (p->midi == NULL) {fputs ("Memory error",stderr); exit (2);}
    size_t result = fread(p->midi,s,1,f);
    if (result != 1) {fputs ("Reading error",stderr); exit (3);}
    fclose(f);
    //f = fopen("test.mid","w");
    //fwrite(p->midi,s,1,f);
    //fclose(f);
    p->tracker = 10; //skip to number of tracks in the header
    p->number_of_tracks = player_read_next_word(p);
    printf("Number of tracks: %i\n",p->number_of_tracks);
    int time_division = player_read_next_word(p);
    if(time_division & 0x8000){
        //time division is in frames per second
        p->ticks_per_second = (time_division&0x7F00) * (time_division&0x00FF);
    }else{
        //time division is in ticks per beat
        p->ticks_per_beat = time_division&0x7FFF;
        p->ticks_per_second = 2 * p->ticks_per_beat; //assume 120bpm to begin with
    }
    p->tracker = 14;//skip the rest of the header for now
    p->tracks = (Track**) malloc(sizeof(Track*)*p->number_of_tracks);
    int i;
    for(i=0;i<p->number_of_tracks;i++){
        long track_start = p->tracker;
        printf("new track! track_start:                %i\n",track_start);
        player_read_next_dword(p);
        long track_length = player_read_next_dword(p);
        printf("           length:                     %i\n",track_length);
        printf("           next track should start at: %i\n\n",(track_length+track_start+8));
        p->tracks[i] = create_track(track_length);
        memcpy(
               p->tracks[i]->midi,
                p->midi+track_start+8, //8 is the length of the track header
                track_length);
        p->tracker += track_length;
    }
    for(i=0;i<p->number_of_tracks;i++){
        p->tracks[i]->dt = track_read_next_vlf(p->tracks[i]);
    }
}


void player_forward(Player* p){
    int i;
    for(i=0;i<p->number_of_tracks;i++){
        if(!p->tracks[i]->active) continue;
        Track* tr = p->tracks[i];
        tr->dt--;
        while(tr->dt <= 0){
            int status = track_read_next_byte(tr);
            track_forward_running_status_hack: //label to jump to if we need to reswitch after forcing running status :3
            ;int event_type = (status&0xF0)>>4;
            switch(event_type){
                case 0x0:
                case 0x1:
                case 0x2:
                case 0x3:
                case 0x4:
                case 0x5:
                case 0x6:
                case 0x7:
                    //running status, same type as last event
                    track_rewind_one_byte(tr);
                    status = tr->running_status;
                    goto track_forward_running_status_hack;
                case 0x8: //note off
                    ; // gcc bug fix (Bug c/37231)
                    unsigned char midi_channel = status&0xF;
                    unsigned char note_number = track_read_next_byte(tr);
                    unsigned char velocity = track_read_next_byte(tr);
                    instrument_note_off(p->m->instruments[midi_channel],note_number);
                    break;
                case 0x9: //note on
                    midi_channel = status&0xF;
                    note_number = track_read_next_byte(tr);
                    velocity = track_read_next_byte(tr);
                    /*
                        printf("printed! i: %i\n",i);
                        printf("   channel: %i\n", midi_channel);
                        printf("      note: %i\n\n", note_number);
                    */
                    //printf("trying to play note %i on channel %i with vel %i\n",note_number,midi_channel,velocity);

                    if(velocity == 0){
                        //event considered by convention to be note off because of the running status hack
                        instrument_note_off(p->m->instruments[midi_channel],note_number);
                    }else{
                        instrument_note_on(p->m->instruments[midi_channel],note_number,velocity);
                        }
                    break;
                case 0xE: //pitch bend
                        midi_channel = status&0xF;
                    int value = track_read_next_byte(tr)&0x7F;
                    value |= (track_read_next_byte(tr)&0x7F) << 7;
                    p->m->instruments[midi_channel]->pitch_bend = (float)(value-8192)/8192*2;
                    break;
                case 0xA: //note aftertouch
                case 0xB: //controller
                    //not supported yet, so just read on
                    track_read_next_word(tr);
                    break;
                case 0xC: //program change
                case 0xD: //channel aftertouch
                    //not supported yet, so just read on
                    track_read_next_byte(tr);
                    break;
                case 0xF: //meta or system exclusive event
                    //not supported yet, so just read on
                    ;int meta_type = track_read_next_byte(tr);
                    long length = track_read_next_vlf(tr); //keep length of data
                    switch(meta_type){

                        case 0x51: //set tempo
                            ;long microseconds_per_quaver = track_read_next_byte(tr);
                            microseconds_per_quaver = (microseconds_per_quaver<<8) | track_read_next_byte(tr);
                            microseconds_per_quaver = (microseconds_per_quaver<<8) | track_read_next_byte(tr);
                            player_set_microseconds_per_quaver(p,microseconds_per_quaver);
                            break;

                        default: //throw away data for so-far unsupported events
                            while(length-->0){
                                track_read_next_byte(tr);
                            }
                    }
                    break;
            }
            tr->running_status = status;
            if(tr->tracker >= tr->track_length){
                tr->active = 0;
                break;
            }else{
                tr->dt = track_read_next_vlf(tr);
                //printf("[Track %i] Next dt: %i\n", i,tr->dt);
            }
        }
    }

}

long player_read_next_vlf(Player* p){
    unsigned long vlf = 0;
    unsigned long next = (unsigned char) p->midi[p->tracker++];
    while(next&0x80){
        next &= 0x7F;
        vlf <<= 7;
        vlf |= next;
        next = (unsigned char) p->midi[p->tracker++];
    }
    return (vlf<<7) | next;
}

long player_read_next_dword(Player* p){
    long next = (unsigned char)p->midi[p->tracker++];
    next <<= 8; next |= (unsigned char)p->midi[p->tracker++];
    next <<= 8; next |= (unsigned char)p->midi[p->tracker++];
    next <<= 8; next |= (unsigned char)p->midi[p->tracker++];
    return next;
}

short player_read_next_word(Player* p){
    long next = (unsigned char)p->midi[p->tracker++];
    next <<= 8; next |= (unsigned char)p->midi[p->tracker++];
    return next;
}


char player_read_next_byte(Player* p){
    return (unsigned char)p->midi[p->tracker++];
}


void player_set_microseconds_per_quaver(Player* p,long microseconds_per_quaver){
    p->ticks_per_second = p->ticks_per_beat /( (float)(microseconds_per_quaver)/1000000);
}
