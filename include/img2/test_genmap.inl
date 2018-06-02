#if 0
// 柏林噪声产生火焰等纹理
//柏林噪声是一种特殊的随机噪声，即对于每个给定的值产生的随机数是唯一的，但是不同的
//值产生不同的随机数。关于柏林噪声更详细的解释可以参考这里：
//http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
//本文主要是探讨如何使用柏林噪声产生火焰效果与乌云效果的纹理，在解释实现代码之前，
//首先来看一下影响柏林噪声效果的两个参数音度(Octave) 与余辉(Persistence)，可以调节
//纹理的大小和密度。
//最终实现的火焰纹理效果
//最终实现的乌云纹理效果
//最终实现的草地纹理效果–怎么感觉有点魔兽中精灵族的草地啊，哈哈
//代码解释
//首先产生随机空白噪声，使用随机空白噪声产生柏林噪声，最后将柏林噪声映射为RGB值
//输出到指定大小的图像中，代码如下:
//float[][] whiteNoise = GenerateWhiteNoise(height, width);
//float[][] perlinNoise = GeneratePerlinNoise(whiteNoise, 6); //
//float[][] colorData = MapGray(perlinNoise);
//白噪声产生主要是利用Java中的系统时间作为种子，产生[0~1]之间的噪声数组
//代码如下：
float[][] GenerateWhiteNoise(int width, int height)
{
  Random random = new Random(System.currentTimeMillis());
  float[][] noise = new float[width][height];
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      noise[i][j] = (float)random.nextDouble();
    }
  }
  return noise;
}
//柏林噪声的产生稍微复杂一点点，首先把上面的白噪声数据带入，利用插值公式产生平滑的噪声
//数据，具体要产生几组平滑噪声数据取决于音度(Octave)参数。本程序的插值公式非常简单，
//代码如下：Interpolate
//最后把这些组的平滑噪声加上不同的振幅混合在一起产生一个输出数组结果即为柏林噪声。
//完成上面这些还不足以产生那些效果震撼的纹理，另外一个顶级秘诀在于怎么把柏林噪声
//映射到你想要的RGB值。代码如下：MapGradient
//程序完全源代码如下：
#endif
#include "cstd.h"
// PerlinNoiseCreator
float Interpolate(float x0, float x1, float alpha)
{
  return x0 * (1 - alpha) + alpha * x1;
}
void GenerateWhiteNoise(uint* pseed, float* noise, int step, int width, int height)
{
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      noise[i * step + j] = (float)myrandreal(pseed);
    }
  }
}
void GenerateSmoothNoise(float* smoothNoise, int smooth_step, const float* baseNoise, int base_step, int width, int height, int octave)
{
  int i, j;
  int samplePeriod = 1 << octave; // calculates 2 ^ k
  float sampleFrequency = 1.0f / samplePeriod;
  for (i = 0; i < height; i++) {
    //calculate the horizontal sampling indices
    int sample_i0 = (i / samplePeriod) * samplePeriod;
    int sample_i1 = (sample_i0 + samplePeriod) % height; //wrap around
    float horizontal_blend = (i - sample_i0) * sampleFrequency;
    for (j = 0; j < width; j++) {
      //calculate the vertical sampling indices
      int sample_j0 = (j / samplePeriod) * samplePeriod;
      int sample_j1 = (sample_j0 + samplePeriod) % width; //wrap around
      float vertical_blend = (j - sample_j0) * sampleFrequency;
      //blend the top two corners
      float top = Interpolate(baseNoise[sample_i0 * base_step + sample_j0], baseNoise[sample_i1 * base_step + sample_j0], horizontal_blend);
      //blend the bottom two corners
      float bottom = Interpolate(baseNoise[sample_i0 * base_step + sample_j1],
          baseNoise[sample_i1 * base_step + sample_j1], horizontal_blend);
      //final blend
      smoothNoise[i * smooth_step + j] = Interpolate(top, bottom, vertical_blend);
    }
  }
}
void GeneratePerlinNoise(float* perlinNoise, int perlin_step, const float* baseNoise, int base_step, int width, int height, int octaveCount)
{
  int i, j, octave, n = width * height, step = width;
  float persistance = 0.5f; // default value is 0.5f
  float amplitude = 1.0f, totalAmplitude = 0.0f;
  float* smoothNoise = NULL;
  MYREALLOC(smoothNoise, octaveCount * width * height); //an array of 2D arrays containing
  //generate smooth noise
  for (i = 0; i < octaveCount; i++) {
    GenerateSmoothNoise(smoothNoise + i * n, step, baseNoise, base_step, width, height, i);
  }
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      perlinNoise[i * perlin_step + j] = 0;
    }
  }
  //blend noise together
  for (octave = octaveCount - 1; octave >= 0; octave--) {
    amplitude *= persistance;
    totalAmplitude += amplitude;
    for (i = 0; i < height; i++) {
      for (j = 0; j < width; j++) {
        perlinNoise[i * perlin_step + j] += smoothNoise[octave * n + i * step + j] * amplitude;
      }
    }
  }
  //normalization
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      perlinNoise[i * perlin_step + j] /= totalAmplitude;
    }
  }
  FREE(smoothNoise);
  return ;
}
void MapGray(float* image, int image_step, const float* perlinNoise, int perlin_step, int width, int height)
{
  int i, j, ta = 0, tr = 0, tb = 0, tg = 0;
  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      ta = 255;
      int u = (int)(perlinNoise[i * perlin_step + j] * (float)80.0);
      tr = u + 100;
      tg = u + 100;
      tb = u + 100;
      //ta = (int)(255.0f * perlinNoise[i][j]);
      image[i * image_step + j] = (ta << 24) | (tr << 16) | (tg << 8) | tb;
    }
  }
  return ;
}
void generateNoise(uint* pseed, int* noiseData, int noise_step, int width, int height)
{
  float* whiteNoise = NULL;
  float* perlinNoise = NULL;
  float* colorData = NULL;
  int i, j, n = height * width, step = width;
  MYREALLOC(whiteNoise, 3 * n);
  perlinNoise = whiteNoise + n;
  colorData = whiteNoise + 2 * n;
  GenerateWhiteNoise(pseed, whiteNoise, step, width, height);
  GeneratePerlinNoise(perlinNoise, step, whiteNoise, step, width, height, 6); // default value is 6
  //float[][] colorData = MapGradient(perlinNoise);
  MapGray(colorData, step, perlinNoise, step, width, height);
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      noiseData[i * noise_step + j] = (int)colorData[i * step + j];
    }
  }
  FREE(whiteNoise);
}
static const COLOR angryFireColorTable[] = {
#define ANGRY_RGB(r, g, b) _RGB(r, g, b),
#include "angryFireColorTable.txt"
#undef ANGRY_RGB
};
void MapGradient(COLOR* image, int image_step, float* perlinNoise, int perlin_step, int width, int height)
{
  int i, j;
  enum {angryFireColorTable_length = countof(angryFireColorTable)};
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      int u = (int)(perlinNoise[i * perlin_step + j] * (float)angryFireColorTable_length);
      image[i * image_step + j] = angryFireColorTable[u];
    }
  }
}
#include "ui/window.inl"
#include "test_lic.inl"
#include "test_shade.inl"
int test_genmap()
{
  int i = 0;
  img_t im[10] = {0};
  img_t* white = im + i++;
  img_t* perlin = im + i++;
  int h = 1024, w = h, step = w;
  uint pseed[2] = {12};
  imsetsize(im, h, w, sizeof(COLOR), 1);
  imsetsize(white, h, w, sizeof(float), 1);
  imsetsize(perlin, h, w, sizeof(float), 1);
  test_shade();
  //test_lic();
  for (;;) {
    float* whiteNoise = (float*)white->tt.data;
    float* perlinNoise = (float*)perlin->tt.data;
    GenerateWhiteNoise(pseed, whiteNoise, step, w, h);
    GeneratePerlinNoise(perlinNoise, step, whiteNoise, step, w, h, 6);
    MapGradient((COLOR*)im->tt.data, step, perlinNoise, step, w, h);
    imshow(im);
    cvWaitKey(100);
  }
  imfrees(im, countof(im));
  return 0;
}

