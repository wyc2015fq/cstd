# 梳理 Opengl ES 3.0 （一）宏观着眼 - DesignYourDream - 博客园
# [梳理 Opengl ES 3.0 （一）宏观着眼](https://www.cnblogs.com/designyourdream/p/6737774.html)
　　Opengl ES 可以理解为是在**嵌入式设备**上工作的一层用于处理图形显示的软件，是Opengl 的缩水版本。
下图是它的工作流程示意图：
![](https://images2015.cnblogs.com/blog/698097/201704/698097-20170420143236759-2083858437.png)
**注意图中手机左边的EGL Layer**
Opengl ES是跨平台（即可支持不同操作系统）的软件。它如果想调用与系统相关，与硬件相关的操作，并不是通过直接访问操作系统，而是经过了一个中间层，即 EGL Layer，这个EGL layer当然也是Opengl es不可分割的一部分，失去了它，自然也就失去了和操作系统交互的方式了。不同的系统，要实现自己的EGL，比如在IOS系统下，定义了一个EAGL对象来实现EGL；在Android平台下，定义了GLView类，通过GLView.EGLContextFactory来提供EGL。EGL究竟提供了哪些内容，可以参考这个[官方链接](https://www.khronos.org/egl)。
Opengl ES 3.0这个软件的一代即1.1版本，使用的是固定式流水线；目前的2.0/3.0版本可理解为可修改式流水线。流水线这个词非常适合描述这个软件的工作流程，其实就是个标准的代码顺序执行的过程吧，应该是和并行相对的词。
先看看这个**固定流水线**的图：
![](https://images2015.cnblogs.com/blog/698097/201704/698097-20170420141029931-1568227597.png)
后续的解读会逐步展开每一个阶段的具体过程。
**可编程流水线**。
![](https://images2015.cnblogs.com/blog/698097/201704/698097-20170420142739009-1193072746.png)
所谓“可修改”在程序员看来自然就是可编程了，所谓可编程，就是可以开放方法，可以让开发者自定义方法，这个写出来的方法，就是程序代码了。普通代码是在CPU下运行，让CPU去处理代码。而这种为渲染流水线制作的代码，是放在GPU上运行的，被称为 Shader 。
在前述固定管线长长的流水作业过程中，可编程管线**强制要求**上层应用程序员使用Shader来完成其中的某些步骤。
 简单提下 **Shader**
Opengl ES 3.0内部有两种可编程处理单元，即Vertex processor和Fragment processor，分别用来处理Vertex shader executable和Fragment shader executable。注意，Opengl ES 3.0不支持Geometry Shader。上图中，紫色部分就是可执行体了，即 executable .
**不同平台上的开发 **
1.Android。可以使用包含了OpenGL es的基于java的框架来使用java语言实现相关功能。还可以使用Native Developement Kit（NDK）来使用C/C++语言开发，这个时候JNI会作为JAVA核心框架与NDK C++代码之间的桥梁。
2.IOS。Opengl es 3.0在IOS 7以及以后的版本里进行了完整的实现。也就是Iphone 5s以后的版本。Mac系统提供了Xcode IDE来开发IOS应用，可以发布到iphone,ipad,ipod上，opengl es 3.0要求xcode 5以上版本。即IOS7 SDK + XCODE 5.0

