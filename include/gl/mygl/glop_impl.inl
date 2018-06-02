#define gl_malloc malloc
#define gl_free free
void gl_fatal_error(char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  fprintf(stderr, "TinyGL: fatal error: ");
  vfprintf(stderr, format, ap);
  fprintf(stderr, "\n");
  exit(1);
  va_end(ap);
}
#define NDEBUG
#ifdef NDEBUG
#define NO_DEBUG_OUTPUT
#endif
/* Use this function to output messages when something unexpected
   happens (which might be an indication of an error). *Don't* use it
   when there's internal errors in the code - these should be handled
   by asserts. */
void tgl_warning(const char* format, ...)
{
#ifndef NO_DEBUG_OUTPUT
  va_list args;
  va_start(args, format);
  fprintf(stderr, "*WARNING* ");
  vfprintf(stderr, format, args);
  va_end(args);
#endif /* !NO_DEBUG_OUTPUT */
}
/* This function should be used for debug output only. */
void tgl_trace(const char* format, ...)
{
#ifndef NO_DEBUG_OUTPUT
  va_list args;
  va_start(args, format);
  fprintf(stderr, "*DEBUG* ");
  vfprintf(stderr, format, args);
  va_end(args);
#endif /* !NO_DEBUG_OUTPUT */
}
/* Use this function to output info about things in the code which
   should be fixed (missing handling of special cases, important
   features not implemented, known bugs/buglets, ...). */
