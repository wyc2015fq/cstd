#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

#include ".\dat\resource.h"
#include "lbp.h"
#include "algo.h"
#include "faceRecognition.h"
#include "feature.h"
#include "lbp59.h"


static HMODULE hModuleX = 0;

const int map[] = {0, 1, 2, 3, 4, 58, 5, 6, 7, 58, 58, 58, 8, 58, 9, 10, 11, 58, 58, 58, 58, 58, 58, 58, 12, 58, 58, 58, 13, 58, 14, 15, 16, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 17, 58, 58, 58, 58, 58, 58, 58, 18, 58, 58, 58, 19, 58, 20, 21, 22, 58, 
	58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 23, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 24, 58, 58, 58, 58, 58, 58, 58, 25, 58, 58, 58, 26, 58, 27, 28, 29, 
	30, 58, 31, 58, 58, 58, 32, 58, 58, 58, 58, 58, 58, 58, 33, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 34, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 35, 
	36, 37, 58, 38, 58, 58, 58, 39, 58, 58, 58, 58, 58, 58, 58, 40, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 41, 42, 43, 58, 44, 58, 58, 58, 45, 58, 58, 58, 58, 58, 58, 58, 46, 47, 48, 58, 49, 58, 58, 58, 50, 51, 52, 58, 53, 54, 55, 56, 57
};


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (0 == hModuleX)
	{
		hModuleX = (HMODULE)hModule;
	}

    return TRUE;
}

// 得到数据指针
inline LPVOID get_data(int id, unsigned int size, HGLOBAL &hGlobal, const char *name)
{
	HRSRC hSrc = ::FindResource(hModuleX, MAKEINTRESOURCE(id), name);
	if (NULL == hSrc) return NULL;

	hGlobal = ::LoadResource(hModuleX, hSrc);
	if (NULL == hGlobal) return NULL;

	LPVOID ptr = ::LockResource(hGlobal);

	DWORD dwSize = ::SizeofResource(hModuleX, hSrc);
	if (dwSize != size) return NULL;

	return ptr;
}

// 释放数据
inline void release(HGLOBAL hGlobal)
{
	::FreeResource(hGlobal);
}

inline void lbp59bin(const uchar *pdata, float *hist)
{
	register int i, j, k, m, n;

	// lbp处理
	const int size = 102 * 102;
	const int sizex = 100 * 100;
	// 内存分配
	uchar *pext = new uchar[size];
	uchar *prst = new uchar[sizex];

	memset(pext, 0, size * sizeof(uchar));

	for (i = 0; i < 100; ++i)
	{
		for (j = 0; j < 100; ++j)
		{
			*(pext + (i + 1) * 102 + (j + 1)) = *(pdata + i * 100 + j);
		}
	}

	for (i = 1; i < 101; ++i)
	{
		for (j = 1; j < 101; ++j)
		{
			uchar *px = pext + i * 102 + j;
			uchar *py = prst + (i - 1) * 100 + (j - 1);

			LBP59_FT(px, 102, 1, py);
		}
	}

	delete []pext; pext = 0;

	// 映射59bin
	for (i = 0; i < sizex; ++i)
	{
		prst[i] = (uchar)map[prst[i]];
	}

	// 分成100块进行直方图统计
	memset(hist, 0, 5900 * sizeof(float));

	k = 0;
	for (i = 0; i < 100; i += 10)
	{
		for (j = 0; j < 100; j += 10)
		{
			// 得到直方图段首指针、子块左上角指针
			float *pt = hist + k;
			uchar *ptr = prst + i * 100 + j;

			for (m = 0; m < 10; ++m)
			{
				for (n = 0; n < 10; ++n)
				{
					const int loc = *(ptr + m * 100 + n);

					pt[loc] += 1.0f;
				}
			}

			k += 59;
		}
	}

	for (i = 0; i < 5900; ++i)
	{
		hist[i] /= 100.0f;
	}

	delete []prst; prst = 0;
}

