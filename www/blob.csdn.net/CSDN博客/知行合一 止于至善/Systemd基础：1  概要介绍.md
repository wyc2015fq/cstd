
# Systemd基础：1: 概要介绍 - 知行合一 止于至善 - CSDN博客

2019年04月08日 19:55:24[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：86标签：[systemd																](https://so.csdn.net/so/search/s.do?q=systemd&t=blog)[linux																](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[介绍																](https://so.csdn.net/so/search/s.do?q=介绍&t=blog)[journalctl																](https://so.csdn.net/so/search/s.do?q=journalctl&t=blog)[init																](https://so.csdn.net/so/search/s.do?q=init&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=journalctl&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=介绍&t=blog)个人分类：[Unix/Linux																](https://blog.csdn.net/liumiaocn/article/category/6328278)
[
																								](https://so.csdn.net/so/search/s.do?q=介绍&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=systemd&t=blog)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
Systemd在不知不觉中已经得到了大多数Linux发型版的支持，这系列文章中将来介绍一下在争议中砥砺前行的Systemd。
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# Systemd是什么
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Systemd中的d是daemon（守护进程）的缩写，使用ps即可看到这个进程systemd的存在。看一下ps的systemd的进程号，你就会意识到它的作用，就是那个存在了很长时间的pid为1的init进程，终于改名字了。
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@liumiaocn ~
```
```python
]
```
```python
# ps -ef |grep systemd |grep -v grep
```
```python
root         1     0  0 Mar31 ?        00:02:20 /usr/lib/systemd/systemd --switched-root --system --deserialize 22
root      1402     1  0 Mar31 ?        00:20:03 /usr/lib/systemd/systemd-journald
root      1431     1  0 Mar31 ?        00:00:02 /usr/lib/systemd/systemd-udevd
root      2604     1  0 Mar31 ?        00:00:08 /usr/lib/systemd/systemd-logind
dbus      2607     1  0 Mar31 ?        00:00:36 /usr/bin/dbus-daemon --system --address
```
```python
=
```
```python
systemd: --nofork --nopidfile --systemd-activation
```
```python
[
```
```python
root@liumiaocn ~
```
```python
]
```
```python
#
```
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 发起的原因
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)在Redhat工作的工程师Lennart Poettering 和 Kay Sievers完成了Systemd最初版本的开发，发起的原因也非常简单，就是对init守护进程的改进。
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# Systemd的目标
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)在2014年， Poettering对于systemd的作用，提到了如下三个通用功能：
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)系统和服务管理
作为开发其他软件的基础平台
起到应用和内核之间的粘合作用，提供各种由内核提供的功能性接口
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# Systemd包含什么
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)虽然又一个systemd名称的守护进行，但systemd并不只是这个pid为1的damon进程，从上文的ps结果你还可以看到systemd-journald和systemd-logind等子进程的存在。事实上Systemd是由一系列守护进程所组成，考虑到Poettering关于其通用功能的三个目标，在2013年发布的时候Systemd包含了69个组件也不会让我们感到太意外了。当然Systemd引起了多少非议，有兴趣的可以看一下这篇文章：The Biggest Myths，这篇文章针对30条主要的非议进行了答复，比如：
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Systemd 不符合Unix哲学
Systemd是monolithic的巨石应用因为由69个组件组成
…
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)这篇文章中充满了辩论的智慧，比如：A package involving 69 individual binaries can hardly be called monolithic. 值得每个有志于口水战的技术从业人员学习，毕竟生活和技术可能充满很多无奈。
[
](https://img-blog.csdnimg.cn/20190408194350698.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)关于Systemd为什么会引起那么多口水，看一下Tizen（三星所使用的基于Linux的移动终端操作系统）对于起所使用的systemd的描述就会了解
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190408102318741.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 支持的平台
在众多的非议之中，systemd已然在大部分Linux发行版中站稳脚跟。截止到目前为之，可以看到基本上主流的Linux发行版已然支持。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190408102617497.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 不支持Systemd的linux联盟
截止到目前为止，非主流的Linux发行版中，还有不少without systemd
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190408102829691.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 永远在路上的Systemd
Poettering描述systemd使用了如下的语句：“never finished, never complete, but tracking progress of technology“，他认为systemd应该追随技术的脚步不停更新，小哥也用他的行动证实了这一点，作为systemd功能的最为核心的提供者一直从未停下脚步。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190408103314519.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
把争议放在一边不谈，这种想法是一个真正的技术人员应该拥有的。在后面也将会结合实际的简单例子来介绍Systemd的使用方法。
# 其他相关文章
目录：Systemd实践指南
[https://liumiaocn.blog.csdn.net/article/details/89079485](https://liumiaocn.blog.csdn.net/article/details/89079485)
其他不太相关文章
口水：The Biggest Myths
[http://0pointer.de/blog/projects/the-biggest-myths.html](http://0pointer.de/blog/projects/the-biggest-myths.html)
参考内容
[https://en.wikipedia.org/wiki/Systemd](https://en.wikipedia.org/wiki/Systemd)
[https://en.wikipedia.org/wiki/Category:Linux_distributions_without_systemd](https://en.wikipedia.org/wiki/Category:Linux_distributions_without_systemd)
[https://www.freedesktop.org/wiki/Software/systemd/](https://www.freedesktop.org/wiki/Software/systemd/)
[https://en.wikipedia.org/wiki/Tizen](https://en.wikipedia.org/wiki/Tizen)
[http://0pointer.de/blog/projects/systemd.html](http://0pointer.de/blog/projects/systemd.html)

