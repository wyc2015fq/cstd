# GDB 调试 Nginx  磨刀不误砍柴工 - youbingchen的博客 - CSDN博客





2016年06月09日 08:53:26[youbingchen](https://me.csdn.net/youbingchen)阅读数：1482








# 源码分析工具

#### windows平台

首选的是sourceInsight

#### linux平台
- 利用Vi，Taglist,Cscope以及Ctag这几个工具 结合使用
- 有一个 更好的选择：Source Navigator

# 测试辅助工具
- wget (最好是wget-1.13之后的 版本)
- curl （显示请求的 详细信息 ）
- wireshark：抓包使用
- Nu:网络工具的瑞士刀
- fireFox：结合 firebug查看HTTP请求响应内容
- Opera：测试HTTP
- Hexdump： 看十六进制数据

# 利用GDB调试

```python
# 开启-g选项 并且去掉Gcc优化
./configure --with-cc-opt='-g -O0'
或者
CFLAGS="-g -O0" ./configure
```

接下来执行 make
`make `
接下来可以利用Gdb 

方法一：直接法

```
gdb --args  ./objs/nginx -c  /usr/local/nginx/conf/nginx.cof
或者
gdb ./objs/nginx
然后 set args -c /usr/local/nginx/conf/nginx.conf
再执行r命令
```

方法二：间接法

运行 ./objs/nginx

```bash
cd ./objs
./nginx
ps aux |  grep nginx #查看进程id
```

```
gdb  -p 进程号
或是
先启动 gdb 再 attach 进程号
```

方法三: core 文件

```bash
cd ./objs
./nginx
ps aux |  grep nginx #查看进程id
```

看到nginx启动了一个master进程（18237）和一个worker进程（18238）

```bash
# 在./objs文件夹中
gcore 18237 # 会生成 core.18237文件
```

关掉nginx进程

```bash
./nginx -s stop（或者直接使用kill）
gdb ./nginx core.18237
```

注意**因为nginx 是支持多进程的，如何实现进程的正确跟踪，一种方法 就是使用上面第三种的方法,另一种方法使用`set follow-fork-mode child` 同时修改nginx 的配置文件将`daemon off`让Nginx从main()函数开始 **

# GDB 进阶

### GDB 的watch指令

WatchPoints可以帮助我们监控某个变量在什么 时候被 修改。

```
# 设置断点
b 某个断点(初始化)
r
p &变量的地址
watch  上一步的 值
c
```

### cgdb

cgdb是一个 封装的gdb的开源调试工具

### 利用日志信息跟踪Nginx

### 利用strace/pstack 调试 Nginx

Linux下两个命令strace和ltrace可以分别用来查看 一个应用程序在运行过程中所引发的系统函数调用 和 动态库函数调用 。strace跟踪的 是 系统调用，对于Nginx本身的函数调用关系无法给出明确的 信息 ，想知道 Nginx当前内部到底发生了什么 ，那么 命令 pstack就是 一个很实用命令。



**总结：gdb不仅仅可以用到Nginx，还可以用到很多开源代码的调试当中**

# 获得Nginx程序完整的执行流程
- 使用System Tap
- 利用Gcc -finstrument-functions 选项

# 加桩调试

加桩调试就是为了让一个模块执行起来，额外添加一些支撑代码。比如简单测试一个排序程序算法的子函数的功能是否正常，也许会写一个main()函数，设置一个数组，提供一些乱选数据，通过被测试的函数接口，然后通过输出结果，看是否正常排序。

> 
一般来说，正常运行，Nginx本身对内存的申请与释放都是不可控的，所以直接去调试Nginx内存管理的机制相关代码逻辑是非常困难的。利用加桩的方法，ngx_slab_alloc()申请内存和ngx_slab_free()释放内存都能精确掌控的。


# 特殊应用逻辑调试

上面几种调试选择都是针对Nginx本身很容易跑到的逻辑，但是某些只有在特定情况才会被执行到的代码。 

比如Nginx里面有大量的超时处理，如果客户端请求头部数据超时，Nginx就将执行对应的超时处理函数，假设想通过单步执行的方式来了解这部分的逻辑关系，无非就是让Nginx的执行逻辑走到这条路径来，但是影响Nginx行为的决定是客户端所发送请求头部。一般的浏览器，如IE，Firefox等发出的请求行为基本已经固定，同时常用的curl、wget源代码又太复杂了。定制他们的请求动作和改变场景太麻烦了，所以一种较便利的方法就是我们自己写出socket通信的客户端。



