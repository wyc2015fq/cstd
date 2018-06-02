
#include "facedetect.h"
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib,"libfacedetect.lib")

typedef unsigned int DWORD;
typedef int LONG;
typedef unsigned short WORD;

struct BitmapInfo {
  DWORD      biSize;
  LONG       biWidth;
  LONG       biHeight;
  WORD       biPlanes;
  WORD       biBitCount;
  DWORD      biCompression;
  DWORD      biSizeImage;
  LONG       biXPelsPerMeter;
  LONG       biYPelsPerMeter;
  DWORD      biClrUsed;
  DWORD      biClrImportant;
};

int readBmp(const char* path, unsigned char **imageData, int *pWidth, int *pHeight, int* pLineByte, int *pChannel)
{
  FILE *fp;
  int i, lineByte, biBitCount, ret = 0;
  struct BitmapInfo head;
  printf("sizeof(head) %d\n", sizeof(head));
  fp = fopen(path, "rb");
  if (fp)
  {
    fseek(fp, 14, 0);
    fread(&head, sizeof(struct BitmapInfo), 1, fp);
    *pWidth = head.biWidth;
    *pHeight = head.biHeight;
    biBitCount = head.biBitCount;
    if ((biBitCount&7)==0) {
      *pChannel = biBitCount>>3;
      lineByte = ((*pWidth) * biBitCount / 8 + 3) / 4 * 4;
      *pLineByte = lineByte;
      *imageData = (unsigned char *)malloc(lineByte * (*pHeight));
      for (i = 0; i < *pHeight; i++) {
        fseek(fp, (*pHeight - 1 - i) * lineByte + 54, SEEK_SET);
        fread(*imageData + i * (*pWidth) * 3, 1, (*pWidth) * 3, fp);
      }
      ret = 1;
    }
  }
  fclose(fp);
  return ret;
}
int main() {
  struct FaceSet* faceset = NULL;
  unsigned char* imageData = NULL;
  int Width = 0, Height = 0, LineByte=0, Channel=0;
  int ret = readBmp("hztest.BMP", &imageData, &Width, &Height, &LineByte, &Channel);
  if (ret && imageData) {
    int i, buflen = 100*(1<<20);
    unsigned char *buf = NULL;
    buf = (unsigned char*)malloc(buflen);
    printf("Height:%d Width:%d\n", Height, Width);
    for (i=0; i<1; ++i) {
      faceset = facedetect(buf, buflen, Height, Width, imageData, LineByte, Channel, 60);
      printf("time_used: %f ms\n", faceset->time_used);
    }
    if (faceset) {
      for (i=0; i<faceset->nfaces; ++i) {
        struct Face* face = faceset->faces + i;
        printf("%2d (%3d %3d %3d %3d)(%6.2f %6.2f %6.2f)\n", i,
          (int)face->rect.x, (int)face->rect.y, (int)face->rect.width, (int)face->rect.height, 
          face->headpose.pitch_angle, face->headpose.yaw_angle, face->headpose.roll_angle);
      }
    }
    free(buf);
    free(imageData);
  }
  return 0;
}
