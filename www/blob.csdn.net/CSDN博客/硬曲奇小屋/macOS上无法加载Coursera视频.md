# macOS上无法加载Coursera视频 - 硬曲奇小屋 - CSDN博客





2017年12月29日 09:49:36[HardCookies](https://me.csdn.net/james_616)阅读数：388








由于某种限制，我们在Coursera上无法正常显示课程视频。 

下面给出在macOS上的解决方案：
- 在Finder中前往，输入/private/etc/，在打开的文件夹中找到hosts文件；
- 将其拖到桌面，因为在文件夹内不允许修改；
- 用文本编辑器打开，在最后一行加入52.84.246.72 d3c33hcgiwev3.cloudfront.net，保存；
- 拖回至原文件夹内，可正常显示。



