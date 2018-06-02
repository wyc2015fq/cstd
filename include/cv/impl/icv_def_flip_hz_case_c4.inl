
#define icv_def_flip_hz_case_c4(type)

for (i = 0; i < (len + 1) / 2; i++)
{
  type t0 = src[i * 4];
  type t1 = src[(len - i) * 4 - 4];
  dst[i * 4] = t1;
  dst[(len - i) * 4 - 4] = t0;
  t0 = src[i * 4 + 1];
  t1 = src[(len - i) * 4 - 3];
  dst[i * 4 + 1] = t1;
  dst[(len - i) * 4 - 3] = t0;
  t0 = src[i * 4 + 2];
  t1 = src[(len - i) * 4 - 2];
  dst[i * 4 + 2] = t1;
  dst[(len - i) * 4 - 2] = t0;
  t0 = src[i * 4 + 3];
  t1 = src[(len - i) * 4 - 1];
  dst[i * 4 + 3] = t1;
  dst[(len - i) * 4 - 1] = t0;
}

