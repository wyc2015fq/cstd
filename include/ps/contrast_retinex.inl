// Retinex这个词是由视网膜(Retina)和大脑皮层(Cortex) 两个词组合构成的。Retinex理论主要包含了两个方面的内容：物体的颜色是由物体对长波、 中波和短波光线的反射能力决定的，而不是由反射光强度的绝对值决定的；物体的色彩不受光照 非均匀性的影响,具有一致性 。
// 根据Retinex理论，人眼感知物体的亮度取决于环境的照明和物体表面对照射光的反射，其数学表达式为：
// I(x,y)=L(x,y)*R(x,y)　　　　　　　　　　　　　　　　　　　　　　　　　　　　(2-1)
// 　　式中： I(x,y)代表被观察或照相机接收到的图像信号；L(x,y)代表环境光的照射分量 ；R(x,y)表示携带图像细节信息的目标物体的反射分量 。
// 将(2-1)式两边取对数，则可抛开入射光的性质得到物体的本来面貌，即有关系式 ：
// Log[R(x,y)] = Log[I(x,y)]-Log[L(x,y)];　　　　　　　　　　　　　　　　  　　　　(2-2)
// 对上面的理论的进行一个简单的注释吧。把这个技术运用到图像处理上，就是针对我们现在已经获得的一副图像数据I(x,y），计算出对应的R(x,y)，则R(x,y)认为是增强后的图像，现在的关键是如何得到L(X,Y)。Retinex理论的提出者指出这个L(x,y）可以通过对图像数据I(x,y）进行高斯模糊而得到，很多论文中都列出了那个中心/围绕函数以及需要归一化的K值，搞的很多新手都不明白是什么了，其实就是一个模糊而已。从实际运用的角度来说，也可以用均值模糊来代替高斯模糊。
// 因此这个算法的细路就很简单了，具体步骤如下：
// 1、输入: 原始图像数据I(x,y),尺度（也就是所谓的模糊的半径）
// 2、处理：(1) 计算原始图像按指定尺度进行模糊后的图像 L(x,y);
// (2) 按照2-2式的计算方法计算出 Log[R(x,y)]的值。
// (3) 将 Log[R(x,y)]量化为0到255范围的像素值，作为最终的输出。
// 可以看得出，算法很简单，其核心的东西还是在于高斯模糊的实现。关于高斯模糊，网上有很多快速优化的文章参考，具体的参考代码可能很少有好人提供的。
// 注意到一点，似乎在量化的时候没有谁会将 Log[R(x,y)]进行Exp函数的运算而直接得到R(x,y)，至于为什么，我无法给出明确的答案。
// 量化的方式其实有很多种，而这个方法在很大的程度上对处理的效果有着决定性的影响。至今我没看到有哪一篇论文对这一块讲的很清楚，也不知道他们的那些结果是如何取得的，一种最简单的方式就是计算出Log[R(x,y)]的最大值Max和最小值Min,然后对每一个值Value，进行线性量化，公式为：
// R(x,y) = ( Value - Min ) / (Max - Min) * (255-0)                           (2-3)

typedef unsigned char guchar;
typedef int gint;
typedef int gboolean;
typedef float gfloat;
typedef double gdouble;

#define MAX_RETINEX_SCALES    8
#define MIN_GAUSSIAN_SCALE   16
#define MAX_GAUSSIAN_SCALE  250
#define SCALE_WIDTH         150
#define ENTRY_WIDTH           4


typedef struct {
  gint     scale;
  gint     nscales;
  gint     scales_mode;
  gdouble   cvar;
} RetinexParams;

/*
  Definit comment sont repartis les
  differents filtres en fonction de
  l'echelle (~= ecart type de la gaussienne)
 */
#define RETINEX_UNIFORM 0
#define RETINEX_LOW     1
#define RETINEX_HIGH    2

static gdouble RetinexScales[MAX_RETINEX_SCALES];

typedef struct {
  gint    N;
  gdouble sigma;
  gdouble B;
  gdouble b[4];
} gauss3_coefs;

/*
 * Private variables.
 */
