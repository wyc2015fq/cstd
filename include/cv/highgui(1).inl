
#ifndef _HIGH_GUI_
#define _HIGH_GUI_

#ifndef SKIP_INCLUDES

  #include "cxcore.h"
  #if defined WIN32 || defined WIN64
    #include <windows.h>
  #endif

#else // SKIP_INCLUDES

  #if defined WIN32 || defined WIN64
    #define C_CDECL __cdecl
    #define C_STDCALL __stdcall
  #else
    #define C_CDECL
    #define C_STDCALL
  #endif

  #ifndef CC_EXTERN_C
    #ifdef __cplusplus
      #define CC_EXTERN_C extern "C"
      #define CC_DEFAULT(val) = val
    #else
      #define CC_EXTERN_C
      #define CC_DEFAULT(val)
    #endif
  #endif

  #ifndef CC_EXTERN_C_FUNCPTR
    #ifdef __cplusplus
      #define CC_EXTERN_C_FUNCPTR(x) extern "C" { typedef x; }
    #else
      #define CC_EXTERN_C_FUNCPTR(x) typedef x
    #endif
  #endif

  #ifndef CC_INLINE
    #if defined __cplusplus
      #define CC_INLINE inline
    #elif (defined WIN32 || defined WIN64) && !defined __GNUC__
      #define CC_INLINE __inline
    #else
      #define CC_INLINE static
    #endif
  #endif /* CC_INLINE */

  #if (defined WIN32 || defined WIN64) && defined CVAPI_EXPORTS
    #define CC_EXPORTS __declspec(dllexport)
  #else
    #define CC_EXPORTS
  #endif

  #ifndef CVAPI
    #define CVAPI(rettype) CC_EXTERN_C CC_EXPORTS rettype C_CDECL
  #endif

#endif // SKIP_INCLUDES

#if defined(_CH_)
  #pragma package <chopencv>
  #include <chdl.h>
  LOAD_CHDL(highgui)
#endif

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

/****************************************************************************************\
*                                  Basic GUI functions                                   *
\****************************************************************************************/

/* this function is used to set some external parameters in case of X Window */
CVAPI(int) cvInitSystem(int argc, char** argv);

CVAPI(int) cvStartWindowThread();

#define CC_WINDOW_AUTOSIZE  1
/* create window */
CVAPI(int) cvNamedWindow(const char* name, int flags CC_DEFAULT(CC_WINDOW_AUTOSIZE));

/* display image within window (highgui windows remember their content) */
CVAPI(void) cvShowImage(const char* name, const img_t* image);

/* resize/move window */
CVAPI(void) cvResizeWindow(const char* name, int width, int height);
CVAPI(void) cvMoveWindow(const char* name, int x, int y);


/* destroy window and all the trackers associated with it */
CVAPI(void) cvDestroyWindow(const char* name);

CVAPI(void) cvDestroyAllWindows(void);

/* get native window handle (HWND in case of Win32 and Widget in case of X Window) */
CVAPI(void*) cvGetWindowHandle(const char* name);

/* get name of highgui window given its native handle */
CVAPI(const char*) cvGetWindowName(void* window_handle);


typedef void (C_CDECL *CvTrackbarCallback)(int pos);

/* create trackbar and display it on top of given window, set callback */
CVAPI(int) cvCreateTrackbar(const char* trackbar_name, const char* window_name,
                             int* value, int count, CvTrackbarCallback on_change);

/* retrieve or set trackbar position */
CVAPI(int) cvGetTrackbarPos(const char* trackbar_name, const char* window_name);
CVAPI(void) cvSetTrackbarPos(const char* trackbar_name, const char* window_name, int pos);

#define CC_EVENT_MOUSEMOVE      0
#define CC_EVENT_LBUTTONDOWN    1
#define CC_EVENT_RBUTTONDOWN    2
#define CC_EVENT_MBUTTONDOWN    3
#define CC_EVENT_LBUTTONUP      4
#define CC_EVENT_RBUTTONUP      5
#define CC_EVENT_MBUTTONUP      6
#define CC_EVENT_LBUTTONDBLCLK  7
#define CC_EVENT_RBUTTONDBLCLK  8
#define CC_EVENT_MBUTTONDBLCLK  9

