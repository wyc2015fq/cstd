
#ifndef __OPENCC_IMGPROC_FILTERENGINE_HPP__
#define __OPENCC_IMGPROC_FILTERENGINE_HPP__

//! type of the kernel
enum
{
    KERNEL_GENERAL      = 0, // the kernel is generic. No any type of symmetry or other properties.
    KERNEL_SYMMETRICAL  = 1, // kernel[i] == kernel[ksize-i-1] , and the anchor is at the center
    KERNEL_ASYMMETRICAL = 2, // kernel[i] == -kernel[ksize-i-1] , and the anchor is at the center
    KERNEL_SMOOTH       = 4, // all the kernel elements are non-negative and summed to 1
    KERNEL_INTEGER      = 8  // all the kernel coefficients are integer numbers
};

/*!
 The Base Class for 1D or Row-wise Filters

 This is the base class for linear or non-linear filters that process 1D data.
 In particular, such filters are used for the "horizontal" filtering parts in separable filters.

 Several functions in OpenCV return BaseRowFilter* for the specific types of filters,
 and those pointers can be used directly or within FilterEngine.
*/
class BaseRowFilter
{
public:
    //! the default constructor
    BaseRowFilter();
    //! the destructor
    virtual ~BaseRowFilter();
    //! the filtering operator. Must be overridden in the derived classes. The horizontal border interpolation is done outside of the class.
    virtual void operator()(const uchar* src, uchar* dst, int width, int cn) = 0;

    int ksize;
    int anchor;
};


/*!
 The Base Class for Column-wise Filters

 This is the base class for linear or non-linear filters that process columns of 2D arrays.
 Such filters are used for the "vertical" filtering parts in separable filters.

 Several functions in OpenCV return BaseColumnFilter* for the specific types of filters,
 and those pointers can be used directly or within FilterEngine.

 Unlike BaseRowFilter, BaseColumnFilter may have some context information,
 i.e. box filter keeps the sliding sum of elements. To reset the state BaseColumnFilter::reset()
 must be called (e.g. the method is called by FilterEngine)
 */
class BaseColumnFilter
{
public:
    //! the default constructor
    BaseColumnFilter();
    //! the destructor
    virtual ~BaseColumnFilter();
    //! the filtering operator. Must be overridden in the derived classes. The vertical border interpolation is done outside of the class.
    virtual void operator()(const uchar** src, uchar* dst, int dststep, int dstcount, int width) = 0;
    //! resets the internal buffers, if any
    virtual void reset();

    int ksize;
    int anchor;
};


/*!
 The Base Class for Non-Separable 2D Filters.

 This is the base class for linear or non-linear 2D filters.

 Several functions in OpenCV return BaseFilter* for the specific types of filters,
 and those pointers can be used directly or within FilterEngine.

 Similar to BaseColumnFilter, the class may have some context information,
 that should be reset using BaseFilter::reset() method before processing the new array.
*/
class BaseFilter
{
public:
    //! the default constructor
    BaseFilter();
    //! the destructor
    virtual ~BaseFilter();
    //! the filtering operator. The horizontal and the vertical border interpolation is done outside of the class.
    virtual void operator()(const uchar** src, uchar* dst, int dststep, int dstcount, int width, int cn) = 0;
    //! resets the internal buffers, if any
    virtual void reset();

    CSize ksize;
    CPoint anchor;
};


