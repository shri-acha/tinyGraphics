#include "../graphics.h"
#include <stdio.h>
#include <stdlib.h>

int format_frame_buffer (frameBuffer* fb){
  printf("\n");
  for (int i = 0;i<fb->width; i++) {
    for (int j = 0;j<fb->height; j++) {
      if (fb->buffer[j*fb->width+i].intensity) {
        printf("\e[32m%d\e[0m\t",fb->buffer[(j*(fb->width))+i].intensity);
      }else{
        printf("%d\t",fb->buffer[(j*(fb->width))+i].intensity);
      }
    }
    printf("\n");
  }
  return 0;
}

int main() {
  frameBuffer fb = {.buffer = (pixelBuffer *)calloc(50 * 50,sizeof(pixelBuffer)), .height = 50, .width = 50};
  // renderLine(&fb,1,1,50,50);
  // renderLine(&fb,1,50,50,50);
  renderLine(&fb,1,1,50,25);
  format_frame_buffer(&fb);
  return 0;
}

