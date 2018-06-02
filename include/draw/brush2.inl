
#define ERR printf

struct Solid {
  ARGB color;
};

struct Hatch {
  uchar* hatch_data;
  int x, y;
  ARGB forecol;
  ARGB backcol;
};
struct LineGradient{
  FPOINT startpoint;
  FPOINT endpoint;
  ARGB startcolor;
  ARGB endcolor;
  FRECT rect;
  WrapMode wrap;
  BOOL gamma;
  float* blendfac;  /* blend factors */
  float* blendpos;  /* blend positions */
  INT blendcount;
  ARGB* pblendcolor; /* preset blend colors */
  float* pblendpos; /* preset blend positions */
  INT pblendcount;
};

typedef struct {
  BOOL enabled;
  ARGB low;
  ARGB high;
} color_key;

typedef struct {
  BOOL enabled;
  ColorMatrixFlags flags;
  ColorMatrix colormatrix;
  ColorMatrix graymatrix;
} color_matrix;

typedef struct {
  BOOL enabled;
  INT mapsize;
  ColorMap *colormap;
} color_remap_table;

typedef struct {
  ARGB outside_color;
  BOOL clamp;
  WrapMode wrap;
  color_key colorkeys[ColorAdjustTypeCount];
  color_matrix colormatrices[ColorAdjustTypeCount];
  color_remap_table colorremaptables[ColorAdjustTypeCount];
  BOOL gamma_enabled[ColorAdjustTypeCount];
  float gamma[ColorAdjustTypeCount];
} ImageAttributes;

struct Texture {
  WrapMode wrap;
  float transform[6];
  texture_t* image;
  FRECT rectf;
  ImageAttributes* imageattributes;
  InterpolationMode interpolation;
  PixelOffsetMode pixeloffset;
  BYTE *bitmap_bits; /* image bits converted to ARGB and run through imageattributes */
};

static Status BrushFillSolid(Solid* fill, DWORD *argb_pixels, UINT cdwStride, int Width, int Height) {
  int x, y;
  for (x=0; x<Width; x++) {
    for (y=0; y<Height; y++) {
      argb_pixels[x + y*cdwStride] = fill->color;
    }
  }
  return Ok;
}
static Status BrushFillHatch(Hatch* fill, DWORD *argb_pixels, UINT cdwStride, int Width, int Height) {
  int x, y;
  for (x=0; x<Width; x++) {
    for (y=0; y<Height; y++) {
      int hx, hy;
      
      /* FIXME: Account for the rendering origin */
      hx = (x + fill->x) % 8;
      hy = (y + fill->y) % 8;
      
      if ((fill->hatch_data[7-hy] & (0x80 >> hx)) != 0)
        argb_pixels[x + y*cdwStride] = fill->forecol;
      else
        argb_pixels[x + y*cdwStride] = fill->backcol;
    }
  }
  
  return Ok;
}

static ARGB blend_colors(ARGB start, ARGB end, float position)
{
  ARGB result=0;
  ARGB i;
  INT a1, a2, a3;
  
  a1 = (start >> 24) & 0xff;
  a2 = (end >> 24) & 0xff;
  
  a3 = (int)(a1*(1.0f - position)+a2*(position));
  
  result |= a3 << 24;
  
  for (i=0xff; i<=0xff0000; i = i << 8) {
    result |= (int)((start&i)*(1.0f - position)+(end&i)*(position))&i;
  }
  return result;
}

