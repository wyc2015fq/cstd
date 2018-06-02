void InitialTable();

void YUV422_C_RGB(unsigned char* yuv, unsigned char* rgb, int height, int width);

void RGB_EDDY(unsigned char* src_rgb, int height, int width, unsigned char* dest_rgb);

void YUYV_C_RGB(unsigned char* yuv, unsigned char* rgb, int height, int width);
