# 跨平台的WebRTC客户端框架：OpenWebRTC - DoubleLi - 博客园






# Webrtc的ios框架编译

http://www.th7.cn/Program/IOS/201502/390418.shtml





# [WebRTC in WebKit](http://www.webrtcinwebkit.org/) : http://www.webrtcinwebkit.org/







OpenWebRTC was designed for flexibility and modularity. The bulk of the API layer is implemented in [JavaScript](http://lib.csdn.net/base/javascript), making it super fast to modify and extend with new functionality. Below is a simplified sketch of the architecture.

OpenWebRTC 是基于 Gstreamer 实现的开源的、跨平台的 WebRTC 客户端框架，支持 H.264 和 VP8。利用 OpenWebRTC，WebRTC就不再仅仅是纯粹浏览器技术了，你可以在NativeAPP中使用WebRTC，并且还可以与浏览器WebRTC互联互通。\![Simplified architecture](https://camo.githubusercontent.com/3c850ef3644418391ba6328a48f69b52d829aa67/687474703a2f2f7374617469632e73717561726573706163652e636f6d2f7374617469632f3533663165656465653462303433396266386434383063352f742f3534323431653332653462303465363938646666656365632f313431313635333137303130322f417263682e706e67)








[](http://gstreamer.freedesktop.org/)
![icon-projects.png](http://static1.squarespace.com/static/53f1eedee4b0439bf8d480c5/t/53fc7035e4b00c4d1bfcd98a/1409404624884/icon-projects.png?format=300w)







## Based on GStreamer

Among [other](http://www.openwebrtc.org/third-party-licenses) great projects, OpenWebRTC is built on top of widely used and powerful [GStreamer](http://gstreamer.freedesktop.org/) multimedia framework.











## Extension of UIWebView



Bowser is based on the official `UIWebView` provided by the platform and the [WebRTC](http://www.w3.org/2011/04/webrtc/) API's are implemented with JavaScript that is injected into web pages as they load, the injected JavaScript code is using remote procedure calls to control the [OpenWebRTC](https://github.com/EricssonResearch/openwebrtc) backend.

The [plan](https://github.com/EricssonResearch/bowser/issues/1) is to move to the `WKWebView`, introduced in [iOS](http://lib.csdn.net/base/ios) 8, as soon as possible.







## Background



Bowser was originally developed by Ericsson Research and released in October of 2012, for both iOS and [Android](http://lib.csdn.net/base/android) devices. Back then Bowser was the world's first WebRTC-enabled browser for mobile devices. Bowser was later removed from the Apple App Store and Google Play but was resurrected and released as Open Source together with OpenWebRTC.











**·**[爱立信](http://www.c114.net/keyword/%B0%AE%C1%A2%D0%C5)研究院通过推出自由开源的网页浏览器和底层框架，继续致力于WebRTC标准

**·**目前这一浏览器是iOS上唯一的WebRTC浏览器

**·**OpenWebRTC是一个灵活的跨平台WebRTC客户端框架，可用于构建本地 WebRTC应用和浏览器后端

C114讯 爱立信研究院日前宣布推出自由开源的网页浏览器Bowser和底层框架OpenWebRTC，旨在为开发人员提供更多的选择和更大的灵活性，从而进一步加快WebRTC（网页实时通信）领域的创新。

WebRTC提供了一种实时构建语音、视频及数据应用的超简单方法，WebRTC包括一系列的API及协议，万维网联盟（W3C）和[互联网](http://www.c114.net/keyword/%BB%A5%C1%AA%CD%F8)工程任务组（IETF）正在对这些API和协议进行标准化。

OpenWebRTC依托的理念就是，WebRTC标准会超越纯浏览器环境，而本地应用会实现相同的协议和API，成为WebRTC生态系统的重要部分。在[移动](http://www.c114.net/keyword/%D2%C6%B6%AF)平台上尤其如此，发布本地应用通常比纯[网络](http://www.c114.net/keyword/%CD%F8%C2%E7)应用更可取。

![](http://image.c114.net/webmail(4).jpg)

图 ：爱立信研究院研究主管Stefan Ålund

爱立信研究院研究主管Stefan Ålund表示：“我们在2012年推出了Bowser，获得了广泛欢迎。现在，我们不仅推出了Bowser，还推出了近几年一直在爱立信内部开发和使用的底层跨平台WebRTC框架。”

爱立信研究院积极参与 WebRTC的标准化进程，自成立以来一直致力于实现该标准的原型。此标准要实现，IETF和W3C均要求至少实现两个独立的、可互操作的标准。

Ålund继续说：“WebRTC标准仍在发展，开发人员也在不断寻找利用这一技术的新方法。我院工程师构建的OpenWebRTC ，可极其简单地进行修改和扩展，以便留出空间尝试更多的API及新特点。”

Bower作为开放源码推出，已提交至[苹果](http://www.c114.net/keyword/%C6%BB%B9%FB)应用程序商店，很快就可免费下载。













# Safari开始支持WebRTC


 2015年9月12日







WebRTC和WebKit

WebRTC是基于浏览器的实时通信接口，主要支持通过浏览器进行点对点的音频以及视频通信。WebRTC规范由[W3C](http://www.w3.org/2011/04/webrtc/)和[IETF](https://datatracker.ietf.org/wg/rtcweb/)共同制定。通过这些API，Web开发者可以通过标准的JavaScript API在网页中嵌入视频、音频通信功能。

[WebKit](https://www.webkit.org/)是一个开源的浏览器引擎，它由WebCore排版引擎、JSCore JavaScript引擎和各种平台移植代码组成。WebKit目前缺乏对WebRTC标准的支持，因此开发者构建了[webrtcinwebkit.org](http://www.webrtcinwebkit.org/)网站，开始在WebKit上加入对WebRTC的支持。最初，该项目在WebKit的GTK移植中使用[OpenWebRTC](http://www.openwebrtc.io/)增加对WebRTC支持，后续的支持，将会逐渐放入到WebCore中，这样基于WebKit的所有移植环境就能方便的使用。同时，该项目未来还将接入更多的WebRTC实现后段，比如[webrtc.org](http://webrtc.org/)等。

这些工作会使苹果公司更容易的在Safari上实现WebRTC功能。

苹果公司做了什么？

WebKit只是一个浏览器引擎，每个使用它的浏览器，都是一个移植版本（port）。webrtcinwebkit项目，最早在WebKit的GTK移植上进行了[测试](http://lib.csdn.net/base/softwaretest)，在[7月](http://www.webrtcinwebkit.org/blog/2015/7/10/updated-mediastream-api-upstreamed)的时候，[Linux](http://lib.csdn.net/base/linux)版浏览器实现了对getUserMedia接口的支持。

针对不同浏览器针对WebRTC协议栈的实现，可以参见这个[幻灯片](http://www.slideshare.net/alexpiwi5/webrtc-browsers-n-stacks-implementation-differences)。作为背景知识，它介绍了不同浏览器如何将WebRTC实现融合到自己的[架构](http://lib.csdn.net/base/architecture)中去。

对于苹果公司，mac平台上的Safari和iOS平台上的Safari是独立的WebKit移植版本。它们没法直接使用之前为GTK移植版本上使用的WebRTC接入代码。因此苹果公司需要单独为这两个移植版本实现WebRTC的接入。

截至8月底，通过的[Git](http://lib.csdn.net/base/git)仓库的提交记录的[筛选](http://pastebin.com/DbwYw3GD)，苹果公司的主要改动有：
- 针对mac和iOS平台上的改动
- /platform/mediastream/mac
- /platform/mac-mavericks/
- /platform/mac-yosemite/

- 针对AVFoundation接口的修改（AVVideoCaptureSource.h）
- 针对苹果平台构建文件的修改（WebCore.xcodeproj/project.pbxproj）

从改动来看，目前苹果已经实现了对GetUserMedia和MediaStream接口在mac和iOS两个平台上的支持。进而支持了对音频、视频设备的列举和选择，支持视频、音频流作为audio和video两个标签的输入来源，支持视频抓取等等功能。

不过，和webrtcinwebkit项目最初实现一样，这些代码改动主要在mac系统移植部分。这是由于WebRTC中的很多功能，涉及到平台相关硬件设备交互、媒体流等需要在标签页之间共享，使得改动主要针对渲染层。

希望这些修改能够让我们能够尽快的在Safari或者iOS的WebView中看见效果。











# [openwebrtc（1） 服务端和android客户端demo安装](http://blog.csdn.net/freewebsys/article/details/46649667)









### 1，关于openwebrtc



OpenWebRTC 是基于 Gstreamer 实现的开源的、跨平台的 WebRTC 客户端框架，支持 H.264 和 VP8。利用 OpenWebRTC，WebRTC就不再仅仅是纯粹浏览器技术了，你可以在NativeAPP中使用WebRTC，并且还可以与浏览器WebRTC互联互通。

![这里写图片描述](http://img.blog.csdn.net/20150626134730312)

### 2，安装服务端demo

demo使用nodejs做服务器。centos安装：
<code class="hljs cmake has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;">yum <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">install</span> nodejs nodejs-devel</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; "><li style="box-sizing: border-box; padding: 0px 5px;">1</li></ul>
下载example代码：
<code class="hljs vala has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;">cd /data git clone https:<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">//github.com/EricssonResearch/openwebrtc-examples.git</span> cd openwebrtc-examples/web <span class="hljs-preprocessor" style="color: rgb(68, 68, 68); box-sizing: border-box;">#启动</span> nohup node channel_server.js & <span class="hljs-preprocessor" style="color: rgb(68, 68, 68); box-sizing: border-box;">#默认端口8080</span></code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; "><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li></ul>
安装文档参考： 
[https://github.com/EricssonResearch/openwebrtc-examples/tree/master/web](https://github.com/EricssonResearch/openwebrtc-examples/tree/master/web)

之后就可以在web进行视频了，使用支出webrtc的浏览器。首先创建房间，然后让另一个客户端，join再call就可以了。

![](http://img.blog.csdn.net/20150626135321792)

### 3，android客户端demo

下载最新的android studio，配置好。编译android项目。 
[https://github.com/EricssonResearch/openwebrtc-examples/tree/master/android/NativeCall](https://github.com/EricssonResearch/openwebrtc-examples/tree/master/android/NativeCall)
项目NativeCall 是客户端sdkdemo。比较粗糙。 
需要注意的是，编译会出警告。增加配置：abortOnError ： false
<code class="hljs cs has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;">apply plugin: <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">'com.android.application'</span> android { compileSdkVersion <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">22</span> buildToolsVersion <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"21.1.2"</span> defaultConfig { applicationId <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"com.ericsson.research.owr.examples.nativecall"</span> minSdkVersion <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">16</span> targetSdkVersion <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">22</span> versionCode <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span> versionName <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"1.0"</span> ndk { abiFilter <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"armeabi-v7a"</span> } } <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">// compileOptions {</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">// sourceCompatibility JavaVersion.VERSION_1_7</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">// targetCompatibility JavaVersion.VERSION_1_7</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">// }</span> productFlavors { } android { lintOptions { abortOnError <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">false</span> } } } dependencies { <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">// See settings.gradle for local dev instructions</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">// compile project(':openwebrtc-sdk')</span> compile <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">'io.openwebrtc:openwebrtc-android-sdk:0.1.0'</span> } </code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; "><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li></ul>
其中buildToolsVersion 根据自己的版本修改。

修改config配置，进入app再修改也行。假设服务安装在192.168.1.60。
<code class="hljs java has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">package</span> com.ericsson.research.owr.examples.nativecall; <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">public</span> <span class="hljs-class" style="box-sizing: border-box;"><span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">class</span> <span class="hljs-title" style="box-sizing: border-box; color: rgb(102, 0, 102);">Config</span> {</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">/* Changing this does not take effect until app user data is cleared or the app is uninstalled. The server address can be configured inside the application by pressing the icon in the top right. */</span> <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">//public static final String DEFAULT_SERVER_ADDRESS = "http://demo.openwebrtc.io:38080";</span> <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">public</span> <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">static</span> <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">final</span> String DEFAULT_SERVER_ADDRESS = <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"http://192.168.1.60:8080"</span>; <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">public</span> <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">static</span> <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">final</span> String STUN_SERVER = <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"mmt-stun.verkstad.net"</span>; } </code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; "><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li></ul>
### 4，总结

本文的原文连接是: [http://blog.csdn.net/freewebsys/article/details/46649667](http://blog.csdn.net/freewebsys/article/details/46649667) 转载请一定注明出处！

android客户端，可以和web端进行视频，但是好像木有声音，需要再研究下。 
使用openwebrtc可以很简单的搭建web视频聊天。但是需要优化的地方还很多。继续研究。