static ARGB blend_line_gradient(LineGradient* brush, float position)
{
  float blendfac;
  
  /* clamp to between 0.0 and 1.0, using the wrap mode */
  if (brush->wrap == WrapModeTile)
  {
    position = fmodf(position, 1.0f);
    if (position < 0.0f) position += 1.0f;
  }
  else /* WrapModeFlip* */
  {
    position = fmodf(position, 2.0f);
    if (position < 0.0f) position += 2.0f;
    if (position > 1.0f) position = 2.0f - position;
  }
  
  if (brush->blendcount == 1)
    blendfac = position;
  else
  {
    int i=1;
    float left_blendpos, left_blendfac, right_blendpos, right_blendfac;
    float range;
    
    /* locate the blend positions surrounding this position */
    while (position > brush->blendpos[i])
      i++;
    
    /* interpolate between the blend positions */
    left_blendpos = brush->blendpos[i-1];
    left_blendfac = brush->blendfac[i-1];
    right_blendpos = brush->blendpos[i];
    right_blendfac = brush->blendfac[i];
    range = right_blendpos - left_blendpos;
    blendfac = (left_blendfac * (right_blendpos - position) +
      right_blendfac * (position - left_blendpos)) / range;
  }
  
  if (brush->pblendcount == 0)
    return blend_colors(brush->startcolor, brush->endcolor, blendfac);
  else
  {
    int i=1;
    ARGB left_blendcolor, right_blendcolor;
    float left_blendpos, right_blendpos;
    
    /* locate the blend colors surrounding this position */
    while (blendfac > brush->pblendpos[i])
      i++;
    
    /* interpolate between the blend colors */
    left_blendpos = brush->pblendpos[i-1];
    left_blendcolor = brush->pblendcolor[i-1];
    right_blendpos = brush->pblendpos[i];
    right_blendcolor = brush->pblendcolor[i];
    blendfac = (blendfac - left_blendpos) / (right_blendpos - left_blendpos);
    return blend_colors(left_blendcolor, right_blendcolor, blendfac);
  }
}

static ARGB transform_color(ARGB color, const ColorMatrix *matrix)
{
  float val[5], res[4];
  int i, j;
  unsigned char a, r, g, b;
  
  val[0] = ((color >> 16) & 0xff) / 255.0; /* red */
  val[1] = ((color >> 8) & 0xff) / 255.0; /* green */
  val[2] = (color & 0xff) / 255.0; /* blue */
  val[3] = ((color >> 24) & 0xff) / 255.0; /* alpha */
  val[4] = 1.0; /* translation */
  
  for (i=0; i<4; i++)
  {
    res[i] = 0.0;
    
    for (j=0; j<5; j++)
      res[i] += matrix->m[j][i] * val[j];
  }
  
  a = MIN(MAX(floorf(res[3]*255.0), 0.0), 255.0);
  r = MIN(MAX(floorf(res[0]*255.0), 0.0), 255.0);
  g = MIN(MAX(floorf(res[1]*255.0), 0.0), 255.0);
  b = MIN(MAX(floorf(res[2]*255.0), 0.0), 255.0);
  
  return (a << 24) | (r << 16) | (g << 8) | b;
}

static int color_is_gray(ARGB color)
{
  unsigned char r, g, b;
  
  r = (color >> 16) & 0xff;
  g = (color >> 8) & 0xff;
  b = color & 0xff;
  
  return (r == g) && (g == b);
}

