# vs2013编译ffmpeg之二十九 tesseract - 枫林港的博客 - CSDN博客





2018年07月10日 23:25:09[枫林港](https://me.csdn.net/u013601132)阅读数：38标签：[tesseract																[ffmpeg																[vs2013																[编译](https://so.csdn.net/so/search/s.do?q=编译&t=blog)
个人分类：[vs2013编译ffmpeg](https://blog.csdn.net/u013601132/article/category/7785737)





# tesseract

对应ffmpeg configure选项–enable-libtesseract。

[https://github.com/tesseract-ocr](https://github.com/tesseract-ocr)

自动识别验证码，最初是google的开源下面，现在放在github上了。

tesseract 有对应的MSVC工程和编译方法：

[https://github.com/tesseract4java/tesseract-vs2013](https://github.com/tesseract4java/tesseract-vs2013)

[https://vorba.ch/2014/tesseract-3.03-vs2013.html](https://vorba.ch/2014/tesseract-3.03-vs2013.html)

相关资料：[http://blog.csdn.net/yasi_xi/article/details/8763385](http://blog.csdn.net/yasi_xi/article/details/8763385)

下面讲的方法是基于原始的[https://github.com/tesseract-ocr](https://github.com/tesseract-ocr)上的代码，用源代码里面的sln文件。这个方法可能有问题，因为有些图片解码库没有加进来。tesseract4java/ tesseract-vs2013里面的工程包含了leptonica、若干图片解码库、zlib，下面的方法只包含了zlib。tesseract-vs2013代码下载不了。

tesseract依赖于leptonica，其编译请参考本文下一章节的leptonica编译。

tesseract-3.04.01源代码里面有vs2010/ tesseract.sln，直接打开，两个工程libtesseract304、tesseract。
- 
设置一下leptonica头文件（allheaders.h）路径，不然编译会报错。

- 
去掉下图中的LIBLEPT_IMPORTS（这个会影响到environ.h里面的LEPT_DLL定义），不然链接会报错。 
![这里写图片描述](https://img-blog.csdn.net/20180707001944241?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 
libtesseract304工程里面，将生成dll改成lib。

- 
tesseract工程里面，加上leptonica的库。

- 
tesseract 工程里面去掉定义TESS_EXPORTS（这个会影响到ccutil/platform.h里面的TESS_API定义），不然链接的时候会报错。LIBLEPT_IMPORTS、TESS_EXPORTS只有在生成dll的时候才需要。


# leptonica

从[http://www.leptonica.org/download.html](http://www.leptonica.org/download.html)下载leptonica-1.73。产生sln文件： 

mkdir  vc2013_build;  cd vc2013_build;  cmake -G “Visual Studio 12 2013” ../ 

必须在某个子目录下执行cmake，在leptonica-1.73下执行会报错。 

修改vs2013_build\src\ config_auto.h，将：`/* #undef HAVE_LIBZ */`改成：`#define HAVE_LIBZ 1`

用MSVC2013打开sln文件后，修改leptonica工程属性。工程原本是生成dll的，改成lib（定义了HAVE_LIBZ，如果生成dll，则会链接zlib，调试也不方便），工程设置里面的绝对路径都改成相对路径。
## 编译错误一

```
1>  equationdetect.cpp

1>....\ccmain\equationdetect.cpp : warning C4819: The file contains a character that cannot be represented in the current code page (936). Save the file in Unicode format to prevent data loss

1>....\ccmain\equationdetect.cpp(253): error C2146: syntax error : missing '}' before identifier '銆'

1>....\ccmain\equationdetect.cpp(253): error C2146: syntax error : missing ';' before identifier '銆'
```

equationdetect.cpp最初的编码是UTF-8的，在“Solution Explorer”里面选中equationdetect.cpp，然后点菜单–>File–>Save equationdetect.cpp As…：

![这里写图片描述](https://img-blog.csdn.net/20180707002014896?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在弹出的对话框中，点save按钮后面的下拉箭头，选Save with Encoding…，

![这里写图片描述](https://img-blog.csdn.net/20180707002024344?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后选择“Chinese Simplified (GB2312) - Codepage 936”： 
![这里写图片描述](https://img-blog.csdn.net/20180707002137962?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 编译错误二

```
1>  baseapi.cpp

1>....\api\baseapi.cpp(1383): error C2057: expected constant expression

1>....\api\baseapi.cpp(1383): error C2466: cannot allocate an array of constant size 0

1>....\api\baseapi.cpp(1383): error C2133: 'id_buffer' : unknown size
```

解决办法：

[https://github.com/tesseract-ocr/tesseract/commit/809bbd9bfa0e3123ca81f2341fab37b130bac9d2](https://github.com/tesseract-ocr/tesseract/commit/809bbd9bfa0e3123ca81f2341fab37b130bac9d2)

即直接将bufsize固定成64。

## 编译错误三
`tesseract.lib(language_model.obj) : error LNK2005: _log2 已经在 mingwex.lib(log2.o) 中定义`
tesseract-3.04.01\wordrec\language_model.cpp里面的log2的定义注释掉。](https://so.csdn.net/so/search/s.do?q=vs2013&t=blog)](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)](https://so.csdn.net/so/search/s.do?q=tesseract&t=blog)




