# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之69---面向FLASH的开发 - 我相信...... - CSDN博客





2010年08月26日 10:04:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：863标签：[brew																[flash																[actionscript																[http服务器																[xml																[手机](https://so.csdn.net/so/search/s.do?q=手机&t=blog)
个人分类：[消失的技术](https://blog.csdn.net/wireless_com/article/category/597607)





Flash Lite可以为用户提供丰富的用户体验，而且全球有超过6500万的支持Flash的设备。通过Flash可以使内容制作者访问设备的特殊属性，Adobe 公司提供了面向Flash Lite的BREW扩展类，能够支持同计算机上Flash Player 7所播放的一样的SWF 文件格式，能够使面向Flash的内容和应用在支持BREW的手机上运行。该扩展类基于Flash Lite 2.x.，能支持BREW 2.1.3 及以上版本的手机。同时，在BREW Mobile Platform上集成了不同层次的Flash功能。

基于BREW的Flash Lite 由三部分组成（图14-8），专业的制作工具可以通过后处理的方式将内容生成BREW手机支持的*.mod 文件，放映工具可以将相关的信息打包成*.mod 文件和 *.MIF 文件，以及一个BREW的扩展类。





    基于BREW的Flash Lite 能够播放SWF 内容，显示BMP，JPEG，PNG 和其他手机支持的图像格式，支持多种字体甚至矢量字库。同时，能够处理和执行ActionScript (基于ECMA-262 标准的脚本语言，与Java Script类似)，从而可以播放动画，并支持各种色深的调色板 (例如 8 bit 调色板，RGB 444, RGB 555, RGB 565, RGB 666, RGB888 等等)，灵活的提供不同质量的显示能力。同时，可以加载和播放各种的音频文件，例如MIDI, SMAF, AAC, AACPlus等等，还可以播放Flash的视频文件。基于BREW的Flash Lite可以在不同的SWF中共享4KB大小的数据，所有的共享对象空间最大累计可达256KB。

基于BREW的Flash Lite 支持在内容播放过程中的挂起和恢复，支持内容的缩放，能够在MBCS 和UTF16的设备上运行，可以支持全屏或局部显示。同时，提供了按键和其他定制化的事件处理，允许通过网络通信连接任意的HTTP服务器并接收流式数据，通过ActionScript 可以实现SWF文件的嵌套，加载文本数据，XML数据等。更重要的是，实现了socket通信的能力，能够发送和下载XML 数据。在面向TCP 的socket 连接中发送XML 消息，每个XML 消息都是一个完整的XML 文档，通过一个XMLSocket 可以收发任意多的XML 消息。

基于BREW的Flash Lite的安全性是通过域来限定的，例如如果根上访问的URL是[http://www.adobe.com](http://www.adobe.com/)的话，那么就不允许制作者从其他的[http://www.xyz.com](http://www.xyz.com/)站点下载数据。

从BREW3.X 开始，Flash Lite开始直接与部分手机集成在一起，并提供了各种与时间相关的函数：“GetDateDay”, “GetDateMonth”, “GetLocaleLongDate”, “GetLocalShortDate” 等。同时，提供了手机振动的能力，能够读取ISO-639-1标准的语言代码，或者ISO-3166标准的国家代码，通过指定的URL可以启动应用例如web浏览器。进一步，能够发送短信，建立电话呼叫，发送email，还能够与移动商店相集成，为用户提供购买面向Flash的内容和应用的能力。](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=http服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=actionscript&t=blog)](https://so.csdn.net/so/search/s.do?q=flash&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




