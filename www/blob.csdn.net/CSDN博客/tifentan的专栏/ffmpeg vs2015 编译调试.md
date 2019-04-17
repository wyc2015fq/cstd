# ffmpeg vs2015 编译调试 - tifentan的专栏 - CSDN博客





2018年03月07日 20:37:52[露蛇](https://me.csdn.net/tifentan)阅读数：1114








一直想用vs的强大调试能力去研究ffmpeg，其实很久之前就有留意在github上有个类似的项目，现在做一遍记录下来。

## 项目位置

[https://github.com/ShiftMediaProject/FFmpeg](https://github.com/ShiftMediaProject/FFmpeg)

装一个git，配置好后拉代码（使用git拉别直接下载，下面会快点） 

建议搞两层目录，不然很头疼，例如 shiftmedia/source/，在这目录下执行：
```php
git clone https://github.com/ShiftMediaProject/FFmpeg
```

## 准备

所有的项目文件都在SMP文件夹下，查看这目录下的readme文件，介绍了如何build。 

1.必须支持c99，也就是vs2013版本以上才支持

## 下载依赖包

执行SMP下的project_get_dependencies.bat 

注：这脚本需要git依赖，你的项目如果**不是git拉下来的话只能手动下载**了。 
![这里写图片描述](https://img-blog.csdn.net/20180307165654862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下载完你会发现一大堆依赖库的vs项目，幸福来得太突然，像x264,x265这些也能调试了吗？
## nasm安装

下载很慢，先装一下nasm，[https://github.com/ShiftMediaProject/VSNASM/releases](https://github.com/ShiftMediaProject/VSNASM/releases)

按readme说法，运行一下安装脚本*install_script.bat*搞定。

## 编译

漫长的等待下载完成后，目录如下： 
![这里写图片描述](https://img-blog.csdn.net/20180307203453741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

SMP目录下打开ffmpeg_deps.sln，选择一个依赖库先生成，例如libass，目的是先生成目标目录。 

然后到这里[https://www.khronos.org/registry/OpenGL/api/GL/](https://www.khronos.org/registry/OpenGL/api/GL/)下载glext.h跟wglext.h，放置到目标目录msvc/include/gl/下。 

然后选个exe的启动项目就可以玩了。。。。。 
![这里写图片描述](https://img-blog.csdn.net/20180307203627278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

调试试试？ 
![这里写图片描述](https://img-blog.csdn.net/20180307204728420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 配置ffmpeg参数

需要重新生成一下vs项目文件。 

他那有个工具提供这功能：[https://github.com/ShiftMediaProject/FFVS-Project-Generator](https://github.com/ShiftMediaProject/FFVS-Project-Generator)

同理应该也可以配置其他的依赖项目的。






