#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL/SDL.h"

#include "mixer.h"
#include "player.h"
#include "oscillators.h"

SDL_Event event;
Mixer* m;
Player* p;
long t = 0;

int ticker = 0;

const int CHANNELS = 2;
const int FREQUENCY = 44100;

// Buffer fill-upper
void fill_audio(void *data,Uint8 *stream,int len)
{
    short *buff;
	int i;
	// Cast
	buff = (short*)stream;
	len /= 2; //Because we're now using shorts
	//Noise
    short temp;
    float pitch = 56;


	for(i = 0;i < len;i+=2){
		//buff[i] = (short)(30000 * osc_squ(pitch + sin((double)(t/10000.0)),t) );

		while(ticker > (int)(CHANNELS*FREQUENCY/p->ticks_per_second)){
            player_forward(p);
            ticker -=  (int)(CHANNELS*FREQUENCY/p->ticks_per_second);
		}ticker+=2;
		buff[i] = mixer_render(m,t);
		buff[i+1] = buff[i];

		t+=2;
    }

}

//Open the audio device to what we want
void open_audio()
{
	SDL_AudioSpec as;
	SDL_Init(SDL_INIT_AUDIO);
	as.freq = 44100;
	as.format = AUDIO_S16SYS;
	as.channels = 2;
	as.samples = 1024;
	as.callback = fill_audio;
	SDL_OpenAudio(&as,NULL);
	SDL_PauseAudio(0);
}

void close_audio()
{
	SDL_CloseAudio();
	SDL_Quit();
}

int main(int argc, char**argv)
{
    int running = 1;

    m = create_mixer();
    p = create_player();
    p->m = m;
    if(argc == 2){
        player_load(p, argv[1]);
    }else{
        player_load(p, "res/1000ad-B.mid");
    }
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SQU));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));
    mixer_add_instrument(m,create_instrument(OSC_SIN));

    SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );
	// Open the audio
	open_audio();
	while(running){
	    while(SDL_PollEvent( &event )){
	        switch( event.type ){
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ){
                    case SDLK_ESCAPE:
                        running = 0;
                    default:;
                }
                break;
            case SDL_QUIT:
                running = 0;
                break;
	        }
	    }
	    SDL_Delay(1);
	}
	// Close audio
	close_audio();
	return 0;
}
