# sws_scale函数的用法-具体应用 - DoubleLi - 博客园







移植ffmpeg过程中，遇到swscale的用法问题，所以查到这篇文章。文章虽然已经过去很长时间，但是还有颇多可以借鉴之处。谢谢“咕咕鐘"。



    转自：http://guguclock.blogspot.com/2009/12/ffmpeg-swscale.html



    如果想將某個PixelFormat轉換至另一個PixelFormat，例如，將YUV420P轉換成YUYV422，或是想變換圖的大小，都可以使用swscale達成。

其中，PixelFormat 的列表在 libavutil/pixfmt.h 內定義。

swscale的用法可以參考libswscale/swscale-example.c的sample code。主要function有三個
sws_getContext() 
sws_scale() 
sws_freeContext() 

其中，我們可以把sws_getContext() 看成初始化函數，把sws_freeContext()看成結束函數。這兩個函數分別在起始及結束各執行一次即可。

真正主要的函數，是sws_scale()。

sws_getContext() 的宣告如下

SwsContext *sws_getContext(int srcW, int srcH, enum PixelFormat srcFormat, int dstW, int dstH, enum PixelFormat dstFormat, int flags, SwsFilter *srcFilter, SwsFilter *dstFilter, const double *param)

總共有十個參數，其中，較重要的是前七個； 
前三個參數分別代表 source 的寬、高及PixelFormat； 
四到六個參數分別代表 destination 的寬、高及PixelFormat； 
第七個參數則代表要使用哪種scale的方法；此參數可用的方法可在 libswscale/swscale.h 內找到。

最後三個參數，如無使用，可以都填上NULL。

sws_getContext會回傳一個 SwsContext struct，我們可以把這個 struct 看成是個 handler，之後的sws_scale和sws_freeContext皆會用到。

以下是一個sws_getContext的簡單例子：

struct SwsContext *img_convert_ctx; 
img_convert_ctx = sws_getContext(in_width, in_height, PIX_FMT_YUV420P, 
out_width, out_height, PIX_FMT_YUV420P, SWS_POINT, 
NULL, NULL, NULL);

一開始，我們宣告img_convert_ctx 為指向 SwsContext 的一個 pointer；接著，我們將 sws_getContext 的回傳值賦予給img_convert_ctx。

注意sws_getContext的參數；in_width及in_height分別代表 source 的寬及高，out_width及out_height分別代表轉換後的寬與高；input 和 output 的 PixelFormat 皆為 YUV420P；使用SWS_POINT的scale方法。

初始化完成後，接著就要進行主要的 scale 動作；我們透過 sws_scale() 完成。sws_scale() 的宣告如下

int sws_scale(SwsContext *c, uint8_t* src[], int srcStride[], int srcSliceY, int srcSliceH, uint8_t* dst[], int dstStride[])

總共有七個參數； 
第一個參數即是由 sws_getContext 所取得的參數。 
第二個 src 及第六個 dst 分別指向input 和 output 的 buffer。 
第三個 srcStride 及第七個 dstStride 分別指向 input 及 output 的 stride；如果不知道什麼是 stride，姑且可以先把它看成是每一列的 byte 數。
第四個 srcSliceY，就註解的意思來看，是指第一列要處理的位置；這裡我是從頭處理，所以直接填0。想知道更詳細說明的人，可以參考 swscale.h 的註解。
第五個srcSliceH指的是 source slice 的高度。

舉一個例子如下

sws_scale(img_convert_ctx, inbuf, inlinesize, 0, in_height, outbuf, outlinesize);

這裡應該比較好懂，可以參考上面的參數說明。

最後，全部處理完後，需呼叫sws_freeContext() 結束。用法很簡單，把sws_getContext 取得的參數填入即可。如下

sws_freeContext(img_convert_ctx);

最後再整理一次，要使用swscale，只要使用 sws_getContext() 進行初始化、sws_scale() 進行主要轉換、sws_freeContext() 結束，即可完成全部動作。

