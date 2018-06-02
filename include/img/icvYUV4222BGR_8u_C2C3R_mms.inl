
#include <assert.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "imcolor.h"
#include "tables.h"
#include "mathfuncs.h"

#include "macro.h"

typedef unsigned __int64 UInt64;

const UInt64 csMMX_16_b = 0x1010101010101010; // byte{16,16,16,16,16,16,16,16}
const UInt64 csMMX_128_w = 0x0080008000800080; //short{  128,  128,  128,  128}
const UInt64 csMMX_0x00FF_w = 0x00FF00FF00FF00FF; //掩码
const UInt64 csMMX_Y_coeff_w = 0x2543254325432543; //short{ 9539, 9539, 9539, 9539} =1.164383*(1<<13)
const UInt64 csMMX_U_blue_w = 0x408D408D408D408D; //short{16525,16525,16525,16525} =2.017232*(1<<13)
const UInt64 csMMX_U_green_w = 0xF377F377F377F377; //short{-3209,-3209,-3209,-3209} =(-0.391762)*(1<<13)
const UInt64 csMMX_V_green_w = 0xE5FCE5FCE5FCE5FC; //short{-6660,-6660,-6660,-6660} =(-0.812968)*(1<<13)
const UInt64 csMMX_V_red_w = 0x3313331333133313; //short{13075,13075,13075,13075} =1.596027*(1<<13)

//一次处理8个颜色输出


