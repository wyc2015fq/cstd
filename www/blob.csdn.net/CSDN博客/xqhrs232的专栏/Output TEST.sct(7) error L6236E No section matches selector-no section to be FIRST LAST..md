# Output\TEST.sct(7): error: L6236E: No section matches selector - no section to be FIRST/LAST. - xqhrs232的专栏 - CSDN博客
2017年06月03日 22:41:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1834
原文地址::[http://www.cnblogs.com/htsky/p/5946213.html](http://www.cnblogs.com/htsky/p/5946213.html)

击错误信息，跳转到了一个.sct文件：*.o (RESET, +First) 
按照如下操作，也不能解决问题。对比别的工程，也没找出问题。
“操作是： 
Options for Target ' xxxx '，里面的Linker选项下面： 
1、去掉勾选 Use Memory Layout from Target Dialog 
2、清空 Acatter File里面的内容 ”
后来重新捋了一下过程。我是从别人的项目继承过来的，原来是mdk4.7，并且是自带了很多“系统文件”。
我拿过来后，修改为自己的芯片型号等，从mdk5上的"manage run-time environment"里重新选择了需要的文件。
原来的启动文件与现有芯片型号不符，被删掉了。重新选择对应的启动文件，解决问题。