static RetinexParams rvals = {
  240,             /* 最大尺度 Scale Biggest scale value */
  3,               /* 尺度数量 Scales Number of scales*/
  RETINEX_UNIFORM, /* 对比度 Echelles reparties uniformement  Retinex distribution through scales*/
  1.2f              /* A voir Variance value*/
};


// N_("Enhance contrast using the Retinex method"),
// "The Retinex Image Enhancement Algorithm is an "
// "automatic image enhancement method that enhances "
// "a digital image in terms of dynamic range "
// "compression, color independence from the spectral "
// "distribution of the scene illuminant, and "
// "color/lightness rendition.",
// "Fabien Pelisson",
// "Fabien Pelisson",
// "2003",
// N_("Retine_x..."),
// "RGB*",
// GIMP_PLUGIN,
// G_N_ELEMENTS (args), 0,
// args, NULL);




/*
 * calculate scale values for desired distribution.
 */
static void retinex_scales_distribution(gdouble* scales, gint nscales, gint mode, gint s)
{
  if (nscales == 1) {
    /* For one filter we choose the median scale */
    scales[0] = (gdouble) s / 2;
  }
  else if (nscales == 2) {
    /* For two filters whe choose the median and maximum scale */
    scales[0] = (gdouble) s / 2;
    scales[1] = (gdouble) s;
  }
  else {
    gdouble size_step = (gdouble) s / (gdouble) nscales;
    gint   i;

    switch (mode) {
    case RETINEX_UNIFORM:
      for (i = 0; i < nscales; ++i) {
        scales[i] = 2.f + (gdouble) i * size_step;
      }

      break;

    case RETINEX_LOW:
      size_step = (gdouble) log(s - 2.0) / (gdouble) nscales;

      for (i = 0; i < nscales; ++i) {
        scales[i] = 2.f + pow(10, (i * size_step) / log(10));
      }

      break;

    case RETINEX_HIGH:
      size_step = (gdouble) log(s - 2.0) / (gdouble) nscales;

      for (i = 0; i < nscales; ++i) {
        scales[i] = s - pow(10, (i * size_step) / log(10));
      }

      break;

    default:
      break;
    }
  }
}

/*
 * Calculate the coefficients for the recursive filter algorithm
 * Fast Computation of gaussian blurring.
 */
static void compute_coefs3(gauss3_coefs* c, gdouble sigma)
{
  /*
   * Papers:  "Recursive Implementation of the gaussian filter.",
   *          Ian T. Young , Lucas J. Van Vliet, Signal Processing 44, Elsevier 1995.
   * formula: 11b       computation of q
   *          8c        computation of b0..b1
   *          10        alpha is normalization constant B
   */
  gdouble q, q2, q3;

  q = 0;

  if (sigma >= 2.5) {
    q = 0.98711 * sigma - 0.96330;
  }
  else if ((sigma >= 0.5) && (sigma < 2.5)) {
    q = 3.97156 - 4.14554 * (gdouble) sqrt((double) 1 - 0.26891 * sigma);
  }
  else {
    q = 0.1147705018520355224609375;
  }

  q2 = q * q;
  q3 = q * q2;
  c->b[0] = (1.57825 + (2.44413 * q) + (1.4281 * q2) + (0.422205 * q3));
  c->b[1] = ((2.44413 * q) + (2.85619 * q2) + (1.26661 * q3));
  c->b[2] = (-((1.4281 * q2) + (1.26661 * q3)));
  c->b[3] = ((0.422205 * q3));
  c->B = 1.0 - ((c->b[1] + c->b[2] + c->b[3]) / c->b[0]);
  c->sigma = sigma;
  c->N = 3;

  /*
    g_printerr ("q %f\n", q);
    g_printerr ("q2 %f\n", q2);
    g_printerr ("q3 %f\n", q3);
    g_printerr ("c->b[0] %f\n", c->b[0]);
    g_printerr ("c->b[1] %f\n", c->b[1]);
    g_printerr ("c->b[2] %f\n", c->b[2]);
    g_printerr ("c->b[3] %f\n", c->b[3]);
    g_printerr ("c->B %f\n", c->B);
    g_printerr ("c->sigma %f\n", c->sigma);
    g_printerr ("c->N %d\n", c->N);
  */
}