#define CC_EVENT_FLAG_LBUTTON   1
#define CC_EVENT_FLAG_RBUTTON   2
#define CC_EVENT_FLAG_MBUTTON   4
#define CC_EVENT_FLAG_CTRLKEY   8
#define CC_EVENT_FLAG_SHIFTKEY  16
#define CC_EVENT_FLAG_ALTKEY    32

typedef void (C_CDECL *CvMouseCallback)(int event, int x, int y, int flags, void* param);

/* assign callback for mouse events */
CVAPI(void) cvSetMouseCallback(const char* window_name, CvMouseCallback on_mouse,
                                void* param CC_DEFAULT(NULL));

/* 8bit, color or not */
#define CC_LOAD_IMAGE_UNCHANGED  -1
/* 8bit, gray */
#define CC_LOAD_IMAGE_GRAYSCALE   0
/* ?, color */
#define CC_LOAD_IMAGE_COLOR       1
/* any depth, ? */ 
#define CC_LOAD_IMAGE_ANYDEPTH    2
/* ?, any color */
#define CC_LOAD_IMAGE_ANYCOLOR    4

/* load image from file 
  iscolor can be a combination of above flags where CC_LOAD_IMAGE_UNCHANGED
  overrides the other flags
  using CC_LOAD_IMAGE_ANYCOLOR alone is equivalent to CC_LOAD_IMAGE_UNCHANGED
  unless CC_LOAD_IMAGE_ANYDEPTH is specified images are converted to 8bit
*/
CVAPI(img_t*) cvLoadImage(const char* filename, int iscolor CC_DEFAULT(CC_LOAD_IMAGE_COLOR));
CVAPI(img_t*) cvLoadImageM(const char* filename, int iscolor CC_DEFAULT(CC_LOAD_IMAGE_COLOR));

/* save image to file */
CVAPI(int) cvSaveImage(const char* filename, const img_t* image);

#define CC_CVTIMG_FLIP      1
#define CC_CVTIMG_SWAP_RB   2 
/* utility function: convert one image to another with optional vertical flip */
CVAPI(void) cvConvertImage(const img_t* src, img_t* dst, int flags CC_DEFAULT(0));

/* wait for key event infinitely (delay<=0) or for "delay" milliseconds */
CVAPI(int) WaitKey(int delay CC_DEFAULT(0));


/****************************************************************************************\
*                         Working with Video Files and Cameras                           *
\****************************************************************************************/

/* "black box" capture structure */
typedef struct CvCapture CvCapture;

/* start capturing frames from video file */
CVAPI(CvCapture*) cvCreateFileCapture(const char* filename);

#define CC_CAP_ANY      0     // autodetect

#define CC_CAP_MIL      100   // MIL proprietary drivers

#define CC_CAP_VFW      200   // platform native
#define CC_CAP_V4L      200
#define CC_CAP_V4L2     200

#define CC_CAP_FIREWARE 300   // IEEE 1394 drivers
#define CC_CAP_IEEE1394 300
#define CC_CAP_DC1394   300
#define CC_CAP_CMU1394  300

#define CC_CAP_STEREO   400   // TYZX proprietary drivers
#define CC_CAP_TYZX     400
#define CC_TYZX_LEFT    400
#define CC_TYZX_RIGHT   401
#define CC_TYZX_COLOR   402
#define CC_TYZX_Z       403

#define CC_CAP_QT       500   // QuickTime

/* start capturing frames from camera: index = camera_index + domain_offset (CC_CAP_*) */
CVAPI(CvCapture*) cvCreateCameraCapture(int index);

/* grab a frame, return 1 on success, 0 on fail. 
  this function is thought to be fast               */  
CVAPI(int) cvGrabFrame(CvCapture* capture);

/* get the frame grabbed with cvGrabFrame(..) 
  This function may apply some frame processing like 
  frame decompression, flipping etc.
  !!!DO NOT RELEASE or MODIFY the retrieved frame!!! */
