# the import java.util.* cannot be resolve,怎么解决 - =朝晖= - 博客园
# [the import java.util.* cannot be resolve,怎么解决](https://www.cnblogs.com/dhcn/p/7105643.html)
 我碰到这个问题是因为重装系统后，原先的JDK6换成了JDK7， Eclipse中的旧项目中jsp文件的此类import出现错误提示。在以下页面找到解决方案，专贴出来：
[http://www.myexception.cn/eclipse/17226.html](http://www.myexception.cn/eclipse/17226.html)
显示 the import [Java](http://lib.csdn.net/base/java).util cannot be resolve,如何解决？
我在使用eclipse的时候， 好像无意中更改了安装位置(workspace)，现在所有的包都显示无法导入：the import java [cannot be resolved](http://www.myexception.cn/eclipse/14725.html) 请问如何解决这个问题？
------解决方案--------------------
解决方案：
右键项目-------buildpath--------最下面那个configura。。。的
选择libraries
找到JRE（这个时候你会发现这个jre前面有！或者是红X）
选中remove掉
重新为该项目选择一个JRE
选中项目，project----clean
OK，全部的包都找到了 
------解决方案--------------------
安装楼上的把工程里面的JAR配置一下就可以了
因为你换了工作环境，而工程里面的JAR包都是之前的
你现在把工程里面的Java环境配置成你现在的就可以了。

