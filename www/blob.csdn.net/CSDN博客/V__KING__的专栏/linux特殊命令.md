# linux特殊命令 - V__KING__的专栏 - CSDN博客





2015年12月07日 16:18:08[v__king__](https://me.csdn.net/V__KING__)阅读数：302







- ldd 

ldd sketch.elf 

查看具体调用了哪些动态软件库。- file sketch.elf 

查看文件具体属性
- dpkg -L libjpeg-dev 

查看libjpeg-dev这个库被安装到了哪里 

什么时候用到： 交叉编译后，需要所用到的库移植到目标平台的时候




