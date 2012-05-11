#ifndef SAMPLE_H
#define SAMPLE_H

typedef struct{
    short* data;
    int length;
    int loop_point;
} Sample;

Sample* load_sample(const char*filename);

short sample_render(Sample*s, float pitch, int time);

#endif

