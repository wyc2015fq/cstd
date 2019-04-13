
# Clojure 学习入门（2）—— eclipse 插件 - 阳光岛主 - CSDN博客

2013年12月22日 17:40:49[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：11515所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



以前听说过LISP函数式编程，也听人说Erlang的牛逼，接触了Clojure才知道，原来Clojure就是JVM中的Erlang。网上基于Ubuntu环境下开发Clojure的资料太少，在这里整理总结，分享给大家。

**1. 安装Clojure**
Clojure下载地址：[download](http://clojure.org/downloads)
Clojure托管源码：[github](https://github.com/clojure/clojure)

Cloure在Ubuntu下安装，只要简单的一条命令：sudo apt-get install clojure
Clojure启动：clojure或/usr/bin/clojure（Ctrl + D 退出）

可以手动的方式：java -cp /opt/clojure-1.5.1/clojure-1.5.1.jar clojure.main\#/opt/clojure-1.5.1/ 是clojure 安装的目录

安装遇到问题，可以求助clojure FAQ

然后输入clojure就可以进行clojure编程，例如：
**示例1：**计算 1 + 2 +3
\#clojure
Clojure 1.1.0
user=>(+ 1 2 3)
6
**示例2：**打印“IT-Homer”
user=>(javax.swing.JOptionPane/showMessageDialog nil "IT-Homer")
运行弹出结果：
![](https://img-blog.csdn.net/20131209131712187)


**2. 安装Eclipse Clojure插件**
这里安装的插件是Counterclockwise，可以通过Eclipse自动安装软件的功能直接安装，具体如下:
Eclipse菜单Help->Install New Software->输入插件网址：[http://ccw.cgrand.net/updatesite/](http://ccw.cgrand.net/updatesite/)，根据提示安装，重启Eclipse
安装好Counterclockwise插件后，可以新建Clojure工程和Clojure文件，使用方法和Java一样

**3. 安装 Leiningen**
Leiningen是专门针对Clojure的类似Maven的工具，手动安装方法可以通过[leiningen](https://github.com/technomancy/leiningen/)上面的介绍
Ubuntu下一条命令安装：sudo apt-get install leiningen（此种方式仅安装 1.x版本，想安装最新的，需[手动安装](https://github.com/technomancy/leiningen/blob/stable/README.md)）

**4.****安装leiningen针对eclipse的插件**
这个插件可以通过读取project.clj 来自动建立eclipse的源码工程，之后你再通过eclipse的import exists project即可
命令：lein plugin install lein-eclipse 1.0.0
用法：
1）创建工程：lein new project_name
2）下载依赖：lein deps
3）生成eclipse工程：lein eclipse
示例：lein创建项目 test222
![](https://img-blog.csdn.net/20131222182333484)

**5. 导入clojure 到 eclipse**
To import a Leiningen project into Eclipse, so that you can use CounterClockwise with all its nice features, such as syntax highlighting, there is a handy plugin called**lein-eclipse**.
To get started, all you have to do is to add this plugin as a*dev-dependency*in your project’sproject.clj.

```python
(defproject my-project "1.0.0-SNAPSHOT"  
  :description "My Clojure Leiningen Project"  
  :dependencies [  
        [org.clojure/clojure "1.2.0"]  
        [org.clojure/clojure-contrib "1.2.0"]  
    [ring/ring-jetty-adapter "0.2.5"]  
  ]  
  :dev-dependencies [  
        [lein-eclipse "1.0.0"]  
  ]  
)
```

Then, you need to download the project dependencies via
**prompt> lein deps**
This will automatically install*lein-eclipse*and add a new task called*eclipse*to*leiningen*. Invoking
**prompt> lein eclipse**
runs this task and creates the Eclipse projects files.projectand.classpath. Now you can simply import the project into Eclipse via “File->Import->Existing Project into Workspace”.

**参考推荐：**
[Clojure Quick Start](http://clojure.org/getting_started)
[Clojure 官方](http://clojure.org/)
[Clojure 源码](https://github.com/clojure/clojure)（github）
[Leiningen 官方](http://leiningen.org/)
[Leiningen 源码](https://github.com/technomancy/leiningen)（github）
[Window7下配置Clojure+Emacs](http://www.cnblogs.com/darkluck99/archive/2012/02/20/2360216.html)



