# 中文环境下PostgreSQL的使用 - DoubleLi - 博客园






虽然官方文档有提到编码的问题，但是对于中文讲的比较简单，给中文的PostgreSQL用户带来很多困扰，本文简单简述一下中文环境下PostgreSQL如何正确设置编码。

一、服务器端的编码设置

PostgreSQL在服务器端只支持两种简体中文编码: EUC_CN和UTF-8，而由于windows不支持EUC所以，在windows环境底下只能选UTF-8。

1. 安装时的编码设置

1.1 Linux下的中文编码设置

设置为EUC_CN
[localhost ~]$ initdb -E EUC_CN -D data7 --locale=zh_CN

设置为UTF-8
[[galy@localhost](mailto:galy@localhost) ~]$ initdb -E UTF-8 -D data7 --locale=zh_CN.UTF-8

1.2 Windows下的中文编码设置

设置为UTF-8
initdb.exe -E UTF8  -D c:\data3  --locale=chinese

2. 常见的问题：

2.1 不支持的服务器编码

如果环境的默认编码是PostgreSQL不支持的话，初始化的时候会报错，如下面环境的中文编码为GBK，而PG不支持GBK,所以报错。
[localhost ~]$ export LANG=zh_CN.gbk
[localhost ~]$ initdb -D testencoding

The database cluster will be initialized with locale zh_CN.gbk.
initdb: locale zh_CN.gbk requires unsupported encoding GBK
Encoding GBK is not allowed as a server-side encoding.
Rerun initdb with a different locale selection.

2.2 locale和编码有冲突

如果指定的locale和编码格式有冲突，同样也会报错。解决的方便是，指定编码兼容的locale.

[localhost ~]$ locale
LANG=zh_CN.gbk
LC_CTYPE="zh_CN.gbk"

默认的locale是中文gbk, 而下面的编码确实EUC_CN,因而系统报错，解决的方法是，指定--locale=zh_CN

[localhost ~]$ initdb -E EUC_CN -D data6
The files belonging to this database system will be owned by user "galy".
This user must also own the server process.

The database cluster will be initialized with locale zh_CN.gbk.
initdb: encoding mismatch
The encoding you selected (EUC_CN) and the encoding that the
selected locale uses (GBK) do not match.  This would lead to
misbehavior in various character string processing functions.
Rerun initdb and either do not specify an encoding explicitly,
or choose a matching combination.

initdb: 警告: 编码不匹配
您选择的编码 (EUC_CN) 和所选择的语言环境使用的编码 (GBK) 不匹配的.
这样将导致处理不同字符串的函数时产生错误.
要修复此问题, 重新运行 initdb 并且不要明确指定编码, 或者先选择一个匹配
组合类型.

二、客户端的编码

虽然PG支持客户端和服务器端的编码自动转换，但是还需要遵从一个原则：本地环境的编码和客户端编码需一致。否则将会出现乱码或者是其他问题。

本地环境的编码为GBK
C:\Program Files\PostgreSQL\9.1\bin>chcp
活动代码页: 936

数据库的编码为UTF8
=#\t
Name              | lenovo
Owner             | lenovo
Encoding          | UTF8
Collate           | Chinese (Simplified)_People's Republic of China.936
Ctype             | Chinese (Simplified)_People's Republic of China.936
Access privileges |

客户端编码为GBK
lenovo=# \encoding
GBK

现在本地环境和客户端编码都是GBK,所以没有问题
客户端输入的GBK字符，会自动转化为UTF-8
lenovo=# insert into test values('测试');
INSERT 0 1

服务器端的UTF-8编码，传到客户端时候，也会自动转换编码为GBK：
lenovo=# select * from test;
 name
------
 测试
(1 row)

如果客户端设置为UTF-8的话,和本地环境的GBK不一致则会出现问题
lenovo=# \encoding UTF-8

由于客户端编码和服务器端一致，为UTF-8,所以不转换传到客户端，而操作系统把它当作GBK显示，所以显示乱码：
lenovo=# select * from test;
 name
------
 娴嬭瘯
(1 row)

而插入的字符则直接以GBK编码的形式传到服务器端，服务器端认识不了，所以报错。
lenovo=# insert into test values('测试');
ERROR:  invalid byte sequence for encoding "UTF8": 0xb2
ERROR:  invalid byte sequence for encoding "UTF8": 0xb2

输入和显示都有问题，这是因为如果客户端和服务器的编码都一致的话，则不进行转码，而输入是按照本地环境的GBK进行编码，GBK的编码进入UTF-8的库当然会有问题。

三、显示信息的中文话

PostgreSQL支持多语言显示提示信息，如果希望服务器端及客户端的提示信息为中文，还需要进行一些额外的设置。

首先， 编译的时候需要加上自然语言支持模块

./configure --enable-nls

其次，在参数配置文件postgresql.conf设置信息为中文

lc_messages = 'zh_CN'                     # locale for system error message

这样在服务器端和客户端的提示信息都显示为中文

[localhost ~]$
日志:  已启动autovacuum
日志:  数据库系统准备接受连接

# dsd;
错误:  语法错误 在 "dsd" 或附近的 第 1 个字符处
语句:  dsd;
错误:  语法错误 在 "dsd" 或附近的。









