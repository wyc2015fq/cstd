# Android开发中遇到的问题(五)——Eclipse导入Android项目出现"Invalid project description overlaps the location of another project"错误的解决办法 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Android开发中遇到的问题(五)——Eclipse导入Android项目出现"Invalid project description overlaps the location of another project"错误的解决办法](https://www.cnblogs.com/xdp-gacl/p/4339945.html)



## 一、错误描述

　　使用如下的命令手动创建一个Android项目

```
android create project -n LinearLayoutTest -t 1 -p E:/Android/workspace/LinearLayoutTest -k me.gacl.ui -a MainActivity
```

　　命令执行完成之后就会在E:/Android/workspace目录下创建一个名字为LinearLayoutTest的Android项目，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/151531099172760.png)

　　E:/Android/workspace目录就是我的Eclipse设置的工作目录

　　然后我将LinearLayoutTest项目导入到Eclipse中，结果就出现了如下错误：

```
Invalid project description.E:\Android\workspace\LinearLayoutTest overlaps the location of another project: 'LinearLayoutTest'
```

　　导入Android项目的步骤如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/151537149332075.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/151526590745059.png)

　　试了好几次都不行，把项目删掉重新创建再重新导入还是不行。

## 二、问题的解决办法

### 2.1、解决办法一：把当前项目剪切到别的地方，再导入一次就可以正常导入了

　　使用"overlaps the location of another project"关键字在百度上搜索，没想到这个问题还是蛮多人遇到的，而出现这个问题的原因是：**ADT是要导入是我们当前已经存在的一个Android项目的，导入后是会放到当前的workspace的根目录下的。而我们要导入的项目，本来就被我放到“当前的workspace的根目录下”了，所以才导致冲突的。找出了问题所在，那么解决问题的办法就很简单了，把当前项目，剪切到别的地方，再导入一次，就可以正常导入了。**

　　这种方法是可以解决上述的所说的问题的，这个解决办法也是网上所说的办法，但是我又发现了一个新的问题，由于我的项目是使用如下的命令在cmd命令行手动创建的：

```
android create project -n LinearLayoutTest -t 1 -p E:/Android/workspace/LinearLayoutTest -k me.gacl.ui -a MainActivity
```

　　上面的命令中，-n指定项目的名称，-t指定android平台，-p指定项目存放路径，-k指定项目包名，-a指定Activity名称。

　　我的创建的项目的项目名是LinearLayoutTest，Activity名称是MainActivity，那么Eclipse在导入LinearLayoutTest项目时，会将项目命名成MainActivity，因为我的Activity名称是MainActivity，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/151626397922822.png)

　　这个结果不是我所希望的，所以方法一不是很好的解决办法。

### 2.2、解决办法二：手动创建Android项目时将Activity名称设置成和项目名称一样

　　后来我又尝试了另一种做法，同样也是使用命令手动创建Android项目，只不过在创建项目时，将Activity名称设置成和项目名称一样，命令如下：

```
android create project -n LinearLayoutTest -t 1 -p E:/Android/workspace/LinearLayoutTest -k me.gacl.ui -a LinearLayoutTest
```

　　上述命令执行完成之后就会在E:/Android/workspace目录(E:/Android/workspace目录就是我的Eclipse设置的工作目录)下创建一个名字为LinearLayoutTest的Android项目，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/151531099172760.png)

　　这次我们再将LinearLayoutTest项目直接导入到我们的Eclipse中，导入Android项目的步骤如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/151537149332075.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/151635334953205.png)

　　点击【Finish】按钮成功将项目导入到Eclipse中，并且也不会出现上述的错误，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/151637518706779.png)

　　这种解决办法是比较好的，使用命令手动创建Android项目时，将Activity名称设置成和项目名称一样，这样即使创建的项目是位于Eclipse工作的workspace目录下也能够正常导入，不会出现上述的问题。

　　这个问题是第一次遇到，在此记录一下解决办法！









