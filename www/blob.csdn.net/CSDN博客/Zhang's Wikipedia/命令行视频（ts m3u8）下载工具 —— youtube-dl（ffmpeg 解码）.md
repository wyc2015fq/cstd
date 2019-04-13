
# 命令行视频（ts-m3u8）下载工具 —— youtube-dl（ffmpeg 解码） - Zhang's Wikipedia - CSDN博客


2018年04月05日 23:08:19[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：2683


youtube-dl 支持的站点：[youtube-dl  Supported sites](https://rg3.github.io/youtube-dl/supportedsites.html)
youtube-dl 命令行参数：
–version：查看版本；
1. 命令行工具安装
安装视频编解码工具 ffmpeg：[Download FFmpeg](https://ffmpeg.org/download.html)
免安装，解压即可，并将 ffmpeg 下的 bin 路径添加进环境变量里；
安装 youtube-dl：[youtube-dl downloads](https://www.youtube-dl.org/)
将下载的 youtube-dl.exe 拷贝至 ffmpeg 的 bin 路径下，这样 windows 命令行便能识别 youtube-dl 命令了；
2. ts/m3u8 视频下载
打开浏览器，找到 ts 分片（或m3u8）片源url，如[万门 | 课程详情](https://www.wanmen.org/courses/586d23485f07127674135d07/lectures/586d23535f071276741585d6)，
进入 chrome 的开发者工具，点开 Network 选项卡，如下图所示，获取 ts 分片（一般片长为 10s） url：
![这里写图片描述](https://img-blog.csdn.net/20180405230411649?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[](https://img-blog.csdn.net/20180405230411649?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
进入 windows cmd 命令行进行下载：
>youtube-dl"https://media.wanmen.org/d20ed477-b551-4f29-8563-9b235612e206_pc_high00000.ts"

