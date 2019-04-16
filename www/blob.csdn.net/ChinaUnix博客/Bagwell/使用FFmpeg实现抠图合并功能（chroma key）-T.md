# 使用FFmpeg实现抠图合并功能（chroma key）-T-Bagwell-ChinaUnix博客




[使用FFmpeg实现抠图合并功能（chroma key）](/uid-11344913-id-5208801.html)

****
分类： 其他平台

2015-09-29 11:22:53







	在很多视频中可以看到图像是合成的，例如有些神剧里面某大侠跳下万丈深渊的场景，某人在三昧真火中被烧的场景，还有些游戏主播，体育主播在主场景前面有个人解说的场景，都是chroma key的技术实现的，具体的链接可以参考：


[](http://www.cs.utah.edu/~michael/chroma/)[http://www.cs.utah.edu/~michael/chroma/](http://www.cs.utah.edu/~michael/chroma/)





	效果图如下：


![](http://blog.chinaunix.net/attachment/201509/29/11344913_144349691608c5.png)



	在最新版本的ffmpeg中，已经增加了chroma key功能的filter，只需要一条命令即可搞定







				点击(此处)折叠或打开
			


- ./ffmpeg -i ~/fuck.mp4 -i ~/1.mp4 -shortest -filter_complex "[1:v]chromakey=0x70de77:0.1:0.2[ckout];[0:v][ckout]overlay[out]"-map "[out]" output.mp4







	将1.mp4中的人物抠出来，合并到fuck.mp4中，就可以搞定了



	合并之前的1.mp4的效果


![](http://blog.chinaunix.net/attachment/201509/29/11344913_1443496938vjc6.jpg)



	执行命令行如下：


![](http://blog.chinaunix.net/attachment/201509/29/11344913_14434969498U8t.png)



	与fuck合并后的效果


![](http://blog.chinaunix.net/attachment/201509/29/11344913_1443496959XKcK.png)






