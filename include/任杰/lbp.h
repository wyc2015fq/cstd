#ifndef _LBP_H_
#define _LBP_H_

#include <assert.h>
#include <stdlib.h>
#include <memory.h>

typedef unsigned char uchar;


/*
 * 较早版本的LBP和直方图统计
 * 宏LBP8aBIN
 * 宏HISTO8BIN
 * 宏HISTO8BIN_NORM
 * 宏HISTO32BIN
 * 常规LBP
*/

/* LBP8BIN只处理最高3位, 仅适用于8bin情况 */
#define LBP8BIN(src, h, w, rst) do {                \
	double *src_ext, a, b, c, d;                     \
	register int len0, len1, i, j;                   \
	uchar ch;                                        \
	len0 = (w) + 2; len1 = ((h) + 1) * len0;         \
	src_ext = new double[len1];                      \
	memset(src_ext, 0, len1 * sizeof(double));       \
	for (i = 0; i < (h); ++i) {                      \
		for (j = 0; j < (w); ++j) {                   \
			*(src_ext + (i + 1) * len0 + (j + 1)) =    \
			*((src) + i * (w) + j);                    \
		}                                             \
	}                                                \
	for (i = 0; i < (h); ++i) {                      \
		for (j = 0; j < (w); ++j) {                   \
			a = *(src_ext + i * len0 + (j    ));       \
			b = *(src_ext + i * len0 + (j + 1));       \
			c = *(src_ext + i * len0 + (j + 2));       \
			d = *(src_ext + (i + 1) * len0 + (j + 1)); \
			ch  = (a >= d); ch <<= 1;                  \
			ch |= (b >= d); ch <<= 1;                  \
			ch |= (c >= d);                            \
			*((rst) + i * (w) + j) = ch;               \
		}                                             \
	}                                                \
	delete []src_ext; src_ext = 0;                   \
} while (0);

/* LBP8BIN分块直方图统计, 只能与上面的LBP8BIN的结果配合使用, rst为float型 */
#define HISTO8BIN(src, h, w, sh, sw, rst) do {                     \
	register int i, j, m, n, x, y;                                  \
	x = (w) / (sw); y = (h) / (sh);                                 \
	memset((rst), 0, ((x * y) << 3) * sizeof(float));               \
	for (i = 0; i < y; ++i) {                                       \
		for (j = 0; j < x; ++j) {                                    \
			for (m = 0; m < (sh); ++m) {                              \
				for (n = 0; n < (sw); ++n) {                           \
					*((rst) +                                           \
					   ((i * x + j) << 3) +                             \
						*((src) + (i * (sh) + m) * (w) + (j * (sw) + n)) \
					 )                                                  \
					+= 1.0f;                                            \
				}                                                      \
			}                                                         \
		}                                                            \
	}                                                               \
} while (0);

inline void LBP(const double *src, int h, int w, int ls, uchar *rst, int ld)
{
	double c;
	const int len0 = (w + 2);
	const int len1 = len0 * (h + 2);
	int i, j;
	double *src_ext;
	uchar ch;
	src_ext = new double[len1];
	memset(src_ext, 0, len1 * sizeof(double));
	for (i = 0; i < h; ++i) {
		for (j = 0; j < w; ++j) {
			*(src_ext + (i + 1) * len0 + (j + 1)) = *(src + i * ls + j);
		}
	}
	for (i = 0; i < h; ++i) {
		for (j = 0; j < w; ++j) {
			c = *(src_ext + (i + 1) * len0 + (j + 1));
			ch  = (*(src_ext + (i    ) * len0 + (j    )) >= c); ch <<= 1;
			ch |= (*(src_ext + (i    ) * len0 + (j + 1)) >= c); ch <<= 1;
			ch |= (*(src_ext + (i    ) * len0 + (j + 2)) >= c); ch <<= 1;
			ch |= (*(src_ext + (i + 1) * len0 + (j + 2)) >= c); ch <<= 1;
			ch |= (*(src_ext + (i + 2) * len0 + (j + 2)) >= c); ch <<= 1;
			ch |= (*(src_ext + (i + 2) * len0 + (j + 1)) >= c); ch <<= 1;
			ch |= (*(src_ext + (i + 2) * len0 + (j    )) >= c); ch <<= 1;
			ch |= (*(src_ext + (i + 1) * len0 + (j    )) >= c);
			*(rst + i * ld + j) = ch;
		}
	}
	delete []src_ext; src_ext = 0;
}

