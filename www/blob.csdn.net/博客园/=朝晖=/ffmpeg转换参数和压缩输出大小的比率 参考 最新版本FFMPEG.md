# ffmpeg转换参数和压缩输出大小的比率 参考 最新版本FFMPEG - =朝晖= - 博客园
# [ffmpeg转换参数和压缩输出大小的比率 参考 最新版本FFMPEG](https://www.cnblogs.com/dhcn/p/7130478.html)
https://blog.cnlabs[.NET](http://lib.csdn.net/base/dotnet)/3668.html
[ffmpeg](https://blog.cnlabs.net/tag/ffmpeg) 转换压缩比例
FFMPEG如果是压缩为FLV[文件](https://blog.cnlabs.net/tag/%e6%96%87%e4%bb%b6) 3个编码可选
1. -c:v flv 标准FLV编码 [这个](https://blog.cnlabs.net/tag/%e8%bf%99%e4%b8%aa)好处是[速度](https://blog.cnlabs.net/tag/%e9%80%9f%e5%ba%a6)快 清晰度高的话 视频文件会比较大
2. -c:v vp6 VP6编码 这个大家都很少[使用](https://blog.cnlabs.net/tag/%e4%bd%bf%e7%94%a8) 其实这个也算不错
3. -c:v [libx264](https://blog.cnlabs.net/tag/libx264) H.264编码 估计使用这个的比较多 优点是同等清晰度 视频文件更小 缺点就是转换慢的吐血
以1.11G大小的电影 转换为例子
libx264 500k 出片为 572.36M
flv编码 清晰度5 出片为 872.94M
2个看起来flv的清晰度 还不如libx264的
竟然大了足足300M……
音频都是一样 都是Pass 1
下面是以前的备注文档 贴出来共享下
壓縮配置選項
===============================
512Kbps
[視頻](https://blog.cnlabs.net/tag/%e8%a6%96%e9%a0%bb)bitrate 設置為 360k 最大416k 音頻設置為64k
1Mbps
視頻bitrate 設置為720k 最大832k 音頻設置為 [128k](https://blog.cnlabs.net/tag/128k)
2Mbps HD
視頻bitrate 設置為1550k 最大1792k 音頻設置為128k
視頻壓縮大小
===============================
出片 Bitrate 10分钟的视频
320p 180 [kbit](https://blog.cnlabs.net/tag/kbit)/s ~13 MB
360p 300 kbit/s ~22 MB
480p 500 kbit/s ~37 MB
576p 850 kbit/s ~63 MB (SD/PAL)
720p 1000 kbit/s ~75 MB
FFMPEG參數
===============================================
-i 輸入文件 test.avi 或其他
-c:v libx264 使用h.264 編碼
-vcodec libx264 強制指定視頻編碼[模式](https://blog.cnlabs.net/tag/%e6%a8%a1%e5%bc%8f)
-profile:v high 使用H.264的High模式 比較消耗資源
-pre slow 使用慢速模式 耗時間 清晰度高
//該參數還可選擇 ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow, placebo
-b:v 360k / 720k / 1550k 視頻比特率 (該參數比較重要 如果是轉換Web用途 是否正確直接影響視頻播放流暢程度)
-maxrate 500k 最大比特率
-bufsize 1000k 比特率緩衝大小
-s 000×000
視頻大小 建议值
240P 320×240 //Mobile iPhone MP4
360P 640×360 //SD FLV
480P 864×480 //HD MP4
720P 960×720 //HD MP4
-threads 0 處理器核心利用數量
-aspect 16:9 / 4:3 視頻比例
-pass N 1-3可選
音頻
-acodec libfaac 強制指定音頻處理模式
-ac 2 聲道選擇
-ar 44100 音頻赫茲
-ab 128k 比特率 64k/128k
-acodec libfaac -ac 2 -ar 44100 -ab 64k

