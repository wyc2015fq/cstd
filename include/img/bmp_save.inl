
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define f_eof(stream)                    (stream)->eof(stream)
#define f_close(stream)                  (stream)->close(stream)
#define f_tell(stream)                   (stream)->tell(stream)
#define f_get8(stream)                   (stream)->get8(stream)
#define f_unget8(ch, stream)             (stream)->unget8(ch, stream)
#define f_put8(stream, x)                (stream)->put8(stream, x)
#define f_seek(stream, offset, origin)   (stream)->seek((stream), offset, origin)
#define f_read(stream, buffer, len)      (stream)->read((stream), buffer, len)
#define f_write(stream, buffer, len)     fwrite(buffer, len, 1, stream)
#define f_init_func(s, name)  (s->put8 = name##f_put8 , s->eof = name##f_eof , s->close = name##f_close , s->tell = name##f_tell , s->get8 = name##f_get8 , s->unget8 = name##f_unget8 , s->seek = name##f_seek , s->read = name##f_read , s->write = name##f_write )
#define f_getu1(stream) f_get8(stream)
#define f_getu2(stream)  ((f_getu1(stream))|(f_getu1(stream)<<8))
#define f_getu4(stream)  ((f_getu1(stream))|(f_getu1(stream)<<8)|(f_getu1(stream)<<16)|(f_getu1(stream)<<24))

#define MEM_GET16B(p, i, x) x = (i+=2, (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET16L(p, i, x) x = (i+=2, (((uchar*)p)[i-1]<<8) | (((uchar*)p)[i-2]))
#define MEM_GET32B(p, i, x) x = (i+=4, (((uchar*)p)[i-4]<<24) | (((uchar*)p)[i-3]<<16) | (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET32L(p, i, x) x = (i+=4, (((uchar*)p)[i-1]<<24) | (((uchar*)p)[i-2]<<16) | (((uchar*)p)[i-3]<<8) | (((uchar*)p)[i-4]))

#define MEM_SET16B(p, i, x) (((uchar*)p)[i+0] = ((x)>>8), ((uchar*)p)[i+1] = (x)&0xff, i+=2)
#define MEM_SET16L(p, i, x) (((uchar*)p)[i+1] = ((x)>>8), ((uchar*)p)[i+0] = (x)&0xff, i+=2)
#define MEM_SET32B(p, i, x) (((uchar*)p)[i+0] = ((x)>>24), ((uchar*)p)[i+1] = ((x)>>16), ((uchar*)p)[i+2] = ((x)>>8), ((uchar*)p)[i+3] = (x), i+=4)
#define MEM_SET32L(p, i, x) (((uchar*)p)[i+3] = ((x)>>24), ((uchar*)p)[i+2] = ((x)>>16), ((uchar*)p)[i+1] = ((x)>>8), ((uchar*)p)[i+0] = (x), i+=4)
#define MEM_SETMEM(p, i, x, n) (memcpy((uchar*)p + i, x, n), i+=n)


typedef unsigned char uchar;

static int palette_fillgray1(int n, uchar* palette, int negative)
{
  int i;
  int xor_mask = negative ? 255 : 0;
  
  for (i = 0; i < n; i++) {
    uchar* p = palette + i * 4;
    int val = (i * 255 / (n - 1)) ^ xor_mask;
    p[0] = p[1] = p[2] = (unsigned char) val;
    palette[3] = 0;
  }
  
  return 0;
}

static int bmp_save(const char* filename, int height, int width, const uchar* data, int step, int channels, const uchar* palette)
{
  FILE* s = fopen(filename, "wb");
  uchar buf[1024];
  int i = 0;
  int fileStep = (width * channels + 3) & -4; //4×Ö½Ú¶ÔÆë
  int bitmapHeaderSize = 40;
  int paletteSize = channels > 1 ? 0 : 1024;
  const unsigned char zeropad[4] = {0, 0, 0, 0};
  int headerSize = 14 /* fileheader */ + bitmapHeaderSize + paletteSize;
  int val;
  short st;
  memcpy(buf, "BM", 2);
  i = 2;
  val = fileStep * height + headerSize;
  MEM_SET32L(buf, i, val);   // file size
  val = 0;
  MEM_SET32L(buf, i, val);
  MEM_SET32L(buf, i, headerSize);
  
  // write bitmap header
  MEM_SET32L(buf, i, bitmapHeaderSize);
  MEM_SET32L(buf, i, width);
  MEM_SET32L(buf, i, height);
  st = 1;
  MEM_SET16L(buf, i, st);
  st = (short)(channels << 3);
  MEM_SET16L(buf, i, st);
  val = 0;
  MEM_SET32L(buf, i, val);
  val = 0;
  MEM_SET32L(buf, i, val);
  MEM_SET32L(buf, i, val);
  MEM_SET32L(buf, i, val);
  MEM_SET32L(buf, i, val);
  MEM_SET32L(buf, i, val);
  f_write(s, buf, i);
  
  if (channels == 1) {
    palette_fillgray1(1 << 8, buf, 0);
    f_write(s, buf, 1024);
  }
  
  width *= channels;
  data += step * (height - 1);
  
  for (; height--; data -= step) {
    f_write(s, data, width);
    
    if (fileStep > width) {
      f_write(s, zeropad, fileStep - width);
    }
  }
  fclose(s);
  return 1;
}
#define BOUND(x,a,b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))


static int save_double_to_bmp(const char* filename, double* pd, int width, int height, int cn) {
  int i, len = width*height;
  if (3==cn || 1==cn) {
    uchar* data = (uchar*)malloc(cn*len);
    if (3==cn) {
      for(i=0; i<len; ++i) {
        data[i*3+0] = (int)(BOUND(pd[i*3+2], 0, 1)*255);
        data[i*3+1] = (int)(BOUND(pd[i*3+1], 0, 1)*255);
        data[i*3+2] = (int)(BOUND(pd[i*3+0], 0, 1)*255);
      }
    } else if (1==cn) {
      for(i=0; i<len; ++i) {
        data[i] = (int)(BOUND(pd[i], 0, 1)*255);
      }
    }
    bmp_save(filename, height, width, data, width*cn, cn, NULL);
    free(data);
  }
  return 0;
}
