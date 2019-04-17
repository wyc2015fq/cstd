# arm交叉编译工具搭建 - xiahouzuoxin - CSDN博客





2012年09月30日 21:01:15[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2555








操作平台：REL5.1

交叉编译工具版本：arm-linux-gcc 4.3.2          

附工具下载地址：[http://www.arm9.net/download-arm-linux-gcc-4.3.2.asp](http://www.arm9.net/download-arm-linux-gcc-4.3.2.asp)




1、进入root用户命令行模式，运行su root

2、拷贝下载的工具链到任何目录下，我放在   /mnt/hgfs/Linux/  目录下

3、进入工具链放置的目录下，cd /mnt/hgfs/Linux/ 

      解压工具链     
tar xzvf arm-linux-gcc-4.3.2.tgz -C /

      注：-C /     选项作用是指将工具链指解压到 /usr/local/ 目录下，这是默认的用户软件安装目录

      到此工具链可以使用了，但使用时需要加上工具链所在路径，比较麻烦，因此下面为系统添加环境变量。

4、为工具添加环境变量，修改 
/etc/profile 文件，

      找到对应位置添加 
pathmunge /usr/local/arm/4.3.2/bin，或者在末尾添加export PATH=/usr/local/arm/4.3.2/bin:$PATH，

      完后执行source /etc/profile

      查看环境变量方法，如export $PATH




      工具链安装完成，说白了就是一个解压到目标文件的过程，没有自己配置。

      可以使用 arm-linux-gcc -v  测试工具链是否安装成功。