int icvYUV4222BGR_8u_C2C3R(int height, int width, const unsigned char* src, int srcstep, int srccn,
    unsigned char* dst, int dststep, int dstcn)
{
  int x, y;

  for (y = 0; y < height; ++y) {
    unsigned char* pDstLine = dst + y * dststep;
    const unsigned char* pYUYV = src + y * srcstep;

    for (x = 0; x < width; x += 2 * dstcn, pYUYV += 4) {
      long expand8_width = (width >> 3) << 3;

      if (expand8_width > 0) {
        __asm {
          mov ecx, expand8_width
          mov eax, pYUYV
          mov edx, pDstLine
          lea eax, [ eax + ecx * 2 ]
          lea edx, [ edx + ecx * 4 ]
          neg ecx

          loop_beign:
#define in_yuv_reg eax+ecx*2
#define out_RGB_reg edx+ecx*4
#define WriteCode movq

          movq mm0, [ in_yuv_reg ]   /*mm0=V1 Y3 U1 Y2 V0 Y1 U0 Y0  */
          movq mm4, [ in_yuv_reg + 8 ]   /*mm4=V3 Y7 U3 Y6 V2 Y5 U2 Y4  */
          movq mm1, mm0
          movq mm5, mm4
          psrlw mm1, 8              /*mm1=00 V1 00 U1 00 V0 00 U0  */
          psrlw mm5, 8              /*mm5=00 V3 00 U3 00 V2 00 U2  */
          pand mm0, csMMX_0x00FF_w /*mm0=00 Y3 00 Y2 00 Y1 00 Y0  */
          pand mm4, csMMX_0x00FF_w /*mm4=00 Y7 00 Y6 00 Y5 00 Y4  */
          packuswb mm1, mm5            /*mm1=V3 U3 V2 U2 V1 U1 V0 U0  */
          movq mm2, mm1
          packuswb mm0, mm4            /*mm0=Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0  */
          psllw mm1, 8              /*mm1=U3 00 U2 00 U1 00 U0 00  */
          psrlw mm2, 8              /*mm2=00 V3 00 V2 00 V1 00 V0  */
          psrlw mm1, 8              /*mm1=00 U3 00 U2 00 U1 00 U0  */
          //YUV422ToRGB32_MMX(edx+ecx*4,movq)
          /*input :  mm0 = Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0    */
          /*         mm1 = 00 u3 00 u2 00 u1 00 u0    */
          /*         mm2 = 00 v3 00 v2 00 v1 00 v0    */
          /*output : [out_RGB_reg -- out_RGB_reg+8*4]                 */

          psubusb mm0, csMMX_16_b        /* mm0 : Y -= 16                       */
          psubsw mm1, csMMX_128_w       /* mm1 : u -= 128                      */
          movq mm7, mm0
          psubsw mm2, csMMX_128_w      /* mm2 : v -= 128                      */
          pand mm0, csMMX_0x00FF_w     /* mm0 = 00 Y6 00 Y4 00 Y2 00 Y0       */
          psllw mm1, 3                 /* mm1 : u *= 8                        */
          psllw mm2, 3                 /* mm2 : v *= 8                        */
          psrlw mm7, 8                 /* mm7 = 00 Y7 00 Y5 00 Y3 00 Y1       */
          movq mm3, mm1
          movq mm4, mm2

          pmulhw mm1, csMMX_U_green_w   /* mm1 = u * U_green                   */
          psllw mm0, 3                 /* y*=8                                */
          pmulhw mm2, csMMX_V_green_w   /* mm2 = v * V_green                   */
          psllw mm7, 3                 /* y*=8                                */
          pmulhw mm3, csMMX_U_blue_w
          paddsw mm1, mm2
          pmulhw mm4, csMMX_V_red_w
          movq mm2, mm3
          pmulhw mm0, csMMX_Y_coeff_w
          movq mm6, mm4
          pmulhw mm7, csMMX_Y_coeff_w
          movq mm5, mm1
          paddsw mm3, mm0               /* mm3 = B6 B4 B2 B0       */
          paddsw mm2, mm7               /* mm2 = B7 B5 B3 B1       */
          paddsw mm4, mm0               /* mm4 = R6 R4 R2 R0       */
          paddsw mm6, mm7               /* mm6 = R7 R5 R3 R1       */
          paddsw mm1, mm0               /* mm1 = G6 G4 G2 G0       */
          paddsw mm5, mm7               /* mm5 = G7 G5 G3 G1       */

          packuswb mm3, mm4               /* mm3 = R6 R4 R2 R0 B6 B4 B2 B0 to [0-255] */
          packuswb mm2, mm6               /* mm2 = R7 R5 R3 R1 B7 B5 B3 B1 to [0-255] */
          packuswb mm5, mm1               /* mm5 = G6 G4 G2 G0 G7 G5 G3 G1 to [0-255] */
          movq mm4, mm3
          punpcklbw mm3, mm2               /* mm3 = B7 B6 B5 B4 B3 B2 B1 B0     */
          punpckldq mm1, mm5               /* mm1 = G7 G5 G3 G1 xx xx xx xx     */
          punpckhbw mm4, mm2               /* mm4 = R7 R6 R5 R4 R3 R2 R1 R0     */
          punpckhbw mm5, mm1               /* mm5 = G7 G6 G5 G4 G3 G2 G1 G0     */

          /*out*/
          pcmpeqb mm2, mm2               /* mm2 = FF FF FF FF FF FF FF FF     */

          movq mm0, mm3
          movq mm7, mm4
          punpcklbw mm0, mm5             /* mm0 = G3 B3 G2 B2 G1 B1 G0 B0       */
          punpcklbw mm7, mm2             /* mm7 = FF R3 FF R2 FF R1 FF R0       */
          movq mm1, mm0
          movq mm6, mm3
          punpcklwd mm0, mm7             /* mm0 = FF R1 G1 B1 FF R0 G0 B0       */
          punpckhwd mm1, mm7             /* mm1 = FF R3 G3 B3 FF R2 G2 B2       */
          WriteCode [ out_RGB_reg ], mm0
          movq mm7, mm4
          punpckhbw mm6, mm5             /* mm6 = G7 B7 G6 B6 G5 B5 G4 B4       */
          WriteCode [ out_RGB_reg + 8 ], mm1
          punpckhbw mm7, mm2             /* mm7 = FF R7 FF R6 FF R5 FF R4      */
          movq mm0, mm6
          punpcklwd mm6, mm7             /* mm6 = FF R5 G5 B5 FF R4 G4 B4      */
          punpckhwd mm0, mm7             /* mm0 = FF R7 G7 B7 FF R6 G6 B6      */
          WriteCode [ out_RGB_reg + 8 * 2 ], mm6
          WriteCode [ out_RGB_reg + 8 * 3 ], mm0

          add ecx, 8
          jnz loop_beign

          mov pYUYV, eax
          mov pDstLine, edx
        }
      }

      //处理边界
      //DECODE_YUYV_Common_line(pDstLine,pYUYV,width-expand8_width);
    }
  }

  return 1;
}