CVAPI(img_t*) cvRetrieveFrame(CvCapture* capture);

/* Just a combination of cvGrabFrame and cvRetrieveFrame
   !!!DO NOT RELEASE or MODIFY the retrieved frame!!!      */
CVAPI(img_t*) cvQueryFrame(CvCapture* capture);

/* stop capturing/reading and free resources */
CVAPI(void) cvReleaseCapture(CvCapture** capture);

#define CC_CAP_PROP_POS_MSEC       0
#define CC_CAP_PROP_POS_FRAMES     1
#define CC_CAP_PROP_POS_AVI_RATIO  2
#define CC_CAP_PROP_FRAME_WIDTH    3
#define CC_CAP_PROP_FRAME_HEIGHT   4
#define CC_CAP_PROP_FPS            5
#define CC_CAP_PROP_FOURCC         6
#define CC_CAP_PROP_FRAME_COUNT    7 
#define CC_CAP_PROP_FORMAT         8
#define CC_CAP_PROP_MODE           9
#define CC_CAP_PROP_BRIGHTNESS    10
#define CC_CAP_PROP_CONTRAST      11
#define CC_CAP_PROP_SATURATION    12
#define CC_CAP_PROP_HUE           13
#define CC_CAP_PROP_GAIN          14
#define CC_CAP_PROP_CONVERT_RGB   15


/* retrieve or set capture properties */
CVAPI(double) cvGetCaptureProperty(CvCapture* capture, int property_id);
CVAPI(int)    cvSetCaptureProperty(CvCapture* capture, int property_id, double value);

/* "black box" video file writer structure */
typedef struct CvVideoWriter CvVideoWriter;

#define CC_FOURCC(c1,c2,c3,c4)  \
    (((c1)&255) + (((c2)&255)<<8) + (((c3)&255)<<16) + (((c4)&255)<<24))

/* initialize video file writer */
CVAPI(CvVideoWriter*) cvCreateVideoWriter(const char* filename, int fourcc,
                                           double fps, CSize frame_size,
                                           int is_color CC_DEFAULT(1));

/* write frame to video file */
CVAPI(int) cvWriteFrame(CvVideoWriter* writer, const img_t* image);

/* close video file writer */
CVAPI(void) cvReleaseVideoWriter(CvVideoWriter** writer);

/****************************************************************************************\
*                              Obsolete functions/synonyms                               *
\****************************************************************************************/

#ifndef HIGHGUI_NO_BACKWARD_COMPATIBILITY
    #define HIGHGUI_BACKWARD_COMPATIBILITY
#endif

#ifdef HIGHGUI_BACKWARD_COMPATIBILITY

#define cvCaptureFromFile cvCreateFileCapture
#define cvCaptureFromCAM cvCreateCameraCapture
#define cvCaptureFromAVI cvCaptureFromFile
#define cvCreateAVIWriter cvCreateVideoWriter
#define cvWriteToAVI cvWriteFrame
#define cvAddSearchPath(path)
#define cvvInitSystem cvInitSystem
#define cvvNamedWindow cvNamedWindow
#define cvvShowImage cvShowImage
#define cvvResizeWindow cvResizeWindow
#define cvvDestroyWindow cvDestroyWindow
#define cvvCreateTrackbar cvCreateTrackbar
#define cvvLoadImage(name) cvLoadImage((name),1)
#define cvvSaveImage cvSaveImage
#define cvvAddSearchPath cvAddSearchPath
#define cvvWaitKey(name) WaitKey(0)
#define cvvWaitKeyEx(name,delay) WaitKey(delay)
#define cvvConvertImage cvConvertImage
#define HG_AUTOSIZE CC_WINDOW_AUTOSIZE
#define set_preprocess_func cvSetPreprocessFuncWin32
#define set_postprocess_func cvSetPostprocessFuncWin32

#ifdef WIN32

typedef int (C_CDECL * CvWin32WindowCallback)(HWND, UINT, WPARAM, LPARAM, int*);
CVAPI(void) cvSetPreprocessFuncWin32(CvWin32WindowCallback on_preprocess);
CVAPI(void) cvSetPostprocessFuncWin32(CvWin32WindowCallback on_postprocess);

