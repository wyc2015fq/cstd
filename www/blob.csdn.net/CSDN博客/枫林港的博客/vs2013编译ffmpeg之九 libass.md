# vs2013编译ffmpeg之九 libass - 枫林港的博客 - CSDN博客





2018年07月07日 22:49:03[枫林港](https://me.csdn.net/u013601132)阅读数：174








# vs2013编译libass

ShiftMediaProject下载最新的[libass](https://github.com/ShiftMediaProject/libass)，对应于官方版本0.13.7。 

VS2013打开libass-master\SMP\libass.sln，如果遇到libass.vcxproj加载失败，则编辑libass.vcxproj，将里面的yasm.props改成vsyasm.props，yasm.targets改成vsyasm.targets，然后重新打开libass.sln就ok了。yasm在编译其他第三方库的时候已经准备好了的，请参考这里。

由于libass依赖很多第三方软件（ShiftMediaProject的libass依赖于freetype-2.5.3、fribidi、fontconfig、libiconv-1.9.2、HarfBuzz），而ShiftMediaProject里面把这些都加进来了，有其特定的目录安排，但这里没有按其方法来，第三方软件的头文件，都是按自己的路径来，所以头文件路径需要修改。

此次没有编译HarfBuzz，直接在config.h里面修改，注释掉CONFIG_HARFBUZZ的定义：

```bash
/* found harfbuzz-ng via pkg-config 
#define CONFIG_HARFBUZZ 1*/
```

Project properties->Configuration properties->C/C++->Code generation->Runtime Library改成Mtd，和ffmpeg保持一致。

编译32位版本的时候，asm文件的编译，下面的错误：

```
1>..\libass\x86\x86inc.asm(47): error : undefined symbol `ARCH_X86_64' in preprocessor
1>..\libass\x86\rasterizer.asm(854): error : (FILL_GENERIC_TILE:52) undefined symbol `HAVE_ALIGNED_STACK' in preprocessor
1>..\libass\x86\rasterizer.asm(854): error : (GET_RES_ADDR:1) undefined symbol `HAVE_ALIGNED_STACK' in preprocessor
```

解决办法，例如编译debug版本的： 
![这里写图片描述](https://img-blog.csdn.net/20180702233028297?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

工程属性->Yasm Assembler->Symbols里面加上下面的定义：
```
WIN32=1
ARCH_X86_64=0
HAVE_ALIGNED_STACK=0
```

同时工具栏选择Debug、x86： 
![这里写图片描述](https://img-blog.csdn.net/20180702235149887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这个libass的工程属性里面有很多配置，包括Debug、release、x86、x64等，编译的时候要注意选择，避免在工程属性里面修改的和编译的时候用的不是同一套属性。如果要编译64位版本的，ARCH_X86_64要定义成1，64位版本的未测试。 

编译成功后，ffmpeg下面的pc_files\libass.pc需要修改里面的路径，然后再重新configure并编译ffmpeg。




