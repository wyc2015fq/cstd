#include "clipper/clipper.inl"
enum clipper_op_e { clipper_or, clipper_and, clipper_xor, clipper_a_minus_b, clipper_b_minus_a };
enum clipper_PolyFillType {clipper_even_odd, clipper_non_zero, clipper_positive, clipper_negative};
int poly2d_to_paths(const poly2d_t* src, Paths& p, double m_scaling_factor)
{
  int i, j, npt;
  IntPoint v;
  const DPOINT* pt = src->pt;
  p.resize(src->n);
  for (i = 0; i < src->n; ++i) {
    npt = src->len[i];
    p[i].resize(npt);
    for (j = 0; j < npt; ++j) {
      v.X = Round(pt[j].x * m_scaling_factor);
      v.Y = Round(pt[j].y * m_scaling_factor);
      p[i][j] = v;
    }
    pt += npt;
  }
  return 0;
}
int paths_to_poly2d(const Paths& p, poly2d_t* src, double m_scaling_factor)
{
  int i, j, npt = 0, n = p.size();
  IntPoint v;
  DPOINT* pt;
  for (i = 0; i < n; ++i) {
    npt += p[i].size();
  }
  poly2d_setsize(src, n, npt);
  pt = src->pt;
  for (i = 0; i < n; ++i) {
    src->len[i] = npt = p[i].size();
    for (j = 0; j < npt; ++j) {
      v = p[i][j];
      pt[j].x = v.X / m_scaling_factor;
      pt[j].y = v.Y / m_scaling_factor;
    }
    pt += npt;
  }
  return 0;
}
int conv_clipper(poly2d_t* m_src_c, const poly2d_t* m_src_a, const poly2d_t* m_src_b, int m_operation,
    clipper_PolyFillType m_subjFillType, clipper_PolyFillType m_clipFillType)
{
  double m_scaling_factor = 2;
  Clipper m_clipper;
  Paths m_poly_a, m_poly_b, m_result;
  poly2d_to_paths(m_src_a , m_poly_a, m_scaling_factor);
  poly2d_to_paths(m_src_b , m_poly_b, m_scaling_factor);
  m_result.resize(0);
  PolyFillType pftSubj, pftClip;
  switch (m_subjFillType) {
  case clipper_even_odd:
    pftSubj = pftEvenOdd;
    break;
  case clipper_non_zero:
    pftSubj = pftNonZero;
    break;
  case clipper_positive:
    pftSubj = pftPositive;
    break;
  default:
    pftSubj = pftNegative;
  }
  switch (m_clipFillType) {
  case clipper_even_odd:
    pftClip = pftEvenOdd;
    break;
  case clipper_non_zero:
    pftClip = pftNonZero;
    break;
  case clipper_positive:
    pftClip = pftPositive;
    break;
  default:
    pftClip = pftNegative;
  }
  m_clipper.Clear();
  switch (m_operation) {
  case clipper_or:
    m_clipper.AddPaths(m_poly_a , ptSubject, true);
    m_clipper.AddPaths(m_poly_b , ptClip, true);
    m_clipper.Execute(ctUnion , m_result , pftSubj, pftClip);
    break;
  case clipper_and:
    m_clipper.AddPaths(m_poly_a , ptSubject, true);
    m_clipper.AddPaths(m_poly_b , ptClip, true);
    m_clipper.Execute(ctIntersection , m_result, pftSubj, pftClip);
    break;
  case clipper_xor:
    m_clipper.AddPaths(m_poly_a , ptSubject, true);
    m_clipper.AddPaths(m_poly_b , ptClip, true);
    m_clipper.Execute(ctXor , m_result, pftSubj, pftClip);
    break;
  case clipper_a_minus_b:
    m_clipper.AddPaths(m_poly_a , ptSubject, true);
    m_clipper.AddPaths(m_poly_b , ptClip, true);
    m_clipper.Execute(ctDifference , m_result, pftSubj, pftClip);
    break;
  case clipper_b_minus_a:
    m_clipper.AddPaths(m_poly_b , ptSubject, true);
    m_clipper.AddPaths(m_poly_a , ptClip, true);
    m_clipper.Execute(ctDifference , m_result, pftSubj, pftClip);
    break;
  }
  paths_to_poly2d(m_result, m_src_c, m_scaling_factor);
  return 0;
}

