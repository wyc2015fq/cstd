# JSP运维JSP项目部署到server		



实战：真正server端部署jsp项目经验总结与记录（完整过程从0到10适合对server端部署0经验新手）

> jsp+tomcat+mysql项目部署到真正server; servermysql安装; serverjdk安装; servertomcat安装; serversql文件管理; serverjsp项目与servermysql连接;

将项目与域名进行绑定的相关教程已经更新。[网址](http://blog.csdn.net/s_gy_zetrov/article/details/70159259)

- 第一步是安装jdk，jdk应该是全部须要安装的软件中最先安装的。

  

  jdk详细怎样安装能够參考[这篇](http://blog.csdn.net/qq_15267341/article/details/52299654)

  ```repl
  我截取了能够參考的部分
  1. 装JDK ,我从本地右键复制一个jdk,然后到远程桌面的C盘以下，鼠标右键粘贴（我是直接在server端用浏览器下载的，要保证版本号与你本地开发环境所使用版本号号同样。假设不同，但本地实验过不会出bug。那也能够）安装就可以，选项一般选默认就可以
  2. 配置一下JAVA_HOME--》系统高级设置->环境变量->系统变量新建->变量名JAVA_HOME->变量值jdk文件夹且最后不带分号
  ```

- 第二步是配置tomcat，一般官网下载下来都是zip包，解压到c盘就可以。在tomcat的bin文件夹双击startup.bat启动tomcat服务。在浏览器中输入`http://localhost:8080/` 如出现tomcat的首页则说明tomcat已经成功启动。但非常多时候都须要tomcat开机自己主动启动。

  以下简介一怎样在win server设置开机自己主动启动。

  新建系统环境变量:CATALINA_HOME，值为你的TOMCAT的文件夹，最后打开Path，加入变量值：%CATALINA_HOME%\bin; 然后打开cmd。将tomcat文件夹中bin文件夹中的service.bat拖进小黑框。回车，若显示

  ![cmd应出现的结果图片1.PNG](https://ooo.0o0.ooo/2017/04/08/58e866cfabf78.png)

  说明一切ok能够进行下一步。假设不是这样，那便是安装tomcat的那个路径错了或者环境变量配错了。请检查。

  之后cmd窗体键入 `service.bat install Tomcat`或者`service.bat install`输完然后按Enter键。出现例如以下相似窗体。便成功了。

  ![cmd应出现的结果图片2](http://img.blog.csdn.net/20170408123309083?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU19neV9aZXRyb3Y=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

  最后->服务->Apache Tomcat->右键点击，选择启动，再在属性中将启动类型改为”自己主动”

  ![服务选项在哪里](http://img.blog.csdn.net/20170408123821607?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU19neV9aZXRyb3Y=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

  ![在哪里选择启动.PNG](http://img.blog.csdn.net/20170408124115657?</p><p>watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU19neV9aZXRyb3Y=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

  ![类型设为自己主动.PNG](http://img.blog.csdn.net/20170408124423770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU19neV9aZXRyb3Y=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

  再将你本地tomcat的webapp中的项目文件夹复制进远程桌面下c盘中tomcat文件夹的webapp就可以。

  

- 第三步是安装mysql。參考我之前的[这篇](http://blog.csdn.net/s_gy_zetrov/article/details/55669018)博客，假设`net start mysql`在路径为system32文件夹内的cmd中不能正常启动试试用cd命令切回bin文件夹再试一下，多走几遍就通了。

  至于导入sql文件。须要从本地用mysql管理工具（如navicat）导出，然后存到server上再使用server上的mysql管理工具（如navicat，server端mysql管理工具自行安装，这里不赘述）导入就可以

  ```
  导入方法我简要描写叙述一下（以管理工具为navicat为例）
  1. 先依照我上面给的博客在mysql中配置连接。然后在navicat中建立连接。连接名任意。我取名为localhost_3306，password一栏取刚改的password。这个时候不要着急点确定，点击“连接測试”，人品好的都不会报错。然后关闭对话框，在左側导航栏双击已经建立的localhost_3306连接名能够看到他变绿了
  2. 这时右键点击这个绿名字选择新建数据库，数据库名称与你jsp项目中jdbc中传进去的数据库字符串名字同样，字符集因人而异，我选了GBK。点击确定
  3. 然后双击新建好的数据库使它变绿，接下来找到已经传到远程桌面中的.sql文件。拖到这个数据库里面。弹出的对话框中一般不改什么，就改一下编码，与之前的编码一样，如我的是GBK。
  4. 完毕后关闭，数据库右键选择刷新。sql文件应已成功导入“表”中
  5. 至此数据库导入结束
  ```

  如今在server端浏览器输入`http://localhost:8080/xxx`假设能够正常打开你的项目。则也应该已经能够从本机訪问server中部署好的站点了。若訪问不了没准是servertomcat还未启动。

## 遇到的问题与解决方法

- server端的navicat连接数据库的时候假设依照我的博客改动过password。会报错1862。

  

  解决的方法使用`mysql -h localhost -u root -p`登陆进mysql后键入`SET PASSWORD = PASSWORD('root');`，将password改动为root，这时候navicat中password改为root就能够登进去了，假设与自己jsp项目中的password字符串不一样了也没关系。反复这一步，再把password改为你想要的，与项目中连接数据库文件一样的password就可以。如今已经能够正常连接了，仅仅是一个小bug不必惊慌。

- server端浏览器能够`localhost：8080/...`訪问可是外网不能`ip:8080`訪问的解决：添加TCP8080port

  在系统防火墙入站规则中选新建规则->点击加入port->规则适用tcp->指定本地port为8080->下一步->下一步->给port定义名称：web,描写叙述为8080.

- server怎样打开ping

  Windows防火墙 -> 高级设置 -> 入站规则 -> 在列表里找到“文件和打印机共享(回显请求 -  ICMPv4-In)” -> 右击选择启用规则，并在其属性里设置为‘同意连接’-> 确定 ->  大功告成。要禁止ping,去掉勾选就可以.

- 訪问时发现自己html格式解析不正确须要改动源代码

  遇到小的html问题，那种加点东西减点内容的。能够直接在server远程桌面里面tomcat文件夹webapp中项目文件的源代码用记事本改动的，改动完再次双击tomcat的start.dat就可以。已经执行着的tomcat黑框不用关

  吐槽：这样的东西本地开发的时候就应该考虑周全，比方我昨晚遇到不同浏览器訪问server站点html字体解析不同，这样的小插曲在本地开发的时候就应该想到，并解决的。

=============================与主题无关项===========================================

### 昨天晚上9点50分站点正式上线

纪念一下~~

![远程桌面图标.PNG](https://ooo.0o0.ooo/2017/04/08/58e857579ad37.png)

![ag站点上线时间tomcat截图.PNG](https://ooo.0o0.ooo/2017/04/08/58e857b9505a9.png)

[假装另一个server的桌面截图] –不是不能截，仅仅是我不愿意放到这里罢了

开发周期：40天
 核心开发：21天
 主观时间投入：200h
 后期系统运维：6个月
 职务：系统管理员（自封）
 ===================================================================================

> 參考资源
>    entry1-tomcat: <http://jingyan.baidu.com/article/a65957f4b12b8724e77f9b5a.html> 
>    若entry1报错则->entry2: <http://blog.csdn.net/ruanqiangqiang/article/details/7414905> 
>    entry2验证: <http://www.cnblogs.com/qisel/p/3915262.html> 
>    entry2验证: <http://www.jb51.net/article/87456.htm> 
>    entry3-mysql: <http://blog.csdn.net/s_gy_zetrov/article/details/55669018> 
>    entry4-mysql: error1862 <https://segmentfault.com/q/1010000008150194> 
>    entry5-8080: <http://jingyan.baidu.com/article/ed15cb1b0e520a1be369810c.html> 
>    entry6-ping: www.jb51.net/article/89163.htm 

最后的最后，来一点碎碎念。

一般像那种原来我都不敢想象的事情如今做成了的时候，我都会有跟认识的人到处炫耀把自己吹的非常强那种想法。但我一贯又不喜炫耀，甚至除去炫耀，连一般的生活记录都不会广而告知。

看我朋友圈就知道了，我非常少发朋友圈。我属于这样的说一分做七分剩下二分在发呆的那种。相应俗话中的“说三分做七分”。我原来对自己的评价是说一分做六分剩下三分在发呆。如今总算也做成了点事就给自己加一分吧。尽管如今我还有强烈的把这件事告诉全部朋友的冲动，但我的人生哲学禁止我这么做，最多。也仅仅是在博客里面提一下，毕竟看我博客的大都是陌生人。so,  尽管这里多说了几句但我会把这段话处理一下，除非细致找，否则是看不到的。

假设你如今在看这段话，而你又知道我是谁，请不要替我宣传。我第二讨厌的就是别人把我不想说的话说了出来让其它人知道。至于第一讨厌，这里我不明说。



