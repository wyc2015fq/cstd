# 使用egypt+graphviz生成函数调用关系图示例 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 21:26:30[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：902
总结：
make  （-fdump-rtl-expand）  去除编译优化，比如-O3
egypt test.c.128r.expand >test.dot  可以手动打开dot文件去除一些孤立的点，
 主要是egypt的使用
dot test.dot -Tpng -o test.png
egypt  --omit av_ts_make_time_string,av_ts_make_string
  --include-external test.c.128r.expand 
其中--omit是忽略哪些函数
--include-external显示调用的第3方库函数，比如av_register_all， fwrite
dot选项中
-Grankdir=LR表示将图从左到右，而不是从上到下排列
环境：centos 64
1，安装perl
yum install perl 
为了防止后面“perl Makefile.PL”时出现“Can't locate ExtUtils/MakeMaker.pm”错误，需要：
yum install perl-devel
如果还不行：
yum install perl-CPAN
2，安装egypt： create call graph from gcc RTL dump
让编译器自己dump出调用关系，然后分析分析，交给Graphviz去绘图
下载[http://www.gson.org/egypt/](http://www.gson.org/egypt/)
链接：[http://pan.baidu.com/s/1bnwd2W3](http://pan.baidu.com/s/1bnwd2W3)密码：n3t9
perl Makefile.PL
 make
 make install
其文档位于[http://www.gson.org/egypt/egypt.html](http://www.gson.org/egypt/egypt.html)
3，安装Graphviz
yum list 'graphviz*'
yum install 'graphviz*'  
4，准备一份源码测试：
主要是要加上-fdump-rtl-expand编译选项，参见makefile文件 ，让gcc dump出egypt需要的文件
a,输入 make命令dump出egypt需要的文件 test.c.128r.expand
b,
egypt test.c.128r.expand >test.dot
如果有N个expand文件，可以
egypt *.expand > all.dot
图太大可用svg格式
c,
dot test.dot -Tpng -o test.png
生成的png文件:
附：dot文件内容
digraph callgraph {
"test2" -> "test3" [style=solid];
"main" -> "test1" [style=solid];
"main" -> "test2" [style=solid];
}
