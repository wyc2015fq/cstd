
#include "cstd.h"
#include "img/imgio.inl"
#include "ui/window.inl"

int DistSq(IPOINT pt) { return pt.x*pt.x + pt.y*pt.y; }

typedef struct {
  IPOINT* grid;
  int h, w;
} Grid;

void Grid_SetSize(Grid* g, int h, int w) {
  MYREALLOC(g->grid, h*w);
  g->h = h;
  g->w = w;
}

void Grid_Free(Grid* g) {
  FREE(g->grid);
  g->grid = NULL;
  g->h = g->w = 0;
}

IPOINT Get( Grid* g, int x, int y ) 
{ 
  static const IPOINT empty = { 9999, 9999 };
  // OPTIMIZATION: you can skip the edge check code if you make your grid 
  // have a 1-pixel gutter. 
  if ( x >= 0 && y >= 0 && x < g->w && y < g->h ) 
    return g->grid[y*g->w+x]; 
  else 
    return empty;
} 

void Put( Grid* g, int x, int y, IPOINT p )
{ 
  g->grid[y*g->w+x] = p; 
} 

void Compare( Grid* g, IPOINT &p, int x, int y, int offsetx, int offsety ) 
{ 
  IPOINT other = Get( g, x+offsetx, y+offsety ); 
  other.x += offsetx;
  other.y += offsety;
  
  if (DistSq(other) < DistSq(p)) {
    p = other; 
  }
} 

void GenerateSDF( Grid* g ) 
{ 
  int x, y;
  // Pass 0 
  for (y=0;y<g->h;y++) {
    for (x=0;x<g->w;x++) {
      IPOINT p = Get( g, x, y ); 
      Compare( g, p, x, y, -1, 0 );
      Compare( g, p, x, y, 0, -1 );
      Compare( g, p, x, y, -1, -1 );
      Compare( g, p, x, y, 1, -1 ); 
      Put( g, x, y, p ); 
    } 
    
    for (x=g->w-1;x>=0;x--) {
      IPOINT p = Get( g, x, y ); 
      Compare( g, p, x, y, 1, 0 ); 
      Put( g, x, y, p ); 
    } 
  } 
  
  // Pass 1
  for (y=g->h-1;y>=0;y--) {
    for (x=g->w-1;x>=0;x--) {
      IPOINT p = Get( g, x, y ); 
      Compare( g, p, x, y, 1, 0 ); 
      Compare( g, p, x, y, 0, 1 ); 
      Compare( g, p, x, y, -1, 1 ); 
      Compare( g, p, x, y, 1, 1 ); 
      Put( g, x, y, p ); 
    } 
    
    for (x=0;x<g->w;x++) { 
      IPOINT p = Get( g, x, y ); 
      Compare( g, p, x, y, -1, 0 ); 
      Put( g, x, y, p ); 
    } 
  } 
}

int sdf(int h, int w, const uchar* A, int al, double thd, uchar* B, int bl) {
  int x, y, t;
  IPOINT inside = { 0, 0 };
  IPOINT empty = { 9999, 9999 };
  Grid grid1[2] = {0};
  Grid* grid2 = grid1 + 1;

  Grid_SetSize(grid1, h, w);
  Grid_SetSize(grid2, h, w);
  // Initialize the grid from the BMP file.
  for( y=0;y<h;y++ ) {
    for ( x=0;x<w;x++ ) {
      int t1 = A[y * al + x];
      t = MAX(t, t1);
    }
  }
  t *= thd;
  for( y=0;y<h;y++ ) {
    for ( x=0;x<w;x++ ) {
      uchar g = A[y * al + x];
      // Points inside get marked with a dx/dy of zero. 
      // Points outside get marked with an infinitely large distance. 
      if ( g < t ) {
        Put( grid1, x, y, inside ); 
        Put( grid2, x, y, empty ); 
      } else {
        Put( grid2, x, y, inside );
        Put( grid1, x, y, empty ); 
      }
    }
  }
  
  // Generate the SDF.
  GenerateSDF( grid1 );
  GenerateSDF( grid2 );
  
  // Render out the results. 
  for( y=0; y<h; y++ ) {
    for ( x=0; x<w; x++ ) {
      // Calculate the actual distance from the dx/dy
      double dist1 = ( sqrt( DistSq(Get( grid1, x, y )) ) ); 
      double dist2 = ( sqrt( DistSq(Get( grid2, x, y )) ) ); 
      int c = (int)((dist1 - dist2)*10) + 128;
      
      // Clamp and scale it, just for display purposes.
      B[y * bl + x] = BOUND(c, 0, 255);
    }
  }
  Grid_Free(grid1);
  Grid_Free(grid2);
  return 0;
}

float fmix(float a, float b, float weight)
{
  return (((1.f) - weight) * a + weight * b);
}
static uchar sample(const img_t* bitmap, DPOINT pos)
{
  int w = bitmap->w, h = bitmap->h, pix2;
  uchar* pix = (uchar*)bitmap->tt.data;
  double x = pos.x * w - .5;
  double y = pos.y * h - .5;
  int l = (int) floor(x);
  int b = (int) floor(y);
  int r = l + 1;
  int t = b + 1;
  double lr = x - l;
  double bt = y - b;
  l = BOUND(l, 0, w - 1), r = BOUND(r, 0, w - 1);
  b = BOUND(b, 0, h - 1), t = BOUND(t, 0, h - 1);
  pix2 = (uchar)fmix(fmix(pix[l + b * w], pix[r + b * w], lr), fmix(pix[l + t * w], pix[r + t * w], lr), bt);
  return pix2;
}

int sdf_rander(const img_t* sdf, img_t* output, const COLOR* pal) {
  int x, y, w = output->w, h = output->h;
  //pxRange *= (double)(w + h) / (sdf->w + sdf->h);
  for (y = 0; y < h; ++y) {
    COLOR* output1 = (COLOR*)(output->tt.data + output->s * y);
    for (x = 0; x < w; ++x) {
      uchar s = sample(sdf, dPOINT((x + .5) / w, (y + .5) / h));
      output1[x] = pal[s];
    }
  }
  return 0;
}
int test_render_distance_field()
{
  img_t im[10] = {0};
  img_t* im1 = im+1;
  sys_chdir("E:/pub/bin/draw");
  imread("Font_R2_Dist.png", 1, 1, im);
  //imwrite("C://aaa.bmp", im);
  imshow2("im0", im);
  int t = 64;
  imresize(im, t, t, im);
  imsetsize(im1, 256, 256, sizeof(COLOR), 1);
  sdf(im->h, im->w, im->tt.data, im->s, 0.7, im->tt.data, im->s);
  COLOR clrs[256];
  COLOR clr[] = {0, 0, ColorRed, ColorRed, ColorBlue, ColorBlue};
  float pos[] = {0, 0.4, 0.5, 0.55, 0.6, 1};
  fill_color_array2(256, clrs, countof(pos), clr, pos);
  //fill_color_array(256, clrs, ColorBlack, ColorWhite);
  sdf_rander(im, im1, clrs);
  imshow2("im3", im);
  imshow2("im11", im1);
  cvWaitKey(-1);
  imfrees(im, countof(im));
  return 0;
}
