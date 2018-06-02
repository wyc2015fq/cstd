/* Copyright 1993-2003 The MathWorks, Inc.  */

/*

   applylutc.c  .MEX file
               Applies a 16-by-1 lookup table to 2-by-2 sliding
               neighborhoods in a binary image; or applies a
               512-by-1 lookup table to 3-by-3 sliding neighborhoods
               in a binary image.

               For 2-by-2 neighborhoods, the neighborhood examined
               for each pixel is the lower right one, and lookup
               indices are determined by applying this mask:

                        1 4
                        2 8

               For 3-by-3 neighborhoods, the lookup indices are
               determined by applying this mask:

                        1  8  64
                        2 16 128
                        4 32 256

               A = APPLYLUTC(BW, LUT)

               BW can be either numeric or logical.  BW must be real, 
               two-dimensional, and nonsparse.

               LUT can be either numeric or logical.  LUT must be a real 
               vector. 

               BW and LUT and be either numeric double or numeric
               uint8.  BW and LUT must be real.

               Case 1: LUT is uint8
                 Output will be uint8

               Case 2: LUT is double
                 If all LUT values are integers between 0 and 255,
                 output will be uint8; otherwise output will be
                 double.           

               If output contains only 0's and 1's, it will be logical.

               Steven L. Eddins
               March 1996

*/

static char rcsid[] = "$Revision: 1.1.6.1 $";

#include <math.h>
//#include "mex.h"
typedef short int16_T;
typedef uchar uint8_T;
typedef uchar mxLogical;
typedef uchar luttype;

#define MATRIX_REF(PR, STEP, R, C)    (*((PR) + (C) + (STEP)*(R)))

static int16_T weights2[2][2] = {1, 4, 2, 8};

int16_T Nhood2Offset(const mxLogical *pBWin, int step, int numRows, int numCols, 
                     int r, int c) {

    int maxR, maxC;
    int rr, cc;
    int16_T result = 0;

    /* Figure out the neighborhood extent that does not go past */
    /* image boundaries */
    if (r == (numRows-1)) {
        maxR = 0;
    } else {
        maxR = 1;
    }
    if (c == (numCols-1)) {
        maxC = 0;
    } else {
        maxC = 1;
    }

    for (rr = 0; rr <= maxR; rr++) {
        for (cc = 0; cc <= maxC; cc++) {
            result += weights2[rr][cc] *
              (MATRIX_REF(pBWin, step, r+rr, c+cc) != 0);
        }
    }

    return(result);
}

static int16_T weights3[3][3] = {1, 8, 64, 2, 16, 128, 4, 32, 256};

int16_T Nhood3Offset(const mxLogical *pBWin, int step, int numRows, int numCols, int r, int c) {

    int minR, maxR, minC, maxC;
    int rr, cc;
    int16_T result = 0;
    
    /* Figure out the neighborhood extent that does not go past */
    /* image boundaries */
    if (r == 0) {
        minR = 1;
    } else {
        minR = 0;
    }
    if (r == (numRows-1)) {
        maxR = 1;
    } else {
        maxR = 2;
    }
    if (c == 0) {
        minC = 1;
    } else {
        minC = 0;
    }
    if (c == (numCols-1)) {
        maxC = 1;
    } else {
        maxC = 2;
    }

    for (rr = minR; rr <= maxR; rr++) {
        for (cc = minC; cc <= maxC; cc++) {
            result += weights3[rr][cc] * 
              (MATRIX_REF(pBWin, step, r + rr - 1, c + cc - 1) != 0);
        }
    }

    return(result);
}

#define mxGetLogicals(im)  im->tt.data
#define mxGetData(im)  im->tt.data
#define mxGetData(im)  im->tt.data
#define mxGetM(im)  im->h
#define mxGetN(im)  im->w

void Compute2by2DoubleResult(img_t *BWout, 
                             const img_t *BWin, 
                             img_t *lut) {
    
    int numRows, numCols;
    int r, c;
    mxLogical *pBWin;
    luttype *plut;
    double *pBWout;

    pBWin = mxGetLogicals(BWin);
    plut = (luttype *) mxGetData(lut);
    pBWout = (double *) mxGetData(BWout);
    
    numRows = mxGetM(BWin);
    numCols = mxGetN(BWin);

    for (c = 0; c < numCols; c++) {
        for (r = 0; r < numRows; r++) {
            MATRIX_REF(pBWout, BWout->s, r, c) =
              *(plut + Nhood2Offset(pBWin, BWin->s, numRows, numCols, r, c));
        }
    }
}

