# 【基于libRTMP的流媒体直播之 AAC、H264 解析】 - DoubleLi - 博客园






[前文](http://billhoo.blog.51cto.com/2337751/1557646)我们说到如何在基于 libRTMP 库的流媒体直播过程中推送 AAC 、H264 音视频流。本文以上文为基础，阐释如何对 RTMP 包进行解析、重组得到原始的 AAC 音频帧以及 H264 码流。

        在继续阅读本文之前，我们首先假设读者已经能够使用 libRTMP 库从 RTMP 直播服务器不断地获取 RTMP 包，如前提不成立，请自行阅读 [[抛开flash，自己开发实现C++ RTMP直播流播放器](http://www.cnblogs.com/haibindev/p/3466094.html)] 一文，实现一个简单的 RtmpDownloader 测试用例。这一部分恕 Bill 不再赘述。

        废话颇多，下面让我们一起来看看如何对 RTMP 包进行解析和重组。

        根据[前文](http://billhoo.blog.51cto.com/2337751/1557646)所述，我们在推送音、视频包之前，会首先向服务器推送一个音、视频同步包，该包包含了 AAC 音频帧以及 H264 码流的解码信息。因此当我们首次向服务器请求 RTMP 包之后，服务器会下发给我们对应的音、视频同步包，这将决定我们如何解析并重组音、视频。



RTMP 音频包的解析及 AAC 重组                 

        当我们得到服务器下发的第一个音频包（音频同步包）之后，即可按照[前文](http://billhoo.blog.51cto.com/2337751/1557646)所述的封包顺序进行逆操作，解析得到我们所需要的 AAC 音频信息，根据这些信息，我们便可重组在推送时被我们去掉的那 7 个（或 9 个）字节：ADTS（Audio Data Transport Stream），请参见维基百科 [[ADTS](http://wiki.multimedia.cx/index.php?title=ADTS)]。一旦 ADTS 构造完毕，直接在其后添加原始 AAC 音频数据即完成了整个 AAC 音频帧的重组。

        根据 ADTS 属性的描述，我们不难抽象出一个反映该信息的结构体如下：



![wKiom1QneDSCSvzZAAEoq6e7vv0953.jpg](http://s3.51cto.com/wyfs02/M00/4A/F1/wKiom1QneDSCSvzZAAEoq6e7vv0953.jpg)

        接着让我们回顾一下前文提到的音频同步包的结构：

![wKioL1QneQ7gzPzqAAKNXyQMvTU227.jpg](http://s3.51cto.com/wyfs02/M01/4A/F3/wKioL1QneQ7gzPzqAAKNXyQMvTU227.jpg)

        我们只需要从服务器获得的音频同步包中获取 AACDecoderSpecificInfo 以及 AudioSpecificConfig 这 4 个字节，并将其中的内容解析到 RtmpLiveAudioMetadataTy 这个结构中以备使用。示例代码如下：



![wKiom1QneyCy6Vu9AAMM-JEbp-c240.jpg](http://s3.51cto.com/wyfs02/M00/4A/F2/wKiom1QneyCy6Vu9AAMM-JEbp-c240.jpg)

        在获得了 AAC 音频所必要的信息之后，我们便可构造 ADTS 并重组 RTMP 服务器下发来的后续音频包。构造 ADTS 示例代码如下：



![wKiom1QnfOPwtSxZAAKoOmSuPco828.jpg](http://s3.51cto.com/wyfs02/M01/4A/F4/wKiom1QnfOPwtSxZAAKoOmSuPco828.jpg)



![wKiom1Qnfbvjs0rrAAGPacckG4E928.jpg](http://s3.51cto.com/wyfs02/M01/4A/F5/wKiom1Qnfbvjs0rrAAGPacckG4E928.jpg)

        至此，ADTS 7个字节便重构完毕，我们只需要在这 7 个字节后面添加上 AAC 的原始数据便可重组一个 AAC 音频帧。针对之后的每一个 RTMP 音频包，都进行上述步骤，不断将重组后的 AAC 音频帧写入文件，使用支持的播放器播放以验证解析正确与否。





RTMP 视频包的解析及 H.264 重组               

        与重组音频帧类似的，我们首先会从服务器获得视频同步包，首先对视频同步包进行拆包解析，得到本次 H264 码流的 Sps 以及 Pps，并从 Sps 中获取视频的宽高信息以备使用。根据前文提到的视频同步包封包流程，我们进行逆向解析的示例如下：



![wKiom1QngRKCQGkjAALprfsuRBc380.jpg](http://s3.51cto.com/wyfs02/M02/4A/F6/wKiom1QngRKCQGkjAALprfsuRBc380.jpg)

![wKiom1QngUPhgjl-AAGPsu_PbYs052.jpg](http://s3.51cto.com/wyfs02/M00/4A/F6/wKiom1QngUPhgjl-AAGPsu_PbYs052.jpg)

        其中从 Sps 获取视频的宽高信息请读者自行 Google，Bill 不再赘述。解析完视频同步包，我们只需要对接下来的视频包进行分类即可，针对 H264 P 帧，我们直接将原始数据写入文件，针对 I 帧，我们在其前面添加保存好的 Sps 以及 Pps，然后写入原始数据即可。针对之后从 RTMP 服务器接收的每一个视频包，均进行上述重组操作，并将获得的 H264 编码写入文件，使用 Elecard StreamEye Tools 播放即可。





参考博文

[1][[抛开flash，自己开发实现C++ RTMP直播流播放器](http://www.cnblogs.com/haibindev/p/3466094.html)]

[2][[使用 libRtmp 进行 H264 与 AAC 直播](http://www.codeman.net/2014/01/439.html)]

[3][[RTMP直播到FMS中的AAC音频直播](http://niulei20012001.blog.163.com/blog/static/7514721120130694144813/)]









