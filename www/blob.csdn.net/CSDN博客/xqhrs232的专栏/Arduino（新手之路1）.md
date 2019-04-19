# Arduino（新手之路1） - xqhrs232的专栏 - CSDN博客
2014年05月02日 10:36:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1271
原文地址::[http://blog.csdn.net/hai_qing_xu_kong/article/details/14454067](http://blog.csdn.net/hai_qing_xu_kong/article/details/14454067)
相关文章
1、[Arduino（新手之路2）](http://blog.csdn.net/hai_qing_xu_kong/article/details/14454629)----[http://blog.csdn.net/hai_qing_xu_kong/article/details/14454629](http://blog.csdn.net/hai_qing_xu_kong/article/details/14454629)
2、
# [Arduino（新手之路3）](http://blog.csdn.net/hai_qing_xu_kong/article/details/14523203)----[http://blog.csdn.net/hai_qing_xu_kong/article/details/14523203](http://blog.csdn.net/hai_qing_xu_kong/article/details/14523203)

对于Arduino，我今天刚接触了下，对于新手来说，学一样东西必须先去了解它，于是去百度了下，一下是Arduino的简介、特色、功能等等，或许这些百度都有，但为了广大的Arduino新手们。我还是将这些复制下来，供大家去了解Arduino是什么。
Arduino
[编辑](http://blog.csdn.net/hai_qing_xu_kong/article/details/14454067)
## 目 录
1[简介](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#1)
2[特色](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#2)
3[功能](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#3)
4[发展历史](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#4)
5[中文翻译](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#5)
6[开发团队](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#6)
7[外界评价](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#7)
## 1、简介[](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#)
Arduino，满足您创新创意的需要！
中文名：阿德伟诺
Arduino 是一款便捷灵活、方便上手的开源电子原型平台，包含硬件（各种型号的[arduino](http://baike.baidu.com/view/1268436.htm)板）和软件（arduino IDE)。它适用于艺术家、设计师、爱好者和对于“互动”有兴趣的朋友们。
Arduino，是一个基于开放原始码的软硬体平台，构建于开放原始码simple I/O介面版，并且具有使用类似Java，C语言的Processing/Wiring开发环境。[1]
Arduino能通过各种各样的传感器来感知环境，通过控制灯光、马达和其他的装置来反馈、影响环境。板子上的微控制器可以通过Arduino的编程语言来编写程序，编译成二进制文件，收录进微控制器。对Arduino的编程是利用 Arduino编程语言 (基于 Wiring)和Arduino开发环境(based on Processing)来实现
![](http://h.hiphotos.baidu.com/baike/s%3D220/sign=7612e395c9177f3e1434fb0f40ce3bb9/43a7d933c895d1430a91950073f082025aaf0729.jpg)
的。基于Arduino的项目，可以只包含Arduino，也可以包含Arduino和其他一些在PC上运行的软件，他们之间进行通信 (比如 Flash, Processing, MaxMSP)来实现。
你可以自己动手制作，也可以购买成品套装;Arduino所使用到的软件都可以免费下载. 硬件参考设计 (CAD 文件)也是遵循availableopen-source协议, 你可以非常自由地 根据你自己的要求去修改他们.
[Arduino](http://baike.baidu.com/view/1268436.htm)可以使用开发完成的电子元件例如Switch或sensors或其他控制器、LED、步进马达或其他输出装置。Arduino也可以独立运作成为一个可以跟软件沟通的接口，例如说：[flash](http://baike.baidu.com/view/7641.htm)、processing、Max/MSP、[VVVV](http://baike.baidu.com/view/777692.htm)或其他互动软件…。Arduino开发IDE接口基于[开放源代码](http://baike.baidu.com/view/1708.htm)原，可以让您免费下载使用开发出更多令人惊艳的互动作品。
## 2、特色[](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#)
1、开放[源代码](http://baike.baidu.com/view/60376.htm)的电路图设计，[程序](http://baike.baidu.com/view/17674.htm)开发接口[免费](http://baike.baidu.com/view/2763.htm)下载，也可依个人需求自己修改。
2、使用低价格的微处理[控制器](http://baike.baidu.com/view/122229.htm)(AVR系列控制器)，可以采用[USB接口](http://baike.baidu.com/view/26083.htm)供电，不需外接电源，也可以使用外部9VDC输入。
3、Arduino支持ISP在线烧，可以将新的“bootloader”[固件](http://baike.baidu.com/view/33738.htm)烧入AVR芯片。有了bootloader之后，可以通过[串口](http://baike.baidu.com/view/69108.htm)或者USB
 to Rs232线更新[固件](http://baike.baidu.com/view/33738.htm)。
4、可依据官方提供的Eagle格式PCB和SCH电路图简化Arduino模组，完成独立运作的微处理控制；可简单地与传感器，各式各样的电子元件连接(例如：红外线,超音波,热敏电阻,光敏电阻,伺服马达,…等)
5、支持多种互动程序，如：Flash、Max/Msp、[VVVV](http://baike.baidu.com/view/777692.htm)、PD、C、Processing等。
6、应用方面，利用Arduino，突破以往只能使用[鼠标](http://baike.baidu.com/view/2199.htm)、[键盘](http://baike.baidu.com/view/7402.htm)、CCD等输入的装置的互动内容，可以更简单地达成单人或多人游戏互动。
## 3、功能[](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#)
您可以快速使用Arduino与Adobe Flash, Processing, Max/MSP, Pure Data, SuperCollider等软件结合，作出互动作品。 Arduino可以使用现有的[电子元件](http://baike.baidu.com/view/44052.htm)例如开关或者传感器或者其他控制器件、[LED](http://baike.baidu.com/view/52538.htm)、步进马达或其他输出装置。
 Arduino也可以独立运行，并与软件进行交互，例如： Macromedia Flash, Processing, Max/MSP, Pure Data, VVVV或其他互动软件…。 Arduino的IDE界面基于开放源代码，可以让您免费下载使用，开发出更多令人惊艳的互动作品。
*基于[知识共享](http://baike.baidu.com/view/89040.htm)开放原始码的电路图设计
*基于[知识共享](http://baike.baidu.com/view/89040.htm)开放原始码的程式开发环境
*免费下载，也可依需求自己修改!!遵照姓名标示。您必须按照作者或授权人所指定的方式，表彰其姓名
*依相同方式分享，若您改变、转变著作，当散布该衍生著作时，您需采用与本著作相同或类似的授权条款
* Arduino可使用[ICSP](http://baike.baidu.com/view/2971938.htm)线上烧入器，将「bootloader」烧入新的[IC](http://baike.baidu.com/view/16703.htm)晶片
*可依据官方电路图，简化Arduino模组，完成独立运作的微处理控制
*可简单地与传感器，各式各样的电子元件连接（例如：红外线，超声波，热敏电阻，光敏电阻，伺服马达等）
*支援多样的互动程序例如: Adobe Flash、Max/Msp、VVVV、PD、C、Processing等
*使用低价格的微处理控制器(ATMEGA 8-168)
* [USB](http://baike.baidu.com/view/854.htm)接口，不需外接电源。另外有提供9V直流电源输入
*应用方面，利用Arduino，突破以往只能使用[鼠标](http://baike.baidu.com/view/2199.htm)，[键盘](http://baike.baidu.com/view/7402.htm)，CCD等输入的装置进行互动内容的设计，可以更简单地达成单人或多人游戏互动。[1]
## 4、发展历史[](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#)
据说Massimo Banzi之前是意大利Ivrea一家高科技设计学校的老师。他的学生们经常抱怨找不到便宜好用的微控制器。 2005年冬天， Massimo Banzi跟David Cuartielles讨论了这个问题。 David Cuartielles是一个西班牙籍晶片工程师，当时在这所学校做访问学者。两人决定设计自己的电路板，并引入了Banzi的学生David Mellis为电路板设计编程语言。两天以后，David Mellis就写出了程式码。又过了三天，电路板就完工了。这块电路板被命名为Arduino。几乎任何人，即使不懂电脑编程，也能用Arduino做出
 很酷的东西，比如对感测器作出回应，闪烁灯光，还能控制马达。随后Banzi，Cuartielles，和Mellis把设计图放到了网上。保持设计的开 放源码理念，因为版权法可以监管开源软体，却很难用在硬体上，他们决定采用Creative Commons许可。 Creative Commons（CC）是为保护开放版权行为而出现的类似GPL的一种许可（license）。在Creative Commons许可下，任何人都被允许生产电路板的复制品，还能重新设计，甚至销售原设计的复制品。你不需要付版税，甚至不用取得Arduino团队的许
 可。然而，如果你重新发布了引用设计，你必须说明原始Arduino团队的贡献。如果你调整或改动了电路板，你的最新设计必须使用相同或类似的 Creative Commons许可，以保证新版本的Arduino电路板也会一样的自由和开放。唯一被保留的只有Arduino这个名字。它被注册成了商标。如果有人想 用这个名字卖电路板，那他们可能必须付一点[商标](http://baike.baidu.com/view/37392.htm)费用给Arduino的[核心](http://baike.baidu.com/view/22680.htm)开发团队成员。[1]
## 5、中文翻译[](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#)
互动式逻辑
## 6、开发团队[](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#)
Arduino的核心开发团队成员包括：Massimo Banzi，David Cuartielles，Tom Igoe，Gianluca Martino，David Mellis和Nicholas Zambetti。[1]
## 7、外界评价[](http://baike.baidu.com/link?url=3NujfCpDw6lSyasxgyuxgN26Ha17YLX7RqzuX3WrPJE880as45G-b8zVRewAGK6fZvZT3ru9XVs9mi7HDyZDza#)
自从读过关于Arduino的书籍后，我彻底地爱上Arduino了！它的自由性，互交性以及它的易用性深深的吸引了我。
它没有复杂的[单片机](http://baike.baidu.com/view/1012.htm)底层代码，没有难懂的汇编，只是简单而实用的函数。它必将引领一个新的时代！
Arduino，丰富的接口，简便的[编程环境](http://baike.baidu.com/view/1405990.htm)，极大的自由度，可拓展性能非常高！标准化的接口模式为它的可持续发展奠定了坚实的[基础](http://baike.baidu.com/view/123417.htm)。
——锋影

[http://arduino.cc/en/Main/Software](http://arduino.cc/en/Main/Software)大家可以去这个Arduino官网下载
