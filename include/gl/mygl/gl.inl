#ifndef _GL_INL_
#define _GL_INL_
#define GLCN 3
//#define PSZB GLCN
#include <assert.h>
void myassert(int x)
{
  if (!x) {
    int asdf = 0;
  }
}
#define ASSERT assert
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
typedef unsigned char uchar;
#include "glop.inl"
/* glVertex */
void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  // glopVertex
  GLParam p[5];
  p[0].op = OP_Vertex;
  p[1].f = x;
  p[2].f = y;
  p[3].f = z;
  p[4].f = w;
  gl_add_op(p);
}
void glVertex2f(GLfloat x, GLfloat y)
{
  glVertex4f(x, y, 0, 1);
}
void glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
  glVertex4f(x, y, z, 1);
}
// void glVertex2fv(const GLfloat *v);
#define DEF_GLVERTEXV(t, type) \
  void glVertex2 ## t ## v(const type *v) {glVertex4f((GLfloat)v[0], (GLfloat)v[1], 0, 1);}\
  void glVertex3 ## t ## v(const type *v) {glVertex4f((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2], 1);}\
  void glVertex4 ## t ## v(const type *v) {glVertex4f((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2], (GLfloat)v[3]);}

DEF_GLVERTEXV(d, GLdouble);
DEF_GLVERTEXV(f, GLfloat);
DEF_GLVERTEXV(i, GLint);
DEF_GLVERTEXV(s, GLshort);
#undef DEF_GLVERTEXV
/* glNormal */
void glNormal3f(GLfloat x, GLfloat y, GLfloat z)
{
  GLParam p[4];
  p[0].op = OP_Normal;
  p[1].f = x;
  p[2].f = y;
  p[3].f = z;
  gl_add_op(p);
}
void glNormal3fv(GLfloat* v)
{
  glNormal3f(v[0], v[1], v[2]);
}
/* glColor */
void glColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
  GLParam p[8];
  p[0].op = OP_Color;
  r = (GLfloat)BOUND(r, 0, 1);
  g = (GLfloat)BOUND(g, 0, 1);
  b = (GLfloat)BOUND(b, 0, 1);
  a = (GLfloat)BOUND(a, 0, 1);
  p[1].f = r;
  p[2].f = g;
  p[3].f = b;
  p[4].f = a;
  /* direct convertion to integer to go faster if no shading */
  p[5].ui = (unsigned int)(b * (ZB_POINT_RED_MAX - ZB_POINT_RED_MIN) + ZB_POINT_RED_MIN);
  p[6].ui = (unsigned int)(g * (ZB_POINT_GREEN_MAX - ZB_POINT_GREEN_MIN) + ZB_POINT_GREEN_MIN);
  p[7].ui = (unsigned int)(r * (ZB_POINT_BLUE_MAX - ZB_POINT_BLUE_MIN) + ZB_POINT_BLUE_MIN);
  gl_add_op(p);
}
void glColor4fv(GLfloat* v)
{
  glColor4f(v[0], v[1], v[2], v[3]);
}
void glColor3f(GLfloat x, GLfloat y, GLfloat z)
{
  glColor4f(x, y, z, 1);
}
void glColor3fv(const GLfloat* v)
{
  glColor4f(v[0], v[1], v[2], 1);
}
/* TexCoord */
void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
  GLParam p[5];
  p[0].op = OP_TexCoord;
  p[1].f = s;
  p[2].f = t;
  p[3].f = r;
  p[4].f = q;
  gl_add_op(p);
}
void glTexCoord2f(GLfloat s, GLfloat t)
{
  glTexCoord4f(s, t, 0, 1);
}
void glTexCoord2fv(GLfloat* v)
{
  glTexCoord4f(v[0], v[1], 0, 1);
}
void glEdgeFlag(int flag)
{
  GLParam p[2];
  p[0].op = OP_EdgeFlag;
  p[1].i = flag;
  gl_add_op(p);
}
/* misc */
void glShadeModel(int mode)
{
  GLParam p[2];
  assert(mode == GL_FLAT || mode == GL_SMOOTH);
  p[0].op = OP_ShadeModel;
  p[1].i = mode;
  gl_add_op(p);
}
void glCullFace(int mode)
{
  GLParam p[2];
  assert(mode == GL_BACK ||
      mode == GL_FRONT ||
      mode == GL_FRONT_AND_BACK);
  p[0].op = OP_CullFace;
  p[1].i = mode;
  gl_add_op(p);
}
void glFrontFace(int mode)
{
  GLParam p[2];
  assert(mode == GL_CCW || mode == GL_CW);
  mode = (mode != GL_CCW);
  p[0].op = OP_FrontFace;
  p[1].i = mode;
  gl_add_op(p);
}
void glPolygonMode(int face, int mode)
{
  // glopPolygonMode
  GLParam p[3];
  assert(face == GL_BACK ||
      face == GL_FRONT ||
      face == GL_FRONT_AND_BACK);
  assert(mode == GL_POINT || mode == GL_LINE || mode == GL_FILL);
  p[0].op = OP_PolygonMode;
  p[1].i = face;
  p[2].i = mode;
  gl_add_op(p);
}
/* glEnable / glDisable */
void glEnable(int cap)
{
  // glopEnableDisable
  GLParam p[3];
  p[0].op = OP_EnableDisable;
  p[1].i = cap;
  p[2].i = 1;
  gl_add_op(p);
}
void glDisable(int cap)
{
  GLParam p[3];
  p[0].op = OP_EnableDisable;
  p[1].i = cap;
  p[2].i = 0;
  gl_add_op(p);
}
/* glBegin / glEnd */
void glBegin(GLenum mode)
{
  GLParam p[2];
  p[0].op = OP_Begin;
  p[1].i = mode;
  gl_add_op(p);
}
void glEnd(void)
{
  GLParam p[1];
  p[0].op = OP_End;
  gl_add_op(p);
}
/* matrix */
void glMatrixMode(GLenum mode)
{
  // glopMatrixMode
  GLParam p[2];
  p[0].op = OP_MatrixMode;
  p[1].i = mode;
  gl_add_op(p);
}
void glLoadMatrixf(const GLfloat* m)
{
  int i;
  GLParam p[17];
  p[0].op = OP_LoadMatrix;
  for (i = 0; i < 16; i++) {
    p[i + 1].f = (GLfloat)m[i];
  }
  gl_add_op(p);
}
void glLoadMatrixd(const GLdouble* m)
{
  int i;
  GLParam p[17];
  p[0].op = OP_LoadMatrix;
  for (i = 0; i < 16; i++) {
    p[i + 1].f = (GLfloat)m[i];
  }
  gl_add_op(p);
}
void glLoadIdentity(void)
{
  GLParam p[1];
  p[0].op = OP_LoadIdentity;
  gl_add_op(p);
}
void glMultMatrixf(const GLfloat* m)
{
  GLParam p[17];
  int i;
  p[0].op = OP_MultMatrix;
  for (i = 0; i < 16; i++) {
    p[i + 1].f = m[i];
  }
  gl_add_op(p);
}
void glPushMatrix(void)
{
  GLParam p[1];
  p[0].op = OP_PushMatrix;
  gl_add_op(p);
}
void glPopMatrix(void)
{
  GLParam p[1];
  p[0].op = OP_PopMatrix;
  gl_add_op(p);
}
void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
  GLParam p[5];
  p[0].op = OP_Rotate;
  p[1].f = angle;
  p[2].f = x;
  p[3].f = y;
  p[4].f = z;
  gl_add_op(p);
}
void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
  // glopTranslate
  GLParam p[4];
  p[0].op = OP_Translate;
  p[1].f = x;
  p[2].f = y;
  p[3].f = z;
  gl_add_op(p);
}
void glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
  GLParam p[4];
  p[0].op = OP_Translate;
  p[1].f = (float)x;
  p[2].f = (float)y;
  p[3].f = (float)z;
  gl_add_op(p);
  return ;
}
void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
  GLParam p[4];
  p[0].op = OP_Scale;
  p[1].f = x;
  p[2].f = y;
  p[3].f = z;
  gl_add_op(p);
}
void glViewport(int x, int y, int width, int height)
{
  GLParam p[5];
  p[0].op = OP_Viewport;
  p[1].i = x;
  p[2].i = y;
  p[3].i = width;
  p[4].i = height;
  gl_add_op(p);
}
void glFrustum(double left, double right, double bottom, double top, double nearv, double farv)
{
  // glopFrustum
  GLParam p[7];
  p[0].op = OP_Frustum;
  p[1].f = (GLfloat)left;
  p[2].f = (GLfloat)right;
  p[3].f = (GLfloat)bottom;
  p[4].f = (GLfloat)top;
  p[5].f = (GLfloat)nearv;
  p[6].f = (GLfloat)farv;
  gl_add_op(p);
}
void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearv, GLdouble farv)
{
  // glopOrtho
  GLParam p[7];
  p[0].op = OP_Ortho;
  p[1].f = (GLfloat)left;
  p[2].f = (GLfloat)right;
  p[3].f = (GLfloat)bottom;
  p[4].f = (GLfloat)top;
  p[5].f = (GLfloat)nearv;
  p[6].f = (GLfloat)farv;
  gl_add_op(p);
  return ;
}
/* lightening */
void glMaterialfv(GLenum mode, GLenum type, const GLfloat* v)
{
  GLParam p[7];
  int i, n;
  assert(mode == GL_FRONT || mode == GL_BACK || mode == GL_FRONT_AND_BACK);
  p[0].op = OP_Material;
  p[1].i = mode;
  p[2].i = type;
  n = 4;
  if (type == GL_SHININESS) {
    n = 1;
  }
  for (i = 0; i < 4; i++) {
    p[3 + i].f = v[i];
  }
  for (i = n; i < 4; i++) {
    p[3 + i].f = 0;
  }
  gl_add_op(p);
}
void glMaterialf(int mode, int type, GLfloat v)
{
  GLParam p[7];
  int i;
  p[0].op = OP_Material;
  p[1].i = mode;
  p[2].i = type;
  p[3].f = v;
  for (i = 0; i < 3; i++) {
    p[4 + i].f = 0;
  }
  gl_add_op(p);
}
void glColorMaterial(int mode, int type)
{
  GLParam p[3];
  p[0].op = OP_ColorMaterial;
  p[1].i = mode;
  p[2].i = type;
  gl_add_op(p);
}
void glLightfv(int light, int type, const GLfloat* v)
{
  // glopLight
  GLParam p[7];
  int i;
  p[0].op = OP_Light;
  p[1].i = light;
  p[2].i = type;
  /* TODO: 3 composants ? */
  for (i = 0; i < 4; i++) {
    p[3 + i].f = v[i];
  }
  gl_add_op(p);
}
void glLightf(int light, int type, GLfloat v)
{
  GLParam p[7];
  int i;
  p[0].op = OP_Light;
  p[1].i = light;
  p[2].i = type;
  p[3].f = v;
  for (i = 0; i < 3; i++) {
    p[4 + i].f = 0;
  }
  gl_add_op(p);
}
void glLightModeli(int pname, int param)
{
  GLParam p[7];
  int i;
  p[0].op = OP_LightModel;
  p[1].i = pname;
  p[2].f = (GLfloat)param;
  for (i = 0; i < 4; i++) {
    p[3 + i].f = 0;
  }
  gl_add_op(p);
}
void glLightModelfv(int pname, GLfloat* param)
{
  GLParam p[6];
  int i;
  p[0].op = OP_LightModel;
  p[1].i = pname;
  for (i = 0; i < 4; i++) {
    p[2 + i].f = param[i];
  }
  gl_add_op(p);
}
/* clear */
void glClear(int mask)
{
  // glopClear
  GLParam p[2];
  p[0].op = OP_Clear;
  p[1].i = mask;
  gl_add_op(p);
}
void glClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
  GLParam p[5];
  p[0].op = OP_ClearColor;
  p[1].f = BOUND(r, 0, 1);
  p[2].f = BOUND(g, 0, 1);
  p[3].f = BOUND(b, 0, 1);
  p[4].f = a;
  gl_add_op(p);
}
void glClearDepth(double depth)
{
  GLParam p[2];
  p[0].op = OP_ClearDepth;
  p[1].f = (float)depth;
  gl_add_op(p);
}
/* textures */
void glTexImage2D(int target, int level, int components,
    int width, int height, int border,
    int format, int type, void* pixels)
{
  GLParam p[10];
  p[0].op = OP_TexImage2D;
  p[1].i = target;
  p[2].i = level;
  p[3].i = components;
  p[4].i = width;
  p[5].i = height;
  p[6].i = border;
  p[7].i = format;
  p[8].i = type;
  p[9].p = pixels;
  gl_add_op(p);
}
void glBindTexture(GLenum target, GLenum texture)
{
  GLParam p[3];
  p[0].op = OP_BindTexture;
  p[1].i = target;
  p[2].i = texture;
  gl_add_op(p);
}
void glTexEnvi(int target, int pname, int param)
{
  GLParam p[8];
  p[0].op = OP_TexEnv;
  p[1].i = target;
  p[2].i = pname;
  p[3].i = param;
  p[4].f = 0;
  p[5].f = 0;
  p[6].f = 0;
  p[7].f = 0;
  gl_add_op(p);
}
void glTexParameteri(int target, int pname, int param)
{
  GLParam p[8];
  p[0].op = OP_TexParameter;
  p[1].i = target;
  p[2].i = pname;
  p[3].i = param;
  p[4].f = 0;
  p[5].f = 0;
  p[6].f = 0;
  p[7].f = 0;
  gl_add_op(p);
}
void glPixelStorei(int pname, int param)
{
  GLParam p[3];
  p[0].op = OP_PixelStore;
  p[1].i = pname;
  p[2].i = param;
  gl_add_op(p);
}
/* selection */
void glInitNames(void)
{
  GLParam p[1];
  p[0].op = OP_InitNames;
  gl_add_op(p);
}
void glPushName(unsigned int name)
{
  GLParam p[2];
  p[0].op = OP_PushName;
  p[1].i = name;
  gl_add_op(p);
}
void glPopName(void)
{
  GLParam p[1];
  p[0].op = OP_PopName;
  gl_add_op(p);
}
void glLoadName(unsigned int name)
{
  GLParam p[2];
  p[0].op = OP_LoadName;
  p[1].i = name;
  gl_add_op(p);
}
void glPolygonOffset(GLfloat factor, GLfloat units)
{
  GLParam p[3];
  p[0].op = OP_PolygonOffset;
  p[1].f = factor;
  p[2].f = units;
}
/* Special Functions */
void glCallList(unsigned int list)
{
  GLParam p[2];
  p[0].op = OP_CallList;
  p[1].i = list;
  gl_add_op(p);
}
void glFlush(void)
{
  /* nothing to do */
}
void glHint(int target, int mode)
{
  GLParam p[3];
  p[0].op = OP_Hint;
  p[1].i = target;
  p[2].i = mode;
  gl_add_op(p);
}
/* Non standard functions */
void glDebug(int mode)
{
  GLContext* c = gl_get_context();
  c->print_flag = mode;
}
void glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
  glBegin(GL_POLYGON);
  glVertex2f(x1, y1);
  glVertex2f(x1, y2);
  glVertex2f(x2, y2);
  glVertex2f(x2, y1);
  glEnd();
  glFlush();
}
void glClose(void)
{
  GLContext* c = gl_get_context();
  endSharedState(c);
  if (c->zb->zbuf) {
    free(c->zb->zbuf);
  }
}
void glDeleteTextures(int n, const unsigned int* textures)
{
  GLContext* c = gl_get_context();
  int i;
  GLTexture* t;
  for (i = 0; i < n; i++) {
    t = find_texture(c, textures[i]);
    if (t != NULL && t != 0) {
      if (t == c->current_texture) {
        glBindTexture(GL_TEXTURE_2D, 0);
      }
      free_texture(c, textures[i]);
    }
  }
}
void glInit()
{
  GLContext* c = gl_get_context();
  GLViewport* v = &c->viewport;
  ZBuffer* zb = c->zb;
  int i;
  /* allocate GLVertex array */
  c->vertex_max = POLYGON_MAX_VERTEX;
  c->vertex = (GLVertex*)gl_malloc(POLYGON_MAX_VERTEX * sizeof(GLVertex));
  /* viewport */
  v->xmin = 0;
  v->ymin = 0;
  v->w = zb->w;
  v->h = zb->h;
  v->updated = 1;
  /* shared state */
  initSharedState(c);
  /* lists */
  c->exec_flag = 1;
  c->compile_flag = 0;
  c->print_flag = 0;
  c->in_begin = 0;
  /* lights */
  for (i = 0; i < MAX_LIGHTS; i++) {
    GLLight* l = &c->lights[i];
    l->ambient = gl_V4_New(0, 0, 0, 1);
    l->diffuse = gl_V4_New(1, 1, 1, 1);
    l->specular = gl_V4_New(1, 1, 1, 1);
    l->position = gl_V4_New(0, 0, 1, 0);
    l->norm_position = gl_V3_New(0, 0, 1);
    l->spot_direction = gl_V3_New(0, 0, -1);
    l->norm_spot_direction = gl_V3_New(0, 0, -1);
    l->spot_exponent = 0;
    l->spot_cutoff = 180;
    l->attenuation[0] = 1;
    l->attenuation[1] = 0;
    l->attenuation[2] = 0;
    l->enabled = 0;
  }
  c->first_light = NULL;
  c->ambient_light_model = gl_V4_New(0.2f, 0.2f, 0.2f, 1.f);
  c->local_light_model = 0;
  c->lighting_enabled = 0;
  c->light_model_two_side = 0;
  /* default materials */
  for (i = 0; i < 2; i++) {
    GLMaterial* m = &c->materials[i];
    m->emission = gl_V4_New(0, 0, 0, 1);
    m->ambient = gl_V4_New(0.2f, 0.2f, 0.2f, 1.f);
    m->diffuse = gl_V4_New(0.8f, 0.8f, 0.8f, 1.f);
    m->specular = gl_V4_New(0, 0, 0, 1);
    m->shininess = 0;
  }
  c->current_color_material_mode = GL_FRONT_AND_BACK;
  c->current_color_material_type = GL_AMBIENT_AND_DIFFUSE;
  c->color_material_enabled = 0;
  /* textures */
  glInitTextures(c);
  /* default state */
  c->current_color.X = 1.0;
  c->current_color.Y = 1.0;
  c->current_color.Z = 1.0;
  c->current_color.W = 1.0;
  c->longcurrent_color[0] = 65535;
  c->longcurrent_color[1] = 65535;
  c->longcurrent_color[2] = 65535;
  c->current_normal.X = 1.0;
  c->current_normal.Y = 0.0;
  c->current_normal.Z = 0.0;
  c->current_normal.W = 0.0;
  c->current_edge_flag = 1;
  c->current_tex_coord.X = 0;
  c->current_tex_coord.Y = 0;
  c->current_tex_coord.Z = 0;
  c->current_tex_coord.W = 1;
  c->polygon_mode_front = GL_FILL;
  c->polygon_mode_back = GL_FILL;
  c->current_front_face = 0; /* 0 = GL_CCW  1 = GL_CW */
  c->current_cull_face = GL_BACK;
  c->current_shade_model = GL_SMOOTH;
  c->cull_face_enabled = 0;
  /* clear */
  c->clear_color.v[0] = 0;
  c->clear_color.v[1] = 0;
  c->clear_color.v[2] = 0;
  c->clear_color.v[3] = 0;
  c->clear_depth = 0;
  /* selection */
  c->render_mode = GL_RENDER;
  c->select_buffer = NULL;
  c->name_stack_size = 0;
  /* matrix */
  c->matrix_mode = 0;
  c->matrix_stack_depth_max[0] = MAX_MODELVIEW_STACK_DEPTH;
  c->matrix_stack_depth_max[1] = MAX_PROJECTION_STACK_DEPTH;
  c->matrix_stack_depth_max[2] = MAX_TEXTURE_STACK_DEPTH;
  for (i = 0; i < 3; i++) {
    c->matrix_stack[i] = (M4*)gl_malloc(c->matrix_stack_depth_max[i] * sizeof(M4));
    c->matrix_stack_ptr[i] = c->matrix_stack[i];
  }
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  c->matrix_model_projection_updated = 1;
  /* opengl 1.1 arrays */
  c->client_states = 0;
  /* opengl 1.1 polygon offset */
  c->offset_states = 0;
  /* specular buffer */
  c->specbuf_first = NULL;
  c->specbuf_used_counter = 0;
  c->specbuf_num_buffers = 0;
  /* depth test */
  c->depth_test = 0;
}
void glPointSize(GLfloat size)
{
  // glopPointSize
  GLParam p[2];
  p[0].op = OP_PointSize;
  p[1].i = (int)size;
  gl_add_op(p);
}
void glPolygonStipple(const GLubyte* mask)
{
  ASSERT(0);
}
#undef X
#undef Y
#undef Z
#undef W
#endif // _GL_INL_

