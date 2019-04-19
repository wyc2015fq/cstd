# OCR识别引擎tesseract使用方法 - xqhrs232的专栏 - CSDN博客
2016年12月06日 11:11:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2138
原文地址::[http://blog.csdn.net/lhfslhfs/article/details/7288389](http://blog.csdn.net/lhfslhfs/article/details/7288389)
相关文章
1、基于Qt和Opencv的OCR  ----[http://download.csdn.net/download/a310989583/9628222](http://download.csdn.net/download/a310989583/9628222)
2、tesseract-ocr中文识别问题----[http://bbs.csdn.net/topics/390558726](http://bbs.csdn.net/topics/390558726)
3、屏幕取词 被取对象为QT程序----[http://bbs.csdn.net/topics/390442683](http://bbs.csdn.net/topics/390442683)
4、vs2010+Qt5+Tesseract-OCR实现字符识别----[http://www.codeforge.cn/article/257490](http://www.codeforge.cn/article/257490)
5、[OCR之Tesseract在VS2010中的使用](http://blog.csdn.net/yangbingzhou/article/details/20230533)----[http://blog.csdn.net/yangbingzhou/article/details/20230533](http://blog.csdn.net/yangbingzhou/article/details/20230533)
  最近一直跟OCR打交道，学习了下google的OCR引擎TESSERACT，是个很好的识别工具。tesseract-3.0已支持版面分析，功能很强大。安装tesseract前可选择性地安装leptonica和libtiff。不过建议先安装这两个库。不安装tiff的话只能处理bmp文件。
     这里只是说明怎么识别中文。依次安装好libtiff,leptonica和tesseract后，下载简体中文和繁体中文的训练数据，在tesseract的下载页可以找到。放到某个目录的tessdata文件夹下。然后设置环境变量TESSDATA_PREFIX=tessdata的目录。然后，新建一个ocr.cpp文件，编写如下代码：
#include <mfcpch.h>
#include <ctype.h>
#include <sys/time.h>
#include "applybox.h"
#include "control.h"
#include "tessvars.h"
#include "tessedit.h"
#include "baseapi.h"
#include "thresholder.h"
#include "pageres.h"
#include "imgs.h"
#include "varabled.h"
#include "tprintf.h"
#include "stderr.h"
#include "notdll.h"
#include "mainblk.h"
#include "output.h"
#include "globals.h"
#include "helpers.h"
#include "blread.h"
#include "tfacep.h"
#include "callnet.h"
#include "allheaders.h"
int main(int argc,char **argv){
if(argc!=3){
printf("usage:%s <bmp file> <txt file>/n",argv[0]);
return -1;
}
char *image_file=argv[1];
char *txt_file=argv[2];
STRING text_out;
struct timeval beg,end;
tesseract::TessBaseAPI  api;
IMAGE image;
api.Init(argv[0], "chi_sim", NULL, 0, false);//初始化api对象
api.SetPageSegMode(tesseract::PSM_AUTO);//设置自动进行版面分析
api.SetAccuracyVSpeed(tesseract::AVS_FASTEST);//要求速度最快
if (image.read_header(image_file) < 0) {//读取bmp文件的元信息
printf("Read of file %s failed./n", image_file);
exit(1);
    }
if (image.read(image.get_ysize ()) < 0){//读取bmp文件
printf("Read of image %s error/n", image_file);
exit(1);
}
invert_image(&image);//反转图像的每个像素，即使1变0,0变1
int bytes_per_line = check_legal_image_size(image.get_xsize(),
                                                image.get_ysize(),
                                                image.get_bpp());//计算每一行像素所占字节数
    api.SetImage(image.get_buffer(), image.get_xsize(), image.get_ysize(),
                  image.get_bpp() / 8, bytes_per_line);//设置图像
gettimeofday(&beg,NULL);
char* text = api.GetUTF8Text();//识别图像中的文字
gettimeofday(&end,NULL);
printf("%s:reconize sec=%f/n",argv[0],end.tv_sec-beg.tv_sec+(double)(end.tv_usec-beg.tv_usec)/1000000.0);//打印识别的时间
text_out += text;
delete [] text;
FILE* fout = fopen(txt_file, "w");
fwrite(text_out.string(), 1, text_out.length(), fout);//将识别结果写入输出文件
fclose(fout);
}
再编写一个makefile文件如下：
all:ocr
CFLAGS=-Wall -g
LDFLAGS= -lz  -lm  -ltesseract_textord /
-ltesseract_wordrec -ltesseract_classify -ltesseract_dict -ltesseract_ccstruct/
-ltesseract_ccstruct -ltesseract_cutil -ltesseract_viewer -ltesseract_ccutil/
-ltesseract_api -ltesseract_image -ltesseract_main -llept
LD_LIBRARY_PATH =
INCLUDES= -I/usr/local/include/tesseract/ -I/usr/local/include/leptonica/
%.o:%.cpp
g++ -c $(CFLAGS) $(INCLUDES)  $(SOURCE) -o $@ $<
ocr:ocr.o
g++ -o $@ $^ -g $(LD_LIBRARY_PATH) $(LDFLAGS)
clean:
rm ocr.o
    在该目录下运行make编译成可执行文件ocr,运行./ocr 1.bmp 1.txt就可以将图像1.bmp识别结果写到1.txt了，程序会打印识别的时间。值得注意的是，tesseract中文识别速度很慢，运行几分钟很正常。不知有哪位大虾知道怎么调优？
   更郁闷的是tesseract不支持多线程，不能在同一进程中运行多个实例。。
[](http://blog.csdn.net/lhfslhfs/article/details/7288389#)[](http://blog.csdn.net/lhfslhfs/article/details/7288389#)[](http://blog.csdn.net/lhfslhfs/article/details/7288389#)[](http://blog.csdn.net/lhfslhfs/article/details/7288389#)[](http://blog.csdn.net/lhfslhfs/article/details/7288389#)[](http://blog.csdn.net/lhfslhfs/article/details/7288389#)
# 屏幕取词 被取对象为QT程序
