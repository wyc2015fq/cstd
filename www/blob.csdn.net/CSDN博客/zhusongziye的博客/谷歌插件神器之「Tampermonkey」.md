# 谷歌插件神器之「Tampermonkey」 - zhusongziye的博客 - CSDN博客





2018年12月26日 22:05:56[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：2160标签：[谷歌插件神器之「Tampermonkey」](https://so.csdn.net/so/search/s.do?q=谷歌插件神器之「Tampermonkey」&t=blog)
个人分类：[软件测试其它实用小工具](https://blog.csdn.net/zhusongziye/article/category/7144600)









之前我给大家推荐了 3 个非常好用的谷歌插件「[推荐 3 个非常好用的谷歌插件](http://mp.weixin.qq.com/s?__biz=MzIxMzgyOTg1MQ==&mid=2247485515&idx=1&sn=3c4eb6fb0e8e027e53ed2895dd1ddb1b&chksm=97b199ffa0c610e9b53b5d75a700bcc7f1b6e31af320d12991ed61d7cbb8367ae7cc8576d64d&scene=21#wechat_redirect)」，得到了大家不错的反响。其中我文末提到了一个叫「Tampermonkey」的插件，俗称「油猴」。但在评论区有好多人说不懂怎么使用，今天我就写篇文章手把手教大家如何使用这个神器。



「油猴」绝对是插件中的王者，因为你通过「油猴」就相当于拥有其他绝大多数的插件。「油猴」就像一个脚本提供商，你在它上面可以找到海量的插件脚本，通过「油猴」安装过后就可以直接使用。



「油猴」上有非常多好用的插件，比如



1. 直接下载百度网盘文件(全速)

2. 重新定制繁杂的微博页面

3. 去掉视频播放广告(甚至播放vip视频)

4. 将网站默认的「二维码登录」改回「账号密码登录」

5. 绕过搜索引擎的跳转提示

6. 还原清新的小说阅读模式



**Tampermonkey 安装**



「油猴」安装方式很简单，首先你需要有谷歌浏览器，其次去谷歌网上应用店搜索「Tampermonkey」点击添加至 Chrome 即可安装，但这需要大家可以科学上网。



![](https://img-blog.csdnimg.cn/20181226220007675.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



考虑到有些同学还不会科学上网，这里给大家提供另一个网站「Crx4Chrome」（https://www.crx4chrome.com/）也可以直接下载「Tampermonkey」插件。



![](https://img-blog.csdnimg.cn/20181226220037332.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



把「Tampermonkey」插件下载到本地之后，再打开 Chrome 浏览器「扩展程序」页面，将下载的 crx 文件拖拽到页面即可完成安装。



**Tampermonkey 使用**



安装成功之后在 Chrome  右上角就会有个「油猴」的小图标。



![](https://img-blog.csdnimg.cn/20181226220049779.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



点击图标你就可以看到官方提供的一些文档，一些常见的和不常见的问题都可以在这里找到。



![](https://img-blog.csdnimg.cn/20181226220229574.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



「油猴」默认是没有安装任何脚本，所以如果你是第一次使用有可能会觉得一脸懵逼，别人都说「油猴」是神器怎么我下载之后跟呆瓜一样什么功能也没有。「油猴」的强大之处就在这里，你可以通过点击「获取新的脚本」来下载各种脚本，打造出自己的专属「油猴」，



比较常用的下载渠道有三个：

**1 Greasy Fork**

**https://greasyfork.org/zh-CN**

![](https://img-blog.csdnimg.cn/2018122622024775.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**2 OpenUserJS  **

**https://openuserjs.org/**

![](https://img-blog.csdnimg.cn/20181226220304626.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**3 Userscripts Mirror **



**https://userscripts-mirror.org/**



![](https://img-blog.csdnimg.cn/20181226220321426.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



Greasy Fork 支持中文，按照今日安装、总安装数、得分、创建日期等排序方式给出脚本列表，可按脚本生效的网站过滤，每一脚本都有中文介绍，并且详细列出了作者、安装数、更新日期日志、安装使用截图、兼容性、应用到、代码查看等信息描述。



而 OpenUserJS 和 Userscripts Mirror 都只有英文界面，并且 Userscripts Mirror 已经停止更新，但用户还是可以在这个站点上找到历史资源。



所以建议大家去 Greasy Fork 上找脚本资源。找到需要的脚本后，会在介绍页面看到安装（install）按钮，点击下载脚本后会自动跳转到安装界面，再点击安装就可以使用脚本了。



下面我以「百度网盘直接下载助手」为例。



**百度网盘直接下载助手**



进入到 Greasy Fork 直接搜索「百度网盘」，就可以看到很多可以直接下载的脚本。



![](https://img-blog.csdnimg.cn/20181226220339424.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



我们点击第一个链接，然后再点击「安装此脚本」，即可安装。



![](https://img-blog.csdnimg.cn/20181226220351639.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



安装完之后是没有任何提示，我们可以去百度网盘页面看下脚本是否有效。



![](https://img-blog.csdnimg.cn/20181226220359581.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



进入百度网盘页面随意点击一个文件，在上方你就可以看到多出一个「下载助手」，「油猴」也会多了个红点显示现在有一个脚本正在运行，这时候大家就可以愉快的无限速下载百度网盘的资源了。



**好用的脚本推荐**



在上面说到的 3 个脚本下载网站中有非常多好用的脚本，大家可以多去逛逛。在这里我也给大家推荐一些，我认为比较好用的一些脚本。



**1 GitHub 中文化界面**



https://openuserjs.org/scripts/52cik/GitHub_汉化插件



![](https://img-blog.csdnimg.cn/20181226220409806.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



很多新手朋友不太会玩 GitHub，可能被全英文界面所困扰，这款脚本实现汉化了 GitHub 界面的部分菜单及内容，新手熟悉之后可选择停用脚本恢复英文模式。



**2 Anti Redirect**



https://greasyfork.org/zh-CN/scripts/11915-anti-redirect



![](https://img-blog.csdnimg.cn/20181226220422604.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



这个脚本可以让你在各大平台搜索出的结果是真实的地址，即去除各搜索引擎/常用网站出站链接重定向。那这个是什么意思？通俗的讲，你安装了这个脚本在用百度搜索 Python 时就不会出现全面培训机构的网站，非常好用的脚本。作者也把脚本的源代码分享到 GitHub 上，感兴趣的同学可以去研究一下。



**3 iqiyi-player-switch**



![](https://img-blog.csdnimg.cn/20181226220435425.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



爱奇艺视频广告去除，不多说只需要喊「666」就好。



**4 一键离线下载**



![](https://img-blog.csdnimg.cn/20181226220447462.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



种子神器，一键自动将磁链、bt 种子或其他下载资源离线下载至网盘。





文章参考链接：https://sspai.com/post/40485



PS:

同类型另一个神器： Violentmonkey 中文名：暴力猴 Violentmonkey比Tampermonkey更为简洁，脚本管理更加的方便，暴力猴插件是一款可以支持UserScript用户脚本的浏览器扩展程序。 支持Userscripts.org、GreasyFork、OpenUserJS三大脚本下载源。



