
CC_INLINE int vn_add_s(int n, T* m, const T value)
{
  int i;

  for (i = 0; i < n; ++i) {
    m[i] += value;
  }

  return 0;
}

CC_INLINE int vn_sub_s(int n, T* m, const T value)
{
  int i;

  for (i = 0; i < n; ++i) {
    m[i] -= value;
  }

  return 0;
}

CC_INLINE int vn_mul_s(int n, T* m, const T value)
{
  int i;

  for (i = 0; i < n; ++i) {
    m[i] *= value;
  }

  return 0;
}

CC_INLINE int vn_div_s(int n, T* m, const T value)
{
  int i;

  for (i = 0; i < n; ++i) {
    m[i] /= value;
  }

  return 0;
}

CC_INLINE int vn_add(int n, T* m, const T* _matrix)
{
  int i;

  for (i = 0; i < n; ++i) {
    m[i] += _matrix[i];
  }

  return 0;
}

CC_INLINE int vn_sub(int n, T* m, const T* _matrix)
{
  int i;

  for (i = 0; i < n; ++i) {
    m[i] -= _matrix[i];
  }

  return 0;
}

CC_INLINE void vn_zero(int n, T* m)
{
  int i;

  for (i = 0; i < n; ++i) {
    m[i] = (0.0);
  }
}

CC_INLINE void matrix_identity(int M, int N, T* m)
{
  int x, y;

  for (y = 0; y < N; ++y) {
    for (x = 0; x < M; ++x) {
      m[x + M * y] = ((x == y) ? (1.0) : (0.0));
    }
  }
}

CC_INLINE void matrix_swap(int M, T* m, const unsigned int x1, const unsigned int y1,
    const unsigned int x2, const unsigned int y2)
{
  T temp = m[x1 + M * y1];
  m[x1 + M * y1] = m[x2 + M * y2];
  m[x2 + M * y2] = temp;
}

CC_INLINE void transpose_1x1(T* m)
{
}

CC_INLINE void transpose_2x2(T* m)
{
  matrix_swap(2, m, 0, 1, 1, 0);
}

CC_INLINE void transpose_3x3(T* m)
{
  matrix_swap(3, m, 0, 1, 1, 0);
  matrix_swap(3, m, 0, 2, 2, 0);
  matrix_swap(3, m, 1, 2, 2, 1);
}

CC_INLINE void transpose_4x4(T* m)
{
  matrix_swap(4, m, 0, 1, 1, 0);
  matrix_swap(4, m, 0, 2, 2, 0);
  matrix_swap(4, m, 0, 3, 3, 0);
  matrix_swap(4, m, 1, 2, 2, 1);
  matrix_swap(4, m, 1, 3, 3, 1);
  matrix_swap(4, m, 2, 3, 3, 2);
}

CC_INLINE T det_1x1(const T* m)
{
  return m[0];
}

CC_INLINE T det_2x2(const T* m)
{
  return m[0] * m[3] - m[1] * m[2];
}

CC_INLINE T det_3x3(const T* m)
{
  enum {M = 3};
  return (m[0 + M * 0] * (m[1 + M * 1] * m[2 + M * 2] - m[1 + M * 2] * m[2 + M * 1]) -
      m[1 + M * 0] * (m[0 + M * 1] * m[2 + M * 2] - m[0 + M * 2] * m[2 + M * 1]) +
      m[2 + M * 0] * (m[0 + M * 1] * m[1 + M * 2] - m[0 + M * 2] * m[1 + M * 1]));
}

CC_INLINE T det_4x4(const T* m)
{
  T A0 = m[ 0] * m[ 5] - m[ 1] * m[ 4];
  T A1 = m[ 0] * m[ 6] - m[ 2] * m[ 4];
  T A2 = m[ 0] * m[ 7] - m[ 3] * m[ 4];
  T A3 = m[ 1] * m[ 6] - m[ 2] * m[ 5];
  T A4 = m[ 1] * m[ 7] - m[ 3] * m[ 5];
  T A5 = m[ 2] * m[ 7] - m[ 3] * m[ 6];
  T B0 = m[ 8] * m[13] - m[ 9] * m[12];
  T B1 = m[ 8] * m[14] - m[10] * m[12];
  T B2 = m[ 8] * m[15] - m[11] * m[12];
  T B3 = m[ 9] * m[14] - m[10] * m[13];
  T B4 = m[ 9] * m[15] - m[11] * m[13];
  T B5 = m[10] * m[15] - m[11] * m[14];
  return A0 * B5 - A1 * B4 + A2 * B3 + A3 * B2 - A4 * B1 + A5 * B0;
}

CC_INLINE int inverse_2x2(const T* m, T* m_)
{
  enum {M = 2};
  T d = det_2x2(m);

  if (d != (0.0)) {
    d = (1.0) / d;
    m_[0 + M * 0] = m[1 + M * 1] * d;
    m_[1 + M * 1] = m[0 + M * 0] * d;
    m_[1 + M * 0] *= -m[1 + M * 0] * d;
    m_[0 + M * 1] *= -m[0 + M * 1] * d;
  }

  return 0;
}

