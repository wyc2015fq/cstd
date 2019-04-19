# Java 命令参考 - 定义域 - CSDN博客
2016年03月03日 16:35:21[定义域](https://me.csdn.net/u010278882)阅读数：203
# Java 命令参考
## java（c）
使用 javac 命令进行编译，要求如下 
a. Hello.java 源文件位于 /java/src/com/example/apt 目录下 
    Util.java 源文件位于 /java/src/com/example/utils 目录下 
b. Hello.java 包声明为 com.example.apt 
    Util.java 包声明为 com.example.utils 
c. Hello.java 引用 Util.java 中的方法 
d. 需要将编译输出到 /java/out 目录下
![文件结构](https://img-blog.csdn.net/20160303163632178)
执行如下的指令
```
// 当前目录为 src
javac  -d ../out com/example/apt/Hello.java     #1
java -cp ../out com.example.apt.Hello   #2
```
javac 参数1 -d  参数2 
参数1：类文件路径。相对于当前目录的编译出的类文件的路径。 
参数2：源文件路径。相对于当前目录想要编译的Java源文件的完整路径
编译过程中会自动在当前目录下查找引用到的源文件，并一同编译。 
比如，这里与 Util.java 被位于不同包的 Hello.java 引用，编译 Hello.java 同时编译了 Util.java。
java -cp 参数1 参数2 
参数1：类文件路径。 
参数2：类的完整限定名。
执行过程中会自动在参数1指定的目录及其子目录中查找类文件。
示例[下载](http://download.csdn.net/download/u010278882/9451880)
## apt
注解处理器
`apt -cp ../out -s ../out -nocompile -factory annotation.InterfaceExtractorProcessorFactory annotation\Multiplier.java`
apt -cp 注解处理器类文件查找路径 -s 生成物位置 -factory 使用的注解处理器 待处理的源文件
