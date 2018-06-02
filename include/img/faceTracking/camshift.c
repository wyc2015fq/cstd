
#include "macro.h"
#include "xtypes.h"
#include "moments.h"
#include <assert.h>
#include <math.h>
#include <string.h>

int cvCamShift2( int height, int width, const unsigned char* img, int step, int cn, IRECT windowIn,
                 int max_iter, int eps, CvConnectedComp* _comp, CvBox2D* box );
                 
#define ABS(X)   ((X)>0. ? (X) : -(X))
                 
void adsf_hist( int height, int width, unsigned char* img, int step,
                unsigned char* mask, int maskStep, int a, int b, int* bins, int sz ) {
  int x;
  memset( bins, 0, sizeof( int ) * sz );
  for ( ; height--; img += step ) {
    unsigned char * ptr = img;
    if ( !mask ) {
      for ( x = 0; x <= width - 4; x += 4 ) {
        int v0 = ( ptr[ x ] * a + b );
        int v1 = ( ptr[ x + 1 ] * a + b );
        
        if ( ( unsigned ) v0 < ( unsigned ) sz )
          bins[ v0 ] ++;
        if ( ( unsigned ) v1 < ( unsigned ) sz )
          bins[ v1 ] ++;
          
        v0 = cvFloor( ptr[ x + 2 ] * a + b );
        v1 = cvFloor( ptr[ x + 3 ] * a + b );
        
        if ( ( unsigned ) v0 < ( unsigned ) sz )
          bins[ v0 ] ++;
        if ( ( unsigned ) v1 < ( unsigned ) sz )
          bins[ v1 ] ++;
      }
      
      for ( ; x < width; x++ ) {
        int v0 = ( ptr[ x ] * a + b );
        if ( ( unsigned ) v0 < ( unsigned ) sz )
          bins[ v0 ] ++;
      }
    } else {
      for ( x = 0; x < width; x++ )
        if ( mask[ x ] ) {
          int v0 = ( ptr[ x ] * a + b );
          if ( ( unsigned ) v0 < ( unsigned ) sz )
            bins[ v0 ] ++;
        }
      mask += maskStep;
    }
  }
}
/* The function calculates center of gravity and central second order moments */
void icvCompleteMomentState( CvMoments* moments ) {
  double cx = 0, cy = 0;
  double mu20, mu11, mu02;
  
  assert( moments != 0 );
  moments->inv_sqrt_m00 = 0;
  
  if ( fabs( moments->m00 ) > DBL_EPSILON ) {
    double inv_m00 = 1. / moments->m00;
    cx = moments->m10 * inv_m00;
    cy = moments->m01 * inv_m00;
    moments->inv_sqrt_m00 = sqrt( fabs( inv_m00 ) );
  }
  
  /* mu20 = m20 - m10*cx */
  mu20 = moments->m20 - moments->m10 * cx;
  /* mu11 = m11 - m10*cy */
  mu11 = moments->m11 - moments->m10 * cy;
  /* mu02 = m02 - m01*cy */
  mu02 = moments->m02 - moments->m01 * cy;
  
  moments->mu20 = mu20;
  moments->mu11 = mu11;
  moments->mu02 = mu02;
  
  /* mu30 = m30 - cx*(3*mu20 + cx*m10) */
  moments->mu30 = moments->m30 - cx * ( 3 * mu20 + cx * moments->m10 );
  mu11 += mu11;
  /* mu21 = m21 - cx*(2*mu11 + cx*m01) - cy*mu20 */
  moments->mu21 = moments->m21 - cx * ( mu11 + cx * moments->m01 ) - cy * mu20;
  /* mu12 = m12 - cy*(2*mu11 + cy*m10) - cx*mu02 */
  moments->mu12 = moments->m12 - cy * ( mu11 + cy * moments->m10 ) - cx * mu02;
  /* mu03 = m03 - cy*(3*mu02 + cy*m01) */
  moments->mu03 = moments->m03 - cy * ( 3 * mu02 + cy * moments->m01 );
}

typedef double momtype;
typedef double temptype;
#define __op__(x)  (x)
#define CC_OK 1

