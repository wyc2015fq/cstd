# html中object和embed标签的区别 - 深之JohnChen的专栏 - CSDN博客

2017年03月05日 15:22:53[byxdaz](https://me.csdn.net/byxdaz)阅读数：11659
个人分类：[web前端](https://blog.csdn.net/byxdaz/article/category/5719017)



        object定义一个嵌入的对象。请使用此元素向您的 XHTML 页面添加多媒体。此元素允许您规定插入 HTML 文档中的对象的数据和参数，以及可用来显示和操作数据的代码。

<object> 标签用于包含对象，比如图像、音频、视频、Java applets、ActiveX、PDF 以及 Flash。

embed标签定义嵌入的内容，比如插件。

object和embed的区别：

1、是为了兼容不同浏览器，IE只支持对Object的解析；火狐，谷歌，Safari只支持对Embed的解析。

2、object标签用clsid表示控件的唯一id，而embed标签用type表示插件的唯一名称。比如flash插件type为：application/x-shockwave-flash，mp3播放插件type为audio/mpeg。

3、为了兼容多个浏览器，可以通过ie浏览器动态加载Object标签，非ie浏览器动态加载embed标签；或者在object标签里面嵌入embed标签。

flash的简单实例

下面是简单的网页中插入flash的代码示例： 

代码如下:

```
<OBJECT classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" 
codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,40,0"
WIDTH="550" HEIGHT="400" id="myMovieName"> 
<PARAM NAME=movie VALUE="myFlashMovie.swf"> 
<PARAM NAME=quality VALUE=high> 
<PARAM NAME=bgcolor VALUE=#FFFFFF> 
<EMBED src="http://www.doflash.net/"/support/flash/ts/documents/myFlashMovie.swf"" quality=high bgcolor=#FFFFFF WIDTH="550" HEIGHT="400" 
NAME="myMovieName" ALIGN="" type="application/x-shockwave-flash" 
PLUGINSPAGE="http://www.macromedia.com/go/getflashplayer"> 
</EMBED> 
</OBJECT>
```
使用 object 和 embed 标签来嵌入，细心的会发现，object 的很多参数和 embed 里面的很多属性是重复的，为什么这样做？为了浏览器兼容性，有的浏览器支持 object，有的支持 embed，这也是为什么要修改 Flash 的参数时两个地方都要改的原因。这种方法是 Macromedia 一直以来的官方方法，最大限度的保证了 Flash 的功能，没有兼容性问题。

现阶段用 JS 嵌入 Flash 是最完美的方法，有人写了很完善的嵌入脚本可以方面地下载使用（推荐 SWFObject），SWFObject.js下载地址https://github.com/swfobject/swfobject

objec参数说明

param name标签是在这个播放插件中嵌入的一些功能和播放参数,比如（你可以抄下来）: 

<param name="playcount" value="1"><!--控制重复次数: “x”为几重复播放几次; x=0，无限循环。--> 

<param name="autostart" value="0"><!--控制播放方式: x=1，打开网页自动播放; x=0，按播放键播放。--> 

<param name="clicktoplay" value="1"><!--控制播放开关: x=1，可鼠标点击控制播放或暂停状态; x=0，禁用此功能。--> 

<param name="displaysize" value="0"><!--控制播放画面: x=0，原始大小; x=1，一半大小; x=2，2倍大小。--> 

<param name="enablefullscreen controls" value="1"><!--控制切换全屏: x=1，允许切换为全屏; x=0，禁用此功能。--> 

<param name="showaudio controls" value="1"><!--控制音量: x=1，允许调节音量; x=0，禁止音量调节。--> 

<param name="enablecontext menu" value="1"><!--控制快捷菜单: x=1，允许使用右键菜单; x=0，禁用右键菜单。--> 

<param name="showdisplay" value="1"><!--控制版权信息: x=1，显示电影及作者信息;x=0，不显示相关信息--> 

<param NAME="AutoStart" VALUE="-1"><!--是否自动播放--> 

<param NAME="Balance" VALUE="0"><!--调整左右声道平衡,同上面旧播放器代码--> 

<param name="enabled" value="-1"><!--播放器是否可人为控制--> 

<param NAME="EnableContextMenu" VALUE="-1"><!--是否启用上下文菜单--> 

<param NAME="url" VALUE="http://1.wma"> <!--播放的文件地址--><param NAME="PlayCount" VALUE="1"><!--播放次数控制,为整数--> 

<param name="rate" value="1"><!--播放速率控制,1为正常,允许小数,1.0-2.0--> 

<param name="currentPosition" value="0"><!--控件设置:当前位置--> 

<param name="currentMarker" value="0"><!--控件设置:当前标记--> 

<param name="defaultFrame" value=""><!--显示默认框架--> 

<param name="invokeURLs" value="0"><!--脚本命令设置:是否调用URL--> 

<param name="baseURL" value=""><!--脚本命令设置:被调用的URL--> 

<param name="stretchToFit" value="0"><!--是否按比例伸展--> 

<param name="volume" value="50"><!--默认声音大小0%-100%,50则为50%--> 

<param name="mute" value="0"><!--是否静音-- 

<param name="uiMode" value="mini"><!--播放器显示模式:Full显示全部;mini最简化;None不显示播放控制,只显示视频窗口;invisible全部不显示--><param name="windowlessVideo" value="0"><!--如果是0可以允许全屏,否则只能在窗口中查看--> 

<param name="fullScreen" value="0"><!--开始播放是否自动全屏--> 

<param name="enableErrorDialogs" value="-1"><!--是否启用错误提示报告--> 

<param name="SAMIStyle" value><!--SAMI样式--> 

<param name="SAMIFilename" value><!--字幕ID--> 

