# Onvif开发之基础介绍篇 - DoubleLi - 博客园






**什么是Onvif协议,谁开启了Onvif时代?**

![](http://img.blog.csdn.net/20131224183013406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWF4X21pbl9nbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

[ONVIF](http://www.onvif.org/)：原意为 开放型网络视频接口论坛，即 Open Network Video Interface Forum ，是安讯士、博世、索尼等三家公司在2008年共同成立的一个国际性 开放型 网络视频产品 标准网络接口 的开发论坛，后来由这个技术开发论坛共同制定的开放性行业标准，就用该论坛的大写字母命名，即ONVIF 网络视频标准规范，习惯简称为：ONVIF协议。ONVIF网络视频协议的出现，解决了不同厂商之间开发的各类设备不能融合使用的难题，提供了统一的网络视频开发标准，即最终能够通过ONVIF这个标准化的平台实现不同产品之间的集成。在安防监控行业，ONVIF协议将会在较长时间内成为网络视频领域的首选

ONVIF规范作用
       ONVIF规范描述了网络视频的模型、接口、数据类型以及数据交互的模式。并复用了一些现有的标准，如WS系列标准等。ONVIF规范的目标是实现一个网络视频框架协议，使不同厂商所生产的网络视频产品（包括摄录前端、录像设备等）完全互通。
ONVIF规范的实现
     ONVIF规范中设备管理和控制部分所定义的接口均以Web Services的形式提供。ONVIF规范涵盖了完全的XML WSDL的定义。每一个支持ONVIF规范的终端设备均须提供与功能相应的Web Service。服务端与客户端的数据交互采用SOAP议。ONVIF中的其他部分比如音视频流则通过RTP/RTSP进行 。 
ONVIF规范的优势
    协同性：不同厂商所提供的产品，均可以通过一个统一的“语言”来进行交流。方便了系统的集成。
    灵活性：终端用户和集成用户不需要被某些设备的固有解决方案所束缚。大大降低了开发成本。
    质量保证：不断扩展的规范将由市场来导向，遵循规范的同时也满足主流的用户需求。
ONVIF的将来
     由于采用WSDL+XML模式，使ONVIF规范的后续扩展不会遇到太多的麻烦。XML极强的扩展性与SOAP协议开发   的便捷性将吸引到更多的人来关注和使用ONVIF规范。ONVIF的组织日益壮大，与同领域的PSIA，HDCCTV相比，占据了绝对的人员优势。会员企业不乏国内外著名的设备制造商与集成商。一套规范、协议的生命周期，与市场占有率是息息相关的。而ONVIF规范的发展则正是由市场来导向，由用户来充实的。每一个成员企业都拥有加强、扩充ONVIF规范的权利。ONVIF规范所涵盖的领域将不断增大。目前门禁系统的相关内容也即将被纳入ONVIF规范之中。
在安防、监控系统急速发展的今天，效率和质量的领先所带来的价值不言而喻。ONVIF协议提供了这样的潜质。

ONVIF几个基本模块
设备发现、设备管理、设备输入输出服务、图像配置、媒体配置、实时流媒体、接收端配置、显示服务、事件处理、PTZ控制、其他。
基于ONVIF规范项目的开发基本流程
![](http://img.blog.csdn.net/20131224183222781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWF4X21pbl9nbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
ONVIF规范的实现机制
Web Service是基于网络的、分布式的模块化组件，执行特定的任务。Web Service 主要利用 HTTP 和 SOAP 协议使数据在 Web 上传输。Web 用户能够使用 SOAP 和 HTTP通过 Web 调用的方法来调用远程对象。  

![](http://img.blog.csdn.net/20131224183319859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWF4X21pbl9nbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


Web Service简介
客户端根据 WSDL 描述文档，会生成一个 SOAP 请求消息，该请求会被嵌入在一个HTTP POST请求中，发送到 Web Services 所在的Web 服务器。Web Services 请求处理器解析收到的 SOAP 请求，调用相应的 Web Services。然后再生成相应的 SOAP 应答。Web 服务器得到 SOAP 应答后，会再通过 HTTP应答的方式把信息送回到客户端。

![](http://img.blog.csdn.net/20131224183352703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWF4X21pbl9nbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

构建SOAP消息
SOAP是Simple Object Access Protocol的缩写。是基于XML的一种协议。一条 SOAP 消息就是一个普通的 XML 文档，包含下列元素：
1  必需的 Envelope 元素，可把此 XML 文档标识为一条 SOAP 消息
2  可选的 Header 元素，包含头部信息
3  必需的 Body 元素，包含所有的调用和响应信息
4  可选的 Fault 元素，提供有关在处理此消息所发生错误的信息
在向Web Service发送的SOAP请求中，Body元素中的字段需与WSDL中数据类型的相符合。在构建SOAP的过程中，必须从WSDL文件中获取并映射这一种对应关系。然而这样一个对应过程将是充满了重复性和机械性的，为了避免不必要的人工差错以及节约开发时间，一个名为gSOAP的编译工具应运而生。gSOAP利用编译器技术提供了一组透明化的SOAP API，并将与开发无关的SOAP实现细节相关的内容对用户隐藏起来。通过将WSDL文件解析序列化为C/C++文件，最小化了Web Service的开发过程。

Web Service与视频监控
ONVIF规范向视频监控引入了Web Service的概念。设备的实际功能均被抽象为了Web Service的服务，视频监控系统的控制单元以客户端的身份出现，通过Web请求的形式完成控制操作。

![](http://img.blog.csdn.net/20131224183645984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWF4X21pbl9nbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](http://img.blog.csdn.net/20131224183708234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWF4X21pbl9nbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](http://img.blog.csdn.net/20131224183726546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWF4X21pbl9nbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

ONVIF的应用
下面左边图为传统视频监控系统的一个局域网应用场景
1. PU设备上线后，向CMU注册，建立连接。2. CMU与PU进行信令交互，请求能力集，获取配置
3. CU上线，向CMU注册，建立连接。4. CMU与CU进行信令交互，传输设备列表5. CU向PU请求码流

![](http://img.blog.csdn.net/20131224190207312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWF4X21pbl9nbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

1. PU设备上线后，向CMU发送HELLO消息。2. CMU需要搜寻设备时，向PU发送PROBE消息。
3. CMU与PU进行信令交互，请求能力集，获取配置4. CU上线，向CMU注册，建立连接。
5. CMU与CU进行信令交互，传输设备列表6. 在CMU的协调下， CU同PU建立连接传输码流
在上述的场景中，通过ONVIF，PU与CMU的交互方式发生了改变；CMU不再与PU保持长连接。遵循ONVIF规范，信令以及消息内容有了统一的标准









