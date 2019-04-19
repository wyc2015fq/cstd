# FFmpeg中MPEG-4编码程序追踪,其他FFMPEG自带编码器同理 - maopig的专栏 - CSDN博客
2011年09月07日 13:58:23[maopig](https://me.csdn.net/maopig)阅读数：1927
FFmpeg中与MPEG-4编码相关的主要文件有mpegvideo.c，h263.c等。
在mpegvideo.c中，
AVCodec mpeg4_encoder = {
   "mpeg4",
   CODEC_TYPE_VIDEO,
   CODEC_ID_MPEG4,
   sizeof(MpegEncContext),
   MPV_encode_init,
   MPV_encode_picture,
   MPV_encode_end,
    .pix_fmts=(enum PixelFormat[]){PIX_FMT_YUV420P, -1},
   .capabilities= CODEC_CAP_DELAY,
};
MPV_encode_picture()
{
 encode_picture();
}
encode_picture()
{
 //对每个宏块MB进行运动估计
 estimate_motion_thread();
 encode_thread();
}
encode_thread()
{
 encode_mb_hq();
}
encode_mb_hq()
{
 encode_mb();
}
encode_mb()
{
 encode_mb_internal();
}
encode_mb_internal()
{
 //对单一的宏块进行运动补偿
 MPV_motion();
 //DCT & quantize
 dct_quantize_c();
 //huffman encode
 mpeg4_encode_mb();
}
帧内和帧间的量化矩阵在mpeg4data.h中
const int16_t ff_mpeg4_default_intra_matrix[64] = {
  8, 17, 18, 19, 21, 23, 25, 27,
 17, 18, 19, 21, 23, 25, 27, 28,
 20, 21, 22, 23, 24, 26, 28, 30,
 21, 22, 23, 24, 26, 28, 30, 32,
 22, 23, 24, 26, 28, 30, 32, 35,
 23, 24, 26, 28, 30, 32, 35, 38,
 25, 26, 28, 30, 32, 35, 38, 41,
 27, 28, 30, 32, 35, 38, 41, 45,
};
const int16_t ff_mpeg4_default_non_intra_matrix[64] = {
 16, 17, 18, 19, 20, 21, 22, 23,
 17, 18, 19, 20, 21, 22, 23, 24,
 18, 19, 20, 21, 22, 23, 24, 25,
 19, 20, 21, 22, 23, 24, 26, 27,
 20, 21, 22, 23, 25, 26, 27, 28,
 21, 22, 23, 24, 26, 27, 28, 30,
 22, 23, 24, 26, 27, 28, 30, 31,
 23, 24, 25, 27, 28, 30, 31, 33,
};
D、量化矩阵
mpeg中的量化是对8x8 YUV 信号图块进行DCT变换之后的系数的量化。通过对高频分量使用比较大的量化阶从而达到减小高频分量的编码空间，达到压缩的目的。代价就是丧失图像细节，边缘模糊等。
1）帧内编码量化矩阵：这是指对I帧使用的帧内编码量化矩阵
2）帧间编码量化矩阵：是指对非I帧的帧间预测误差编码所用的量化矩阵。威龙汉化版中叫外部矩阵
