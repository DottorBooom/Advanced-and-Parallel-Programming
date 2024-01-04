// Davide Martinelli SM3201226
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdint.h>
#include <omp.h>
#include <immintrin.h>

#define THREADS omp_get_max_threads()
#include "pgm.h"
#include "mandelbrot.h"

int8_t open_image(char * path, pgm_ptr img)
{
  img->fd = fopen(path, "r+");
  if (img->fd == NULL) {
    return -8;
  }
  struct stat sbuf;
  stat(path, &sbuf);
  img->size = sbuf.st_size;
  if (fscanf(img->fd, "P5\n%d %d\n255\n", &img->width, &img->height) != 2) {
    fclose(img->fd);
    return -9;
  }
  img->offset = ftell(img->fd);
  img->data = mmap((void *)0, img->size, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(img->fd), 0);
  if (img->data == MAP_FAILED) {
    fclose(img->fd);
    return -10;
  }
  return 0;
}

int8_t empty_image(char * path, pgm_ptr img, int width, int height)
{
  FILE * fd = fopen(path, "w+");
  if (fd == NULL) {
    return -6;
  }
  int written = fprintf(fd, "P5\n%d %d\n255\n", width, height);
  int r = ftruncate(fileno(fd), written + width * height);
  if(r != 0){
    return -7;
  }
  fclose(fd);
  return open_image(path, img);
}

char * pixel_at(pgm_ptr img, int x, int y)
{
  if (img == NULL) {
    return NULL;
  }
  if (x < 0 || x >= img->width) {
    return NULL;
  }
  if (y < 0 || y >= img->height) {
    return NULL;
  }
  return &img->data[y * img->width + x + img->offset];
}

int8_t close_image(pgm_ptr img)
{
  if (img == NULL) {
    return -11;
  }
  munmap(img->data, img->size);
  fclose(img->fd);
  return 0;
}

void print_image(int * components, pgm_ptr image)
{
  float x_step = (float)(3)/(float)(components[2] - 1);
  float y_step = (float)(2)/(float)(components[1] - 1);
  #pragma omp parallel for schedule(dynamic) num_threads(THREADS)
  for (int y = 0; y < components[1]/2; y++) {
    float imm = -1+(y*y_step);
    for (int x = 0; x < components[2]; x++) {
      float real = -2+(x*x_step);
      char * c = pixel_at(image, x, y);
      char * c_p = pixel_at(image, x, components[1]-1-y); 
      if (c == NULL) {
        printf("...error at x = %d y = %d.\n", x, y);
      }else{
        if (c_p == NULL) {
          printf("...error at x = %d y = %d.\n", x, (components[1]/2)-1-y);
        }else{
          uint16_t n = abs_check(real,imm,components[0]);
          *c = (int)(255*log(n)/(float)log(components[0]));
          *c_p = (int)(255*log(n)/(float)log(components[0]));
        }
      }
    }
  }
  if(components[3] == 1){
    #pragma omp parallel for schedule(dynamic) num_threads(THREADS)
    for (int x = 0; x < components[2]; x++) {
      float real = -2+(x*x_step);
      char * c = pixel_at(image, x, components[1]/2);
      if (c == NULL) {
        printf("...error at x = %d y = %d.\n", x, components[1]/2);
      }else{
        uint16_t n = abs_check(real,0,components[0]);
        *c = (int)(255*log(n)/(float)log(components[0]));
      }
    }
  }
}