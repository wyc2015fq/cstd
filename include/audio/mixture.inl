double pdf(int D, const double* m, const double* v, const double* x)
{
  int i;
  double p = 0, prodv = 1, y = 0;
  for (i = 0; i < D; ++i) {
    double t = x[i] - m[i];
    y += t * t / v[i];
    prodv *= v[i];
  }
  p = exp(-0.5 * y) / sqrt(2 * PI * prodv);
  return p;
}
double mixture(const img_t* mix, const double* x)
{
  //计算输出概率
  //输入:
  // mix -- 混合高斯结构
  // x -- 输入向量, SIZE*1
  //输出:
  // prob -- 输出概率
  int j, M = mix->h, D = (mix->w) / 2;
  double prob = 0;
  //imshowmat_f8(mix);cvWaitKey(-1);
  for (j = 0; j < M; ++j) {
    const double* m = mix->tt.f8 + j * mix->w;
    const double* v = m + D;
    const double w = m[2 * D];
    prob += w * pdf(D, m, v, x);
  }
  // 加上realmin, 以防止viterbi.m中计算log(prob)时溢出
  if (prob == 0) {
    prob = DBL_MIN;
  }
  return prob;
}

