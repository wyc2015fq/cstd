# 关于苹果IOS相关的信息整理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月04日 11:42:53[boonya](https://me.csdn.net/boonya)阅读数：873








     笔者序：之所以要写这篇文章是因为对IOS产生了学习的兴趣，所以先了解其背景、发展和市场等等，然后进入IOS世界拥抱苹果乱啃。

#### 1.0IOS是什么？

**       IOS**是由苹果公司开发的移动操作系统。苹果公司最早于2007年1月9日的Macworld大会上公布这个系统，最初是设计给iPhone使用的，后来陆续套用到iPod touch、iPad以及Apple TV等产品上。iOS与苹果的Mac OS X操作系统一样，属于类Unix的商业操作系统。原本这个系统名为iPhone OS，因为iPad，iPhone，iPod touch都使用iPhone OS，所以2010WWDC大会上宣布改名为iOS（iOS为美国Cisco公司网络设备操作系统注册商标，苹果改名已获得Cisco公司授权）。目前苹果已经推出了Apple
 watch，进一步说明了其年轻发展的态势，虽然乔帮主已经仙逝，但是Apple却还在。

#### 2.0IOS发展历史

以下内容来至百度百科：[http://baike.baidu.com/link?url=4OUX5cErT66qGsbRgSt6eewzcTsdndubuuSdHYMDwU3JB4Hv9c2sYlH-Y1eL0o2nfP-uxH9OOAb1HWwP8NxNclzHX1CbAlyYHWt3UySfyze](http://baike.baidu.com/link?url=4OUX5cErT66qGsbRgSt6eewzcTsdndubuuSdHYMDwU3JB4Hv9c2sYlH-Y1eL0o2nfP-uxH9OOAb1HWwP8NxNclzHX1CbAlyYHWt3UySfyze)




2007年1月9日苹果公司在[Macworld](http://baike.baidu.com/view/824205.htm)展览会上公布，随后于同年的6月发布第一版iOS操作系统，最初的名称为“iPhone Runs OS X”。

2007年10月17日，[苹果公司](http://baike.baidu.com/view/15181.htm)发布了第一个本地化iPhone应用程序开发包（[SDK](http://baike.baidu.com/view/429424.htm)），并且计划在2月发送到每个开发者以及开发商手中。

2008年3月6日，苹果发布了第一个测试版开发包，并且将“iPhone runs OS X”改名为“[iPhone OS](http://baike.baidu.com/view/2481813.htm)”。

2008年9月，苹果公司将[iPod touch](http://baike.baidu.com/view/1143407.htm)的系统也换成了”[iPhone OS](http://baike.baidu.com/view/2481813.htm)“。

2010年2月27日，苹果公司发布[iPad](http://baike.baidu.com/view/2761768.htm)，iPad同样搭载了”iPhone OS”。这年，苹果公司重新设计了“iPhone OS”的系统结构和自带程序。

2010年6月，苹果公司将“iPhone OS”改名为“iOS”，同时还获得了[思科](http://baike.baidu.com/view/23860.htm)iOS的名称授权。

2010年第四季度，苹果公司的iOS占据了全球[智能手机操作系统](http://baike.baidu.com/view/1047304.htm)26%的市场份额。

2011年10月4日，苹果公司宣布iOS平台的[应用程序](http://baike.baidu.com/view/330120.htm)已经突破50万个。

2012年2月，应用总量达到552,247个，其中游戏应用最多，达到95,324个，比重为17.26%;书籍类以60,604个排在第二，比重为10.97%;娱乐应用排在第三，总量为56,998个，比重为10.32%。

2012年6月，苹果公司在WWDC 2012上宣布了[iOS 6](http://baike.baidu.com/view/8032273.htm)，提供了超过 200 项新功能。

2013年6月10日，苹果公司在WWDC 2013上发布了[iOS 7](http://baike.baidu.com/view/9365469.htm)，几乎重绘了所有的系统[APP](http://baike.baidu.com/subview/132427/8069631.htm)，去掉了所有的仿实物化，整体设计风格转为扁平化设计。将于2013年秋正式开放下载更新。[3]

2013年9月10日，苹果公司在2013秋季新品发布会上正式提供[iOS 7](http://baike.baidu.com/view/9365469.htm)下载更新。

2014年6月3日（西八区时间2014年6月2日），苹果公司在WWDC 2014上发布了[iOS 8](http://baike.baidu.com/view/10060934.htm)，并提供了开发者预览版更新。[4]


##### 2.1Java


苹果至今仍没有宣布任何让[iPhone](http://baike.baidu.com/view/710887.htm)运行[Java](http://baike.baidu.com/view/29.htm)的计划。但太阳微系统已宣布其将会发布能在iPhone上运行的[Java虚拟机](http://baike.baidu.com/view/374952.htm)([JVM](http://baike.baidu.com/view/160708.htm))的计划，它是基于Java的Micro
 Edition版本。这将让用Java应用程序得以在iPhone和iPod Touch上运行。在这个计划发表之后,熟悉iOS软件开发协议的程序员们相信虽然iOS软件开发协议不允许应用程序后台运行(比如说在接电话的时候仍然运行程序), 但却允许自带的应用程序从其他的来源下载[代码](http://baike.baidu.com/view/41.htm)，而且它们还能与[第三方应用程序](http://baike.baidu.com/view/5755965.htm)相互作用(比如说[Safari](http://baike.baidu.com/subview/110484/5036395.htm)和[Java](http://baike.baidu.com/subview/29/12654100.htm)虚拟机),
 这可能会阻碍不与苹果合作的Java虚拟机的发展。 很明显，在iPhone运行的[Java](http://baike.baidu.com/view/29.htm)在iOS软件开发协议所规定的范畴以外。


##### **2.2Flash**


iOS不支持[Flash](http://baike.baidu.com/view/7641.htm)。[Adobe](http://baike.baidu.com/view/7578.htm)计划在iPhone发行第三方[Flash
 Lite](http://baike.baidu.com/view/820295.htm)软件程序，但现时仍未有计划。以现时来说，只支持标准Flash。现时可以使用iPhone第三方程序jailbreak观看flash动画。但不支持在线播放。


##### **2.3Swift**


Swift，[苹果](http://baike.baidu.com/view/1331.htm)于2014年[WWDC](http://baike.baidu.com/view/404495.htm)（苹果开发者大会）发布的新开发语言，可与Objective-C共同运行于Mac OS和iOS平台，用于搭建基于苹果平台的应用程序。

Swift是苹果公司在WWDC2014上发布的全新开发语言。从演示视频及随后在appstore上线的标准文档看来，语法内容混合了OC,JS,Python，语法简单，使用方便，并可与OC混合使用。作为一项苹果独立发布的支持型开发语言，已经有了数个应用演示及合作开发公司的测试，相信将在未来得到更广泛的应用。某种意义上Swift作为苹果的新商业战略，将吸引更多的开发者入门，从而增强[App
 Store](http://baike.baidu.com/view/2771827.htm)和Mac Store本来就已经实力雄厚的应用数量基础。




#### 3.0软件开发语言和环境






软件平台：iOS



    开发商： Apple - 苹果



软件版本：iOS8.1





软件语言：Objective-C、C、C++



软件大小：1.2G以上



软件授权：仅苹果移动设备使用

开发工具(IDE)：Xcode







**关于更多IOS的信息后面在了解，对于开发而言知道IOS是什么，然后知道其开发语言、工具，接下来就可以开始自学了。**







