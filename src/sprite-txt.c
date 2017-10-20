#include "sprite-txt.h"

struct _Sprite{
  unsigned int width;
  unsigned int height;
  unsigned int depth;
  short*** pixels;
};

/*
 * Private function, definition below. Returns the floor of a base 2 log of x.
 */
int log2_floor (int x);

Sprite *sprite_new(const char * filename){
  if (!filename){
    handle_error("null filename","sprite_new",__FILE__,__LINE__);
    return NULL;
  }

  FILE* fp;
  fp = fopen(filename, "r");
  if (!fp){
    handle_error("error opening the file","sprite_new",__FILE__,__LINE__);
    return NULL;
  }

  Sprite *spr = calloc (1, sizeof(Sprite));
  if(!spr){
    handle_error("spr out of memory", "sprite_new",__FILE__,__LINE__);
    fclose(fp);
    return NULL;
  }
  /*
   * Gets width and height
   */
   char buffer[MAXBUFF];
   fgets(buffer, MAXBUFF, fp);
   sscanf(buffer, "# ImageMagick pixel enumeration: %d,%d,%d,%*s", &spr->width, &spr->height, &spr->depth);
   spr->depth = log2_floor(log2_floor(spr->depth+1));
   spr->depth ++; /* Space for alpha channel */
   /*
    *Allocs memory for pixel data
    */
    spr->pixels = (short***) calloc(spr->width, sizeof(short**));
    if (!spr->pixels){
      handle_error("spr->pixel out of memory","sprite_new",__FILE__,__LINE__);
      free(spr);
      fclose(fp);
      return NULL;
    }
    for (int i = 0; i < spr->width; i++){
      spr->pixels[i]=(short**) calloc (spr->height, sizeof(short *));
        if (!spr->pixels[i]){
          handle_error("spr->pixel[i] out of memory","sprite_new",__FILE__,__LINE__);
          for (int i2 = 0; i2 < i; i2--) {
            free(spr->pixels[i2]);
          }
          free(spr->pixels);
          free(spr);
          fclose(fp);
          return NULL;
        }
      for (int j = 0; j < spr->height; j++){
        spr->pixels[i][j] = (short*) calloc (spr->depth, sizeof(short));
        if (!spr->pixels[i][j]){
          handle_error("spr->pixel[i][j] out of memory","sprite_new",__FILE__,__LINE__);
          for(int i2 = 0; i2 < i; i2++){
            for (int j2 = 0; j2 < j; j2++){
              free(spr->pixels[i][j]);
            }
            free(spr->pixels[i]);
          }
          free(spr->pixels);
          free(spr);
          fclose(fp);
          return NULL;
        }
      }
    }
    /*
     * Charge pixel data
     */
     int pixel_x, pixel_y, pixel_red, pixel_green, pixel_blue, pixel_alpha;
     int limit = spr->width * spr->height;
     for(size_t i = 0; i < limit; ++i){
         fgets(buffer, MAXBUFF, fp);
         sscanf(buffer,"%d,%d: (%d,%d,%d,%d)  #%*s  %*s", &pixel_x, &pixel_y, &pixel_red, &pixel_green, &pixel_blue, &pixel_alpha);
         spr->pixels[pixel_x][pixel_y][RED]=pixel_red;
         spr->pixels[pixel_x][pixel_y][GREEN]=pixel_green;
         spr->pixels[pixel_x][pixel_y][BLUE]=pixel_blue;
         spr->pixels[pixel_x][pixel_y][ALPHA]=pixel_alpha;
     }
     fclose(fp);
     return spr;
}

void sprite_destroy(Sprite *spr){
  if(!spr){
    handle_error("null parameter", "sprite_destroy", __FILE__, __LINE__);
    return;
  }
  for (size_t i = 0; i < spr->width; i++) {
    for (size_t j = 0; j < spr->height; j++) {
      if (spr->pixels[i][j]) free(spr->pixels[i][j]);
    }
    if (spr->pixels[i]) free(spr->pixels[i]);
  }
  if (spr->pixels) free (spr->pixels);
  if (spr) free (spr);
}

void sprite_draw(FILE* fp, Sprite* spr, int x0, int y0){
  if (!fp || !spr){
    handle_error("null parameters", "sprite_draw", __FILE__, __LINE__);
    return;
  }
  if (x0 < 1 || y0 < 1){
    handle_error("coordinates < (1,1)", "sprite_draw", __FILE__, __LINE__);
    return;
  }
  x0 *= 2;
  x0--;
  fprintf(fp, "\033[%d;%dH", y0, x0);

  for (int y = 0; y < spr->height; y++){
    for(int x = 0; x < spr->width; x++){

      if (spr->pixels[x][y][ALPHA] == 0){
        fprintf(fp, "\033[%d;%dH", y0+y, x0+2*(x+1));
      }else{
        fprintf (fp, "\033[38;2;%d;%d;%dm\u2588\u2588",spr->pixels[x][y][RED],
                                                       spr->pixels[x][y][GREEN],
                                                       spr->pixels[x][y][BLUE]);
      }
    }
    fprintf(fp, "\n");
    fprintf(fp, "\033[%d;%dH", y0+y+1, x0);
  }
}

int log2_floor (int x){
    int res = -1;
    while (x) { res++ ; x = x >> 1; }
    return res;
}
