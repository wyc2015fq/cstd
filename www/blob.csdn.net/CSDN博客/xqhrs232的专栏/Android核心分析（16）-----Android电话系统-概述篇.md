# Android核心分析（16）-----Android电话系统-概述篇 - xqhrs232的专栏 - CSDN博客
2011年03月31日 09:59:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：687标签：[电话																[android																[多媒体系统																[框架																[手机																[service](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=多媒体系统&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=电话&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/maxleng/archive/2010/05/10/5576509.aspx](http://blog.csdn.net/maxleng/archive/2010/05/10/5576509.aspx)
Android电话系统之概述篇
    首先抛开Android的一切概念来研究一下电话系统的最基本的描述。我们的手机首先用来打电话的，随后是需要一个电话本，随后是PIM，随后是网络应用，随后是云计算，随后是想我们的手机无所不能，替代PC。但是作为一个电话的基本功能如下：
0)拨叫电话,接听电话，挂断电话,发送短信，网络连接,PIM管理
1)由于电话运营商为我们提供了呼叫等待，电话会议等补充业务，所以我们的手机需要管理多路通话，如何管理?
2)来电时，我们要播出来电铃声，接通时我们需要切换语音通道，这个又跟多媒体系统打上了交道，例如有耳机插上了，有蓝牙耳机连上了，系统该做如何的管理和切换？
3）上网的网络通路建立（例如GSM GPRS），如何PPP连接并连接到LinuxSocket通道上的？系统如何管理数据连接？
4）AP跟Modem通讯时通过AT指令的，如何将AT指令变成一个个具体的操作函数，如何管理Modem发给我们的回应，AT命令通道，数据通道如何管理？
5）sim卡的电话本如何管理？
上面的关于手机的基本问题，Android电话系统设计者必须要解答的问题。该设计如何的管理框架，提出什么概念来表达？所以要分析Android的电话部分，还是需要理解电话实现的背景知识，通讯协议，大体框架。
    我们回到电话系统基本构成上，先从整体上去把握一下电话模块的大体框架，先从空中俯瞰。我给出的图是一般的智能手机的框架图，该框架基本能够概括所有手机电话模块的构成，当然也包括Android的电话系统构成。
    智能机架构一般是应用处理器+Modem。应用处理器与Modem的连接使用串口或者USB。在一个硬件串口通路上实现为了要同时实现数据传输并同时实现控制Modem,就需要实现多路复用协议（GSM TS07.10），在底层我们在多路复用的基础上虚拟了两个串口，一个用于CMD通道，一个用于DATA通道。电话的所有控制通路都是在这连个通道上。
   RIL,Radio Interface Layer。本层为一个协议转换层，手机框架需要适应多类型的Modem接入到系统中，而对于不同的Modem有不同的特性，AT指令的格式或者回应有所不同，但是这种特性在设计应用时不可能完全考虑和兼容。所以设计者在设计电话系统时，建立了一个虚拟电话系统，为该虚拟电话系统规定了标准的功能，上层的电话管理都是建立在这些标准的功能基础之上。而RIL则是将虚拟电话系统的标准功能转换成实际的所使用的Modem的AT指令。
Android设计者将电话系统设计成了三部分。
    Andoird的Phone Service其实是PhoneApp。GSMPhone（CDMAPhone)是Phone Service核心的对象，他包含了如下的相关对象。
       我们的分析任务就是要把这些对象的相互关系，及其对象间数据传递关系弄清楚。首先我们给出以下的Android电话系统的框架，以便对Android电话系统有个概要的认识，然后从数据流的角度，以及对象的引用关系来分析系统。下面是android电话系统整体框架图。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/maxleng/archive/2010/05/10/5576509.aspx](http://blog.csdn.net/maxleng/archive/2010/05/10/5576509.aspx)