以下為一個簡單的範例程式，可從foreman.yuv內取出第一張圖，轉換大小後存成另一張圖。

=====================================================================================

/* 
* 需設定 SRCFILE 及 DSTFILE, 長寬等資訊 
* 需 link libswscale 
* 主要有三個 function 
* sws_getContext() 是 initial 用, sws_freeContext() 是結束用 
* sws_scale() 是主要運作的 function 
* 預設只會轉換第一張 YUV, 如果要轉換整個檔, 可以把 Decoding loop 的註解拿掉 
*/ 

#include "libswscale/swscale.h" 

#define SRCFILE "foreman_cif.yuv" 
#define DSTFILE "out.yuv" 

int main() 
{ 
// 設定原始 YUV 的長寬 
const int in_width = 352; 
const int in_height = 288; 
// 設定目的 YUV 的長寬 
const int out_width = 640; 
const int out_height = 480; 

const int read_size = in_width * in_height * 3 / 2; 
const int write_size = out_width * out_height * 3 / 2; 
struct SwsContext *img_convert_ctx; 
uint8_t *inbuf[4]; 
uint8_t *outbuf[4]; 
int inlinesize[4] = {in_width, in_width/2, in_width/2, 0}; 
int outlinesize[4] = {out_width, out_width/2, out_width/2, 0}; 

uint8_t in[352*288*3>>1]; 
uint8_t out[640*480*3>>1]; 

FILE *fin = fopen(SRCFILE, "rb"); 
FILE *fout = fopen(DSTFILE, "wb"); 

if(fin == NULL) { 
printf("open input file %s error.\n", SRCFILE); 
return -1; 
} 

if(fout == NULL) { 
printf("open output file %s error.\n", DSTFILE); 
return -1; 
} 

inbuf[0] = malloc(in_width*in_height); 
inbuf[1] = malloc(in_width*in_height>>2); 
inbuf[2] = malloc(in_width*in_height>>2); 
inbuf[3] = NULL; 

outbuf[0] = malloc(out_width*out_height); 
outbuf[1] = malloc(out_width*out_height>>2); 
outbuf[2] = malloc(out_width*out_height>>2); 
outbuf[3] = NULL; 

// ********* Initialize software scaling ********* 
// ********* sws_getContext ********************** 
img_convert_ctx = sws_getContext(in_width, in_height, PIX_FMT_YUV420P, 
out_width, out_height, PIX_FMT_YUV420P, SWS_POINT, 
NULL, NULL, NULL); 
if(img_convert_ctx == NULL) { 
fprintf(stderr, "Cannot initialize the conversion context!\n"); 
return -1; 
} 

fread(in, 1, read_size, fin); 

memcpy(inbuf[0], in, in_width*in_height); 
memcpy(inbuf[1], in+in_width*in_height, in_width*in_height>>2); 
memcpy(inbuf[2], in+(in_width*in_height*5>>2), in_width*in_height>>2); 

// ********* 主要的 function ****** 
// ********* sws_scale ************ 
sws_scale(img_convert_ctx, inbuf, inlinesize, 
0, in_height, outbuf, outlinesize); 

memcpy(out, outbuf[0], out_width*out_height); 
memcpy(out+out_width*out_height, outbuf[1], out_width*out_height>>2); 
memcpy(out+(out_width*out_height*5>>2), outbuf[2], out_width*out_height>>2); 

fwrite(out, 1, write_size, fout); 

// ********* 結束的 function ******* 
// ********* sws_freeContext ******* 
sws_freeContext(img_convert_ctx); 

fclose(fin); 
fclose(fout); 

return 0; 
} 

=====================================================================================

以下兩張圖為執行結果

Input Image

![](http://img.my.csdn.net/uploads/201301/25/1359081201_8076.png)

Output Image

![](http://img.my.csdn.net/uploads/201301/25/1359081212_1265.png)




- 顶
0- 踩










