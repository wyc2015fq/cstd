# OpenCV在图片中输出中文乱码解决方案 - DumpDoctorWang的博客 - CSDN博客





2018年04月03日 21:13:23[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：2325








# 一、缘起

在一个项目中需要把中文字符输出到图片上，也就是输出到Mat上，OpenCV 的putText函数不能输出中文。通过搜索，网上普遍的解决方案是使用freetype库来输出中文，结果还是乱码，图片上是方框。经过艰苦的调试（前前后后调了几天bug），终于折腾出一个能用的版本。

# 二、出现乱码的原因

使用freetype这个库，可以说是非常复杂。要保证程序源码、setlocale和FT_Select_CharMap三者的编码一致，才不会乱码。

程序源码的编码当然很重要，编码不同，其中汉字的编码也不同；setlocale是用来配置地域的信息，设置当前程序使用的本地化信息，很多函数依赖于这个函数的设置(比如mbstowcs函数，功能是将char字符串数组转换为wchar_t字符数组，依赖setlocale函数的设置来判断char字符数组的编码)；FT_Select_CharMap是freetype库中用来设置字符编码的函数；因此，要想不出现乱码，这三者的编码必须要一样。

还有一个非常关键的问题，字体文件必须包含中文。

在我的程序中，三者的编码均为UTF-8。

# 三、代码

本文的代码在https://github.com/Mannix1994/OpenCVUtils更新。

1. 代码

首先放上使用代码，simhei.ttf是Windows系统自带的字体。

```cpp
Mat mat = ...;
CvText text("simhei.ttf");//字体文件需要放在程序目录，或者在参数中指定字体路径
//text.setFont函数可以修改字体大小等属性，具体请看注释
string str = "EnglishTest中文测试";
text.putText(mat,str,Point(5,25),Scalar(255,255,255));
imshow("CvText.bmp",mat);
```

CvText.h

```cpp
#ifndef CV_TEXT_H
#define CV_TEXT_H

#include <opencv2/opencv.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

class CvText {
public:

    /**
     * 构造函数，初始化一个字体
     * @param fontName 字体名称
     */
    explicit CvText(const char *fontName);

    virtual ~CvText();

    /**
     * 设置字体属性，属性为空时保持默认值
     * @param type 类型
     * @param size 大小
     * @param underline 下划线
     * @param diaphaneity 透明度
     */
    void setFont(int *type, cv::Scalar *size = nullptr,
                 bool *underline = nullptr, float *diaphaneity = nullptr);

    /**
     * 恢复默认字体设置
     */
    void restoreFont();

    /**
     * 将text的内容放到frame的指定位置(pos)，默认文本颜色为黑色。遇到不能输出的字符将停止。
     * @param frame 输出的影象
     * @param text 文本内容
     * @param pos 文本位置
     * @param color 文本颜色
     * @return 返回成功输出的字符长度，失败返回-1。
     */
    int putText(cv::Mat &frame, std::string text, cv::Point pos,
                cv::Scalar color = cv::Scalar(0, 0, 0));

    /**
      * 将text的内容放到frame的指定位置(pos)，默认颜色为黑色。遇到不能输出的字符将停止。
      * @param frame 输出的影象
      * @param text 文本内容
      * @param pos 文本位置
      * @param color 文本颜色
      * @return 返回成功输出的字符长度，失败返回-1。
      */
    int putText(cv::Mat &frame, const char *text, cv::Point pos,
                cv::Scalar color = cv::Scalar(0, 0, 0));

    //私有函数区
private:
    /**
     * 输出wc到frame的pos位置
     * @param frame 输出Mat
     * @param wc 字符
     * @param pos 位置
     * @param color 颜色
     */
    void putWChar(cv::Mat &frame, wchar_t wc, cv::Point &pos, cv::Scalar color);

    /**
     * 将char字符数组转换为wchar_t字符数组
     * @param src char字符数组
     * @param dst wchar_t字符数组
     * @param locale 语言环境，mbstowcs函数依赖此值来判断src的编码方式
     * @return 运行成功返回0,否则返回-1
     */
    int char2Wchar(const char *&src, wchar_t *&dst, const char *locale = "zh_CN.utf8");

    //私有变量区
private:
    FT_Library m_library;   // 字库
    FT_Face m_face;         // 字体

    // 默认的字体输出参数
    int m_fontType;
    cv::Scalar m_fontSize;
    bool m_fontUnderline;
    float m_fontDiaphaneity;
};

#endif // CV_TEXT_H
```

CvText.cpp

```cpp
#include <cwchar>
#include <clocale>
#include <cctype>
#include <utility>
#include "tools.h"
#include "CvText.h"

CvText::CvText(const char *fontName) {
    ASSERT(fontName != nullptr,"字体名称为空");

    // 打开字库文件, 创建一个字体
    if (FT_Init_FreeType(&m_library)) throw;
    if (FT_New_Face(m_library, fontName, 0, &m_face)) throw;
    FT_Select_Charmap(m_face,FT_ENCODING_UNICODE);

    // 设置字体输出参数
    restoreFont();

    // 设置C语言的字符集环境
//    setlocale(LC_ALL, "zh_CN.utf8");
}

// 释放FreeType资源
CvText::~CvText() {
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_library);
}

// 设置字体属性
void CvText::setFont(int *type, cv::Scalar *size, bool *underline, float *diaphaneity) {
    // 参数合法性检查
    if (type) {
        if (type >= 0) m_fontType = *type;
    }
    if (size) {
        m_fontSize.val[0] = fabs(size->val[0]);
        m_fontSize.val[1] = fabs(size->val[1]);
        m_fontSize.val[2] = fabs(size->val[2]);
        m_fontSize.val[3] = fabs(size->val[3]);
    }
    if (underline) {
        m_fontUnderline = *underline;
    }
    if (diaphaneity) {
        m_fontDiaphaneity = *diaphaneity;
    }
}

// 恢复默认的字体设置
void CvText::restoreFont() {
    m_fontType = 0;             // 字体类型(不支持)

    m_fontSize.val[0] = 15;     // 字体大小
    m_fontSize.val[1] = 0.5;    // 空白字符大小比例
    m_fontSize.val[2] = 0.1;    // 间隔大小比例
    m_fontSize.val[3] = 0;      // 旋转角度(不支持)

    m_fontUnderline = false;    // 下画线(不支持)

    m_fontDiaphaneity = 1.0;    // 色彩比例(可产生透明效果)

    // 设置字符大小
    FT_Set_Pixel_Sizes(m_face, (FT_UInt) m_fontSize.val[0], 0);
}


int CvText::putText(cv::Mat &frame, std::string text, cv::Point pos, cv::Scalar color) {
    return putText(frame,text.c_str(),pos, std::move(color));
}

int CvText::putText(cv::Mat &frame, const char *text, cv::Point pos, cv::Scalar color) {


    if (frame.empty())
        return -1;
    if (text == nullptr)
        return -1;

    wchar_t *w_str ;
    int count = char2Wchar(text, w_str);
    //
    int i=0;
    for (; i<count; ++i) {
        wchar_t wc = w_str[i];
        //如果是ascii字符(范围0~127)，调整字体大小
        //因为ascii字符在同样的m_fontSize下更小，所以要放大一点
        if(wc<128)
            FT_Set_Pixel_Sizes(m_face, (FT_UInt)(m_fontSize.val[0]*1.15), 0);
        else
            FT_Set_Pixel_Sizes(m_face, (FT_UInt)m_fontSize.val[0], 0);
        // 输出当前的字符
        putWChar(frame, wc, pos, color);
    }
    delete(w_str);
    return i;
}

/**
 * 将char字符数组转换为wchar_t字符数组
 * @param src char字符数组
 * @param dst wchar_t字符数组
 * @param locale 语言环境，mbstowcs函数依赖此值来判断src的编码方式
 * @return 运行成功返回0,否则返回-1
 */
int CvText::char2Wchar(const char *&src, wchar_t *&dst, const char *locale)
{
    if (src == nullptr) {
        dst = nullptr;
        return 0;
    }

    // 设置C语言的字符集环境
    setlocale(LC_CTYPE, locale);

    // 得到转化为需要的宽字符大小
    int w_size = (int)mbstowcs(nullptr, src, 0) + 1;

    // w_size = 0 说明mbstowcs返回值为-1。即在运行过程中遇到了非法字符(很有可能是locale没有设置正确)
    if (w_size == 0) {
        dst = nullptr;
        return -1;
    }

    dst = new wchar_t[w_size];
    if (dst == nullptr) {
        return -1;
    }

    auto ret = (int)mbstowcs(dst, src, strlen(src)+1);
    if (ret <= 0) {
        return -1;
    }
    return ret;
}


// 输出当前字符, 更新m_pos位置
void CvText::putWChar(cv::Mat &frame, wchar_t wc, cv::Point &pos, cv::Scalar color) {
    // 根据unicode生成字体的二值位图  
    IplImage img = IplImage(frame);

    FT_UInt glyph_index = FT_Get_Char_Index(m_face, (FT_ULong)wc);
    FT_Load_Glyph(m_face, glyph_index, FT_LOAD_DEFAULT);
    FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_MONO);

    FT_GlyphSlot slot = m_face->glyph;

    // 行列数
    int rows = slot->bitmap.rows;
    int cols = slot->bitmap.width;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int off = ((img.origin == 0) ? i : (rows - 1 - i)) * slot->bitmap.pitch + j / 8;

            if (slot->bitmap.buffer[off] & (0xC0 >> (j % 8))) {
                int r = (img.origin == 0) ? pos.y - (rows - 1 - i) : pos.y + i;;
                int c = pos.x + j;

                if (r >= 0 && r < img.height
                    && c >= 0 && c < img.width) {
                    CvScalar scalar = cvGet2D(&img, r, c);

                    // 进行色彩融合
                    float p = m_fontDiaphaneity;
                    for (int k = 0; k < 4; ++k) {
                        scalar.val[k] = scalar.val[k] * (1 - p) + color.val[k] * p;
                    }
                    cvSet2D(&img, r, c, scalar);
                }
            }
        } // end for  
    } // end for  

    // 修改下一个字的输出位置
    double space = m_fontSize.val[0] * m_fontSize.val[1];
    double sep = m_fontSize.val[0] * m_fontSize.val[2];

    pos.x += (int) ((cols ? cols : space) + sep);
}
```

2. CMakeLists.txt

Linux系统一般已经配置了freetype库，我在此仅仅贴出和freetype库的CMakeLists.txt

```cpp
# required cmake version
cmake_minimum_required(VERSION 2.8)
# 项目名
project(MergePicture)
# 编译器
set(CMAKE_CXX_COMPILER g++)
# 编译类型
#set(CMAKE_BUILD_TYPE "Release")
# C++标准
set(CMAKE_CXX_FLAGS "-std=c++11")
# 指定opencv的路径
#set(OpenCV_DIR "/usr/local/include")
# 寻找OpenCV库
find_package(OpenCV REQUIRED)
#指定可执行文件的位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../run)
# 包含OpenCV头文件目录
include_directories(${OPENCV_INCLUDE_DIRS})
# 包含freetype的头文件目录
include_directories("/usr/local/include/freetype2")

file(GLOB CURRENT_HEADERS  include/*.* include/lib/*.h)
file(GLOB CURRENT_SOURCES  source/*.* *.cpp)

add_executable(${PROJECT_NAME} ${CURRENT_HEADERS} ${CURRENT_SOURCES})

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}  # 添加OpenCV库
        freetype        # 添加freetype库
        )
```