static int gausssmooth(gdouble* in, gdouble* out, gint size, gint rowstride, gauss3_coefs* c)
{
  /*
   * Papers:  "Recursive Implementation of the gaussian filter.",
   *          Ian T. Young , Lucas J. Van Vliet, Signal Processing 44, Elsevier 1995.
   * formula: 9a        forward filter
   *          9b        backward filter
   *          fig7      algorithm
   */
  gint i, n, bufsize;
  gdouble* w1, *w2;

#define g_try_malloc malloc
#define g_free free
  /* forward pass */
  bufsize = size + 3;
  size -= 1;
  w1 = (gdouble*) g_try_malloc(bufsize * sizeof(gdouble));
  w2 = (gdouble*) g_try_malloc(bufsize * sizeof(gdouble));
  w1[0] = in[0];
  w1[1] = in[0];
  w1[2] = in[0];

  for (i = 0 , n = 3; i <= size ; i++, n++) {
    w1[n] = (gdouble)(c->B * in[i * rowstride] +
        ((c->b[1] * w1[n - 1] +
            c->b[2] * w1[n - 2] +
            c->b[3] * w1[n - 3]) / c->b[0]));
  }

  /* backward pass */
  w2[size + 1] = w1[size + 3];
  w2[size + 2] = w1[size + 3];
  w2[size + 3] = w1[size + 3];

  for (i = size, n = i; i >= 0; i--, n--) {
    w2[n] = out[i * rowstride] = (gdouble)(c->B * w1[n] +
        ((c->b[1] * w2[n + 1] +
            c->b[2] * w2[n + 2] +
            c->b[3] * w2[n + 3]) / c->b[0]));
  }

  g_free(w1);
  g_free(w2);
  return 0;
}

// Calculate the average and variance in one go.
static void compute_mean_var(gdouble* src, gdouble* mean, gdouble* var, gint size, gint bytes)
{
  gdouble vsquared;
  gint i, j;
  gdouble* psrc;

  vsquared = 0;
  *mean = 0;

  for (i = 0; i < size; i += bytes) {
    psrc = src + i;

    for (j = 0 ; j < 3 ; j++) {
      *mean += psrc[j];
      vsquared += psrc[j] * psrc[j];
    }
  }

  *mean /= (gdouble) size; /* mean */
  vsquared /= (gdouble) size; /* mean (x^2) */
  *var = (vsquared - (*mean * *mean));
  *var = sqrt(*var); /* var */
}

