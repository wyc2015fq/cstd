
// 感知哈希算法
__int64 perceptual_hash(int h, int w, const uchar* A, int al, int ai)
{
  int sum[64] = {0};
  int cnt[64] = {0};
  int i, j, i1, j1, avg = 0;
  __int64 ret = 0;

  // 第一步，缩小尺寸。
  // 将图片缩小到8x8的尺寸，总共64个像素。这一步的作用是去除图片的细节，只保留结构、明暗等基本信息，摒弃不同尺寸、比例带来的图片差异。
  for (i = 0; i < h; ++i) {
    const uchar* A0 = A + i * al;
    i1 = i * 8 / h;

    for (j = 0; j < w; ++j) {
      j1 = i1 * 8 + j * 8 / w;
      sum[j1] += (A0[0] + A0[1] + A0[1] + A0[2]) >> 2;
      cnt[j1]++;
    }
  }

  // 第二步，简化色彩。
  // 将缩小后的图片，转为64级灰度。也就是说，所有像素点总共只有64种颜色。
  // 第三步，计算平均值。
  // 计算所有64个像素的灰度平均值。
  for (i = 0; i < 64; ++i) {
    sum[i] /= cnt[i];
    avg += sum[i];
  }

  avg /= 64;

  // 第四步，比较像素的灰度。
  // 将每个像素的灰度，与平均值进行比较。大于或等于平均值，记为1；小于平均值，记为0。
  for (i = 0; i < 64; ++i) {
    ret |= ((__int64)(sum[i] > avg)) << (i);
  }

  return ret;
}

int hamming_distance_i64(__int64 a, __int64 b)
{
  int i = 0, d = 0;
  __int64 c = a ^ b;

  for (i = 0; i < 64; ++i) {
    d += !!(c & (1 << i));
  }

  return d;
}

//#include "../fmtimg/fmtimg.inl"

int test_perceptual_hash()
{
  img_t im[1] = {0};
  FILE* pf;
  char buf[256];
  int i = 0, d;
  __int64 sig[100];
  _chdir("D:/code/c/图形图像/相似图片搜索/SimilarImageSearch/images");
  pf = fopen("list.txt", "rb");

  for (; fgets(buf, 256, pf) > 0; ++i) {
    strtrim(buf, "\n\r\t ");
    imread(buf, 3, 1, im);
    {
      utime_start(_start_time);
      sig[i] = perceptual_hash(im->h, im->w, im->data, im->s, im->c);
      d = hamming_distance_i64(sig[i], sig[0]);
      printf("%f\n", utime_elapsed(_start_time));
    }
    // 汉明距离 查表
    printf("%d %I64X  %s\n", d, sig[i], buf);
  }

  imfree(im);
  return 0;
}
