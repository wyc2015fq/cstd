
bool decompose_win32_glyph_outline1(const char* gbuf, unsigned total_size, bool flip_y)
{
  const char* cur_glyph = gbuf;
  const char* end_glyph = gbuf + total_size;
#if 0
  double x, y;
  while(cur_glyph < end_glyph)
  {
    const TTPOLYGONHEADER* th = (TTPOLYGONHEADER*)cur_glyph;
    
    const char* end_poly = cur_glyph + th->cb;
    const char* cur_poly = cur_glyph + sizeof(TTPOLYGONHEADER);
    
    x = fx_to_dbl(th->pfxStart.x);
    y = fx_to_dbl(th->pfxStart.y);
    if(flip_y) y = -y;
    //path.move_to(value_type(dbl_to_int26p6(x)), value_type(dbl_to_int26p6(y)));
    
    while(cur_poly < end_poly)
    {
      const TTPOLYCURVE* pc = (const TTPOLYCURVE*)cur_poly;
      
      if (pc->wType == TT_PRIM_LINE)
      {
        int i;
        for (i = 0; i < pc->cpfx; i++) {
          x = fx_to_dbl(pc->apfx[i].x);
          y = fx_to_dbl(pc->apfx[i].y);
          //path.line_to(value_type(dbl_to_int26p6(x)), value_type(dbl_to_int26p6(y)));
        }
      }
      
      if (pc->wType == TT_PRIM_QSPLINE)
      {
        int u;
        for (u = 0; u < pc->cpfx - 1; u++) {// Walk through points in spline
          double x2, y2;
          POINTFX pnt_b = pc->apfx[u];    // B is always the current point
          POINTFX pnt_c = pc->apfx[u+1];
          
          if (u < pc->cpfx - 2) { // If not on last spline, compute C
            // midpoint (x,y)
            *(int*)&pnt_c.x = (*(int*)&pnt_b.x + *(int*)&pnt_c.x) / 2;
            *(int*)&pnt_c.y = (*(int*)&pnt_b.y + *(int*)&pnt_c.y) / 2;
          }
          
          x  = fx_to_dbl(pnt_b.x);
          y  = fx_to_dbl(pnt_b.y);
          x2 = fx_to_dbl(pnt_c.x);
          y2 = fx_to_dbl(pnt_c.y);
          //if(flip_y) { y = -y; y2 = -y2; }
          //mtx.transform(&x,  &y);
          //mtx.transform(&x2, &y2);
          //path.curve3(value_type(dbl_to_int26p6(x)), value_type(dbl_to_int26p6(y)), value_type(dbl_to_int26p6(x2)), value_type(dbl_to_int26p6(y2)));
        }
      }
      cur_poly += sizeof(WORD) * 2 + sizeof(POINTFX) * pc->cpfx;
    }
    cur_glyph += th->cb;
  }
#endif
  return true;
}

ushort hz[] = 
#define HANZIDEF(ID, CODE, C, F, R) L#CODE
#include "font/hanzi_frequency.inl"
#undef HANZIDEF
;


#include "path.inl"
#include "math/rand.inl"
//#include "draw2/graphicspath.inl"

int test_sysfont() {
  img_t im[1] = {0};
  ttffont_t f[1] = {0};
  char obuf[1<<12];
  Path path[1] = {0};
  int i, len=0, n=0;
  float matrix[6];
  int sdf = wcslen(L"g");
  char str[1<<13] =  "叉";
  const char* fontname = NULL;
  int fontstyle;
  fontname = "微软雅黑";
  fontname = "宋体";
  fontname = "Times New Roman";
  randstr_cn(NULL, str, countof(str)-1);
  randstr(NULL, str, countof(str)-1, NULL);
  fontstyle = FontStyleBold;
  fontstyle = 0;
  sys_font_set(f, fontname, fontstyle);
  //len = ttffont_get_glyph(f, *L"叉", obuf, countof(obuf));
  M23Set(matrix);
  M23Translate(matrix, 0, 200, MatrixOrderPrepend);
  M23Scale(matrix, 4, 4, MatrixOrderPrepend);
  //M23Shear(matrix, 0.2, 0, MatrixOrderPrepend);
  //M23Rotate(matrix, -20, MatrixOrderPrepend);
  imsetsize(im, 800, 1200, 4, 1);
  if (1) {
    ttffont_get_glyph_pathW(f, *L"G", matrix, path);
  } else {
    PathSetSize(path, 100000);
    tic;
    PathAddStringA(path, str, -1, f, im->w, 20/f->info->size, 0, 0, 0, NULL, NULL);
    toc;
  }
  if (0) {
    for (i=0; i<countof(hz); ++i) {
      // L"齉"
      len = ttffont_get_glyph(f, hz[i], obuf, countof(obuf));
      n += len;
    }
  }
  if (1) {
    if (0) {
      //创建画板从Paint事件中的直接引用Graphics对象
      path->Count = 0;
      //定义画笔
      PathAddBezier(path, 100, 100, 200, 400, 400, 400, 500, 100);
      PathCloseFigure(path);
    }
    if (1) {
      for (i=0; i<path->Count; ++i) {
        printf("%3d %3d %5.1f %5.1f\n", i, path->Types[i]&7, path->Points[i].x, path->Points[i].y);
      }
      tic;
      PathFlatten1(path, NULL, 0.1);
      spanv_t sp[1] = {0};
      //imdrawaa_poly_solid(im, 0, NULL, path->Points, len, j, ColorWhite);
      spanv_set_path(sp, im, NULL, path->Points, path->Types, path->Count);
      //spanv_hatch(sp, im, 0, 0, 0, ColorRed, ColorYellow, g_hatch_datas[HatchStyleDiagonalCross]);
      spanv_solid(sp, im, 0, ColorWhite);
      spanv_free(sp);
      toc;
    }
    tictoc(NULL);
    imshow(im);cvWaitKey(-1);
  }
  PathFree(path);
  ttffont_del(f);
  return 0;
}
