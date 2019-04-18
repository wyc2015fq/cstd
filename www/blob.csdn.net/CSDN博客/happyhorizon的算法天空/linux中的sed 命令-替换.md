# linux中的sed 命令-替换 - happyhorizon的算法天空 - CSDN博客
2018年05月22日 16:12:43[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：930
sed源自stream editor, 意为流编辑器. 是linux的三大文本编辑工具之一(seg, grep, awk). sed是一种在线编辑器,它一次只处理一行内容，并将结果发送到屏幕。
具体过程如下： 
1) 首先sed把当前正在处理的行保存在一个临时缓存区中（也称为模式空间）; 
2) 然后处理临时缓冲区中的行，完成后把该行发送到屏幕上。
sed每处理完一行就将其从临时缓冲区删除，然后将下一行读入，进行处理和显示。处理完输入文件的最后一行后，sed便结束运行。sed把每一行都存在临时缓冲区中，对这个副本进行编辑，所以不会修改原文件。
关于如何使用sed,请参考: 
- linux的sed命令: 
[http://www.runoob.com/linux/linux-comm-sed.html](http://www.runoob.com/linux/linux-comm-sed.html);  
[https://blog.csdn.net/holandstone/article/details/8488434](https://blog.csdn.net/holandstone/article/details/8488434)
- 正则表达式: [http://www.runoob.com/regexp/regexp-syntax.html](http://www.runoob.com/regexp/regexp-syntax.html)
- sed搜索常用表达式速查: [http://www.jb51.net/article/72867.htm](http://www.jb51.net/article/72867.htm)
# sed的基本语法
`sed [-hnV][-e'script'][-f script文件][文本文件]`
参数说明：
- -h或–help 显示帮助; -n或- -quiet或–silent 仅显示script处理后的结果; -V或- -version 显示版本信息。
- -e ‘script’]或- -expression=’script’ 以选项中指定的script来处理输入的文本文件。-e可以省略,用’’标注script就可以了.
- -f script文件或- -file=script文件 以选项中指定的script文件来处理输入的文本文件。-f也可以省略, 直接给出文件名称就可以.
但是! **用sed选项-e将改动直接修改到源文件是非常危险的, 可以采用重定向的方式保存sed的输出**,例如:
```bash
sed 'some-sed-commands' input_file>output_file
```
这里主要说说sed 的替换功能.
# sed替换指定文字
sed命令配合正则表达式可以替换指定文本,基本的script格式为:
```bash
sed 's/要被取代的字串/新的字串/g'
```
这里s表示替换命令, 命令末端的g表示在行内进行全局替换. $/$符号称为delimiter, sed命令支持的delimiter有很多,除了常见的$/$以外, 还有@, _, #, 甚至空格都是sed命令支持的delimiter. 丰富的delimiter支持可以让正则表达式script更好读. 参考: [http:///backreference.org/2010/02/20/using-different-delimiters-in-sed/](http:///backreference.org/2010/02/20/using-different-delimiters-in-sed/)
例如: 如果datafile中有多个My开头的文字，所有开头的My都被替换为You。
```bash
sed 's/^My/You/g' datafile
```
例如: 取消默认输出，处理1到20行里匹配以My结尾的行，把行内所有的My替换为You，并打印到屏幕上。
`sed -n '1,20s/My$/You/gp' datafile`
再举一个比较难的例子:取出一个文件路径的目录名称;
```bash
echo "/etc/rc.d/abc/edu/" | sed -r 's@^(/.*/)[^/]+/?@\1@g'
```
因sed支持扩展正则表达式，在扩展正则表达式中，+表示匹配其前面的字符至少1次. () 为子表达式, []为字符列表. 这里的delimiter符号是@. 对script每个部分详细展开:  
`s@^` 这里的^表示指定开头符号; 
`[^/]+/`则是在匹配的字符中匹配 “非/符号和/构成的字段”.  
`?` 匹配前面的子表达式零次或一次，或指明一个非贪婪限定符。 
`@\1@g` 语句的最后部分代表将s@…@中间正则表达式^(/.*/)[^/]+/?得到的匹配字符串全部替换为倒数第1个 “非/符号和/构成的字段”. 
取出一个文件路径的最后一个文件名；
```bash
echo "/etc/rc.d/abc/edu/" | sed -r 's@^/.*/([^/]+)/?@\1@g'
```
更多例子,值得仔细理解:  
1、删除/etc/grub.conf文件中行首的空白符； 
`sed  -r 's/^[[:space:]]+//' /etc/grub.conf`
2、替换/etc/inittab文件中“id:3:initdefault:”一行中的数字为5； 
`sed 's/\(id:\)[0-9]\(:initdefault:\)/\15\2/g' /etc/inittab`
3、删除/etc/inittab文件中的空白行； 
`sed '/^$/d' /etc/inittab`
4、删除/etc/inittab文件中开头的#号； 
`sed 's/^#//g' /etc/inittab`
5、删除某文件中开头的#号及其后面的空白字符，但要求#号后面必须有空白符； 
`sed 's/^#[[:space:]]\{1,\}//g' /etc/inittab`     或者 
`sed  -r 's/^#[[:space:]]+//g' /etc/inittab`
6、删除某文件中以空白字符后面跟#类的行中的开头的空白字符及# 
`sed -r 's/^[[:space:]]+#//g' /etc/inittab`
最后附上常用的正则表达式备忘表: 
![这里写图片描述](https://img-blog.csdn.net/2018052216164152?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
