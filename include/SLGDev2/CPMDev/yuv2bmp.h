#ifndef __YUV_2_BMP__H
#define __YUV_2_BMP__H
#ifdef __cplusplus
extern "C"
{
#endif
  void InitialTable() ;
  void RGB_C_YUV420(unsigned char* pRGB, unsigned char* pYUV, int height, int width);
  void YUV420_C_RGB(unsigned char* pYUV, unsigned char* pRGB, int height, int width);
  void RGB_C_YUV422p(unsigned char* rgb, unsigned char* yuv, int height, int width);
  void YUV422p_C_RGB(unsigned char* yuv, unsigned char* rgb, int height, int width);
  void RGB_C_YUV422(unsigned char* rgb, unsigned char* yuv, int height, int width);
  void YUV422_C_RGB(unsigned char* yuv, unsigned char* rgb, int height, int width);
  void YUYV_C_RGB(unsigned char* yuv, unsigned char* rgb, int height, int width);
  void SaveRGB(unsigned char* rgb, int height, int width, const char* path);
  void RGB_EDDY(unsigned char* src_rgb, int height, int width, unsigned char* dest_rgb);
#ifdef __cplusplus
}
#endif
#endif

