
#include "draw/imdraw.inl"

/// <summary>
/// Chart series type enumeration
/// </summary>
enum SeriesType
{
  /// <summary>
  /// Line style
  /// </summary>
  SeriesType_Line,
    /// <summary>
    /// Dots style
    /// </summary>
    SeriesType_Dots,
    /// <summary>
    /// Connected dots style
    /// </summary>
    SeriesType_ConnectedDots
};

/// <summary>
/// Paint the control with all available data series
/// </summary>
/// 
/// <param name="pe">Data for Paint event</param>
/// 
int imdraw_Chart(img_t* im, const IRECT* pclip, IRECT rc, int type, int rows, int cols, const double* data, int step)
{
  int			clientWidth = RCW(&rc);
  int			clientHeight = RCH(&rc);
  double	minY = DBL_MAX;
  double	maxY = DBL_MIN;
  double	minX = DBL_MAX;
  double	maxX = DBL_MIN;
  double xFactor, yFactor;
  int i, n;
  int		width = 4;
  int		r = width >> 1;
  COLOR	clr = Color_Blue;
  
  // fill with white background
  // draw a black rectangle
  imdraw_rect( im, pclip, rc, Color_White, Color_Black, 1, 0 );
  
  // get data of the series
  for ( i = 0; i < rows; ++i ) {
    double v = data[i*step+1];
    // check for max
    maxX = MAX(maxX, v);
    maxY = MAX(maxY, v);
    // check for min
    minX = MIN(minX, v);
    minY = MIN(minY, v);
  }
  // check if there are any series
  xFactor = (double)( clientWidth - 10 ) / ( maxX - minX );
  yFactor = (double)( clientHeight - 10 ) / ( maxY - minY );
  
  // walk through all data series
    // get data of the series
    
  switch(type) {
    // check series type
  case SeriesType_Dots:
    {
      // draw dots
      
      // draw all points
      for ( i = 0, n = rows; i < n; i++ )
      {
        int x = (int) ( ( data[i*step+0] - minX ) * xFactor );
        int y = (int) ( ( data[i*step+1] - minY ) * yFactor );
        
        x += 5;
        y = clientHeight - 6 - y;
        
        imdraw_rect( im, pclip, iRECT2(x - r, y - r, width, width), clr, 0, 0, 0 );
      }
    }
    break;
  case SeriesType_ConnectedDots:
    {
      // draw dots connected with 1-pixel width line
      int x1 = (int) ( ( data[0] - minX ) * xFactor );
      int y1 = (int) ( ( data[1] - minY ) * yFactor );
      
      x1 += 5;
      y1 = clientHeight - 6 - y1;
      imdraw_rect( im, pclip, iRECT2(x1 - r, y1 - r, width, width), clr, 0, 0, 0 );
      
      // draw all lines
      for ( i = 1, n = rows; i < n; i++ )
      {
        int x2 = (int) ( ( data[i*step+0] - minX ) * xFactor );
        int y2 = (int) ( ( data[i*step+1] - minY ) * yFactor );
        
        x2 += 5;
        y2 = clientHeight - 6 - y2;
        imdraw_rect( im, pclip, iRECT2(x2 - r, y2 - r, width, width), rgb(1,0,0), 0, 0, 0 );
        imdrawaa_line( im, pclip, x1, y1, x2, y2, clr, 1 );
        
        x1 = x2;
        y1 = y2;
      }
    }
    break;
  case SeriesType_Line:
    {
      // draw line
      int x1 = (int) ( ( data[0] - minX ) * xFactor );
      int y1 = (int) ( ( data[1] - minY ) * yFactor );
      
      x1 += 5;
      y1 = clientHeight - 6 - y1;
      
      // draw all lines
      for ( i = 1, n = rows; i < n; i++ )
      {
        int x2 = (int) ( ( data[i*step+ 0] - minX ) * xFactor );
        int y2 = (int) ( ( data[i*step+ 1] - minY ) * yFactor );
        
        x2 += 5;
        y2 = clientHeight - 6 - y2;
        
        imdrawaa_line( im, pclip, x1, y1, x2, y2, clr, 1 );
        
        x1 = x2;
        y1 = y2;
      }
    }
    break;
  }
  
  return 0;
}
