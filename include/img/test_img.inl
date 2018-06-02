
#define WINVER 0x0500 // this is needed for UpdateLayeredWindow
#define _WIN32_WINNT 0x0500 // this is needed for the WS_EX_LAYERED
//#include "debug.h"

#include "cstd.h"

#ifndef MEM_CHECK_BEGIN
#define MEM_CHECK_BEGIN()
#endif // MEM_CHECK_BEGIN
#ifndef MEM_CHECK_END
#define MEM_CHECK_END()
#endif // MEM_CHECK_END

//#include "imgio.inl"
#include "ui/window.inl"


#if 1
//#include "fmtimg1/fmtimg.inl"
//#include "cap.inl"
//#include "findfile.h"
//#include "test_sift.inl"
//#include "imload.inl"
#include "imgio.inl"
#include "imgcache.inl"
#include "quantizer.inl"
#else
#include "test_yuv2bmp.inl"
#include "test_imresize.inl"
#include "test_WarpAffine.inl"
#include "test_cap.inl"
#include "test_Carving_Pattern.inl"
#include "test_sse2.inl"
#include "test_smooth.inl"
#include "test_BrightAndContrast.inl"
#include "test_imresize.inl"
#include "color.inl"
//#include "window.inl"
//#include "imdraw.inl"
#include "sftime.h"

//#include "../fmtimg/bmpfile.inl"
//#include "../fmtimg/fmtimg.inl"
#include "findfile.h"
//#include "icofile.inl"
//#include "tgafile.inl"
#include "..\imdraw\ui.h"
//#include "test_imgproc.inl"
//#include "test_cv.inl"
//#include "test_contours.inl"
//#include "test_imdraw.inl"
//#include ".\draw\win.inl"
//#include "test_drawaa.inl"
//#include ".\draw\test_draw.inl"
//#include "test_geometry1.inl"
//#include "headcounts.inl"
//#include "test_bgfg2.inl"
//#include "test_svg.inl"
//#include "GDIBITMAP.inl"
//#include "../monofilt/test_pushbox.inl"
//#include "codec2.inl"
//#include "test_distrans.inl"
//#include "test_paint.inl"
//#include "test_geometry.inl"
//#include "test_Graphviz.inl"
//#include "../GPRS/test_mms.inl"
//#include "test_abssub.inl"
//#include "test_isee.inl"
//#include "test_zip.inl"
//#include "test_svg.inl"
#include "test_euler_distance.inl"
#include "test_bgfg2.inl"
#include "test_cam_face_recog.inl"
#include "test_clickpoint.inl"
#include "test_clickpoint2.inl"
#include "imeffect.inl"
#include "img/imgopt.inl"
#include "ui/window.inl"
#include "draw/imdraw.inl"
#include "cv/cvcanny.inl"
#include "test_FitLine.inl"
//#include "test_hough.inl"
//#include "test_ps.inl"
#include "test_cam.inl"
#include "fmtimg/fmtimg.inl"
#include "test_jpgcode.inl"
//#include "test_cvFindContours.inl"
#endif


//#include "cv\cxmatrix.inl"

//#include "test_perceptual_hash.inl"
int test_img()
{
#if 1
  //BCGPGLOBAL_DATA_set();
  MEM_CHECK_BEGIN();
  test_imgio();
  //test_msize();
  //test_quantizer();
  //test_imgio();
  //test_wav();
  //test_imresize();
  //test_WarpAffine();
  //test_fitline();
  MEM_CHECK_END();
#else
  test_imgcache();
  test_yuv2bmp();
  test_cam();
  test_Carving_Pattern();
  test_sse2();
  test_stitcher();
  test_FindContours();
  test_matmul();
  test_sift();
  test_BrightAndContrast();
  test_cam2();
  test_jpgcode();
  test_ps();
  test_hough();
  test_cam();
  test_cam_face_recog();
  test_showsvg();
  test_buttonst();
  test_circles();
  test_clickpoint2();
  test_aa_demo();
  test_spirograph();
  test_clickpoint();
  test_imsubref();
  test_cam();
  test_bgfg_pic();
  test_wav();
  test_fractal();
  test_capopenni();
  test_dshow();
  test_conv_stroke();
  test_gradients();
  test_aa_test();
  test_circles();
  test_spirograph();
  test_image_perspective();
  test_showims();
  test_HBITMAP();
  test_SHGetFileInfo();
  test_perceptual_hash();
  test_BucketSort();
  test_gradient_focal();
  test_kinect();
  test_rand_chinese();
  test_BorderStyle();
  test_imfile_to_inlfile();
  test_zip();
  test_tab();
  test_clip_image();
  test_conv_stroke();
  //test_aa_test();
  //test_layout_it();
  //tets_bezier_div();
  //test_stbi();
  //test_imresize();
  test_jmp();
  test_draw();
  //_chdir("F:/pub/≥µ≈∆ ∂± ”∆µÕº∆¨/ff");
  //_chdir("F:/pub/Ω¸∫ÏÕ‚—€æ¶≤‚ ‘Õº∆¨/DSPÕº∆¨/‘≠Õº/9009");
  test_icofile();
  test_img2avi();
  test_yuv2bmp();
  test_yuv2bmp();
  test_distrans(argc, argv);
  test_imglib();
  test_mms();
  test_3D();
  test_xml();
  test_loadbmpfile();
  test_bwlabel3();
  test_imdilate();
  test_multiply1();
  test_bgfg_pic();
  test_newwin();
  test_pushbox();
  test_svg();
  test_bwlabel3();

  test_findfile();
  test_draw_aa();
  test_drawing();
  test_cap();
  test_avi();
  test_aa();
  test_cvFilter2D();
#endif
  return 0;
}
