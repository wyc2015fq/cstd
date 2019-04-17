# Android SDK代理服务器解决国内不能更新下载问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年08月22日 11:23:55[boonya](https://me.csdn.net/boonya)阅读数：159952标签：[代理服务器																[android																[sdk																[解决方案																[局域网](https://so.csdn.net/so/search/s.do?q=局域网&t=blog)
个人分类：[Android](https://blog.csdn.net/boonya/article/category/2496125)





读者须知：本篇文章中最靠谱的是第三种方式，最近有读者反映第三种方式也不行了，下面提供一点其他途径的开源镜像站点：



### 国内高校的开源镜像站
- 中国科学技术大学([debian.ustc.edu.cn](http://debian.ustc.edu.cn/))
- 上海交通大学([ftp.stju.edu.cn](http://ftp.stju.edu.cn/))
- 大连理工大学([mirror.dlut.edu.cn](http://mirror.dlut.edu.cn/))
- 北京交通大学([mirror.bjtu.edu.cn](http://mirror.bjtu.edu.cn/))
- 北京理工大学([mirror.bit.edu.cn](http://mirror.bit.edu.cn/))
- 东北大学([mirror.neu.edu.cn](http://mirror.neu.edu.cn/))
- 厦门大学([mirrors.xmu.edu.cn](http://mirrors.xmu.edu.cn/))
- 兰州大学([mirror.lzu.edu.cn](http://mirror.lzu.edu.cn/))
- 西安电子科技大学([linux.xidian.edu.cn](http://linux.xidian.edu.cn/))
- 哈尔滨工业大学([run.hit.edu.cn](http://run.hit.edu.cn/))
- 天津大学([mirror.tju.edu.cn](http://mirror.tju.edu.cn/))
- 东软信息学院([mirrors.neusoft.edu.cn](http://mirrors.neusoft.edu.cn/))
- 电子科技大学([mirrors.stuhome.net](http://mirrors.stuhome.net/))

### 国内其他机构的开源镜像站
- 中国互联网信息中心([mirrors.cnnic.cn](http://mirrors.cnnic.cn/))  只提供了Apache镜像
- 网易开源镜像站([mirrors.163.com](http://mirrors.163.com/))
- 搜狐([mirrors.sohu.com](http://mirrors.sohu.com/))
- 淘宝([ruby.taobao.org](http://ruby.taobao.org/))
- 猫扑([labs.mop.com/apache-mirror](http://labs.mop.com/apache-mirror))
 只提供了Apache镜像

### 日本各大学的镜像站
- 北陆先端科学技术大学院大学JAIST([http://ftp.jaist.ac.jp/pub/](http://ftp.jaist.ac.jp/pub/))

### 美国各大学的镜像站
- 卡内基梅隆大学CMU([http://www.club.cc.cmu.edu/pub](http://www.club.cc.cmu.edu/pub))
- 麻省理工学院MIT([http://mirrors.mit.edu/](http://mirrors.mit.edu/))
- 哥伦比亚大学([http://mirror.cc.columbia.edu/](http://mirror.cc.columbia.edu/))
- 俄勒冈州立大学([http://ftp.osuosl.org/pub](http://ftp.osuosl.org/pub))
- 伊利诺伊大学厄巴纳－香槟分校([http://cosmos.cites.illinois.edu/](http://cosmos.cites.illinois.edu/))
- 杜克大学([http://archive.linux.duke.edu/](http://archive.linux.duke.edu/))
- 约翰·霍普金斯大学 [http://mirrors.acm.jhu.edu/](http://mirrors.acm.jhu.edu/)



更多信息请查看：[http://www.oschina.net/news/38492/mirrors-tsinghua-will-close](http://www.oschina.net/news/38492/mirrors-tsinghua-will-close)

临时推荐几个国内的镜像：

大连东软信息学院镜像服务器地址:http://mirrors.neusoft.edu.cn 端口：80 （此处可以参考：[http://mirrors.neusoft.edu.cn/more.we](http://mirrors.neusoft.edu.cn/more.we)给出的配置，即之前本篇文章推荐的第三种方式）


北京化工大学镜像服务器地址:IPv4: http://ubuntu.buct.edu.cn/ 端口：80

        IPv4: http://ubuntu.buct.cn/ 端口：80

       IPv6: http://ubuntu.buct6.edu.cn/ 端口：80

上海GDG镜像服务器地址:http://sdk.gdgshanghai.com 端口：8000


下面是原本篇博客的内容，如有兴趣可以一看：


** 前言：Android SDK代理服务器解决国内Android SDK不能更新下载问题，经常会遇到Fitch fail URL错误，要不就是Nothing was installed。目下Google遭受在中国用户史上的严冬（堪比The ice age），余花了三天时间去搞这个代理问题，但是收效甚微，因为许多代理都不起作用了。写此篇博客的目的是：一方面是为了向大家征求最新的解决方案，另一方面是为了让更多的Android的开发者把时间用到业务和技术实现上去而不是纠结于环境搭建。下面介绍下我用过的代理方式：**


#### 1.方式一：无界代理

      此款代理软件不错，支持本地代理、局域网代理和广域网代理。如不能直接代理可使用经典代理模式。

![](https://img-blog.csdn.net/20140822110732669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


设置：在SDK Manager 界面Tools>Options...下设置代理服务器的IP（如：127.0.0.1(本地代理)）和代理服务器的端口（如：80（服务器默认端口，这里是9666））。

#### 2. 方式二：GoAgent代理

    此款代理工具目前在国内使用还是很广泛的，但是最近不是很方便了，经常是找不到代理数据源，如果上Google玩有时候直接断开连接，而且后台很多连接找不到并且有警告产生（警告是小事 问题是找不到代理网站的数据不能进行正常的访问）。

![](https://img-blog.csdn.net/20140822111505881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


设置略。

#### 3. 方式三：mirrors.neusoft.edu.cn镜像代理

启动SDK Manager界面Tools>Options...下进行如下操作：

![](https://img-blog.csdn.net/20140822112024109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


体验效果图：

![](https://img-blog.csdn.net/20140822112316783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：1和2代理感觉不怎么好用，3镜像代理的方式感觉比上面两种要好很多。以上三种代理的设置方式都是一样的，只是第三种用的是界面的直观表达。](https://so.csdn.net/so/search/s.do?q=解决方案&t=blog)](https://so.csdn.net/so/search/s.do?q=sdk&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=代理服务器&t=blog)




