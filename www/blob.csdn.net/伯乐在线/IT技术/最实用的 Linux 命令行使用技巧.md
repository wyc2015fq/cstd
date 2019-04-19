# 最实用的 Linux 命令行使用技巧 - 文章 - 伯乐在线
原文出处： [Seco Max](https://dzone.com/articles/most-useful-linux-command-line-tricks)   译文出处：[开源中国 - leoxu](https://www.oschina.net/translate/most-useful-linux-command-line-tricks)
我们可能每天都会要使用到很多的 [Linux 命令行](https://likegeeks.com/main-linux-commands-easy-guide/)。 我们也会网络上知晓一些使用它们的小技巧，但是如果我们没有时常来进行练习，就有可能会忘掉怎么去使用它们。 所以我就决定把那些你可能会忘记的小提示和小技巧，或者对你而言可能是全新的东西都列表来。
### 将输出内容以表格的形式显示出来
有时，在你看到命令行执行的输出时，因为字符串过度拥挤（比如说 mount 命令的输出）导致输出内容难以识别。如果我们看到的内容是一张表格会如何呢？其实这是很容易做到的！


```
mount | column –t:
```
![](http://jbcdn2.b0.upaiyun.com/2017/08/8105f229bd5aab1933e69a673b050b77.png)
在此例中，由于内容中留了空格，所以输出的形式就美观了起来。 那如果想要的分隔符是别的什么符号，比如说冒号，又该怎么去做呢？ （例如，在 cat/etc/passwd 的输出内容中使用）
这时候只需要使用 -s 参数指定分隔符就行了，像下面这样。


```
cat /etc/passwd | column -t -s:
```
![](http://jbcdn2.b0.upaiyun.com/2017/08/ef22e95df16fb93a17c96954e6950305.png)
### 重复执行一个命令，直到它运行成功
如果在 Google 上搜索这个功能，你会发现很多人都问到了如何重复执行命令，直到这个命令成功返回并且运行正常。 Google上的建议里就包括 ping 服务器，直到它变得空闲为止，还有就是检查是否有向特定的目录上传了具有特定扩展名的文件，还有就是检查特定的URL是否已经存在，诸如此类的办法。
其实你还可以使用 while true 的循环来实现来实现这个功能：
![](http://jbcdn2.b0.upaiyun.com/2017/08/aa7339a9cd7cf1369192fd1007cb76cc.png)
在上面这个示例中，>/dev/null 2>＆1 会让程序的输出重定向到 /dev/ null。标准错误和标准输出都会被包含进去。
这是我认为最酷的Linux命令行技巧之一。
### 按内存资源的使用量对进程进行排序


```
ps aux | sort -rnk 4:
```
![](http://jbcdn2.b0.upaiyun.com/2017/08/02ff6a80a4e1632d459fbe1ff580d8fb.png)
### 按 CPU 资源的使用量对进程进行排序


```
ps aux | sort -nk 3:
```
![](http://jbcdn2.b0.upaiyun.com/2017/08/a71a5974b1fbae9a9614f2f65d66bfdd.png)
如果想要检查你的机器架构，那就执行 getconf LONG_BIT。
### 可以同时查看多个日志文件
毫无疑问，你可能已经会使用 tail  命令来查看日志文件了，但可能有时候你还想着能同时查看多个日志文件。 使用 multi-tail 命令就支持支持文本的高亮显示，内容过滤以及更多你可能需要的功能：
![](http://jbcdn2.b0.upaiyun.com/2017/08/ba6ceb511883799d251d17f9b4e28f0e.png)![](http://jbcdn2.b0.upaiyun.com/2017/08/d81c3712dd9b9986860c3e89467852fb.png)
如果系统里还没有这个命令，运行 apt-get install multitail 命令就可以把它给装上。
### 回到你操作过的上一个目录去
只需敲入 cd – 就会返回到你操作过的上一个目录中去。
### 让非交互式 Shell 会话可进行交互
想要这样做，那就把设置从 ~/.bashrc 改成 ~/.bash_profile 吧。
### 定时的监视性命令输出
使用 watch 命令（watch df -h），你就可以查看到任何命令的任何输出。例如，你可以查看可用空间以及它的使用量增长情况。
通过利用 watch 命令来操作会变化的数据，你可以尽情想象自己能拿这个来做些什么哦。
### 在会话关掉以后继续运行程序
如果你在后台运行了什么程序，然后你关掉了的 shell 会话，那么这个后台运行的程序就会被你的 shell 杀掉。关闭 shell 以后怎么让程序继续运行下去呢？
这个可以用 nohup 命令做到 – 该指令表示不做挂断操作：


```
nohup wget site.com/file.zip
```
此命令是最容易被遗忘的 Linux 命令行技巧之一，因为我们许多人都会使用另外一个类命令的 screen 环境：
![](http://jbcdn2.b0.upaiyun.com/2017/08/8e6ac97ad36188d5a566a851d1e2c8c5.png)
这样会在同一个目录下生成一个名称为 nohup.out 的文件，其中包含了正在运行的程序的输出内容：
![](http://jbcdn2.b0.upaiyun.com/2017/08/73f4a9d25594c685db6714ee46d96810.png)
命令很酷，对吧？
### 自动对任何命令回答 Yes 或者 No
如果你想自动化需要向用户说的 Yes 的过程，可以使用 yes 命令来实现：yes | apt-get update。
也许你想做的是自动地说“No”。这个可以使用 yes no | 命令来实现。
![](http://jbcdn2.b0.upaiyun.com/2017/08/9d3ada6e55c6b9738b0dc40667db4c50.png)
### 创建具有指定大小的文件
可以使用 dd 命令来创建出具有指定大小的文件：dd if=/dev/zero of=out.txt bs=1M count=10。
这样就会创建出一个 10 MB 的文件，填充零作为内容：
![](http://jbcdn2.b0.upaiyun.com/2017/08/caf1d4163993de8e51e0eb04e45c59d6.png)
### 以根目录用户来运行最后一个命令
有时，你会忘记在需要 root 权限的命令之前敲入 sudo。这时候你没必要去重写命令; 只要输入 sudo 就行了！
![](http://jbcdn2.b0.upaiyun.com/2017/08/1f79f8f42fcd5f08552b17cc3398d258.png)
### 对命令行会话进行记录
如果想要把自己在 shell 屏幕上敲的内容记录下来，可以使用 script 命令将所有敲写的内容保存到一个名为 typescriptscript 的文件中去。
等你敲入 exit 命令以后，所有命令就都会被写入该文件，以便你事后再回过头去查看。
### 用标签符号替换空格符
可以使用 tr 命令替换任何字符，这个用起来非常方便：cat geeks.txt | tr ‘:[space]:’ ‘\t’ > out.txt.
![](http://jbcdn2.b0.upaiyun.com/2017/08/7dcc5cc23045ed18fd3ed993c3065028.png)
### 将文件内容转换为大写或者小写
可以这样实现：cat myfile | tr a-z A-Z> output.txt。
### 强大的 Xargs 命令
xargs 命令是最重要的 Linux 命令行技巧之一。你可以使用这个命令将命令的输出作为参数传递给另一个命令。例如，搜索 png 文件然后对其进行压缩或者其它操作：


```
find. -name *.png -type f -print | xargs tar -cvzf images.tar.gz
```
又或者你的文件中有一个 URL 的列表，而你想要做的是以不同的方式下载或者处理这些 URL，可以这样做：


```
cat urls.txt | xargs wget
```
![](http://jbcdn2.b0.upaiyun.com/2017/08/219b583b1bb3cac2c59130e9bb7e62bf.png)
![xargs command](http://jbcdn2.b0.upaiyun.com/2017/08/fc0b4e63f0d1e1009371efcca207fb23.png)
请你要记得，第一个命令的输出会在 xargs 命令结尾处传递。
那如果命令需要中间过程的输出，该怎么办呢？这个简单！
只需要使用 {} 并结合 -i 参数就行了。如下所示，替换在第一个命令的输出应该去的地方的参数：


```
ls /etc/*.conf | xargs -i cp {} /home/likegeeks/Desktop/out
```
这只是 Linux 命令行技巧其中的几个而已。你可以使用其他命令来做一些更 HAPPY 的事情，比如 awk 命令和 sed 命令！
如果你知道一些我在这里没有提到的任何其它的极客命令，欢迎评论。