void tgl_fixme(const char* format, ...)
{
#ifndef NO_DEBUG_OUTPUT
  va_list args;
  va_start(args, format);
  fprintf(stderr, "*FIXME* ");
  vfprintf(stderr, format, args);
  va_end(args);
#endif /* !NO_DEBUG_OUTPUT */
}
void glopTexCoord(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopEdgeFlag(GLContext* c, GLParam* p)
{
  c->current_edge_flag = p[1].i;
}
void glopNormal(GLContext* c, GLParam* p)
{
  V3 v;
  v.X = p[1].f;
  v.Y = p[2].f;
  v.Z = p[3].f;
  c->current_normal.X = v.X;
  c->current_normal.Y = v.Y;
  c->current_normal.Z = v.Z;
  c->current_normal.W = 0;
}
void gl_eval_viewport(GLContext* c)
{
  GLViewport* v;
  GLfloat zsize = (1 << (ZB_Z_BITS + ZB_POINT_Z_FRAC_BITS));
  v = &c->viewport;
  v->trans.X = (float)((v->w - 0.5) / 2.0) + v->xmin;
  v->trans.Y = (float)((v->h - 0.5) / 2.0) + v->ymin;
  v->trans.Z = (float)((zsize - 0.5) / 2.0) + ((1 << ZB_POINT_Z_FRAC_BITS)) / 2;
  v->scale.X = (float)(v->w - 0.5f) / 2.0f;
  v->scale.Y = -(v->h - 0.5f) / 2.0f;
  v->scale.Z = -((zsize - 0.5f) / 2.0f);
}
#define CLIP_XMIN   (1<<0)
#define CLIP_XMAX   (1<<1)
#define CLIP_YMIN   (1<<2)
#define CLIP_YMAX   (1<<3)
#define CLIP_ZMIN   (1<<4)
#define CLIP_ZMAX   (1<<5)
void gl_transform_to_viewport(GLContext* c, GLVertex* v)
{
  GLfloat winv;
  /* coordinates */
  winv = 1.0f / v->pc.W;
  v->zp.x = (int)(v->pc.X * winv * c->viewport.scale.X + c->viewport.trans.X);
  v->zp.y = (int)(v->pc.Y * winv * c->viewport.scale.Y + c->viewport.trans.Y);
  v->zp.z = (int)(v->pc.Z * winv * c->viewport.scale.Z + c->viewport.trans.Z);
  /* color */
  if (c->lighting_enabled) {
    v->zp.r = (int)(v->color.v[0] * (ZB_POINT_RED_MAX - ZB_POINT_RED_MIN) + ZB_POINT_RED_MIN);
    v->zp.g = (int)(v->color.v[1] * (ZB_POINT_GREEN_MAX - ZB_POINT_GREEN_MIN) + ZB_POINT_GREEN_MIN);
    v->zp.b = (int)(v->color.v[2] * (ZB_POINT_BLUE_MAX - ZB_POINT_BLUE_MIN) + ZB_POINT_BLUE_MIN);
  }
  else {
    /* no need to convert to integer if no lighting : take current color */
    v->zp.r = c->longcurrent_color[0];
    v->zp.g = c->longcurrent_color[1];
    v->zp.b = c->longcurrent_color[2];
  }
  /* texture */
  if (c->texture_2d_enabled) {
    v->zp.s = (int)(v->tex_coord.X * (ZB_POINT_S_MAX - ZB_POINT_S_MIN) + ZB_POINT_S_MIN);
    v->zp.t = (int)(v->tex_coord.Y * (ZB_POINT_T_MAX - ZB_POINT_T_MIN) + ZB_POINT_T_MIN);
  }
}
int glRenderMode(int mode)
{
  GLContext* c = gl_get_context();
  int result = 0;
  switch (c->render_mode) {
  case GL_RENDER:
    break;
  case GL_SELECT:
    if (c->select_overflow) {
      result = -c->select_hits;
    }
    else {
      result = c->select_hits;
    }
    c->select_overflow = 0;
    c->select_ptr = c->select_buffer;
    c->name_stack_size = 0;
    break;
  default:
    assert(0);
  }
  switch (mode) {
  case GL_RENDER:
    c->render_mode = GL_RENDER;
    break;
  case GL_SELECT:
    c->render_mode = GL_SELECT;
    assert(c->select_buffer != NULL);
    c->select_ptr = c->select_buffer;
    c->select_hits = 0;
    c->select_overflow = 0;
    c->select_hit = NULL;
    break;
  default:
    assert(0);
  }
  return result;
}
void APIENTRY glSelectBuffer(int size, unsigned int* buf)
{
  GLContext* c = gl_get_context();
  assert(c->render_mode != GL_SELECT);
  c->select_buffer = buf;
  c->select_size = size;
}
void glopInitNames(GLContext* c, GLParam* p)
{
  if (c->render_mode == GL_SELECT) {
    c->name_stack_size = 0;
    c->select_hit = NULL;
  }
}
void glopPushName(GLContext* c, GLParam* p)
{
  if (c->render_mode == GL_SELECT) {
    assert(c->name_stack_size < MAX_NAME_STACK_DEPTH);
    c->name_stack[c->name_stack_size++] = p[1].i;
    c->select_hit = NULL;
  }
}
void glopPopName(GLContext* c, GLParam* p)
{
  if (c->render_mode == GL_SELECT) {
    assert(c->name_stack_size > 0);
    c->name_stack_size--;
    c->select_hit = NULL;
  }
}
void glopLoadName(GLContext* c, GLParam* p)
{
  if (c->render_mode == GL_SELECT) {
    assert(c->name_stack_size > 0);
    c->name_stack[c->name_stack_size - 1] = p[1].i;
    c->select_hit = NULL;
  }
}
void gl_add_select(GLContext* c, unsigned int zmin, unsigned int zmax)
{
  unsigned int* ptr;
  int n, i;
  if (!c->select_overflow) {
    if (c->select_hit == NULL) {
      n = c->name_stack_size;
      if ((c->select_ptr - c->select_buffer + 3 + n) >
          c->select_size) {
        c->select_overflow = 1;
      }
      else {
        ptr = c->select_ptr;
        c->select_hit = ptr;
        *ptr++ = c->name_stack_size;
        *ptr++ = zmin;
        *ptr++ = zmax;
        for (i = 0; i < n; i++) {
          *ptr++ = c->name_stack[i];
        }
        c->select_ptr = ptr;
        c->select_hits++;
      }
    }
    else {
      if (zmin < c->select_hit[1]) {
        c->select_hit[1] = zmin;
      }
      if (zmax > c->select_hit[2]) {
        c->select_hit[2] = zmax;
      }
    }
  }
}
static void gl_add_select1(GLContext* c, int z1, int z2, int z3)
{
  int min, max;
  min = max = z1;
  if (z2 < min) {
    min = z2;
  }
  if (z3 < min) {
    min = z3;
  }
  if (z2 > max) {
    max = z2;
  }
  if (z3 > max) {
    max = z3;
  }
  gl_add_select(c, 0xffffffff - min, 0xffffffff - max);
}
#define ZCMP(z,zpix) ((z) >= (zpix))
void ZB_plot(ZBuffer* zb, ZBufferPoint* p, int point_size)
{
  int zz;
  switch (zb->cn) {
  case 4: {
#define PUT_PIX()  RGB_TO_PIXEL32(pp, p->r, p->g, p->b);
#include "zplot_impl.inl"
  }
  break;
  case 3: {
    int r = p->r >> 8, g = p->g >> 8, b = p->b >> 8;
#define PUT_PIX()  RGB_TO_PIXEL24(pp, r, g, b)
#include "zplot_impl.inl"
  }
  break;
  case 2:
#define PUT_PIX()  RGB_TO_PIXEL16(pp, p->r, p->g, p->b);
#include "zplot_impl.inl"
    break;
  }
}
static void ZB_line_flat_z(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2, int color)
{
#define INTERP_Z
#include "zline_impl_cn.inl"
}
/* line with color interpolation */
static void ZB_line_interp_z(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2)
{
#define INTERP_Z
#define INTERP_RGB
#include "zline_impl_cn.inl"
}
/* no Z interpolation */
static void ZB_line_flat(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2, int color)
{
#include "zline_impl_cn.inl"
}
#define INTERP_RGB
static void ZB_line_interp(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2)
{
#include "zline_impl_cn.inl"
}
void ZB_line_z(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2)
{
  int color1, color2;
  RGB_TO_PIXEL32(&color1, p1->r, p1->g, p1->b);
  RGB_TO_PIXEL32(&color2, p2->r, p2->g, p2->b);
  /* choose if the line should have its color interpolated or not */
  if (color1 == color2) {
    ZB_line_flat_z(zb, p1, p2, color1);
  }
  else {
    ZB_line_interp_z(zb, p1, p2);
  }
}
void ZB_line(ZBuffer* zb, ZBufferPoint* p1, ZBufferPoint* p2)
{
  int color1, color2;
  RGB_TO_PIXEL32(&color1, p1->r, p1->g, p1->b);
  RGB_TO_PIXEL32(&color2, p2->r, p2->g, p2->b);
  /* choose if the line should have its color interpolated or not */
  if (color1 == color2) {
    ZB_line_flat(zb, p1, p2, color1);
  }
  else {
    ZB_line_interp(zb, p1, p2);
  }
}
/* point */
void gl_draw_point(GLContext* c, GLVertex* p0)
{
  if (p0->clip_code == 0) {
    if (c->render_mode == GL_SELECT) {
      gl_add_select(c, p0->zp.z, p0->zp.z);
    }
    else {
      ZB_plot(c->zb, &p0->zp, c->point_size);
    }
  }
}
/* line */
static CC_INLINE void interpolate(GLVertex* q, GLVertex* p0, GLVertex* p1, GLfloat t)
{
  q->pc.X = p0->pc.X + (p1->pc.X - p0->pc.X) * t;
  q->pc.Y = p0->pc.Y + (p1->pc.Y - p0->pc.Y) * t;
  q->pc.Z = p0->pc.Z + (p1->pc.Z - p0->pc.Z) * t;
  q->pc.W = p0->pc.W + (p1->pc.W - p0->pc.W) * t;
  q->color.v[0] = p0->color.v[0] + (p1->color.v[0] - p0->color.v[0]) * t;
  q->color.v[1] = p0->color.v[1] + (p1->color.v[1] - p0->color.v[1]) * t;
  q->color.v[2] = p0->color.v[2] + (p1->color.v[2] - p0->color.v[2]) * t;
}
/*
 * Line Clipping
 */
/* Line Clipping algorithm from 'Computer Graphics', Principles and
   Practice */
static CC_INLINE int ClipLine1(GLfloat denom, GLfloat num, GLfloat* tmin, GLfloat* tmax)
{
  GLfloat t;
  if (denom > 0) {
    t = num / denom;
    if (t > *tmax) {
      return 0;
    }
    if (t > *tmin) {
      *tmin = t;
    }
  }
  else if (denom < 0) {
    t = num / denom;
    if (t < *tmin) {
      return 0;
    }
    if (t < *tmax) {
      *tmax = t;
    }
  }
  else if (num > 0) {
    return 0;
  }
  return 1;
}
void gl_draw_line(GLContext* c, GLVertex* p1, GLVertex* p2)
{
  GLfloat dx, dy, dz, dw, x1, y1, z1, w1;
  GLfloat tmin, tmax;
  GLVertex q1, q2;
  int cc1, cc2;
  cc1 = p1->clip_code;
  cc2 = p2->clip_code;
  if ((cc1 | cc2) == 0) {
    if (c->render_mode == GL_SELECT) {
      gl_add_select1(c, p1->zp.z, p2->zp.z, p2->zp.z);
    }
    else {
      if (c->depth_test) {
        ZB_line_z(c->zb, &p1->zp, &p2->zp);
      }
      else {
        ZB_line(c->zb, &p1->zp, &p2->zp);
      }
    }
  }
  else if ((cc1 & cc2) != 0) {
    return;
  }
  else {
    dx = p2->pc.X - p1->pc.X;
    dy = p2->pc.Y - p1->pc.Y;
    dz = p2->pc.Z - p1->pc.Z;
    dw = p2->pc.W - p1->pc.W;
    x1 = p1->pc.X;
    y1 = p1->pc.Y;
    z1 = p1->pc.Z;
    w1 = p1->pc.W;
    tmin = 0;
    tmax = 1;
    if (ClipLine1(dx + dw, -x1 - w1, &tmin, &tmax) &&
        ClipLine1(-dx + dw, x1 - w1, &tmin, &tmax) &&
        ClipLine1(dy + dw, -y1 - w1, &tmin, &tmax) &&
        ClipLine1(-dy + dw, y1 - w1, &tmin, &tmax) &&
        ClipLine1(dz + dw, -z1 - w1, &tmin, &tmax) &&
        ClipLine1(-dz + dw, z1 - w1, &tmin, &tmax)) {
      interpolate(&q1, p1, p2, tmin);
      interpolate(&q2, p1, p2, tmax);
      gl_transform_to_viewport(c, &q1);
      gl_transform_to_viewport(c, &q2);
      if (c->depth_test) {
        ZB_line_z(c->zb, &q1.zp, &q2.zp);
      }
      else {
        ZB_line(c->zb, &q1.zp, &q2.zp);
      }
    }
  }
}
/* triangle */
/*
 * Clipping
 */
/* We clip the segment [a,b] against the 6 planes of the normal volume.
 * We compute the point 'c' of intersection and the value of the parameter 't'
 * of the intersection if x=a+t(b-a).
 */
#define clip_func(name,sign,dir,dir1,dir2) \
  static GLfloat name(V4 *c,V4 *a,V4 *b) \
  {\
    GLfloat t,dX,dY,dZ,dW,den;\
    dX = (b->X - a->X);\
    dY = (b->Y - a->Y);\
    dZ = (b->Z - a->Z);\
    dW = (b->W - a->W);\
    den = -(sign d ## dir) + dW;\
    if (den == 0) t=0;\
    else t = ( sign a->dir - a->W) / den;\
    c->dir1 = a->dir1 + t * d ## dir1;\
    c->dir2 = a->dir2 + t * d ## dir2;\
    c->W = a->W + t * dW;\
    c->dir = sign c->W;\
    return t;\
  }
clip_func(clip_xmin, -, X, Y, Z)
clip_func(clip_xmax, +, X, Y, Z)
clip_func(clip_ymin, -, Y, X, Z)
clip_func(clip_ymax, +, Y, X, Z)
clip_func(clip_zmin, -, Z, X, Y)
clip_func(clip_zmax, +, Z, X, Y)
GLfloat(*clip_proc[6])(V4*, V4*, V4*) =  {
  clip_xmin, clip_xmax,
  clip_ymin, clip_ymax,
  clip_zmin, clip_zmax
};
static CC_INLINE void updateTmp(GLContext* c, GLVertex* q, GLVertex* p0, GLVertex* p1, GLfloat t)
{
  if (c->current_shade_model == GL_SMOOTH) {
    q->color.v[0] = p0->color.v[0] + (p1->color.v[0] - p0->color.v[0]) * t;
    q->color.v[1] = p0->color.v[1] + (p1->color.v[1] - p0->color.v[1]) * t;
    q->color.v[2] = p0->color.v[2] + (p1->color.v[2] - p0->color.v[2]) * t;
  }
  else {
    q->color.v[0] = p0->color.v[0];
    q->color.v[1] = p0->color.v[1];
    q->color.v[2] = p0->color.v[2];
  }
  if (c->texture_2d_enabled) {
    q->tex_coord.X = p0->tex_coord.X + (p1->tex_coord.X - p0->tex_coord.X) * t;
    q->tex_coord.Y = p0->tex_coord.Y + (p1->tex_coord.Y - p0->tex_coord.Y) * t;
  }
  q->clip_code = gl_clipcode(q->pc.X, q->pc.Y, q->pc.Z, q->pc.W);
  if (q->clip_code == 0) {
    gl_transform_to_viewport(c, q);
  }
}
static void gl_draw_triangle_clip(GLContext* c,
    GLVertex* p0, GLVertex* p1, GLVertex* p2, int clip_bit);
void gl_draw_triangle(GLContext* c, GLVertex* p0, GLVertex* p1, GLVertex* p2)
{
  int co, c_and, cc[3], front;
  GLfloat norm;
  cc[0] = p0->clip_code;
  cc[1] = p1->clip_code;
  cc[2] = p2->clip_code;
  co = cc[0] | cc[1] | cc[2];
  /* we handle the non clipped case here to go faster */
  if (co == 0) {
    norm = (GLfloat)(p1->zp.x - p0->zp.x) * (GLfloat)(p2->zp.y - p0->zp.y) -
        (GLfloat)(p2->zp.x - p0->zp.x) * (GLfloat)(p1->zp.y - p0->zp.y);
    if (norm == 0) {
      return;
    }
    front = norm < 0.0;
    front = front ^ c->current_front_face;
    /* back face culling */
    if (c->cull_face_enabled) {
      /* most used case first */
      if (c->current_cull_face == GL_BACK) {
        if (front == 0) {
          return;
        }
        c->draw_triangle_front(c, p0, p1, p2);
      }
      else if (c->current_cull_face == GL_FRONT) {
        if (front != 0) {
          return;
        }
        c->draw_triangle_back(c, p0, p1, p2);
      }
      else {
        return;
      }
    }
    else {
      /* no culling */
      if (front) {
        c->draw_triangle_front(c, p0, p1, p2);
      }
      else {
        c->draw_triangle_back(c, p0, p1, p2);
      }
    }
  }
  else {
    c_and = cc[0] & cc[1] & cc[2];
    if (c_and == 0) {
      gl_draw_triangle_clip(c, p0, p1, p2, 0);
    }
  }
}
static void gl_draw_triangle_clip(GLContext* c, GLVertex* p0, GLVertex* p1, GLVertex* p2, int clip_bit)
{
  int co, c_and, co1, cc[3], edge_flag_tmp, clip_mask;
  GLVertex tmp1, tmp2, *q[3];
  GLfloat tt;
  cc[0] = p0->clip_code;
  cc[1] = p1->clip_code;
  cc[2] = p2->clip_code;
  co = cc[0] | cc[1] | cc[2];
  if (co == 0) {
    gl_draw_triangle(c, p0, p1, p2);
  }
  else {
    c_and = cc[0] & cc[1] & cc[2];
    /* the triangle is completely outside */
    if (c_and != 0) {
      return;
    }
    /* find the next direction to clip */
    while (clip_bit < 6 && (co & (1 << clip_bit)) == 0)  {
      clip_bit++;
    }
    /* this test can be true only in case of rounding errors */
    if (clip_bit == 6) {
#if 0
      printf("Error:\n");
      printf("%f %f %f %f\n", p0->pc.X, p0->pc.Y, p0->pc.Z, p0->pc.W);
      printf("%f %f %f %f\n", p1->pc.X, p1->pc.Y, p1->pc.Z, p1->pc.W);
      printf("%f %f %f %f\n", p2->pc.X, p2->pc.Y, p2->pc.Z, p2->pc.W);
#endif
      return;
    }
    clip_mask = 1 << clip_bit;
    co1 = (cc[0] ^ cc[1] ^ cc[2]) & clip_mask;
    if (co1)  {
      /* one point outside */
      if (cc[0] & clip_mask) {
        q[0] = p0;
        q[1] = p1;
        q[2] = p2;
      }
      else if (cc[1] & clip_mask) {
        q[0] = p1;
        q[1] = p2;
        q[2] = p0;
      }
      else {
        q[0] = p2;
        q[1] = p0;
        q[2] = p1;
      }
      tt = clip_proc[clip_bit](&tmp1.pc, &q[0]->pc, &q[1]->pc);
      updateTmp(c, &tmp1, q[0], q[1], tt);
      tt = clip_proc[clip_bit](&tmp2.pc, &q[0]->pc, &q[2]->pc);
      updateTmp(c, &tmp2, q[0], q[2], tt);
      tmp1.edge_flag = q[0]->edge_flag;
      edge_flag_tmp = q[2]->edge_flag;
      q[2]->edge_flag = 0;
      gl_draw_triangle_clip(c, &tmp1, q[1], q[2], clip_bit + 1);
      tmp2.edge_flag = 1;
      tmp1.edge_flag = 0;
      q[2]->edge_flag = edge_flag_tmp;
      gl_draw_triangle_clip(c, &tmp2, &tmp1, q[2], clip_bit + 1);
    }
    else {
      /* two points outside */
      if ((cc[0] & clip_mask) == 0) {
        q[0] = p0;
        q[1] = p1;
        q[2] = p2;
      }
      else if ((cc[1] & clip_mask) == 0) {
        q[0] = p1;
        q[1] = p2;
        q[2] = p0;
      }
      else {
        q[0] = p2;
        q[1] = p0;
        q[2] = p1;
      }
      tt = clip_proc[clip_bit](&tmp1.pc, &q[0]->pc, &q[1]->pc);
      updateTmp(c, &tmp1, q[0], q[1], tt);
      tt = clip_proc[clip_bit](&tmp2.pc, &q[0]->pc, &q[2]->pc);
      updateTmp(c, &tmp2, q[0], q[2], tt);
      tmp1.edge_flag = 1;
      tmp2.edge_flag = q[2]->edge_flag;
      gl_draw_triangle_clip(c, q[0], &tmp1, &tmp2, clip_bit + 1);
    }
  }
}
void gl_draw_triangle_select(GLContext* c,
    GLVertex* p0, GLVertex* p1, GLVertex* p2)
{
  gl_add_select1(c, p0->zp.z, p1->zp.z, p2->zp.z);
}
#ifdef PROFILE
int count_triangles, count_triangles_textured, count_pixels;
#endif
#define ZCMP(z,zpix) ((z) >= (zpix))
void ZB_fillTriangleFlat(ZBuffer* zb, ZBufferPoint* p0, ZBufferPoint* p1, ZBufferPoint* p2)
{
  switch (zb->cn) {
  case 3: {
    enum {PSZB = 3};
    typedef uchar PIXEL;
    unsigned char colorR, colorG, colorB;
#define INTERP_Z
#define DRAW_INIT()  { colorR=p2->r>>8; colorG=p2->g>>8; colorB=p2->b>>8; }
#define PUT_PIXEL(_a)   \
{           \
  zz=z >> ZB_POINT_Z_FRAC_BITS;   \
  if (ZCMP(zz,pz[_a])) { uchar* pp1 = pp+3 * _a; \
    RGB_TO_PIXEL24(pp1, colorR, colorG, colorB);  \
    pz[_a]=zz;        \
  }\
  z+=dzdx;          \
}
#include "ztriangle_impl.1inl"
  }
  break;
  case 2: {
    enum {PSZB = 2};
    typedef ushort PIXEL;
    PIXEL color;
#define INTERP_Z
#define DRAW_INIT()   { RGB_TO_PIXEL16(&color, p2->r,p2->g,p2->b); }
#define PUT_PIXEL(_a) { zz=z >> ZB_POINT_Z_FRAC_BITS; if (ZCMP(zz,pz[_a])) { pp[_a]=color; pz[_a]=zz; } z+=dzdx; }
#include "ztriangle_impl.1inl"
  }
  break;
  }
}
/*
 * Smooth filled triangle.
 * The code below is very tricky :)
 */
void ZB_fillTriangleSmooth(ZBuffer* zb, ZBufferPoint* p0, ZBufferPoint* p1, ZBufferPoint* p2)
{
#define SAR_RND_TO_ZERO(v,n) (v / (1<<n))
  switch (zb->cn) {
  case 3: {
    enum {PSZB = 3};
    typedef uchar PIXEL;
#define INTERP_Z
#define INTERP_RGB
#define DRAW_INIT()   {}
#define PUT_PIXEL(_a)       \
{           \
  zz=z >> ZB_POINT_Z_FRAC_BITS;   \
  if (ZCMP(zz,pz[_a])) {  uchar* pp1 = pp+3 * _a; \
    RGB_TO_PIXEL24(pp1, or1 >> 8, og1 >> 8, ob1 >> 8);  \
    pz[_a]=zz;        \
  }\
  z+=dzdx;          \
  og1+=dgdx;          \
  or1+=drdx;          \
  ob1+=dbdx;          \
}
#include "ztriangle_impl.1inl"
  }
  break;
  case 2: {
    int _drgbdx;
    enum {PSZB = 2};
    typedef ushort PIXEL;
#define INTERP_Z
#define INTERP_RGB
#define DRAW_INIT()  {           \
  _drgbdx=(SAR_RND_TO_ZERO(drdx,6) << 22) & 0xFFC00000;   \
  _drgbdx|=SAR_RND_TO_ZERO(dgdx,5) & 0x000007FF;    \
  _drgbdx|=(SAR_RND_TO_ZERO(dbdx,7) << 12) & 0x001FF000;  \
}
#define PUT_PIXEL(_a)       \
{           \
  zz=z >> ZB_POINT_Z_FRAC_BITS;   \
  if (ZCMP(zz,pz[_a])) {        \
    tmp=rgb & 0xF81F07E0;     \
    pp[_a]=tmp | (tmp >> 16);     \
    pz[_a]=zz;        \
  }           \
  z+=dzdx;          \
  rgb=(rgb+drgbdx) & ( ~ 0x00200800);   \
}
#define DRAW_LINE()                \
{                    \
  register unsigned short *pz;             \
  register PIXEL *pp;            \
  register unsigned int tmp,z,zz,rgb,drgbdx;           \
  register int n;                \
  n=(x2 >> 16) - x1;                 \
  pp=pp1+x1;                   \
  pz=pz1+x1;                   \
  z=z1;                    \
  rgb=(r1 << 16) & 0xFFC00000;               \
  rgb|=(g1 >> 5) & 0x000007FF;               \
  rgb|=(b1 << 5) & 0x001FF000;               \
  drgbdx=_drgbdx;                \
  while (n>=3) {                 \
    PUT_PIXEL(0);                \
    PUT_PIXEL(1);                \
    PUT_PIXEL(2);                \
    PUT_PIXEL(3);                \
    pz+=4;                   \
    pp+=4;                   \
    n-=4;                  \
  }                    \
  while (n>=0) {                 \
    PUT_PIXEL(0);                \
    pz+=1;                   \
    pp+=1;                   \
    n-=1;                  \
  }                    \
}
#include "ztriangle_impl.1inl"
  }
  break;
  case 4: {
    enum {PSZB = 4};
    typedef uint PIXEL;
#define INTERP_Z
#define INTERP_RGB
#define DRAW_INIT()         \
{           \
}
#define PUT_PIXEL(_a)       \
{           \
  zz=z >> ZB_POINT_Z_FRAC_BITS;   \
  if (ZCMP(zz,pz[_a])) {        \
    RGB_TO_PIXEL32((pp+_a), or1, og1, ob1);\
    pz[_a]=zz;        \
  }\
  z+=dzdx;          \
  og1+=dgdx;          \
  or1+=drdx;          \
  ob1+=dbdx;          \
}
#include "ztriangle_impl.1inl"
  }
  break;
  }
}
void ZB_setTexture(ZBuffer* zb, uchar* texture)
{
  zb->current_texture = texture;
}
void ZB_fillTriangleMapping(ZBuffer* zb,
    ZBufferPoint* p0, ZBufferPoint* p1, ZBufferPoint* p2)
{
  switch (zb->cn) {
  case 3: {
    enum {PSZB = 3};
    typedef uchar PIXEL;
    PIXEL* texture;
#define INTERP_Z
#define INTERP_ST
#define DRAW_INIT()   { texture=(PIXEL*)zb->current_texture; }
#define PUT_PIXEL(_a)   {           \
  unsigned char *ptr;\
  zz=z >> ZB_POINT_Z_FRAC_BITS;    \
  if (ZCMP(zz,pz[_a])) { uchar* pp1 = pp + 3*_a;      \
    ptr = texture + (((t & 0x3FC00000) | s) >> 14) * 3; \
    pp1[0]= ptr[0]; pp1[1]= ptr[1]; pp1[2]= ptr[2];\
    pz[_a]=zz;       \
  }           \
  z+=dzdx;          \
  s+=dsdx;          \
  t+=dtdx;          \
}
#include "ztriangle_impl.1inl"
  }
  break;
  case 2: {
    enum {PSZB = 2};
    typedef ushort PIXEL;
    PIXEL* texture;
#define INTERP_Z
#define INTERP_ST
#define DRAW_INIT()   { texture=(PIXEL*)zb->current_texture; }
#define PUT_PIXEL(_a)    {           \
  zz=z >> ZB_POINT_Z_FRAC_BITS;    \
  if (ZCMP(zz,pz[_a])) {       \
    pp[_a]=texture[((t & 0x3FC00000) | s) >> 14];  \
    pz[_a]=zz;       \
  }           \
  z+=dzdx;          \
  s+=dsdx;          \
  t+=dtdx;          \
}
#include "ztriangle_impl.1inl"
  }
  break;
  }
}
/*
 * Texture mapping with perspective correction.
 * We use the gradient method to make less divisions.
 * TODO: pipeline the division
 */