static void apply_image_attributes(const ImageAttributes *attributes, LPBYTE data,
                                   UINT width, UINT height, INT stride, ColorAdjustType type)
{
  UINT x, y, i;
  
  if (attributes->colorkeys[type].enabled ||
    attributes->colorkeys[ColorAdjustTypeDefault].enabled)
  {
    const color_key *key;
    BYTE min_blue, min_green, min_red;
    BYTE max_blue, max_green, max_red;
    
    if (attributes->colorkeys[type].enabled)
      key = &attributes->colorkeys[type];
    else
      key = &attributes->colorkeys[ColorAdjustTypeDefault];
    
    min_blue = key->low&0xff;
    min_green = (key->low>>8)&0xff;
    min_red = (key->low>>16)&0xff;
    
    max_blue = key->high&0xff;
    max_green = (key->high>>8)&0xff;
    max_red = (key->high>>16)&0xff;
    
    for (x=0; x<width; x++)
      for (y=0; y<height; y++)
      {
        ARGB *src_color;
        BYTE blue, green, red;
        src_color = (ARGB*)(data + stride * y + sizeof(ARGB) * x);
        blue = *src_color&0xff;
        green = (*src_color>>8)&0xff;
        red = (*src_color>>16)&0xff;
        if (blue >= min_blue && green >= min_green && red >= min_red &&
          blue <= max_blue && green <= max_green && red <= max_red)
          *src_color = 0x00000000;
      }
  }
  
  if (attributes->colorremaptables[type].enabled ||
    attributes->colorremaptables[ColorAdjustTypeDefault].enabled)
  {
    const color_remap_table *table;
    
    if (attributes->colorremaptables[type].enabled)
      table = &attributes->colorremaptables[type];
    else
      table = &attributes->colorremaptables[ColorAdjustTypeDefault];
    
    for (x=0; x<width; x++) {
      for (y=0; y<height; y++) {
        ARGB *src_color;
        src_color = (ARGB*)(data + stride * y + sizeof(ARGB) * x);
        for (i=0; i<table->mapsize; i++) {
          if (*src_color == table->colormap[i].oldColor) {
            *src_color = table->colormap[i].newColor;
            break;
          }
        }
      }
    }
  }
  
  if (attributes->colormatrices[type].enabled ||
    attributes->colormatrices[ColorAdjustTypeDefault].enabled)
  {
    const color_matrix *colormatrices;
    
    if (attributes->colormatrices[type].enabled)
      colormatrices = &attributes->colormatrices[type];
    else
      colormatrices = &attributes->colormatrices[ColorAdjustTypeDefault];
    
    for (x=0; x<width; x++)
      for (y=0; y<height; y++)
      {
        ARGB *src_color;
        src_color = (ARGB*)(data + stride * y + sizeof(ARGB) * x);
        
        if (colormatrices->flags == ColorMatrixFlagsDefault ||
          !color_is_gray(*src_color))
        {
          *src_color = transform_color(*src_color, &colormatrices->colormatrix);
        }
        else if (colormatrices->flags == ColorMatrixFlagsAltGray)
        {
          *src_color = transform_color(*src_color, &colormatrices->graymatrix);
        }
      }
  }
  
  if (attributes->gamma_enabled[type] ||
    attributes->gamma_enabled[ColorAdjustTypeDefault])
  {
    float gamma;
    
    if (attributes->gamma_enabled[type])
      gamma = attributes->gamma[type];
    else
      gamma = attributes->gamma[ColorAdjustTypeDefault];
    
    for (x=0; x<width; x++)
      for (y=0; y<height; y++)
      {
        ARGB *src_color;
        BYTE blue, green, red;
        src_color = (ARGB*)(data + stride * y + sizeof(ARGB) * x);
        
        blue = *src_color&0xff;
        green = (*src_color>>8)&0xff;
        red = (*src_color>>16)&0xff;
        
        /* FIXME: We should probably use a table for this. */
        blue = floorf(powf(blue / 255.0, gamma) * 255.0);
        green = floorf(powf(green / 255.0, gamma) * 255.0);
        red = floorf(powf(red / 255.0, gamma) * 255.0);
        
        *src_color = (*src_color & 0xff000000) | (red << 16) | (green << 8) | blue;
      }
  }
}