void Compute3by3DoubleResult(img_t *BWout, 
                             const img_t *BWin, 
                             img_t *lut) {
    
    int numRows, numCols;
    int r, c;
    mxLogical *pBWin;
    luttype *plut;
    double *pBWout;

    pBWin = mxGetLogicals(BWin);
    plut = (luttype *) mxGetData(lut);
    pBWout = (double *) mxGetData(BWout);

    numRows = mxGetM(BWin);
    numCols = mxGetN(BWin);

    for (c = 0; c < numCols; c++) {
        for (r = 0; r < numRows; r++) {
            MATRIX_REF(pBWout, BWout->s, r, c) =
              *(plut + Nhood3Offset(pBWin, BWin->s, numRows, numCols, r, c));
        }
    }
}

void Compute2by2Uint8Result(img_t *BWout, 
                            const img_t *BWin, 
                            img_t *lut) {
    
    int numRows, numCols;
    int r, c;
    mxLogical *pBWin;
    luttype *plut;
    uint8_T *pBWout;

    pBWin = mxGetLogicals(BWin);
    plut = (luttype *) mxGetData(lut);
    pBWout = (uint8_T *) mxGetData(BWout);

    numRows = mxGetM(BWin);
    numCols = mxGetN(BWin);

    for (c = 0; c < numCols; c++) {
        for (r = 0; r < numRows; r++) {
            MATRIX_REF(pBWout, BWout->s, r, c) = (uint8_T)
              *(plut + Nhood2Offset(pBWin, BWin->s, numRows, numCols, r, c));
        }
    }
}

void Compute3by3Uint8Result(img_t *BWout, 
                            const img_t *BWin, 
                            img_t *lut) {
    
    int numRows, numCols;
    int r, c;
    mxLogical *pBWin;
    luttype *plut;
    uint8_T *pBWout;

    pBWin = mxGetLogicals(BWin);
    plut = (luttype *) mxGetData(lut);
    pBWout = (uint8_T *) mxGetData(BWout);

    numRows = mxGetM(BWin);
    numCols = mxGetN(BWin);

    for (c = 0; c < numCols; c++) {
        for (r = 0; r < numRows; r++) {
            MATRIX_REF(pBWout, BWout->s, r, c) = (uint8_T)
              *(plut + Nhood3Offset(pBWin, BWin->s, numRows, numCols, r, c));
        }
    }
}

void Compute2by2LogicalResult(img_t *BWout, 
                              const img_t *BWin, 
                              img_t *lut) {
    
    int numRows, numCols;
    int r, c;
    mxLogical *pBWin;
    luttype *plut;
    mxLogical *pBWout;

    pBWin = mxGetLogicals(BWin);
    plut = (luttype *) mxGetData(lut);
    pBWout = mxGetLogicals(BWout);

    numRows = mxGetM(BWin);
    numCols = mxGetN(BWin);

    for (c = 0; c < numCols; c++) {
        for (r = 0; r < numRows; r++) {
            MATRIX_REF(pBWout, BWout->s, r, c) = (mxLogical)
              *(plut + Nhood2Offset(pBWin, BWin->s, numRows, numCols, r, c));
        }
    }
}

int Compute3by3LogicalResult(img_t *BWout, 
                              const img_t *BWin, 
                              img_t *lut) {
    
    int numRows, numCols;
    int r, c, cnt=0;
    mxLogical *pBWin;
    luttype *plut;
    mxLogical *pBWout;

    pBWin = mxGetLogicals(BWin);
    plut = (luttype *) mxGetData(lut);
    pBWout = mxGetLogicals(BWout);

    numRows = mxGetM(BWin);
    numCols = mxGetN(BWin);
    
    for (r = 0; r < numRows; r++) {
      for (c = 0; c < numCols; c++) {
            mxLogical t =  (mxLogical)*(plut + Nhood3Offset(pBWin, BWin->s, numRows, numCols, r, c));
            cnt+= t!=MATRIX_REF(pBWout, BWout->s, r, c);
            MATRIX_REF(pBWout, BWout->s, r, c) =t;
        }
    }
    return cnt;
}

//#define BW_IN  (prhs[0])
//#define LUT_IN (prhs[1])

/* In addition to validating the inputs, this function */
/* converts the input BW array to uint8 and the input LUT */
/* array to double if necessary. */

    //case 16:        *nhoodSize = 2;
    //case 512:        *nhoodSize = 3;



