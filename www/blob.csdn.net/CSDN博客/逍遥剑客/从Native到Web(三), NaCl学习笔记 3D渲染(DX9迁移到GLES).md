# 从Native到Web(三), NaCl学习笔记: 3D渲染(DX9迁移到GLES) - 逍遥剑客 - CSDN博客
2012年11月12日 00:41:47[xoyojank](https://me.csdn.net/xoyojank)阅读数：5206
NaCl的3D渲染API使用的GLES2.0, 这也很好理解, 因为这已经是公认的跨平台标准了. 手机, 平板, 网页, PC都可以使用.
就算在Windows上, 也有一些基于DX9的GLES2.0实现, 比如[ANGLE](http://www.google.com.hk/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&cad=rja&ved=0CCcQFjAA&url=http%3A%2F%2Fcode.google.com%2Fp%2Fangleproject%2F&ei=q8SfUKqCMeyViQettoGoCw&usg=AFQjCNHxqKPfwiocXLvgY9BZufegYrDSuw). 有时候我真在想, 像M$这种升级个操作系统版本号就淘汰一种API的做法, 还不如全都使用标准库, 这样也节省了跨平台移植的成本~所以什么DX10/DX11, 还是省省吧
其实从DX9到GLES(本文提到的都是指2.0版本), 大部分只是API换了种写法, 难度不大. 之前有在手机上做过一个简单的GLES渲染器, 有一些差异总结一下吧(不限于NaCl):
- GLES没有固定管线, 就算画条线要也写个shader. 不过我到是挺喜欢这种设计的, 干净
- GLES现在的支持最低相当于SM2.0(一般是嵌入式设备), 主流的手机等都是SM3.0的标准. 所以, 常用的渲染效果都可以实现, 做个次世代画面也有一定的可行性
- GLES的API都是C函数, 不像DX9那种有点面向对象的设计. 但是实际写代码, 这种API比较方便, 不用依赖指针
- GLES因为是为移动平台设计的, 所以对于标准支持的特性有限. 比如FloatTexture, DXTn, BufferLock等都是基于扩展来实现的, 这点跟GL很像
- GLES投影后Z的范围是[-1, 1], 这点需要特别注意. 至于什么左右手坐标系的问题, 这个看使用习惯了
- GLES的AlphaTest是在Shader中使用discard指定完成, 没有对应的渲染状态
- GLES的VertexBuffer和IndexBuffer在类型上是没有区别的, 只有使用上的区别
- GLES中没有DX9烦人的DeviceLost, 这点还是很不错的
- GLES中没有VertexDeclaration, 需要自己管理, 还可以接受. 不过最不爽的是, Shader里竟然没有SEMANTIC, 参见《[看得让人DT的GLSL](http://blog.csdn.net/xoyojank/article/details/4683613)》
- GLES使用Shader是GLSL的语法, 不得不说, 没有HLSL好用. 很多引擎的做法是弄个HLSL2GLSL的转换器
- GLES的Shader多了一个Link的过程, 所以能编译通过并不代表这个Shader就是可用的
- GLES的Shader中可以指定浮点数的精度, 这个更多的是基于移动平台机能的考虑
- GLES没有像D3DX这样的扩展库, 所以很多像数学库, 纹理载入, Shader管理等是需要自己动手的. 不过想想, 这些东西在商业游戏中很少有人用, 都是自己实现的. 只不过相对于初学者来说, 门槛高了一点而已. 但是DX9那些COM什么的, 也是增加复杂度的东西, 从这个角度讲, 两者算个平手吧
- 图形调试工具的支持, 不得不说, 这方面DX9/10/11有先天优势...
- 待补充
比较下来, 也就是Shader部分需要多操点心, 其它都还好, 甚至还强过DX9. 所以, 我有种用ANGLE代替DX9的冲动...
