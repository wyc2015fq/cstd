# Eclipse中将项目打包成jar的常用方法 - Machine Learning with Peppa - CSDN博客





2018年06月06日 15:04:17[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：359
所属专栏：[JavaSE](https://blog.csdn.net/column/details/21699.html)









第一种：利用eclipse中自带的export功能

第一种方法分两种情况先来看第一种情况：没有引用外部jar的项目打包

步骤一：右键点击项目选择导出（export），选择java>jar文件(不是选择可运行jar文件)

![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219184139037-829991110.png)

![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219184253131-117653222.png)

步骤二：选择你要导出的项目以及文件，指定文件导出路径。连续点击两个下一步后到第四步。

![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219184505006-548040916.png)

步骤三：选择主类。

![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219185040240-174840904.png)

按照以上步骤即可完成对一个不引用外部jar项目的打包。





第二种情况：引用了外部jar的项目打包



当我们引用了外部jar后，使用eclipse自带的export打包略显繁琐。

步骤一：准备主清单文件 “ MAINFEST.MF ”，放在项目的文件夹下面。

MAINFEST.MF是jar的配置文件，第一行定义版本，第二行指出外部jar的路径，第三行是主类指示，第四行是空行。



```
1 Manifest-Version: 1.0
2 Class-Path: lib/RXTXcomm.jar
3 Main-Class: com.main.Client
4
```



值得注意的是，MAINFEST.MF编写需要严格规范，稍有不慎将会导致jar执行无效



```
1 第一行不能空，行与行之间不能有空行，每一行的最后一个字符不能是空格
2 最后一行一定是空行
3 每个属性的名称和值之间（冒号后面）一定要有空格
4 文件的每一行都不能超过72个字节（一般是70个ASCII字母加上回车换行符）；如果72个字节不够用，则另起一行并以空格开头：以空格开头的行都被视为前一行的续行。
```



详细规则这篇文章有讲到 [MANIFEST.MF文件编写的注意事项](https://www.2cto.com/kf/201305/208332.html)

步骤二：与没有外部jar的项目一样，像上面那样先执行到[步骤三](https://www.cnblogs.com/tianyanzhi/p/8067239.html?from=groupmessage&isappinstalled=0#s4)，不同的是要选着现有的清单，然后点击完成。



![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219222116240-602832622.png)



以上就是第一种方法的两种情况，至此，我们已经可以生成jar包了。（如何运行jar文件？点击 [这里](https://www.cnblogs.com/tianyanzhi/p/8067239.html?from=groupmessage&isappinstalled=0#q1)）



 第二种：利用eclipse插件Fat jar打包jar

 步骤一：先给eclipse安装好Fat jar插件

Fat jar 更新站点：  http://kurucz-grafika.de/fatjar

![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219222954225-938963403.png)



待文件加载出来后点击全部选中，一直下一步知道完成，会提示重启，安装完成。（Fat jar安装报错？点击 [这里](https://www.cnblogs.com/tianyanzhi/p/8067239.html?from=groupmessage&isappinstalled=0#q2)）

步骤二：右键点击项目名称，会看到build fat jar，点击。



![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219224123365-697112152.png)



步骤三：配置jar的存放位置以及main类。

红框内为必填，main-class点击browse选择主类。（无法点击browse？点击 [此处](https://www.cnblogs.com/tianyanzhi/p/8067239.html?from=groupmessage&isappinstalled=0#q3) 查看解决方法）



![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219224354881-465153087.png)



步骤四：选择要打包的文件以及外部jar，点击完成。



![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219224826959-2037407500.png)



至此，我们完成了用fat jar插件打包含有外部jar的项目工程。





 中途可能遇到的问题以及解决方法

 问题一：打包好jar 文件后如何运行？

解决方法：

第一种方法：cmd下直接键入Java -jar xxx.jar  开始运行。（可能需要切换盘符与文件夹，方法点 [这里](https://jingyan.baidu.com/article/5552ef473e2df6518ffbc916.html)）



![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219225758756-232110319.png)

（如图是放在C盘用户>chen下面的jar）



第二种方法：新建一个批处理文件，内容：Java -jar xxx.jar。放在与jar包同一个文件夹下面即可，然后双击运行查看效果。



![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219230350459-1657706975.png)



问题二：安装Fat jar提示出错怎么办？

解决方法：

在eclipse中

Help -> Install New Software... -> Work with

-> 选择“The Eclipse Project Updates - http://download.eclipse.org/eclipse/updates/4.7”

-> 勾选"Eclipse Tests,tools, Examples, and Extras" 下的 "Eclipse 2.0 Style Plugin Support"



![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219231225615-233700824.png)

安装完后再安装Fat jar即可



 问题三：为什么我的main-class无法browse文件？（猜测可能有eclipse与Fat jar的版本兼容问题）

 解决方法：

先用eclipse自带的export查看main-class名字（export方法如 [上面](https://www.cnblogs.com/tianyanzhi/p/8067239.html?from=groupmessage&isappinstalled=0#s4)），再复制粘贴填下去继续步骤。

一般main-class为 包名.主类名 ，可能不带Java、main等后缀



![](https://images2017.cnblogs.com/blog/1266294/201712/1266294-20171219232058928-2019469472.png)





注：文中所用到的软件及版本如下

eclips oxygen with官译插件

jdk 9

Fat jar插件



