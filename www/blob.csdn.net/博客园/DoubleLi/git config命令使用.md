# git config命令使用 - DoubleLi - 博客园






**1. git config简介**



我们知道config是配置的意思，那么git config命令就是对git进行一些配置。而配置一般都是写在配置文件里面，那么git的配置文件在哪里呢？互动一下，先问下大家。

你们所知的git配置文件是放在哪里的？git有几个配置文件呢？是的，聪明的你，稍微查查资料就知道咯，git里面一共有3个配置文件，首先是：仓库级配置文

件：该文件位于当前仓库下，路径.git/，文件名为.gitconfig，这个配置中的设置只对当前所在仓库有效仓库级配置文件内容如下：

![](http://img.my.csdn.net/uploads/201212/09/1355039263_7395.JPG)

第二个是全局级配置文件：win7下面路径，在用户目录下，以个人的PC机为例，其路径为：C:\Users\zuoyu.ht，文件名为.gitconfig

全局级配置文件内容如下：

![](http://img.my.csdn.net/uploads/201212/09/1355039318_5934.JPG)



最后是系统级配置文件：本地git的安装目录下，以左御的git安装路径为例：D:\Program Files\Git\etc，文件名为：gitconfig，内容如下：

![](http://img.my.csdn.net/uploads/201212/09/1355039375_9010.JPG)

从三个图的对比中，我们可以发现，有些配置项的名称是一样的，比如“core”，在git中，每一项的配置名称又叫做“section”，

“section”下面的每一行对应着一个key和一个value。需要注意的是：系统级配置文件不同于另外两个配置文件，是因为他少了

一个点，因此用vim打开的时候，是没有高亮显示的。”core”这个section在每个配置文件中都有，那么它git最终会调用哪个配置

文件呢？这就要有一个先后顺序了。

**2. 配置文件如何生效的**

用一个我们大家都熟悉的例子来比较，就是css。一般来说，在一个页面中引入css文件的方法有三种，见：[http://www.cnblogs.com/hutaoer/archive/2010/12/07/1899105.html](http://www.cnblogs.com/hutaoer/archive/2010/12/07/1899105.html)，

通俗的讲就是行内样式“<div style=“width:200px”>”，内嵌样式“<style type=“text/css”>div{width:200px}</style>”

和外部样式“<link rel=“stylesheet” href=“div.css”>”。这三种样式的权重是行内>内嵌>外部。类似的，对于git来说，配置文件

的权重是仓库>全局>系统。Git会使用这一系列的配置文件来存储你定义的偏好，它首先会查找/etc/gitconfig文件（系统级），该

文件含有对系统上所有用户及他们所拥有的仓库都生效的配置值。接下来Git会查找每个用户的~/.gitconfig文件（全局级）。最后

Git会查找由用户定义的各个库中Git目录下的配置文件.git/config（仓库级），该文件中的值只对当前所属仓库有效。以上阐述的三

层配置从一般到特殊层层推进，如果定义的值有冲突，以后面层中定义的为准，例如：.git/config和/etc/gitconfig的较量中，

.git/config取得了胜利。虽然你也可以直接手动编辑这些配置文件，但是运行git config命令将会来得简单些。下面我们就来看看如

何利用git config命令来编辑配置各个级别的文件。

**   3. 用git config命令查看配置文件**

命令参数 –list, 简写 -l


格式：git config [–local|–global|–system] -l

查看仓库级的config，即.git/.config，命令：git config –local -l

查看全局级的config，即C:\Users\zuoyu.ht\.gitconfig，命令：git config –global -l

查看系统级的config，即D:\Program Files\Git\etc\gitconfig，命令：git config –system -l

查看当前生效的配置，命令：git config -l，这个时候会显示最终三个配置文件计算后的配置信息，如下图：

![](http://img.my.csdn.net/uploads/201212/09/1355039577_3573.JPG)

**   4. 使用git config命令编辑配置文件**

编辑的英文单词是什么，没错，edit

命令参数 –edit, 简写 -e

格式：git config [–local|–global|–system] -e

查看仓库级的config，即.git/.config，命令：git config –local -e，与–list参数不同的是，git config -e默认是编辑仓库级的配置文件。

查看全局级的config，即C:\Users\zuoyu.ht\.gitconfig，命令：git config –global -e

查看系统级的config，即D:\Program Files\Git\etc\gitconfig，命令：git config –system -e

   执行这个命令的时候，git会用配置文件中设定的编辑器打开配置文件。

**  5. 增加一个配置项**

参数 –add

格式: git config [–local|–global|–system] –add section.key value(默认是添加在local配置中)

注意add后面的section,key,value一项都不能少，否则添加失败。比如我们执行：git config –add cat.name tom

![](http://img.my.csdn.net/uploads/201212/09/1355039687_5535.JPG)

然后查看local中的配置：

![](http://img.my.csdn.net/uploads/201212/09/1355039702_1741.JPG)

注意增加一项配置而不赋值 git config –add cat.age，或者单单增加一个section,git config –add cat1，不会成功。

**6.获取一个配置项**

有时候，我们并不需要查看所有配置的值，而是查看某个配置项的值，怎么做呢？

命令参数 –get

格式：git config [–local|–global|–system] –get section.key(默认是获取local配置中内容)

我们先往global配置中写入一个cat.name=Tomcat的配置项，再使用git config –get cat.name看看得到的是什么

![](http://img.my.csdn.net/uploads/201212/09/1355039780_1439.JPG)

结果就是local中的cat.name=Tom，因此git config –get section.key 等价于git config –local –get section.key

如果获取一个section不存在的key值，不会返回任何值

如果获取一个不存在的section的key值，则会报错

![](http://img.my.csdn.net/uploads/201212/09/1355039813_8061.JPG)

**7.删除一个配置项**

命令参数 –unset

格式：git config [–local|–global|–system] –unset section.key

相信有了前两个命令的使用基础，大家举一反三就知道改怎么用了，来，我们试试删除local配置中的cat.name

![](http://img.my.csdn.net/uploads/201212/09/1355039883_4974.JPG)










