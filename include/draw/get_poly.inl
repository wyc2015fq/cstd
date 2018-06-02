static int vcgen_spiral(FPOINT* Points, int ipos, int n, double x, double y, double r1, double r2, double start_angle)
{
  int i;
  double curr_r = r1, angle = (start_angle);
  double da = (deg2rad(4.0));
  double dr = (r2 - r1) / n;
  if (Points) {
    FPOINT* pt = Points + ipos;
    for (i = 0; i < n; ++i) {
      pt[i].x = x + cos(angle) * curr_r;
      pt[i].y = y + sin(angle) * curr_r;
      curr_r += dr;
      angle += da;
    }
  }
  return ipos + n;
}
static int dPOLYGON_print(const DPOLYGON* s)
{
  int i, j, l;
  const DPOINT* pt = s->pt;
  for (i = 0; i < s->n; ++i) {
    l = s->len[i];
    printf("%d: %d\n", i, l);
    for (j = 0; j < l; ++j) {
      printf(" %d:[%g, %g]\n", j, pt[j].x, pt[j].y);
    }
    pt += l;
  }
  return 0;
}

#define GET_POLY_DEF_DEF(GET_POLY_DEF) \
  GET_POLY_DEF(Null) \
  GET_POLY_DEF(Path_0) \
  GET_POLY_DEF(Path_1) \
  GET_POLY_DEF(Rect) \
  GET_POLY_DEF(Arrows) \
  GET_POLY_DEF(Britain) \
  GET_POLY_DEF(Spiral) \
  GET_POLY_DEF(Glyph_a)

typedef enum {
#define GET_POLY_DEF(name) gp_##name,
  GET_POLY_DEF_DEF(GET_POLY_DEF)
#undef GET_POLY_DEF
} poly_id_t;
static const char* s_get_poly = "|"
#define GET_POLY_DEF(name) #name "|"
    GET_POLY_DEF_DEF(GET_POLY_DEF)
#undef GET_POLY_DEF
    ;

Status PathAddPathPoints(Path* path, const path_point* pp, int npp) {
  int old_count = path->Count;
  PathSetSize(path, old_count + npp);
  path->Count = vcgen_path_point(path->Points, path->Types, old_count, pp, npp);
  return Ok;
}

