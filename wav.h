/**************************************************************************
 * wav.h - Simple library for reading and writing basic WAV files. 
 *
 * This library can not read WAV files that are not simple 8 or 16 bit
 * PCM encoded.  You might need additional software like Audacity
 * (http://audacity.sourceforge.net/) to convert other sound files to
 * a format that this software can read.
 */

#ifndef _WAV_H_ 
#define _WAV_H_

#include <stdio.h>

/* Constants to describe common sample rates for digital audio. */
enum wav_sample_rate {

  /* Pretty bad for anything better than speech. */
  wav_11025 = 11025,

  /* About the quality of an old 78 RPM record. */
  wav_22050 = 22050,
  
  /* CD quality sound. */
  wav_44100 = 44100, 

};

/* Constants to encode the number of audio channels in a file. */
enum wav_num_chans {
  wav_mono = 1,
  wav_stereo = 2,
};

/* Constants to encode number of bits per sample. The number of
   discrete volume levels possible is 2^n for a recording with n bits
   per sample. */
enum wav_bits_per_sample {

  /* May result in "scratchy" sounds in some contexts. */
  wav_8_bit = 8,

  /* CD quality */
  wav_16_bit = 16,

};

/* Open a wave file for writing. */
FILE* wav_open(const char* filename, 
               unsigned short num_chans,
               unsigned int sample_rate,
               unsigned short bits_per_sample);

/* Finish writing the wave file.  Call this instead of just calling
   fclose(wav) because this will seek back to the start of the file
   and write important information about the total number of samples
   written. */
void wav_finish(FILE* wav);

/* Open a wave file for reading. The arguments after the filename are
   all passed in by reference, and can be read after this returns. */
FILE* wav_read(const char* filename,
               unsigned short* num_chans,
               unsigned int*   sample_rate,
               unsigned short* bits_per_sample,
               unsigned int*   num_samples);

/* Read a 32-bit sample from a wave file, converting automatically
   from the indicated number of bits. */
void wav_read_sample(FILE* wav,
                     unsigned short bits_per_sample,
                     int* sample);

/* Write a 32-bit sample to a wave file, converting automatically to
   the indicated number of bits. */
void wav_write_sample(FILE* wav,
                      unsigned short bits_per_sample,
                      int sample);

#endif
