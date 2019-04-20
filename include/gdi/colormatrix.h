
#ifndef __GDI_COLOR_MATRIX_H__
#define __GDI_COLOR_MATRIX_H__

//----------------------------------------------------------------------------
// Color channel look up table (LUT)
//----------------------------------------------------------------------------

typedef BYTE ColorChannelLUT[256];

//----------------------------------------------------------------------------
// Per-channel Histogram for 8bpp images.
//----------------------------------------------------------------------------

enum HistogramFormat
{
    HistogramFormatARGB,
    HistogramFormatPARGB,
    HistogramFormatRGB,
    HistogramFormatGray,
    HistogramFormatB,
    HistogramFormatG,
    HistogramFormatR,
    HistogramFormatA    
};

//----------------------------------------------------------------------------
// Color matrix
//----------------------------------------------------------------------------

struct ColorMatrix
{
    REAL m[5][5];
};

//----------------------------------------------------------------------------
// Color Matrix flags
//----------------------------------------------------------------------------

enum ColorMatrixFlags
{
    ColorMatrixFlagsDefault   = 0,
    ColorMatrixFlagsSkipGrays = 1,
    ColorMatrixFlagsAltGray   = 2
};

//----------------------------------------------------------------------------
// Color Adjust Type
//----------------------------------------------------------------------------

enum ColorAdjustType
{
    ColorAdjustTypeDefault,
    ColorAdjustTypeBitmap,
    ColorAdjustTypeBrush,
    ColorAdjustTypePen,
    ColorAdjustTypeText,
    ColorAdjustTypeCount,
    ColorAdjustTypeAny      // Reserved
};

//----------------------------------------------------------------------------
// Color Map
//----------------------------------------------------------------------------

struct ColorMap
{
    Color oldColor;
    Color newColor;
};


#endif
