# codeBlocks安装小注意 - wjheha的博客 - CSDN博客
2017年09月23日 20:21:32[wjheha](https://me.csdn.net/wjheha)阅读数：1071
  进行C/C++开发，codeBlocks是一款不错的轻量级IDE。 
codeBlocks下载官网：[http://www.codeblocks.org/](http://www.codeblocks.org/)，下载的时候会有自带编译器和不带编译器的安装包，自己按需求下载，自带的编译器是MinGW,编译完的程序是面对32位系统的，64位机当然也可以运行。 
MinGW:[http://www.mingw.org/](http://www.mingw.org/)
  如果下载的是不带编译器的，那么可以去下载Mingw-w64：[http://www.mingw-w64.org/doku.php](http://www.mingw-w64.org/doku.php)，编译完的程序是面对64位系统的。 
  安装完codeBlocks和相应的编译器后，可以设置使用哪一个编译器，设置步骤：
1，打开菜单栏中settings下的compiler 
2，在selectd compiler中选择 GNU GCC Compiler 
3，然后在下面的菜单选项中选择Toolchain executables 
4，点击浏览，在地址框中选择编译器的安装目录 
***设置完成后，可能还会出现编译器不可用，这个时候可能需要手动设置一下，见下图：***
![这里写图片描述](https://img-blog.csdn.net/20170923202954764?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
主要是设置下面那个框里面的内容： 
C compiler、C++ compiler、linker for dynamic libs的内容。好了，现在应该可以正常使用了
