# 命令行编译运行Java - Grandyang - 博客园







# [命令行编译运行Java](https://www.cnblogs.com/grandyang/p/4948902.html)







首先要安装JDK，然后设置环境变量Path，添加C:\Program Files (x86)\Java\jdk1.8.0_66\bin

然后建立一个名为j.java的文件，里面加入如下代码：

```
public class j {
    public static void main(String[] args) {
       System.out.println("Hello World!");
    }
}
```



然后打开命令行，输入 **javac j.java**

然后运行** java j**



这就是命令行编译运行单个java文件的方法。。。