/*!
 The Main Class for Image Filtering.

 The class can be used to apply an arbitrary filtering operation to an image.
 It contains all the necessary intermediate buffers, it computes extrapolated values
 of the "virtual" pixels outside of the image etc.
 Pointers to the initialized FilterEngine instances
 are returned by various OpenCV functions, such as createSeparableLinearFilter(),
 createLinearFilter(), createGaussianFilter(), createDerivFilter(),
 createBoxFilter() and createMorphologyFilter().

 Using the class you can process large images by parts and build complex pipelines
 that include filtering as some of the stages. If all you need is to apply some pre-defined
 filtering operation, you may use filter2D(), erode(), dilate() etc.
 functions that create FilterEngine internally.

 Here is the example on how to use the class to implement Laplacian operator, which is the sum of
 second-order derivatives. More complex variant for different types is implemented in Laplacian().

 \code
 void laplace_f(const CvMat& src, CvMat& dst)
 {
     CC_Assert( src->tid == CC_32F );
     // make sure the destination array has the proper size and type
     dst.create(src.size(), src->tid);

     // get the derivative and smooth kernels for d2I/dx2.
     // for d2I/dy2 we could use the same kernels, just swapped
     CvMat kd, ks;
     getSobelKernels( kd, ks, 2, 0, ksize, false, ktype );

     // let's process 10 source rows at once
     int DELTA = MIN(10, src.rows);
     FilterEngine* Fxx = createSeparableLinearFilter(src->tid,
     dst->tid, kd, ks, CPoint(-1,-1), 0, borderType, borderType, Scalar() );
     FilterEngine* Fyy = createSeparableLinearFilter(src->tid,
     dst->tid, ks, kd, CPoint(-1,-1), 0, borderType, borderType, Scalar() );

     int y = Fxx->start(src), dsty = 0, dy = 0;
     Fyy->start(src);
     const uchar* sptr = src.data + y*src.step;

     // allocate the buffers for the spatial image derivatives;
     // the buffers need to have more than DELTA rows, because at the
     // last iteration the output may take max(kd.rows-1,ks.rows-1)
     // rows more than the input.
     CvMat Ixx( DELTA + kd.rows - 1, src.cols, dst->tid );
     CvMat Iyy( DELTA + kd.rows - 1, src.cols, dst->tid );

     // inside the loop we always pass DELTA rows to the filter
     // (note that the "proceed" method takes care of possibe overflow, since
     // it was given the actual image height in the "start" method)
     // on output we can get:
     //  * < DELTA rows (the initial buffer accumulation stage)
     //  * = DELTA rows (settled state in the middle)
     //  * > DELTA rows (then the input image is over, but we generate
     //                  "virtual" rows using the border mode and filter them)
     // this variable number of output rows is dy.
     // dsty is the current output row.
     // sptr is the pointer to the first input row in the portion to process
     for( ; dsty < dst.rows; sptr += DELTA*src.step, dsty += dy )
     {
         Fxx->proceed( sptr, (int)src.step, DELTA, Ixx.data, (int)Ixx.step );
         dy = Fyy->proceed( sptr, (int)src.step, DELTA, d2y.data, (int)Iyy.step );
         if( dy > 0 )
         {
             CvMat dstripe = dst.rowRange(dsty, dsty + dy);
             add(Ixx.rowRange(0, dy), Iyy.rowRange(0, dy), dstripe);
         }
     }
 }
 \endcode
*/
class FilterEngine
{
public:
    //! the default constructor
    FilterEngine();
    //! the full constructor. Either _filter2D or both _rowFilter and _columnFilter must be non-empty.
    FilterEngine(const BaseFilter* _filter2D,
                 const BaseRowFilter* _rowFilter,
                 const BaseColumnFilter* _columnFilter,
                 int srcType, int dstType, int bufType,
                 int _rowBorderType = BORDER_REPLICATE,
                 int _columnBorderType = -1,
                 const Scalar& _borderValue = Scalar());
    //! the destructor
    virtual ~FilterEngine();
    //! reinitializes the engine. The previously assigned filters are released.
    void init(const BaseFilter* _filter2D,
              const BaseRowFilter* _rowFilter,
              const BaseColumnFilter* _columnFilter,
              int srcType, int dstType, int bufType,
              int _rowBorderType = BORDER_REPLICATE,
              int _columnBorderType = -1,
              const Scalar& _borderValue = Scalar());

    //! starts filtering of the specified ROI of an image of size wholeSize.
    virtual int start(const CSize &wholeSize, const CSize &sz, const CPoint &ofs);
    //! starts filtering of the specified ROI of the specified image.
    virtual int start(const CvMat& src, const CSize &wsz, const CPoint &ofs);
    //! processes the next srcCount rows of the image.
    virtual int proceed(const uchar* src, int srcStep, int srcCount,
                        uchar* dst, int dstStep);
    //! applies filter to the specified ROI of the image. if srcRoi=(0,0,-1,-1), the whole image is filtered.
    virtual void apply(const CvMat& src, CvMat& dst, const CSize &wsz, const CPoint &ofs);

    //! returns true if the filter is separable
    bool isSeparable() const { return !filter2D; }
    //! returns the number
    int remainingInputRows() const;
    int remainingOutputRows() const;

    int srcType;
    int dstType;
    int bufType;
    CSize ksize;
    CPoint anchor;
    int maxWidth;
    CSize wholeSize;
    Rect roi;
    int dx1;
    int dx2;
    int rowBorderType;
    int columnBorderType;
    std::vector<int> borderTab;
    int borderElemSize;
    std::vector<uchar> ringBuf;
    std::vector<uchar> srcRow;
    std::vector<uchar> constBorderValue;
    std::vector<uchar> constBorderRow;
    int bufStep;
    int startY;
    int startY0;
    int endY;
    int rowCount;
    int dstY;
    std::vector<uchar*> rows;