static int get_poly(poly_id_t id, Path* path)
{
  path->Count = 0;
  switch (id) {
  case gp_Null:
    break;
  case gp_Path_0:
    if (path) {
      // Two simple paths
      path_point ps1[] = {
        m_path_moveto(140, 145),
        m_path_lineto(225, 44),
        m_path_linetoc(296, 219),
        m_path_lineto(226, 289),
        m_path_lineto(82, 292),
        m_path_moveto(220, 222),
        m_path_lineto(363, 249),
        m_path_lineto(265, 331),
        m_path_moveto(242, 243),
        m_path_lineto(268, 309),
        m_path_lineto(325, 261),
        m_path_moveto(259, 259),
        m_path_lineto(273, 288),
        m_path_lineto(298, 266),
      };
      PathAddPathPoints(path, ps1, countof(ps1));
    }
    break;
  case gp_Path_1:
    if (path) {
      path_point ps1[] = {
        m_path_moveto(32, 77),
        m_path_lineto(473, 263),
        m_path_lineto(351, 290),
        m_path_linetoc(354, 374),
      };
      PathAddPathPoints(path, ps1, countof(ps1));
    }
    break;
  case gp_Arrows:
    if (path) {
      // Great Britain and arrows
      path_point ps1[] = {
#if 1
        m_path_moveto(1268.200000000000045, 1266.799999999999955),
          m_path_lineto(1315.000000000000000, 1266.799999999999955),
          m_path_lineto(1315.000000000000000, 1220.000000000000000),
          m_path_lineto(1299.400000000000091, 1235.599999999999909),
          m_path_lineto(1283.799999999999955, 1204.399999999999864),
          m_path_lineto(1252.599999999999909, 1235.599999999999909),
          m_path_linetoc(1283.799999999999955, 1251.200000000000045),
#else
          m_path_moveto(1330.599999999999909, 1282.399999999999864),
          m_path_lineto(1377.400000000000091, 1282.399999999999864),
          m_path_lineto(1361.799999999999955, 1298.000000000000000),
          m_path_lineto(1393.000000000000000, 1313.599999999999909),
          m_path_lineto(1361.799999999999955, 1344.799999999999955),
          m_path_lineto(1346.200000000000045, 1313.599999999999909),
          m_path_linetoc(1330.599999999999909, 1329.200000000000045),
          
          m_path_moveto(1330.599999999999909, 1266.799999999999955),
          m_path_lineto(1377.400000000000091, 1266.799999999999955),
          m_path_lineto(1361.799999999999955, 1251.200000000000045),
          m_path_lineto(1393.000000000000000, 1235.599999999999909),
          m_path_lineto(1361.799999999999955, 1204.399999999999864),
          m_path_lineto(1346.200000000000045, 1235.599999999999909),
          m_path_linetoc(1330.599999999999909, 1220.000000000000000),
          
          m_path_moveto(1315.000000000000000, 1282.399999999999864),
          m_path_lineto(1315.000000000000000, 1329.200000000000045),
          m_path_lineto(1299.400000000000091, 1313.599999999999909),
          m_path_lineto(1283.799999999999955, 1344.799999999999955),
          m_path_lineto(1252.599999999999909, 1313.599999999999909),
          m_path_lineto(1283.799999999999955, 1298.000000000000000),
          m_path_linetoc(1268.200000000000045, 1282.399999999999864),
          
          m_path_moveto(1268.200000000000045, 1266.799999999999955),
          m_path_lineto(1315.000000000000000, 1266.799999999999955),
          m_path_lineto(1315.000000000000000, 1220.000000000000000),
          m_path_lineto(1299.400000000000091, 1235.599999999999909),
          m_path_lineto(1283.799999999999955, 1204.399999999999864),
          m_path_lineto(1252.599999999999909, 1235.599999999999909),
          m_path_linetoc(1283.799999999999955, 1251.200000000000045),
          
#endif
      };
      affine_info_t ai1[] = {GEO_TRANS, -1150, -1150, GEO_SCALING, 2, 2};
      PathAddPathPoints(path, ps1, countof(ps1));
      pt_trans_info(path->Count, path->Points, countof(ai1), ai1);
    }
    break;
  case gp_Britain:
    if (path) {
      static const FPOINT get_poly_britain[] = {
#include "get_poly_britain.txt"
      };
      int len[2] = {702, countof(get_poly_britain) - 702};
      PathAddPolygonX(path, get_poly_britain, len, 2, GC_FLOAT, sizeof(*get_poly_britain));
    }
    break;
  case gp_Spiral:
    if (path) {
      int n = vcgen_spiral(NULL, path->Count, 30, 0, 0, 150, 10, 30);
      PathSetSize(path, n);
      path->Count = vcgen_spiral(path->Points, path->Count, 30, 0, 0, 150, 10, 30);
    }
    break;
  case gp_Glyph_a:
    if (path) {
      path_point ps1[] = {
#if 1
        m_path_moveto(28.47, 6.45),
        m_path_curve3(21.58, 1.12, 19.82, 0.29),
        m_path_curve3(17.19, -0.93, 14.21, -0.93),
        m_path_curve3(9.57, -0.93, 6.57, 2.25),
        m_path_curve3(3.56, 5.42, 3.56, 10.60),
        m_path_curve3(3.56, 13.87, 5.03, 16.26),
        m_path_curve3(7.03, 19.58, 11.99, 22.51),
        m_path_curve3(16.94, 25.44, 28.47, 29.64),
        m_path_lineto(28.47, 31.40),
        m_path_curve3(28.47, 38.09, 26.34, 40.58),
        m_path_curve3(24.22, 43.07, 20.17, 43.07),
        m_path_curve3(17.09, 43.07, 15.28, 41.41),
        m_path_curve3(13.43, 39.75, 13.43, 37.60),
        m_path_lineto(13.53, 34.77),
        m_path_curve3(13.53, 32.52, 12.38, 31.30),
        m_path_curve3(11.23, 30.08, 9.38, 30.08),
        m_path_curve3(7.57, 30.08, 6.42, 31.35),
        m_path_curve3(5.27, 32.62, 5.27, 34.81),
        m_path_curve3(5.27, 39.01, 9.57, 42.53),
        m_path_curve3(13.87, 46.04, 21.63, 46.04),
        m_path_curve3(27.59, 46.04, 31.40, 44.04),
        m_path_curve3(34.28, 42.53, 35.64, 39.31),
        m_path_curve3(36.52, 37.21, 36.52, 30.71),
        m_path_lineto(36.52, 15.53),
        m_path_curve3(36.52, 9.13, 36.77, 7.69),
        m_path_curve3(37.01, 6.25, 37.57, 5.76),
        m_path_curve3(38.13, 5.27, 38.87, 5.27),
        m_path_curve3(39.65, 5.27, 40.23, 5.62),
        m_path_curve3(41.26, 6.25, 44.19, 9.18),
        m_path_lineto(44.19, 6.45),
        m_path_curve3(38.72, -0.88, 33.74, -0.88),
        m_path_curve3(31.35, -0.88, 29.93, 0.78),
        m_path_curve3c(28.52, 2.44, 28.47, 6.45),
        m_path_moveto(28.47, 9.62),
        m_path_lineto(28.47, 26.66),
        m_path_curve3(21.09, 23.73, 18.95, 22.51),
        m_path_curve3(15.09, 20.36, 13.43, 18.02),
        m_path_curve3(11.77, 15.67, 11.77, 12.89),
        m_path_curve3(11.77, 9.38, 13.87, 7.06),
        m_path_curve3(15.97, 4.74, 18.70, 4.74),
        m_path_curve3c(22.41, 4.74, 28.47, 9.62),
#else
#endif
      };
      PathAddPathPoints(path, ps1, countof(ps1));
    }
    break;
  case gp_Rect:
    if (path) {
      path_point ps1[] = {
        m_path_moveto(0, 0),
        m_path_lineto(4, 0),
        m_path_lineto(4, 4),
        m_path_linetoc(0, 4),
        m_path_moveto(1, 1),
        m_path_lineto(1, 3),
        m_path_lineto(3, 3),
        m_path_linetoc(3, 1),
      };
      PathAddPathPoints(path, ps1, countof(ps1));
    }
    break;
  }
  return path->Count;
}
