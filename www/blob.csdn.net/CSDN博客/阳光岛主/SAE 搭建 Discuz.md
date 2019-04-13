
# SAE 搭建 Discuz - 阳光岛主 - CSDN博客

2013年10月04日 16:16:41[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：10188个人分类：[Cloud																](https://blog.csdn.net/sunboy_2050/article/category/1398664)



**[SAE](http://sae.sina.com.cn/)**（Sina App Engine）是新浪研发中心于2009年8月开始内部开发，并在2009年11月3日正式推出第一个Alpha版本的国内首个公有App Engine，SAE是新浪云计算战略的核心组成部分。

**SAE 功能**
SAE作为国内的公有云计算，从开发伊始借鉴吸纳Google、Amazon等国外公司的公有云计算的成功技术经验，并很快推出有别于国外公司并具有自身特色的App Engine。SAE选择在国内流行最广的Web开发语言PHP作为首选的支持语言，Web开发者可以在Linux/Mac/Windows上通过SVN或者Web版在线代码编辑器进行开发、部署、调试，团队开发时还可以进行成员协作，不同的角色将对代码、项目拥有不同的权限；SAE提供了一系列分布式计算、存储服务供开发者使用，包括分布式文件存储（Storage）、分布式数据库集群（MySQL）、分布式缓存（Memcache）、分布式定时服务（Cron）等，这些服务将大大降低开发者的开发成本。同时又由于SAE整体架构的高可靠性和新浪的品牌保证，大大降低了开发者的运营风险。另外，作为典型的云计算，SAE采用“所付即所用，所付仅所用”的计费理念，通过日志和统计中心精确的计算每个应用的资源消耗（包括CPU、内存、磁盘等）。
![](https://img-blog.csdn.net/20131008192328125)

本文将讲解，在新浪SAE云服务上，搭建属于自己的Discuz x2.0论坛，并对网上过时、错误、模糊的教程进行了修正和扩展。
本教程搭建的论坛实例：[http://forum.ithomer.net](http://forum.ithomer.net)（原域名：[http://iforum.sinaapp.com](http://iforum.sinaapp.com/)，下同）


SAE是国内首个公有云计算平台，也是国内第一件云计算平台，具有简单，高效，快捷等一系列优点，使用Sina App Engine，不用租服务器就可以做各种应用、网站，不需要花一分钱，今天教大家的就是利用新浪SAE云空间免费搭建一个属于自己的Discuz x2.0论坛。

**1、SAE账号****注册**
注册网址：[http://weibo.com/signup/signup.php](http://weibo.com/signup/signup.php)
新浪微博和SAE账号互通，利用新浪微博账号即可登录SAE，注册时需手机号验证

**2、Discuz 下载**
由于SAE不支持IO操作，上传的代码目录没有写入权限，因此无法安装Discuz原生版本（需在服务器上修改部分安装目录的写入权限），所以只能安装专门针对SAE移植的Discuz版本。
Discuz4SAE移植版下载地址：[https://code.google.com/p/discuzx4sae/downloads/list](https://code.google.com/p/discuzx4sae/downloads/list)
下载最新版本：[Discuz!X 2.0 For SAE.zip](https://discuzx4sae.googlecode.com/files/Discuz%21X%202.0%20For%20SAE.zip)

**3、SAE 创建应用**
1） 登陆进入SAE首页后，进入“我的首页”——》”创建新应用“——》如下图填写应用名称等信息（iforum我已创建，因此提示已经被占用）
![](https://img-blog.csdn.net/20131004150920078)

2） 点击”创建应用“按钮后，第一次登陆会弹出“安全认证”，如下图
![](https://img-blog.csdn.net/20131004151507765)
如果没有开通微盾，则”安全密码“即为你登陆SAE账号的密码；如果开通了微盾，则”安全密码“为你SAE账号密码+微盾动态密码（6位数字），在你手机上一分钟动态生成1次

**4、 初始化准备工作**
创建完iforum应用后，点击iforum，进入一些初始化工作，主要有MySQL、Memcache、Storage，具体步骤如下：
1）MySQL 初始化
进入”iforum“应用后，依次点击“服务管理”——》MySQL，如下图
![](https://img-blog.csdn.net/20131004152513484)
点击“点此初始化Mysql”，完成后如下图：
![](https://img-blog.csdn.net/20131004152829312)

2） Memcache 初始化
进入”iforum“应用后，依次点击“服务管理”——》Memcache，如下图
![](https://img-blog.csdn.net/20131004153016921)
点击“点此初始化MC”，输入“10”M大小，提交

3） Storage 初始化
进入”iforum“应用后，依次点击“服务管理”——》Storage，如下图
![](https://img-blog.csdn.net/20131004153314812)
点击“点此新建一个domain”，配置参数，如下图：
![](https://img-blog.csdn.net/20131004154106375)
如上图注释，这里有几点需要特别注意：
1） Domain名称设置为“discuzx”，因为移植版的discuzx4sae默认为“discuzx”，后面会介绍，这里一定设置为“**discuzx**”
2） 不得勾选“私有”，否则外面的http请求无法访问到资源
3） 缓存器和防盗链，根据自己情况设置

**5、上传Discuzx代码**
1） 进入”iforum“应用后，依次点击“应用管理”——》“代码管理”，如下图
![](https://img-blog.csdn.net/20131004154614031)

2） 点击”上传代码包“——》”选择文件“——》上传步骤2下载的zip代码，如下图：
![](https://img-blog.csdn.net/20131004154818421)

**6、 Discuzx 安装**
1）上传Discuzx代码完成后，点击iforum链接，在浏览器输入：[http://1.iforum.sinaapp.com/install/](http://1.iforum.sinaapp.com/install/)
![](https://img-blog.csdn.net/20131004155147875)

2） 点击“我同意”，继续
![](https://img-blog.csdn.net/20131004155328000)

3） 不要管当前状态的提示，直接点击“下一步”，继续
![](https://img-blog.csdn.net/20131004155442437)

4）如上图，选择“全新安装Discuz！（含UCenter Server），点击”下一步“，继续
![](https://img-blog.csdn.net/20131004155950203)

5）如上图，数据库前缀，根据自己爱好修改； 系统邮箱和管理员邮箱，修改成自己邮箱； 管理员密码，一定要设置，登陆管理discuz论坛时使用，点击”下一步“，继续
![](https://img-blog.csdn.net/20131004160233500)

6） 如上图，数据库安装完成后，会自动跳转到管理界面：[http://1.iforum.sinaapp.com/admin.php](http://1.iforum.sinaapp.com/admin.php)
![](https://img-blog.csdn.net/20131004160500687)

7） 如上图，输入管理员账户和密码，登陆管理页面，如下图：
![](https://img-blog.csdn.net/20131004160713046)

8） 如上图，提示删除安装目录”install“下的”index.php“页面，进入”应用管理“——》”代码管理“，如下图：
![](https://img-blog.csdn.net/20131004160859062)

9） 如上图，点击”编辑代码“，依次进入”iforum"——》“install”——》右键删除“index.php"
![](https://img-blog.csdn.net/20131004161146875)

10） 访问论坛首页：[http://1.iforum.sinaapp.com/index.php](http://1.iforum.sinaapp.com/index.php)
![](https://img-blog.csdn.net/20131004161639078)

11）如上图，发现论坛排版格式混乱，这是由于css引用路径不对，利用chrome浏览器，按F12调试，找出原因如下：
![](https://img-blog.csdn.net/20131004162247296)

12）如上图，css路径指向不对并找不到，并且js的路径也找不到，他们都是相对路径，这是因为移植版的discuzx把需要在服务器上修改写入权限的目录（资源），改放到了SAE的storage文件存储上了，解决办法是在浏览器输入：[http://1.iforum.sinaapp.com/uploadimg.php](http://1.iforum.sinaapp.com/uploadimg.php)
![](https://img-blog.csdn.net/20131004162825312)

13） 如上图，uploadimg.php上传静态资源到storage，回到storage查看验证一下，如下图：
![](https://img-blog.csdn.net/20131004163611703)

14）如上图，storage——》discuzx——》static目录下，存在了uploadimg.php上传的文件或目录。此时访问论坛首页：[http://1.iforum.sinaapp.com/](http://1.iforum.sinaapp.com/)
发现css排版还是不正确，这是由于discuzx使用了缓存的缘故，请见步骤11）红色方框内”data/cache/xxx"，解决办法就是清除缓存即可：
依次进入discuz论坛——》管理中心——》工具——》更新缓存——》勾选“数据缓存”和“模板缓存”——》确定，如下图：
![](https://img-blog.csdn.net/20131004164101375)

15）清除缓存后，再访问论坛首页，css样式正确：[http://1.iforum.sinaapp.com/](http://1.iforum.sinaapp.com/)
![](https://img-blog.csdn.net/20131004164426375)

至此，已经在 SAE 上，成功搭建起了属于自己的 Discuzx 论坛！！【本文原创，欢迎大家转载，请一定要注明出处（[IT-Homer](http://blog.csdn.net/ithomer)）】
**本教程搭建的论坛：**[http://forum.ithomer.net](http://forum.ithomer.net)


**参考推荐：**
[Discuz 样式修改定制](http://blog.csdn.net/ithomer/article/details/12298333)（推荐）
[新浪SAE上搭建自己的Discuz](http://blog.csdn.net/liuqiyao_01/article/details/8551097)
[新浪SAE免费空间搭建Discuz x2.0论坛新手教程](http://hi.baidu.com/gb107/item/192b13149980ca453a176e8d)
[新浪SAE免费空间上安装DiscuzX2.0教程和SAE空间绑定域名方法](http://www.freehao123.com/sae-discuzx2-0/)
[Linux 搭建 discuz 论坛](http://blog.csdn.net/ithomer/article/details/8922062)



