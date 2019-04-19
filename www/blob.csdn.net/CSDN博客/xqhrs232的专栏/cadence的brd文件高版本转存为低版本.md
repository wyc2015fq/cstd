# cadence的brd文件高版本转存为低版本 - xqhrs232的专栏 - CSDN博客
2018年01月02日 09:58:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2769
原文地址::[http://blog.sina.com.cn/s/blog_66eaee8f0102vvr0.html](http://blog.sina.com.cn/s/blog_66eaee8f0102vvr0.html)
相关文章
1、[Allegro PCB Designer 17.2如何打开旧版本.brd文件](http://blog.csdn.net/hanxuexiaoma/article/details/52386491)----[http://blog.csdn.net/hanxuexiaoma/article/details/52386491](http://blog.csdn.net/hanxuexiaoma/article/details/52386491)
cadence的brd文件如何实现高版本向低版本的转换？
cacence软件设计之初，能够实现高版本软件打开低版本软件生成的brd文件，但是反之，高版本存储的brd文件，低版本是无法打开的。
好在cadence16.3和16.6两个版本都给予了部分降版本的功能。
16.6能够降为16.5和16.3
16.3能够降为16.2和16.0
16.x的版本无法降为15.x的版本(或者有间接的方法）
![cadence的brd文件高版本转存为低版本](http://s5.sinaimg.cn/mw690/001SQWoTzy6PhlBRfBW34&690)
===================================
间接的转换方法：参考来自网络
![cadence的brd文件高版本转存为低版本](http://s9.sinaimg.cn/mw690/001SQWoTzy6PhxPL1ws38&690)
![cadence的brd文件高版本转存为低版本](http://s9.sinaimg.cn/mw690/001SQWoTzy6PhxPMqPS48&690)
![cadence的brd文件高版本转存为低版本](http://s3.sinaimg.cn/mw690/001SQWoTzy6PhxPLH1g52&690)
![cadence的brd文件高版本转存为低版本](http://s16.sinaimg.cn/mw690/001SQWoTzy6PhxPPOCH5f&690)
**说了这么多，还有一个问题，就是低版本软件打开高版本存储的brd文件，那么高版本到底是什么版本呢？**
**下面是获取版本的方法：**
Allegro确实非常好用，但是其软件不向上兼容的策略实在是不方便，低版本创建的文件用高版本软件编辑之后，低版本就再也打不
开该文件了，不像AutoCAD等工具可以保存成较低版本的格式。
如何查看创建.brd文件所用的Allegro版本呢？其实很简单，版本信息就包含文件本身内部。用记事本打开.brd，文件开头有一串字
符可显示编辑该brd软件的版本号及该brd所导入网表的版本号。
如：
Allegro 15.7创建，网表由Design Entry CIS15.7生成:
allv15-71/16/.....................netv15-71/15/allv15-71/1615.7
//-------------------------------
Allegro 15.7创建，用16.3打开之后另存，网表由Design Entry CIS15.7生成。
allv16-311/10allv15-71/16/.....................netv15-71/15
//------------------------------
Allegro 16.2创建，网表由Design Entry CIS16.2生成:
allv16-22/17/.netv16-22/17/allv16-22/17/  
//------------------------------
Allegro 16.2创建，用16.3打开之后另存，网表由Design Entry CIS16.2生成:
allv16-311/10......allv16-22/17/.netv16-22/17/allv16-22/17/
可见Allegro 会记录该.brd文件的创建软件版本和修改过该文件软件的版本，以及创建文件时导入的网表由哪个版本导出。具体这些
版本信息是如何组织的我们不得而知，不过能看到的这些信息也够我用的了。
![cadence的brd文件高版本转存为低版本](http://s10.sinaimg.cn/mw690/001SQWoTzy6PhxPCSEx49&690)

