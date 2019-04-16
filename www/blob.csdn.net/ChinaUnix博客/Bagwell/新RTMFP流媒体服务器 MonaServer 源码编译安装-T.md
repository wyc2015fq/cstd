# 新RTMFP流媒体服务器 MonaServer 源码编译安装-T-Bagwell-ChinaUnix博客






[新RTMFP流媒体服务器 MonaServer 源码编译安装](/uid-11344913-id-4976154.html)

****
分类： LINUX

2015-04-21 11:26:41






# MonaServer 源码编译安装



MonaServer 是Cumulus这个项目衍生出来的新项目，前身Cumulus在github中注明了Cumulus已经比较老，希望使用最新的RTMFP的server的话可以尝试MonaServer，跟着github中的引导，就到了MonaServer的项目目录中，看了看Readme之类的说明文档后，发现MonaServer所支持的功能还是比较多的：





			点击(此处)折叠或打开
		


- 1. RTMP     常见的流媒体直播，实时性相对来说还可以

- 
				2. RTMFP    常见的p2p的流媒体直播，实时性相对来说还可以

- 
				3. RTMPE    加密RTMP数据

- 
				4. WebSocket  HTML5最近比较火的基于HTTP的Socket通信

- 
				5. HTTP     HTTP传输的FLV直播，就是HDL或者HLV
			



MonaServer的编译要求相对来说比较简单，环境需求也比较简单，依赖包：




			点击(此处)折叠或打开
		


- 1. SSL库

- 
				2. LUA库
			



依赖环境：







			点击(此处)折叠或打开
		


- GCC 4.8.x +







CentOS
Linux 6.5 默认的编译器是4.4.7 编译的话会提示libstdc++支持的错误，所以需要升级GCC编译器。

我是用的源码编译的GCC，所以，可以再http:/gcc.gnu.org中找到对应的源码包，在gcc对应下载目录的里面可以找到gmp、mpfr、mpc的对应的文件，编译安装即可；

统一编译方法:





			点击(此处)折叠或打开
		


- ./configure –prefix=/usr –libdir=/usr/lib64;make;make install







接下来编译gcc





			点击(此处)折叠或打开
		


- mkdir build;../configure;make;make install







编译安装完GCC之后，在MonaServer目录中直接编译即可：





			点击(此处)折叠或打开
		


- make;make install







编译完成之后，会在MonaServer目录下面看到MonaServer子目录，里面生成了一个MonaServer应用程序，





			点击(此处)折叠或打开
		


- [root@www MonaServer]# pwd

- 
				/root/MonaServer/MonaServer

- [root@www MonaServer]# ls

- 
				logs  Makefile  MonaServer  MonaServer.vcxproj  MonaServer.vcxproj.filters  sources  tmp  www

- [root@www MonaServer]#






以及www目录，

需要注意的是，如果想使用rtmp，需要先在www目录下建立一个目录，以确保可以正常推流，如果想省略这一步，可以修改代码来完成灵活的支持。





			点击(此处)折叠或打开
		


- [root@www MonaServer]# mkdir www/myapp

- [root@www MonaServer]# ls www

- 
				myapp

- [root@www MonaServer]#






通过使用推流工具来进行测试，可以使用OBS或者FMLE：

![](http://blog.chinaunix.net/attachment/201504/21/11344913_1429586709Oq30.png)


然后点击Connect，及可看到MonaServer输出的log：





			点击(此处)折叠或打开
		


- [root@www MonaServer]#./MonaServer

- 
				Mona/Protocols.h[68] RTMFP server started on 0.0.0.0:1935 (UDP)

- 
				Mona/Protocols.h[68] RTMP server started on 0.0.0.0:1935 (TCP)

- 
				Mona/Protocols.h[68] HTTP server started on 0.0.0.0:80 (TCP)

- 
				Mona/Protocols.h[68] RTSP server started on 0.0.0.0:554 (TCP)

- 
				sources/Server.cpp[137] 1 clients
			







可以看到log中多出来一条sources/Server.cpp[137] 1
clients

这个时候点Start，可以看到输出的log：





			点击(此处)折叠或打开
		


- sources/FlashMainStream.cpp[162] Method 'releaseStream'not found on application /myapp

- 
				sources/FlashMainStream.cpp[162] Method 'FCPublish'not found on application /myapp

- 
				sources/Publication.cpp[97] Publication gosun started

- 
				sources/Publication.cpp[256] Write gosun publication properties

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1
			







这时说明流已经推至MonaServer了，可以通过播放器进行播放确认：
![](http://blog.chinaunix.net/attachment/201504/21/11344913_1429586699QZ1M.png)

当播放器连接上来时，会看到log如下：





			点击(此处)折叠或打开
		


- sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/FlashStream.cpp[209] setBufferTime 1000ms on stream 0

- 
				sources/Listener.cpp[211] Properties sent to one listener of gosun publicaition

- 
				sources/FlashStream.cpp[209] setBufferTime 1000ms on stream 1

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/Server.cpp[137] 2 clients

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/Listener.cpp[164] H264 codec infos sent to one listener of gosun publication

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1
			







RTMP的播放功能是支持了，那么RTMP转HTTP的功能如何，同时也可以验证：

![](http://blog.chinaunix.net/attachment/201504/21/11344913_1429586748tvt9.png)


HTTP的功能同样可以，看一下server部分的log：




			点击(此处)折叠或打开
		


- sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/Listener.cpp[211] Properties sent to one listener of gosun publicaition

- 
				sources/Writer.cpp[57] writeMedia method not supported by this protocol for 03 type

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/Server.cpp[137] 2 clients

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/Listener.cpp[164] H264 codec infos sent to one listener of gosun publication

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1

- 
				sources/FlashStream.cpp[195] Message 'onFI' unknown on stream 1
			






从此RTMP与HTTP的直播输出功能已经验证完毕，这是最基本的功能





