# 在Ubuntu上使用SystemTap - Arthur的随笔 - CSDN博客
2013年08月23日 11:08:58[largetalk](https://me.csdn.net/largetalk)阅读数：2838
[http://www.ningoo.net/html/2010/use_systemtap_on_ubuntu.html](http://www.ningoo.net/html/2010/use_systemtap_on_ubuntu.html)
虽然很早以前听说过，但基本没用过，最近[褚霸同学的介绍](http://blog.yufeng.info/archives/826)，勾起了我对这个东西的兴趣。最近在工作笔记本上装了个Ubuntu10.10做为主操作系统，因此正好在上面也实验学习下SystemTap。
安装systemtap
sudo apt-get install systemtap
Ubuntu Desktop默认没有安装kernel debug info的包，systemtap无法追踪内核信息。查看内核版本
ningoo@ning:~/stap$ uname -r
2.6.35-22-generic
从[这里下载](http://ddebs.ubuntu.com/pool/main/l/linux/)对应的kernel debug info包，安装
sudo dpkg -i linux-image-2.6.35-22-generic-dbgsym_2.6.35-22.35_i386.ddeb
至此内核追踪已经可以执行，但module的信息还需要多做些工作
sudo apt-get install elfutils
for file in `find /usr/lib/debug -name '*.ko' -print`
do
      buildid=`eu-readelf -n $file| grep Build.ID: | awk '{print $3}'`
      dir=`echo $buildid | cut -c1-2`
      fn=`echo $buildid | cut -c3-`
      mkdir -p /usr/lib/debug/.build-id/$dir
      ln -s $file /usr/lib/debug/.build-id/$dir/$fn
      ln -s $file /usr/lib/debug/.build-id/$dir/${fn}.debug
done
Hello world
ningoo@ning:~/stap$ sudo stap -ve 'probe begin { log("hello world") exit() }'
Pass 1: parsed user script and 72 library script(s)
 using 18896virt/12868res/1880shr kb, in 130usr/20sys/150real ms.
Pass 2: analyzed script: 1 probe(s), 2 function(s), 0 embed(s), 0 global(s)
 using 19160virt/13132res/1908shr kb, in 10usr/0sys/5real ms.
Pass 3: using cached /home/ningoo/.systemtap/cache
 /f1/stap_f10ab2aeba4f2da2c03646b27b4d3627_757.c
Pass 4: using cached /home/ningoo/.systemtap/cache
 /f1/stap_f10ab2aeba4f2da2c03646b27b4d3627_757.ko
Pass 5: starting run.
hello world
Pass 5: run completed in 0usr/30sys/297real ms.
参考：
[http://sourceware.org/systemtap/documentation.html](http://sourceware.org/systemtap/documentation.html)
[http://sourceware.org/systemtap/wiki/SystemtapOnUbuntu](http://sourceware.org/systemtap/wiki/SystemtapOnUbuntu)
