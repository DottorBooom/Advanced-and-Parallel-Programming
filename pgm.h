#ifndef _PGM_H
#define _PGM_H

#include <stdio.h>

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

int open_image(char * path, pgm_ptr img);

int empty_image(char * path, pgm_ptr img, int width, int height);

char * pixel_at(pgm_ptr img, int x, int y);

int close_image(pgm_ptr img);

#endif