// MSRCR = MultiScale Retinex with Color Restoration
//  * This function is the heart of the algo.
//  * (a)  Filterings at several scales and sumarize the results.
//  * (b)  Calculation of the final values.
int MSRCR(gint width, gint height, const guchar* src, guchar* b, int bl, gint bytes)
{
  gint          scale, row, col;
  gint          i, j;
  gint          size;
  gint          pos;
  gint          channel;
  const guchar*       psrc = NULL;            /* backup pointer for src buffer */
  gdouble*       dst  = NULL;            /* float buffer for algorithm */
  gdouble*       pdst = NULL;            /* backup pointer for float buffer */
  gdouble*       in, *out;
  gint          channelsize;            /* Float memory cache for one channel */
  gdouble        weight;
  gauss3_coefs  coef;
  gdouble        mean, var;
  gdouble        mini, range, maxi;
  gdouble        alpha;
  gdouble        gain;
  gdouble        offset;

#define g_warning printf
  /* Allocate all the memory needed for algorithm*/
  size = width * height * bytes;
  dst = g_try_malloc(size * sizeof(gdouble));

  if (dst == NULL) {
    g_warning("Failed to allocate memory");
    return 0;
  }

  memset(dst, 0, size * sizeof(gdouble));

  channelsize  = (width * height);
  in  = (gdouble*) g_try_malloc(channelsize * sizeof(gdouble));

  if (in == NULL) {
    g_free(dst);
    g_warning("Failed to allocate memory");
    return 0; /* do some clever stuff */
  }

  out  = (gdouble*) g_try_malloc(channelsize * sizeof(gdouble));

  if (out == NULL) {
    g_free(in);
    g_free(dst);
    g_warning("Failed to allocate memory");
    return 0; /* do some clever stuff */
  }



  // Calculate the scales of filtering according to the
  // number of filter and their distribution.
  retinex_scales_distribution(RetinexScales, rvals.nscales, rvals.scales_mode, rvals.scale);


  // Filtering according to the various scales.
  // Summerize the results of the various filters according to a
  // specific weight(here equivalent for all).
  weight = 1. / (gdouble) rvals.nscales;


  // The recursive filtering algorithm needs different coefficients according
  // to the selected scale (~ = standard deviation of Gaussian).
  pos = 0;

  for (channel = 0; channel < 3; channel++) {
    for (i = 0, pos = channel; i < channelsize ; i++, pos += bytes) {
      /* 0-255 => 1-256 */
      in[i] = (gdouble)(src[pos] + 1.0);
    }

    for (scale = 0; scale < rvals.nscales; scale++) {
      compute_coefs3(&coef, RetinexScales[scale]);

      //Filtering (smoothing) Gaussian recursive.
      //Filter rows first
      for (row = 0 ; row < height; row++) {
        pos =  row * width;
        gausssmooth(in + pos, out + pos, width, 1, &coef);
      }

      memcpy(in,  out, channelsize * sizeof(gdouble));
      memset(out, 0  , channelsize * sizeof(gdouble));

      //Filtering (smoothing) Gaussian recursive.
      //Second columns
      for (col = 0; col < width; col++) {
        pos = col;
        gausssmooth(in + pos, out + pos, height, width, &coef);
      }

      // Summarize the filtered values.
      //  In fact one calculates a ratio between the original values and the filtered values.
      for (i = 0, pos = channel; i < channelsize; i++, pos += bytes) {
        dst[pos] += weight * (log(src[pos] + 1.) - log(out[i]));
      }
    }
  }

  g_free(in);
  g_free(out);

  // Final calculation with original value and cumulated filter values.
  // The parameters gain, alpha and offset are constants.

  // Ci(x,y)=log[a Ii(x,y)]-log[ Ei=1-s Ii(x,y)]

  alpha  = 128.;
  gain   = 1.;
  offset = 0.;

  for (i = 0; i < size; i += bytes) {
    gdouble logl;

    psrc = src + i;
    pdst = dst + i;

    logl = log((gdouble)psrc[0] + (gdouble)psrc[1] + (gdouble)psrc[2] + 3.);

    pdst[0] = gain * ((log(alpha * (psrc[0] + 1.)) - logl) * pdst[0]) + offset;
    pdst[1] = gain * ((log(alpha * (psrc[1] + 1.)) - logl) * pdst[1]) + offset;
    pdst[2] = gain * ((log(alpha * (psrc[2] + 1.)) - logl) * pdst[2]) + offset;
  }

  // if (!preview_mode)
  // gimp_progress_update ((2.0 + (rvals.nscales * 3)) / ((rvals.nscales * 3) + 3));

  //Adapt the dynamics of the colors according to the statistics of the first and second order.
  // The use of the variance makes it possible to control the degree of saturation of the colors.
  pdst = dst;

  compute_mean_var(pdst, &mean, &var, size, bytes);
  mini = mean - rvals.cvar * var;
  maxi = mean + rvals.cvar * var;
  range = maxi - mini;

  if (!range) {
    range = 1.0;
  }

  for (i = 0; i < size; i += bytes) {
    guchar* b1 = b + i;
    psrc = src + i;
    pdst = dst + i;

    for (j = 0 ; j < 3 ; j++) {
      gdouble c = 255 * (pdst[j] - mini) / range;

      b1[j] = (guchar) CLAMP(c, 0, 255);
    }
  }

  g_free(dst);
  return 0;
}