/* Given a bitmap and its source rectangle, find the smallest rectangle in the
* bitmap that contains all the pixels we may need to draw it. */
static void get_bitmap_sample_size(InterpolationMode interpolation, WrapMode wrap,
                                   bitmap_t* bitmap, float srcx, float srcy, float srcwidth, float srcheight, IRECT *rect)
{
  INT left, top, right, bottom;
  
  switch (interpolation)
  {
  case InterpolationModeHighQualityBilinear:
  case InterpolationModeHighQualityBicubic:
    /* FIXME: Include a greater range for the prefilter? */
  case InterpolationModeBicubic:
  case InterpolationModeBilinear:
    left = (INT)(floorf(srcx));
    top = (INT)(floorf(srcy));
    right = (INT)(ceilf(srcx+srcwidth));
    bottom = (INT)(ceilf(srcy+srcheight));
    break;
  case InterpolationModeNearestNeighbor:
  default:
    left = gdip_round(srcx);
    top = gdip_round(srcy);
    right = gdip_round(srcx+srcwidth);
    bottom = gdip_round(srcy+srcheight);
    break;
  }
  
  if (wrap == WrapModeClamp)
  {
    if (left < 0)
      left = 0;
    if (top < 0)
      top = 0;
    if (right >= bitmap->w)
      right = bitmap->w-1;
    if (bottom >= bitmap->h)
      bottom = bitmap->h-1;
  }
  else
  {
  /* In some cases we can make the rectangle smaller here, but the logic
  * is hard to get right, and tiling suggests we're likely to use the
    * entire source image. */
    if (left < 0 || right >= bitmap->w)
    {
      left = 0;
      right = bitmap->w-1;
    }
    
    if (top < 0 || bottom >= bitmap->h)
    {
      top = 0;
      bottom = bitmap->h-1;
    }
  }
  
  rect->l = left;
  rect->t = top;
  rect->r = right + 1;
  rect->b = bottom + 1;
}

static Status BrushFillLinearGradient(LineGradient* fill, DWORD *argb_pixels, UINT cdwStride, int Width, int Height, int X, int Y) {
  FPOINT draw_points[3], line_points[3];
  Status stat = Ok;
  static const FRECT box_1 = { 0.0, 0.0, 1.0, 1.0 };
  float world_to_gradient[6]; /* FIXME: Store this in the brush? */
  int x, y;
  
  draw_points[0].x = X;
  draw_points[0].y = Y;
  draw_points[1].x = X+1;
  draw_points[1].y = Y;
  draw_points[2].x = X;
  draw_points[2].y = Y+1;
  
  if (stat == Ok)
  {
    line_points[0] = fill->startpoint;
    line_points[1] = fill->endpoint;
    line_points[2].x = fill->startpoint.x + (fill->startpoint.y - fill->endpoint.y);
    line_points[2].y = fill->startpoint.y + (fill->endpoint.x - fill->startpoint.x);
    
    stat = M23Set3(&box_1, line_points, world_to_gradient);
  }
  
  if (stat == Ok)
  {
    stat = M23Invert(world_to_gradient);
    
    if (stat == Ok)
      stat = M23TransformPoints(world_to_gradient, draw_points, 3);
    
  }
  
  if (stat == Ok)
  {
    float x_delta = draw_points[1].x - draw_points[0].x;
    float y_delta = draw_points[2].x - draw_points[0].x;
    
    for (y=0; y<Height; y++)
    {
      for (x=0; x<Width; x++)
      {
        float pos = draw_points[0].x + x * x_delta + y * y_delta;
        argb_pixels[x + y*cdwStride] = blend_line_gradient(fill, pos);
      }
    }
  }
  
  return stat;
}

static ARGB sample_bitmap_pixel(const IRECT *src_rect, BYTE* bits, UINT width,
                                UINT height, INT x, INT y, const ImageAttributes *attributes)
{
  if (attributes->wrap == WrapModeClamp)
  {
    if (x < 0 || y < 0 || x >= width || y >= height)
      return attributes->outside_color;
  }
  else
  {
    /* Tiling. Make sure co-ordinates are positive as it simplifies the math. */
    if (x < 0)
      x = width*2 + x % (width * 2);
    if (y < 0)
      y = height*2 + y % (height * 2);
    
    if ((attributes->wrap & 1) == 1)
    {
      /* Flip X */
      if ((x / width) % 2 == 0)
        x = x % width;
      else
        x = width - 1 - x % width;
    }
    else
      x = x % width;
    
    if ((attributes->wrap & 2) == 2)
    {
      /* Flip Y */
      if ((y / height) % 2 == 0)
        y = y % height;
      else
        y = height - 1 - y % height;
    }
    else
      y = y % height;
  }
  if (x < src_rect->l || y < src_rect->t || x >= src_rect->r || y >= src_rect->b)
  {
    ERR("out of range pixel requested\n");
    return 0xffcd0084;
  }
  
  return ((DWORD*)(bits))[(x - src_rect->l) + (y - src_rect->t) * RCW(src_rect)];
}

