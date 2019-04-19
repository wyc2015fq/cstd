# Java环境配置的常见问题汇总 - HJ - CSDN博客
2017年11月28日 12:55:00[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：583
步骤：
**（1）安装JDK**
一般是JDK 8.0版本，在官网下载，安装完之后右键点击“我的电脑”，选择“属性”，进入之后选择“高级系统设置”，选择“环境变量”
![这里写图片描述](https://img-blog.csdn.net/20171128123625300?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里有三步要做：
```
a.新建 JAVA_HOME 变量 。变量值填写jdk的安装目录（本人是 "E:\Java\jdk 8.0" )
b.寻找 Path 变量→编辑,在变量值最后输入 "%JAVA_HOME%\bin;%JAVA_HOME%\jre\bin;"
（注意原来Path的变量值末尾有没有;号，如果没有，先输入；号再输入上面的代码）
c.新建 CLASSPATH 变量 变量值填写 ".;%JAVA_HOME%\lib;%JAVA_HOME%\lib\tools.jar"（注意最前面有一点）
```
系统变量配置完毕。
检测是否配置成功：运行cmd 输入 java -version （java 和 -version 之间有空格）。若如图所示 显示版本信息 则说明安装和配置成功。 
![这里写图片描述](https://img-blog.csdn.net/20171128124045404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
问题说明：
如果出现的不是这个，而是找不到“java”命令之类的，就说明环境配置未成功。即上述的三个步骤未完成。
**（2）Eclipse安装运行**
问题说明：
如果出现“由于误导文件eclipse启动出现An Error has…”，删除eclipse的临时文件： eclipse/configuration 目录下的 org.eclipse.osgi org.eclipse.update 两个子目录 
重新启动 eclipse
如果还是不行 
将workspace中 项目做一下备份，删除workspace目录和上面两个子目录 
再次启动 eclipse，再导入备份的项目即可
如果还是不行，则可能是JDK的版本与Eclipse版本不同，比如说一个是32位，另一个是64位的。只需要卸载其中一个重新下载同系统版本的即可
**（3）出现找不到主类？**
1、是因为.java文件不在项目的src路径内，也就是说源代码未被eclipse编译，字节码不存在无法运行了在项目名上右键 -> Builder Path -> Configure Build Path -> 选择Source面板 再点Add Folder, 把源代码所在的包路径的上层目录加进来，而且如果你是把两个类写在一个文件里的话,你在右键选择Run As Java Appication 的时候,要把光标至于包括main方法的类上
2、最快捷的解决办法是，打开带有main函数的类，ctrl +A （全选）—> ctrl +X（剪切）—–> ctrl+S(保存)—>关闭—->再打开—>ctrl +C (粘贴) —->OK;一句话就是粘出去 关闭 再粘进来
3、重建Project也行。
4、项目的Java Build Path中的Libraries中也许某个jar包是不可用的，显示红色叉叉。这说明系统找不到这个这个jar文件，把这个jar删除或者重新加载进来即可。
5、如果上述不行，大家直接在 cmd 下 javac 然后 java 运行试试，如果这都报错，那是你的 jdk 或者 os 环境有问题了。
**（4）启动 Eclipse 弹出“Failed to load the JNI shared library jvm.dll”错误的解决方法！**
原因1：给定目录下jvm.dll不存在。
对策：（1）重新安装jre或者jdk并配置好环境变量。（2）copy一个jvm.dll放在该目录下。
原因2：eclipse的版本与jre或者jdk版本不一致
对策：要么两者都安装64位的，要么都安装32位的，不能一个是32位一个是64位。
原因2的概率更大一些，原因1不太可能发生
**（5）当我们安装使用时，会出现eclipse启动不了，出现“Java was started but returned exit code=13……”的问题。**
原因： 
通过第三方更新JRE时，第三方安装的是32位的JRE，与64位的eclipse不匹配(报错信息的–launcher.library中有x86_64说明这个eclipse是64位的)
JDK8/JRE8在安装完成之后，会添加一个环境变量C:\ProgramData\Oracle\Java\javapath
这个环境变量是三个快捷方式，分别是java.exe.lnk javaw.exe.lnk javaws.exe.lnk，你每安装一次JDK/JRE，这三个快捷方式都会更新路径为最后一次的JRE路径
解决办法1： 
到高级系统设置中，把系统环境变量中32位的JRE路径删了就好了
解决办法2：
进入控制面板，选择“程序”，进入之后，选择“程序和功能”，进入到卸载页面，选择“Java 8 Update 25”右击，选择“卸载”。
