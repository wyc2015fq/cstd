/*
 * $Id: bitmap.h,v 1.14 2008/11/15 19:38:54 sfeam Exp $
 */

/* GNUPLOT - bitmap.h */

#ifndef GNUPLOT_BITMAP_H
# define GNUPLOT_BITMAP_H

#include "syscfg.h"

/* allow up to 16 bit width for character array */
typedef unsigned int char_row;
typedef char_row const GPFAR* GPFAR char_box;

#define FNT_CHARS   96    /* Number of characters in the font set */

#define FNT5X9 0
#define FNT5X9_VCHAR 11   /* vertical spacing between characters */
#define FNT5X9_VBITS 9    /* actual number of rows of bits per char */
#define FNT5X9_HCHAR 7    /* horizontal spacing between characters */
#define FNT5X9_HBITS 5    /* actual number of bits per row per char */
extern const char_row GPFAR fnt5x9[FNT_CHARS][FNT5X9_VBITS];

#define FNT9X17 1
#define FNT9X17_VCHAR 21  /* vertical spacing between characters */
#define FNT9X17_VBITS 17  /* actual number of rows of bits per char */
#define FNT9X17_HCHAR 13  /* horizontal spacing between characters */
#define FNT9X17_HBITS 9   /* actual number of bits per row per char */
extern const char_row GPFAR fnt9x17[FNT_CHARS][FNT9X17_VBITS];

#define FNT13X25 2
#define FNT13X25_VCHAR 31 /* vertical spacing between characters */
#define FNT13X25_VBITS 25 /* actual number of rows of bits per char */
#define FNT13X25_HCHAR 19 /* horizontal spacing between characters */
#define FNT13X25_HBITS 13 /* actual number of bits per row per char */
extern const char_row GPFAR fnt13x25[FNT_CHARS][FNT13X25_VBITS];


typedef unsigned char pixels;  /* the type of one set of 8 pixels in bitmap */
typedef pixels* bitmap[]; /* the bitmap */

extern bitmap* b_p;   /* global pointer to bitmap */
extern unsigned int b_xsize, b_ysize; /* the size of the bitmap */
extern unsigned int b_planes; /* number of color planes */
extern unsigned int b_psize;  /* size of each plane */
extern unsigned int b_rastermode; /* raster mode rotates -90deg */
extern unsigned int b_linemask; /* 16 bit mask for dotted lines */
extern unsigned int b_angle;  /* rotation of text */
extern int b_maskcount;


/* Prototypes from file "bitmap.c" */

unsigned int b_getpixel __PROTO((unsigned int, unsigned int));
void b_makebitmap __PROTO((unsigned int, unsigned int, unsigned int));
void b_freebitmap __PROTO((void));
void b_charsize __PROTO((unsigned int));
void b_setvalue __PROTO((unsigned int));

void b_setlinetype __PROTO((int));
void b_move __PROTO((unsigned int, unsigned int));
void b_vector __PROTO((unsigned int, unsigned int));
void b_put_text __PROTO((unsigned int, unsigned int, const char*));
int b_text_angle __PROTO((int));
void b_boxfill __PROTO((int, unsigned int, unsigned int, unsigned int, unsigned int));

#endif /* GNUPLOT_BITMAP_H */
