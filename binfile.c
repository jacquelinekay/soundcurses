#include "binfile.h"
#include <stdlib.h>

void bin_write_1(FILE* fp, unsigned char d) {
  if (fwrite(&d, sizeof(d), 1, fp) != 1) {
    fprintf(stderr, "error writing to file\n");
    exit(-1);
  }
}

void bin_write_2(FILE* fp, unsigned short d) {
  if (fwrite(&d, sizeof(d), 1, fp) != 1) {
    fprintf(stderr, "error writing to file\n");
    exit(-1);
  }
}

void bin_write_4(FILE* fp, unsigned int d) {
  if (fwrite(&d, sizeof(d), 1, fp) != 1) {
    fprintf(stderr, "error writing to file\n");
    exit(-1);
  }
}

void bin_write_tag(FILE* fp, const char* c) {
  while (*c) {
    bin_write_1(fp, *c);
    ++c;
  }
}

void bin_read_1(FILE* fp, unsigned char* d) {
  if (fread(d, sizeof(*d), 1, fp) != 1) {
    fprintf(stderr, "error reading from file\n");
    exit(-1);
  }
}

void bin_read_2(FILE* fp, unsigned short* d) {
  if (fread(d, sizeof(*d), 1, fp) != 1) {
    fprintf(stderr, "error reading from file\n");
    exit(-1);
  }
}

void bin_read_4(FILE* fp, unsigned int* d) {
  if (fread(d, sizeof(*d), 1, fp) != 1) {
    fprintf(stderr, "error reading from file\n");
    exit(-1);
  }
}

void bin_match_tag(FILE* fp, const char* c) {
  const char* corig = c;
  unsigned char t;
  while (*c) {
    bin_read_1(fp, &t);
    if ((char)t != *c) {
      fprintf(stderr, "error reading %s tag from file\n", corig);
      exit(-1);
    }
    ++c;
  }
}
