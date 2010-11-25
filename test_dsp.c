/**
 * NON-Functional Sample code from Prof. Zucker[berg]
 *
 */

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Libraries
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main() {


    /*
    int fd;
    fd = open("/dev/dsp", O_RDONLY);
    
    
    
    if (fd < 0) {
        fprintf(stderr, "couldn't open!\n");
        return -1;
    }
    */
    
    FILE* dsp = fopen("/dev/dsp", "rb");
    if (!dsp) {
        fprintf(stderr, "couldn't open!\n");
        return -1;
    }
    
    printf("opened device!\n");
    
    // guess we have 16-bit samples
    int sample_rate = 20000;
    int freq = 400;
    float dt = 1.0/sample_rate;
    float time;
    int count = 0;
    
    for (time=0; time<100; time+=dt) {
        short sample = rand() % 65536 - 32768;
        //short sample = sin(time * freq * 2 * M_PI) * 10000;
        //char sample = sin(time * freq * 2 * M_PI) * 120;
        //write(fd, &sample, sizeof(sample));
        fwrite(&sample, 1, sizeof(sample), dsp);
        ++count; 
    }
    printf("wrote %d samples\n", count);
    
    fflush(dsp);
    printf("closed device!\n");

    //close(fd);
    fclose(dsp);

    return 0;
}
