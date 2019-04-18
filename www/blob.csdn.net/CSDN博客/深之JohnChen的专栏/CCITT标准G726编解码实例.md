# CCITT标准G726编解码实例 - 深之JohnChen的专栏 - CSDN博客

2017年11月02日 23:11:57[byxdaz](https://me.csdn.net/byxdaz)阅读数：1864


//g726.h

```cpp
/*! Bitstream handler state */
typedef struct bitstream_state_s
{
	/*! The bit stream. */
	unsigned int bitstream;
	/*! The residual bits in bitstream. */
	int residue;
}bitstream_state_t;

typedef struct g726_state_s g726_state_t;
typedef short (*g726_decoder_func_t)(g726_state_t *s, unsigned char code);
typedef unsigned char (*g726_encoder_func_t)(g726_state_t *s, short amp);

/*!
* The following is the definition of the state structure
* used by the G.726 encoder and decoder to preserve their internal
* state between successive calls.  The meanings of the majority
* of the state structure fields are explained in detail in the
* CCITT Recommendation G.726.  The field names are essentially indentical
* to variable names in the bit level description of the coding algorithm
* included in this recommendation.
*/
struct g726_state_s
{
	/*! The bit rate */
	int rate;
	/*! The external coding, for tandem operation */
	//int ext_coding;
	/*! The number of bits per sample */
	int bits_per_sample;
	/*! One of the G.726_PACKING_xxx options */
	//int packing;

	/*! Locked or steady state step size multiplier. */
	int yl;
	/*! Unlocked or non-steady state step size multiplier. */
	short yu;
	/*! short term energy estimate. */
	short dms;
	/*! Long term energy estimate. */
	short dml;
	/*! Linear weighting coefficient of 'yl' and 'yu'. */
	short ap;

	/*! Coefficients of pole portion of prediction filter. */
	short a[2];
	/*! Coefficients of zero portion of prediction filter. */
	short b[6];
	/*! Signs of previous two samples of a partially reconstructed signal. */
	short pk[2];
	/*! Previous 6 samples of the quantized difference signal represented in
	an internal floating point format. */
	short dq[6];
	/*! Previous 2 samples of the quantized difference signal represented in an
	internal floating point format. */
	short sr[2];
	/*! Delayed tone detect */
	int td;

	/*! \brief The bit stream processing context. */
	bitstream_state_t bs;

	/*! \brief The current encoder function. */
	g726_encoder_func_t enc_func;
	/*! \brief The current decoder function. */
	g726_decoder_func_t dec_func;
};

/*
* Maps G.726_16 code word to reconstructed scale factor normalized log
* magnitude values.
*/
static const int g726_16_dqlntab[4] =
{
	116, 365, 365, 116
};

/* Maps G.726_16 code word to log of scale factor multiplier. */
static const int g726_16_witab[4] =
{
	-704, 14048, 14048, -704
};

/*
* Maps G.726_16 code words to a set of values whose long and short
* term averages are computed and then compared to give an indication
* how stationary (steady state) the signal is.
*/
static const int g726_16_fitab[4] =
{
	0x000, 0xE00, 0xE00, 0x000
};

/*
* Maps G.726_24 code word to reconstructed scale factor normalized log
* magnitude values.
*/
static const int g726_24_dqlntab[8] =
{
	-2048, 135, 273, 373, 373, 273, 135, -2048
};

/* Maps G.726_24 code word to log of scale factor multiplier. */
static const int g726_24_witab[8] =
{
	-128, 960, 4384, 18624, 18624, 4384, 960, -128
};

/*
* Maps G.726_24 code words to a set of values whose long and short
* term averages are computed and then compared to give an indication
* how stationary (steady state) the signal is.
*/
static const int g726_24_fitab[8] =
{
	0x000, 0x200, 0x400, 0xE00, 0xE00, 0x400, 0x200, 0x000
};

/*
* Maps G.726_32 code word to reconstructed scale factor normalized log
* magnitude values.
*/
static const int g726_32_dqlntab[16] =
{
	-2048,   4, 135, 213, 273, 323, 373,   425,
	425, 373, 323, 273, 213, 135,   4, -2048
};

/* Maps G.726_32 code word to log of scale factor multiplier. */
static const int g726_32_witab[16] =
{
	-384,   576,  1312,  2048,  3584,  6336, 11360, 35904,
	35904, 11360,  6336,  3584,  2048,  1312,   576,  -384
};

/*
* Maps G.726_32 code words to a set of values whose long and short
* term averages are computed and then compared to give an indication
* how stationary (steady state) the signal is.
*/
static const int g726_32_fitab[16] =
{
	0x000, 0x000, 0x000, 0x200, 0x200, 0x200, 0x600, 0xE00,
	0xE00, 0x600, 0x200, 0x200, 0x200, 0x000, 0x000, 0x000
};

/*
* Maps G.726_40 code word to ructeconstructed scale factor normalized log
* magnitude values.
*/
static const int g726_40_dqlntab[32] =
{
	-2048, -66, 28, 104, 169, 224, 274, 318,
	358, 395, 429, 459, 488, 514, 539, 566,
	566, 539, 514, 488, 459, 429, 395, 358,
	318, 274, 224, 169, 104, 28, -66, -2048
};

/* Maps G.726_40 code word to log of scale factor multiplier. */
static const int g726_40_witab[32] =
{
	448,   448,   768,  1248,  1280,  1312,  1856,  3200,
	4512,  5728,  7008,  8960, 11456, 14080, 16928, 22272,
	22272, 16928, 14080, 11456,  8960,  7008,  5728,  4512,
	3200,  1856,  1312,  1280,  1248,   768,   448,   448
};

/*
* Maps G.726_40 code words to a set of values whose long and short
* term averages are computed and then compared to give an indication
* how stationary (steady state) the signal is.
*/
static const int g726_40_fitab[32] =
{
	0x000, 0x000, 0x000, 0x000, 0x000, 0x200, 0x200, 0x200,
	0x200, 0x200, 0x400, 0x600, 0x800, 0xA00, 0xC00, 0xC00,
	0xC00, 0xC00, 0xA00, 0x800, 0x600, 0x400, 0x200, 0x200,
	0x200, 0x200, 0x200, 0x000, 0x000, 0x000, 0x000, 0x000
};

g726_state_t *g726_init(g726_state_t *s, int bit_rate);

int g726_decode(g726_state_t *s, short amp[], const unsigned char g726_data[], int g726_bytes);

int g726_encode(g726_state_t *s, unsigned char g726_data[], const short amp[], int len);
```

//g726.cpp

```cpp
/*
	Copyright (c) 2013-2016 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "g726.h"

static const int qtab_726_16[1] =
{
    261
};

static const int qtab_726_24[3] =
{
    8, 218, 331
};

static const int qtab_726_32[7] =
{
    -124, 80, 178, 246, 300, 349, 400
};

static const int qtab_726_40[15] =
{
    -122, -16,  68, 139, 198, 250, 298, 339,
     378, 413, 445, 475, 502, 528, 553
};

static __inline int top_bit(unsigned int bits)
{
#if defined(__i386__)  ||  defined(__x86_64__)
	int res;

	__asm__ (" xorl %[res],%[res];\n"
		" decl %[res];\n"
		" bsrl %[bits],%[res]\n"
		: [res] "=&r" (res)
		: [bits] "rm" (bits));
	return res;
#elif defined(__ppc__)  ||   defined(__powerpc__)
	int res;

	__asm__ ("cntlzw %[res],%[bits];\n"
		: [res] "=&r" (res)
		: [bits] "r" (bits));
	return 31 - res;
#elif defined(_M_IX86) // Visual Studio x86
	__asm
	{
		xor eax, eax
			dec eax
			bsr eax, bits
	}
#else
	int res;

	if (bits == 0)
		return -1;
	res = 0;
	if (bits & 0xFFFF0000)
	{
		bits &= 0xFFFF0000;
		res += 16;
	}
	if (bits & 0xFF00FF00)
	{
		bits &= 0xFF00FF00;
		res += 8;
	}
	if (bits & 0xF0F0F0F0)
	{
		bits &= 0xF0F0F0F0;
		res += 4;
	}
	if (bits & 0xCCCCCCCC)
	{
		bits &= 0xCCCCCCCC;
		res += 2;
	}
	if (bits & 0xAAAAAAAA)
	{
		bits &= 0xAAAAAAAA;
		res += 1;
	}
	return res;
#endif
}

static bitstream_state_t *bitstream_init(bitstream_state_t *s)
{
	if (s == NULL)
		return NULL;
	s->bitstream = 0;
	s->residue = 0;
	return s;
}

/*
 * Given a raw sample, 'd', of the difference signal and a
 * quantization step size scale factor, 'y', this routine returns the
 * ADPCM codeword to which that sample gets quantized.  The step
 * size scale factor division operation is done in the log base 2 domain
 * as a subtraction.
 */
static short quantize(int d,                  /* Raw difference signal sample */
                        int y,                  /* Step size multiplier */
                        const int table[],     /* quantization table */
                        int quantizer_states)   /* table size of short integers */
{
    short dqm;    /* Magnitude of 'd' */
    short exp;    /* Integer part of base 2 log of 'd' */
    short mant;   /* Fractional part of base 2 log */
    short dl;     /* Log of magnitude of 'd' */
    short dln;    /* Step size scale factor normalized log */
    int i;
    int size;

    /*
     * LOG
     *
     * Compute base 2 log of 'd', and store in 'dl'.
     */
    dqm = (short) abs(d);
    exp = (short) (top_bit(dqm >> 1) + 1);
    /* Fractional portion. */
    mant = ((dqm << 7) >> exp) & 0x7F;
    dl = (exp << 7) + mant;

    /*
     * SUBTB
     *
     * "Divide" by step size multiplier.
     */
    dln = dl - (short) (y >> 2);

    /*
     * QUAN
     *
     * Search for codword i for 'dln'.
     */
    size = (quantizer_states - 1) >> 1;
    for (i = 0;  i < size;  i++)
    {
        if (dln < table[i])
            break;
    }
    if (d < 0)
    {
        /* Take 1's complement of i */
        return (short) ((size << 1) + 1 - i);
    }
    if (i == 0  &&  (quantizer_states & 1))
    {
        /* Zero is only valid if there are an even number of states, so
           take the 1's complement if the code is zero. */
        return (short) quantizer_states;
    }
    return (short) i;
}
/*- End of function --------------------------------------------------------*/

/*
* returns the integer product of the 14-bit integer "an" and
* "floating point" representation (4-bit exponent, 6-bit mantessa) "srn".
*/
static short fmult(short an, short srn)
{
	short anmag;
	short anexp;
	short anmant;
	short wanexp;
	short wanmant;
	short retval;

	anmag = (an > 0)  ?  an  :  ((-an) & 0x1FFF);
	anexp = (short) (top_bit(anmag) - 5);
	anmant = (anmag == 0)  ?  32  :  (anexp >= 0)  ?  (anmag >> anexp)  :  (anmag << -anexp);
	wanexp = anexp + ((srn >> 6) & 0xF) - 13;

	wanmant = (anmant*(srn & 0x3F) + 0x30) >> 4;
	retval = (wanexp >= 0)  ?  ((wanmant << wanexp) & 0x7FFF)  :  (wanmant >> -wanexp);

	return (((an ^ srn) < 0)  ?  -retval  :  retval);
}

/*
* Compute the estimated signal from the 6-zero predictor.
*/
static __inline short predictor_zero(g726_state_t *s)
{
	int i;
	int sezi;

	sezi = fmult(s->b[0] >> 2, s->dq[0]);
	/* ACCUM */
	for (i = 1;  i < 6;  i++)
		sezi += fmult(s->b[i] >> 2, s->dq[i]);
	return (short) sezi;
}
/*- End of function --------------------------------------------------------*/

/*
* Computes the estimated signal from the 2-pole predictor.
*/
static __inline short predictor_pole(g726_state_t *s)
{
	return (fmult(s->a[1] >> 2, s->sr[1]) + fmult(s->a[0] >> 2, s->sr[0]));
}

/*
* Computes the quantization step size of the adaptive quantizer.
*/
static int step_size(g726_state_t *s)
{
	int y;
	int dif;
	int al;

	if (s->ap >= 256)
		return s->yu;
	y = s->yl >> 6;
	dif = s->yu - y;
	al = s->ap >> 2;
	if (dif > 0)
		y += (dif*al) >> 6;
	else if (dif < 0)
		y += (dif*al + 0x3F) >> 6;
	return y;
}
/*- End of function --------------------------------------------------------*/

/*
* Returns reconstructed difference signal 'dq' obtained from
* codeword 'i' and quantization step size scale factor 'y'.
* Multiplication is performed in log base 2 domain as addition.
*/
static short reconstruct(int sign,    /* 0 for non-negative value */
						 int dqln,    /* G.72x codeword */
						 int y)       /* Step size multiplier */
{
	short dql;    /* Log of 'dq' magnitude */
	short dex;    /* Integer part of log */
	short dqt;
	short dq;     /* Reconstructed difference signal sample */

	dql = (short) (dqln + (y >> 2));  /* ADDA */

	if (dql < 0)
		return ((sign)  ?  -0x8000  :  0);
	/* ANTILOG */
	dex = (dql >> 7) & 15;
	dqt = 128 + (dql & 127);
	dq = (dqt << 7) >> (14 - dex);
	return ((sign)  ?  (dq - 0x8000)  :  dq);
}
/*- End of function --------------------------------------------------------*/

/*
* updates the state variables for each output code
*/
static void update(g726_state_t *s,
				   int y,       /* quantizer step size */
				   int wi,      /* scale factor multiplier */
				   int fi,      /* for long/short term energies */
				   int dq,      /* quantized prediction difference */
				   int sr,      /* reconstructed signal */
				   int dqsez)   /* difference from 2-pole predictor */
{
	short mag;
	short exp;
	short a2p;        /* LIMC */
	short a1ul;       /* UPA1 */
	short pks1;       /* UPA2 */
	short fa1;
	short ylint;
	short dqthr;
	short ylfrac;
	short thr;
	short pk0;
	int i;
	int tr;

	a2p = 0;
	/* Needed in updating predictor poles */
	pk0 = (dqsez < 0)  ?  1  :  0;

	/* prediction difference magnitude */
	mag = (short) (dq & 0x7FFF);
	/* TRANS */
	ylint = (short) (s->yl >> 15);            /* exponent part of yl */
	ylfrac = (short) ((s->yl >> 10) & 0x1F);  /* fractional part of yl */
	/* Limit threshold to 31 << 10 */
	thr = (ylint > 9)  ?  (31 << 10)  :  ((32 + ylfrac) << ylint);
	dqthr = (thr + (thr >> 1)) >> 1;            /* dqthr = 0.75 * thr */
	if (!s->td)                                 /* signal supposed voice */
		tr = 0;
	else if (mag <= dqthr)                      /* supposed data, but small mag */
		tr = 0;                             /* treated as voice */
	else                                        /* signal is data (modem) */
		tr = 1;

	/*
	* Quantizer scale factor adaptation.
	*/

	/* FUNCTW & FILTD & DELAY */
	/* update non-steady state step size multiplier */
	s->yu = (short) (y + ((wi - y) >> 5));

	/* LIMB */
	if (s->yu < 544)
		s->yu = 544;
	else if (s->yu > 5120)
		s->yu = 5120;

	/* FILTE & DELAY */
	/* update steady state step size multiplier */
	s->yl += s->yu + ((-s->yl) >> 6);

	/*
	* Adaptive predictor coefficients.
	*/
	if (tr)
	{
		/* Reset the a's and b's for a modem signal */
		s->a[0] = 0;
		s->a[1] = 0;
		s->b[0] = 0;
		s->b[1] = 0;
		s->b[2] = 0;
		s->b[3] = 0;
		s->b[4] = 0;
		s->b[5] = 0;
	}
	else
	{
		/* Update the a's and b's */
		/* UPA2 */
		pks1 = pk0 ^ s->pk[0];

		/* Update predictor pole a[1] */
		a2p = s->a[1] - (s->a[1] >> 7);
		if (dqsez != 0)
		{
			fa1 = (pks1)  ?  s->a[0]  :  -s->a[0];
			/* a2p = function of fa1 */
			if (fa1 < -8191)
				a2p -= 0x100;
			else if (fa1 > 8191)
				a2p += 0xFF;
			else
				a2p += fa1 >> 5;

			if (pk0 ^ s->pk[1])
			{
				/* LIMC */
				if (a2p <= -12160)
					a2p = -12288;
				else if (a2p >= 12416)
					a2p = 12288;
				else
					a2p -= 0x80;
			}
			else if (a2p <= -12416)
				a2p = -12288;
			else if (a2p >= 12160)
				a2p = 12288;
			else
				a2p += 0x80;
		}

		/* TRIGB & DELAY */
		s->a[1] = a2p;

		/* UPA1 */
		/* Update predictor pole a[0] */
		s->a[0] -= s->a[0] >> 8;
		if (dqsez != 0)
		{
			if (pks1 == 0)
				s->a[0] += 192;
			else
				s->a[0] -= 192;
		}
		/* LIMD */
		a1ul = 15360 - a2p;
		if (s->a[0] < -a1ul)
			s->a[0] = -a1ul;
		else if (s->a[0] > a1ul)
			s->a[0] = a1ul;

		/* UPB : update predictor zeros b[6] */
		for (i = 0;  i < 6;  i++)
		{
			/* Distinguish 40Kbps mode from the others */
			s->b[i] -= s->b[i] >> ((s->bits_per_sample == 5)  ?  9  :  8);
			if (dq & 0x7FFF)
			{
				/* XOR */
				if ((dq ^ s->dq[i]) >= 0)
					s->b[i] += 128;
				else
					s->b[i] -= 128;
			}
		}
	}

	for (i = 5;  i > 0;  i--)
		s->dq[i] = s->dq[i - 1];
	/* FLOAT A : convert dq[0] to 4-bit exp, 6-bit mantissa f.p. */
	if (mag == 0)
	{
		s->dq[0] = (dq >= 0)  ?  0x20  :  0xFC20;
	}
	else
	{
		exp = (short) (top_bit(mag) + 1);
		s->dq[0] = (dq >= 0)
			?  ((exp << 6) + ((mag << 6) >> exp))
			:  ((exp << 6) + ((mag << 6) >> exp) - 0x400);
	}

	s->sr[1] = s->sr[0];
	/* FLOAT B : convert sr to 4-bit exp., 6-bit mantissa f.p. */
	if (sr == 0)
	{
		s->sr[0] = 0x20;
	}
	else if (sr > 0)
	{
		exp = (short) (top_bit(sr) + 1);
		s->sr[0] = (short) ((exp << 6) + ((sr << 6) >> exp));
	}
	else if (sr > -32768)
	{
		mag = (short) -sr;
		exp = (short) (top_bit(mag) + 1);
		s->sr[0] =  (exp << 6) + ((mag << 6) >> exp) - 0x400;
	}
	else
	{
		s->sr[0] = (short) 0xFC20;
	}

	/* DELAY A */
	s->pk[1] = s->pk[0];
	s->pk[0] = pk0;

	/* TONE */
	if (tr)                 /* this sample has been treated as data */
		s->td = 0;      /* next one will be treated as voice */
	else if (a2p < -11776)  /* small sample-to-sample correlation */
		s->td = 1;       /* signal may be data */
	else                    /* signal is voice */
		s->td = 0;

	/* Adaptation speed control. */
	/* FILTA */
	s->dms += ((short) fi - s->dms) >> 5;
	/* FILTB */
	s->dml += (((short) (fi << 2) - s->dml) >> 7);

	if (tr)
		s->ap = 256;
	else if (y < 1536)                      /* SUBTC */
		s->ap += (0x200 - s->ap) >> 4;
	else if (s->td)
		s->ap += (0x200 - s->ap) >> 4;
	else if (abs((s->dms << 2) - s->dml) >= (s->dml >> 3))
		s->ap += (0x200 - s->ap) >> 4;
	else
		s->ap += (-s->ap) >> 4;
}

/*
* Decodes a 2-bit CCITT G.726_16 ADPCM code and returns
* the resulting 16-bit linear PCM, A-law or u-law sample value.
*/
static short g726_16_decoder(g726_state_t *s, unsigned char code)
{
	short sezi;
	short sei;
	short se;
	short sr;
	short dq;
	short dqsez;
	int y;

	/* Mask to get proper bits */
	code &= 0x03;
	sezi = predictor_zero(s);
	sei = sezi + predictor_pole(s);

	y = step_size(s);
	dq = reconstruct(code & 2, g726_16_dqlntab[code], y);

	/* Reconstruct the signal */
	se = sei >> 1;
	sr = (dq < 0)  ?  (se - (dq & 0x3FFF))  :  (se + dq);

	/* Pole prediction difference */
	dqsez = sr + (sezi >> 1) - se;

	update(s, y, g726_16_witab[code], g726_16_fitab[code], dq, sr, dqsez);

	return (sr << 2);
}
/*- End of function --------------------------------------------------------*/

/*
 * Encodes a linear PCM, A-law or u-law input sample and returns its 3-bit code.
 */
static unsigned char g726_16_encoder(g726_state_t *s, short amp)
{
    int y;
    short sei;
    short sezi;
    short se;
    short d;
    short sr;
    short dqsez;
    short dq;
    short i;
    
    sezi = predictor_zero(s);
    sei = sezi + predictor_pole(s);
    se = sei >> 1;
    d = amp - se;

    /* Quantize prediction difference */
    y = step_size(s);
    i = quantize(d, y, qtab_726_16, 4);
    dq = reconstruct(i & 2, g726_16_dqlntab[i], y);

    /* Reconstruct the signal */
    sr = (dq < 0)  ?  (se - (dq & 0x3FFF))  :  (se + dq);

    /* Pole prediction difference */
    dqsez = sr + (sezi >> 1) - se;
    
    update(s, y, g726_16_witab[i], g726_16_fitab[i], dq, sr, dqsez);
    return (unsigned char) i;
}

/*
* Decodes a 3-bit CCITT G.726_24 ADPCM code and returns
* the resulting 16-bit linear PCM, A-law or u-law sample value.
*/
static short g726_24_decoder(g726_state_t *s, unsigned char code)
{
	short sezi;
	short sei;
	short se;
	short sr;
	short dq;
	short dqsez;
	int y;

	/* Mask to get proper bits */
	code &= 0x07;
	sezi = predictor_zero(s);
	sei = sezi + predictor_pole(s);

	y = step_size(s);
	dq = reconstruct(code & 4, g726_24_dqlntab[code], y);

	/* Reconstruct the signal */
	se = sei >> 1;
	sr = (dq < 0)  ?  (se - (dq & 0x3FFF))  :  (se + dq);

	/* Pole prediction difference */
	dqsez = sr + (sezi >> 1) - se;

	update(s, y, g726_24_witab[code], g726_24_fitab[code], dq, sr, dqsez);

	return (sr << 2);
}
/*- End of function --------------------------------------------------------*/

/*
 * Encodes a linear PCM, A-law or u-law input sample and returns its 3-bit code.
 */
static unsigned char g726_24_encoder(g726_state_t *s, short amp)
{
    short sei;
    short sezi;
    short se;
    short d;
    short sr;
    short dqsez;
    short dq;
    short i;
    int y;
    
    sezi = predictor_zero(s);
    sei = sezi + predictor_pole(s);
    se = sei >> 1;
    d = amp - se;

    /* Quantize prediction difference */
    y = step_size(s);
    i = quantize(d, y, qtab_726_24, 7);
    dq = reconstruct(i & 4, g726_24_dqlntab[i], y);

    /* Reconstruct the signal */
    sr = (dq < 0)  ?  (se - (dq & 0x3FFF))  :  (se + dq);

    /* Pole prediction difference */
    dqsez = sr + (sezi >> 1) - se;
    
    update(s, y, g726_24_witab[i], g726_24_fitab[i], dq, sr, dqsez);
    return (unsigned char) i;
}

/*
* Decodes a 4-bit CCITT G.726_32 ADPCM code and returns
* the resulting 16-bit linear PCM, A-law or u-law sample value.
*/
static short g726_32_decoder(g726_state_t *s, unsigned char code)
{
	short sezi;
	short sei;
	short se;
	short sr;
	short dq;
	short dqsez;
	int y;

	/* Mask to get proper bits */
	code &= 0x0F;
	sezi = predictor_zero(s);
	sei = sezi + predictor_pole(s);

	y = step_size(s);
	dq = reconstruct(code & 8, g726_32_dqlntab[code], y);

	/* Reconstruct the signal */
	se = sei >> 1;
	sr = (dq < 0)  ?  (se - (dq & 0x3FFF))  :  (se + dq);

	/* Pole prediction difference */
	dqsez = sr + (sezi >> 1) - se;

	update(s, y, g726_32_witab[code], g726_32_fitab[code], dq, sr, dqsez);

	return (sr << 2);
}
/*- End of function --------------------------------------------------------*/

/*
 * Encodes a linear input sample and returns its 4-bit code.
 */
static unsigned char g726_32_encoder(g726_state_t *s, short amp)
{
    short sei;
    short sezi;
    short se;
    short d;
    short sr;
    short dqsez;
    short dq;
    short i;
    int y;
    
    sezi = predictor_zero(s);
    sei = sezi + predictor_pole(s);
    se = sei >> 1;
    d = amp - se;

    /* Quantize the prediction difference */
    y = step_size(s);
    i = quantize(d, y, qtab_726_32, 15);
    dq = reconstruct(i & 8, g726_32_dqlntab[i], y);

    /* Reconstruct the signal */
    sr = (dq < 0)  ?  (se - (dq & 0x3FFF))  :  (se + dq);

    /* Pole prediction difference */
    dqsez = sr + (sezi >> 1) - se;

    update(s, y, g726_32_witab[i], g726_32_fitab[i], dq, sr, dqsez);
    return (unsigned char) i;
}

/*
* Decodes a 5-bit CCITT G.726 40Kbps code and returns
* the resulting 16-bit linear PCM, A-law or u-law sample value.
*/
static short g726_40_decoder(g726_state_t *s, unsigned char code)
{
	short sezi;
	short sei;
	short se;
	short sr;
	short dq;
	short dqsez;
	int y;

	/* Mask to get proper bits */
	code &= 0x1F;
	sezi = predictor_zero(s);
	sei = sezi + predictor_pole(s);

	y = step_size(s);
	dq = reconstruct(code & 0x10, g726_40_dqlntab[code], y);

	/* Reconstruct the signal */
	se = sei >> 1;
	sr = (dq < 0)  ?  (se - (dq & 0x7FFF))  :  (se + dq);

	/* Pole prediction difference */
	dqsez = sr + (sezi >> 1) - se;

	update(s, y, g726_40_witab[code], g726_40_fitab[code], dq, sr, dqsez);

	return (sr << 2);
}
/*- End of function --------------------------------------------------------*/

/*
 * Encodes a 16-bit linear PCM, A-law or u-law input sample and retuens
 * the resulting 5-bit CCITT G.726 40Kbps code.
 */
static unsigned char g726_40_encoder(g726_state_t *s, short amp)
{
    short sei;
    short sezi;
    short se;
    short d;
    short sr;
    short dqsez;
    short dq;
    short i;
    int y;
    
    sezi = predictor_zero(s);
    sei = sezi + predictor_pole(s);
    se = sei >> 1;
    d = amp - se;

    /* Quantize prediction difference */
    y = step_size(s);
    i = quantize(d, y, qtab_726_40, 31);
    dq = reconstruct(i & 0x10, g726_40_dqlntab[i], y);

    /* Reconstruct the signal */
    sr = (dq < 0)  ?  (se - (dq & 0x7FFF))  :  (se + dq);

    /* Pole prediction difference */
    dqsez = sr + (sezi >> 1) - se;

    update(s, y, g726_40_witab[i], g726_40_fitab[i], dq, sr, dqsez);
    return (unsigned char) i;
}

g726_state_t *g726_init(g726_state_t *s, int bit_rate)
{
	int i;

	if (bit_rate != 16000  &&  bit_rate != 24000  &&  bit_rate != 32000  &&  bit_rate != 40000)
		return NULL;

	s->yl = 34816;
	s->yu = 544;
	s->dms = 0;
	s->dml = 0;
	s->ap = 0;
	s->rate = bit_rate;

	for (i = 0; i < 2; i++)
	{
		s->a[i] = 0;
		s->pk[i] = 0;
		s->sr[i] = 32;
	}
	for (i = 0; i < 6; i++)
	{
		s->b[i] = 0;
		s->dq[i] = 32;
	}
	s->td = 0;
	switch (bit_rate)
	{
	case 16000:
		s->enc_func = g726_16_encoder;
		s->dec_func = g726_16_decoder;
		s->bits_per_sample = 2;
		break;
	case 24000:
		s->enc_func = g726_24_encoder;
		s->dec_func = g726_24_decoder;
		s->bits_per_sample = 3;
		break;
	case 32000:
	default:
		s->enc_func = g726_32_encoder;
		s->dec_func = g726_32_decoder;
		s->bits_per_sample = 4;
		break;
	case 40000:
		s->enc_func = g726_40_encoder;
		s->dec_func = g726_40_decoder;
		s->bits_per_sample = 5;
		break;
	}
	bitstream_init(&s->bs);
	return s;
}

int g726_decode(g726_state_t *s,
				short amp[],
				const unsigned char g726_data[],
				int g726_bytes)
{
	int i;
	int samples;
	unsigned char code;
	int sl;

	for (samples = i = 0;  ;  )
	{
		if (s->bs.residue < s->bits_per_sample)
		{
			if (i >= g726_bytes)
				break;
			s->bs.bitstream = (s->bs.bitstream << 8) | g726_data[i++];
			s->bs.residue += 8;
		}
		code = (unsigned char) ((s->bs.bitstream >> (s->bs.residue - s->bits_per_sample)) & ((1 << s->bits_per_sample) - 1));

		s->bs.residue -= s->bits_per_sample;

		sl = s->dec_func(s, code);

		amp[samples++] = (short) sl;
	}
	return samples;
}

int g726_encode(g726_state_t *s,
                unsigned char g726_data[],
                const short amp[],
                int len)
{
    int i;
    int g726_bytes;
    short sl;
    unsigned char code;

    for (g726_bytes = i = 0;  i < len;  i++)
    {
		sl = amp[i] >> 2;

        code = s->enc_func(s, sl);

		s->bs.bitstream = (s->bs.bitstream << s->bits_per_sample) | code;
		s->bs.residue += s->bits_per_sample;
		if (s->bs.residue >= 8)
		{
			g726_data[g726_bytes++] = (unsigned char) ((s->bs.bitstream >> (s->bs.residue - 8)) & 0xFF);
			s->bs.residue -= 8;
		}
		
    }
	
    return g726_bytes;
}
```

//调用实例

pcm to ccitt g726

//pcm数据采样率：，位宽：bit，通道：单通道

//一般-100ms打包一帧数据，如何ms打包一帧，pcm数据大小为字节

//pcm大小=
（采样率*位宽*通道数/1000毫秒* 20毫秒）/ 8

intnPcmSize = (8000*16*1/1000 * 20) / 8;

g726_state_t*pSate726 = (g726_state_t*)malloc(sizeof(g726_state_t));

intnG726Size = 0;

intnG726SizePerFrame = 20;            
//每帧pcm大小

intnRateZipType = 3;                  
//码流类型，分别用、、、表示kpbs、kpbs、kbps、kbps

intnRateZipValue = 2;

switch(nRateZipType)

    {

case0:

nRateZipValue= 2;                 
//16kpbs

break;

case1:

nRateZipValue= 3;                 
//24kpbs

break;

case2:

nRateZipValue= 4;                 
//32kbps

break;

case3:

nRateZipValue= 5;                 
//40kbps

break;

    }

nG726Size= 
nRateZipValue * nG726SizePerFrame;

pSate726= 
g726_init(pSate726,nRateZipValue*8000);

CStringfilePath =
"";

CStringnewlFilePath =
"";

charszFilter[] = {"PcmFiles (*.pcm)|*.pcm||"};

CFileDialogdlg(TRUE,NULL,NULL,OFN_HIDEREADONLY
 | OFN_OVERWRITEPROMPT,szFilter,NULL);

if(dlg.DoModal()==
IDOK)

    {

filePath= 
dlg.GetPathName();

newlFilePath= 
filePath;

newlFilePath.Replace(".pcm",".g726_ccitt");

FILE* 
fpSrc = fopen(filePath.GetBuffer(filePath.GetLength()),"rb");

FILE* 
fpDst = fopen(newlFilePath.GetBuffer(newlFilePath.GetLength()),"wb+");

charszData[320] = {0};

unsignedcharszOutData[200]= {0};

int
nDataLen =nPcmSize;

int
nOutDataLen= nG726Size;

int
nReadedSize= 0;

int
ret = 0;

if(fpSrc !=
NULL)

        {

while(TRUE)

            {

nReadedSize = 
fread(szData,sizeof(char),nDataLen,fpSrc);

if(nReadedSize<
nDataLen)

                {

break;

                }

int
iRet = g726_encode(pSate726,szOutData, (short*)szData,
nReadedSize/2);

nOutDataLen = 
iRet;

if (nOutDataLen> 0)

                {

//写入数据

int
iiiii = fwrite(szOutData,1,
nOutDataLen, fpDst);

                }

            }

fclose(fpSrc);

fclose(fpDst);

        }

    }

if(pSate726 !=
NULL)

    {

free(pSate726);

pSate726= 
NULL;

    }

//ccitt g726 to pcm

//pcm数据采样率：，位宽：bit，通道：单通道

//一般-100ms打包一帧数据，如何ms打包一帧，pcm数据大小为字节

//pcm大小=
（采样率*位宽*通道数/1000毫秒* 20毫秒）/ 8

intnPcmSize = (8000*16*1/1000 * 20) / 8;

g726_state_t*pSate726 = (g726_state_t*)malloc(sizeof(g726_state_t));

intnG726Size = 0;

intnG726SizePerFrame = 20;            
//每帧pcm大小

intnRateZipType = 3;                  
//码流类型，分别用、、、表示kpbs、kpbs、kbps、kbps

intnRateZipValue = 2;

switch(nRateZipType)

    {

case0:

nRateZipValue= 2;                 
//16kpbs

break;

case1:

nRateZipValue= 3;                 
//24kpbs

break;

case2:

nRateZipValue= 4;                 
//32kbps

break;

case3:

nRateZipValue= 5;                 
//40kbps

break;

    }

nG726Size= 
nRateZipValue * nG726SizePerFrame;

pSate726= 
g726_init(pSate726,nRateZipValue*8000);

CStringfilePath =
"";

CStringnewlFilePath =
"";

charszFilter[] = {"g726_ccittFiles (*.g726_ccitt)|*.g726_ccitt||"};

CFileDialogdlg(TRUE,NULL,NULL,OFN_HIDEREADONLY
 | OFN_OVERWRITEPROMPT,szFilter,NULL);

if(dlg.DoModal()==
IDOK)

    {

filePath= 
dlg.GetPathName();

newlFilePath= 
filePath;

newlFilePath.Replace(".g726_ccitt","_g726_ccitt.pcm");

FILE* 
fpSrc = fopen(filePath.GetBuffer(filePath.GetLength()),"rb");

FILE* 
fpDst = fopen(newlFilePath.GetBuffer(newlFilePath.GetLength()),"wb+");

unsignedcharszData[200]= {0};

shortszOutData[640] = {0};

int
nDataLen =nG726Size;

int
nOutDataLen= nPcmSize;

int
nReadedSize= 0;

int
ret = 0;

if(fpSrc !=
NULL)

        {

while(TRUE)

            {

nReadedSize = 
fread(szData,sizeof(unsignedchar),nDataLen,fpSrc);

if(nReadedSize<
nDataLen)

                {

break;

                }

int
iRet = g726_decode(pSate726,szOutData,szData,nReadedSize);

nOutDataLen = 
iRet*2;

if (nOutDataLen> 0)

                {

//写入数据

int
iiiii = fwrite(szOutData,sizeof(short),
iRet, fpDst );

                }

            }

fclose(fpSrc);

fclose(fpDst);

        }

    }

if(pSate726 !=
NULL)

    {

free(pSate726);

pSate726= 
NULL;

    }
[代码下载](http://download.csdn.net/download/byxdaz/10051164)

