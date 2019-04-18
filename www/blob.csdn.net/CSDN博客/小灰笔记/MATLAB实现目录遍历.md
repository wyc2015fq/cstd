# MATLAB实现目录遍历 - 小灰笔记 - CSDN博客





2016年12月25日 19:34:39[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：965








之前工作中需要实现目录遍历的时候一直依赖于Python的OS模块，后来发现了Perl中的find。在使用MATLAB的时候，如果需要用到目录遍历进行某些处理的时候一般也是借助于这两种工具。今天终于发现，MATLAB也有相应的功能。说起来，MATLAB的功能集成也真够令人佩服了！




当然，MATLAB的使用，我还是比较肤浅。如今的MATLAB都不能够用编程语言来描述了，只能够以平台来称呼。




今天发现的这个MATLAB功能是genpath函数，传入的参数是一个目录，返回一个以分号分隔的子目录拼接字符串。




如果能够实现这么一个子目录的清单输出功能，一个简单的遍历已经很简单了。




1，编写简单的脚本代码如下

dir_name = genpath(pwd);

dirs = regexp(dir_name,';','split');

[long,wide] = size(dirs);

for i = 1:wide

    disp(dirs{1,i});

end




2，脚本运行结果如下

![](https://img-blog.csdn.net/20161225193434280?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




只截取了结果中的部分截图，但是这种结果已经不错了。如果要遍历处理什么文件，一个简单的循环嵌套就基本实现了。




