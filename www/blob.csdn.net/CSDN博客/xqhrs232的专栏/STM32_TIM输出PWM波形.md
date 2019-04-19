# STM32_TIM输出PWM波形 - xqhrs232的专栏 - CSDN博客
2018年07月09日 10:01:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：283
原文地址::[https://blog.csdn.net/ybhuangfugui/article/details/51417207](https://blog.csdn.net/ybhuangfugui/article/details/51417207)
相关文章
1、[STM32之PWM波形输出配置总结](https://www.cnblogs.com/zhoubatuo/p/6135103.html)----[https://www.cnblogs.com/zhoubatuo/p/6135103.html](https://www.cnblogs.com/zhoubatuo/p/6135103.html)
2、**stm32 pwm实现过程----[https://blog.csdn.net/zjl50/article/details/78282166](https://blog.csdn.net/zjl50/article/details/78282166)**
3、**基于stm32单片机的可调节pwm输出----[https://blog.csdn.net/wheat_in_winner/article/details/53375461](https://blog.csdn.net/wheat_in_winner/article/details/53375461)**
4、STM32 PWM输出无信号的问题解决----[http://www.51hei.com/bbs/dpj-42691-1.html](http://www.51hei.com/bbs/dpj-42691-1.html)
5、**STM32F10X PWM配置例程详解,测试无误----[https://blog.csdn.net/moses1994/article/details/50616512](https://blog.csdn.net/moses1994/article/details/50616512)**
6、STM32-PWM输出源码----[https://download.csdn.net/download/kevin_iot/977883](https://download.csdn.net/download/kevin_iot/9778837)
7、STM32的PWM输出程序，亲测能用----[https://download.csdn.net/download/zhaoxin5002/10131693](https://download.csdn.net/download/zhaoxin5002/10131693)
前言
今天讲解“STM32F103  TIM输出PWM波形”，提供的软件工程支持最大输出18MHz的PWM波形，函数接口只有两个参数：频率Freq 和 占空比Dutycycle。
定时器的功能很强大，今天讲解的PWM属于定时器中比较输出的功能，难度算一般吧（懂了的人决定很简单），因为使用了比较的功能。
PWM在实际应用中特别广泛，可能在校的学生不是很了解，但对于很多工作的人来说PWM的应用基本上遍布整个电子化的时代。对于电子相关专业的学生来说，信号发生器都不陌生，信号发生器就是PWM典型的应用。
每天讲解的不仅仅是基础，而是重点，不起眼的重点，容易被人忽视的重点。关注微信公众号“EmbeddDeveloper”还有更多精彩等着你。
![](https://img-blog.csdn.net/20160515151857426)
本文特点：1.提供讲解的软件工程； 2.图片代码比直接贴出源代码容易阅读。
本着免费分享的原则，将讲解的工程源代码分享给大家，还望看到的朋友关注一下微信公众号，并将文章分享给你的朋友。
下载
提供下载的“软件工程”都是在硬件板子上进行多次测试、并保证没问题才上传至360云盘。
今天的软件工程下载地址（360云盘）：
[https://yunpan.cn/cSftz7gAJFujm](https://yunpan.cn/cSftz7gAJFujm)[访问密码](http://mp.weixin.qq.com/s?__biz=MzI4MDI4MDE5Ng==&mid=100000341&idx=1&sn=2e8058f2ec226d47b4b0cbb92ff3d257#rd)
STM32F10x的资料可以在我360云盘下载：
[https://yunpan.cn/crBUdUGdYKam2](https://yunpan.cn/crBUdUGdYKam2)   访问密码 ca90
内容讲解
工程概要说明： 今天配置定时器2通道1输出PWM波形（PA0口输出1KHz、20%占空比），可根据情况修改其他TIM、其他通道。
注意：最大频率和占空比有一定关系。
占空比等于1%时， 最大频率为360KHz
占空比等于50%时，最大频率为18MHz
关于“STM32F103  TIM输出PWM波形” 我把重要的几点在下面分别讲述：
一、RCC时钟配置
![](https://img-blog.csdn.net/20160515151907833)
该函数位于bsp.c文件下面；
使能RCC时钟：RCC_APB2Periph_AFIO 和 RCC_APB1Periph_TIM2；
这两个时钟是今天工程中重要的。
二、PWM输出引脚
![](https://img-blog.csdn.net/20160515151916036)
该函数位于timer.c文件下面；
配置PA0为PWM输出引脚（这里的引脚需要和定时器的通道对应，否则没有波形输出），详情请看数据手册关于引脚的说明。
三、输出PWM配置
![](https://img-blog.csdn.net/20160515151924317)
该函数位于timer.c文件下面；
决定输出频率和占空比主要由这三个参数（TIM2_PRESCALER_VALUE、tim2_period、tim2_pulse）决定的。
以今天工程输出1KHz,20%来举例说明：
TIM2_PRESCALER_VALUE = 72M / 36M - 1 = 1
tim2_period = 36000 - 1
tim2_pulse = 7200
这三个怎么得来的请看代码，今天工程输出的波形如下图：
![](https://img-blog.csdn.net/20160515151935380)
四、今天的重点提示
A.细心研究过代码的人会发现TIM_OCMode_PWM1，还有一个TIM_OCMode_PWM2，这两个的区别是什么呢？ 答案是：输出的极性(也就是方向)不一样。
这时候又有人问：那下面的输出极性TIM_OCPolarity_High与上面的关系是什么呢？答案是：这两个参数配合一起输出决定PWM波形的极性。
也就是说不同参数可以达到一样的效果，下图参数的配置同样可以输出一样的波形。
![](https://img-blog.csdn.net/20160515151942848)
感兴趣的朋友可以试着修改测试一下效果。
B.今天的工程可以设置周期性中断（也就是1ms中断一次），感兴趣的朋友可以试着写一下。使用串口打印测试的话，尽量频率低一点，不然串口响应不过来。
说明
今天提供的软件工程基于STM32F103大容量芯片，其实只要适当修改工程的部分配置,STM32F1的芯片都适用。
最后
如果觉得内容适合你（如：内容易读易懂等），你又想了解更多相关内容，请关注公众号，新内容持续更新中，后期将会有更多优秀内容出现。
赞是一种鼓励、分享是最好的支持！
