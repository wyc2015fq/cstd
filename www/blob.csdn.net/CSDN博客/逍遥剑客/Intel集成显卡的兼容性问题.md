# Intel集成显卡的兼容性问题 - 逍遥剑客 - CSDN博客
2007年10月29日 21:55:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1968
最近游戏好像老是在GMA950之类的显卡上出现问题
查了一下,应该是因为它不支持硬件VertexShader造成的吧
具体解决方法待查
### GMA 900
应用在采用了[Intel i915G](http://wiki.ccw.com.cn/index.php?title=Intel_i915G&action=edit)[芯片组](http://wiki.ccw.com.cn/%E8%8A%AF%E7%89%87%E7%BB%84)的[主板](http://wiki.ccw.com.cn/%E4%B8%BB%E6%9D%BF)上。 
|核心频率|333 MHz|
|----|----|
|RAMDAC时钟|400 MHz|
|[T/L](http://wiki.ccw.com.cn/index.php?title=Hardware_Transform_and_Lighting&action=edit)|软件模拟（无硬件T&L支持）|
|[像素流水线](http://wiki.ccw.com.cn/index.php?title=%E5%83%8F%E7%B4%A0%E6%B5%81%E6%B0%B4%E7%BA%BF&action=edit)|4|
|[像素填充率](http://wiki.ccw.com.cn/%E5%83%8F%E7%B4%A0%E5%A1%AB%E5%85%85%E7%8E%87)|1333百万像素/秒|
|[顶点着色引擎](http://wiki.ccw.com.cn/%E9%A1%B6%E7%82%B9%E7%9D%80%E8%89%B2%E5%BC%95%E6%93%8E)|1 (v2.0) (软件)|
|[像素着色引擎](http://wiki.ccw.com.cn/index.php?title=%E5%83%8F%E7%B4%A0%E7%9D%80%E8%89%B2%E5%BC%95%E6%93%8E&action=edit)|4 (v2.0) (硬件)|
|[OpenGL](http://wiki.ccw.com.cn/OpenGL)|1.4|
|[DirectX](http://wiki.ccw.com.cn/DirectX)|9.0|
|视频|硬件运动补偿（MPEG-2视频），色彩空间转换，[DirectDraw](http://wiki.ccw.com.cn/DirectDraw)覆盖|
采用 [PowerVR](http://wiki.ccw.com.cn/PowerVR) 的[Zone Rendering Technology](http://wiki.ccw.com.cn/index.php?title=Zone_Rendering_Technology&action=edit),[ST Microelectronics](http://wiki.ccw.com.cn/index.php?title=ST_Microelectronics&action=edit)授权。 
### GMA 950
应用在 i945G芯片组的主板上 
|核心频率|400 MHz|
|----|----|
|RAMDAC时钟|400 MHz|
|[T/L](http://wiki.ccw.com.cn/index.php?title=Hardware_Transform_and_Lighting&action=edit)|软件模拟（无硬件T&L支持）|
|[像素流水线](http://wiki.ccw.com.cn/index.php?title=%E5%83%8F%E7%B4%A0%E6%B5%81%E6%B0%B4%E7%BA%BF&action=edit)|4|
|[像素填充率](http://wiki.ccw.com.cn/%E5%83%8F%E7%B4%A0%E5%A1%AB%E5%85%85%E7%8E%87)|1600百万像素/秒|
|[顶点着色引擎](http://wiki.ccw.com.cn/%E9%A1%B6%E7%82%B9%E7%9D%80%E8%89%B2%E5%BC%95%E6%93%8E)|1 (v3.0)* (软件)|
|[像素着色引擎](http://wiki.ccw.com.cn/index.php?title=%E5%83%8F%E7%B4%A0%E7%9D%80%E8%89%B2%E5%BC%95%E6%93%8E&action=edit)|4 (v2.0) (硬件)|
|[OpenGL](http://wiki.ccw.com.cn/OpenGL)|1.4，增强光照支持|
|[DirectX](http://wiki.ccw.com.cn/DirectX)|9.0|
|视频|[MPEG-2](http://wiki.ccw.com.cn/MPEG-2) VLC，[iDCT](http://wiki.ccw.com.cn/index.php?title=IDCT&action=edit)，硬件运动补偿, 双视频覆盖窗口（1 HD + 1 SD）|
采用 [PowerVR](http://wiki.ccw.com.cn/PowerVR) 的[Zone Rendering Technology](http://wiki.ccw.com.cn/index.php?title=Zone_Rendering_Technology&action=edit),[ST Microelectronics](http://wiki.ccw.com.cn/index.php?title=ST_Microelectronics&action=edit)授权。 
GMA950图形加速器相比上一代的GMA900有所增强。 GMA950核心频率达到了400MHz，搭配DDR2-667内存时最大带宽可达10.6 GB/s，最大可共享192MB系统内存。像素填充率是1.6 GPixels/s和1.6 GTexels/s，共有4条像素渲染管线，分辨率最高支持2048x1536@75Hz，另外GMA950支持DX9硬件Pixel Shader 2和Vertex Shader 3.0，相比GMA900有提高，估计可以达到6200TC的水平。其[OpenGL](http://wiki.ccw.com.cn/OpenGL)版本仍是1.4。此外GMA950也支持HDTV 720p/1080i和宽屏输出，通过专用ADD2扩展卡还可以支持双显示设备以及DVI和S-Video输出。  
