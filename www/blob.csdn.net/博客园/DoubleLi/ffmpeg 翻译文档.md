# ffmpeg 翻译文档 - DoubleLi - 博客园






# ffmpeg 翻译文档 (参考源文件[ffmpeg-all](http://xdsnet.github.io/other-doc-cn-ffmpeg/http/ffmpeg-doc/ffmpeg-all.html) 包含重要组件）

目录：
- [1 命令语法](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-01)
- [2 描概览](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-02)
- [3 详细说明](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-03)
- [4 流的选择（指定）](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-04)
- [5 选项](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-05)
- [技提示（原版已废弃）](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-06x)
- [6 例子](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-06)
- [7 语法](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-07)
- [8 表达式计求值](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-08)
- [9 OpenCL选项](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-09)
- [10 编码选项](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-10)
- [11 解码器](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-11)
- [12 视频解码](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-12)
- [13 音频解码](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-13)
- [14 字幕解码](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-14)
- [15 编码](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-15)
- [16 音频编码器](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-16)
- [17 视频编码器](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-17)
- [18 字幕编码器](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-18)
- [19 比特流滤镜（过滤器）](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-19)
- [20 格式选项](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-20)
- [21 分离器（解复用）](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-21)
- [22 混合器](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-22)
- [23 元数据](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-23)
- [24 协议](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-24)
- [25 设备选项](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-25)
- [26 输入设备](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-26)
- [27 输出设备](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-27)
- [28 重采样(resampler)选项](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-28)
- [29 放缩选项](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-29)
- [30 滤镜入门](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-30)
- [31 graph2dot](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-31)
- [32 滤镜链图描述](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-32)
- [33 时间线编辑](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-33)
- [34 音频滤镜](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-34)
- [35 音频源](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-35)
- [36 音频槽](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-36)
- [37 视频滤镜](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-37)
- [38 视频源](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-38)
- [39 视频槽](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-39)
- [40 多媒体滤镜](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-40)
- [41 多媒体源](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-41)
- [42 参考](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-42)
- [43 开发人员](http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html?name=ffmpeg-doc-cn-43)

from:http://xdsnet.github.io/other-doc-cn-ffmpeg/index.html









