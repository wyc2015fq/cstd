# FFmpeg X264  H264编码指南[译] - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月17日 15:10:28[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：268

本文目标：如何创建一个高质量的H.264视频
x264 是一个 H.264 编码器.
通常有2种码率控制（rate control）模式：Constant Rate Factor (CRF) or Two-Pass ABR
码率控制是一种决定为每一帧分配多少比特数bits的方法，它将决定文件的大小和质量的分配how
 quality is distributed。
## CRF恒定比率因子
目的是为了使整个文件达到特定的输出质量，而不关心文件大小；此方法在1pass时就可以提供最大的压缩效率。每一帧都得到了其需要的比特数，缺点是：不能得到一个特定大小的文件或者难以掌控比特率。
### 选择一个CRF值
量化规模quantizer scale的范围为 0-51（ 针对8-bit），0 为无损，默认23，建议在18-28之间，18往往被认为从视觉上看是无损的。
CRF值每加6，bitrate码率大概减少一半；每次减6则会使码率增加一倍。
常用做法是：在质量可接受的情况下选择一个最大的CRF值，如果看起来不错，就尝试一个更大的值，如果看起来很糟糕，就降低CRF值。
### 选择一个预设preset
预设是一系列选项的集合，能够在编码速度和压缩率之间做出一个权衡，一个编码速度稍慢的预设会提供更高的压缩效率（压缩效率是以文件大小来衡量的)。
假如你想得到一个指定大小的文件或者想得到恒定的比特率，你可以采用一个较慢的预设值来获得更好的质量。
同样的，对于恒定质量编码模式，你可以通过选择一个较慢的预设轻松地节省比特率。
通常的建议是使用最慢的预设。
预设值按编码速度从大到小的顺序为：
ultrafast,superfast, veryfast, faster, fast, medium, slow, slower, veryslow, placebo
默认为medium(一般不用placebo)。
你也可以根据输入文件的特性选择-tune选项，其值如下：
film,animation, grain, stillimage, psnr, ssim, fastdecode, zerolatency
如果你的输入文件是动画，则可以用animation。如果你想保留纹理，可以使用grain，如果不确定可以不设置。
另一个可选的参数是-profile:v，可以将输出限定到一个特定的H.264 profile,通常不用，除非目标设备仅支持某种profile.有如下可选值：
baseline, main, high, high10, high422, high444
注意-profile:v 与无损编码不兼容。
To list all possible internal preset and tunes:
ffmpeg -f lavfi -i nullsrc -c:v libx264 -preset help -f mp4 -
示例：
ffmpeg -i input -c:v libx264 -preset slow -crf 22 -c:a copy output.mkv
## Two-Pass
如果你的目标是一个确定大小的文件而且帧与帧之间的视频质量并不重要，这个方法很适用。
例如：你的视频时长 10 minutes (600 seconds) 想要输出一个 50 MB大小的文件. 因为比特率= 文件大小/ 时长:
(50 MB * 1024*8 [converts MB to kilobits]) / 600 seconds = ~683 kilobits/s （全局比特率）
683k - 128k (期望的音频比特率) = 555k （视频比特率）
从而命令如下：
ffmpeg -y -i input -c:v libx264 -preset medium -b:v 555k -pass 1 -an -f mp4 /dev/null && \
ffmpeg -i input -c:v libx264 -preset medium -b:v 555k -pass 2 -c:a libfdk_aac -b:a 128k output.mp4
其中-an表示禁用audio，windows下请将/dev/null改为NUL
## 无损H.264编码
可通过-qp 0 or -crf 0实现，建议用-qp选项。因为8 bitx264和10 bitx264中的 crf 针对无损模式使用了不同的值。可同时配合2个有用的预设ultrafast 或 veryslow。
注意很多不基于FFmpeg的播放器可能不能解码无损压缩的视频。如果不兼容最好不要用无损编码。
示例一：（快速编码）
ffmpeg -i input -c:v libx264 -preset ultrafast -qp 0 output.mkv
示例二：（高压缩比）
ffmpeg -i input -c:v libx264 -preset veryslow -qp 0 output.mkv
## 覆盖默认preset值
可通过x264opts 选项或x264-params，或libx264 私有选项，（参见ffmpeg -h encoder=libx264）。
一般不建议使用。
示例：
ffmpeg -i input -c:v libx264 -preset slow -crf 22 -x264opts keyint=123:min-keyint=20 -c:a copy output.mkv
用MediaInfo 艺搜下载 打开可以看到：
## 平均比特率; ABR (Average Bit Rate)
ffmpeg -i input -c:v libx264 -b:v 1000k output.mp4
对比较简单的画面，将用小于设定的比特率去编码，而对于复杂的画面，将进行高质量的编码，从而在整体上达到指定的码率，和2-pass一起用效果更好，同时也可以指定max
 bit rate防止码率产生大的波动。
## 常量比特率CBR (Constant Bit Rate)
没有原生的CBR模式，但可通过调整ABR参数进行模拟。
ffmpeg -i input -c:v libx264 -b:v 4000k -minrate 4000k -maxrate 4000k -bufsize 1835k out.m2v
-bufsize是一个“码率控制缓冲区”,它会在每一个有用的1835k视频数据内强制一个你所要求的均值（此处为4000k） 可以同时指定crf和max bit rate
ffmpeg -i input -c:v libx264 -crf 20 -maxrate 400k -bufsize 1835k output.mp4
这会有效地接近crf 20，但当输出超过400kb/s时，会降低crf的值。
通过-tune zerolatency可以达到低延迟Latency
## 视频兼容性
### 所有设备
如果想要最大兼容(比如老版ios或所有Android设备)
可以指定
-profile:v baseline -level 3.0
这将会关闭很多高级特性。
### ios
|Profile|Level|Devices|Options|
|----|----|----|----|
|Baseline|3.0|All devices|-profile:v baseline -level 3.0|
|Baseline|3.1|iPhone 3G and later, iPod touch 2nd generation and later|-profile:v baseline -level 3.1|
|Main|3.1|iPad (all versions), Apple TV 2 and later, iPhone 4 and later|-profile:v main -level 3.1|
|Main|4.0|Apple TV 3 and later, iPad 2 and later, iPhone 4S and later|-profile:v main -level 4.0|
|High|4.0|Apple TV 3 and later, iPad 2 and later, iPhone 4S and later|-profile:v high -level 4.0|
|High|4.1|iPad 2 and later and iPhone 4S and later|-profile:v high -level 4.1|
### Apple Quicktime
Apple Quicktime 仅支持采用 YUV 420 颜色空间编码（-pix_fmt yuv420p）的 H.264 影片,不支持高于main的profile.所以可以在baseline和main之间选择一个。
### Blu-ray
参见 [http://www.x264bluray.com/](http://www.x264bluray.com/)
### 网络视频
可以在输出选项中加上-movflags +faststart，可以在全部下载完成之前就可以开始播放。
## 艺搜参考
[](http://trac.ffmpeg.org/wiki/x264EncodingGuide)
[https://trac.ffmpeg.org/wiki/Encode/H.264](https://trac.ffmpeg.org/wiki/Encode/H.264)
