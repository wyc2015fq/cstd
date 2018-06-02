
typedef struct dash_pen_t {
  int dashes[10];
  int num_dashes;
  COLOR color;
} dash_pen_t;

typedef struct gradient_brush_t {
  int x1, y1, x2, y2;
  COLOR clr1, clr2;
} gradient_brush_t;

typedef struct gouraud_brush_t {
  int x1, y1, x2, y2, x3, y3;
  COLOR clr1, clr2, clr3;
} gradient_brush_t;

enum {
  PEN_SOLID,
  PEN_DASH,
};

enum {
  BRUSH_SOLID,
  BRUSH_GRADIENT,
};

typedef struct DC {
  int h, w, s, c;
  uchar* data;
  int lineType;
  int fillType;
  IRECT clip;
  union {
    struct {
      COLOR color;
    } solid;
    dash_pen_t* dash;
  } pen;
  union {
    struct {
      COLOR color;
    } solid;
    gradient_brush_t* gradient;
  } brush;
} DC;

int dd_getimg(DC* dc, img_t* im)
{
  IRECT rc = iRectInter(dc->clip, iRECT(0, 0, w, h));
  IMINIT(im, RCH(&rc), RCW(&rc), dc->data + rc.t * dc->s + rc.l * dc->c, dc->s, dc->c, 1);
  return 0;
}

int dd_poly(DC* dc,)
{
  img_t im[1] = {0};
  dd_getimg(dc, im);
  return 0;
}