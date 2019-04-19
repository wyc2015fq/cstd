# Java 第三方库依赖引用说明 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年03月27日 16:39:36[boonya](https://me.csdn.net/boonya)阅读数：69
java中调用第三方库的方式是通过JNI调用dll或so来实现的。这里以一个linux依赖异常为例：java.lang.UnsatisfiedLinkError: no webp-imageio in java.library.path。此为使用webp调用依赖而出现的问题，下一篇着重讲webp使用的坑。
**目录**
[编译系统依赖路径java.library.path](#%E7%BC%96%E8%AF%91%E8%8E%B7%E5%8F%96%E7%B3%BB%E7%BB%9F%E4%BE%9D%E8%B5%96%E6%94%BE%E7%BD%AE%E8%B7%AF%E5%BE%84java.library.path)
[Linux so 放置路径](#Linux%20so%20%E6%94%BE%E7%BD%AE%E8%B7%AF%E5%BE%84)
[Windows dll放置路径](#Windows%20dll%E6%94%BE%E7%BD%AE%E8%B7%AF%E5%BE%84)
## 编译系统依赖路径java.library.path
使用以下代码编译后便于在其他系统中使用：
```java
import java.io.IOException;
public class Library {
	public static void main(String[] args) throws IOException {
		 System.out.println(System.getProperty("java.library.path"));
	}
	
}
```
进入Library.java所在的目录执行编译：
```
C:\Users\admin\Desktop>javac Library.java
C:\Users\admin\Desktop>java Library
C:\Program Files\Java\jdk1.8.0_121\bin;C:\windows\Sun\Java\bin;C:\windows\system32;C:\windows;C:\Program Files (x86)\NetSarang\Xftp 6\;C:\Progra
s\Java\jdk1.8.0_121\bin;C:\windows\system32;C:\windows;C:\windows\System32\Wbem;C:\windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x8
QL\MySQL Utilities 1.4.3\;C:\Program Files (x86)\MySQL\MySQL Utilities 1.4.3\Doctrine extensions for PHP\;C:\Oracle\product\11.2.0\client_lite\b
DEVELOPEWORKS\Maven\apache-maven-3.5.0/bin;D:\DEVELOPEWORKS\SVNInstall\bin;D:\DEVELOPEWORKS\Tomcat\apache-tomcat-7.0.92\lib;D:\DEVELOPEWORKS\Tom
ache-tomcat-7.0.92\bin;%JMETER_HOME%\bin;C:\Program Files\nodejs\;C:\strawberry\c\bin;C:\strawberry\perl\bin;C:\Program Files\Git\cmd;D:\DEVELOP
\zookeeper-3.4.13\bin;D:\DEVELOPEWORKS\mytest\apache-jmeter-5.0\bin;C:\Program Files\CMake\bin;C:\Users\admin\AppData\Roaming\npm;.
C:\Users\admin\Desktop>
```
## Linux so 放置路径
使用我们编译好的class获取java.library.path路径：
```bash
$java Library
/usr/java/packages/lib/amd64:/usr/lib64:/lib64:/lib:/usr/lib
```
看一下这几个目录，/usr/java/packages/lib/amd64不存在，/usr/lib64正好是我们要放置的so路径。
![](https://img-blog.csdnimg.cn/20190327163415503.png)
将依赖的webp-imageio.so包放入/usr/lib64下，启动项目操作正常。
## Windows dll放置路径
windows下就简单多了直接执行Java：
![](https://img-blog.csdnimg.cn/20190327163533106.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
Windows下直接放在jdk/bin目录即可，当然如果有使用tomcat也可以尝试直接放在tomcat下但估计此方式并不好用以笔者的经验和实践，最好根据上面输出的路径来放置是最稳当的。