int icvmoment( int height, int width, const unsigned char* img, int step, int cn, double *moments ) {
  int x, y, sx_init = ( width & -4 ) * ( width & -4 ), sy = 0;
  momtype mom[ 10 ];
  
  if ( 0 == width || 0 == height ) {
    return 0;
  }
  assert( img && width && ( width | height ) >= 0 );
  memset( mom, 0, 10 * sizeof( mom[ 0 ] ) );
  
  
  for ( y = 0; y < height; sy += 2 * y + 1, y++, img += step ) {
    temptype x0 = 0;
    temptype x1 = 0;
    temptype x2 = 0;
    momtype x3 = 0;
    int sx = sx_init;
    const unsigned char* ptr = img;
    
    for ( x = 0; x < width - 3; x += 4, ptr += cn * 4 ) {
      temptype p0 = __op__( ptr[ 0 ] ), p1 = __op__( ptr[ cn ] ),
                                             p2 = __op__( ptr[ 2 * cn ] ), p3 = __op__( ptr[ 3 * cn ] );
      temptype t = p1;
      temptype a, b, c;
      
      p0 += p1 + p2 + p3; /* p0 + p1 + p2 + p3 */
      p1 += 2 * p2 + 3 * p3;      /* p1 + p2*2 + p3*3 */
      p2 = p1 + 2 * p2 + 6 * p3;  /* p1 + p2*4 + p3*9 */
      p3 = 2 * p2 - t + 9 * p3;   /* p1 + p2*8 + p3*27 */
      
      a = x * p0 + p1;    /* x*p0 + (x+1)*p1 + (x+2)*p2 + (x+3)*p3 */
      b = x * p1 + p2;    /* (x+1)*p1 + 2*(x+2)*p2 + 3*(x+3)*p3 */
      c = x * p2 + p3;    /* (x+1)*p1 + 4*(x+2)*p2 + 9*(x+3)*p3 */
      
      x0 += p0;
      x1 += a;
      a = a * x + b;      /*(x^2)*p0+((x+1)^2)*p1+((x+2)^2)*p2+((x+3)^2)*p3 */
      x2 += a;
      x3 += ( ( momtype ) ( a + b ) ) * x + c;  /*x3 += (x^3)*p0+((x+1)^3)*p1 +  */
      /*  ((x+2)^3)*p2+((x+3)^3)*p3   */
    }
    
    /* process the rest */
    for ( ; x < width; sx += 2 * x + 1, x++, ptr += cn ) {
      temptype p = __op__( ptr[ 0 ] );
      temptype xp = x * p;
      
      x0 += p;
      x1 += xp;
      x2 += sx * p;
      x3 += ( ( momtype ) sx ) * xp;
    }
    
    {
      temptype py = y * x0;
      
      mom[ 9 ] += ( ( momtype ) py ) * sy;  /* m03 */
      mom[ 8 ] += ( ( momtype ) x1 ) * sy;  /* m12 */
      mom[ 7 ] += ( ( momtype ) x2 ) * y;   /* m21 */
      mom[ 6 ] += x3;                  /* m30 */
      mom[ 5 ] += x0 * sy;             /* m02 */
      mom[ 4 ] += x1 * y;              /* m11 */
      mom[ 3 ] += x2;                  /* m20 */
      mom[ 2 ] += py;                  /* m01 */
      mom[ 1 ] += x1;                  /* m10 */
      mom[ 0 ] += x0;                  /* m00 */
    }
  }
  
  for ( x = 0; x < 10; x++ )
    moments[ x ] = ( double ) mom[ x ];
    
  return CC_OK;
}

