/**************************************************************************
 * soundcurses.c - Send sound to the device when a button is pressed.
 */

#include "wav.h"
#include "binfile.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <linux/soundcard.h>
#include <ncurses.h>

#define LENGTH 3    /* how many seconds of speech to store */
#define RATE 8000   /* the sampling rate */
#define SIZE 16      /* sample size: 8 or 16 bits */
#define CHANNELS 1  /* 1 = mono 2 = stereo */

/* Generate a pure tone by sampling a sine wave with the given
   frequency at the time indicated. */
float pure_tone(float time, float freq_hz) {
  return sin(time * freq_hz * 2 * M_PI);
}

int main(int argc, char** argv) {

  const int sample_rate = RATE;
  const float dt = 1.0 / sample_rate;
  const float freq_hz = 440; /* A above middle C */
  float time;
  
  int fd;	/* sound device file descriptor */
  int arg;	/* argument for ioctl calls */
  int status;   /* return status of system calls */

  /* open sound device */
  fd = open("/dev/dsp", O_RDWR);
  if (fd < 0) {
    perror("open of /dev/dsp failed");
    exit(1);
  }

  arg = SIZE;	   /* sample size */
  status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);
  if (status == -1)
    perror("SOUND_PCM_WRITE_BITS ioctl failed");
  if (arg != SIZE)
    perror("unable to set sample size");

  arg = CHANNELS;  /* mono or stereo */
  status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);
  if (status == -1)
    perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
  if (arg != CHANNELS)
    perror("unable to set number of channels");

  arg = RATE;	   /* sampling rate */
  status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);
  if (status == -1)
    perror("SOUND_PCM_WRITE_WRITE ioctl failed");
  
  for (time=0; time<LENGTH; time+=dt) {  //Generate sound and write it to dsp

    float fsample = pure_tone(time, freq_hz);

    int sample = (int)(fsample * 1e16);
    
    status = write(fd, sample, sizeof(sample));     //This line is giving problems.
    if (status != sizeof(sample))
      perror("wrote wrong number of bytes");
      
    status = ioctl(fd, SOUND_PCM_SYNC, 0); 
    if (status == -1)
        perror("SOUND_PCM_SYNC ioctl failed");
  }
  
  return 0;

}
