# Introduction to RenderMonkey - 逍遥剑客 - CSDN博客
2007年09月18日 03:15:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2892

RenderMokey是ATi发布的一个快速开发着色器程序和效果的工具.它解决了几个问题:
1.着色器程序通常需要一个框架才能运行.这样的一个工具必须可以设置几何和纹理等信息而不用额外的代码
2.传统的着色器程序开发时间消耗太大,而且需要高质量的代码做为依托.这就增加了开发难度,并限制了技术的共享
3.着色器开发工具应该在程序员和美工之间建立一个桥梁,所以它不应该依赖任何API,也不应该需要太高的编程能力
4.工具的框架应该要能够适应未来技术的发展
基于这些目标,ATi推出一个强大而且高效的开发工具:RenderMonkey,它大大降低了着色器程序开发的门槛,你只需要基本的图形学知识就可以使用它.
**主界面**
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/RenderMokey_1.JPG)
|**Workspace****窗口**![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/RenderMokey_2.JPG)|这里面包含很多不同类型的元素,可以分成4组:1.组元素,用来把元素组织成一个层次结构,如effect,pass等2.着色器或效果用到的参数,包括矩阵,向量,颜色等.它们可以用Artist编辑器来设定,我们会在后面提到3.着色器或效果的状态控制,包括着色器程序,硬件渲染状态,或顶点流映射4.着色器用到的资源,通常是网格和纹理这些东西被保存到一个.RFX文件中,里面是XML格式的.这样可以用程序来方便地读取|
|----|----|
|**效果预览窗口**![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/RenderMokey_3.JPG)|从这里你可以看到着色器程序的最终效果.当然,你可以用鼠标来控制摄像机观察方式|
**输出窗口**
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/RenderMokey_4.JPG)
如果在这里出现了错误信息,你可以像在VC中一样双击它,RenderMonkey会自动打开编辑器窗口,并把光标定位到相应的位置
**编辑器窗口**
**![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/RenderMokey_5.JPG)**
它可以用来编辑ASM代码或HLSL,而且在编辑的同时状态栏还会给出相应的代码提示
|**Artist****编辑器**![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/RenderMokey_6.JPG)|这个窗口默认不显示,可以通过View->Artist Editor来调出来在这里你可以设置各个参数元素,而且可以即时显示结果|
下面看一下整个工程的组织形式
1.库存变量,这里是View-Projection矩阵
2.流映射,即这里的PNTT,应该是定义的VertexShader的顶点输入格式,双击可以打开编辑框
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/RenderMokey_7.JPG)
3.效果组,它代表一个独立的Shader程序,你可以在一个工作空间中建立多个Shader
|变量列表纹理模型Pass模型引用渲染状态        VertexShader        PixelShader纹理信息(我想可能是Sampler吧?)![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/RenderMokey_9.JPG)|![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/RenderMokey_8.JPG)|
