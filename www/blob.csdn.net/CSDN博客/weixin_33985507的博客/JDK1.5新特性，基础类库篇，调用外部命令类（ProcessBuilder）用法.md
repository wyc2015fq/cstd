# JDK1.5新特性，基础类库篇，调用外部命令类（ProcessBuilder）用法 - weixin_33985507的博客 - CSDN博客
2013年09月11日 15:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
## 一. 背景
ProcessBuilder类是用来创建操作系统进程的。与Runtime.exec相比，它提供了更加方便的方法以创建子进程。
每个ProcessBuilder实例管理着一个进程属性的集合。 [`start()`](http://docs.oracle.com/javase/1.5.0/docs/api/java/lang/ProcessBuilder.html#start())方法用这些属性创建一个新的[`Process`](http://docs.oracle.com/javase/1.5.0/docs/api/java/lang/Process.html)实例。[`start()`](http://docs.oracle.com/javase/1.5.0/docs/api/java/lang/ProcessBuilder.html#start())方法可以从同一个实例重复调用（可以更改属性值后再调用）。
每个ProcessBuilder实例管理着进程的如下属性：
- 一条命令（a command），可以用List来表示，也可以用变长字符串。命令后面的参数与命令合并在一起。如：“cmd /c dir”，可以拆分为三个字符串，也可以放到一个List中。
- 一个运行环境（an *environment*），提供系统变量，初始值是当前进程的环境变量（参考：[`System.getenv()`](http://docs.oracle.com/javase/1.5.0/docs/api/java/lang/System.html#getenv())）。
- 一个工作目录（a *working directory*），默认值为当前进程所在目录。
- 一个重定向错误流属性（a *redirectErrorStream* property），初始值为false。就是设置错误流是否重定向到标准输出流（合二为一），还是分开。 如果合并（属性值设置为true）的话，那么可以通过[`Process.getInputStream()`](http://docs.oracle.com/javase/1.5.0/docs/api/java/lang/Process.html#getInputStream())获取标准输出和错误输出。
## 二. 示范代码
这是一个执行调用外部命令（D:\\Software\\PngMate.exe）完成将gif文件（D:\\Work\\yz.gif）转换为png文件(D:\\Work\\yz.png)的方法。
### **1. 旧代码**
```
@Test
    // JDK1.5之前的做法
    public void testProcessOld() throws Exception {
        // 调用Runtime的命令，直接拼成一个可以在DOS中运行的命令即可。
        String cmd = "cmd /c PngMate.exe " + gifSource + " " + pngDest;
        
        // 命令执行环境，这里就是设置Path变量
        String[] envp = new String[]{
            "Path=" + System.getenv("Path") + exeDir
        };
        Process p = Runtime.getRuntime().exec(cmd, envp, new File(workDir));
        try {
            p.waitFor();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
```
### **2. 新代码**
```
@Test
    // JDK1.5之后的新方式
    public void testProcessNew() throws Exception {
        ProcessBuilder pb = new ProcessBuilder("cmd", "/c", "PngMate.exe", gifSource, pngDest);
        Map<String, String> env = pb.environment();
        env.put("Path", env.get("Path") + exeDir);    // 更方便的设置环境变量的方式
        pb.directory(new File(workDir));
        Process p2 = pb.start();
        try {
            p2.waitFor();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
    }
```
ProcessBuilder的优势在于，可以更改command、environment、working directory、redirectErrorStream property这些值，再次启动
这在需要循环处理任务、或者是多个相近的任务时非常有效。下面两行代码表示再转换一个文件。
```
pb.command("cmd", "/c", "PngMate.exe", "505.gif", "505.png");
        pb.start();
```
**完整代码**
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
package com.clzhang.sample.thinking;
import java.io.*;
import java.util.*;
import org.junit.Test;
// 这是一个执行调用外部命令（D:\\Software\\PngMate.exe）完成将gif文件（D:\\Work\\yz.gif）转换为png文件(D:\\Work\\yz.png)的方法。
public class JDK15Process {
    String exeDir = "D:\\Software"; // PngMate.exe文件存放目录
    String workDir = "D:\\Work";     // 工作目录
    String gifSource = "yz.gif";    // 需要转换的源文件，于工作目录下
    String pngDest = "yz.png";        // 转换后的目标文件，转换后存放于工作目录下
    @Test
    // JDK1.5之前的做法
    public void testProcessOld() throws Exception {
        // 调用Runtime的命令，直接拼成一个可以在DOS中运行的命令即可。
        String cmd = "cmd /c PngMate.exe " + gifSource + " " + pngDest;
        
        // 命令执行环境，这里就是设置Path变量
        String[] envp = new String[]{
            "Path=" + System.getenv("Path") + exeDir
        };
        Process p = Runtime.getRuntime().exec(cmd, envp, new File(workDir));
        try {
            p.waitFor();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    @Test
    // JDK1.5之后的新方式
    public void testProcessNew() throws Exception {
        ProcessBuilder pb = new ProcessBuilder("cmd", "/c", "PngMate.exe", gifSource, pngDest);
        Map<String, String> env = pb.environment();
        env.put("Path", env.get("Path") + exeDir);    // 更方便的设置环境变量的方式
        pb.directory(new File(workDir));
        Process p2 = pb.start();
        try {
            p2.waitFor();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        // ProcessBuilder的优势在于，可以更改command、environment、working directory、redirectErrorStream property这些值，再次启动
        // 这在需要循环处理任务、或者是多个相近的任务时非常有效。
        // 下面两行代码表示再转换一个文件。
        pb.command("cmd", "/c", "PngMate.exe", "505.gif", "505.png");
        pb.start();
    }
}
View Code
```
