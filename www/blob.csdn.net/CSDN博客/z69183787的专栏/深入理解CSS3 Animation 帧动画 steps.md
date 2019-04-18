# 深入理解CSS3 Animation 帧动画 steps - z69183787的专栏 - CSDN博客
2016年11月18日 10:58:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1092
CSS3我在5年之前就有用了，包括公司项目都一直在很前沿的技术。
最近在写慕课网的七夕主题，用了大量的CSS3动画，但是真的沉淀下来仔细的去深入CSS3动画的各个属性发现还是很深的，这里就写下关于帧动画steps属性的理解
我们知道CSS3的Animation有八个属性
- animation-name
- animation-duration
- animation-delay
- animation-iteration-count
- animation-direction
- animation-play-state
- animation-fill-mode
- animation-timing-function
其中1-7大多都有介绍，但是animation-timing-function是控制时间的属性
在取值中除了常用到的 三次贝塞尔曲线 以外，还有个让人比较困惑的 **steps() 函数**
animation默认以ease方式过渡，它会在每个关键帧之间插入补间动画，所以动画效果是连贯性的
除了ease，linear、cubic-bezier之类的过渡函数都会为其插入补间。但有些效果不需要补间，只需要关键帧之间的跳跃，这时应该使用steps过渡方式
animation-timing-function 规定动画的速度曲线
![image](http://images0.cnblogs.com/blog/329084/201507/110906325499781.png)
以上w3school网站上给的使用方法，但是漏掉一个很重要的** steps**
简单的来说，我们一直使用animation基本都是实现线性渐变的动画
如
- 位置在固定的时间从起点到终点
- 尺寸在固定的时间线性变化
- 颜色的线性改变等等
看效果 [线性动画](http://sandbox.runjs.cn/show/5u3ovsfb)
截取CSS如下
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
.test1 {
    width: 90px;
    height: 60px;
    -webkit-animation-name: skyset;
    -webkit-animation-duration: 2000ms;
    -webkit-animation-iteration-count: infinite; /*无限循环*/
    -webkit-animation-timing-function: linear;
}
@-webkit-keyframes skyset {
    0% { background: red;}
    50%{ background: blue}
    100% {background: yellow;}
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
timing-function:linear 定义的是一个匀速变化的动画，就是在2秒内，从红色过度到蓝色到黄色，是一个很线性的颜色变化
如果要实现帧动画效果而不是线性的变化就需要引入step这个值了，换句话就是没有过渡的效果，而是一帧帧的变化
同样可以看测试 [帧动画](http://sandbox.runjs.cn/show/t5xqz6i4)
**理解steps**
steps 函数指定了一个阶跃函数
第一个参数指定了时间函数中的间隔数量（必须是正整数）
第二个参数可选，接受 start 和 end 两个值，指定在每个间隔的起点或是终点发生阶跃变化，默认为 end。
step-start等同于steps(1,start)，动画分成1步，动画执行时为开始左侧端点的部分为开始；
step-end等同于steps(1,end)：动画分成一步，动画执行时以结尾端点为开始，默认值为end。
看看W3C的规范 [transition-timing-function](http://www.w3.org/TR/2012/WD-css3-transitions-20120403/#transition-timing-function-property)
**steps第一个参数的错误的理解：**
steps(5，start)
steps() 第一个参数 number 为指定的间隔数，即把动画分为 n 步阶段性展示，估计大多数人理解就是keyframes写的变化次数
例如:
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@-webkit-keyframes circle {
        0% {}
        25%{}
        50%{}
        75%{}
        100%{}
 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
我之前也一直认为steps(5，start）中的5 就是指的keyframes中的0% 25% 50% 75% 100% 分成5个间隔等分
为什么会出现这种理解错误，我们看一个例子
keyframes的关键帧是只有2个规则的时候，假如我们有一张400px长度的雪碧图
```
@-webkit-keyframes circle {
        0% {`background-position-x: ``0``;`}
        100%{`background-position-x: -400px;}`
 }
```
此刻设置steps(5，start）那么会发现5张图会出现帧动画的效果，因为steps中的5把 0% – 100%的规则，内部分成5个等分
实际内部会执行这样一个关键帧效果
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@-webkit-keyframes circle {
        0% {background-position-x: 0;}
        25% {background-position-x: -100px;}
        50% {background-position-x:-200px;}
        75%{background-position-x: -300px;}
        100%{background-position-x: -400px;}
 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
将这个规则稍微修改下，加入一个50%的状态
```
@-webkit-keyframes circle {
        0% {background-position-x: 0;}
        50% {background-position-x: -200px;}
        100%{background-position-x: -400px;}
 }
```
那么同样用steps(5，start）效果就会乱套
此刻你会很迷惑，所以关键要理解第一个参数的针对点，首先引入一个核心点：
**timing-function 作用于每两个关键帧之间，而不是整个动画**
那么第一个参数很好理解了，steps的设置都是针对两个关键帧之间的，而非是整个keyframes，所以第一个参数对 - 次数对应了每次steps的变化
换句话说也是 0-25 之间变化5次，  25-50之间 变化5次 ，50-75 之间变化5次，以此类推
第二个参数可选，接受 start 和 end 两个值，指定在每个间隔的起点或是终点发生阶跃变化，默认为 end
通过案例看下 [step-start](http://sandbox.runjs.cn/show/d3dqpslp)，[step-end](http://sandbox.runjs.cn/show/viihpjmo) 的区别
```
@-webkit-keyframes circle {
        0% {background: red}
        50%{background: yellow}
        100% {background: blue}
    }
```
[step-start](http://sandbox.runjs.cn/show/d3dqpslp) ： 黄色与蓝色相互切换
[step-end](http://sandbox.runjs.cn/show/viihpjmo)  ： 红色与黄色相互切换
2个参数都会选择性的跳过前后部分，start跳过0%，end跳过100%
step-start在变化过程中，都是以下一帧的显示效果来填充间隔动画，所以0% 到 50%  直接就显示了黄色yellow
step-end与上面相反，都是以上一帧的显示效果来填充间隔动画，所以0% 到 50% 直接就显示了红色red
引用[w3c](http://www.w3.org/)的一张step的工作机制图
![](http://images.cnitblog.com/i/596159/201406/091121212334792.png)
总结：
steps函数，它可以传入两个参数，第一个是一个大于0的整数，他是将间隔动画等分成指定数目的小间隔动画，然后根据第二个参数来决定显示效果。
第二个参数设置后其实和step-start，step-end同义，在分成的小间隔动画中判断显示效果。可以看出：steps(1, start) 等于step-start，steps(1,end)等于step-end
最核心的一点就是：**timing-function 作用于每两个关键帧之间，而不是整个动画**
[http://designmodo.com/steps-css-animations/](http://designmodo.com/steps-css-animations/)
如果您看完本篇文章感觉不错，请点击一下右下角的**【推荐】**来支持一下博主，谢谢！
如果是原创文章，转载请注明出处！！！
by Aaron：[http://www.cnblogs.com/aaronjs/p/4642015.html](http://www.cnblogs.com/aaronjs/p/4642015.html)
