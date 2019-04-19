# bak 9.NDK 下面 编译 libpng.已经如何在opengl es 里面读取png图片. - xqhrs232的专栏 - CSDN博客
2011年04月14日 14:22:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2255标签：[pointers																[null																[struct																[image																[wildcard																[include](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=wildcard&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=pointers&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/untosil/archive/2010/07/16/5739002.aspx](http://blog.csdn.net/untosil/archive/2010/07/16/5739002.aspx)
手上有好些,前一阶段的工作成果.一直没有时间整理出来.
先把cola2d 里面使用的一些基础整理下.
在ndk 下如何读取png到opengl es 的texture了?
其实做法和linux下面没什么区别,我们首先要用到libpng这个开源的库.
从 [http://www.libpng.org/pub/png/libpng.html](http://www.libpng.org/pub/png/libpng.html) 下载到 libpng-1.2.41这个版本.
然后在android ndk 新建一个libpng的project .
我的android.mk 文件如下.
LOCAL_PATH := $(call my-dir)   
include $(CLEAR_VARS)   
LOCAL_MODULE := png  
LOCAL_CFLAGS := -I$(LOCAL_PATH)  /   
    -DPNG_USE_GLOBAL_ARRAYS /   
    -DPNG_READ_SUPPORTED   
LOCAL_CPP_EXTENSION := .cpp   
# Note this simple makefile var substitution, you can find even simpler examples in different Android projects  
LOCAL_SRC_FILES :=  $(notdir $(wildcard $(LOCAL_PATH)/*.c))   
include $(BUILD_STATIC_LIBRARY)  
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := png
LOCAL_CFLAGS := -I$(LOCAL_PATH)  /
 -DPNG_USE_GLOBAL_ARRAYS /
 -DPNG_READ_SUPPORTED
LOCAL_CPP_EXTENSION := .cpp
# Note this simple makefile var substitution, you can find even simpler examples in different Android projects
LOCAL_SRC_FILES :=  $(notdir $(wildcard $(LOCAL_PATH)/*.c))
include $(BUILD_STATIC_LIBRARY)
这样就可以编译好libpng.a
需要注意的是 如果使用该库,链接时,需要加上-lz 这儿需要带上zlib库 这个库,android ndk 已经自带了.不需要自行编译.
opengl es 如何使用该库读取一个png图片了?
读取png 代码:
unsigned char* Texture::ReadPNG (const char* file, png_uint_32* w, png_uint_32* h)  
{   
    FILE* f;   
    unsigned char sig[8];   
    png_structp png_ptr;   
    png_infop   info_ptr;   
    unsigned char* image_data;   
    int bit_depth;   
    int color_type;   
    unsigned int rowbytes;   
    png_uint_32 i;   
    png_bytepp row_pointers;   
    if ((f = fopen(file, "rb")) == NULL)   
        return NULL;   
    fread(sig, sizeof(*sig), sizeof(sig), f);   
    if (!png_check_sig(sig, sizeof(*sig))) {   
        fclose(f);   
        return NULL;   
    }   
    if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL) {  
        fclose(f);   
        return NULL;   
    }   
    if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {   
        png_destroy_read_struct(&png_ptr, NULL, NULL);   
        fclose(f);   
        return NULL;   
    }   
    if (setjmp(png_jmpbuf(png_ptr))) {   
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);   
        fclose(f);   
        return NULL;   
    }   
    png_ptr->io_ptr = (png_voidp) f;   
    png_set_sig_bytes(png_ptr, 8);   
    png_read_info(png_ptr, info_ptr);   
    png_get_IHDR(png_ptr, info_ptr, w, h, &bit_depth, &color_type, NULL, NULL, NULL);  
    if (color_type & PNG_COLOR_MASK_ALPHA)   
        png_set_strip_alpha(png_ptr);   
    if (bit_depth > <IMG class=wp-smiley alt=8) src="[http://www.adevgames.com/wordpress/wp-includes/images/smilies/icon_cool.gif](http://www.adevgames.com/wordpress/wp-includes/images/smilies/icon_cool.gif)"> 
        png_set_strip_16(png_ptr);   
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)  
        png_set_gray_to_rgb(png_ptr);   
    if (color_type == PNG_COLOR_TYPE_PALETTE)   
        png_set_palette_to_rgb(png_ptr);   
    png_read_update_info(png_ptr, info_ptr);   
    rowbytes = png_get_rowbytes(png_ptr, info_ptr);   
    if ((image_data =(unsigned char *) malloc(*h * rowbytes)) == NULL) {   
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);   
        fclose(f);   
        return NULL;   
    }   
    if ((row_pointers =(png_bytepp) malloc(*h * sizeof(png_bytep))) == NULL) {   
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);   
        fclose(f);   
        free(image_data);   
        return NULL;   
    }   
    for (i = 0; i < *h; i++)   
        row_pointers[*h - 1 - i] = image_data + i*rowbytes;   
    png_read_image(png_ptr, row_pointers);   
    free(row_pointers);   
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);   
    fclose(f);   
    return image_data;   
}  
unsigned char* Texture::ReadPNG (const char* file, png_uint_32* w, png_uint_32* h)
{
 FILE* f;
 unsigned char sig[8];
 png_structp png_ptr;
 png_infop   info_ptr;
 unsigned char* image_data;
 int bit_depth;
 int color_type;
 unsigned int rowbytes;
 png_uint_32 i;
 png_bytepp row_pointers;
 if ((f = fopen(file, "rb")) == NULL)
  return NULL;
 fread(sig, sizeof(*sig), sizeof(sig), f);
 if (!png_check_sig(sig, sizeof(*sig))) {
  fclose(f);
  return NULL;
 }
 if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL) {
  fclose(f);
  return NULL;
 }
 if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {
  png_destroy_read_struct(&png_ptr, NULL, NULL);
  fclose(f);
  return NULL;
 }
 if (setjmp(png_jmpbuf(png_ptr))) {
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  fclose(f);
  return NULL;
 }
 png_ptr->io_ptr = (png_voidp) f;
 png_set_sig_bytes(png_ptr, 8);
 png_read_info(png_ptr, info_ptr);
 png_get_IHDR(png_ptr, info_ptr, w, h, &bit_depth, &color_type, NULL, NULL, NULL);
 if (color_type & PNG_COLOR_MASK_ALPHA)
  png_set_strip_alpha(png_ptr);
 if (bit_depth > 
  png_set_strip_16(png_ptr);
 if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
  png_set_gray_to_rgb(png_ptr);
 if (color_type == PNG_COLOR_TYPE_PALETTE)
  png_set_palette_to_rgb(png_ptr);
 png_read_update_info(png_ptr, info_ptr);
 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
 if ((image_data =(unsigned char *) malloc(*h * rowbytes)) == NULL) {
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  fclose(f);
  return NULL;
 }
 if ((row_pointers =(png_bytepp) malloc(*h * sizeof(png_bytep))) == NULL) {
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
  fclose(f);
  free(image_data);
  return NULL;
 }
 for (i = 0; i < *h; i++)
  row_pointers[*h - 1 - i] = image_data + i*rowbytes;
 png_read_image(png_ptr, row_pointers);
 free(row_pointers);
 png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
 fclose(f);
 return image_data;
}然后使用
下面方法即可读入到texture里面
m_width=imgw;   
    m_height=imgh;   
      glGenTextures(1, &m_id);   
        glBindTexture(GL_TEXTURE_2D, m_id);   
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgw, imgh, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);  
        GLint crop[4] = { 0, 0, m_width, m_height };   
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, crop);   
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   
    //glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );  
m_width=imgw;
   m_height=imgh;
   glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgw, imgh, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  GLint crop[4] = { 0, 0, m_width, m_height };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, crop);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
这部分内容和普通opengl es 的开发都没有区别.
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/untosil/archive/2010/07/16/5739002.aspx](http://blog.csdn.net/untosil/archive/2010/07/16/5739002.aspx)

