# jdk 环境变量配置 - 战斗蜗牛的专栏 - CSDN博客





2010年01月13日 15:17:00[vbskj](https://me.csdn.net/vbskj)阅读数：542标签：[jdk																[java																[path																[windows																[class																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[java](https://blog.csdn.net/vbskj/article/category/643636)





Windows 下JAVA 用到的环境变量主要有3 个，JAVA_HOME、classpath、path。 下面逐个分析。 JAVA_HOME 指向的是JDK 的安装路径，如x:/JDK_1.5，在这路径下你应该 能够找到bin、lib 等目录。值得一提的是，JDK 的安装路径可以选择任意磁盘 目录，不过建议你放的目录层次浅一点，如果你放的目录很深，比如 x:/XXXXXX/xxxxx/XXXX/xxxx/XXXX/xxxx/XXXX/xxx…… 那么，下面的步骤和以后的应用你都要受累了，呵呵。设置方法： JAVA_HOME=x:/JDK_1.5 path 环境变量原来Windows 里面就有，你只需修改一下，使他指向JDK 的 bin 目录，这样你在控制台下面编译、执行程序时就不需要再键入一大串路径了。 设置方法是保留原来的PATH 的内容，并在其中加上%JAVA_HOME%/bin (注，如果 你对DOS 批处理不了解，你可能不明白%%引起来的内容是什么意思；其实这里是 引用上一步设定好的环境变量JAVA_HOME，你写成x:/JDK_1.4.2 也是可以的； 你可以打开一个控制台窗口，输入echo %JAVA_HOME%来看一下你的设置结果) ： path=%JAVA_HOME%/bin;%path% 同样，%path%是引用以前你设置的PATH 环境变量，你照抄以前的值就行了。 classpath 环境变量我放在最后面，是因为以后你出现的莫名其妙的怪问题 80%以上都可能是由于classpath 设置不对引起的，所以要加倍小心才行。 classpath=.;%JAVA_HOME%/lib;%JAVA_HOME%/lib/tools.jar 首先要注意的是最前面的".;"，如果你看不清，我给你念念--句点分号。这个 是告诉JDK，搜索CLASS 时先查找当前目录的class 文件--为什么这样搞，这 是由于LINUX 的安全机制引起的，LINUX 用户很明白，WINDOWS 用户就很难理解(因 为WINDOWS 默认的搜索顺序是先搜索当前目录的，再搜索系统目录的，再搜索 path 环境变量设定的) ，所以如果喜欢盘根究底的朋友不妨研究一下LINUX。 为什么classpath 后面指定了tools.jar 这个具体文件？不指定行不行？显然不 行，行的话我还能这么罗索嘛！:) 这个是由java 语言的import 机制和jar 机 制决定的，你可以查资料解决。 
第一步:右击“我的电脑”，点击“属性”： 
第二步：选择“高级”选项卡，点击“环境变量”： 
第三步：在“系统变量”中，设置3项属性，JAVA_HOME,PATH,CLASSPATH(大小写无所谓),若已存在则点击“编辑”，不存在则点击“新建”： 
第四步：JAVA_HOME设为JDK的安装路径(如F:/JAVA编译器/Java/jdk1.5.0_10 )，此路径下包括lib，bin，jre等文件夹（此变量最好设置，因为以后运行tomcat，eclipse等都需要依靠此变量）； Path使得系统可以在任何路径下识别java命令，设为： %JAVA_HOME%/bin;%JAVA_HOME%/jre/bin CLASSPATH为java加载类(class or lib)路径，只有类在classpath中，java命令才能识别，设为： .;%JAVA_HOME%/lib;%JAVA_HOME%/lib/tools.jar (要加.表示当前路径) %JAVA_HOME%就是引用前面指定的JAVA_HOME 
第五步：“开始”－>“运行”，键入“cmd”： 
第六步：键入命令“java -version”，出现下图画面，说明环境变量配置成功. 
忠告：文件目录最好不要有中文，可能会因此不能正常运行！







写一个简单的java程序来[测试](http://softtest.chinaitlab.com/)J2SDK是否已安装成功： 
public class hello
{ 
 public static void main(String args[])
 { 
 System.out.println("Hello"); 
 } 
} 

将程序保存为文件名为hello.java的文件。 

打开命令提示符窗口，进入到hello.java所在目录，键入下面的命令 
javac hello.java 
java hello 
此时若打印出来hello则安装成功，若没有打印出这句话，仔细检查以上配置是否正确。](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)