CC_INLINE int iplWidth(const img_t* img);
CC_INLINE int iplWidth(const img_t* img)
{
    return !img ? 0 : !img->roi ? img->width : img->roi->width;
}

CC_INLINE int iplHeight(const img_t* img);
CC_INLINE int iplHeight(const img_t* img)
{
    return !img ? 0 : !img->roi ? img->height : img->roi->height;
}

#endif

#endif /* obsolete functions */

/* For use with Win32 */
#ifdef WIN32

CC_INLINE RECT NormalizeRect(RECT r);
CC_INLINE RECT NormalizeRect(RECT r)
{
    int t;

    if(r.left > r.right)
    {
        t = r.left;
        r.left = r.right;
        r.right = t;
    }

    if(r.top > r.bottom)
    {
        t = r.top;
        r.top = r.bottom;
        r.bottom = t;
    }

    return r;
}

CC_INLINE CRect RectToCvRect(RECT sr);
CC_INLINE CRect RectToCvRect(RECT sr)
{
    sr = NormalizeRect(sr);
    return cRect(sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top);
}

CC_INLINE RECT CvRectToRect(CRect sr);
CC_INLINE RECT CvRectToRect(CRect sr)
{
    RECT dr;
    dr.left = sr.x;
    dr.top = sr.y;
    dr.right = sr.x + sr.width;
    dr.bottom = sr.y + sr.height;

    return dr;
}

CC_INLINE IplROI RectToROI(RECT r);
CC_INLINE IplROI RectToROI(RECT r)
{
    IplROI roi;
    r = NormalizeRect(r);
    roi.xOffset = r.left;
    roi.yOffset = r.top;
    roi.width = r.right - r.left;
    roi.height = r.bottom - r.top;
    roi.coi = 0;

    return roi;
}

#endif /* WIN32 */

#ifdef __cplusplus
}  /* end of extern "C" */
#endif /* __cplusplus */


#if defined __cplusplus && (!defined WIN32 || !defined (__GNUC__))

#define CImage CvvImage

/* CvvImage class definition */
class CC_EXPORTS CvvImage
{
public:
    CvvImage();
    virtual ~CvvImage();

    /* Create image (BGR or grayscale) */
    virtual bool  Create(int width, int height, int bits_per_pixel, int image_origin = 0);

    /* Load image from specified file */
    virtual bool  Load(const char* filename, int desired_color = 1);

    /* Load rectangle from the file */
    virtual bool  LoadRect(const char* filename,
                            int desired_color, CRect r);

#ifdef WIN32
    virtual bool  LoadRect(const char* filename,
                            int desired_color, RECT r)
    {
        return LoadRect(filename, desired_color,
                         cRect(r.left, r.top, r.right - r.left, r.bottom - r.top));
    }
#endif

    /* Save entire image to specified file. */
    virtual bool  Save(const char* filename);

    /* Get copy of input image ROI */
    virtual void  CopyOf(CvvImage& image, int desired_color = -1);
    virtual void  CopyOf(img_t* img, int desired_color = -1);

    img_t* GetImage() { return m_img; };
    virtual void  Destroy(void);

    /* width and height of ROI */
    int Width() { return !m_img ? 0 : !m_img->roi ? m_img->width : m_img->roi->width; };
    int Height() { return !m_img ? 0 : !m_img->roi ? m_img->height : m_img->roi->height;};
    int Bpp() { return m_img ? (m_img->depth & 255)*m_img->nChannels : 0; };

    virtual void  Fill(int color);

    /* draw to highgui window */
    virtual void  Show(const char* window);

#ifdef WIN32
    /* draw part of image to the specified DC */
    virtual void  Show(HDC dc, int x, int y, int width, int height,
                        int from_x = 0, int from_y = 0);
    /* draw the current image ROI to the specified rectangle of the destination DC */
    virtual void  DrawToHDC(HDC hDCDst, RECT* pDstRect);
#endif

protected:

    img_t*  m_img;
};

#endif /* __cplusplus */

#endif /* _HIGH_GUI_ */
