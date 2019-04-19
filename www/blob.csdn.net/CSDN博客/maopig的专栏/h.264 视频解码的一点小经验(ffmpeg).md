# h.264 视频解码的一点小经验(ffmpeg) - maopig的专栏 - CSDN博客
2011年08月02日 14:46:07[maopig](https://me.csdn.net/maopig)阅读数：7639标签：[h.264																[codec																[video																[stream																[buffer																[initialization](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)](https://so.csdn.net/so/search/s.do?q=h.264&t=blog)
个人分类：[ffmpeg																[x264 h264](https://blog.csdn.net/maopig/article/category/847676)](https://blog.csdn.net/maopig/article/category/847674)
最近做视频文件264解码，由于对这个领域不是很熟悉，感觉困难重重。不过经过不懈的努力，已经取得一些进展，心里感觉特别庆幸。 刚开始做这个的时候，由于不熟悉，就在网上搜寻资料，网络上的资料虽然多，但是却很杂乱，因此一开始走了不少弯路，现在把我的一点小小心得写出来，后来的兄弟们可以参考一下，没准能够少走些弯路。当然啦，我在视频处理方面仍然是个非常菜的菜鸟，如果是高手路过，看到我这所谓的“心得”，也请不要见笑，看到不对的地方请批评指正，呵呵。
刚开始做的时候，先是在网络上查找资料，我觉得有一篇文章非常的有用，因为当时我最需要了解的就是世界上现存的各种编解码器，每种都有什么特性，比如说解码速度是否能够满足实时播放的需求、对h.264标准的支持程度等等。这篇文章就是《H.264开源解码器评测》，这篇文章详细的评测了当今流行的几种h.264解码器，包括JM Decoder,T264 Decoder,X264 Decoder,ffmpeg libavcodec和Intel的IPP库,经过作者的评测，发现速度最快的就是intel IPP了，但是intel
 IPP属于商品化软件，而其他的各种解码器都属于开源项目，所以最适合选择的就是解码速度第二的ffmpeg了，而且其速度完全可以满足实时播放的要求；
选择好了解码器，第一步算是完成了，第二步就是研究ffmpeg的用法了。经过摸索，我的选择是：到中华视频网下在ffmpeg SDK 2.0,这恐怕是目前最适合在V[C++](http://www.rosoo.net/a/list_100_1.html)6下使用的基于ffmpeg的SDK了，其易用性比较好。
第三步就是编写播放器外壳了，外壳代码采用VC++6编写，我会在文张末尾给出外壳的所有代码；注意：外科代码获取的lpdata是windows内存位图，具有dword对齐的特性，另外，解码出的图像是倒立的，因此我专门写了一个把图像倒转的函数，运行速度还是挺快的，完全不妨碍实时播放；
上一阶段的工作完成得还算满意，下一阶段的工作就是h.264 的 RTP payload[协议](http://www.rosoo.net/a/list_46_1.html)了。
附录：
h.264播放的外壳代码-------------------------------------------------------------------------------------------------
// Decode264.cpp : Defines the initialization routines for the DLL.
#include "stdafx.h"
#include "Decode264.h"
//以下代码为自己添加////////////////////////////
#include <stdlib.h>
#include <time.h>
#include "avformat.h"
#include "avcodec.h"
#include <windows.h>
//定义目标格式
#define DEST_FORMAT PIX_FMT_BGR24
//PIX_FMT_YUV420P
//定义全局变量
AVFormatContext *pFormatCtx; //
int i, videoStream;
AVCodecContext *pCodecCtx; 
AVCodec *pCodec; //编解码器
AVFrame *pFrame; //帧
AVFrame *pFrameYUV; //YUV帧
clock_t t;
double fps;
int y_size, i_frame=0;
int numBytes;
uint8_t *buffer; 
////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//
// Note!
//
// If this DLL is dynamically linked against the MFC
// DLLs, any functions exported from this DLL which
// call into MFC must have the AFX_MANAGE_STATE macro
// added at the very beginning of the function.
//
// For example:
//
// extern "C" BOOL PASCAL EXPORT ExportedFunction()
// {
// AFX_MANAGE_STATE(AfxGetStaticModuleState());
// // normal function body here
// }
//
// It is very important that this macro appear in each
// function, prior to any calls into MFC. This means that
// it must appear as the first statement within the 
// function, even before any object variable declarations
// as their constructors may generate calls into the MFC
// DLL.
//
// Please see MFC Technical Notes 33 and 58 for additional
// details.
//
/////////////////////////////////////////////////////////////////////////////
// CDecode264App
BEGIN_MESSAGE_MAP(CDecode264App, CWinApp)
//{{AFX_MSG_MAP(CDecode264App)
// NOTE - the ClassWizard will add and remove mapping macros here.
// DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CDecode264App construction
CDecode264App::CDecode264App()
{
// TODO: add construction code here,
// Place all significant initialization in InitInstance
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CDecode264App object
CDecode264App theApp;
//以下代码为自己添加/////////////////////////////////////////////////////////
//把图像倒立过来；
long UpendBmp(unsigned char *lpdata,long width ,long height)
{
long lBPL;//每行的字节数，因为要考虑dword对齐
long x,y,idx_src,idx_dest;
unsigned char *tmpdata;
if (0==((width*3)%4)) //nWidth * 3 是存储每行像素需要的字节数，如果是4的整数倍。
lBPL = (width*3); //那么返回 nWidth * 3 ，就是每行的字节数
else //如果不是4的整数倍，那么就一定要加上一个数，达到4的整数倍，才是每行的字节数。
lBPL = (width*3+(4-((width*3)%4)));
tmpdata= new unsigned char[lBPL * height];
x =0;
for (y=0 ; y<height ; y++)
{
idx_src =(height-1-y)*lBPL;//idx_src =(height-1-y)*lBPL+x*3;优化前
idx_dest=y*lBPL;//idx_dest=y*lBPL+x*3;优化前
memcpy(&tmpdata[idx_dest],&lpdata[idx_src],lBPL);//复制一行
}
memcpy(lpdata,tmpdata,lBPL * height);
delete[] tmpdata;
return 0;
}
//创建一个bmp文件。用于调试
static int av_create_bmp(char* filename,uint8_t *pRGBBuffer,
int width,int height,int bpp)
{
BITMAPFILEHEADER bmpheader;
BITMAPINFO bmpinfo;
FILE *fp;
fp = fopen(filename,"wb");
if(!fp)return -1;
bmpheader.bfType = (''M''<<8)|''B'';
bmpheader.bfReserved1 = 0;
bmpheader.bfReserved2 = 0;
bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
bmpheader.bfSize = bmpheader.bfOffBits + width*height*bpp/8;
bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
bmpinfo.bmiHeader.biWidth = width;
bmpinfo.bmiHeader.biHeight = height;
bmpinfo.bmiHeader.biPlanes = 1;
bmpinfo.bmiHeader.biBitCount = bpp;
bmpinfo.bmiHeader.biCompression = BI_RGB;
bmpinfo.bmiHeader.biSizeImage = 0;
bmpinfo.bmiHeader.biXPelsPerMeter = 100;
bmpinfo.bmiHeader.biYPelsPerMeter = 100;
bmpinfo.bmiHeader.biClrUsed = 0;
bmpinfo.bmiHeader.biClrImportant = 0;
fwrite(&bmpheader,sizeof(BITMAPFILEHEADER),1,fp);
fwrite(&bmpinfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);
fwrite(pRGBBuffer,width*height*bpp/8,1,fp);
fclose(fp);
return 0;
}
//获取下一帧
static bool GetNextFrame(AVFormatContext *pFormatCtx, 
AVCodecContext *pCodecCtx,
int videoStream, 
AVFrame *pFrame)
{
static AVPacket packet; //AV包。静态变量。
static int bytesRemaining=0; //字节剩余。静态变量。
static uint8_t *rawData; //原始数据字节数。静态变量。
static bool fFirstTime=true; //标志，第一次；。静态变量。
int bytesDecoded; //解码后获得的字节；
int frameFinished; //帧解码完毕标志；
// First time we''re called, set packet.data to NULL to indicate it
// doesn''t have to be freed 当第一次被调用的时候，把packet.data设置为NULL,以表示
//它没有必要被释放；
if (fFirstTime){
fFirstTime = false;
packet.data = NULL;
}
//解码那些包，直到我们解码出一个完整的帧;
// Decode packets until we have decoded a complete frame
while (true)
{
//在当前包上工作，直到我们解码出所有的。
//Work on the current packet until we have decoded all of it
while (bytesRemaining > 0)
{
// Decode the next chunk of data 解码出下一个数据块
bytesDecoded = avcodec_decode_video(pCodecCtx, pFrame,
&frameFinished, rawData, bytesRemaining);
// Was there an error?
if (bytesDecoded < 0){
fprintf(stderr, "Error while decoding frame\\n");
return false;
}
bytesRemaining -= bytesDecoded;
rawData += bytesDecoded;
// Did we finish the current frame? Then we can return
if (frameFinished) //如果我们完成了当前帧的解码，就可以返回了
return true;
}
//读取下一个包，跳过所有的不是属于这个流的包；
// Read the next packet, skipping all packets that aren''t for this
// stream
do{
// Free old packet 释放旧包
if(packet.data != NULL)
av_free_packet(&packet);
// Read new packet 读取新包
if(av_read_frame(pFormatCtx, &packet) < 0)
goto loop_exit;
} while(packet.stream_index != videoStream); //当不是要找的视频流的时候，继续循环，就是重新读了；
//直到找到要找的视频流，退出循环；
bytesRemaining = packet.size; //纪录包的字节数；
rawData = packet.data; //
}
loop_exit:
// Decode the rest of the last frame
bytesDecoded = avcodec_decode_video(pCodecCtx, pFrame, &frameFinished, 
rawData, bytesRemaining);
// Free last packet
if(packet.data != NULL)
av_free_packet(&packet);
return frameFinished != 0;
}
//对外的API接口。打开264文件，并且获取必要的信息，比如宽度高度帧数等等
long __stdcall open264file(char *filename,long *out_width ,
long *out_height,long *out_framenum,
long *out_bufsize)
{
// Register all formats and codecs 注册所有的格式和编解码器
av_regi[FS:PAGE]ster_all();
// Open video file//打开视频文件
if (av_open_input_file(&pFormatCtx, filename, NULL, 0, NULL) != 0) 
return -1; // Couldn''t open file如果不能打开，那么返回-1
// Retrieve stream information 取流信息 
if (av_find_stream_info(pFormatCtx) < 0)
return -1; // Couldn''t find stream information
// Dump information about file onto standard error
dump_format(pFormatCtx, 0, filename, false);
t = clock();
// Find the first video stream 寻找第一个视频流
videoStream = -1;
for (i=0; i<pFormatCtx->nb_streams; i++)
if(pFormatCtx->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO){
videoStream=i;
break;
}
if (videoStream == -1)
return -1; // Didn''t find a video stream
//获取该视频流的一个编解码器上下文的指针；
// Get a pointer to the codec context for the video stream 
pCodecCtx = pFormatCtx->streams[videoStream]->codec;
// Find the decoder for the video stream 获取解码器
pCodec = avcodec_find_decoder(pCodecCtx->codec_id); 
if (pCodec == NULL)
return -1; // Codec not found解码器没有找到；
//告知解码器，我们能处理被删节的位流
// 也就是说，帧的分界处的位流可以落到包的中间；
// Inform the codec that we can handle truncated bitstreams -- i.e., 
// bitstreams where frame boundaries can fall in the middle of packets 
if ( pCodec->capabilities & CODEC_CAP_TRUNCATED )
pCodecCtx->flags|=CODEC_FLAG_TRUNCATED;
// Open codec //打开解码器
if ( avcodec_open(pCodecCtx, pCodec) < 0 )
return -1; // Could not open codec 不能打开解码器，返回-1；
// Allocate video frame 分配视频帧
pFrame = avcodec_alloc_frame();
// Allocate an AVFrame structure 分配一个AVFrame结构
pFrameYUV=avcodec_alloc_frame(); //解码后的帧
if(pFrameYUV == NULL)
return -1;
//决定需要多大的缓冲空间，并且分配空间；
// Determine required buffer size and allocate buffer 
numBytes=avpicture_get_size(DEST_FORMAT, pCodecCtx->width,
pCodecCtx->height);
buffer = (uint8_t*)malloc(numBytes);
//向外界输出宽高、帧数；
*out_width = pCodecCtx->width;
*out_height = pCodecCtx->height;
*out_framenum = pCodecCtx->frame_number;
*out_bufsize = numBytes;
// Assign appropriate parts of buffer to image planes in pFrameRGB 
//把缓冲区中合适的部分指派到pFrameRGB中的图像面板
avpicture_fill((AVPicture *)pFrameYUV, buffer, DEST_FORMAT,
pCodecCtx->width, pCodecCtx->height);
return 0;
}
//对外的API接口。关闭264文件，释放相关资源
long __stdcall close264file()
{
//calculate decode rate 计算解码速率
t = clock() - t;
fps = (double)(t) / CLOCKS_PER_SEC;
fps = i_frame / fps;
printf("\\n==>Decode rate %.4f fps!\\n", fps);
// Free the YUV image 释放yuv图像
free(buffer);
av_free(pFrameYUV);
// Free the YUV frame 释放yuv帧
av_free(pFrame);
// Close the codec 关闭解码器
avcodec_close(pCodecCtx);
// Close the video file 关闭视频文件
av_close_input_file(pFormatCtx);
return 0;
}
//对外的API接口。获取一帧解码后的数据
long __stdcall GetNextFrame(unsigned char *lpdata)
{
// Read frames 读取个个帧
if (GetNextFrame(pFormatCtx, pCodecCtx, videoStream, pFrame))
{
img_convert((AVPicture *)pFrameYUV, DEST_FORMAT, (AVPicture*)pFrame, 
pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
//调试用，向C盘写入一个bmp文件；
//av_create_bmp("c:\\\\1.bmp",(unsigned char *)pFrameYUV->data[0],pCodecCtx->width,pCodecCtx->height,24);
i_frame++;
y_size = pCodecCtx->width * pCodecCtx->height;
//写入文件
/*fwrite(pFrameYUV->data[0], 1, y_size, fp);
fwrite(pFrameYUV->data[1], 1, (y_size/4), fp);
fwrite(pFrameYUV->data[2], 1, (y_size/4), fp);*/
memcpy(lpdata,pFrameYUV->data[0],y_size*3);
UpendBmp(lpdata,pCodecCtx->width,pCodecCtx->height);
return 0;
}
else
{return -1;}
}