static ARGB resample_bitmap_pixel(const IRECT *src_rect, BYTE* bits, UINT width,
                                  UINT height, FPOINT *point, const ImageAttributes *attributes,
                                  InterpolationMode interpolation, PixelOffsetMode offset_mode)
{
  static int fixme;
  
  switch (interpolation)
  {
  default:
    if (!fixme++)
      FIXME("Unimplemented interpolation %i\n", interpolation);
    /* fall-through */
  case InterpolationModeBilinear:
    {
      float leftxf, topyf;
      INT leftx, rightx, topy, bottomy;
      ARGB topleft, topright, bottomleft, bottomright;
      ARGB top, bottom;
      float x_offset;
      
      leftxf = floorf(point->x);
      leftx = (INT)leftxf;
      rightx = (INT)ceilf(point->x);
      topyf = floorf(point->y);
      topy = (INT)topyf;
      bottomy = (INT)ceilf(point->y);
      
      if (leftx == rightx && topy == bottomy)
        return sample_bitmap_pixel(src_rect, bits, width, height,
        leftx, topy, attributes);
      
      topleft = sample_bitmap_pixel(src_rect, bits, width, height,
        leftx, topy, attributes);
      topright = sample_bitmap_pixel(src_rect, bits, width, height,
        rightx, topy, attributes);
      bottomleft = sample_bitmap_pixel(src_rect, bits, width, height,
        leftx, bottomy, attributes);
      bottomright = sample_bitmap_pixel(src_rect, bits, width, height,
        rightx, bottomy, attributes);
      
      x_offset = point->x - leftxf;
      top = blend_colors(topleft, topright, x_offset);
      bottom = blend_colors(bottomleft, bottomright, x_offset);
      
      return blend_colors(top, bottom, point->y - topyf);
    }
  case InterpolationModeNearestNeighbor:
    {
      FLOAT pixel_offset;
      switch (offset_mode)
      {
      default:
      case PixelOffsetModeNone:
      case PixelOffsetModeHighSpeed:
        pixel_offset = 0.5;
        break;
        
      case PixelOffsetModeHalf:
      case PixelOffsetModeHighQuality:
        pixel_offset = 0.0;
        break;
      }
      return sample_bitmap_pixel(src_rect, bits, width, height,
        floorf(point->x + pixel_offset), floorf(point->y + pixel_offset), attributes);
    }
    
  }
}

static float intersect_line_scanline(const FPOINT *p1, const FPOINT *p2, float y)
{
  return (p1->x - p2->x) * (p2->y - y) / (p2->y - p1->y) + p2->x;
}

