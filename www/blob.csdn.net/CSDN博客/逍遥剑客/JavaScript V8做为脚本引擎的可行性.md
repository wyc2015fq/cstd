# JavaScript V8做为脚本引擎的可行性 - 逍遥剑客 - CSDN博客
2011年05月16日 16:19:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3012
[JavaScript V8](http://code.google.com/p/v8/)的速度一流, 这里有个对比:http://shootout.alioth.debian.org/
PC上没有问题, V8的接口很全, 本身就是C++的, 很好集成
问题出在iOS和PowerPC上
见: [http://blog.owned.co.za/?p=327](http://blog.owned.co.za/?p=327)
大体的意思如下:
iOS有个限制: 可写的内存不能标记为可执行的(Writable memory cannot be flagged as executable on iOS)
所以JIT就不能一边编译一边执行了
Unity之所以可以, 是因为它是先编译好了的, 通过Mono载入DLL执行
至于PowerPC(XBOX360/PS3), v8暂时还没移植好
结论: 只在PC平台上可以用用-_-. 
