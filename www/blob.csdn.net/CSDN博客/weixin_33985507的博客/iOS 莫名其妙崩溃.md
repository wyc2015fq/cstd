# iOS 莫名其妙崩溃 - weixin_33985507的博客 - CSDN博客
2018年04月04日 17:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
libc++abi.dylib: terminate_handler unexpectedly threw an exception
经历过几次这样跳.m崩溃，输出这样一串错误。bt输出一堆乱七八糟的堆栈，后来仔细检查发现是xib或者Storyboard的关联问题。百度出来也有说是赋值对象类型错误，我没遇到过。建议检查xib或者Storyboard属性的关联，是不是连错了或者没有删除干净，是不是改了类名文件名但是相关的Storyboard ID没有修改。总之在修改文件名，属性或者其他东西的时候要留心，最好修改完删完运行一遍以便及时找到错误