int feature_gen(int ah, int aw, const unsigned char* A, int al, int ai, int type, float* featptr)
{
	if (0 == featptr)
	{
		return 5900;
	}

	uchar *pp = new uchar[ah * aw];
	CHC_PpLight(ah, aw, A, aw, 1, pp, aw, 1);
	lbp59bin(pp, featptr);

	delete []pp; pp = 0;
	return 5900;
}

// 常量
/*
static const double PI = 3.1415926535897932384626;
static const double kmax = PI * 1.0;
static const double f = sqrt(2.0);
static const double sig = 2.0 * PI;
*/

static const int img_h = 80;
static const int img_w = 80;
static const int bin = 8;
static const int sub_h = 8;
static const int sub_w = 4;

// 函数

// 一维快速傅立叶变换
void fft2(complex_se ft[],int m,int inv)
{ int n,i,j,k,mm,mp;
  double s,t,ang,tpi=6.283185307179586;
  complex_se u,w,*p,*q,*pf;
  n=1; n<<=m; pf=ft+n-1;
  for(j=0,p=ft; p<pf ;++p){ q=ft+j;
    if(p<q){ t=p->re; p->re=q->re; q->re=t;
             t=p->im; p->im=q->im; q->im=t; }
    for(mm=n/2; mm<=j ;mm/=2) j-=mm; j+=mm;
   }
  if(inv!='d') for(p=ft,s=1./n; p<=pf ;){p->re*=s; (p++)->im*=s; }
  for(i=mp=1; i<=m ;++i){
    mm=mp; mp*=2; ang=tpi/mp; if(inv=='d') ang= -ang;
    w.re=cos(ang); w.im=sin(ang);
    for(j=0; j<n ;j+=mp){ p=ft+j;
      u.re=1.; u.im=0.;
      for(k=0; k<mm ;++k,++p){ q=p+mm;
        t=q->re*u.re-q->im*u.im;
        s=q->im*u.re+q->re*u.im;
        q->re=p->re-t; q->im=p->im-s;
        p->re+=t; p->im+=s;
        t=u.re*w.re-u.im*w.im;
        u.im=u.im*w.re+u.re*w.im; u.re=t;
       }
     }
   }
}

// 二维快速傅立叶变换
void fft2_2d(complex_se a[],int m,int n,int f)
{ register int md,nd,i,j; complex_se *p,*q;
  register complex_se *r,*s;
  md=1<<m; nd=1<<n;
  for(i=0,p=a; i<md ;++i){
    fft2(p,n,f); p+=nd;
   }
  q=(complex_se *)malloc(sizeof(*q)*md);
  for(i=0,p=a-nd; i<nd ;++i){
    for(r=q,s=p,j=0; j<md ;++j) *r++ = *(s+=nd);
    fft2(q,m,f);
    for(r=q,s=p++,j=0; j<md ;++j) *(s+=nd)= *r++;
   }
  free(q);
}

// 复数乘法c = a * b
inline void ocmul(const complex_se *a, const complex_se *b, complex_se *c)
{
	const double x = a->re * b->re;
	const double y = a->im * b->im;
	const double z = (a->re + a->im) * (b->re + b->im);

	c->re = x - y;
	c->im = z - x - y;
}

// 根据最合适的2^n尺寸填充扩展后的图像(图像数据填对应位置实部, 虚部全填0)
void fill(complex_se *matrix, const uchar *src, int img_h, int img_w)
{
	memset(matrix, 0, (sizeof(complex_se)) << 14);

	const uchar *ps = src;
	complex_se *pc = matrix;

	for ( ; ps < src + img_h * img_w; )
	{
		const uchar *end = ps + img_w;

		for ( ; ps < end; )
		{
			pc->re = *(ps);

			++pc;
			++ps;
		}

		pc += (128 - img_w);
	}
}