/* summarizes moment values for all tiles */
void icvAccumulateMoments( double *tiles, CvMoments * moments ) {
  double dx = 0, dy = 0;
  double dxm = dx * tiles[ 0 ], dym = dy * tiles[ 0 ];
  
  /* + m00 ( = m00' ) */
  moments->m00 = tiles[ 0 ];
  
  /* + m10 ( = m10' + dx*m00' ) */
  moments->m10 = tiles[ 1 ] + dxm;
  
  /* + m01 ( = m01' + dy*m00' ) */
  moments->m01 = tiles[ 2 ] + dym;
  
  /* + m20 ( = m20' + 2*dx*m10' + dx*dx*m00' ) */
  moments->m20 = tiles[ 3 ] + dx * ( tiles[ 1 ] * 2 + dxm );
  
  /* + m11 ( = m11' + dx*m01' + dy*m10' + dx*dy*m00' ) */
  moments->m11 = tiles[ 4 ] + dx * ( tiles[ 2 ] + dym ) + dy * tiles[ 1 ];
  
  /* + m02 ( = m02' + 2*dy*m01' + dy*dy*m00' ) */
  moments->m02 = tiles[ 5 ] + dy * ( tiles[ 2 ] * 2 + dym );
  
  /* + m30 ( = m30' + 3*dx*m20' + 3*dx*dx*m10' + dx*dx*dx*m00' ) */
  moments->m30 = tiles[ 6 ] + dx * ( 3. * tiles[ 3 ] + dx * ( 3. * tiles[ 1 ] + dxm ) );
  
  /* + m21 (= m21' + dx*(2*m11' + 2*dy*m10' + dx*m01' + dx*dy*m00') + dy*m20') */
  moments->m21 = tiles[ 7 ] + dx * ( 2 * ( tiles[ 4 ] + dy * tiles[ 1 ] ) +
                                     dx * ( tiles[ 2 ] + dym ) ) + dy * tiles[ 3 ];
                                     
  /* + m12 (= m12' + dy*(2*m11' + 2*dx*m01' + dy*m10' + dx*dy*m00') + dx*m02') */
  moments->m12 = tiles[ 8 ] + dy * ( 2 * ( tiles[ 4 ] + dx * tiles[ 2 ] ) +
                                     dy * ( tiles[ 1 ] + dxm ) ) + dx * tiles[ 5 ];
                                     
  /* + m03 ( = m03' + 3*dy*m02' + 3*dy*dy*m01' + dy*dy*dy*m00' ) */
  moments->m03 = tiles[ 9 ] + dy * ( 3. * tiles[ 5 ] + dy * ( 3. * tiles[ 2 ] + dym ) );
  
  icvCompleteMomentState( moments );
}

int cvmoment( int height, int width, const unsigned char* img, int step, int cn, CvMoments *moments ) {
  int icvmoment( int height, int width, const unsigned char * img, int step, int cn, double * moment );
  void icvAccumulateMoments( double * tiles, CvMoments * moments );
  double mom[ 10 ] = {0};
  icvmoment( height, width, img, step, cn, mom );
  icvAccumulateMoments( mom, moments );
  return 0;
}

int cvMeanShift2( int height, int width, const unsigned char* img, int step, int cn, IRECT windowIn,
                  int max_iter, int eps, CvConnectedComp* comp ) {
  int i = 0;
  IRECT cur_rect = windowIn;
  CvMoments moments = {0};
  
  if ( comp )
    comp->rect = windowIn;
    
  moments.m00 = moments.m10 = moments.m01 = 0;
  
  for ( i = 0; i < max_iter; i++ ) {
    int dx, dy, nx, ny;
    double inv_m00;
    
#if 0
    
    CC_CALL( cvGetSubRect( mat, &cur_win, cur_rect ) );
    CC_CALL( cvMoments( &cur_win, &moments ) );
#else
    
    {
      int cvmoment( int height, int width, const unsigned char * img, int step, int cn, CvMoments * moments );
      const unsigned char* pro = img + cur_rect.y * step + cur_rect.x * cn;
      cvmoment( cur_rect.cy, cur_rect.cx, pro, step, cn, &moments );
    }
#endif
    
    /* Calculating center of mass */
    if ( fabs( moments.m00 ) < DBL_EPSILON )
      break;
      
    inv_m00 = moments.inv_sqrt_m00 * moments.inv_sqrt_m00;
    dx = cvRound( moments.m10 * inv_m00 - windowIn.cx * 0.5 );
    dy = cvRound( moments.m01 * inv_m00 - windowIn.cy * 0.5 );
    
    nx = cur_rect.x + dx;
    ny = cur_rect.y + dy;
    
    if ( nx < 0 )
      nx = 0;
    else if ( nx + cur_rect.cx > width )
      nx = width - cur_rect.cx;
      
    if ( ny < 0 )
      ny = 0;
    else if ( ny + cur_rect.cy > height )
      ny = height - cur_rect.cy;
      
    dx = nx - cur_rect.x;
    dy = ny - cur_rect.y;
    cur_rect.x = nx;
    cur_rect.y = ny;
    
    /* Check for coverage centers mass & window */
    if ( dx * dx + dy * dy < eps )
      break;
  }
  
  if ( comp ) {
    comp->rect = cur_rect;
    comp->area = ( float ) moments.m00;
  }
  
  return i;
}

