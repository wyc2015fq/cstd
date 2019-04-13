
# Java使用字节码和汇编语言同步分析volatile，synchronized的底层实现 - 无界 - CSDN博客

置顶2019年03月15日 13:18:09[21aspnet](https://me.csdn.net/21aspnet)阅读数：214


关于怎么查看字节码的五种方法参考本人另一篇文章《[Java以及IDEA下查看字节码的五种方法](https://blog.csdn.net/21aspnet/article/details/88351875)》
## 查看汇编语言汇编码
说要看汇编还是很有必要的，因为有些地方比如加锁其实还是通过汇编实现的，只看字节码不能看出底层实现。
其实就是利用使用hsdis与jitwatch查看JIT后的汇编码。
## 1.首先下载hsids
要查看JIT生成的汇编代码，要先装一个反汇编器：hsdis。从名字来看，即HotSpot disassembler。
实际就是一个动态链接库。网络上有已经编绎好的文件，直接下载即可。
[https://github.com/jkubrynski/profiling/tree/master/bin](https://github.com/jkubrynski/profiling/tree/master/bin)
![](https://img-blog.csdnimg.cn/20190315131509521.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
下载这2个文件拷贝到jre1.8.0_144\bin\server下：
![](https://img-blog.csdnimg.cn/20190315131707610.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
通过以下命令可以测试是否安装成功：
```python
java -XX:+UnlockDiagnosticVMOptions -XX:+PrintAssembly -version
```
![](https://img-blog.csdnimg.cn/20190315132926314.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
这说明安装完成。

## 2.安装JITWatch
JITWatch是分析展现JIT日志等的图形界面工具。
[https://github.com/AdoptOpenJDK/jitwatch](https://github.com/AdoptOpenJDK/jitwatch)
![](https://img-blog.csdnimg.cn/20190315133242469.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
下载好，本地解压开，然后在IDEA里导入项目然后编译之！
![](https://img-blog.csdnimg.cn/20190315141245164.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
首先要写一个足够复杂的类，让JIT编绎器认为它需要进行优化，不然产生的日志可能没什么内容。
启动launchUI.bat
![](https://img-blog.csdnimg.cn/20190315141516786.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
左上方有一个sanbox，点击会在当前目录下生成一个sanbox文件夹，里面存放着sanbox这个示例相关的代码，你也可以直接这里写代码。
```python
public class Test {
    public volatile long sum = 0;
    public int add(int a, int b) {
        int temp = a + b;
        sum += temp;
        return temp;
    }
    public static void main(String[] args) {
        Test test = new Test();
        int sum = 0;
        for (int i = 0; i < 1000000; i++) {
            sum = test.add(sum, 1);
        }
        System.out.println("Sum:" + sum);
        System.out.println("Test.sum:" + test.sum);
    }
}
```
```python
javac Test.java
java -server -XX:+UnlockDiagnosticVMOptions -XX:+TraceClassLoading  -XX:+PrintAssembly -XX:+LogCompilation -XX:LogFile=E:\work\tmp\Test.log  Test
```
![](https://img-blog.csdnimg.cn/20190315143200450.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
在tmp目录会生成对应文件，然后OpenLog打开，再Config好：
![](https://img-blog.csdnimg.cn/20190315143555595.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
点“Start”解析：
![](https://img-blog.csdnimg.cn/20190315143730346.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)点击方法，弹出的页面就可以 源码，字节码，汇编码 同步了：
![](https://img-blog.csdnimg.cn/20190315145042299.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190315145054799.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
可以看到实际上底层还是通过lock来实现，关于lock可以参考intel处理器指令。
![](https://img-blog.csdnimg.cn/20190315153411947.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
另外这一片帖子也可以参考《[volatile，你了解多少](http://www.importnew.com/27863.html)》，作者多少有一点靠蒙的分析，这不科学，不过我觉得还是不如汇编来的彻底。
```python
import static java.lang.Thread.sleep;
public class TestAdd {
    private static int count;
    public static void main(String[] args) {
        for (int i = 0; i < 10000; i++) {
            new Thread() {
                public void run() {
                    //System.out.println(">>>:");
                    synchronized(TestAdd.class){
                        //System.out.println(">>>>:");
                        count++;
                        //System.out.println("<<<<:");
                    }
                }
            }.start();
        }
        try {
            sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("count:" + count);
    }
}
```
![](https://img-blog.csdnimg.cn/20190315151246190.png)
![](https://img-blog.csdnimg.cn/20190315151039893.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190315152655743.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190315153133844.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
参考文献：
在安装hsids和jitwatch过程中遇到一些问题参考了以下帖子
[利用hsdis和JITWatch查看分析HotSpot JIT compiler生成的汇编代码](https://blog.csdn.net/hengyunabc/article/details/26898657)
[如何在windows平台下使用hsdis与jitwatch查看JIT后的汇编码](https://www.cnblogs.com/stevenczp/p/7975776.html)
[使用JITWatch查看JVM的JIT编译代码](https://liuzhengyang.github.io/2017/07/27/jitwatch/)

