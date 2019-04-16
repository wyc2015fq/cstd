# BREW 应用中的3维图像技术 - 我相信...... - CSDN博客





2010年10月20日 10:36:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：829标签：[brew																[microsoft																[图形																[平台																[api																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)




OpenGL(Open Graphics Library)是一个2D和3D的开放式图形标准,前身是SGI公司的IRIS GL，Microsoft、SGI、IBM、DEC、SUN、HP等大公司都采用了OpenGL做为三维图形标准。而作为移动3D图形行业标准的OpenGL ES的出现，避免了手机制造商由于互不兼容的游戏软件格式而导致无谓的市场竞争。

OpenGL ES源自PC市场，是一个从OpenGL淬取出来的低容量的先进绘图用API，能够运用OpenGL的资源。。在应用软件与软硬件之间，它提供了底层应用的可用程序化接口(API)。这是3D针对嵌入式系统所制定的绘图API，能够进行3D绘图与3D游戏的实现，在不同的行动装置或是嵌人式系统间应用非常便利。因此，Open GL-ES已经获得了广泛的行业支持，不仅得到了手机制造商及器件与内核提供商的支持，同时也获得了图形引擎、游戏开发商以及操作系统供应商的支持。

OpenGL ES包括floating-point和fixed-point两个profiles。OpenGL ES也包含了平台接口层的规范，称为EGL。这层接口和平台间是独立的，厂商可以选择是否将他放入自己的产品里。整合平台当然也包含了一致性的测试，厂商也可以定义自己的平台接口层。EGL接口实现了和native窗口系统的绑定。

在BREW中，通过OpenGL ES的扩展类提供了对OpenGL ES的良好支持，包含了三个部分：IEGL接口,IGL接口和数据类型。IEGL接口提供了从应用访问EGL实现，打包了标准EGL函数，同时也提供了函数接口。IGL接口提供了从应用访问OpenGL ES实现。从所需要的文件来说，还需要一个OpenGL ES扩展工具包。在这个工具包中，给用户提供了可用来显示3D的设备文件，3D程序运行相关的头文件，以及OPENGL ES扩展的动态链接库。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/07/21/5751916.aspx](http://blog.csdn.net/wireless_com/archive/2010/07/21/5751916.aspx)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