int cvCamShift2( int height, int width, const unsigned char* img, int step, int cn, IRECT windowIn,
                 int max_iter, int eps, CvConnectedComp* _comp, CvBox2D* box ) {
  const int TOLERANCE = 10;
  CvMoments moments = {0};
  double m00 = 0, m10, m01, mu20, mu11, mu02, inv_m00;
  double a, b, c, xc, yc;
  double rotate_a, rotate_c;
  double theta = 0, square;
  double cs, sn;
  double length = 0, width1 = 0;
  int itersUsed = 0;
  CvConnectedComp comp;
  
  comp.rect = windowIn;
  
  //CC_CALL( itersUsed = cvMeanShift2( mat, windowIn, criteria, &comp ));
  {
    itersUsed = cvMeanShift2( height, width, img, step, cn, windowIn, 10, 1, &comp );
  }
  windowIn = comp.rect;
  
#if 0
  
  windowIn.x -= TOLERANCE;
  if ( windowIn.x < 0 )
    windowIn.x = 0;
    
  windowIn.y -= TOLERANCE;
  if ( windowIn.y < 0 )
    windowIn.y = 0;
    
  windowIn.cx += 2 * TOLERANCE;
  if ( windowIn.x + windowIn.cx > width )
    windowIn.cx = width - windowIn.x;
    
  windowIn.cy += 2 * TOLERANCE;
  if ( windowIn.y + windowIn.cy > height )
    windowIn.cy = height - windowIn.y;
    
    
  /* Calculating moments in new center mass */
  CC_CALL( cvGetSubRect( mat, &cur_win, windowIn ) );
  CC_CALL( cvMoments( &cur_win, &moments ) );
#else
  
  {
    const unsigned char* pro = img + windowIn.y * step + windowIn.x * cn;
    //memset(&moments, 0, sizeof(moments));
    cvmoment( windowIn.cy, windowIn.cx, pro, step, cn, &moments );
  }
#endif
  
  m00 = moments.m00;
  m10 = moments.m10;
  m01 = moments.m01;
  mu11 = moments.mu11;
  mu20 = moments.mu20;
  mu02 = moments.mu02;
  
  if ( fabs( m00 ) < DBL_EPSILON ) {
    return 0;
  }
  
  inv_m00 = 1. / m00;
  xc = cvRound( m10 * inv_m00 + windowIn.x );
  yc = cvRound( m01 * inv_m00 + windowIn.y );
  a = mu20 * inv_m00;
  b = mu11 * inv_m00;
  c = mu02 * inv_m00;
  
  /* Calculating width & height */
  square = sqrt( 4 * b * b + ( a - c ) * ( a - c ) );
  
  /* Calculating orientation */
  theta = atan2( 2 * b, a - c + square );
  
  /* Calculating width & length of figure */
  cs = cos( theta );
  sn = sin( theta );
  
  rotate_a = cs * cs * mu20 + 2 * cs * sn * mu11 + sn * sn * mu02;
  rotate_c = sn * sn * mu20 - 2 * cs * sn * mu11 + cs * cs * mu02;
  length = sqrt( rotate_a * inv_m00 ) * 4;
  width1 = sqrt( rotate_c * inv_m00 ) * 4;
  
  /* In case, when tetta is 0 or 1.57... the Length & Width may be exchanged */
  if ( length < width1 ) {
    double t;
    
    CC_SWAP( length, width1, t );
    CC_SWAP( cs, sn, t );
    theta = CC_PI * 0.5 - theta;
  }
  
  /* Saving results */
  if ( _comp || box ) {
    int t0, t1;
    int _xc = cvRound( xc );
    int _yc = cvRound( yc );
    
    t0 = cvRound( fabs( length * cs ) );
    t1 = cvRound( fabs( width1 * sn ) );
    
    t0 = MAX( t0, t1 ) + 2;
    comp.rect.cx = MIN( t0, ( width - _xc ) * 2 );
    
    t0 = cvRound( fabs( length * sn ) );
    t1 = cvRound( fabs( width1 * cs ) );
    
    t0 = MAX( t0, t1 ) + 2;
    comp.rect.cy = MIN( t0, ( height - _yc ) * 2 );
    
    comp.rect.x = MAX( 0, _xc - comp.rect.cx / 2 );
    comp.rect.y = MAX( 0, _yc - comp.rect.cy / 2 );
    
    comp.rect.cx = MIN( width - comp.rect.x, comp.rect.cx );
    comp.rect.cy = MIN( height - comp.rect.y, comp.rect.cy );
    comp.area = ( float ) m00;
  }
  
  if ( _comp )
    * _comp = comp;
    
  if ( box ) {
    box->size.cy = ( float ) length;
    box->size.cx = ( float ) width1;
    box->angle = ( float ) theta;
    box->center.x = comp.rect.x + comp.rect.cx * 0.5f;
    box->center.y = comp.rect.y + comp.rect.cy * 0.5f;
  }
  
  return itersUsed;
}
