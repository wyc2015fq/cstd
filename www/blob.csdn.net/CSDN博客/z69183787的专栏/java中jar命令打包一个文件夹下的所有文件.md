# java中jar命令打包一个文件夹下的所有文件 - z69183787的专栏 - CSDN博客
2014年05月15日 13:55:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：23343
（1）首先，必须保证java的所有路径都设置好，在dos提示符下输入jar -help
出现C:\Documents and Settings\dly>jar -help
非法选项：h
用法：jar {ctxu}[vfm0Mi] [jar-文件] [manifest-文件] [-C 目录] 文件名 ...
选项：
    -c  创建新的存档
    -t  列出存档内容的列表
    -x  展开存档中的命名的（或所有的〕文件
    -u  更新已存在的存档
    -v  生成详细输出到标准输出上
    -f  指定存档文件名
    -m  包含来自标明文件的标明信息
    -0  只存储方式；未用ZIP压缩格式
    -M  不产生所有项的清单（manifest〕文件
    -i  为指定的jar文件产生索引信息
    -C  改变到指定的目录，并且包含下列文件：
如果一个文件名是一个目录，它将被递归处理。
清单（manifest〕文件名和存档文件名都需要被指定，按'm' 和 'f'标志指定的相同顺序。
示例1：将两个class文件存档到一个名为 'classes.jar' 的存档文件中：
       jar cvf classes.jar Foo.class Bar.class
示例2：用一个存在的清单（manifest）文件 'mymanifest' 将 foo/ 目录下的所有
           文件存档到一个名为 'classes.jar' 的存档文件中：
       jar cvfm classes.jar mymanifest -C foo/ .
则说明已经配置好了环境。
（2）打包mysample下的文件，则先切换到此目录下
在dos中输入jar   cvf   mysamlpe.war   *    或  */.(是打包多级目录)
则会将所有文件打包为一个名为mysamlpe.war的文件。
