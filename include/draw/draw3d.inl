//=====================================================================
// 数学库：此部分应该不用详解，熟悉 D3D 矩阵变换即可
//=====================================================================
int CMID(int x, int min, int max)
{
  return (x < min) ? min : ((x > max) ? max : x);
}
// 计算插值：t 为 [0, 1] 之间的数值
float interp(float x1, float x2, float t)
{
  return x1 + (x2 - x1) * t;
}
// | v |
float vector_length(const vector_t* v)
{
  float sq = v->x * v->x + v->y * v->y + v->z * v->z;
  return (float)sqrt(sq);
}
// z = x + y
void vector_add(vector_t* z, const vector_t* x, const vector_t* y)
{
  z->x = x->x + y->x;
  z->y = x->y + y->y;
  z->z = x->z + y->z;
  z->w = 1.0;
}
// z = x - y
void vector_sub(vector_t* z, const vector_t* x, const vector_t* y)
{
  z->x = x->x - y->x;
  z->y = x->y - y->y;
  z->z = x->z - y->z;
  z->w = 1.0;
}
// 矢量点乘
float vector_dotproduct(const vector_t* x, const vector_t* y)
{
  return x->x * y->x + x->y * y->y + x->z * y->z;
}
// 矢量叉乘
void vector_crossproduct(vector_t* z, const vector_t* x, const vector_t* y)
{
  float m1, m2, m3;
  m1 = x->y * y->z - x->z * y->y;
  m2 = x->z * y->x - x->x * y->z;
  m3 = x->x * y->y - x->y * y->x;
  z->x = m1;
  z->y = m2;
  z->z = m3;
  z->w = 1.0f;
}
// 矢量插值，t取值 [0, 1]
void vector_interp(vector_t* z, const vector_t* x1, const vector_t* x2, float t)
{
  z->x = interp(x1->x, x2->x, t);
  z->y = interp(x1->y, x2->y, t);
  z->z = interp(x1->z, x2->z, t);
  z->w = 1.0f;
}
// 矢量归一化
void vector_normalize(vector_t* v)
{
  float length = vector_length(v);
  if (length != 0.0f) {
    float inv = 1.0f / length;
    v->x *= inv;
    v->y *= inv;
    v->z *= inv;
  }
}
// c = a + b
void matrix_add(matrix_t* c, const matrix_t* a, const matrix_t* b)
{
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      c->m[i][j] = a->m[i][j] + b->m[i][j];
    }
  }
}
// c = a - b
void matrix_sub(matrix_t* c, const matrix_t* a, const matrix_t* b)
{
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      c->m[i][j] = a->m[i][j] - b->m[i][j];
    }
  }
}
// c = a * b
void matrix_mul(matrix_t* c, const matrix_t* a, const matrix_t* b)
{
  matrix_t z;
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      z.m[j][i] = (a->m[j][0] * b->m[0][i]) +
          (a->m[j][1] * b->m[1][i]) +
          (a->m[j][2] * b->m[2][i]) +
          (a->m[j][3] * b->m[3][i]);
    }
  }
  c[0] = z;
}
// c = a * f
void matrix_scale(matrix_t* c, const matrix_t* a, float f)
{
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      c->m[i][j] = a->m[i][j] * f;
    }
  }
}
// y = x * m
void matrix_apply(vector_t* y, const vector_t* x, const matrix_t* m)
{
  float X = x->x, Y = x->y, Z = x->z, W = x->w;
  y->x = X * m->m[0][0] + Y * m->m[1][0] + Z * m->m[2][0] + W * m->m[3][0];
  y->y = X * m->m[0][1] + Y * m->m[1][1] + Z * m->m[2][1] + W * m->m[3][1];
  y->z = X * m->m[0][2] + Y * m->m[1][2] + Z * m->m[2][2] + W * m->m[3][2];
  y->w = X * m->m[0][3] + Y * m->m[1][3] + Z * m->m[2][3] + W * m->m[3][3];
}
void matrix_set_identity(matrix_t* m)
{
  m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = 1.0f;
  m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
  m->m[1][0] = m->m[1][2] = m->m[1][3] = 0.0f;
  m->m[2][0] = m->m[2][1] = m->m[2][3] = 0.0f;
  m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
}
void matrix_set_zero(matrix_t* m)
{
  m->m[0][0] = m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
  m->m[1][0] = m->m[1][1] = m->m[1][2] = m->m[1][3] = 0.0f;
  m->m[2][0] = m->m[2][1] = m->m[2][2] = m->m[2][3] = 0.0f;
  m->m[3][0] = m->m[3][1] = m->m[3][2] = m->m[3][3] = 0.0f;
}
// 平移变换
void matrix_set_translate(matrix_t* m, float x, float y, float z)
{
  matrix_set_identity(m);
  m->m[3][0] = x;
  m->m[3][1] = y;
  m->m[3][2] = z;
}
// 缩放变换
void matrix_set_scale(matrix_t* m, float x, float y, float z)
{
  matrix_set_identity(m);
  m->m[0][0] = x;
  m->m[1][1] = y;
  m->m[2][2] = z;
}
// 旋转矩阵
void matrix_set_rotate(matrix_t* m, float x, float y, float z, float theta)
{
  float qsin = (float)sin(theta * 0.5f);
  float qcos = (float)cos(theta * 0.5f);
  vector_t vec = { x, y, z, 1.0f };
  float w = qcos;
  vector_normalize(&vec);
  x = vec.x * qsin;
  y = vec.y * qsin;
  z = vec.z * qsin;
  m->m[0][0] = 1 - 2 * y * y - 2 * z * z;
  m->m[1][0] = 2 * x * y - 2 * w * z;
  m->m[2][0] = 2 * x * z + 2 * w * y;
  m->m[0][1] = 2 * x * y + 2 * w * z;
  m->m[1][1] = 1 - 2 * x * x - 2 * z * z;
  m->m[2][1] = 2 * y * z - 2 * w * x;
  m->m[0][2] = 2 * x * z - 2 * w * y;
  m->m[1][2] = 2 * y * z + 2 * w * x;
  m->m[2][2] = 1 - 2 * x * x - 2 * y * y;
  m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
  m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
  m->m[3][3] = 1.0f;
}
// 设置摄像机
void matrix_set_lookat(matrix_t* m, const vector_t* eye, const vector_t* at, const vector_t* up)
{
  vector_t xaxis, yaxis, zaxis;
  vector_sub(&zaxis, at, eye);
  vector_normalize(&zaxis);
  vector_crossproduct(&xaxis, up, &zaxis);
  vector_normalize(&xaxis);
  vector_crossproduct(&yaxis, &zaxis, &xaxis);
  m->m[0][0] = xaxis.x;
  m->m[1][0] = xaxis.y;
  m->m[2][0] = xaxis.z;
  m->m[3][0] = -vector_dotproduct(&xaxis, eye);
  m->m[0][1] = yaxis.x;
  m->m[1][1] = yaxis.y;
  m->m[2][1] = yaxis.z;
  m->m[3][1] = -vector_dotproduct(&yaxis, eye);
  m->m[0][2] = zaxis.x;
  m->m[1][2] = zaxis.y;
  m->m[2][2] = zaxis.z;
  m->m[3][2] = -vector_dotproduct(&zaxis, eye);
  m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
  m->m[3][3] = 1.0f;
}
// D3DXMatrixPerspectiveFovLH
void matrix_set_perspective(matrix_t* m, float fovy, float aspect, float zn, float zf)
{
  float fax = 1.0f / (float)tan(fovy * 0.5f);
  matrix_set_zero(m);
  m->m[0][0] = (float)(fax / aspect);
  m->m[1][1] = (float)(-fax); // 上下翻转
  m->m[2][2] = zf / (zf - zn);
  m->m[3][2] = - zn * zf / (zf - zn);
  m->m[2][3] = 1;
}
//=====================================================================
// 坐标变换
//=====================================================================
// 矩阵更新，计算 transform = world * view * projection
void transform_update(transform_t* ts)
{
  matrix_t m;
  matrix_mul(&m, &ts->world, &ts->view);
  matrix_mul(&ts->transform, &m, &ts->projection);
}
void transform_set_projection(transform_t* ts, int width, int height)
{
  float aspect = (float)width / ((float)height);
  matrix_set_perspective(&ts->projection, 3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
  ts->w = (float)width;
  ts->h = (float)height;
  transform_update(ts);
}
// 初始化，设置屏幕长宽
void transform_init(transform_t* ts)
{
  matrix_set_identity(&ts->world);
  matrix_set_identity(&ts->view);
}
// 将矢量 x 进行 project
void transform_apply(const transform_t* ts, vector_t* y, const vector_t* x)
{
  matrix_apply(y, x, &ts->transform);
}
// 检查齐次坐标同 cvv 的边界用于视锥裁剪
int transform_check_cvv(const vector_t* v)
{
  float w = v->w;
  int check = 0;
  if (v->z < 0.0f) {
    check |= 1;
  }
  if (v->z > w) {
    check |= 2;
  }
  if (v->x < -w) {
    check |= 4;
  }
  if (v->x > w) {
    check |= 8;
  }
  if (v->y < -w) {
    check |= 16;
  }
  if (v->y > w) {
    check |= 32;
  }
  return check;
}
// 归一化，得到屏幕坐标
void transform_homogenize(const transform_t* ts, vector_t* y, const vector_t* x)
{
  float rhw = 1.0f / x->w;
  y->x = (x->x * rhw + 1.0f) * ts->w * 0.5f;
  y->y = (1.0f - x->y * rhw) * ts->h * 0.5f;
  y->z = x->z * rhw;
  y->w = 1.0f;
}
//=====================================================================
// 几何计算：顶点、扫描线、边缘、矩形、步长计算
//=====================================================================
typedef struct {
  ImVec4 pos;
  ImVec2 uv;
  ImVec4 color;
  float rhw;
} vertex_t;
typedef struct {
  vertex_t v, v1, v2;
} edge_t;
typedef struct {
  float top, bottom;
  edge_t left, right;
} trapezoid_t;
typedef struct {
  vertex_t v, step;
  int x, y, w;
} scanline_t;
void vertex_rhw_init(vertex_t* v)
{
  float rhw = 1.0f / v->pos.w;
  v->rhw = rhw;
  v->uv.x *= rhw;
  v->uv.y *= rhw;
  v->color.x *= rhw;
  v->color.y *= rhw;
  v->color.z *= rhw;
  v->color.w *= rhw;
}
void vertex_interp(vertex_t* y, const vertex_t* x1, const vertex_t* x2, float t)
{
  vector_interp(&y->pos, &x1->pos, &x2->pos, t);
  y->uv.x = interp(x1->uv.x, x2->uv.x, t);
  y->uv.y = interp(x1->uv.y, x2->uv.y, t);
  y->color.x = interp(x1->color.x, x2->color.x, t);
  y->color.y = interp(x1->color.y, x2->color.y, t);
  y->color.z = interp(x1->color.z, x2->color.z, t);
  y->rhw = interp(x1->rhw, x2->rhw, t);
}
void vertex_division(vertex_t* y, const vertex_t* x1, const vertex_t* x2, float w)
{
  float inv = 1.0f / w;
  y->pos.x = (x2->pos.x - x1->pos.x) * inv;
  y->pos.y = (x2->pos.y - x1->pos.y) * inv;
  y->pos.z = (x2->pos.z - x1->pos.z) * inv;
  y->pos.w = (x2->pos.w - x1->pos.w) * inv;
  y->uv.x = (x2->uv.x - x1->uv.x) * inv;
  y->uv.y = (x2->uv.y - x1->uv.y) * inv;
  y->color.x = (x2->color.x - x1->color.x) * inv;
  y->color.y = (x2->color.y - x1->color.y) * inv;
  y->color.z = (x2->color.z - x1->color.z) * inv;
  y->rhw = (x2->rhw - x1->rhw) * inv;
}
void vertex_add(vertex_t* y, const vertex_t* x)
{
  y->pos.x += x->pos.x;
  y->pos.y += x->pos.y;
  y->pos.z += x->pos.z;
  y->pos.w += x->pos.w;
  y->rhw += x->rhw;
  y->uv.x += x->uv.x;
  y->uv.y += x->uv.y;
  y->color.x += x->color.x;
  y->color.y += x->color.y;
  y->color.z += x->color.z;
}
// 根据三角形生成 0-2 个梯形，并且返回合法梯形的数量
int trapezoid_init_triangle(trapezoid_t* trap, const vertex_t* p1,
    const vertex_t* p2, const vertex_t* p3)
{
  const vertex_t* p;
  float k, x;
  if (p1->pos.y > p2->pos.y) {
    p = p1, p1 = p2, p2 = p;
  }
  if (p1->pos.y > p3->pos.y) {
    p = p1, p1 = p3, p3 = p;
  }
  if (p2->pos.y > p3->pos.y) {
    p = p2, p2 = p3, p3 = p;
  }
  if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) {
    return 0;
  }
  if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x) {
    return 0;
  }
  if (p1->pos.y == p2->pos.y) { // triangle down
    if (p1->pos.x > p2->pos.x) {
      p = p1, p1 = p2, p2 = p;
    }
    trap[0].top = p1->pos.y;
    trap[0].bottom = p3->pos.y;
    trap[0].left.v1 = *p1;
    trap[0].left.v2 = *p3;
    trap[0].right.v1 = *p2;
    trap[0].right.v2 = *p3;
    return (trap[0].top < trap[0].bottom) ? 1 : 0;
  }
  if (p2->pos.y == p3->pos.y) { // triangle up
    if (p2->pos.x > p3->pos.x) {
      p = p2, p2 = p3, p3 = p;
    }
    trap[0].top = p1->pos.y;
    trap[0].bottom = p3->pos.y;
    trap[0].left.v1 = *p1;
    trap[0].left.v2 = *p2;
    trap[0].right.v1 = *p1;
    trap[0].right.v2 = *p3;
    return (trap[0].top < trap[0].bottom) ? 1 : 0;
  }
  trap[0].top = p1->pos.y;
  trap[0].bottom = p2->pos.y;
  trap[1].top = p2->pos.y;
  trap[1].bottom = p3->pos.y;
  k = (p3->pos.y - p1->pos.y) / (p2->pos.y - p1->pos.y);
  x = p1->pos.x + (p2->pos.x - p1->pos.x) * k;
  if (x <= p3->pos.x) { // triangle left
    trap[0].left.v1 = *p1;
    trap[0].left.v2 = *p2;
    trap[0].right.v1 = *p1;
    trap[0].right.v2 = *p3;
    trap[1].left.v1 = *p2;
    trap[1].left.v2 = *p3;
    trap[1].right.v1 = *p1;
    trap[1].right.v2 = *p3;
  }
  else { // triangle right
    trap[0].left.v1 = *p1;
    trap[0].left.v2 = *p3;
    trap[0].right.v1 = *p1;
    trap[0].right.v2 = *p2;
    trap[1].left.v1 = *p1;
    trap[1].left.v2 = *p3;
    trap[1].right.v1 = *p2;
    trap[1].right.v2 = *p3;
  }
  return 2;
}
// 按照 Y 坐标计算出左右两条边纵坐标等于 Y 的顶点
void trapezoid_edge_interp(trapezoid_t* trap, float y)
{
  float s1 = trap->left.v2.pos.y - trap->left.v1.pos.y;
  float s2 = trap->right.v2.pos.y - trap->right.v1.pos.y;
  float t1 = (y - trap->left.v1.pos.y) / s1;
  float t2 = (y - trap->right.v1.pos.y) / s2;
  vertex_interp(&trap->left.v, &trap->left.v1, &trap->left.v2, t1);
  vertex_interp(&trap->right.v, &trap->right.v1, &trap->right.v2, t2);
}
// 根据左右两边的端点，初始化计算出扫描线的起点和步长
void trapezoid_init_scan_line(const trapezoid_t* trap, scanline_t* scanline, int y)
{
  float width = trap->right.v.pos.x - trap->left.v.pos.x;
  scanline->x = (int)(trap->left.v.pos.x + 0.5f);
  scanline->w = (int)(trap->right.v.pos.x + 0.5f) - scanline->x;
  scanline->y = y;
  scanline->v = trap->left.v;
  if (trap->left.v.pos.x >= trap->right.v.pos.x) {
    scanline->w = 0;
  }
  vertex_division(&scanline->step, &trap->left.v, &trap->right.v, width);
}
// 清空 framebuffer 和 zbuffer
void softgc_clear(softgc* g, COLOR clrbg)
{
  img_t* im = g->im;
  int h = im->h, w = im->w;
  int y, x;
  IRECT clip = iRectClip(g->sl->pclip, 0, 0, w, h);
  int h2 = RCH(&clip);
  int clra = 128, clrb = 233;
  //MYREALLOC(g->zbuf, h * w);
  for (y = clip.t; y < clip.b; y++) {
    uint32* dst = (uint32*)(im->tt.data + im->s * (g->flip_y ? y : (h-1-y)));
    //float* zbuf = g->zbuf + w * y;
    int y2 = y - clip.t;
    uint32 cc = clra + ((y - clip.t)) * (clrb-clra) / (h2);
    cc = (255 << 24) | (cc << 16) | (cc << 8) | cc;
    //if (clrbg == 0) {      cc = clrbg;    }
    for (x = 0; x < w; ++x) {
      dst[x] = cc;
      //zbuf[x] = 0.0f;
    }
  }
}
void ImVertex_to_vertex(vertex_t* t, const ImVertex* v1)
{
  t->pos = v1->pos;
  t->uv = v1->uv;
  t->rhw = v1->rhw;
  t->color.x = GetRV(v1->col) / 255.f;
  t->color.y = GetGV(v1->col) / 255.f;
  t->color.z = GetBV(v1->col) / 255.f;
  t->color.w = GetAV(v1->col) / 255.f;
}
// 根据坐标读取纹理
uint32 device_texture_read(const texture_t* tex, float u, float v)
{
  int x, y;
  u = u * tex->w;
  v = v * tex->h;
  x = (int)(u + 0.5f);
  y = (int)(v + 0.5f);
  x = CMID(x, 0, tex->w - 1);
  y = CMID(y, 0, tex->h - 1);
  return *(uint32*)(tex->data + tex->s * y + 4 * x);
}
// 绘制扫描线
void device_draw_scanline(softgc* g, info3d_t* info, scanline_t* scanline)
{
  img_t* im = g->im;
  int h = im->h, w = im->w;
  int state = info->state;
  uint32* framebuffer = (uint32*)(im->tt.data + scanline->y * im->s);
  float* zbuffer = g->zbuf + w * scanline->y;
  int x = scanline->x;
  int w0 = MIN(w, scanline->w);
  for (; w0 > 0; --w0, x++) {
    if (x >= 0 && x < w) {
      float rhw = scanline->v.rhw;
      if (rhw >= zbuffer[x]) {
        float inv_rhw = 1.0f / rhw;
        zbuffer[x] = rhw;
        if (state == GC_COLOR) {
          float r = scanline->v.color.x * inv_rhw;
          float g = scanline->v.color.y * inv_rhw;
          float b = scanline->v.color.z * inv_rhw;
          int R = (int)(r * 255.0f);
          int G = (int)(g * 255.0f);
          int B = (int)(b * 255.0f);
          R = CMID(R, 0, 255);
          G = CMID(G, 0, 255);
          B = CMID(B, 0, 255);
          framebuffer[x] = (R << 16) | (G << 8) | (B);
        }
        if (state == GC_TEXTURE) {
          float u = scanline->v.uv.x * inv_rhw;
          float v = scanline->v.uv.y * inv_rhw;
          uint32 cc = device_texture_read(info->tex, u, v);
          framebuffer[x] = cc;
        }
      }
    }
    vertex_add(&scanline->v, &scanline->step);
    if (x >= w) {
      break;
    }
  }
}
// 主渲染函数
void device_render_trap(softgc* g, info3d_t* info, trapezoid_t* trap)
{
  img_t* im = g->im;
  int h = im->h, w = im->w;
  scanline_t scanline;
  int j, top, bottom;
  top = (int)(trap->top + 0.5f);
  bottom = (int)(trap->bottom + 0.5f);
  for (j = top; j < bottom; j++) {
    if (j >= 0 && j < h) {
      trapezoid_edge_interp(trap, (float)j + 0.5f);
      trapezoid_init_scan_line(trap, &scanline, j);
      device_draw_scanline(g, info, &scanline);
    }
    if (j >= h) {
      break;
    }
  }
}
int draw_solid_line(img_t* im, const IRECT* pclip, double x1, double y1, double x2, double y2, COLOR clr)
{
  int i, steps, increx, increy, x, y, step = im->s >> 2;
  uint32* ptr;
  IRECT clip = iRectClip(pclip, 0, 0, im->w, im->h);
  IPOINT pt1 = iPOINT((int)x1, (int)y1);
  IPOINT pt2 = iPOINT((int)x2, (int)y2);
  i = ClipLine(&clip, &pt1, &pt2);
  if (!i) {
    return 0;
  }
  steps = (int)(MAX(ABS(pt2.x - pt1.x), ABS(pt2.y - pt1.y)));
  if (0 == steps) {
    return 0;
  }
  ptr = (uint32*)(im->tt.data + im->c * pt1.x + im->s * pt1.y);
  x = F2FIX(x1 - (pt1.x)), y = F2FIX(y1 - (pt1.y));
  increx = (F2FIX(x2 - x1)) / steps;
  increy = (F2FIX(y2 - y1)) / steps;
  for (i = 0; i < steps; ++i) {
    int ix = x >> 16, iy = y >> 16;
    //if ((1<<(i/factor))&pattern)
    //ASSERT(ix >= 0 && ix <= RCW(&rc));
    //ASSERT(iy >= 0 && iy <= RCH(&rc));
    ptr[ix + iy * step] = clr;
    x += increx, y += increy;
  }
  return 0;
}
void softgc_line(softgc* g, info3d_t* info, const ImVertex* v1, const ImVertex* v2)
{
  FPOINT4 p1, p2, c1, c2;
  transform_apply(&info->transform, &c1, &v1->pos);
  transform_apply(&info->transform, &c2, &v2->pos);
  if (transform_check_cvv(&c1) != 0) {
    return;
  }
  if (transform_check_cvv(&c2) != 0) {
    return;
  }
  // 归一化
  transform_homogenize(&info->transform, &p1, &c1);
  transform_homogenize(&info->transform, &p2, &c2);
  draw_solid_line(g->im, g->sl->pclip, p1.x, p1.y, p2.x, p2.y, info->clrfg);
}
void softgc_triangle(softgc* g, info3d_t* info, const ImVertex* v1, const ImVertex* v2, const ImVertex* v3)
{
  FPOINT4 p1, p2, p3, c1, c2, c3;
  int state = info->state;
  // 按照 Transform 变化
  transform_apply(&info->transform, &c1, &v1->pos);
  transform_apply(&info->transform, &c2, &v2->pos);
  transform_apply(&info->transform, &c3, &v3->pos);
  // 裁剪，注意此处可以完善为具体判断几个点在 cvv内以及同cvv相交平面的坐标比例
  // 进行进一步精细裁剪，将一个分解为几个完全处在 cvv内的三角形
  if (transform_check_cvv(&c1) != 0) {
    return;
  }
  if (transform_check_cvv(&c2) != 0) {
    return;
  }
  if (transform_check_cvv(&c3) != 0) {
    return;
  }
  // 归一化
  transform_homogenize(&info->transform, &p1, &c1);
  transform_homogenize(&info->transform, &p2, &c2);
  transform_homogenize(&info->transform, &p3, &c3);
  // 纹理或者色彩绘制
  switch (state) {
  case GC_TEXTURE:
  case GC_COLOR: {
    vertex_t t1, t2, t3;
    trapezoid_t traps[2];
    int n;
    ImVertex_to_vertex(&t1, v1);
    ImVertex_to_vertex(&t2, v2);
    ImVertex_to_vertex(&t3, v3);
    t1.pos = p1;
    t2.pos = p2;
    t3.pos = p3;
    t1.pos.w = c1.w;
    t2.pos.w = c2.w;
    t3.pos.w = c3.w;
    vertex_rhw_init(&t1); // 初始化 w
    vertex_rhw_init(&t2); // 初始化 w
    vertex_rhw_init(&t3); // 初始化 w
    // 拆分三角形为0-2个梯形，并且返回可用梯形数量
    n = trapezoid_init_triangle(traps, &t1, &t2, &t3);
    if (n >= 1) {
      device_render_trap(g, info, &traps[0]);
    }
    if (n >= 2) {
      device_render_trap(g, info, &traps[1]);
    }
  }
  break;
  case GC_WIREFRAME: { // 线框绘制
    COLOR clr = info->clrfg;
    draw_solid_line(g->im, g->sl->pclip, p1.x, p1.y, p2.x, p2.y, clr);
    draw_solid_line(g->im, g->sl->pclip, p1.x, p1.y, p3.x, p3.y, clr);
    draw_solid_line(g->im, g->sl->pclip, p3.x, p3.y, p2.x, p2.y, clr);
  }
  break;
  }
  return;
}
int softgc_primitive(softgc* g, info3d_t* info)
{
  img_t* im = g->im;
  int h = im->h, w = im->w;
  const uint16* idx = info->idx;
  const uint16* idx_end = idx + info->n;
  ImVertex* vtx = info->vtx;
  switch (info->type) {
  case GC_POINTS:
    for (; idx < idx_end; idx += 1) {
    }
    break;
  case GC_LINES:
    for (; idx < idx_end; idx += 2) {
      softgc_line(g, info, &vtx[idx[0]], &vtx[idx[1]]);
    }
    break;
  case GC_LINE_STRIP:
    for (; idx < idx_end - 1; idx += 1) {
      softgc_line(g, info, &vtx[idx[0]], &vtx[idx[1]]);
    }
    break;
  case GC_TRIANGLES:
    for (; idx < idx_end; idx += 3) {
      softgc_triangle(g, info, &vtx[idx[0]], &vtx[idx[1]], &vtx[idx[2]]);
    }
    break;
  case GC_TRIANGLE_STRIP:
    for (; idx < idx_end - 2; idx += 1) {
      softgc_triangle(g, info, &vtx[idx[0]], &vtx[idx[1]], &vtx[idx[2]]);
    }
    break;
  case GC_TRIANGLE_FAN:
    for (; idx < idx_end - 1; idx += 1) {
      softgc_triangle(g, info, &vtx[info->idx[0]], &vtx[idx[0]], &vtx[idx[1]]);
    }
    break;
  case GC_QUADS:
    for (; idx < idx_end; idx += 4) {
      softgc_triangle(g, info, &vtx[idx[0]], &vtx[idx[1]], &vtx[idx[2]]);
      softgc_triangle(g, info, &vtx[idx[2]], &vtx[idx[3]], &vtx[idx[0]]);
    }
    break;
  case GC_QUAD_STRIP:
    for (; idx < idx_end; idx += 2) {
      softgc_triangle(g, info, &vtx[idx[0]], &vtx[idx[1]], &vtx[idx[2]]);
      softgc_triangle(g, info, &vtx[idx[2]], &vtx[idx[3]], &vtx[idx[0]]);
    }
    break;
  }
  return 0;
}

