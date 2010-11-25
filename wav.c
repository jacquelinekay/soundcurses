/**************************************************************************
 * wav.c - Simple library for reading and writing basic WAV files. 
 *
 * See this page for details on the implementation of this file:
 * 
 *    https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 *
 * See wav.h for documentation of these functions.
*/

#include "wav.h"
#include "binfile.h"
#include <stdlib.h>

FILE* wav_open(const char* filename, 
               unsigned short num_chans,
               unsigned int sample_rate,
               unsigned short bits_per_sample) {

  const char* riff_tag = "RIFF";
  const unsigned int zero = 0;
  const char* wave_tag = "WAVE";
  const char* fmt_tag = "fmt ";
  const unsigned int subchunk1_size = 16;
  const char* data_tag = "data";

  unsigned int byte_rate = sample_rate * num_chans * bits_per_sample / 8;
  unsigned int block_align = num_chans * bits_per_sample / 8;

  /* Open the file for writing and reading later on, in binary. */
  FILE* wav = fopen(filename, "w+b");

  if (!wav) { 
    fprintf(stderr, "couldn't open wav file %s for output", filename);
    exit(-1);
  }

  bin_write_tag(wav, riff_tag);
  bin_write_4(wav, zero);
  bin_write_tag(wav, wave_tag);
  bin_write_tag(wav, fmt_tag);
  bin_write_4(wav, subchunk1_size);
  bin_write_2(wav, 1);
  bin_write_2(wav, num_chans);
  bin_write_4(wav, sample_rate);
  bin_write_4(wav, byte_rate);
  bin_write_2(wav, block_align);
  bin_write_2(wav, bits_per_sample);
  bin_write_tag(wav, data_tag);
  bin_write_4(wav, zero);

  return wav;

}

void wav_finish(FILE* wav) {

  long pos = ftell(wav);

  fseek(wav, 4, SEEK_SET);
  bin_write_4(wav, pos - 8);

  fseek(wav, 40, SEEK_SET);
  bin_write_4(wav, pos - 44);

  fclose(wav);
  
}


FILE* wav_read(const char* filename,
               unsigned short* num_chans,
               unsigned int* sample_rate,
               unsigned short* bits_per_sample,
               unsigned int* num_samples) {

  const char* riff_tag = "RIFF";
  const char* wave_tag = "WAVE";
  const char* fmt_tag = "fmt ";
  const char* data_tag = "data";


  unsigned int chunk_size;
  unsigned int subchunk1_size;
  unsigned short audio_format;
  unsigned int byte_rate;
  unsigned short block_align;
  unsigned int subchunk2_size;
  unsigned int chan_size;

  /* Open the file for reading in binary. */
  FILE* wav = fopen(filename, "rb");

  if (!wav) { 
    fprintf(stderr, "couldn't open wav file %s for output", filename);
    exit(-1);
  }

  bin_match_tag(wav, riff_tag);
  bin_read_4(wav, &chunk_size); 
  bin_match_tag(wav, wave_tag);
  bin_match_tag(wav, fmt_tag);
  bin_read_4(wav, &subchunk1_size);

  if (subchunk1_size != 16) {
    fprintf(stderr, "don't know what to do with subchunk1_size of %d\n", subchunk1_size);
    exit(-1);
  }

  // check our data

  bin_read_2(wav, &audio_format);

  if (audio_format != 1) {
    fprintf(stderr, "wav input is not PCM, can't read!\n");
    exit(-1);
  }

  bin_read_2(wav, num_chans);
  bin_read_4(wav, sample_rate);
  bin_read_4(wav, &byte_rate);
  bin_read_2(wav, &block_align);
  bin_read_2(wav, bits_per_sample);

  bin_match_tag(wav, data_tag);
  bin_read_4(wav, &subchunk2_size);


  if (*bits_per_sample != 8 &&
      *bits_per_sample != 16 &&
      *bits_per_sample != 32) {
    fprintf(stderr, "can't read bits per sample of %u\n", 
            *bits_per_sample);
    exit(-1);
  }

  if (!*num_chans ) {
    fprintf(stderr, "no channels!\n");
    exit(-1);
  }

  // we want
  //   subchunk2_size = num_samples * num_chans * bits_per_sample / 8
  // so
  //   num_samples = subchunk2_size / (num_chans * bits_per_sample / 8)

  chan_size = (*num_chans * *bits_per_sample / 8);
  
  *num_samples = subchunk2_size / chan_size;

  if (subchunk2_size % *num_samples) {
    fprintf(stderr, "incorrect size for subchunk 2!\n");
    exit(-1);
  }

  if (chunk_size != subchunk2_size + 36) {
    fprintf(stderr, "incorrect chunk size!\n");
    exit(-1);
  }

  return wav;
  

}

void wav_read_sample(FILE* wav,
                     unsigned short bits_per_sample,
                     int* sample) {

  unsigned char d1;
  unsigned short d2;
  unsigned int d4;

  switch (bits_per_sample) {
  case 8:
    bin_read_1(wav, &d1);
    *sample = (unsigned int)d1 << 24;
    break;
  case 16:
    bin_read_2(wav, &d2);
    *sample = (unsigned int)d2 << 16;
    break;
  case 32:
    bin_read_4(wav, &d4);
    *sample = d4;
    break;
  default:
    fprintf(stderr, "unrecognized bits_per_sample in wav_read_sample\n");
    exit(-1);
  }
    
}

void wav_write_sample(FILE* wav,
                      unsigned short bits_per_sample,
                      int sample) {

  unsigned char d1;
  unsigned short d2;
  unsigned int d4;

  switch (bits_per_sample) {
  case 8:
    d1 = sample >> 24;
    bin_write_1(wav, d1);
    break;
  case 16:
    d2 = sample >> 16;
    bin_write_2(wav, d2);
    break;
  case 32:
    d4 = sample;
    bin_write_4(wav, d4);
    break;
  default:
    fprintf(stderr, "unrecognized bits_per_sample in wav_write_sample\n");
    exit(-1);
  }

}