static Status BrushFillLinearGradient(Texture* fill, DWORD *argb_pixels, UINT cdwStride, int Width, int Height, int X, int Y) {
  FPOINT draw_points[3];
  Status stat;
  int x, y;
  texture_t *bitmap;
  int src_stride;
  IRECT src_area = {0};
  uchar* bitmap_bits = NULL;
  
  bitmap = fill->image;
  src_stride = bitmap->s;
  
  src_area = iRECT(0, 0, bitmap->w, bitmap->h);
  
  draw_points[0].x = X;
  draw_points[0].y = Y;
  draw_points[1].x = X+1;
  draw_points[1].y = Y;
  draw_points[2].x = X;
  draw_points[2].y = Y+1;
  
  if (stat == Ok)
  {
    float world_to_texture[6];
    stat = M23InvertCopy(fill->transform, world_to_texture);
    if (stat == Ok)
      stat = M23TransformPoints(world_to_texture, draw_points, 3);
  }
  
  if (stat == Ok && !bitmap_bits)
  {
    if (stat == Ok)
      apply_image_attributes(fill->imageattributes, bitmap_bits, bitmap->w, bitmap->h, src_stride, ColorAdjustTypeBitmap);
  }
  
  if (stat == Ok)
  {
    float x_dx = draw_points[1].x - draw_points[0].x;
    float x_dy = draw_points[1].y - draw_points[0].y;
    float y_dx = draw_points[2].x - draw_points[0].x;
    float y_dy = draw_points[2].y - draw_points[0].y;
    
    for (y=0; y<Height; y++)
    {
      for (x=0; x<Width; x++)
      {
        FPOINT point;
        point.x = draw_points[0].x + x * x_dx + y * y_dx;
        point.y = draw_points[0].y + y * x_dy + y * y_dy;
        
        argb_pixels[x + y*cdwStride] = resample_bitmap_pixel(
          &src_area, bitmap_bits, bitmap->w, bitmap->h,
          &point, fill->imageattributes, fill->interpolation,
          fill->pixeloffset);
      }
    }
  }
  
  return stat;
}

struct PathGradient {
  Path* path;
  ARGB centercolor;
  WrapMode wrap;
  BOOL gamma;
  FPOINT center;
  FPOINT focus;
  float* blendfac;  /* blend factors */
  float* blendpos;  /* blend positions */
  INT blendcount;
  ARGB *surroundcolors;
  INT surroundcolorcount;
  ARGB* pblendcolor; /* preset blend colors */
  float* pblendpos; /* preset blend positions */
  INT pblendcount;
  float transform[6];
};