#define HISTO32BIN(src, h, w, sh, sw, rst) do {                           \
	register int i, j, m, n, x, y;                                         \
	x = (w) / (sw); y = (h) / (sh);                                        \
	memset((rst), 0, ((x * y) << 5) * sizeof(float));                      \
	for (i = 0; i < y; ++i) {                                              \
		for (j = 0; j < x; ++j) {                                           \
			for (m = 0; m < (sh); ++m) {                                     \
				for (n = 0; n < (sw); ++n) {                                  \
					*((rst) +                                                  \
					   ((i * x + j) << 5) +                                    \
						(*((src) + (i * (sh) + m) * (w) + (j * (sw) + n)) >> 3) \
					 )                                                         \
					+= 1.0f;                                                   \
				}                                                             \
			}                                                                \
		}                                                                   \
	}                                                                      \
} while (0);

#define HISTO8BIN_NORM(src, h, w, sh, sw, rst) do {                       \
	register int i, j, m, n, x, y;                                         \
	x = (w) / (sw); y = (h) / (sh);                                        \
	memset((rst), 0, ((x * y) << 3) * sizeof(float));                      \
	for (i = 0; i < y; ++i) {                                              \
		for (j = 0; j < x; ++j) {                                           \
			for (m = 0; m < (sh); ++m) {                                     \
				for (n = 0; n < (sw); ++n) {                                  \
					*((rst) +                                                  \
					   ((i * x + j) << 3) +                                    \
						(*((src) + (i * (sh) + m) * (w) + (j * (sw) + n)) >> 5) \
					 )                                                         \
					+= 1.0f;                                                   \
				}                                                             \
			}                                                                \
		}                                                                   \
	}                                                                      \
} while (0);


/*
 * Function: 通用LBP算子, 适用于(2 ^ n)Bin的直方图统计方法
 * Date: 09.06.19
 * Author: RJsloth
*/

// LBP
template<typename _II, typename _IO> inline
void Lbp(const _II *Img, int h, int w, int StepI, _IO *Rst, int StepO)
{
	// Input and Output pointers can not be null
	assert(NULL != Img && NULL != Rst && h > 0 && w > 0 && StepI >= w && StepO > 0);

	const int StepExt = w + 2;
	const int ImgArea = (h + 2) * StepExt;

	int i, j;

	_II *ImgExt = new _II[ImgArea];

	if (NULL == ImgExt)
	{
		exit(1);
	}

	// Fill extended image
	memset(ImgExt, 0, ImgArea * sizeof(_II));

	for (i = 0; i < h; ++i)
	{
		for (j = 0; j < w; ++j)
		{
			*(ImgExt + (i + 1) * StepExt + (j + 1)) = *(Img + i * StepI + j);
		}
	}

	_IO Output;

	// Lbp
	for (i = 0; i < h; ++i)
	{
		for (j = 0; j < w; ++j)
		{
			const _II *pCtr = ImgExt + (i + 1) * StepExt + (j + 1);
			const _II Center = *(pCtr);

			Output  = *(pCtr - StepExt - 1) >= Center; Output <<= 1;
			Output |= *(pCtr - StepExt    ) >= Center; Output <<= 1;
			Output |= *(pCtr - StepExt + 1) >= Center; Output <<= 1;
			Output |= *(pCtr           + 1) >= Center; Output <<= 1;
			Output |= *(pCtr + StepExt + 1) >= Center; Output <<= 1;
			Output |= *(pCtr + StepExt    ) >= Center; Output <<= 1;
			Output |= *(pCtr + StepExt - 1) >= Center; Output <<= 1;
			Output |= *(pCtr           - 1) >= Center;

			*(Rst + i * StepO + j) = Output;
		}
	}

	delete []ImgExt; ImgExt = NULL;
}

// Histogram X bin(Attention: Bin = 2 ^ (int)n)
template<typename _II, typename _IO> inline
void Histogram(const _II *Img, int h, int w, int Step, _IO *Hist, int SubH, int SubW, int Bin)
{
	assert(NULL != Img && NULL != Hist && h > 0 && w > 0 && Step > 0 && SubH > 0 && SubW > 0 && Bin > 0 && Bin < 257 && SubH <= h && SubW <= w);

	const int RowBlock = h / SubH;
	const int ColBlock = w / SubW;

	int i, j;

	// Bin = 2 ^ (8 - Offset)
	int Offset = 7;
	j = 2;

	while (j != Bin)
	{
		j *= 2;
		--Offset;
	}

	memset(Hist, 0, RowBlock * ColBlock * Bin * sizeof(_IO));

	// Histogram
	for (i = 0; i < h; ++i)
	{
		for (j = 0; j < w; ++j)
		{
			const int Loc = *(Img + i * Step + j) >> Offset;
			*(Hist + Bin * ((i / SubH) * ColBlock + (j / SubW)) + Loc) += 1;
		}
	}
}

#endif