// 卷积函数
void filterx(const uchar *src,              // 80 * 80
				float *rst,                     // 对应小块40幅结果bin存放在连续内存的排序后的原始结果
				const complex_se *kernel        // complex_se: 128 * 128
				)
{
	const int size = img_h * img_w;
	const int len1 = 40 * img_h * img_w * bin / (sub_h * sub_w);
	// 分配处理单一核卷积幅值结果内存
	double *rst_sub = new double[size];
	uchar  *rst_lbp = new uchar[size];
	// 分配扩展内存并填充
	complex_se *src_ext = new complex_se[16384];
	complex_se *rst_ext = new complex_se[16384];

	fill(src_ext, src, img_h, img_w);
	// 傅立叶变换
	fft2_2d(src_ext, 7, 7, 'd');

	int i, j, k;

	// 40幅复数结果图
	for (i = 0; i < 40; ++i)
	{
		// 复数序列点乘
		const complex_se *p0 = src_ext    ;
		const complex_se *p1 = src_ext + 1;
		const complex_se *p2 = src_ext + 2;
		const complex_se *p3 = src_ext + 3;

		const complex_se *q0 = kernel + (i << 14)    ;
		const complex_se *q1 = kernel + (i << 14) + 1;
		const complex_se *q2 = kernel + (i << 14) + 2;
		const complex_se *q3 = kernel + (i << 14) + 3;

		complex_se *r0 = rst_ext    ;
		complex_se *r1 = rst_ext + 1;
		complex_se *r2 = rst_ext + 2;
		complex_se *r3 = rst_ext + 3;

		for ( ; p0 < src_ext + 16384; )
		{
			ocmul(p0, q0, r0);
			ocmul(p1, q1, r1);
			ocmul(p2, q2, r2);
			ocmul(p3, q3, r3);

			p0 += 4; q0 += 4; r0 += 4;
			p1 += 4; q1 += 4; r1 += 4;
			p2 += 4; q2 += 4; r2 += 4;
			p3 += 4; q3 += 4; r3 += 4;
		}
		// 傅立叶逆变换
		fft2_2d(rst_ext, 7, 7, 'i');

		// 左上角80 * 80幅值结果存放到rst_sub中
		for (j = 16; j < img_h + 16; ++j)
		{
			for (k = 16; k < img_w + 16; ++k)
			{
				const double re = (rst_ext + (j << 7) + k)->re;
				const double im = (rst_ext + (j << 7) + k)->im;
				
				*(rst_sub + (j - 16) * img_w + (k - 16)) = sqrt(re * re + im * im);
			}
		}
		// 当前核的卷积结果做LBP8BIN变化
		float *pt = rst + i * (size * bin / (sub_h * sub_w));
		
		LBP8BIN(rst_sub, img_h, img_w, rst_lbp);
		HISTO8BIN(rst_lbp, img_h, img_w, sub_h, sub_w, pt);
	}
	// 释放内存
	delete []rst_ext; rst_ext = 0;
	delete []src_ext; src_ext = 0;
	delete []rst_lbp; rst_lbp = 0;
	delete []rst_sub; rst_sub = 0;
}

int feature_gen2(int ah, int aw, const unsigned char* A, int al, int ai, int type, float* featptr)
{
	const int rst_size = 40 * 200 * 8;

	if (0 == featptr)
	{
		return (rst_size);
	}

	complex_se *kernel = 0;
	HGLOBAL hGlobal;
	kernel = (complex_se*)get_data(IDR_BINARYDATA1, 40 * 128 * 128 * 2 * sizeof(double), hGlobal, "BINARYDATA");

	uchar *pppp = new uchar[ah * aw];
	CHC_PpLight(ah, aw, A, aw, 1, pppp, aw, 1);

	filterx(pppp, featptr, kernel);

	release(hGlobal);
	delete []pppp; pppp = 0;
	return rst_size;
}

template<typename _I> inline 
_I similarity(const _I *lib, const _I *feat, int len)
{
	_I product, sum0, sum1;
	INNER_PRODUCT(lib, lib + len, feat, product);
	SUMMARY_XX(lib, lib + len, sum0);
	SUMMARY_XX(feat, feat + len, sum1);
	return (_I)(product / sqrt(sum0 * sum1));
}

float cos_dist(int n, const float *a, const float *b)
{
	if (7640 != n) return -1.0f;
	float sim = 0.0f;
	for (int i = 0; i < 40; ++i)
	{
		sim += similarity(a + i * 191, b + i * 191, 191);
	}
	return sim;
}