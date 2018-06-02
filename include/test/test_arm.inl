

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

#if 0
void mul_ker(int nout, const float* in, const float* kernel0, float* out) {
  const float* r0 = in;
  const float* r1 = in+nout;
  const float* r2 = in+nout;
  float* outptr = out;
  int nn=nout>>2;
  float32x4_t _k0123 = vld1q_f32(kernel0);
  float32x4_t _k3456 = vld1q_f32(kernel0+3);
  float32x4_t _k6789 = vld1q_f32(kernel0+6);
  for (; nn>0; nn--)
  {
    float32x4_t _sum1 = vld1q_f32(outptr);
    float32x4_t _sum2 = vdupq_n_f32(0.f);
    
    float32x4_t _r00 = vld1q_f32(r0);
    float32x4_t _r00n = vld1q_f32(r0 + 4);
    float32x4_t _r01 = vextq_f32(_r00, _r00n, 1);
    float32x4_t _r02 = vextq_f32(_r00, _r00n, 2);
    
    float32x4_t _r10 = vld1q_f32(r1);
    float32x4_t _r10n = vld1q_f32(r1 + 4);
    float32x4_t _r11 = vextq_f32(_r10, _r10n, 1);
    float32x4_t _r12 = vextq_f32(_r10, _r10n, 2);
    
    float32x4_t _r20 = vld1q_f32(r2);
    float32x4_t _r20n = vld1q_f32(r2 + 4);
    float32x4_t _r21 = vextq_f32(_r20, _r20n, 1);
    float32x4_t _r22 = vextq_f32(_r20, _r20n, 2);
    
    _sum1 = vfmaq_laneq_f32(_sum1, _r00, _k0123, 0);
    _sum2 = vfmaq_laneq_f32(_sum2, _r01, _k0123, 1);
    _sum1 = vfmaq_laneq_f32(_sum1, _r02, _k0123, 2);
    _sum2 = vfmaq_laneq_f32(_sum2, _r10, _k3456, 0);
    _sum1 = vfmaq_laneq_f32(_sum1, _r11, _k3456, 1);
    _sum2 = vfmaq_laneq_f32(_sum2, _r12, _k3456, 2);
    _sum1 = vfmaq_laneq_f32(_sum1, _r20, _k6789, 0);
    _sum2 = vfmaq_laneq_f32(_sum2, _r21, _k6789, 1);
    _sum1 = vfmaq_laneq_f32(_sum1, _r22, _k6789, 2);
    
    _sum1 = vaddq_f32(_sum1, _sum2);
    
    vst1q_f32(outptr, _sum1);
    
    r0 += 4;
    r1 += 4;
    r2 += 4;
    outptr += 4;
  }
}
int test_arm() {
  enum {N=100*4};
  float in = (float*)malloc(4*N);
  float out[N];
  float kernel0[9];
  mul_ker(N, in, kernel0, out);
  return 0;
}
#endif

#if 1
#include <stdio.h>
void test_arm_fun(int n, float* in, float* out) {
  float *inptr = in;
  float *outptr = out;
  int i;
  float kernel0[] = {1,2,3,4,5,6,7,8,9};
  float32x4_t _sum1 = vdupq_n_f32(1.f);
  float32x4_t _sum2 = vdupq_n_f32(0.f);
  float32x4_t _k0123 = vld1q_f32(kernel0);
  float32x4_t _k6789 = vld1q_f32(kernel0+6);
  for (i=0; i<n; ++i) {
    float32x4_t _r22 = vld1q_f32(inptr);
    _sum1 = vfmaq_laneq_f32(_sum1, _r22, _k0123, 0);
    _sum2 = vfmaq_laneq_f32(_sum2, _r22, _k6789, 2);
    inptr += 4;
  }
  _sum1 = vaddq_f32(_sum1, _sum2);
  vst1q_f32(outptr, _sum1);
  return ;
}
int test_arm() {
  float in[] = {1,2,3,4,5,6,7,8};
  float out[8] = {1,2,3,4,5,6,7,8};
  test_arm_fun(2, in, out);
  double tt = out[1]+1.f;
  printf("%f %f\n", out[0], tt);
  return 0;
}
#endif