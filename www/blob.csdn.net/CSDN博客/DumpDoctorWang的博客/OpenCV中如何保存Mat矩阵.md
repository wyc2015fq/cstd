# OpenCV中如何保存Mat矩阵 - DumpDoctorWang的博客 - CSDN博客





2018年03月20日 21:41:41[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：488








# 一、缘起

在有的项目中，需要保存对比实验的结果(类型为Mat)。主流的有两种方案，一是imwrite，二是使用FileStorage([参考这里](http://blog.csdn.net/jianjian1992/article/details/49254667))。前一种方法大家都很熟悉，然而并不能保存数据类型为除CV_8UC1和CV_8UC3之外的其他类型；第二种使用FileStorage呢，需要使用FileStorge<<"mat_name"<<mat这种形式，非要指定mat的名字"mat_name"，占用的存储空间还比较大，让我非常不爽，这在有些情况下，也造成了不便。于是产生了自己写一对save和read函数来读写mat的冲动。

# 二、方案思路

首先确定是使用二进制文件存储Mat。

save函数的思路：第一步是向二进制文件中写入两个字节"mb"(mat binary的缩写)作为是使用save函数保存的Mat的标志;第二步，写入Mat.rows、Mat.cols、Mat.type()组成的信息头，以便在read函数中根据这三个数字来恢复mat;第三步，根据Mat.rows*Mat.rows*Mat.elemSIze()计算出mat中数据的长度，写入二进制文件。至此保存Mat完成。

read函数的思路：第一步，从文件中读取两个字节，判断是等于"mb";若是，第二步，从文件中读取出rows、cols、type这三个数据，根据这三个数据来初始化一个mat;第三步，根据mat.rows*mat.rows*mat.elemSIze()计算出数据的长度，从二进制文件中读入到mat.data指定的内存块。

本文代码github地址：https://github.com/Mannix1994/OpenCVUtils，如果觉得有用，麻烦star一下哦

具体实现：

```cpp
//tools.h
#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <string>

/**
 * condition为false时抛出错误，错误信息为error_message
 */
#define ASSERT(condition,error_message) ((condition)?0:assertion(__FILE__, __func__, __LINE__,error_message))

inline int assertion(const std::string &filePath, const std::string &function,
                     int line, const std::string &info){
    //获取文件名
    unsigned long pos=filePath.find_last_of('/');
    std::string filename(filePath.substr(pos+1));
    std::string err = filename+" "+function+" "+std::to_string(line)+">>  "+info;
    //抛出错误
    throw std::runtime_error(err);
}

#endif //TOOLS_H
```

```cpp
//MatIO.h
#ifndef MATIO_H
#define MATIO_H

#include <opencv2/opencv.hpp>

namespace Utils {
    /**
     * 以二进制文件来存储Mat数据；读取这个函数存储的文件，需要用到下面的read函数。
     * @param fileName 文件名(后缀推荐为.mb)
     * @param src 要存储的mat
     */
    void write(std::string fileName, cv::Mat src);

    /**
     * 读取save函数保存的Mat
     * @param fileName 文件名
     * @return 返回读取到的mat
     */
    cv::Mat read(std::string fileName);

}

#endif //MATIO_H
```



```cpp
//MatIO.cpp
#include "MatIO.h"
#include <fstream>
#include "tools.h"

using namespace std;
using namespace cv;

namespace Utils {
    /**
     * 保存mat头的关键信息
     */
    typedef struct {
        int rows;//行数
        int cols;//列数
        int type;//类型
    } MatHeader;
}

void Utils::write(std::string fileName, cv::Mat src) {
    ASSERT(!fileName.empty(), "文件名为空");
    ASSERT(!src.empty(), "mat为空");
    MatHeader matHeader{src.rows, src.cols, src.type()};
//    printf("%d %d %d\n",matHeader.rows,matHeader.cols,matHeader.type);

    //打开文件
    ofstream out(fileName, ios::binary);
    ASSERT(out.is_open(), "打开文件" + fileName + "失败");

    //写入文件类型，表示为这个函数保存的Mat，长度为两字节
    char fileType[3] = "mb";
    out.write(fileType, 2*sizeof(char));

    //写入Mat头
    out.write((char *) &matHeader, sizeof(MatHeader));

    //写入数据
    //http://blog.csdn.net/dcrmg/article/details/52294259
    out.write((char *) src.data, src.rows * src.cols * src.elemSize());

    out.flush();
    out.close();
}

cv::Mat Utils::read(std::string fileName) {
    //打开文件
    ifstream in(fileName, ios::binary);
    ASSERT(in.is_open(), "打开文件" + fileName + "失败");

    char fileType[3]={'\0','\0','\0'};//初始化一个默认值

    //读取前两字节
    in.read(fileType, 2*sizeof(char));

    //判断是否是"mb"这种文件
    ASSERT(strcmp(fileType,"mb") == 0, fileName + "非Mat类型的数据");

    //读取Mat头
    MatHeader matHeader{0, 0, 0};
    in.read((char *) &matHeader, sizeof(MatHeader));
//    printf("%d %d %d\n",matHeader.rows,matHeader.cols,matHeader.type);

    //初始化一个Mat
    Mat mat(matHeader.rows, matHeader.cols, matHeader.type);

    //写入数据
    in.read((char *) mat.data, mat.rows * mat.cols * mat.elemSize());

    in.close();
    return mat;
}
```

### 调用方法：



```cpp
#include "MatIO.h"
using namespace Utils;

void fun(){
    Mat mat = ...;
    //保存
    write("test.mb",mat);
    //读取
    Mat m = read("test.mb");
}
```



