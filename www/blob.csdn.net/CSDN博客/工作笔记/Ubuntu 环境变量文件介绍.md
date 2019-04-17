# Ubuntu 环境变量文件介绍 - 工作笔记 - CSDN博客





2015年05月28日 09:34:04[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5396
个人分类：[移动开发](https://blog.csdn.net/App_12062011/article/category/1146520)









转自：http://blog.csdn.net/cscmaker/article/details/7261921


Linux中环境变量包括系统级和用户级，系统级的环境变量是每个登录到系统的用户都要读取的系统变量，而用户级的环境变量则是该用户使用系统时加载的环境变量。

所以管理环境变量的文件也分为系统级和用户级的，下面贴一个网上找到的讲的比较明白的文件介绍（略作修改）[1]：


系统级：

/etc/profile：该文件是用户登录时，操作系统定制用户环境时使用的第一个文件，应用于登录到系统的每一个用户。该文件一般是调用/etc/bash.bashrc文件。

/etc/bash.bashrc：系统级的bashrc文件。

/etc/environment:在登录时操作系统使用的第二个文件,系统在读取你自己的profile前,设置环境文件的环境变量。


用户级（这些文件处于家目录下）：

~/.profile:每个用户都可使用该文件输入专用于自己使用的shell信息,当用户登录时,该文件仅仅执行一次!默认情况下,他设置一些环境变量,执行用户的.bashrc文件。**这里是推荐放置个人设置的地方**

~/.bashrc:该文件包含专用于你的bash shell的bash信息,当登录时以及每次打开新的shell时,该该文件被读取。不推荐放到这儿，因为**每开一个shell，这个文件会读取一次**，效率肯定有影响。

~/.bash_profile or ~./bash_login：这里没有引用作者的，下面会提到

~/.pam_environment:用户级的环境变量设置文件，没有做测试，不知道管不管用。



通过上面的文件介绍，就能明白我们需要设置环境变量的时候，需要修改什么文件，要想对所有的用户生效，那就需要设置系统级的环境变量。反之，需要修改用户级的文件（最好是修改.profile文件，理由上面已经讲了）。



另外，我使用的是Ubuntu10.04，之前在查找资料设置用户级环境变量的时候，有资料说要修改~.bash_profile文件，但是在ubuntu10.04系统中是没有该文件。经查找，在ubuntu官网[2]上找到了这个文件的说明：

~/.bash_profile or ~./bash_login - If one of these file exist, bash executes it rather then "~/.profile" when it is started as a login shell. (Bash will prefer "~/.bash_profile" to "~/.bash_login"). However, these files won't influence a graphical session
 by default.

意思应该是说当这个文件存在时，在登录shell时会读取./bash_profile文件，而不是.profile文件。我认为该文件实现的目的跟.profile文件是一样的，当查找的资料中需要你修改.bash_profile文件，但却你没有该文件的时候，也可以修改.profile来完成实现。

[1][http://cab402.appspot.com/2010/05/1/UbuntuEnvironmentVariablesSet.html](http://cab402.appspot.com/2010/05/1/UbuntuEnvironmentVariablesSet.html)

[2][https://help.ubuntu.com/community/EnvironmentVariables](https://help.ubuntu.com/community/EnvironmentVariables)


以上仅为个人观点，如有错误请批评指正！感激涕零……