CC_INLINE int inverse_3x3(const T* m, T* m_)
{
  enum {M = 3};
  T d = det_3x3(m);

  if (d != (0.0)) {
    d = (1.0) / d;
    m_[0 + M * 0] = (m[1 + M * 1] * m[2 + M * 2] - m[1 + M * 2] * m[2 + M * 1]) * d;
    m_[0 + M * 1] = (m[0 + M * 2] * m[2 + M * 1] - m[0 + M * 1] * m[2 + M * 2]) * d;
    m_[0 + M * 2] = (m[0 + M * 1] * m[1 + M * 2] - m[0 + M * 2] * m[1 + M * 1]) * d;

    m_[1 + M * 0] = (m[1 + M * 2] * m[2 + M * 0] - m[1 + M * 0] * m[2 + M * 2]) * d;
    m_[1 + M * 1] = (m[0 + M * 0] * m[2 + M * 2] - m[0 + M * 2] * m[2 + M * 0]) * d;
    m_[1 + M * 2] = (m[0 + M * 2] * m[1 + M * 0] - m[0 + M * 0] * m[1 + M * 2]) * d;

    m_[2 + M * 0] = (m[1 + M * 0] * m[2 + M * 1] - m[1 + M * 1] * m[2 + M * 0]) * d;
    m_[2 + M * 1] = (m[0 + M * 1] * m[2 + M * 0] - m[0 + M * 0] * m[2 + M * 1]) * d;
    m_[2 + M * 2] = (m[0 + M * 0] * m[1 + M * 1] - m[0 + M * 1] * m[1 + M * 0]) * d;
  }

  return 0;
}

CC_INLINE int inverse_4x4(const T* m, T* m_)
{
  T A0 = m[ 0] * m[ 5] - m[ 1] * m[ 4];
  T A1 = m[ 0] * m[ 6] - m[ 2] * m[ 4];
  T A2 = m[ 0] * m[ 7] - m[ 3] * m[ 4];
  T A3 = m[ 1] * m[ 6] - m[ 2] * m[ 5];
  T A4 = m[ 1] * m[ 7] - m[ 3] * m[ 5];
  T A5 = m[ 2] * m[ 7] - m[ 3] * m[ 6];
  T B0 = m[ 8] * m[13] - m[ 9] * m[12];
  T B1 = m[ 8] * m[14] - m[10] * m[12];
  T B2 = m[ 8] * m[15] - m[11] * m[12];
  T B3 = m[ 9] * m[14] - m[10] * m[13];
  T B4 = m[ 9] * m[15] - m[11] * m[13];
  T B5 = m[10] * m[15] - m[11] * m[14];
  T d = A0 * B5 - A1 * B4 + A2 * B3 + A3 * B2 - A4 * B1 + A5 * B0;

  if (d_not_equal(d, (0.0))) {
    d = (1.0) / d;
    m_[ 0] = (+m[ 5] * B5 - m[ 6] * B4 + m[ 7] * B3) * d;
    m_[ 1] = (-m[ 1] * B5 + m[ 2] * B4 - m[ 3] * B3) * d;
    m_[ 2] = (+m[13] * A5 - m[14] * A4 + m[15] * A3) * d;
    m_[ 3] = (-m[ 9] * A5 + m[10] * A4 - m[11] * A3) * d;
    m_[ 4] = (-m[ 4] * B5 + m[ 6] * B2 - m[ 7] * B1) * d;
    m_[ 5] = (+m[ 0] * B5 - m[ 2] * B2 + m[ 3] * B1) * d;
    m_[ 6] = (-m[12] * A5 + m[14] * A2 - m[15] * A1) * d;
    m_[ 7] = (+m[ 8] * A5 - m[10] * A2 + m[11] * A1) * d;
    m_[ 8] = (+m[ 4] * B4 - m[ 5] * B2 + m[ 7] * B0) * d;
    m_[ 9] = (-m[ 0] * B4 + m[ 1] * B2 - m[ 3] * B0) * d;
    m_[10] = (+m[12] * A4 - m[13] * A2 + m[15] * A0) * d;
    m_[11] = (-m[ 8] * A4 + m[ 9] * A2 - m[11] * A0) * d;
    m_[12] = (-m[ 4] * B3 + m[ 5] * B1 - m[ 6] * B0) * d;
    m_[13] = (+m[ 0] * B3 - m[ 1] * B1 + m[ 2] * B0) * d;
    m_[14] = (-m[12] * A3 + m[13] * A1 - m[14] * A0) * d;
    m_[15] = (+m[ 8] * A3 - m[ 9] * A1 + m[10] * A0) * d;
  }

  return 0;
}

CC_INLINE void eigenvalues_2x2(const T* m, T* eigenvalue)
{
  enum {M = 2};
  T delta = sqrt(sqr(m[0 + M * 0] - m[1 + M * 1]) + (4.0) * m[1 + M * 0] * m[0 + M * 1]);
  eigenvalue[0] = (0.5) * (m[0 + M * 0] + m[1 + M * 1] + delta);
  eigenvalue[1] = (0.5) * (m[0 + M * 0] + m[1 + M * 1] - delta);
}

CC_INLINE void eigenvector_2x2(const T* m, DVECTOR2* eigenvector)
{
  enum {M = 2};
  T eigenvalue[M];
  eigenvalues_2x2(m, eigenvalue);
  eigenvector[0] = v2_normalize(d2_make_vector((-1.0) * m[1 + M * 0], m[0 + M * 0] - eigenvalue[0]));
  eigenvector[1] = v2_normalize(d2_make_vector((-1.0) * m[1 + M * 0], m[0 + M * 0] - eigenvalue[1]));
}

