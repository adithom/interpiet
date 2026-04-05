#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm.h"
#include "types.h"

void ppm_free(PPMImage *img) {
    if (!img) return;
    free(img->pixels);
    free(img);
}

PPMImage *ppm_load(const char *path){

 FILE *f = fopen(path, "rb");
 if (!f) {
 fprintf(stderr, "ppm_load: cannot open %s\n", path);
   return NULL;
 }

 char magic[3] = {0};
 fscanf(f, "%2s", magic);

 if (magic[0] != 'P' || (magic[1] != '3' && magic[1] != '6')) {
   fprintf(stderr, "ppm_load: not a PPM file\n");
   fclose(f);
   return NULL;
 }

 int is_binary = (magic[1] == '6');
 int width, height, maxval;

 int c;
 while(1) {
   while ((c = fgetc(f)) != EOF && (c == ' ' || c == '\t' || c == '\n' || c == '\r') );

   if (c == '#') {
     while ((c = fgetc(f)) != EOF && c != '\n');
   } else {
     ungetc(c, f);
     break;
   }
 }

 fscanf(f, "%d %d %d", &width, &height, &maxval);

 if (is_binary) {
   fgetc(f);
 }

 PPMImage *img = malloc(sizeof(PPMImage));
 if (!img) {
   fclose(f);
   return NULL;
 }

 img->width = width;
 img->height = height;
 img->pixels = malloc(width * height * 3);

 if (!img -> pixels) {
   free(img);
   fclose(f);
   return NULL;
 }

 if (is_binary) {
   size_t bytes_read = fread(img->pixels, 1, width * height * 3, f);
   if (bytes_read != (size_t)(width * height * 3)) {
     fprintf(stderr, "ppm_load: unexpected end of file\n");
     ppm_free(img);
     fclose(f);
     return NULL;
   }
 } else {
   for (int i = 0; i < width * height * 3; i++) {
     int val;
     if (fscanf(f, "%d", &val) != 1) {
       fprintf(stderr, "ppm_load: malformed P3 data\n");
       ppm_free(img);
       fclose(f);
       return NULL;
     }
     img->pixels[i] = (uint8_t)val;
   }
 }

 fclose(f);
 return img;
}

