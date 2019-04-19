# 如何用libpng输出一个编辑后的png图片？ - Koma Hub - CSDN博客
2018年06月13日 21:16:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：185

## 如何用libpng输出一个png图片？
/* Copyright (c) 2013 Frank Ham and Phuc Quang */
#include<iostream>
#include<string>
#include<png.h>usingnamespacestd;
#ifndef PNG_WRITER_HPP
#definePNG_WRITER_HPP// Sample usage://    PngWriter png(nx,ny);//    // then one or more calls to...//    png.set(i, j, red, green, blue); // 0 <= red,green,blue <= 255//    png.write("myfile.png");//      at this point you can change the image and write again...//    png.set(i, j, red, green, blue);//    png.write("myfile2.png");classPngWriter {
private:unsignedchar (*buffer)[3]; // 0 <= r,g,b < 255 int nx,ny;
public:PngWriter(constint width,constint height) {
        nx = width;
        ny = height;
        buffer = newunsignedchar[nx*ny][3];
        for (int i = 0; i < nx*ny; ++i) {
            //cyan
            buffer[i][0] = 73;
            buffer[i][1] = 175;
            buffer[i][2] = 205;
        }
    }
    ~PngWriter() {
        delete[] buffer;
    }
    /*给某一点的像素赋值*/voidset(constint i,
           constint j,
           constunsignedchar r,
           constunsignedchar g,         
           constunsignedchar b) 
    {
        buffer[(ny-j-1)*nx+i][0] = r;
        buffer[(ny-j-1)*nx+i][1] = g;
        buffer[(ny-j-1)*nx+i][2] = b;
    }
    /*修复原作者的char **/voidwrite(string filename) {
        FILE * fp = fopen(filename.c_str(),"wb");
        if (!fp) {
            std::cout << "Warning: could not open png file: " 
                    << filename 
                    << std::endl;
            return;
        }
        
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                              (png_voidp)NULL,NULL,NULL);
        if (!png_ptr) {
            fclose(fp);
            std::cout << "Warning: could not create png_ptr" << std::endl;
            return;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
            std::cout << "Warning: could not create info_ptr" << std::endl;
            return;
        }
        png_init_io(png_ptr, fp);
        png_set_IHDR(png_ptr, info_ptr,
             nx, ny,             // width, height8,                  // bits per pixel -- 16 does not work with blockbuster
             PNG_COLOR_TYPE_RGB, // non-alpha options are PNG_COLOR_TYPE_RGB,PNG_COLOR_TYPE_GRAY,
             PNG_INTERLACE_NONE,
             PNG_COMPRESSION_TYPE_DEFAULT,
             PNG_FILTER_TYPE_DEFAULT);
        /* Some bits per pixel notes: 16 does not work with blockbuster, and there are also          issues with PNG_COLOR_TYPE_GRAY interpretation, so stick to 8 and PNG_COLOR_TYPE_RGB         for now. Note that if you do use 16, pay attention to MSB/LSB order. Endian is          flipped on my linux workstation... */png_write_info(png_ptr, info_ptr);
        
        // set up row pointers to point into the raw image data in buffer...
        
        png_byte * row_pointers[ny];
        for (int i = 0; i < ny; ++i)
          row_pointers[i] = (png_byte*)(buffer + i*nx);
        
        png_write_image(png_ptr, row_pointers);
        png_write_end(png_ptr, NULL);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
    }
};
#endif// g++ main.cpp -lpng intmain(int argc,char * argv[]) 
{
    PngWriter png(800,600);
    // set some pixels....for (int i = 0; i < 800; ++i)
        for (int j = 0; j < 600; ++j)
            png.set(i,j,i+j,i-j,j-i); 
    // then write...
    png.write("sample1.png");
    for (int i = 400; i < 500; ++i)
        for (int j = 200; j < 300; ++j)
            png.set(i,j,10,10,10);
    png.write("sample2.png");
}
## [](https://github.com/Rtoax/Knowledge/blob/master/libpng/libpng-writer.md#%E6%88%91%E7%9A%84%E7%8E%AF%E5%A2%83cygwing-gcc-730)我的环境：`cygwin`,`g++ (GCC) 7.3.0`
## [](https://github.com/Rtoax/Knowledge/blob/master/libpng/libpng-writer.md#%E7%BC%96%E8%AF%91%E8%BF%90%E8%A1%8C)编译,运行
$ g++ main.cpp -lpng
$ ./a.exe
## [](https://github.com/Rtoax/Knowledge/blob/master/libpng/libpng-writer.md#%E7%BB%93%E6%9E%9C)结果
![](https://img-blog.csdn.net/20180613211444556)