void ZB_fillTriangleMappingPerspective(ZBuffer* zb, ZBufferPoint* p0, ZBufferPoint* p1, ZBufferPoint* p2)
{
  GLfloat fdzdx, fndzdx, ndszdx, ndtzdx;
#define NB_INTERP 8
#define DRAW_INIT1()       \
  {           \
    texture=(PIXEL*)zb->current_texture;\
    fdzdx=(GLfloat)dzdx;\
    fndzdx=NB_INTERP * fdzdx;\
    ndszdx=NB_INTERP * dszdx;\
    ndtzdx=NB_INTERP * dtzdx;\
  }
#define DRAW_LINE1()       \
  {           \
    register unsigned short *pz;    \
    register PIXEL *pp;   \
    register unsigned int s,t,z,zz; \
    register int n,dsdx,dtdx;   \
    GLfloat sz,tz,fz,zinv; \
    n=(x2>>16)-x1;                             \
    fz=(GLfloat)z1;\
    zinv=1.0f / fz;\
    pp=(PIXEL *)((char *)pp1 + x1 * PSZB); \
    pz=pz1+x1;          \
    z=z1;           \
    sz=sz1;\
    tz=tz1;\
    while (n>=(NB_INTERP-1)) {               \
      {\
        GLfloat ss,tt;\
        ss=(sz * zinv);\
        tt=(tz * zinv);\
        s=(int) ss;\
        t=(int) tt;\
        dsdx= (int)( (dszdx - ss*fdzdx)*zinv );\
        dtdx= (int)( (dtzdx - tt*fdzdx)*zinv );\
        fz+=fndzdx;\
        zinv=1.0f / fz;\
      }\
      PUT_PIXEL(0);                \
      PUT_PIXEL(1);                \
      PUT_PIXEL(2);                \
      PUT_PIXEL(3);                \
      PUT_PIXEL(4);                \
      PUT_PIXEL(5);                \
      PUT_PIXEL(6);                \
      PUT_PIXEL(7);                \
      pz+=NB_INTERP;                 \
      pp=(PIXEL *)((char *)pp + NB_INTERP * PSZB);\
      n-=NB_INTERP;                \
      sz+=ndszdx;\
      tz+=ndtzdx;\
    }                    \
    {\
      GLfloat ss,tt;\
      ss=(sz * zinv);\
      tt=(tz * zinv);\
      s=(int) ss;\
      t=(int) tt;\
      dsdx= (int)( (dszdx - ss*fdzdx)*zinv );\
      dtdx= (int)( (dtzdx - tt*fdzdx)*zinv );\
    }\
    while (n>=0) {                 \
      PUT_PIXEL(0);                \
      pz+=1;                   \
      pp=(PIXEL *)((char *)pp + PSZB);\
      n-=1;                  \
    }                    \
  }
  switch (zb->cn) {
  case 3: {
    enum {PSZB = 3, PSZSH = 5};
    typedef uchar PIXEL;
    PIXEL* texture;
#define INTERP_Z
#define INTERP_STZ
#define DRAW_INIT DRAW_INIT1
#define DRAW_LINE DRAW_LINE1
#define PUT_PIXEL(_a)       \
{           \
  unsigned char *ptr;\
  zz=z >> ZB_POINT_Z_FRAC_BITS;    \
  if (ZCMP(zz,pz[_a])) {       \
    ptr = texture + (((t & 0x3FC00000) | (s & 0x003FC000)) >> 14) * 3;\
    pp[3 * _a]= ptr[0];\
    pp[3 * _a + 1]= ptr[1];\
    pp[3 * _a + 2]= ptr[2];\
    pz[_a]=zz;       \
  }           \
  z+=dzdx;          \
  s+=dsdx;          \
  t+=dtdx;          \
}
#include "ztriangle_impl.1inl"
  }
  break;
  case 2: {
    enum {PSZB = 2, PSZSH = 4};
    typedef ushort PIXEL;
    PIXEL* texture;
#define INTERP_Z
#define INTERP_STZ
#define DRAW_INIT DRAW_INIT1
#define DRAW_LINE DRAW_LINE1
#define PUT_PIXEL(_a)       \
{           \
  zz=z >> ZB_POINT_Z_FRAC_BITS;    \
  if (ZCMP(zz,pz[_a])) {       \
    pp[_a]=*(PIXEL *)((char *)texture+ \
        (((t & 0x3FC00000) | (s & 0x003FC000)) >> (17 - PSZSH)));\
    pz[_a]=zz;       \
  }           \
  z+=dzdx;          \
  s+=dsdx;          \
  t+=dtdx;          \
}
#include "ztriangle_impl.1inl"
  }
  break;
  }
#undef DRAW_LINE1
#undef DRAW_INIT1
}
#if 0
/* slow but exact version (only there for reference, incorrect for 24 bits) */
void ZB_fillTriangleMappingPerspective(ZBuffer* zb, ZBufferPoint* p0, ZBufferPoint* p1, ZBufferPoint* p2)
{
  PIXEL* texture;
#define INTERP_Z
#define INTERP_STZ
#define DRAW_INIT()       \
  {           \
    texture=zb->current_texture;      \
  }
#define PUT_PIXEL(_a)       \
  {           \
    GLfloat zinv; \
    int s,t; \
    zz=z >> ZB_POINT_Z_FRAC_BITS;    \
    if (ZCMP(zz,pz[_a])) {       \
      zinv= 1.0 / (GLfloat) z; \
      s= (int) (sz * zinv); \
      t= (int) (tz * zinv); \
      pp[_a]=texture[((t & 0x3FC00000) | s) >> 14];  \
      pz[_a]=zz;       \
    }           \
    z+=dzdx;          \
    sz+=dszdx;          \
    tz+=dtzdx;          \
  }
#include "ztriangle_impl_cn.inl"
}
#endif
void gl_draw_triangle_fill(GLContext* c, GLVertex* p0, GLVertex* p1, GLVertex* p2)
{
#ifdef PROFILE
  {
    int norm;
    assert(p0->zp.x >= 0 && p0->zp.x < c->w);
    assert(p0->zp.y >= 0 && p0->zp.y < c->h);
    assert(p1->zp.x >= 0 && p1->zp.x < c->w);
    assert(p1->zp.y >= 0 && p1->zp.y < c->h);
    assert(p2->zp.x >= 0 && p2->zp.x < c->w);
    assert(p2->zp.y >= 0 && p2->zp.y < c->h);
    norm = (p1->zp.x - p0->zp.x) * (p2->zp.y - p0->zp.y) -
    (p2->zp.x - p0->zp.x) * (p1->zp.y - p0->zp.y);
    count_pixels += abs(norm) / 2;
    count_triangles++;
  }
#endif
  if (c->texture_2d_enabled) {
#ifdef PROFILE
    count_triangles_textured++;
#endif
    ZB_setTexture(c->zb, (unsigned char*)c->current_texture->images[0].pixmap);
    ZB_fillTriangleMappingPerspective(c->zb, &p0->zp, &p1->zp, &p2->zp);
  }
  else if (c->current_shade_model == GL_SMOOTH) {
    ZB_fillTriangleSmooth(c->zb, &p0->zp, &p1->zp, &p2->zp);
  }
  else {
    ZB_fillTriangleFlat(c->zb, &p0->zp, &p1->zp, &p2->zp);
  }
}
/* Render a clipped triangle in line mode */
void gl_draw_triangle_line(GLContext* c, GLVertex* p0, GLVertex* p1, GLVertex* p2)
{
  if (c->depth_test) {
    if (p0->edge_flag) {
      ZB_line_z(c->zb, &p0->zp, &p1->zp);
    }
    if (p1->edge_flag) {
      ZB_line_z(c->zb, &p1->zp, &p2->zp);
    }
    if (p2->edge_flag) {
      ZB_line_z(c->zb, &p2->zp, &p0->zp);
    }
  }
  else {
    if (p0->edge_flag) {
      ZB_line(c->zb, &p0->zp, &p1->zp);
    }
    if (p1->edge_flag) {
      ZB_line(c->zb, &p1->zp, &p2->zp);
    }
    if (p2->edge_flag) {
      ZB_line(c->zb, &p2->zp, &p0->zp);
    }
  }
}
/* Render a clipped triangle in point mode */
void gl_draw_triangle_point(GLContext* c,
    GLVertex* p0, GLVertex* p1, GLVertex* p2)
{
  if (p0->edge_flag) {
    ZB_plot(c->zb, &p0->zp, c->point_size);
  }
  if (p1->edge_flag) {
    ZB_plot(c->zb, &p1->zp, c->point_size);
  }
  if (p2->edge_flag) {
    ZB_plot(c->zb, &p2->zp, c->point_size);
  }
}
void glopBegin(GLContext* c, GLParam* p)
{
  int type;
  M4 tmp;
  assert(c->in_begin == 0);
  type = p[1].i;
  c->begin_type = type;
  c->in_begin = 1;
  c->vertex_n = 0;
  c->vertex_cnt = 0;
  if (c->matrix_model_projection_updated) {
    if (c->lighting_enabled) {
      /* precompute inverse modelview */
      gl_M4_Inv(&tmp, c->matrix_stack_ptr[0]);
      gl_M4_Transpose(&c->matrix_model_view_inv, &tmp);
    }
    else {
      GLfloat* m = &c->matrix_model_projection.m[0][0];
      /* precompute projection matrix */
      gl_M4_Mul(&c->matrix_model_projection, c->matrix_stack_ptr[1], c->matrix_stack_ptr[0]);
      /* test to accelerate computation */
      c->matrix_model_projection_no_w_transform = 0;
      if (m[12] == 0.0 && m[13] == 0.0 && m[14] == 0.0) {
        c->matrix_model_projection_no_w_transform = 1;
      }
    }
    /* test if the texture matrix is not Identity */
    c->apply_texture_matrix = !gl_M4_IsId(c->matrix_stack_ptr[2]);
    c->matrix_model_projection_updated = 0;
  }
  /*  viewport */
  if (c->viewport.updated) {
    gl_eval_viewport(c);
    c->viewport.updated = 0;
  }
  /* triangle drawing functions */
  if (c->render_mode == GL_SELECT) {
    c->draw_triangle_front = gl_draw_triangle_select;
    c->draw_triangle_back = gl_draw_triangle_select;
  }
  else {
    switch (c->polygon_mode_front) {
    case GL_POINT:
      c->draw_triangle_front = gl_draw_triangle_point;
      break;
    case GL_LINE:
      c->draw_triangle_front = gl_draw_triangle_line;
      break;
    default:
      c->draw_triangle_front = gl_draw_triangle_fill;
      break;
    }
    switch (c->polygon_mode_back) {
    case GL_POINT:
      c->draw_triangle_back = gl_draw_triangle_point;
      break;
    case GL_LINE:
      c->draw_triangle_back = gl_draw_triangle_line;
      break;
    default:
      c->draw_triangle_back = gl_draw_triangle_fill;
      break;
    }
  }
}
/* coords, tranformation , clip code and projection */
/* TODO : handle all cases */
static CC_INLINE void gl_vertex_transform(GLContext* c, GLVertex* v)
{
  GLfloat* m;
  V4* n;
  if (c->lighting_enabled) {
    /* eye coordinates needed for lighting */
    m = &c->matrix_stack_ptr[0]->m[0][0];
    v->ec.X = (v->coord.X * m[0] + v->coord.Y * m[1] + v->coord.Z * m[2] + m[3]);
    v->ec.Y = (v->coord.X * m[4] + v->coord.Y * m[5] + v->coord.Z * m[6] + m[7]);
    v->ec.Z = (v->coord.X * m[8] + v->coord.Y * m[9] + v->coord.Z * m[10] + m[11]);
    v->ec.W = (v->coord.X * m[12] + v->coord.Y * m[13] + v->coord.Z * m[14] + m[15]);
    /* projection coordinates */
    m = &c->matrix_stack_ptr[1]->m[0][0];
    v->pc.X = (v->ec.X * m[0] + v->ec.Y * m[1] + v->ec.Z * m[2] + v->ec.W * m[3]);
    v->pc.Y = (v->ec.X * m[4] + v->ec.Y * m[5] + v->ec.Z * m[6] + v->ec.W * m[7]);
    v->pc.Z = (v->ec.X * m[8] + v->ec.Y * m[9] + v->ec.Z * m[10] + v->ec.W * m[11]);
    v->pc.W = (v->ec.X * m[12] + v->ec.Y * m[13] + v->ec.Z * m[14] + v->ec.W * m[15]);
    m = &c->matrix_model_view_inv.m[0][0];
    n = &c->current_normal;
    v->normal.X = (n->X * m[0] + n->Y * m[1] + n->Z * m[2]);
    v->normal.Y = (n->X * m[4] + n->Y * m[5] + n->Z * m[6]);
    v->normal.Z = (n->X * m[8] + n->Y * m[9] + n->Z * m[10]);
    if (c->normalize_enabled) {
      gl_V3_Norm(&v->normal);
    }
  }
  else {
    /* no eye coordinates needed, no normal */
    /* NOTE: W = 1 is assumed */
    m = &c->matrix_model_projection.m[0][0];
    v->pc.X = (v->coord.X * m[0] + v->coord.Y * m[1] + v->coord.Z * m[2] + m[3]);
    v->pc.Y = (v->coord.X * m[4] + v->coord.Y * m[5] + v->coord.Z * m[6] + m[7]);
    v->pc.Z = (v->coord.X * m[8] + v->coord.Y * m[9] + v->coord.Z * m[10] + m[11]);
    if (c->matrix_model_projection_no_w_transform) {
      v->pc.W = m[15];
    }
    else {
      v->pc.W = (v->coord.X * m[12] + v->coord.Y * m[13] + v->coord.Z * m[14] + m[15]);
    }
  }
  v->clip_code = gl_clipcode(v->pc.X, v->pc.Y, v->pc.Z, v->pc.W);
}
static CC_INLINE GLfloat clampf(GLfloat a, GLfloat min, GLfloat max)
{
  if (a < min) {
    return min;
  }
  else if (a > max) {
    return max;
  }
  else {
    return a;
  }
}
static void calc_buf(GLSpecBuf* buf, const GLfloat shininess)
{
  int i;
  GLfloat val, inc;
  val = 0.0f;
  inc = 1.0f / SPECULAR_BUFFER_SIZE;
  for (i = 0; i <= SPECULAR_BUFFER_SIZE; i++) {
    buf->buf[i] = (float)pow(val, shininess);
    val += inc;
  }
}
GLSpecBuf* specbuf_get_buffer(GLContext* c, const int shininess_i, const GLfloat shininess)
{
  GLSpecBuf* found, *oldest;
  found = oldest = c->specbuf_first;
  while (found && found->shininess_i != shininess_i) {
    if (found->last_used < oldest->last_used) {
      oldest = found;
    }
    found = found->next;
  }
  if (found) { /* hey, found one! */
    found->last_used = c->specbuf_used_counter++;
    return found;
  }
  if (oldest == NULL || c->specbuf_num_buffers < MAX_SPECULAR_BUFFERS) {
    /* create new buffer */
    GLSpecBuf* buf = (GLSpecBuf*)gl_malloc(sizeof(GLSpecBuf));
    if (!buf) {
      gl_fatal_error("could not allocate specular buffer");
    }
    c->specbuf_num_buffers++;
    buf->next = c->specbuf_first;
    c->specbuf_first = buf;
    buf->last_used = c->specbuf_used_counter++;
    buf->shininess_i = shininess_i;
    calc_buf(buf, shininess);
    return buf;
  }
  /* overwrite the lru buffer */
  /*tgl_trace("overwriting spec buffer :(\n");*/
  oldest->shininess_i = shininess_i;
  oldest->last_used = c->specbuf_used_counter++;
  calc_buf(oldest, shininess);
  return oldest;
}
/* non optimized lightening model */
void gl_shade_vertex(GLContext* c, GLVertex* v)
{
  GLfloat R, G, B, A;
  GLMaterial* m;
  GLLight* l;
  V3 n, s, d;
  GLfloat dist, tmp, att, dot, dot_spot, dot_spec;
  int twoside = c->light_model_two_side;
  m = &c->materials[0];
  n.X = v->normal.X;
  n.Y = v->normal.Y;
  n.Z = v->normal.Z;
  R = m->emission.v[0] + m->ambient.v[0] * c->ambient_light_model.v[0];
  G = m->emission.v[1] + m->ambient.v[1] * c->ambient_light_model.v[1];
  B = m->emission.v[2] + m->ambient.v[2] * c->ambient_light_model.v[2];
  A = clampf(m->diffuse.v[3], 0, 1);
  for (l = c->first_light; l != NULL; l = l->next) {
    GLfloat lR, lB, lG;
    /* ambient */
    lR = l->ambient.v[0] * m->ambient.v[0];
    lG = l->ambient.v[1] * m->ambient.v[1];
    lB = l->ambient.v[2] * m->ambient.v[2];
    if (l->position.v[3] == 0) {
      /* light at infinity */
      d.X = l->position.v[0];
      d.Y = l->position.v[1];
      d.Z = l->position.v[2];
      att = 1;
    }
    else {
      /* distance attenuation */
      d.X = l->position.v[0] - v->ec.v[0];
      d.Y = l->position.v[1] - v->ec.v[1];
      d.Z = l->position.v[2] - v->ec.v[2];
      dist = (float)sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z);
      if (dist > 1E-3) {
        tmp = 1 / dist;
        d.X *= tmp;
        d.Y *= tmp;
        d.Z *= tmp;
      }
      att = 1.0f / (l->attenuation[0] + dist * (l->attenuation[1] + dist * l->attenuation[2]));
    }
    dot = d.X * n.X + d.Y * n.Y + d.Z * n.Z;
    if (twoside && dot < 0) {
      dot = -dot;
    }
    if (dot > 0) {
      /* diffuse light */
      lR += dot * l->diffuse.v[0] * m->diffuse.v[0];
      lG += dot * l->diffuse.v[1] * m->diffuse.v[1];
      lB += dot * l->diffuse.v[2] * m->diffuse.v[2];
      /* spot light */
      if (l->spot_cutoff != 180) {
        dot_spot = -(d.X * l->norm_spot_direction.v[0] + d.Y * l->norm_spot_direction.v[1] + d.Z * l->norm_spot_direction.v[2]);
        if (twoside && dot_spot < 0) {
          dot_spot = -dot_spot;
        }
        if (dot_spot < l->cos_spot_cutoff) {
          /* no contribution */
          continue;
        }
        else {
          /* TODO: optimize */
          if (l->spot_exponent > 0) {
            att = (float)(att * pow(dot_spot, l->spot_exponent));
          }
        }
      }
      /* specular light */
      if (c->local_light_model) {
        V3 vcoord;
        vcoord.X = v->ec.X;
        vcoord.Y = v->ec.Y;
        vcoord.Z = v->ec.Z;
        gl_V3_Norm(&vcoord);
        s.X = d.X - vcoord.X;
        s.Y = d.Y - vcoord.X;
        s.Z = d.Z - vcoord.X;
      }
      else {
        s.X = d.X;
        s.Y = d.Y;
        s.Z = (float)(d.Z + 1.0);
      }
      dot_spec = n.X * s.X + n.Y * s.Y + n.Z * s.Z;
      if (twoside && dot_spec < 0) {
        dot_spec = -dot_spec;
      }
      if (dot_spec > 0) {
        GLSpecBuf* specbuf;
        int idx;
        tmp = (float)sqrt(s.X * s.X + s.Y * s.Y + s.Z * s.Z);
        if (tmp > 1E-3) {
          dot_spec = dot_spec / tmp;
        }
        /* TODO: optimize */
        /* testing specular buffer code */
        /* dot_spec= pow(dot_spec,m->shininess);*/
        specbuf = specbuf_get_buffer(c, m->shininess_i, m->shininess);
        idx = (int)(dot_spec * SPECULAR_BUFFER_SIZE);
        if (idx > SPECULAR_BUFFER_SIZE) {
          idx = SPECULAR_BUFFER_SIZE;
        }
        dot_spec = specbuf->buf[idx];
        lR += dot_spec * l->specular.v[0] * m->specular.v[0];
        lG += dot_spec * l->specular.v[1] * m->specular.v[1];
        lB += dot_spec * l->specular.v[2] * m->specular.v[2];
      }
    }
    R += att * lR;
    G += att * lG;
    B += att * lB;
  }
  v->color.v[0] = clampf(R, 0, 1);
  v->color.v[1] = clampf(G, 0, 1);
  v->color.v[2] = clampf(B, 0, 1);
  v->color.v[3] = A;
}
void glopVertex(GLContext* c, GLParam* p)
{
  GLVertex* v;
  int n, i, cnt;
  assert(c->in_begin != 0);
  n = c->vertex_n;
  cnt = c->vertex_cnt;
  cnt++;
  c->vertex_cnt = cnt;
  /* quick fix to avoid crashes on large polygons */
  if (n >= c->vertex_max) {
    GLVertex* newarray;
    c->vertex_max <<= 1;  /* just double size */
    newarray = (GLVertex*)gl_malloc(sizeof(GLVertex) * c->vertex_max);
    if (!newarray) {
      gl_fatal_error("unable to allocate GLVertex array.\n");
    }
    memcpy(newarray, c->vertex, n * sizeof(GLVertex));
    gl_free(c->vertex);
    c->vertex = newarray;
  }
  /* new vertex entry */
  v = &c->vertex[n];
  n++;
  v->coord.X = p[1].f;
  v->coord.Y = p[2].f;
  v->coord.Z = p[3].f;
  v->coord.W = p[4].f;
  gl_vertex_transform(c, v);
  /* color */
  if (c->lighting_enabled) {
    gl_shade_vertex(c, v);
  }
  else {
    v->color = c->current_color;
  }
  /* tex coords */
  if (c->texture_2d_enabled) {
    if (c->apply_texture_matrix) {
      gl_M4_MulV4(&v->tex_coord, c->matrix_stack_ptr[2], &c->current_tex_coord);
    }
    else {
      v->tex_coord = c->current_tex_coord;
    }
  }
  /* precompute the mapping to the viewport */
  if (v->clip_code == 0) {
    gl_transform_to_viewport(c, v);
  }
  /* edge flag */
  v->edge_flag = c->current_edge_flag;
  switch (c->begin_type) {
  case GL_POINTS:
    gl_draw_point(c, &c->vertex[0]);
    n = 0;
    break;
  case GL_LINES:
    if (n == 2) {
      gl_draw_line(c, &c->vertex[0], &c->vertex[1]);
      n = 0;
    }
    break;
  case GL_LINE_STRIP:
  case GL_LINE_LOOP:
    if (n == 1) {
      c->vertex[2] = c->vertex[0];
    }
    else if (n == 2) {
      gl_draw_line(c, &c->vertex[0], &c->vertex[1]);
      c->vertex[0] = c->vertex[1];
      n = 1;
    }
    break;
  case GL_TRIANGLES:
    if (n == 3) {
      gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
      n = 0;
    }
    break;
  case GL_TRIANGLE_STRIP:
    if (cnt >= 3) {
      if (n == 3) {
        n = 0;
      }
      /* needed to respect triangle orientation */
      switch (cnt & 1) {
      case 0:
        gl_draw_triangle(c, &c->vertex[2], &c->vertex[1], &c->vertex[0]);
        break;
      default:
      case 1:
        gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
        break;
      }
    }
    break;
  case GL_TRIANGLE_FAN:
    if (n == 3) {
      gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
      c->vertex[1] = c->vertex[2];
      n = 2;
    }
    break;
  case GL_QUADS:
    if (n == 4) {
      c->vertex[2].edge_flag = 0;
      gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
      c->vertex[2].edge_flag = 1;
      c->vertex[0].edge_flag = 0;
      gl_draw_triangle(c, &c->vertex[0], &c->vertex[2], &c->vertex[3]);
      n = 0;
    }
    break;
  case GL_QUAD_STRIP:
    if (n == 4) {
      gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
      gl_draw_triangle(c, &c->vertex[1], &c->vertex[3], &c->vertex[2]);
      for (i = 0; i < 2; i++) {
        c->vertex[i] = c->vertex[i + 2];
      }
      n = 2;
    }
    break;
  case GL_POLYGON:
    break;
  default:
    gl_fatal_error("glBegin: type %x not handled\n", c->begin_type);
  }
  c->vertex_n = n;
}
void glopEnd(GLContext* c, GLParam* p)
{
  assert(c->in_begin == 1);
  if (c->begin_type == GL_LINE_LOOP) {
    if (c->vertex_cnt >= 3) {
      gl_draw_line(c, &c->vertex[0], &c->vertex[2]);
    }
  }
  else if (c->begin_type == GL_POLYGON) {
    int i = c->vertex_cnt;
    while (i >= 3) {
      i--;
      gl_draw_triangle(c, &c->vertex[i], &c->vertex[0], &c->vertex[i - 1]);
    }
  }
  c->in_begin = 0;
}
void gl_enable_disable_light(GLContext* c, int light, int v)
{
  GLLight* l = &c->lights[light];
  if (v && !l->enabled) {
    l->enabled = 1;
    l->next = c->first_light;
    c->first_light = l;
    l->prev = NULL;
  }
  else if (!v && l->enabled) {
    l->enabled = 0;
    if (l->prev == NULL) {
      c->first_light = l->next;
    }
    else {
      l->prev->next = l->next;
    }
    if (l->next != NULL) {
      l->next->prev = l->prev;
    }
  }
}
void glopEnableDisable(GLContext* c, GLParam* p)
{
  int code = p[1].i;
  int v = p[2].i;
  switch (code) {
  case GL_CULL_FACE:
    c->cull_face_enabled = v;
    break;
  case GL_LIGHTING:
    c->lighting_enabled = v;
    break;
  case GL_COLOR_MATERIAL:
    c->color_material_enabled = v;
    break;
  case GL_TEXTURE_2D:
    c->texture_2d_enabled = v;
    break;
  case GL_NORMALIZE:
    c->normalize_enabled = v;
    break;
  case GL_DEPTH_TEST:
    c->depth_test = v;
    break;
  case GL_POLYGON_OFFSET_FILL:
    if (v) {
      c->offset_states |= TGL_OFFSET_FILL;
    }
    else {
      c->offset_states &= ~TGL_OFFSET_FILL;
    }
    break;
  case GL_POLYGON_OFFSET_POINT:
    if (v) {
      c->offset_states |= TGL_OFFSET_POINT;
    }
    else {
      c->offset_states &= ~TGL_OFFSET_POINT;
    }
    break;
  case GL_POLYGON_OFFSET_LINE:
    if (v) {
      c->offset_states |= TGL_OFFSET_LINE;
    }
    else {
      c->offset_states &= ~TGL_OFFSET_LINE;
    }
    break;
  default:
    if (code >= GL_LIGHT0 && code < GL_LIGHT0 + MAX_LIGHTS) {
      gl_enable_disable_light(c, code - GL_LIGHT0, v);
    }
    else {
      /*
      fprintf(stderr,"glEnableDisable: 0x%X not supported.\n",code);
      */
    }
    break;
  }
}
void gl_print_matrix(const GLfloat* m)
{
  int i;
  for (i = 0; i < 4; i++) {
    fprintf(stderr, "%f %f %f %f\n", m[i], m[4 + i], m[8 + i], m[12 + i]);
  }
}
static CC_INLINE void gl_matrix_update(GLContext* c)
{
  c->matrix_model_projection_updated = (c->matrix_mode <= 1);
}
void glopMatrixMode(GLContext* c, GLParam* p)
{
  int mode = p[1].i;
  switch (mode) {
  case GL_MODELVIEW:
    c->matrix_mode = 0;
    break;
  case GL_PROJECTION:
    c->matrix_mode = 1;
    break;
  case GL_TEXTURE:
    c->matrix_mode = 2;
    break;
  default:
    ASSERT(0);
  }
}
void glopLoadMatrix(GLContext* c, GLParam* p)
{
  M4* m;
  int i;
  GLParam* q;
  m = c->matrix_stack_ptr[c->matrix_mode];
  q = p + 1;
  for (i = 0; i < 4; i++) {
    m->m[0][i] = q[0].f;
    m->m[1][i] = q[1].f;
    m->m[2][i] = q[2].f;
    m->m[3][i] = q[3].f;
    q += 4;
  }
  gl_matrix_update(c);
}
void glopLoadIdentity(GLContext* c, GLParam* p)
{
  gl_M4_Id(c->matrix_stack_ptr[c->matrix_mode]);
  gl_matrix_update(c);
}
void glopMultMatrix(GLContext* c, GLParam* p)
{
  M4 m;
  int i;
  GLParam* q;
  q = p + 1;
  for (i = 0; i < 4; i++) {
    m.m[0][i] = q[0].f;
    m.m[1][i] = q[1].f;
    m.m[2][i] = q[2].f;
    m.m[3][i] = q[3].f;
    q += 4;
  }
  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
}
void glopPushMatrix(GLContext* c, GLParam* p)
{
  int n = c->matrix_mode;
  M4* m;
  ASSERT((c->matrix_stack_ptr[n] - c->matrix_stack[n] + 1) < c->matrix_stack_depth_max[n]);
  m = ++c->matrix_stack_ptr[n];
  gl_M4_Move(&m[0], &m[-1]);
  gl_matrix_update(c);
}
void glopPopMatrix(GLContext* c, GLParam* p)
{
  int n = c->matrix_mode;
  ASSERT(c->matrix_stack_ptr[n] > c->matrix_stack[n]);
  c->matrix_stack_ptr[n]--;
  gl_matrix_update(c);
}
void glopRotate(GLContext* c, GLParam* p)
{
  M4 m;
  GLfloat u[3];
  GLfloat angle;
  int dir_code;
  angle = (float)(p[1].f * M_PI / 180.0);
  u[0] = p[2].f;
  u[1] = p[3].f;
  u[2] = p[4].f;
  /* simple case detection */
  dir_code = ((u[0] != 0) << 2) | ((u[1] != 0) << 1) | (u[2] != 0);
  switch (dir_code) {
  case 0:
    gl_M4_Id(&m);
    break;
  case 4:
    if (u[0] < 0) {
      angle = -angle;
    }
    gl_M4_Rotate(&m, angle, 0);
    break;
  case 2:
    if (u[1] < 0) {
      angle = -angle;
    }
    gl_M4_Rotate(&m, angle, 1);
    break;
  case 1:
    if (u[2] < 0) {
      angle = -angle;
    }
    gl_M4_Rotate(&m, angle, 2);
    break;
  default: {
    GLfloat cost, sint;
    /* normalize vector */
    GLfloat len = u[0] * u[0] + u[1] * u[1] + u[2] * u[2];
    if (len == 0.0f) {
      return;
    }
    len = (float)(1.0f / sqrt(len));
    u[0] *= len;
    u[1] *= len;
    u[2] *= len;
    /* store cos and sin values */
    cost = (float)cos(angle);
    sint = (float)sin(angle);
    /* fill in the values */
    m.m[3][0] = m.m[3][1] = m.m[3][2] =
        m.m[0][3] = m.m[1][3] = m.m[2][3] = 0.0f;
    m.m[3][3] = 1.0f;
    /* do the math */
    m.m[0][0] = u[0] * u[0] + cost * (1 - u[0] * u[0]);
    m.m[1][0] = u[0] * u[1] * (1 - cost) - u[2] * sint;
    m.m[2][0] = u[2] * u[0] * (1 - cost) + u[1] * sint;
    m.m[0][1] = u[0] * u[1] * (1 - cost) + u[2] * sint;
    m.m[1][1] = u[1] * u[1] + cost * (1 - u[1] * u[1]);
    m.m[2][1] = u[1] * u[2] * (1 - cost) - u[0] * sint;
    m.m[0][2] = u[2] * u[0] * (1 - cost) - u[1] * sint;
    m.m[1][2] = u[1] * u[2] * (1 - cost) + u[0] * sint;
    m.m[2][2] = u[2] * u[2] + cost * (1 - u[2] * u[2]);
  }
  }
  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
}
void glopTranslate(GLContext* c, GLParam* p)
{
  GLfloat* m;
  GLfloat x = p[1].f, y = p[2].f, z = p[3].f;
  m = &c->matrix_stack_ptr[c->matrix_mode]->m[0][0];
  m[3] = m[0] * x + m[1] * y + m[2]  * z + m[3];
  m[7] = m[4] * x + m[5] * y + m[6]  * z + m[7];
  m[11] = m[8] * x + m[9] * y + m[10] * z + m[11];
  m[15] = m[12] * x + m[13] * y + m[14] * z + m[15];
  gl_matrix_update(c);
}
void glopScale(GLContext* c, GLParam* p)
{
  GLfloat* m;
  GLfloat x = p[1].f, y = p[2].f, z = p[3].f;
  m = &c->matrix_stack_ptr[c->matrix_mode]->m[0][0];
  m[0] *= x;
  m[1] *= y;
  m[2] *= z;
  m[4] *= x;
  m[5] *= y;
  m[6] *= z;
  m[8] *= x;
  m[9] *= y;
  m[10] *= z;
  m[12] *= x;
  m[13] *= y;
  m[14] *= z;
  gl_matrix_update(c);
}
void glopViewport(GLContext* c, GLParam* p)
{
  int w, h, xmin, ymin, xsize_req, ysize_req;
  xmin = p[1].i;
  ymin = p[2].i;
  w = p[3].i;
  h = p[4].i;
  /* we may need to resize the zbuffer */
  if (c->viewport.xmin != xmin ||
      c->viewport.ymin != ymin ||
      c->viewport.w != w ||
      c->viewport.h != h) {
    xsize_req = xmin + w;
    ysize_req = ymin + h;
#if 0
    if (c->gl_resize_viewport && c->gl_resize_viewport(c, &xsize_req, &ysize_req) != 0) {
      gl_fatal_error("glViewport: error while resizing display");
    }
#endif
    w = xsize_req - xmin;
    h = ysize_req - ymin;
    if (w <= 0 || h <= 0) {
      gl_fatal_error("glViewport: size too small");
    }
    tgl_trace("glViewport: %d %d %d %d\n", xmin, ymin, w, h);
    c->viewport.xmin = xmin;
    c->viewport.ymin = ymin;
    c->viewport.w = w;
    c->viewport.h = h;
    c->viewport.updated = 1;
  }
}
void setFrustum(float* m, float l, float r, float b, float t, float n, float f)
{
  m[0] = 2 * n / (r - l);
  m[2] = (r + l) / (r - l);
  m[5] = 2 * n / (t - b);
  m[6] = (t + b) / (t - b);
  m[10] = -(f + n) / (f - n);
  m[11] = -(2 * f * n) / (f - n);
  m[14] = -1;
}
void glopFrustum(GLContext* c, GLParam* p)
{
#if 1
  M4 m = {0};
  setFrustum(&m.m[0][0], p[1].f, p[2].f, p[3].f, p[4].f, p[5].f, p[6].f);
  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
#else
  GLfloat* r;
  M4 m;
  GLfloat left = p[1].f;
  GLfloat right = p[2].f;
  GLfloat bottom = p[3].f;
  GLfloat top = p[4].f;
  GLfloat nearv = p[5].f;
  GLfloat farp = p[6].f;
  GLfloat x, y, A, B, C, D;
  x = (float)((2.0 * nearv) / (right - left));
  y = (float)((2.0 * nearv) / (top - bottom));
  A = (right + left) / (right - left);
  B = (top + bottom) / (top - bottom);
  C = -(farp + nearv) / (farp - nearv);
  D = -(2.0f * farp * nearv) / (farp - nearv);
  r = &m.m[0][0];
  r[0] = x;
  r[1] = 0;
  r[2] = A;
  r[3] = 0;
  r[4] = 0;
  r[5] = y;
  r[6] = B;
  r[7] = 0;
  r[8] = 0;
  r[9] = 0;
  r[10] = C;
  r[11] = D;
  r[12] = 0;
  r[13] = 0;
  r[14] = -1;
  r[15] = 0;
  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
#endif
}
void setOrtho(float* m, float l, float r, float b, float t, float n, float f)
{
  m[0] = 2 / (r - l);
  m[3] = -(r + l) / (r - l);
  m[5] = 2 / (t - b);
  m[7] = -(t + b) / (t - b);
  m[10] = -2 / (f - n);
  m[11] = -(f + n) / (f - n);
  m[15] = 1;
}
void glopOrtho(GLContext* c, GLParam* p)
{
#if 1
  M4 m = {0};
  setOrtho(&m.m[0][0], p[1].f, p[2].f, p[3].f, p[4].f, p[5].f, p[6].f);
  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
#else
  GLfloat* r;
  M4 m = {0};
  GLfloat left = p[1].f;
  GLfloat right = p[2].f;
  GLfloat bottom = p[3].f;
  GLfloat top = p[4].f;
  GLfloat nearv = p[5].f;
  GLfloat farp = p[6].f;
  GLfloat x, y, z;
  x = (float)(2.0 / (right - left));
  y = (float)(2.0 / (top - bottom));
  z = (float)(2.0 / (farp - nearv));
  r = &m.m[0][0];
  r[0] = x;
  r[1] = 0;
  r[2] = 0;
  r[3] = -(right + left) * 0.5 * x;
  r[4] = 0;
  r[5] = y;
  r[6] = 0;
  r[7] = -(top + bottom) * 0.5 * y;
  r[8] = 0;
  r[9] = 0;
  r[10] = z;
  r[11] = -(farp + nearv) * 0.5 * z;
  r[12] = 0;
  r[13] = 0;
  r[14] = 0;
  r[15] = 1;
  gl_M4_MulLeft(c->matrix_stack_ptr[c->matrix_mode], &m);
  gl_matrix_update(c);
#endif
}
void glopMaterial(GLContext* c, GLParam* p)
{
  int mode = p[1].i;
  int type = p[2].i;
  GLfloat* v = &p[3].f;
  int i;
  GLMaterial* m;
  if (mode == GL_FRONT_AND_BACK) {
    p[1].i = GL_FRONT;
    glopMaterial(c, p);
    mode = GL_BACK;
  }
  if (mode == GL_FRONT) {
    m = &c->materials[0];
  }
  else {
    m = &c->materials[1];
  }
  switch (type) {
  case GL_EMISSION:
    for (i = 0; i < 4; i++) {
      m->emission.v[i] = v[i];
    }
    break;
  case GL_AMBIENT:
    for (i = 0; i < 4; i++) {
      m->ambient.v[i] = v[i];
    }
    break;
  case GL_DIFFUSE:
    for (i = 0; i < 4; i++) {
      m->diffuse.v[i] = v[i];
    }
    break;
  case GL_SPECULAR:
    for (i = 0; i < 4; i++) {
      m->specular.v[i] = v[i];
    }
    break;
  case GL_SHININESS:
    m->shininess = v[0];
    m->shininess_i = (int)((v[0] / 128.0f) * SPECULAR_BUFFER_RESOLUTION);
    break;
  case GL_AMBIENT_AND_DIFFUSE:
    for (i = 0; i < 4; i++) {
      m->diffuse.v[i] = v[i];
    }
    for (i = 0; i < 4; i++) {
      m->ambient.v[i] = v[i];
    }
    break;
  default:
    ASSERT(0);
  }
}
void glopColor(GLContext* c, GLParam* p)
{
  c->current_color.X = p[1].f;
  c->current_color.Y = p[2].f;
  c->current_color.Z = p[3].f;
  c->current_color.W = p[4].f;
  c->longcurrent_color[0] = p[5].ui;
  c->longcurrent_color[1] = p[6].ui;
  c->longcurrent_color[2] = p[7].ui;
  if (c->color_material_enabled) {
    GLParam q[7];
    q[0].op = OP_Material;
    q[1].i = c->current_color_material_mode;
    q[2].i = c->current_color_material_type;
    q[3].f = p[1].f;
    q[4].f = p[2].f;
    q[5].f = p[3].f;
    q[6].f = p[4].f;
    glopMaterial(c, q);
  }
}
void glopColorMaterial(GLContext* c, GLParam* p)
{
  int mode = p[1].i;
  int type = p[2].i;
  c->current_color_material_mode = mode;
  c->current_color_material_type = type;
}
void glopLight(GLContext* c, GLParam* p)
{
  int light = p[1].i;
  int type = p[2].i;
  V4 v;
  GLLight* l;
  int i;
  ASSERT(light >= GL_LIGHT0 && light < GL_LIGHT0 + MAX_LIGHTS);
  l = &c->lights[light - GL_LIGHT0];
  for (i = 0; i < 4; i++) {
    v.v[i] = p[3 + i].f;
  }
  switch (type) {
  case GL_AMBIENT:
    l->ambient = v;
    break;
  case GL_DIFFUSE:
    l->diffuse = v;
    break;
  case GL_SPECULAR:
    l->specular = v;
    break;
  case GL_POSITION: {
    V4 pos;
    gl_M4_MulV4(&pos, c->matrix_stack_ptr[0], &v);
    l->position = pos;
    if (l->position.v[3] == 0) {
      l->norm_position.X = pos.X;
      l->norm_position.Y = pos.Y;
      l->norm_position.Z = pos.Z;
      gl_V3_Norm(&l->norm_position);
    }
  }
  break;
  case GL_SPOT_DIRECTION:
    for (i = 0; i < 3; i++) {
      l->spot_direction.v[i] = v.v[i];
      l->norm_spot_direction.v[i] = v.v[i];
    }
    gl_V3_Norm(&l->norm_spot_direction);
    break;
  case GL_SPOT_EXPONENT:
    l->spot_exponent = v.v[0];
    break;
  case GL_SPOT_CUTOFF: {
    GLfloat a = v.v[0];
    ASSERT(a == 180 || (a >= 0 && a <= 90));
    l->spot_cutoff = a;
    if (a != 180) {
      l->cos_spot_cutoff = (float)cos(a * M_PI / 180.0);
    }
  }
  break;
  case GL_CONSTANT_ATTENUATION:
    l->attenuation[0] = v.v[0];
    break;
  case GL_LINEAR_ATTENUATION:
    l->attenuation[1] = v.v[0];
    break;
  case GL_QUADRATIC_ATTENUATION:
    l->attenuation[2] = v.v[0];
    break;
  default:
    ASSERT(0);
  }
}
void glopLightModel(GLContext* c, GLParam* p)
{
  int pname = p[1].i;
  GLfloat* v = &p[2].f;
  int i;
  switch (pname) {
  case GL_LIGHT_MODEL_AMBIENT:
    for (i = 0; i < 4; i++) {
      c->ambient_light_model.v[i] = v[i];
    }
    break;
  case GL_LIGHT_MODEL_LOCAL_VIEWER:
    c->local_light_model = (int)v[0];
    break;
  case GL_LIGHT_MODEL_TWO_SIDE:
    c->light_model_two_side = (int)v[0];
    break;
  default:
    tgl_warning("glopLightModel: illegal pname: 0x%x\n", pname);
    //ASSERT(0);
    break;
  }
}
void glopClear(GLContext* c, GLParam* p)
{
  int mask = p[1].i;
  int z = 0;
  int r = (int)(c->clear_color.v[0] * 65535);
  int g = (int)(c->clear_color.v[1] * 65535);
  int b = (int)(c->clear_color.v[2] * 65535);
  int clear_z = mask & GL_DEPTH_BUFFER_BIT;
  int clear_color = mask & GL_COLOR_BUFFER_BIT;
  int color, y;
  ushort color_s;
  ZBuffer* zb = c->zb;
  if (clear_z) {
    memset_s(zb->zbuf, z, zb->w * zb->h);
  }
  if (clear_color) {
    switch (zb->bpp) {
    case 15:
      RGB_TO_PIXEL15(&color_s, r, g, b);
      for (y = 0; y < zb->h; y++) {
        memset_s(zb->pbuf + y * zb->step, color_s, zb->w);
      }
      break;
    case 16:
      RGB_TO_PIXEL16(&color_s, r, g, b);
      for (y = 0; y < zb->h; y++) {
        memset_s(zb->pbuf + y * zb->step, color_s, zb->w);
      }
      break;
    case 24:
      for (y = 0; y < zb->h; y++) {
        memset_RGB24(zb->pbuf + y * zb->step, r >> 8, g >> 8, b >> 8, zb->w);
      }
      break;
    case 32:
      RGB_TO_PIXEL32(&color, r, g, b);
      for (y = 0; y < zb->h; y++) {
        memset_l(zb->pbuf + y * zb->step, color, zb->w);
      }
      break;
    default:
      ASSERT(0);
      break;
    }
  }
}
void glopClearColor(GLContext* c, GLParam* p)
{
  c->clear_color.v[0] = p[1].f;
  c->clear_color.v[1] = p[2].f;
  c->clear_color.v[2] = p[3].f;
  c->clear_color.v[3] = p[4].f;
}
void glopClearDepth(GLContext* c, GLParam* p)
{
  c->clear_depth = p[1].f;
}
void glopShadeModel(GLContext* c, GLParam* p)
{
  int code = p[1].i;
  c->current_shade_model = code;
}
void glopCullFace(GLContext* c, GLParam* p)
{
  int code = p[1].i;
  c->current_cull_face = code;
}
void glopFrontFace(GLContext* c, GLParam* p)
{
  int code = p[1].i;
  c->current_front_face = code;
}
void glopPolygonMode(GLContext* c, GLParam* p)
{
  int face = p[1].i;
  int mode = p[2].i;
  switch (face) {
  case GL_BACK:
    c->polygon_mode_back = mode;
    break;
  case GL_FRONT:
    c->polygon_mode_front = mode;
    break;
  case GL_FRONT_AND_BACK:
    c->polygon_mode_front = mode;
    c->polygon_mode_back = mode;
    break;
  default:
    assert(0);
  }
}
void glopHint(GLContext* c, GLParam* p)
{
#if 0
  int target = p[1].i;
  int mode = p[2].i;
  /* do nothing */
#endif
}
void glopArrayElement(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopEnableClientState(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopDisableClientState(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopVertexPointer(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopColorPointer(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopNormalPointer(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopTexCoordPointer(GLContext* c, GLParam* p)
{
  ASSERT(0);
}
void glopPolygonOffset(GLContext* c, GLParam* p)
{
  c->offset_factor = p[1].f;
  c->offset_units = p[2].f;
}
static GLTexture* find_texture(GLContext* c, int h)
{
  GLTexture* t;
  t = c->shared_state.texture_hash_table[h % TEXTURE_HASH_TABLE_SIZE];
  while (t != NULL) {
    if (t->handle == h) {
      return t;
    }
    t = t->next;
  }
  return NULL;
}
static void free_texture(GLContext* c, int h)
{
  GLTexture* t, **ht;
  GLImage* im;
  int i;
  t = find_texture(c, h);
  if (t->prev == NULL) {
    ht = &c->shared_state.texture_hash_table
        [t->handle % TEXTURE_HASH_TABLE_SIZE];
    *ht = t->next;
  }
  else {
    t->prev->next = t->next;
  }
  if (t->next != NULL) {
    t->next->prev = t->prev;
  }
  for (i = 0; i < MAX_TEXTURE_LEVELS; i++) {
    im = &t->images[i];
    if (im->pixmap != NULL) {
      gl_free(im->pixmap);
    }
  }
  gl_free(t);
}
GLTexture* alloc_texture(GLContext* c, int h)
{
  GLTexture* t, **ht;
  t = (GLTexture*)gl_malloc(sizeof(GLTexture));
  ht = &c->shared_state.texture_hash_table[h % TEXTURE_HASH_TABLE_SIZE];
  t->next = *ht;
  t->prev = NULL;
  if (t->next != NULL) {
    t->next->prev = t;
  }
  *ht = t;
  t->handle = h;
  return t;
}
void initSharedState(GLContext* c)
{
  GLSharedState* s = &c->shared_state;
  s->lists = (GLList**)gl_malloc(sizeof(GLList*) * MAX_DISPLAY_LISTS);
  s->texture_hash_table = (GLTexture**)gl_malloc(sizeof(GLTexture*) * TEXTURE_HASH_TABLE_SIZE);
  alloc_texture(c, 0);
}
void endSharedState(GLContext* c)
{
  GLSharedState* s = &c->shared_state;
  int i;
  for (i = 0; i < MAX_DISPLAY_LISTS; i++) {
    /* TODO */
  }
  gl_free(s->lists);
  gl_free(s->texture_hash_table);
}
void glInitTextures(GLContext* c)
{
  /* textures */
  c->texture_2d_enabled = 0;
  c->current_texture = find_texture(c, 0);
}
void APIENTRY glGenTextures(int n, unsigned int* textures)
{
  GLContext* c = gl_get_context();
  int max, i;
  GLTexture* t;
  max = 0;
  for (i = 0; i < TEXTURE_HASH_TABLE_SIZE; i++) {
    t = c->shared_state.texture_hash_table[i];
    while (t != NULL) {
      if (t->handle > max) {
        max = t->handle;
      }
      t = t->next;
    }
  }
  for (i = 0; i < n; i++) {
    textures[i] = max + i + 1;
  }
}
void glopBindTexture(GLContext* c, GLParam* p)
{
  int target = p[1].i;
  int texture = p[2].i;
  GLTexture* t;
  assert(target == GL_TEXTURE_2D && texture >= 0);
  t = find_texture(c, texture);
  if (t == NULL) {
    t = alloc_texture(c, texture);
  }
  c->current_texture = t;
}
/*
 * image conversion
 */
void gl_convertRGB_to_5R6G5B(unsigned short* pixmap, unsigned char* rgb,
    int w, int h)
{
  int i, n;
  unsigned char* p;
  p = rgb;
  n = w * h;
  for (i = 0; i < n; i++) {
    pixmap[i] = ((p[0] & 0xF8) << 8) | ((p[1] & 0xFC) << 3) | ((p[2] & 0xF8) >> 3);
    p += 3;
  }
}
void gl_convertRGB_to_8A8R8G8B(unsigned int* pixmap, unsigned char* rgb,
    int w, int h)
{
  int i, n;
  unsigned char* p;
  p = rgb;
  n = w * h;
  for (i = 0; i < n; i++) {
    pixmap[i] = (((unsigned int)p[0]) << 16) |
        (((unsigned int)p[1]) << 8) |
        (((unsigned int)p[2]));
    p += 3;
  }
}
/*
 * linear interpolation with xf,yf normalized to 2^16
 */
#define INTERP_NORM_BITS  16
#define INTERP_NORM       (1 << INTERP_NORM_BITS)
static CC_INLINE int interpolate1(int v00, int v01, int v10, int xf, int yf)
{
  return v00 + (((v01 - v00) * xf + (v10 - v00) * yf) >> INTERP_NORM_BITS);
}
/*
 * TODO: more accurate resampling
 */
void gl_resizeImage(unsigned char* dest, int xsize_dest, int ysize_dest,
    unsigned char* src, int xsize_src, int ysize_src)
{
  unsigned char* pix, *pix_src;
  GLfloat x1, y1, x1inc, y1inc;
  int xi, yi, j, xf, yf, x, y;
  pix = dest;
  pix_src = src;
  x1inc = (GLfloat)(xsize_src - 1) / (GLfloat)(xsize_dest - 1);
  y1inc = (GLfloat)(ysize_src - 1) / (GLfloat)(ysize_dest - 1);
  y1 = 0;
  for (y = 0; y < ysize_dest; y++) {
    x1 = 0;
    for (x = 0; x < xsize_dest; x++) {
      xi = (int) x1;
      yi = (int) y1;
      xf = (int)((x1 - floor(x1)) * INTERP_NORM);
      yf = (int)((y1 - floor(y1)) * INTERP_NORM);
      if ((xf + yf) <= INTERP_NORM) {
        for (j = 0; j < 3; j++) {
          pix[j] = interpolate1(pix_src[(yi * xsize_src + xi) * 3 + j],
              pix_src[(yi * xsize_src + xi + 1) * 3 + j],
              pix_src[((yi + 1) * xsize_src + xi) * 3 + j],
              xf, yf);
        }
      }
      else {
        xf = INTERP_NORM - xf;
        yf = INTERP_NORM - yf;
        for (j = 0; j < 3; j++) {
          pix[j] = interpolate1(pix_src[((yi + 1) * xsize_src + xi + 1) * 3 + j],
              pix_src[((yi + 1) * xsize_src + xi) * 3 + j],
              pix_src[(yi * xsize_src + xi + 1) * 3 + j],
              xf, yf);
        }
      }
      pix += 3;
      x1 += x1inc;
    }
    y1 += y1inc;
  }
}
#define FRAC_BITS 16
/* resizing with no interlating nor nearest pixel */
void gl_resizeImageNoInterpolate(unsigned char* dest, int xsize_dest, int ysize_dest,
    unsigned char* src, int xsize_src, int ysize_src)
{
  unsigned char* pix, *pix_src, *pix1;
  int x1, y1, x1inc, y1inc;
  int xi, yi, x, y;
  pix = dest;
  pix_src = src;
  x1inc = (int)((GLfloat)((xsize_src) << FRAC_BITS) / (GLfloat)(xsize_dest));
  y1inc = (int)((GLfloat)((ysize_src) << FRAC_BITS) / (GLfloat)(ysize_dest));
  y1 = 0;
  for (y = 0; y < ysize_dest; y++) {
    x1 = 0;
    for (x = 0; x < xsize_dest; x++) {
      xi = x1 >> FRAC_BITS;
      yi = y1 >> FRAC_BITS;
      pix1 = pix_src + (yi * xsize_src + xi) * 3;
      pix[0] = pix1[0];
      pix[1] = pix1[1];
      pix[2] = pix1[2];
      pix += 3;
      x1 += x1inc;
    }
    y1 += y1inc;
  }
}
void glopTexImage2D(GLContext* c, GLParam* p)
{
  int target = p[1].i;
  int level = p[2].i;
  int components = p[3].i;
  int width = p[4].i;
  int height = p[5].i;
  int border = p[6].i;
  int format = p[7].i;
  int type = p[8].i;
  unsigned char* pixels = (unsigned char*)p[9].p;
  GLImage* im;
  unsigned char* pixels1;
  int do_free;
  ZBuffer* zb = c->zb;
  if (!(target == GL_TEXTURE_2D && level == 0 && components == 3 &&
      border == 0 && format == GL_RGB &&
      type == GL_UNSIGNED_BYTE)) {
    gl_fatal_error("glTexImage2D: combinaison of parameters not handled");
  }
  do_free = 0;
  if (width != 256 || height != 256) {
    pixels1 = (unsigned char*)gl_malloc(256 * 256 * 3);
    /* no interpolation is done here to respect the original image aliasing ! */
    gl_resizeImageNoInterpolate(pixels1, 256, 256, pixels, width, height);
    do_free = 1;
    width = 256;
    height = 256;
  }
  else {
    pixels1 = pixels;
  }
  im = &c->current_texture->images[level];
  im->w = width;
  im->h = height;
  if (im->pixmap != NULL) {
    gl_free(im->pixmap);
  }
  switch (zb->bpp) {
  case 24:
    im->pixmap = gl_malloc(width * height * 3);
    if (im->pixmap) {
      memcpy(im->pixmap, pixels1, width * height * 3);
    }
    break;
  case 32:
    im->pixmap = gl_malloc(width * height * 4);
    if (im->pixmap) {
      gl_convertRGB_to_8A8R8G8B((uint*)im->pixmap, pixels1, width, height);
    }
    break;
  case 15:
  case 16:
    im->pixmap = gl_malloc(width * height * 2);
    if (im->pixmap) {
      gl_convertRGB_to_5R6G5B((unsigned short*)im->pixmap, pixels1, width, height);
    }
    break;
  }
  if (do_free) {
    gl_free(pixels1);
  }
}
/* TODO: not all tests are done */
void glopTexEnv(GLContext* c, GLParam* p)
{
  int target = p[1].i;
  int pname = p[2].i;
  int param = p[3].i;
  if (target != GL_TEXTURE_ENV) {
error:
    gl_fatal_error("glTexParameter: unsupported option");
  }
  if (pname != GL_TEXTURE_ENV_MODE) {
    goto error;
  }
  if (param != GL_DECAL) {
    goto error;
  }
}
/* TODO: not all tests are done */
void glopTexParameter(GLContext* c, GLParam* p)
{
  int target = p[1].i;
  int pname = p[2].i;
  int param = p[3].i;
  if (target != GL_TEXTURE_2D) {
error:
    gl_fatal_error("glTexParameter: unsupported option");
  }
  switch (pname) {
  case GL_TEXTURE_WRAP_S:
  case GL_TEXTURE_WRAP_T:
    if (param != GL_REPEAT) {
      goto error;
    }
    break;
  }
}
void glopPixelStore(GLContext* c, GLParam* p)
{
  int pname = p[1].i;
  int param = p[2].i;
  if (pname != GL_UNPACK_ALIGNMENT ||
      param != 1) {
    gl_fatal_error("glPixelStore: unsupported option");
  }
}
int ZB_Init(ZBuffer* zb, int h, int w, uchar* pbuf, int step, int bpp)
{
  if (h != zb->h || w != zb->w || zb->pbuf != pbuf || zb->step != step || zb->bpp != bpp) {
    zb->h = h;
    zb->w = w;
    zb->pbuf = pbuf;
    zb->step = step;
    zb->bpp = bpp;
    zb->cn = BPP2CN(bpp);
    zb->current_texture = NULL;
    zb->zbuf = (unsigned short*)realloc(zb->zbuf, h * w * sizeof(unsigned short));
  }
  return 0;
}
void glopPointSize(GLContext* c, GLParam* p)
{
  c->point_size = p[1].i;
}
/*
画虚线。
首先，使用glEnable(GL_LINE_STIPPLE);来启动虚线模式（使用glDisable(GL_LINE_STIPPLE)可以关闭之）。
然后，使用glLineStipple来设置虚线的样式。
void glLineStipple(GLint factor, GLushort pattern);
pattern是由1和0组成的长度为16的序列，从最低位开始看，如果为1，
则直线上接下来应该画的factor个点将被画为实的；如果为0，则直线上接下来应该画的factor个点将被画为虚的。
*/
void glLineStipple(GLint factor, GLushort pattern)
{
}
void glLineWidth(GLfloat width)
{
}

