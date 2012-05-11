#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sample.h"


Sample* load_sample(const char*filename){
    FILE* fp = fopen(filename, "r");
    if(!fp){
        printf("Could not load %s!\n", filename);
    }
    Sample* sample = (Sample*) malloc(sizeof(Sample));
    sample->loop_point = 0;
    fseek(fp, 0x28, SEEK_SET); //go to data size
    int length  = fgetc(fp);
    length  += fgetc(fp)<<8;
    length  += fgetc(fp)<<16;
    length  += fgetc(fp)<<24;
    length /= 2; //because we're doing short data
    sample->length = length;
    sample->data = (short*)malloc(sizeof(short)*length);
    int i;
    for(i=0;i<length ;i++){
        unsigned char lower = fgetc(fp);
        unsigned char upper = fgetc(fp);
        sample->data[i] = lower + (upper<<8);
    }
    fclose(fp);
    return sample;
}

short sample_render(Sample*s, float pitch, int time){
    /* times by 2 to get only left channel */
    float step = (pow(2,(pitch-57)/12)*2*time);
    int left = step;
    step -= left;
    if(left>=s->length) return 0;
    return step*s->data[left] + (1-step)*s->data[(left+1)];
}

