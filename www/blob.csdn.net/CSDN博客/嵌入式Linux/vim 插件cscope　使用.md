
# vim 插件cscope　使用 - 嵌入式Linux - CSDN博客

2015年09月23日 17:37:32[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：324个人分类：[Vim																](https://blog.csdn.net/weiqifa0/article/category/6018903)



１．安装　sudo apt-get install cscope
２．初始化　cscope -Rbq　你想在哪个目录下面用这个功能，就在哪个目录下面运行这个命令
３．vim kpd.c
４．输入　:cs add cscope.out　先初始化一下
然后想看一下帮助 :cs -b
或者查找某个字符串 :cs find t weiqifa

add ：增加一个新的cscope数据库/链接库
[?](http://my.oschina.net/u/572632/blog/267471#)
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|使用方法：
|:cs add {|file|||dir|} [pre|-|path] [flags]
|其中：
|[pre|-|path] 就是以|-|p选项传递给cscope的文件路径，是以相对路径表示的文件前加上的path，这样你不要切换到你数据库文件所在的目录也可以使用它了。
|[flags] 你想传递给cscope的额外旗标
|实例：
|:cscope add|/|root|/|code|/|vimtest|/|ftpd
|:cscope add|/|project|/|vim|/|cscope.out|/|usr|/|local|/|vim
|:cscope add cscope.out|/|usr|/|local|/|vim –C
|

find ：查询cscope。所有的cscope查询选项都可用除了数字5（“修改这个匹配模式”）。
[?](http://my.oschina.net/u/572632/blog/267471#)
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|使用方法：
|:cs|find|{querytype} {name}
|其中：
|{querytype} 即相对应于实际的cscope行接口数字，同时也相对应于nvi命令：
|0或者s   —— 查找这个C符号
|1或者g  —— 查找这个定义
|2或者d  —— 查找被这个函数调用的函数（们）
|3或者c  —— 查找调用这个函数的函数（们）
|4或者t   —— 查找这个字符串
|6或者e  —— 查找这个|egrep|匹配模式
|7或者f   —— 查找这个文件
|8或者i   —— 查找|\#include这个文件的文件（们）
|实例：（|\#号后为注释）
|cscope|find|c ftpd_send_resp|\# 查找所有调用这个函数的函数（们）
|:cscope|find|3 ftpd_send_resp|\# 和上面结果一样
|:cscope|find|0 FTPD_CHECK_LOGIN|\# 查找FTPD_CHECK_LOGIN这个符号
|执行结果如下：
|Cscope tag: FTPD_CHECK_LOGIN
|\#   line  filename / context / line
|1     19  ftpd.h <<GLOBAL>>
|\#define FTPD_CHECK_LOGIN() /
|2    648  ftpd.c <<ftpd_do_pwd>>
|FTPD_CHECK_LOGIN();
|3    661  ftpd.c <<ftpd_do_cwd>>
|FTPD_CHECK_LOGIN();
|4    799  ftpd.c <<ftpd_do_list>>
|FTPD_CHECK_LOGIN();
|5    856  ftpd.c <<ftpd_do_nlst>>
|FTPD_CHECK_LOGIN();
|6    931  ftpd.c <<ftpd_do_syst>>
|FTPD_CHECK_LOGIN();
|7    943  ftpd.c <<ftpd_do_size>>
|FTPD_CHECK_LOGIN();
|8    960  ftpd.c <<ftpd_do_dele>>
|FTPD_CHECK_LOGIN();
|9    981  ftpd.c <<ftpd_do_pasv>>
|FTPD_CHECK_LOGIN();
|Enter nr of choice (<CR> to abort):
|然后输入最前面的序列号即可。
|

help ：显示一个简短的摘要。
[?](http://my.oschina.net/u/572632/blog/267471#)
|1
|2
|使用方法：
|:cs help
|

kill  ：杀掉一个cscope链接（或者杀掉所有的cscope链接）
[?](http://my.oschina.net/u/572632/blog/267471#)
|1
|2
|3
|4
|使用方法：
|:cs|kill|{num|partial_name}
|为了杀掉一个cscope链接，那么链接数字或者一个部分名称必须被指定。部分名称可以简单的是cscope数据库文件路径的一部分。要特别小心使用部分路径杀死一个cscope链接。
|假如指定的链接数字为-1，那么所有的cscope链接都会被杀掉。
|

reset：重新初始化所有的cscope链接。
[?](http://my.oschina.net/u/572632/blog/267471#)
|1
|2
|使用方法：
|:cs reset
|

show：显示cscope的链接
[?](http://my.oschina.net/u/572632/blog/267471#)
|1
|2
|使用方法：
|:cs show
|

假如你在使用cscope的同时也使用ctags，|:cstag|可以允许你在跳转之前指定从一个或另一个中查找。例如，你可以选择首先从cscope数据库中查找，然后再查找你的tags文件（由ctags生成）。上述执行的顺序取决于|csto|的值。
|:cstag|当从cscope数据库中查找标识符时等同于“:cs find g”。
|:cstag|当从你的tags文件中查找标识符时等同于“|:tjump|”。



