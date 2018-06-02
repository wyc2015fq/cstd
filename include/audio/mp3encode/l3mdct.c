/* mdct变换 */
#include <math.h>

#include "types.h"
#include "layer3.h"
#include "l3mdct.h"
#include "l3subband.h"


/* 下面是表B.9的数据：是去混叠的系数*/
static double c[8] = { -0.6,-0.535,-0.33,-0.185,-0.095,-0.041,-0.0142, -0.0037 };
static double ca[8];
static double cs[8];

static double win[4][36];
static double cos_s[6][12];
static double cos_l[18][36];


void L3_mdct_initialise()//////////////////////////建立MDCT的初始窗函数和各项函数初始化
{
    int i,m,k,N;
    double sq;

/* 初始化去混叠的蝶形图*/
    for (i=0;i<8;i++)
    {
        sq = sqrt(1.0 + c[i]*c[i]);
        ca[i] = c[i] / sq;
        cs[i] = 1.0  / sq;
    }
/*下面是四种窗函数*/

      /* 类型 0 ,正常的长块*/
    for(i=0; i < 36; i++ ) win[0][i] = sin( PI/36 * (i + 0.5) );
    /* 类型 1，start块，长块和短块的过渡块*/
    for(i=0; i < 18; i++ ) win[1][i] = sin( PI/36 * (i + 0.5) );
    for(i=18; i < 24; i++ ) win[1][i] = 1.0;
    for(i=24; i < 30; i++ ) win[1][i] = sin( PI/12 * ( i + 0.5 - 18) );
    for(i=30; i < 36; i++ ) win[1][i] = 0.0;
    /* type 3,stop块，是短块过渡到长块的过渡块*/
    for(i= 0; i < 6; i++ ) win[3][i] = 0.0;
    for(i= 6; i < 12; i++ ) win[3][i] = sin( PI/12 * (i + 0.5 - 6) );
    for(i= 12; i < 18; i++ ) win[3][i] = 1.0;
    for(i= 18; i < 36; i++ ) win[3][i] = sin( PI/36 * (i + 0.5) );
    /* type 2,短块*/
    for(i=0; i < 12; i++ ) win[2][i] = sin( PI/12 * (i + 0.5) );
    for(i=12; i < 36; i++ ) win[2][i] = 0.0;

    N = 12;
    for (m = 0; m < N / 2; m++ )
      for (k = 0; k < N; k++ )
        cos_s[m][k] = cos( (PI /(2 * N)) * (2 * k + 1 + N / 2) *
                     (2 * m + 1) ) / (N / 4);

    N = 36;
    for (m = 0; m < N / 2; m++ )
      for (k = 0; k < N; k++ )
        cos_l[m][k] = cos( (PI / (2 * N)) * (2 * k + 1 + N / 2) *
                     (2 * m + 1) ) / (N / 4);

}


static void mdct( double *in, double *out, int block_type )
/*-------------------------------------------------------------------*/
/*   Function: 计算MDCT                               */
/*   在长区块（块的类型为0、1、3时）的情况下，在时域中有36个系数，在 */
/*频域中有18个系数。而对于短区块（块的类型为2时），有三种变换的情形。*/
/*这样在时域中就有12个系数而在频域中就有6个系数。在这种情况下，运算的*/
/*结果并排存储在向量out[]中                                          */
/*-------------------------------------------------------------------*/
{
    int l,k,m,N;
    double sum;

    if(block_type==2)/////////////////////////////////////短块MDCT处理
    {
        N=12;
        for(l=0;l<3;l++)
            for(m=0;m<N/2;m++)
            {
                sum=0.0;
                for(k=0;k<N;k++)
                sum += win[block_type][k] * in[k + 6 * l + 6] * cos_s[m][k];
                out[ 3 * m + l] = sum;
            }
    }
    else/////////////////////////////////////////////////长块MDCT处理
    {
        N=36;
        for(m=0;m<N/2;m++)
        {
            sum=0.0;
            for(k=0;k<N;k++)
                sum += win[block_type][k] * in[k] * cos_l[m][k];
            out[m] = sum;
        }
    }
}


void L3_mdct_sub(double sb_sample[2][3][18][SBLIMIT], 
                 double (*mdct_freq)[2][576], 
                 L3_side_info_t *side_info)
{

    double (*mdct_enc)[2][32][18] = (double (*)[2][32][18]) mdct_freq;

    int      ch,gr,band,k,j;
    int	     block_type;
    gr_info *cod_info;
    double   mdct_in[36];
    double   bu,bd;
    
    for(gr=0;gr<config.mpeg.mode_gr;gr++)
        for(ch=0;ch<config.wave.channels;ch++)
        {
	    cod_info = (gr_info*) &(side_info->gr[gr].ch[ch]) ;
	    block_type = cod_info->block_type;
	    
/* 对解析滤波器的反转进行补偿*/
		for(band=0;band<32;band++)
		for(k=0;k<18;k++)
		    if((band&1) && (k&1))
			sb_sample[ch][gr+1][k][band] *= -1.0;
	    
/* 对18点的前子带采样和当前子带的18点采样进行imdct */
	   for(band=0;band<32;band++)
	    {
		for(k=0;k<18;k++)
		{
		    mdct_in[k]    = sb_sample[ch][ gr ][k][band];
		    mdct_in[k+18] = sb_sample[ch][gr+1][k][band];
		}
		if(cod_info->mixed_block_flag && (band<2)) block_type=0;
		
		mdct(mdct_in,&mdct_enc[gr][ch][band][0],block_type);
	    }
	    
/* 对长区块进行去混叠的蝶形图运算*/
	    if(block_type!=2)
		for(band=0;band<31;band++)
		    for(k=0;k<8;k++)
		    {
			bu = mdct_enc[gr][ch][band][17-k] * cs[k] + mdct_enc[gr][ch][band+1][k] * ca[k];
			bd = mdct_enc[gr][ch][band+1][k] * cs[k] - mdct_enc[gr][ch][band][17-k] * ca[k];
			mdct_enc[gr][ch][band][17-k] = bu;
			mdct_enc[gr][ch][band+1][k]  = bd;
		    }
	}
    
/* 保存最新的微子带采样值以备下次MDCT变换调用*/
    for(ch=0;ch<config.wave.channels;ch++)
	for(j=0;j<18;j++)
	    for(band=0;band<32;band++)
		sb_sample[ch][0][j][band] = sb_sample[ch][config.mpeg.mode_gr][j][band];
}

