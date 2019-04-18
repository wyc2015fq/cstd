# Android:自定义view之角度与弧度 - weixin_33985507的博客 - CSDN博客
2017年04月18日 10:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：29
##### 前言
- 
为什么讲这个？
在我们自定义View，尤其是制作一些复杂炫酷的效果的时候，实际上是将一些简单的东西通过数学上精密的计算组合到一起形成的效果。
这其中可能会涉及到画布的相关操作(旋转)，以及一些正余弦函数的计算等，这些内容就会用到一些角度、弧度相关的知识。
- 
为什么对角的描述存在角度与弧度两种单位？
简单来说就是为了方便，为了精确描述一个角的大小引入了角度与弧度的概念。
由于两者进制是不同的(角度是60进制，弧度是10进制),在合适的地方使用合适的单位来描述会更加方便。
例如： 角度是60进位制，遇到30°6′这样的角，应该转化为10进制的30.1°。但弧度就不需要，因为弧度本身就是十进制的实数。(当求∠AOB的时候,得到弧度可能是0.52,转换为角度可能就是30.1 °,只是表示形式不同,下文会介绍怎样快速计算弧度从而转化为角度)
##### 角度与弧度的定义
角度和弧度一样都是描述角的一种度量单位，下面是它们的定义：
**角度** 两条射线从圆心向圆周射出，形成一个夹角和夹角正对的一段弧。当这段弧长正好等于圆周长的360分之一时，两条射线的夹角的大小为1度.
**弧度** 两条射线从圆心向圆周射出，形成一个夹角和夹角正对的一段弧。当这段弧长正好等于圆的半径时，两条射线的夹角大小为1弧度.(也就是1弧度是一个double值)
![3846387-61f72f6b0e21d994.jpg](https://upload-images.jianshu.io/upload_images/3846387-61f72f6b0e21d994.jpg)
1度
![3846387-f517452516571643.jpg](https://upload-images.jianshu.io/upload_images/3846387-f517452516571643.jpg)
1弧度
##### 角度和弧度的换算关系
rad 是弧度， deg 是角度
![3846387-627f7ccda99fd1f7.jpg](https://upload-images.jianshu.io/upload_images/3846387-627f7ccda99fd1f7.jpg)
##### 应用
在自定义view中求一个角度的时候,先求出弧度在转化为角度
假设:点击view中一点`A(touchPoint.x,touchPoint.y)`,求出相对于`O(centerPoint.x,centerPoint.y)`的角度(A就是view坐标系中点击的一点,O就相当于view的中心点)
```
// 两点在X轴的距离
float lenX = (float) (touchPoint.x - centerPoint.x);
// 两点在Y轴距离
float lenY = (float) (touchPoint.y - centerPoint.y);
// 两点距离
float lenXY = (float) Math.sqrt((double) (lenX * lenX + lenY * lenY));
// 计算弧度
double radian = Math.acos(lenX / lenXY) * (touchPoint.y < centerPoint.y ? -1 : 1);
double tmp = Math.round(radian / Math.PI * 180); 
// 计算角度
double angle =tmp >= 0 ? tmp : 360 + tmp;
```
[自定义View目录](https://www.jianshu.com/p/a75a6ff4257b)
