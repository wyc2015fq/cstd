
#include "cstd.h"
#include "img/color.inl"
#include "img/imgio.inl"

enum
{
  NORMAL_CLONE = 1,
    MIXED_CLONE  = 2,
    MONOCHROME_TRANSFER = 3
};

typedef struct Cloning
{
  img_t rgbx_channel[4], rgby_channel[4], output[4];
  img_t destinationGradientX[1], destinationGradientY[1];
  img_t patchGradientX[1], patchGradientY[1];
  img_t binaryMaskFloat[1], binaryMaskFloatInverted[1];
  
  float* filter_X;
  float* filter_Y;
} Cloning;

static int Cloning_free(Cloning* s) {
  imfrees(s->rgbx_channel, 4);
  imfrees(s->rgby_channel, 4);
  imfrees(s->output, 4);
  imfree(s->destinationGradientX);
  imfree(s->destinationGradientY);
  imfree(s->patchGradientX);
  imfree(s->patchGradientY);
  imfree(s->binaryMaskFloat);
  imfree(s->binaryMaskFloatInverted);
  FREE(s->filter_X);
  FREE(s->filter_Y);
  return 0;
}

#include "seamless_cloning_impl.inl"

/** @brief Image editing tasks concern either global changes (color/intensity corrections, filters,
deformations) or local changes concerned to a selection. Here we are interested in achieving local
changes, ones that are restricted to a region manually selected (ROI), in a seamless and effortless
manner. The extent of the changes ranges from slight distortions to complete replacement by novel
content @cite PM03 .

@param src Input 8-bit 3-channel image.
@param dst Input 8-bit 3-channel image.
@param mask Input 8-bit 1 or 3-channel image.
@param p Point in dst image where object is placed.
@param blend Output image with the same size and type as dst.
@param flags Cloning method that could be one of the following:
-   **NORMAL_CLONE** The power of the method is fully expressed when inserting objects with
complex outlines into a new background
-   **MIXED_CLONE** The classic method, color-based selection and alpha masking might be time
consuming and often leaves an undesirable halo. Seamless cloning, even averaged with the
original image, is not effective. Mixed seamless cloning based on a loose selection proves
effective.
-   **FEATURE_EXCHANGE** Feature exchange allows the user to easily replace certain features of
one object by alternative features.
 */
static int cvSeamlessClone(const img_t* src, const img_t* dst, const img_t* mask, IPOINT p, img_t* blend, int flags)
{
  img_t im[10] = {0};
  imsetsize(blend, dst->h, dst->w, 3, 1);
  
  int minx = INT_MAX, miny = INT_MAX, maxx = INT_MIN, maxy = INT_MIN;
  int h = mask->h;
  int w = mask->w;
  int i=0, j;
  img_t* gray = im+i++;
  img_t* dst_mask = im+i++;
  img_t* cs_mask = im+i++;
  img_t* cd_mask = im+i++;
  img_t* patch = im+i++;
  
  
  //imsetsize(gray, h, w, 1, 1);
  imzeros(dst_mask, dst->h, dst->w, 1, 1);
  imzeros(cs_mask, src->h, src->w, 3, 1);
  imzeros(cd_mask, dst->h, dst->w, 3, 1);
  
  if(mask->c == 3) {
    //cvtColor(mask, gray, COLOR_BGR2GRAY );
    imcolorcvt(mask, gray, T_BGR, T_GRAY);
  } else {
    //gray = mask;
    imclone2(mask, gray);
  }
  
  for(i=0;i<h;i++) {
    uchar* gray1 = gray->tt.data + i*gray->s;
    for(j=0;j<w;j++) {
      if(gray1[j] == 255) {
        minx = MIN(minx,i);
        maxx = MAX(maxx,i);
        miny = MIN(miny,j);
        maxy = MAX(maxy,j);
      }
    }
  }
  
  int lenx = maxx - minx;
  int leny = maxy - miny;
  
  imzeros(patch, lenx, leny, 3, 1);
#if 0
  int minxd = p.y - lenx/2;
  int maxxd = p.y + lenx/2;
  int minyd = p.x - leny/2;
  int maxyd = p.x + leny/2;
#else
  int minxd = p.y + minx;
  int maxxd = p.y + maxx;
  int minyd = p.x + miny;
  int maxyd = p.x + maxy;
#endif
  
  CC_ASSERT(minxd >= 0 && minyd >= 0 && maxxd <= dst->h && maxyd <= dst->w);
  
  //IRECT roi_d = iRECT2(minyd,minxd,leny,lenx);
  //IRECT roi_s = iRECT2(miny,minx,leny,lenx);
  
  //img_t destinationROI = imroi(dst_mask, roi_d);
  //img_t sourceROI = imroi(cs_mask, roi_s);
  
  //gray(roi_s).copyTo(destinationROI);
  imcopyto(gray, miny, minx, dst_mask, minyd, minxd, leny,lenx);
  //src(roi_s).copyTo(sourceROI,gray(roi_s));
  imcopyto_mask(src, miny, minx, cs_mask, miny,minx, gray, miny,minx, leny,lenx);
  //src(roi_s).copyTo(patch, gray(roi_s));
  imcopyto_mask(src, miny, minx, patch, 0,0, gray, miny,minx, leny,lenx);
  //imshow_(dst);  imshow_(src);  imshow_(mask);  imshow_(gray);  cvWaitKey(-1);
  //destinationROI = cd_mask(roi_d);
  //cs_mask(roi_s).copyTo(destinationROI);
  imcopyto(cs_mask, miny, minx, cd_mask, minyd, minxd, leny,lenx);
  
  //imshow_(src); imshow_(dst); imshow_(cs_mask); imshow_(cd_mask); imshow_(dst_mask);  WaitKey(-1);
  

  //imwrite("dest.bmp", dst);
  //imwrite("cs_mask.bmp", cs_mask);
  //imwrite("cd_mask.bmp", cd_mask);
	//imwrite("dst_mask.bmp", dst_mask);
  Cloning obj[1] = {0};
  normalClone(obj, dst,cd_mask,dst_mask,blend,flags);
  
  imfrees(im, 10);
  Cloning_free(obj);
  return 0;
}

