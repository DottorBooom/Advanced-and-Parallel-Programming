// Davide Martinelli SM3201226
//--------------------------------------------------------------//
//                        LIBRARIES                             //
//--------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <math.h>
#include <stdint.h>
//--------------------------------------------------------------//
//                    HOME MADE LIBRARIES                       //
//--------------------------------------------------------------//
#include "pgm.h"
//--------------------------------------------------------------//
//                            CODE                              //
//--------------------------------------------------------------//
/* This entire library is dedicated to image management and its memory space. 
   I will only comment on the function created to print the image since the other 
   functions were taken from the professor and were not implemented by me.
   P.S. I apologize, Professor Manzoni, if I shouldn't have done that, but they were 
   too well-made and convenient compared to what I could have done.
*/
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

void print_image(int *components, pgm_ptr image, uint16_t *min_n)
{
/* This function scrolls through the entire image space, takes a pointer to the 
   pixel for each row-column intersection, and assigns them a grayscale color 
   from the array calculated in the Mandelbrot set.
*/
  #pragma omp parallel for
  /* Since I find the rest of the code fairly self-explanatory at first glance, 
     it consists of two nested for loops that simulate a row-column structure and take 
     a pointer to a specific pixel. I won't comment much on this code function.
  */
  for (int y = 0; y < components[1]/2; y++) {
    for (int x = 0; x < components[2]; x++) {
      // Why 2 pixel? They consist in the firs and last row. Why? Beacause of the symmetry at y=0, we assign 
      // the value simultaneously from the outside towards the inside. 
      // We just need to be carfule to the indicies.
      char * c = pixel_at(image, x, y);
      char * c_p = pixel_at(image, x, components[1]-1-y); 
      if (c == NULL) {
        printf("...error at x = %d y = %d.\n", x, y);
      }else{
        if (c_p == NULL) {
          printf("...error at x = %d y = %d.\n", x, (components[1]/2)-1-y);
        }else{
          *c = (int)(255*log(min_n[components[2]*y+x])/(float)log(components[0]));
          *c_p = (int)(255*log(min_n[components[2]*y+x])/(float)log(components[0]));
        }
      }
    }
  }
  if(components[1] % 2 == 1){ // This particula line check if the number of rows are odd. If so, we compute the middle lane independently.
    for (int x = 0; x < components[2]; x++) {
      char * c = pixel_at(image, x, components[1]/2);
      if (c == NULL) {
        printf("...error at x = %d y = %d.\n", x, components[1]/2);
      }else{
        *c = (int)(255*log(min_n[components[2]*(components[1]/2)+x])/(float)log(components[0]));
      }
    }
  }
}