    BaseFilter* filter2D;
    BaseRowFilter* rowFilter;
    BaseColumnFilter* columnFilter;
};


//! returns type (one of KERNEL_*) of 1D or 2D kernel specified by its coefficients.
int getKernelType(const CvArr* kernel, CPoint anchor);

//! returns the primitive row filter with the specified kernel
BaseRowFilter* getLinearRowFilter(int srcType, int bufType,
                                            const CvArr* kernel, int anchor,
                                            int symmetryType);

//! returns the primitive column filter with the specified kernel
BaseColumnFilter* getLinearColumnFilter(int bufType, int dstType,
                                            const CvArr* kernel, int anchor,
                                            int symmetryType, double delta = 0,
                                            int bits = 0);

//! returns 2D filter with the specified kernel
BaseFilter* getLinearFilter(int srcType, int dstType,
                                           const CvArr* kernel,
                                           CPoint anchor = CPoint(-1,-1),
                                           double delta = 0, int bits = 0);

//! returns the separable linear filter engine
FilterEngine* createSeparableLinearFilter(int srcType, int dstType,
                          const CvArr* rowKernel, const CvArr* columnKernel,
                          CPoint anchor = CPoint(-1,-1), double delta = 0,
                          int rowBorderType = BORDER_DEFAULT,
                          int columnBorderType = -1,
                          const Scalar& borderValue = Scalar());

//! returns the non-separable linear filter engine
FilterEngine* createLinearFilter(int srcType, int dstType,
                 const CvArr* kernel, CPoint _anchor = CPoint(-1,-1),
                 double delta = 0, int rowBorderType = BORDER_DEFAULT,
                 int columnBorderType = -1, const Scalar& borderValue = Scalar());

//! returns the Gaussian filter engine
FilterEngine* createGaussianFilter( int type, CSize ksize,
                                    double sigma1, double sigma2 = 0,
                                    int borderType = BORDER_DEFAULT);

//! returns filter engine for the generalized Sobel operator
FilterEngine* createDerivFilter( int srcType, int dstType,
                                        int dx, int dy, int ksize,
                                        int borderType = BORDER_DEFAULT );

//! returns horizontal 1D box filter
BaseRowFilter* getRowSumFilter(int srcType, int sumType,
                                              int ksize, int anchor = -1);

//! returns vertical 1D box filter
BaseColumnFilter* getColumnSumFilter( int sumType, int dstType,
                                                     int ksize, int anchor = -1,
                                                     double scale = 1);
//! returns box filter engine
FilterEngine* createBoxFilter( int srcType, int dstType, CSize ksize,
                                              CPoint anchor = CPoint(-1,-1),
                                              bool normalize = true,
                                              int borderType = BORDER_DEFAULT);


//! returns horizontal 1D morphological filter
BaseRowFilter* getMorphologyRowFilter(int op, int type, int ksize, int anchor = -1);

//! returns vertical 1D morphological filter
BaseColumnFilter* getMorphologyColumnFilter(int op, int type, int ksize, int anchor = -1);

//! returns 2D morphological filter
BaseFilter* getMorphologyFilter(int op, int type, const CvArr* kernel,
                                               CPoint anchor = CPoint(-1,-1));

//! returns morphological filter engine. Only MORPH_ERODE and MORPH_DILATE are supported.
CC_EXPORTS FilterEngine* createMorphologyFilter(int op, int type, const CvArr* kernel,
                                                    CPoint anchor = CPoint(-1,-1), int rowBorderType = BORDER_CONSTANT,
                                                    int columnBorderType = -1,
                                                    const Scalar& borderValue = morphologyDefaultBorderValue());

static inline CPoint normalizeAnchor( CPoint anchor, CSize ksize )
{
   if( anchor.x == -1 )
       anchor.x = ksize.width/2;
   if( anchor.y == -1 )
       anchor.y = ksize.height/2;
   CC_Assert( anchor.inside(Rect(0, 0, ksize.width, ksize.height)) );
   return anchor;
}

void preprocess2DKernel( const CvMat& kernel, std::vector<CPoint>& coords, std::vector<uchar>& coeffs );
void crossCorr( const CvMat& src, const CvMat& templ, CvMat& dst,
               CSize corrsize, int ctype,
               CPoint anchor=CPoint(0,0), double delta=0,
               int borderType=BORDER_REFLECT_101 );


#endif