#if 0

void cv::colorChange(InputArray _src, InputArray _mask, OutputArray _dst, float r, float g, float b)
{
    CV_INSTRUMENT_REGION()

    Mat src  = _src.getMat();
    Mat mask  = _mask.getMat();
    _dst.create(src.size(), src.type());
    Mat blend = _dst.getMat();

    float red = r;
    float green = g;
    float blue = b;

    Mat gray = Mat::zeros(mask.size(),CV_8UC1);

    if(mask.channels() == 3)
        cvtColor(mask, gray, COLOR_BGR2GRAY );
    else
        gray = mask;

    Mat cs_mask = Mat::zeros(src.size(),CV_8UC3);

    src.copyTo(cs_mask,gray);

    Cloning obj;
    obj.localColorChange(src,cs_mask,gray,blend,red,green,blue);
}

void cv::illuminationChange(InputArray _src, InputArray _mask, OutputArray _dst, float a, float b)
{
    CV_INSTRUMENT_REGION()


    Mat src  = _src.getMat();
    Mat mask  = _mask.getMat();
    _dst.create(src.size(), src.type());
    Mat blend = _dst.getMat();
    float alpha = a;
    float beta = b;

    Mat gray = Mat::zeros(mask.size(),CV_8UC1);

    if(mask.channels() == 3)
        cvtColor(mask, gray, COLOR_BGR2GRAY );
    else
        gray = mask;

    Mat cs_mask = Mat::zeros(src.size(),CV_8UC3);

    src.copyTo(cs_mask,gray);

    Cloning obj;
    obj.illuminationChange(src,cs_mask,gray,blend,alpha,beta);

}

void cv::textureFlattening(InputArray _src, InputArray _mask, OutputArray _dst,
                           float low_threshold, float high_threshold, int kernel_size)
{
    CV_INSTRUMENT_REGION()


    Mat src  = _src.getMat();
    Mat mask  = _mask.getMat();
    _dst.create(src.size(), src.type());
    Mat blend = _dst.getMat();

    Mat gray = Mat::zeros(mask.size(),CV_8UC1);

    if(mask.channels() == 3)
        cvtColor(mask, gray, COLOR_BGR2GRAY );
    else
        gray = mask;

    Mat cs_mask = Mat::zeros(src.size(),CV_8UC3);

    src.copyTo(cs_mask,gray);

    Cloning obj;
    obj.textureFlatten(src,cs_mask,gray,low_threshold,high_threshold,kernel_size,blend);
}

#endif
