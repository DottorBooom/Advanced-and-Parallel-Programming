// Davide Martinelli SM3201226
#ifndef _PGM_H
#define _PGM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdint.h>

struct _pgm_image {
  int width;
  int height;
  int offset;
  int size;
  FILE * fd;
  char * data;
};

typedef struct _pgm_image pgm;
typedef struct _pgm_image * pgm_ptr;

int8_t open_image(char * path, pgm_ptr img);

int8_t empty_image(char * path, pgm_ptr img, int width, int height);

char * pixel_at(pgm_ptr img, int x, int y);

int8_t close_image(pgm_ptr img);

void print_image(int * components, pgm_ptr image, uint16_t * min_n);

#endif