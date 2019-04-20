# open("/dev/null",o_rdwr)的问题 - h13 - 博客园
 open（"/dev/null",..）
很多程序在一开始的时候要做这样的处理
/* 
* make sure we have stdin/stdout/stderr reserved
*/
while((t = open("/dev/null",O_RDWR)) < 3 && T >= 0)
if(t >= 3) close(t)
写入/dev/null的东西会被系统丢掉
就像注释写的那样，对stdin/stdout/stderr进行保护
代码中利用while先把文件描述符0，1，2分配出去，以后再分配
的时候就不会将stdin/stdout/stderr打开，以达到保护目的
