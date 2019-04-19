# intellij idea run项目运行前先编译 - 左直拳的马桶_日用桶 - CSDN博客
2019年01月14日 19:22:46[左直拳](https://me.csdn.net/leftfist)阅读数：230
我知道，这种文章很low，但没办法，我现在还是一个java小白，而且年纪大了，记忆力差，一有点什么东西，都恨不得马上记录下来。
今天我跑一个项目，发现怎么各种修改都不起作用。后来才发现，run前并没有编译。真奇怪啊，运行前编译难道不应该是约定俗成的吗？但idea可不是，它灵活过头了，居然可以配置的。这个run config是我新生成的，不知为啥，居然没有在 launch 前加上builder。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190114192219723.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
自己加上就好了。
Build似乎没有什么用，+Build Project才有效果。
