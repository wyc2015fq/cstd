# VC操作PDF文件 - 深之JohnChen的专栏 - CSDN博客

2019年03月05日 00:11:36[byxdaz](https://me.csdn.net/byxdaz)阅读数：812


一、使用PDFLib操作PDF文件

PDFLib是用于创建PDF文档的开发库，提供了简单易用的API，隐藏了创建PDF的复杂细节且不需要第3方软件的支持。PDFLib库对于个人是免费的，对于商业产品需要购买许可。
[https://www.pdflib.com/](https://www.pdflib.com/)

使用PDFLib库引用头文件和库文件

#include "PDFLib\PDFLib.hpp"

#pragma comment(lib, "PDFLib\\PDFLib.lib")

1.pdf常用操作

```
#define RADIUS        200.0f
#define MARGIN        20.0f

void CFilePdfDemoDlg::OnBtnPdfCreate() 
{
    PDFlib pdf;
    
    //设置兼容参数
    pdf.set_parameter("compatibility", "1.4");
    
    //打开文档
    if (-1 == pdf.open("PdfTest.pdf"))
    {
        MessageBox("打开文件出错！");
        return;
    }
    
    //设置文档信息
    pdf.set_info("Creator","PDF Creator");
    pdf.set_info("Author","test");
    pdf.set_info("Title","Convert to PDF");
    pdf.set_info("Subject","PDF Creator");
    pdf.set_info("Keywords","vckbase.com");
    
    //开始A4页面
    pdf.begin_page(a4_width,a4_height);
    
    //设置字体为12号宋体
    int font_song = pdf.findfont("STSong-Light","GB-EUC-H",0);
    int font = pdf.findfont("Arial","host", 0);
    pdf.setfont(font_song,12);
    //设置起始点
    pdf.set_text_pos(50,a4_height-50);
    //设置颜色为蓝色
    pdf.setcolor("fill","rgb",0,0,1,0);
    //输出文字
    pdf.show("pdf欢迎您!");
    
    pdf.setcolor("fill","rgb",1,0,0,0);
    pdf.setfont(font,24);
    pdf.continue_text("this is just a test!");
    
    //画两根绿线
    pdf.setcolor("stroke", "rgb", 0.24f, 0.51f, 0.047f, 0);
    pdf.moveto(50, a4_height - 80);
    pdf.lineto(a4_width - 50, a4_height - 80);
    pdf.moveto(50, a4_height - 78);
    pdf.lineto(a4_width - 50, a4_height - 78);
    pdf.stroke();
    
    //填充一个蓝色方框
    pdf.setcolor("fill", "rgb", 0.04f, 0.24f, 0.62f, 0);
    pdf.rect(50, 50, a4_width - 100, 70);
    pdf.fill();
    
    //在指定位置输出文字
    pdf.setcolor("fill", "rgb", 1, 1, 1, 0);
    pdf.setfont(font_song, 16);
    pdf.show_xy("啦啦啦", a4_width - 150, 60);
    
    //打开并显示一个图像
    int img = pdf.open_image_file("jpeg", "test.jpg", "", 0);
    pdf.place_image(img, 200, 400, 1);
    pdf.close_image(img);
    
    //添加附件
    //宽度 高度 x坐标 Y坐标 附件名称 描述 作者 后缀名 图标
    pdf.attach_file(a4_width - 300, 0, 0, a4_height - 400, "test.rar", "description", "author", "zip", "paperclip");
    
    //结束本页
    pdf.end_page();
    
    //关闭PDF文件
    pdf.close();
    
    MessageBox("PdfTest.pdf成功生成!");
}
```

2.去除水印

 最简单也是最直接的方法下载破解版的PDFLib，下载PDFLib-9.1.2-win32。解压缩后，将pdflib中的pdflib.dll pdflib.lib pdflib.h拷贝到项目文件中。并且在VS项目属性->附加依赖项中加入pdflib.lib

3.显示中文字符

PDFlib-CMap为了用PDFlib创建中文，日文或韩文（CJK）文本输出并使用其中一个预定义的CMaps。下载PDFlib-CMap-3.0，并解压缩到任何路径，但是必须在使用时配置CMap文件路径，

p.set_option("SearchPath=/path/to/resource/cmap"); or PDF_set_option(p,"SearchPath=/path/to/resource/cmap")使用最常用的宋体STSong-Light（PDFLib自带的中文字体），解码方式是 GB-EUC-H（在cmap中）

```
#include <stdio.h>
#include<iostream>
#include "pdflib.h"
 
int main(int argc, char *argv[])
 
{
 
    PDF *pdf = PDF_new();
    // 打开文档
    if (PDF_begin_document(pdf, "d://hello.pdf", 0, "") == -1)
    {
        printf("Error: %sn", PDF_get_errmsg(pdf));
        return(0);
    }
    PDF_TRY(pdf)
    {
        PDF_set_option(pdf, "SearchPath=./PDFlib-CMap-3.0/resource/cmap");//配置cmap
        PDF_set_info(pdf, "Creator", "PDF Creator");
        PDF_set_info(pdf, "Title", "Convert to PDF");
        PDF_begin_page_ext(pdf, a4_width, a4_height, "");// 开始A4页面
        int nHeight = a4_height;//当前高度
        int font_song = PDF_load_font(pdf, "STSong-Light", 0, "GB-EUC-H", "");
        PDF_setfont(pdf, font_song, 34);// 设置字体为34号宋体
        // 设置起始点
        nHeight -= 50;
        PDF_set_text_pos(pdf, 50, nHeight);
        // 设置颜色为蓝色
        PDF_setcolor(pdf, "fill", "rgb", 0, 0, 1, 0);
        static const char *DFTitle[] = { "你好!", "中国。" };
        for (int i = 0; i < 2; i++)
        {
            PDF_set_text_pos(pdf, 100* i, nHeight-40*i);//字体坐标
            PDF_show(pdf, DFTitle[i]);
        }
         
    }
    PDF_CATCH(pdf)
    {
        printf("PDFlib exception occurred in sample:/n");
 
        printf("[%d] %s: %s/n",
 
            PDF_get_errnum(pdf), PDF_get_apiname(pdf), PDF_get_errmsg(pdf));
 
    }
    PDF_end_page_ext(pdf, "");// 结束本页
    PDF_end_document(pdf, "");// 关闭PDF文件
    PDF_delete(pdf);
     
    return 0;

}
```

二、使用Libharu库操作PDF文件

Libharu库是使用c语言来生成pdf文件的库。在编译libharu需要用到zlib库和libpng库（libpng 依赖于zlib库）。

zlib 下载地址：http://www.zlib.net/

libpng下载地址：http://www.libpng.org/pub/png/libpng.html

libharu下载地址：https://github.com/libharu/libharu

//测试实例代码

```
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "hpdf.h"
 
const char *text = "This PDF should have an attachment named attach.c";
 
jmp_buf env;
 
#if defined(WIN32) || defined(_WIN64)
  #pragma warning (disable: 4996)  //_s   //declared deprecated ?
#endif
 
#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler(HPDF_STATUS   error_no,
    HPDF_STATUS   detail_no,
    void         *user_data)
{
    printf("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
        (HPDF_UINT)detail_no);
    longjmp(env, 1);
}
 
int
main(int argc, char **argv)
{
    HPDF_Doc  pdf;
    HPDF_Font font;
    HPDF_Page page;
    char fname[256];
    HPDF_REAL tw;
 
    strcpy(fname, argv[0]);
    strcat(fname, ".pdf");
 
    pdf = HPDF_New(error_handler, NULL);
    if (!pdf) {
        printf("error: cannot create PdfDoc object\n");
        return 1;
    }
 
    if (setjmp(env)) {
        HPDF_Free(pdf);
        return 1;
    }
 
    /* create default-font */
    font = HPDF_GetFont(pdf, "Helvetica", NULL);
 
    /* add a new page object. */
    
    page = HPDF_AddPage(pdf);
 
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_LETTER, HPDF_PAGE_PORTRAIT);
 
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, font, 20);
    tw = HPDF_Page_TextWidth(page, text);
    HPDF_Page_MoveTextPos(page, (HPDF_Page_GetWidth(page) - tw) / 2,
        (HPDF_Page_GetHeight(page) - 20) / 2);
    HPDF_Page_ShowText(page, text);
    HPDF_Page_EndText(page);
    
 
    /* attach a file to the document */
    //HPDF_AttachFile(pdf, "attach.c");
    //HPDF_AttachFile(pdf, "jpeg_demo.c");
 
    /* save the document to a file */
    HPDF_SaveToFile(pdf, fname);
 
    /* clean up */
    HPDF_Free(pdf);
 
    return 0;
}
```

//pdf 中文字体生成，及字体旋转代码

```
/*
 * << Haru Free PDF Library 2.0.0 >> -- text_demo.c
 *
 * Copyright (c) 1999-2006 Takeshi Kanno <takeshi_kanno@est.hi-ho.ne.jp>
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include "hpdf.h"
#include "grid_sheet.h"

jmp_buf env;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler (HPDF_STATUS   error_no,
               HPDF_STATUS   detail_no,
               void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}

void
show_stripe_pattern  (HPDF_Page   page,
                      HPDF_REAL   x,
                      HPDF_REAL   y)
{
    HPDF_UINT iy = 0;

    while (iy < 50) {
        HPDF_Page_SetRGBStroke (page, 0.0, 0.0, 0.5);
        HPDF_Page_SetLineWidth (page, 1);
        HPDF_Page_MoveTo (page, x, y + iy);
        HPDF_Page_LineTo (page, x + HPDF_Page_TextWidth (page, "ABCabc123"),
                    y + iy);
        HPDF_Page_Stroke (page);
        iy += 3;
    }

    HPDF_Page_SetLineWidth (page, 2.5);
}

void
show_description  (HPDF_Page          page,
                   HPDF_REAL          x,
                   HPDF_REAL          y,
                   const char   *text)
{
    float fsize = HPDF_Page_GetCurrentFontSize (page);
    HPDF_Font font = HPDF_Page_GetCurrentFont (page);
    HPDF_RGBColor c = HPDF_Page_GetRGBFill (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetRGBFill (page, 0, 0, 0);
    HPDF_Page_SetTextRenderingMode (page, HPDF_FILL);
    HPDF_Page_SetFontAndSize (page, font, 10);
    HPDF_Page_TextOut (page, x, y - 12, text);
    HPDF_Page_EndText (page);

    HPDF_Page_SetFontAndSize (page, font, fsize);
    HPDF_Page_SetRGBFill (page, c.r, c.g, c.b);
}

int main (int argc, char **argv)
{
    const char *page_title = "Text Demo";

    HPDF_Doc  pdf;
    HPDF_Font font;
    HPDF_Page page;
    char fname[256];

    const char* samp_text = "abcdefgABCDEFG123!#$%&+-@?";
    const char* samp_text2 = "The quick brown fox jumps over the lazy dog.";
    float tw;
    float fsize;
    int i;
    int len;

    float angle1;
    float angle2;
    float rad1;
    float rad2;

    float ypos;

    strcpy (fname, argv[0]);
    strcat (fname, ".pdf");

    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        return 1;
    }

    /* set compression mode */
    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

    /* create default-font */
    font = HPDF_GetFont (pdf, "Helvetica", NULL);

    /* add a new page object. */
    page = HPDF_AddPage (pdf);

    /* draw grid to the page */
    print_grid  (pdf, page);

    /* print the lines of the page.
    HPDF_Page_SetLineWidth (page, 1);
    HPDF_Page_Rectangle (page, 50, 50, HPDF_Page_GetWidth(page) - 100,
                HPDF_Page_GetHeight (page) - 110);
    HPDF_Page_Stroke (page);
    */

    /* print the title of the page (with positioning center). */
    HPDF_Page_SetFontAndSize (page, font, 24);
    tw = HPDF_Page_TextWidth (page, page_title);
    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, (HPDF_Page_GetWidth(page) - tw) / 2,
                HPDF_Page_GetHeight (page) - 50, page_title);
    HPDF_Page_EndText (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, 60, HPDF_Page_GetHeight(page) - 60);

    /*
     * font size
     */
    fsize = 8;
    while (fsize < 60) {
        char buf[50];
        int len;

        /* set style and size of font. */
        HPDF_Page_SetFontAndSize(page, font, fsize);

        /* set the position of the text. */
        HPDF_Page_MoveTextPos (page, 0, -5 - fsize);

        /* measure the number of characters which included in the page. */
        strcpy(buf, samp_text);
        len = HPDF_Page_MeasureText (page, samp_text,
                        HPDF_Page_GetWidth(page) - 120, HPDF_FALSE, NULL);

        /* truncate the text. */
        buf[len] = 0x00;

        HPDF_Page_ShowText (page, buf);

        /* print the description. */
        HPDF_Page_MoveTextPos (page, 0, -10);
        HPDF_Page_SetFontAndSize(page, font, 8);
        #ifdef __WIN32__
        _snprintf(buf, 50, "Fontsize=%.0f", fsize);
        #else
        snprintf(buf, 50, "Fontsize=%.0f", fsize);
        #endif
        HPDF_Page_ShowText (page, buf);

        fsize *= 1.5;
    }

    /*
     * font color
     */
    HPDF_Page_SetFontAndSize(page, font, 8);
    HPDF_Page_MoveTextPos (page, 0, -30);
    HPDF_Page_ShowText (page, "Font color");

    HPDF_Page_SetFontAndSize (page, font, 18);
    HPDF_Page_MoveTextPos (page, 0, -20);
    len = strlen (samp_text);
    for (i = 0; i < len; i++) {
        char buf[2];
        float r = (float)i / (float)len;
        float g = 1 - ((float)i / (float)len);
        buf[0] = samp_text[i];
        buf[1] = 0x00;

        HPDF_Page_SetRGBFill (page, r, g, 0.0);
        HPDF_Page_ShowText (page, buf);
    }
    HPDF_Page_MoveTextPos (page, 0, -25);

    for (i = 0; i < len; i++) {
        char buf[2];
        float r = (float)i / (float)len;
        float b = 1 - ((float)i / (float)len);
        buf[0] = samp_text[i];
        buf[1] = 0x00;

        HPDF_Page_SetRGBFill (page, r, 0.0, b);
        HPDF_Page_ShowText (page, buf);
    }
    HPDF_Page_MoveTextPos (page, 0, -25);

    for (i = 0; i < len; i++) {
        char buf[2];
        float b = (float)i / (float)len;
        float g = 1 - ((float)i / (float)len);
        buf[0] = samp_text[i];
        buf[1] = 0x00;

        HPDF_Page_SetRGBFill (page, 0.0, g, b);
        HPDF_Page_ShowText (page, buf);
    }

    HPDF_Page_EndText (page);

    ypos = 450;

    /*
     * Font rendering mode
     */
    HPDF_Page_SetFontAndSize(page, font, 32);
    HPDF_Page_SetRGBFill (page, 0.5, 0.5, 0.0);
    HPDF_Page_SetLineWidth (page, 1.5);

     /* PDF_FILL */
    show_description (page,  60, ypos,
                "RenderingMode=PDF_FILL");
    HPDF_Page_SetTextRenderingMode (page, HPDF_FILL);
    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, 60, ypos, "ABCabc123");
    HPDF_Page_EndText (page);

    /* PDF_STROKE */
    show_description (page, 60, ypos - 50,
                "RenderingMode=PDF_STROKE");
    HPDF_Page_SetTextRenderingMode (page, HPDF_STROKE);
    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, 60, ypos - 50, "ABCabc123");
    HPDF_Page_EndText (page);

    /* PDF_FILL_THEN_STROKE */
    show_description (page, 60, ypos - 100,
                "RenderingMode=PDF_FILL_THEN_STROKE");
    HPDF_Page_SetTextRenderingMode (page, HPDF_FILL_THEN_STROKE);
    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, 60, ypos - 100, "ABCabc123");
    HPDF_Page_EndText (page);

    /* PDF_FILL_CLIPPING */
    show_description (page, 60, ypos - 150,
                "RenderingMode=PDF_FILL_CLIPPING");
    HPDF_Page_GSave (page);
    HPDF_Page_SetTextRenderingMode (page, HPDF_FILL_CLIPPING);
    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, 60, ypos - 150, "ABCabc123");
    HPDF_Page_EndText (page);
    show_stripe_pattern (page, 60, ypos - 150);
    HPDF_Page_GRestore (page);

    /* PDF_STROKE_CLIPPING */
    show_description (page, 60, ypos - 200,
                "RenderingMode=PDF_STROKE_CLIPPING");
    HPDF_Page_GSave (page);
    HPDF_Page_SetTextRenderingMode (page, HPDF_STROKE_CLIPPING);
    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, 60, ypos - 200, "ABCabc123");
    HPDF_Page_EndText (page);
    show_stripe_pattern (page, 60, ypos - 200);
    HPDF_Page_GRestore (page);

    /* PDF_FILL_STROKE_CLIPPING */
    show_description (page, 60, ypos - 250,
                "RenderingMode=PDF_FILL_STROKE_CLIPPING");
    HPDF_Page_GSave (page);
    HPDF_Page_SetTextRenderingMode (page, HPDF_FILL_STROKE_CLIPPING);
    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, 60, ypos - 250, "ABCabc123");
    HPDF_Page_EndText (page);
    show_stripe_pattern (page, 60, ypos - 250);
    HPDF_Page_GRestore (page);

    /* Reset text attributes */
    HPDF_Page_SetTextRenderingMode (page, HPDF_FILL);
    HPDF_Page_SetRGBFill (page, 0, 0, 0);
    HPDF_Page_SetFontAndSize(page, font, 30);

    /*
     * Rotating text
     */
    angle1 = 30;                   /* A rotation of 30 degrees. */
    rad1 = angle1 / 180 * 3.141592; /* Calcurate the radian value. */

    show_description (page, 320, ypos - 60, "Rotating text");
    HPDF_Page_BeginText (page);
    HPDF_Page_SetTextMatrix (page, cos(rad1), sin(rad1), -sin(rad1), cos(rad1),
                330, ypos - 60);
    HPDF_Page_ShowText (page, "ABCabc123");
    HPDF_Page_EndText (page);

    /*
     * Skewing text.
     */
    show_description (page, 320, ypos - 120, "Skewing text");
    HPDF_Page_BeginText (page);

    angle1 = 10;
    angle2 = 20;
    rad1 = angle1 / 180 * 3.141592;
    rad2 = angle2 / 180 * 3.141592;

    HPDF_Page_SetTextMatrix (page, 1, tan(rad1), tan(rad2), 1, 320, ypos - 120);
    HPDF_Page_ShowText (page, "ABCabc123");
    HPDF_Page_EndText (page);

    /*
     * scaling text (X direction)
     */
    show_description (page, 320, ypos - 175, "Scaling text (X direction)");
    HPDF_Page_BeginText (page);
    HPDF_Page_SetTextMatrix (page, 1.5, 0, 0, 1, 320, ypos - 175);
    HPDF_Page_ShowText (page, "ABCabc12");
    HPDF_Page_EndText (page);

    /*
     * scaling text (Y direction)
     */
    show_description (page, 320, ypos - 250, "Scaling text (Y direction)");
    HPDF_Page_BeginText (page);
    HPDF_Page_SetTextMatrix (page, 1, 0, 0, 2, 320, ypos - 250);
    HPDF_Page_ShowText (page, "ABCabc123");
    HPDF_Page_EndText (page);

    /*
     * char spacing, word spacing
     */

    show_description (page, 60, 140, "char-spacing 0");
    show_description (page, 60, 100, "char-spacing 1.5");
    show_description (page, 60, 60, "char-spacing 1.5, word-spacing 2.5");

    HPDF_Page_SetFontAndSize (page, font, 20);
    HPDF_Page_SetRGBFill (page, 0.1, 0.3, 0.1);

    /* char-spacing 0 */
    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, 60, 140, samp_text2);
    HPDF_Page_EndText (page);

    /* char-spacing 1.5 */
    HPDF_Page_SetCharSpace (page, 1.5);

    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, 60, 100, samp_text2);
    HPDF_Page_EndText (page);

    /* char-spacing 1.5, word-spacing 3.5 */
    HPDF_Page_SetWordSpace (page, 2.5);

    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, 60, 60, samp_text2);
    HPDF_Page_EndText (page);

    /* save the document to a file */
    HPDF_SaveToFile (pdf, fname);

    /* clean up */
    HPDF_Free (pdf);

    return 0;
}
```

libHaru在windows下的编译，参考[https://www.cnblogs.com/nightnine/p/8947379.html](https://www.cnblogs.com/nightnine/p/8947379.html)

libHaru的坐标系统、图形模式介绍，参考：[https://blog.csdn.net/anobodykey/article/details/63683843](https://blog.csdn.net/anobodykey/article/details/63683843)

三、libharu和pdflib 两种写pdf库文件比较

pdflib 8.0版本使用后发现存在如下问题：

1.只能在第一页中进行操作，不能生产后续页面；

2.使用中文有问题，在4.0的版本下能使用，但是在8.0版本下中文无法正常使用；

3.最大的问题，由于需要收费，生成的pdf页面背景有水印，这是无法忍受的。

前面2个问题，可能有办法解决，由于时间关系没有更多去研究。

libharu版本可以解决上述的问题。具体使用时需要注意如下几点：

1.在使用中文时，一定要先申明使用的字体和编码为中文，及需要加上HPDF_UseCNSFonts();HPDF_UseCNTFonts();HPDF_UseCNTEncodings();HPDF_UseCNSEncodings()这四句话。

2.用HPDF_GetFont来使用中文字体时，第二个参数发现只能使用"SimSun"和"SimHei"这2种字体，即宋体和黑体两种字体，而且还只能是这种写法，其它像"SIMSUN"、"SIMHEI"这些写法都会抛出异常。不知道是不是老外没有将更多通用的字体加到库中，不过也知足了，总算能将中文正常显示出来了。

3.可以使用HPDF_AddPage()来生成后续页面，HPDF_Page_MoveToNextLine()来换行。

4.对于具体内容的输出，输出的字符一定要是带\x00结尾的字符，不然pdf中会有很多的乱字符出现。

5.HPDF_Page_MoveTextPos()将输出光标移动到某个位置，需要注意的地方，纵坐标是从页面的最下方开始算的，这点和一般的坐标起始点不一样。

