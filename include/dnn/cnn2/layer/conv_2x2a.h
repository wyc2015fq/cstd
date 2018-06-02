

static void conv2x2s1(const Blob* bottom_blob, Blob* top_blob, const float* _kernel, const float* _bias)
{
  int w = bottom_blob->w;
  int inch = Blob_channels(bottom_blob);

  int outw = top_blob->w;
  int outh = top_blob->h;
  int outch = Blob_channels(top_blob);

  const float* kernel = _kernel;
  const float* bias = _bias;

  #pragma omp parallel for

  for (int p = 0; p < outch; p++) {
    Blob out = Blob_channel(top_blob, p);

    const float bias0 = bias ? bias[p] : 0.f;

    Blob_fill(&out, bias0);

    for (int q = 0; q < inch; q++) {
      float* outptr = Blob_datap(&out);
      float* outptr2 = outptr + outw;

      const float* img0 = Blob_channel_p(bottom_blob, q);

      const float* kernel0 = kernel + p * inch * 4  + q * 4;

      const float* r0 = img0;
      const float* r1 = img0 + w;
      const float* r2 = img0 + w * 2;

      const float* k0 = kernel0;
      const float* k1 = kernel0 + 2;

      int i = 0;

      for (; i + 1 < outh; i += 2) {
        int remain = outw;

        for (; remain > 0; remain--) {
          float sum = 0;
          float sum2 = 0;

          sum += r0[0] * k0[0];
          sum += r0[1] * k0[1];
          sum += r1[0] * k1[0];
          sum += r1[1] * k1[1];

          sum2 += r1[0] * k0[0];
          sum2 += r1[1] * k0[1];
          sum2 += r2[0] * k1[0];
          sum2 += r2[1] * k1[1];

          *outptr += sum;
          *outptr2 += sum2;

          r0++;
          r1++;
          r2++;
          outptr++;
          outptr2++;
        }

        r0 += 1 + w;
        r1 += 1 + w;
        r2 += 1 + w;

        outptr += outw;
        outptr2 += outw;
      }

      for (; i < outh; i++) {
        int remain = outw;

        for (; remain > 0; remain--) {
          float sum = 0;

          sum += r0[0] * k0[0];
          sum += r0[1] * k0[1];
          sum += r1[0] * k1[0];
          sum += r1[1] * k1[1];

          *outptr += sum;

          r0++;
          r1++;
          outptr++;
        }

        r0 += 1;
        r1 += 1;
        r2 += 1;
      }

    }
  }

}