static Status BrushFillPathGradient(PathGradient* fill, DWORD *argb_pixels, UINT cdwStride, int Width, int Height, int X, int Y) {
  Path flat_path[1] = {0};
  Status stat;
  int i, figure_start=0;
  FPOINT start_point, end_point, center_point;
  BYTE type;
  float min_yf, max_yf, line1_xf, line2_xf;
  INT min_y, max_y, min_x, max_x;
  INT x, y;
  ARGB outer_color;
  static int transform_fixme_once;
  
  if (fill->focus.x != 0.0 || fill->focus.y != 0.0)
  {
    static int once;
    if (!once++)
      FIXME("path gradient focus not implemented\n");
  }
  
  if (fill->gamma)
  {
    static int once;
    if (!once++)
      FIXME("path gradient gamma correction not implemented\n");
  }
  
  if (fill->blendcount)
  {
    static int once;
    if (!once++)
      FIXME("path gradient blend not implemented\n");
  }
  
  if (fill->pblendcount)
  {
    static int once;
    if (!once++)
      FIXME("path gradient preset blend not implemented\n");
  }
  
  if (!transform_fixme_once)
  {
    BOOL is_identity=TRUE;
    M23IsIdentity(fill->transform, &is_identity);
    if (!is_identity)
    {
      FIXME("path gradient transform not implemented\n");
      transform_fixme_once = 1;
    }
  }
  
  stat = PathFlatten(fill->path, 0.5, flat_path);
  
  if (stat != Ok)
    return stat;
  
  if (stat != Ok)
  {
    PathFree(flat_path);
    return stat;
  }
  
  for (i=0; i<flat_path->Count; i++)
  {
    int start_center_line=0, end_center_line=0;
    int seen_start=0, seen_end=0, seen_center=0;
    float center_distance;
    ARGB start_color, end_color;
    float dy, dx;
    
    type = flat_path->Types[i];
    
    if ((type&PathPointTypePathTypeMask) == PathPointTypeStart)
      figure_start = i;
    
    start_point = flat_path->Points[i];
    
    start_color = fill->surroundcolors[MIN(i, fill->surroundcolorcount-1)];
    
    if ((type&PathPointTypeCloseSubpath) == PathPointTypeCloseSubpath || i+1 >= flat_path->Count)
    {
      end_point = flat_path->Points[figure_start];
      end_color = fill->surroundcolors[MIN(figure_start, fill->surroundcolorcount-1)];
    }
    else if ((flat_path->Types[i+1] & PathPointTypePathTypeMask) == PathPointTypeLine)
    {
      end_point = flat_path->Points[i+1];
      end_color = fill->surroundcolors[MIN(i+1, fill->surroundcolorcount-1)];
    }
    else {
      continue;
    }
    
    outer_color = start_color;
    
    min_yf = center_point.y;
    if (min_yf > start_point.y) min_yf = start_point.y;
    if (min_yf > end_point.y) min_yf = end_point.y;
    
    if (min_yf < Y)
      min_y = Y;
    else
      min_y = (INT)ceil(min_yf);
    
    max_yf = center_point.y;
    if (max_yf < start_point.y) max_yf = start_point.y;
    if (max_yf < end_point.y) max_yf = end_point.y;
    
    if (max_yf > Y + Height)
      max_y = Y + Height;
    else
      max_y = (INT)ceil(max_yf);
    
    dy = end_point.y - start_point.y;
    dx = end_point.x - start_point.x;
    
    /* This is proportional to the distance from start-end line to center point. */
    center_distance = dy * (start_point.x - center_point.x) +
      dx * (center_point.y - start_point.y);
    
    for (y=min_y; y<max_y; y++)
    {
      float yf = (float)y;
      
      if (!seen_start && yf >= start_point.y)
      {
        seen_start = 1;
        start_center_line ^= 1;
      }
      if (!seen_end && yf >= end_point.y)
      {
        seen_end = 1;
        end_center_line ^= 1;
      }
      if (!seen_center && yf >= center_point.y)
      {
        seen_center = 1;
        start_center_line ^= 1;
        end_center_line ^= 1;
      }
      
      if (start_center_line)
        line1_xf = intersect_line_scanline(&start_point, &center_point, yf);
      else
        line1_xf = intersect_line_scanline(&start_point, &end_point, yf);
      
      if (end_center_line)
        line2_xf = intersect_line_scanline(&end_point, &center_point, yf);
      else
        line2_xf = intersect_line_scanline(&start_point, &end_point, yf);
      
      if (line1_xf < line2_xf)
      {
        min_x = (INT)ceil(line1_xf);
        max_x = (INT)ceil(line2_xf);
      }
      else
      {
        min_x = (INT)ceil(line2_xf);
        max_x = (INT)ceil(line1_xf);
      }
      
      if (min_x < X)
        min_x = X;
      if (max_x > X + Width)
        max_x = X + Width;
      
      for (x=min_x; x<max_x; x++)
      {
        float xf = (float)x;
        float distance;
        
        if (start_color != end_color)
        {
          float blend_amount, pdy, pdx;
          pdy = yf - center_point.y;
          pdx = xf - center_point.x;
          blend_amount = ( (center_point.y - start_point.y) * pdx + (start_point.x - center_point.x) * pdy ) / ( dy * pdx - dx * pdy );
          outer_color = blend_colors(start_color, end_color, blend_amount);
        }
        
        distance = (end_point.y - start_point.y) * (start_point.x - xf) +
          (end_point.x - start_point.x) * (yf - start_point.y);
        
        distance = distance / center_distance;
        
        argb_pixels[(x-X) + (y-Y)*cdwStride] =
          blend_colors(outer_color, fill->centercolor, distance);
      }
    }
  }
  
  PathFree(flat_path);
  return stat;
}
