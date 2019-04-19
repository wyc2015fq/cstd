# Intellij IDEA 的使用 - 零度的博客专栏 - CSDN博客
2016年08月08日 14:11:05[零度anngle](https://me.csdn.net/zmx729618)阅读数：634标签：[intellij idea](https://so.csdn.net/so/search/s.do?q=intellij idea&t=blog)
个人分类：[IDEA](https://blog.csdn.net/zmx729618/article/category/6354411)
前言： 
为什么我要选择intellij idea呢？原因有三 
1.以前公司的项目 在myeclipse10中不稳定 myeclipse说不定什么时候就崩溃卡死了  而intellij很稳定 
2.界面高端洋气 用我朋友的话说 看着跟搞苹果开发一样 
3.项目的支持上比较好 基本上web项目都可以导入进来  
比如mybatis的官方项目 是maven的 在myeclipse中就不能直接导入 
intellij支持了 目前基本上所有的项目  尤其是maven项目 
推荐一下黄勇的这篇《使用 IDEA + Maven + Git 快速开发
[Java Web](http://lib.csdn.net/base/13) 应用》
[http://my.oschina.net/huangyong/blog/175363](http://my.oschina.net/huangyong/blog/175363)
下面步入正题： 
# 1.黑色主题 中文乱码修改 
修改黑色主题 Settings->Appearences ->Themes->Darcula  系统提示重启加载主题
重启后Settings->Appearences  ->勾选Override default fonts by 
![](http://static.oschina.net/uploads/space/2013/1117/211301_8urh_140593.jpg)
 出现这种情况 不用害怕   
 只用随便选择一个乱码的   点击保存 中文汉字就出来了 
# **2.WEB项目的部署 以及自动编译**
Intellij中  web的程序 部署 不同于myeclipse和eclipse 
需要把项目打成一个Artifacts(成品包)的war包 然后在 配置的tomcat的Deployment中选择对应的war包 
具体的可以看下这个帖子 [http://my.oschina.net/tsl0922/blog/94621](http://my.oschina.net/tsl0922/blog/94621)
我要强调的是 非maven的项目  比如网上下载的eclipse或者myeclipse项目 
首选要检查项目设置  F4 或者 Ctrl+ALT+SHIFT+S 或者 快捷栏中 点项目设置 
![](http://static.oschina.net/uploads/space/2013/1117/215953_EwZ6_140593.jpg)
以JFinal项目为例子  导入项目后 打开项目设置 
![](http://static.oschina.net/uploads/space/2013/1117/220341_QEII_140593.jpg)
1.首先修改Modules  选择 SDK  也就是你系统上的JDK  然后把项目中的不支持红色的选项去掉
![](http://static.oschina.net/uploads/space/2013/1117/220704_tiCN_140593.jpg)
2.添加项目的类型 支持
 比如你这个项目 是Spring的项目 可以加上Spring  以及在对应Spring的配置中 手中选中spring的配置文件 
加上Web项目的支持 制定web.xml的路径  以为web的目录 
![](http://static.oschina.net/uploads/space/2013/1117/221138_nJBq_140593.jpg)
如果是maven项目  这些都不用管  但是非maven项目 因为目录结构不同  IDEA默认找的 项目下的web目录  
所以项目往往不能正确部署 或者部署不正确 都是这个地方的原因  
这个Modules  设置正确后  就可以创建Artifaces了  IDEA右下角也会提示你的Create Artifaces 
3.添加项目的lib依赖
![](http://static.oschina.net/uploads/space/2013/1117/223854_BGjU_140593.jpg)
需要选择项目中的 web-inf中的lib   默认是不会自动加进来的   
如果是maven项目 这个就不要管了  
4.选择Artifaces的exploded war包  （如果是之前点击右下角系统的提示创建 就不用这样了）
![](http://static.oschina.net/uploads/space/2013/1117/221614_6XNH_140593.jpg)
5修改项目的输出目录
![](http://static.oschina.net/uploads/space/2013/1117/221912_CWR3_140593.jpg)
修改项目的编译输出目录  最好改为 jfinal_demo_for_jsp\target\ROOT（或者项目名）
因为maven的编译目录就是target目录 所以我们最好也按照maven的规则 
6.项目 在服务器中的部署
![](http://static.oschina.net/uploads/space/2013/1117/222234_ZRmZ_140593.jpg)
选中我们的artifaces的war包  右边可以自定义项目的访问 名 
左下角是 我们的项目 在服务器第一次启动时 需要做的事情  1.编译java类 2.打包部署war
这两个项目编译运行过 就可以给两个去掉了 以后我们再启动项目 就不用在去编译打包部署了
7.项目的服务器配置
![](http://static.oschina.net/uploads/space/2013/1117/222940_iXL7_140593.jpg)
在服务器配置中 注意几个地方
1.start browser  项目启动后 自动打开浏览器 访问对应的连接 
2.on update action ：当发现更新时的操作   选择update classes and resources 
on frame deactivation: 当IDEA 切换时的操作 （比如缩下去 打开网页） 选择update classes and resources 
也就是 自动编译 自动部署的功能 很多人说Intellij IDEA 不能自动编译 就是这个这个地方没设置好
3.可以选择端口 一个是jvm的端口 一个是tomcat的端口 如果我们同时运行多个项目 可以选择修改端口 就能直接同时运行多个项目
4.项目运行后 这两个就不需要了  
或者我们是maven项目 直接可以在maven中 packages 打包 编译 这两个也都不需要了
然后启动项目  就会弹出我们的项目首页了 
总结一下 里面需要注意的几个地方 
1.modules里面 添加项目类型 以及配置文件是否指定正确
2.lib文件的添加
3.artifacts 选择exploded 的war包 选择编译目录
4.服务起中 配置 热部署 端口 等
# 3.多项目的同时部署
intellij中工作空间和项目的概念和 myeclipse稍微有点不同  这个大家可以查看相关资料 
之前@黄勇 大哥发的java框架对比  就是一个项目中 有多个modules 
因为黄勇的项目 都是maven项目 就没有过多需要修改配置的地方
当时modules和artifacts里面好像都没有修改  多个子项目 可以在tomcat的deployment 同时部署 
只用给每个项目 在右侧修改一下项目名字就OK了 
![](http://static.oschina.net/uploads/space/2013/1117/230506_w3yy_140593.jpg)
# 4.相关插件提高工作效率 
## 1.JRebel插件 实现热部署 
可以集成JRebel   实现热部署  安装Tasks插件 集成JIRA等任务管理
1.修改idea.properties   在intellij的安装目录  bin\目录下   
![](http://static.oschina.net/uploads/space/2013/1118/205035_qE23_140593.jpg)
修改到你的非C盘目录目录下  默认使用的 # idea.config.path=${user.home}/.IntelliJIdea/ 
这样的好处是 当你更换系统时 intellij的配置和插件都在  可以直接继续使用
2.安装JRebel插件
Settings - Plugins - Browse repositories...  搜索JRebel   然后右键 Download and Install
里面其实有很多插件 具体插件的功能 可以去官网进行查看 [http://plugins.jetbrains.com/?idea](http://plugins.jetbrains.com/?idea)
插件会下载 安装在 E:\.IntelliJIdea\config\plugins  也就是刚才配置的 config\plugins下 
3.JRebel 破解  需要替换E:\.IntelliJIdea\config\plugins\jr-ide-idea\lib\jrebel  下的jrebel.jar
以及在intellij中 指定对应的lrc文件 
![](http://static.oschina.net/uploads/space/2013/1118/212325_kf2a_140593.jpg)
![](http://static.oschina.net/uploads/space/2013/1118/212353_MbXj_140593.jpg)
然后重启下intellij   JRebel就可以使用了  热部署效果非常的好 
修改东西都不用重启 而且项目在长期运行中也不会出现崩溃  （以前热部署遇到过session崩溃的问题） 
如果是maven项目 修改配置文件也都不用重启 比如mybatis的xml文件等 
除了web.xml修改好像需要 
4 如果JRebel不起作用 可以使用下面的方法
![](http://static.oschina.net/uploads/space/2013/1118/212904_1K4O_140593.jpg)
在Modules下 添加JRebel的支持 勾选  并点击后面的Generate rebel.xml 
在你项目的配置文件中找到生成的rebel.xml  你会发现 它里面有两个目录 
一个是java类的编译目录 一个是webroot的目录   
也就是JRebel的监控目录 它会监控那个两个目录  如果发生文件变化 就在内存中重新加载对应的资源 
非maven项目的静态资源发生变化后 如果不能热部署  可能就是JRebel监控的目录不太对  
可以自己指定一下正确的目录 
## 2.Tasks插件 集成任务管理 
![](http://static.oschina.net/uploads/space/2013/1118/111345_pJGI_140593.jpg)
# ![](http://static.oschina.net/uploads/space/2013/1118/111438_j8jn_140593.jpg)
Intellij中可以安装task插件 与jira等任务跟踪管理系统进行集成
安装插件后  会在系统任务栏上显示  任务插件  显示当前的任务 以及最近的任务
每个任务 可以创建不同的视图 
在不同视图下 会自动记录 当前任务下 修改过的页面 
所以这个功能相当的棒  当我们有多个jira任务时  可以随时切换任务 
任务完成后  我们可以根据任务对应的changelist  去提交当前任务下 修改过的文件 
破解文件的连接地址 
[http://note.youdao.com/share/?id=33c21fb3c24e81c2781872525e22ade2&type=note](http://note.youdao.com/share/?id=33c21fb3c24e81c2781872525e22ade2&type=note)
# 5.其它的一些技巧配置（未完待续） 
PS:2013-11-28 Facets的作用 
![](http://static.oschina.net/uploads/space/2013/1128/221127_Frni_140593.jpg)
![](http://static.oschina.net/uploads/space/2013/1128/221142_Yphm_140593.jpg)
[](http://static.oschina.net/uploads/space/2013/1128/221203_Gepp_140593.jpg)
