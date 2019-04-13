
# PJSIP 2.7.2 增加视频支持 - 老雍的博客 - CSDN博客


2018年07月31日 11:18:31[老雍](https://me.csdn.net/yongkai0214)阅读数：659



# 添加步骤
1. 在头文件 pj/config_site.h 增加 宏定义支持
```python
#define PJMEDIA_HAS_VIDEO 1
#define PJMEDIA_VIDEO_DEV_HAS_SDL 1
#define PJMEDIA_VIDEO_DEV_HAS_DSHOW 1
#define PJMEDIA_HAS_FFMPEG 1
```
2. 在工程 pjmedia, pjmedia_codec 增加ffmpeg 支持，加入头文件和库目录
3. 在工程pjmedia_videodev 增加 SDL支持，加入头文件和库目录
4. 在工程pjsua增加ffmpeg，SDL支持，加入头文件和库目录
5. 将ffmpeg, SDL 的dll文件拷贝到 ../pjsip-apps/bin中
6. 检查每个工程的是否可以正确编译，检查pjsip-apps能否生成可执行文件
7. 在命令行界面出现以后，输入：
```python
vid enable
vid acc autotx on
vid acc autorx on
```
# 问题解决：
**1. 链接器工具错误 LNK2026 XXX模块对于SAFESEH 映像是不安全的"**
解决方法：
> 打开该项目的“属性页”对话框。

> 单击“链接器”文件夹。

> 单击“命令行”属性页。

> 将 /SAFESEH:NO 键入“附加选项”框中，然后点击应用。
**2. ubuntu环境下 make dep 时的问题：**
`fatal error: alsa/asoundlib.h: No such file or directory #include <alsa/asoundlib.h>`解决办法：
`sudo apt-get install libasound2-dev`


