# html2txt - Orisun - 博客园







# [html2txt](https://www.cnblogs.com/zhangchaoyang/articles/2033613.html)





我们已经利用larbin从网上抓取了六千多个HTML文件,在使用clucene建立索引之前,我们还需要把HTML文件中的各种无用的标签去掉,形成只包含有实际内容的文档.html2text就是这样一个工具.

其实Linux下两种html2text工具:

![](https://pic002.cnblogs.com/images/2011/103496/2011050111171670.png)

Python Module for converting HTML to text

我们先说用Python写的那个,如果你会写点Python,我想你一定会选择这个.如果不使用Ubuntu软件中心,你需要这样来手动安装:

1)下载htmltotext-0.7.3.tar.gz

2)解压,进入解压后的文件

3)安装Python

$ sudo apt-get install python-dev

4)安装Python的

setuptools模块


# wget http://peak.telecommunity.com/dist/ez_setup.py
# python ez_setup.py

5)安装html2txt

# python setup.py install

6)测试安装是否成功

# python eggsetup.py test

Linux html2text

事实上我完全不懂Python,所以我用的是第一个.

用法:
  html2text -help
  html2text -version
  html2text [ -unparse | -check ] [ -debug-scanner ] [ -debug-parser ] \
     [ -rcfile <file> ] [ -style ( compact | pretty ) ] [ -width <w> ] \

    [ -o <file> ] [ -nobs ] [ -ascii ] [ <input-url> ] ...
Formats HTML document(s) read from <input-url> or STDIN and generates ASCII
text.
  -help          Print this text and exit
   显示本页文本并退出
  -version       Print program version and copyright notice
  -unparse       Generate HTML instead of ASCII output
  -check         Do syntax checking only　　　　　　　　 做语法检查
  -debug-scanner Report parsed tokens on STDERR (debugging)
  -debug-parser  Report parser activity on STDERR (debugging)
  -rcfile <file> Read <file> instead of "$HOME/.html2textrc"
  -style compact Create a "compact" output format (default)
  -style pretty  Insert some vertical space for nicer output
  -width <w>     Optimize for screen widths other than 79
  -o <file>      Redirect output into <file>
 将输入重新输出至 ＜file＞
  -nobs          Do not use backspaces for boldface and underlining
          这个选项要用着。不然的话转换后的文件 里会有很多没用的符号   
  -ascii         Use plain ASCII for output instead of ISO-8859-1

html转换成txt后编码没变。

指转换就要使用脚本了.

如果你会shell:

#!/bin/bash
ls *.html |while read i
do html2text -o $i.txt -nobs $i
done


当然我还是惯用Perl:

#!/usr/bin/perl -w
use strict;
use File::Find;
my $path=shift @ARGV;		#把$path下面的所有.html文件去掉HTML标签,并加上.txt后缀
sub html2txt{
	if(-e $File::Find::name){
		if(-f $File::Find::name){
			system "html2text","-o","$File::Find::name.txt", $File::Find::name;
		}
		elsif(-d $File::Find::name){
			next if $File::Find::name=~/^\./;
		}
	}
	else{
		print "$File::Find::name does not exist.\n";
	}
}
find(\&html2txt,$path);


麻烦并没有至此结束.生成的.txt文件用gedit打开是乱码,当然如果你打开时指定utf-8编码是可转以的,但六千多个文件我不能都这么干吧.

方法1.

使用vim打开,但是需要更改.vimrc

set encoding=utf-8 fileencodings=ucs-bom,utf-8,cp936

这样，就可以让vim自动识别文件编码（可以自动识别UTF-8或者GBK编码的文件），其实就是依照fileencodings提供的编码列表尝试，如果没有找到合适的编码，就用latin-1(ASCII)编码打开。

方法2.

iconv -f GBK -t UTF-8 file1 -o file2

就是把原文件从GBK编码转换这UTF-8编码.

那要是我不知道原文件是什么编码,也不知道应该把它转换为什么编码呢?

查看文件编码:

　　1)直接在vim中

　　:set fileencoding

　　2)使用enca工具,当然要先安装该工具

enca -L zh_CN file 检查文件的编码

enca -L zh_CN -x UTF-8 file 将文件编码转换为”UTF-8″编码

enca -L zh_CN -x UTF-8 < file1 > file2 如果不想覆盖原文件可以这样

除了有检查文件编码的功能以外，”enca”还有一个好处就是如果文件本来就是你要转换的那种编码，它不会报错，还是会print出结果来， 而”iconv”则会报错。这对于脚本编写是比较方便的事情。

要转换为什么编码:

这要看你下一步用什么中文分词工具了,ICTCLAS 3.0 2009 共享版只支持GBK编码，因此，如果是UTF-8编码的字符串，可以先用iconv函数转换成GBK编码，再用httpcws进行分词处理，最后转换回UTF-8编码。SCWS 支持 GBK，UTF-8，BIG5 等汉字编码，切词效率高。

另外linux下还一个工具是专门对文件名进行编码转换的,这主要是解决文件在Windows和Linux之间移动的时候文件乱码的问题.

令人沮丧的是

orisun@zcypc:~$ enca f00004.html.txt

Universal transformation format 8 bits; 

UTF-8  Surrounded by/intermixed with non-text data

它说我的文件不光是utf8,还有一些"

non-text data",gedit用utf8打开时还是乱码.

解决办法,这是我睡了一觉想出来的:既然用vim可以成功打开,那是否可以直接cat到终端呢?然后再用管道把stdout送给一个文件.

首先cat f00004.html.txt,在终端可以正确显示中文.

如果你的终端还是不能正常

显示中文,sudo apt-get install zhcon


cat f00004.html.txt 1>file

打开file发现还是乱码.

有一个html2text的网站,可以在线试用一下:http://www.aaronsw.com/2002/html2text/

Lynx

用html2text工具后还进行一步编码的转换,麻烦,所有想到了用Lynx.这是一个在字符界面查看网页的浏览器,当然看不到图片.由于在Lynx上显示的已经是网页的文本,去除了html标签和各种脚本代码,我们再把Lynx界面上的内容输出到文件里就可以了.

先要设置一下使你的Lynx支持中文,要先让配置文件lynx.cfg(locate一下)可更改,sudo chmod u+w lynx.cfg.然后运行lynx, O选项,设置语言.或者直接更改

lynx.cfg.
CHARACTER_SET:Chinese ASSUME_CHARSET:ChinesePREFERRED_LANGUAGE:zh_CN

lynx -raw -dump <url> > plain.txt 
或者 
cat <file> | lynx -raw -dump -stdin >plain.txt




Links和w3m

和Lynx类似的软件,不过同样默认情况下不支持中文,看来国人亟待为GNU作出自己的贡献.



-------------------------------------

最后不管了,反正把txt文件送给SCWS后它还是可以正常分词,分词结果用vim或cat打开都没问题.但愿下一步用clucene的时候支持复杂的编码.












