# 提高开发效率的 Eclipse 实用操作（2） - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [Sam哥哥](http://www.jobbole.com/members/Sam哥哥) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
### Eclipse 没落了？不一定吧
[最近伯乐在线一篇资讯提到在 JAVA IDE 中](http://top.jobbole.com/35647/)，Eclipse 占比是 41%，IntelliJ IDEA 占比是 46%。但是目前我依然不会选择 Intellij 作为日常的 IDE。原因是周围的同事中，没有一个对 Intellij 非常熟悉，没有充分利用 Intellij 的特性来提高操作效率。相反，Eclipse 中的各种操作，我算是属于用的精熟了。所以跟周围使用 Intellij 的同事相比，我用 Eclipse 来操作代码，反而比他们快很多个级别。
下面我再列举一些使用 Eclipse 的一些技巧。之前我发过一篇《[能够提高开发效率的 Eclipse 实用操作](http://blog.jobbole.com/103503/)》，已经非常详细介绍使用 Eclipse 的各种技巧。本文是之前的一个补充。
### 使用多窗口(window)
有时候同一个`Eclipse` 工作空间中，会存放多个项目，如果你在同一个窗口中不想看到太多的项目，那么可以`new window`窗口来达到这个目的。
**选中工程–>右键 –> Open in New Window**
![这里写图片描述](http://img.blog.csdn.net/20160928141552870)
效果如下：
![这里写图片描述](http://img.blog.csdn.net/20160928141846182)
那么现在有个问题，如何在窗口之间进行切换呢？可以使用`alt + w` 弹出`Window`菜单的子菜单,子菜单中就有所有打开的窗口列表。
![这里写图片描述](http://img.blog.csdn.net/20160928142152871)
这个时候按下1或者2，就可以在`Window`之间进行来回切换了。
### 高亮控制Alt+Shfit+O
默认情况下，选中`JAVA` 代码中的变量，会高亮所有用到这些变量的地方。但是要不要高亮，是有开关的。可以通过使用`Alt + Shit + o` 来达到这个目的。
### 快速打开方法调用层次Ctrl+Alt+H
想知道方法的调用层次可以使用`Ctrl + Shift + G` ,但是这种很慢，是基于整个工作空间的。最好还是使用`Ctrl+Alt+H` ，基于工程的。
### 查看字符串在文件中出现的地方Ctrl+Shift+U
![这里写图片描述](http://img.blog.csdn.net/20160929090344621)
如果想知道字符串`stt`在`Demo1.java`这个文件中出现的地方，直接按下`Ctrl+Shift+U` 就可以显示出来。
### 快速生成代码
像for、ifelse等等语句都是可以通过`Eclipse`快速生成的。 比如输入`if`，然后按下`Alt+/`，`Eclipse`就会给出提示
![这里写图片描述](http://img.blog.csdn.net/20160930133940065)
如果你想自定义**别名**来快速生成代码，可以如下操作：
Window–>Preferences–>Java–>Templates
![这里写图片描述](http://img.blog.csdn.net/20160930134524994)
如果你想快速生成如下语句：


```
Listlist = new ArrayList();
```
可以`New`一个别名`new_list`,然后把上面语句配置好，保存后。写代码时，只需要输入`new_list`就会出现上面的语句。
### 收缩视图
如果你工作空间打开了太多的工程，想要在package视图中，一下子都收缩回来的话，可以使用：


```
ctrl  + shift + 小键盘上的斜杠 /
```
![这里写图片描述](http://img.blog.csdn.net/20160930135118284)
假设上图中的四个工程，要全部收缩，使用这个快捷键就可以快速做到。
备注：
这个快捷键只有焦点在`package`上的时候才有用。你可以使用`ctrl+F7`先切换到`package`视图，然后在使用这个快捷键。
### 直接在Eclipse中连接linux
使用如下快捷键可以弹出一个窗口，用于配置终端去连接Linux：


```
ctrl+shift+alt+t
```
![这里写图片描述](http://img.blog.csdn.net/20160930135632160)
配置好用户名和密码后，使用Window–>Show View –>Terminal找出 Terminal视图，就可以直接操作Linux了。
![这里写图片描述](http://img.blog.csdn.net/20160930135910216)
### 在多个Console切换
![这里写图片描述](http://img.blog.csdn.net/20160930140421382)
假设有多个Console，那么可以使用：


```
Alt + F7
```
来进行切换。
![这里写图片描述](http://img.blog.csdn.net/20160930140512663)
### 打开pom.xml文件切换到pom视图
我们经常需要打开`Maven`的`pom.xml`文件，这个时候，默认是跳转到`Overview`视图的，如何直接跳转到`pom`视图呢？ 使用


```
Alt+PageDown
```
![这里写图片描述](http://img.blog.csdn.net/20160930141243463)
这个时候使用`Alt+PageDown` 跳转到`pom`视图即可。
### 弹出小菜单列表
![这里写图片描述](http://img.blog.csdn.net/20160930141632527)
这个时候我想关闭其中一个JAVA文件，可是使用


```
Alt + -
```
弹出小菜单
![这里写图片描述](http://img.blog.csdn.net/20160930141825305)
### 弹出文件左边菜单列表
有时候想显示出文件的行数，最快的做法是使用


```
Ctrl+F10
```
弹出文件的左边的菜单
![这里写图片描述](http://img.blog.csdn.net/20160930142101824)
然后使用方向键，选择`Show Line Numbers` 即可。
### 黑色界面设置
以往版本的`Eclipse`的黑色字体做的很不好很不彻底，但是最新版的`Eclipse Neno`的黑色字体已经有改进了。推荐大家用一下，设置方法非常简单
Window–>Preferences–>General–>Appearances
![这里写图片描述](http://img.blog.csdn.net/20160930142943939)
选择`Dark`就行。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/07/54f7206fd1f1907bec78ddf32f50d66d.png)
