# RenderMonkey在数字图像处理中的应用 - 逍遥剑客 - CSDN博客
2007年10月02日 04:39:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3161

大家可以拿来做<<数字图像处理>>这门课的实验, 呵呵, 这样就不用自己去实现框架了, 而且速度也快^_^
第一步, 这样来创建一个模板程序:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_1.JPG)
如图
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_2.JPG)
我们先来认识一下左边有些什么:
**base**: 这是一个纹理(-_-图形学的术语…), 在这里我们就把它当成我们要处理的图像, 注意图像的长宽都是2的n次幂. 双击可以打对话框更换.
**Stream Mapping**: 不用管它, 保持默认值就好.
**ScreenAlignedQuad**: 其实只是一个正方形网格而已, 做为base纹理的骨架.( 又来了, 图形学-_-).暂且把它当成画板吧, 不用动它.
**Single Pass**: 进行绘制的地方
       ScreenAlignedQuad: 又一个…看见有个小剪头了没? 上面那个同名东的引用.
       Render State: 设置了一些渲染的参数, 不用管它.
       Vertex Shader: 里面写了怎么把ScreenAlignedQuad画到屏幕上, 这种支画板的活我们不关心.
Pixel Shader: 像素级的处理, 这就是我们要发挥我们想像力的地方了!
       Texture0: 上面那幅图的引用.
       Stream Mapping: 也是上面的那个”不明物体”的引用.
下面让我们打开Pixel Shader看看都有什么:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_3.JPG)
是不是看着很像C语言啊? 我再来XX两句:
sampler2D Texture0;
定义一个叫Texture0的图, 见上面的介绍. 注意保持名字的一致, 不然程序就找不到要处理的图像啦.
float4 ps_main( float2 texCoord : TEXCOORD0 ) : COLOR
所谓的float4就是由4个float组成的一个结构, 像这样:float4(x,y,z,w), 如果当成颜色值的话, 就是这样float4(r,g,b,a). 不要问我RGBA是啥, 图像处理时讲的……..
ps_main就是函数名, 括号里是参数
这里的texCoord就是我们要处理的当前像素的坐标值. 细心的可能看到它是float2格式的, 嗯, 没错, 实际上它就是一个float2(x,y) 型的结构. 不过有一点一定要注意: 这里的坐标单位不是基于像素的, 而是基于纹理坐标系的(别扔我…我解释一下就是了)
召唤一幅图:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_4.JPG)
图上的一格代表一个像素, 括号里面就是传统的像素坐标, 下面批示的就是纹理坐标.
聪明的你一眼就能看出来, 纹理坐标各轴上范围是[0,1], 相当于把原来的像素级的坐标除以了图像的宽度(高度).
来, 我们操刀上阵, 修改一下:
双击base换张彩图:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_5.JPG)
把PixelShader做如下修改:
sampler2D Texture0;
float4 ps_main( float2 texCoord : TEXCOORD0 ) : COLOR
{
//当前像素在图像中的颜色值
float4 color = tex2D( Texture0, texCoord );
 //转换RGB为强度值
float intensity = color.r * 0.299 + color.g * 0.587 + color.b * 0.184;
returnfloat4(intensity,intensity,intensity,1);
}
效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/image_process_6.JPG)
成黑白的了, 简单不?
(RGB到强度转换为什么那样转见课本. 你想偷懒的话直接(R+G+B)/3好了……)
未完待续……….
