# 【Linux入门学习之】grep命令详解 - 江南烟雨 - CSDN博客
2011年12月19日 17:08:10[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：6289
**grep是通用正则表达式解析器（General Regular Expression Parser）的缩写。**
**一、grep命令的功能是分析一行信息，若其中有我们所需要的信息，就将其拿出来。需要注意的是它以整行为单位进行数据的选取。**
语法：grep [-acinv] [--color=auto] '要查找的字符串' filename
-a:将binary文件以text文件的方式查找数据
-c:计算找到查找字符串的次数
-i:忽略大小写的不同
-n:输出行号
-v:反向选择，显示出没有查找字符串的内容的行
--color-auto:将找到的字符串以特殊颜色显示
下面介绍几个使用grep命令的实例。
范例：
先将/etc目录下的manpath.config文件拷贝至/tmp文件夹下，来作实验。
cd /tmp
cp/etc/manpath.config .
将此文件中有包含MANPATH的那一行取出来。
Cat manpath.config| grep 'MANPATH'
范例：
与上例子相反，只要没有包含MANPATH的那一行就取出来。
cat manpath.config| grep -v 'MANPATH'
![](http://hi.csdn.net/attachment/201112/19/0_1324285621H6ku.gif)
**二、下面介绍grep的一些高级参数。**
grep [-A] [-B] [--color=auto]'查找字符串' filename'
-A:后面可加数字，为after的意思，除了列出该行以外，后续的n行也列出来。
-B:后面可加数字，为before的意思，除了列出该行以外，前面的n行也列出。
范例：
用dmesg列出内核信息，然后用grep找出包含eth的那行，并且显示 行号。而且将关键字的前2行和后3行也列出来。
Dmesg | grep -n-A3 -B2 –color=auto 'eth'
![](http://hi.csdn.net/attachment/201112/19/0_1324285671MT5o.gif)
在关键字的显示上，grep可以用—color=auto来将关键字用特殊颜色显示。但是每次使用grep都得加上这个信息很麻烦，于是可以用alias进行一下处理就OK了。可以在~/.bashrc内加上这一行：alias grep='grep –color=auto'。
**三、下面进行一些基础正则表达式的练习。**
1、利用中括号[]来查找集合字符
比如我要查找man或者men字符串，可以这样来查找：
grep -n 'm[ae]n'manpath.config
![](http://hi.csdn.net/attachment/201112/19/0_132428573047T2.gif)
Note：中括号[]里面不论有几个字符，它都只代表某一个字符。
2、反向选择^的使用
查找包含man而且前面没有/的那一行：
grep -n '[^/]man'manpath.config
![](http://hi.csdn.net/attachment/201112/19/0_132428577921vl.gif)
查找包含man但是前面不是小写字符的那一行：
grep -n'[^a-z]man' manpath.config
要取得有数字的那一行：
grep -n '[0-9]'manpath.config
考虑到语系对于编码顺序的影响，因此除了连续编码使用减号-之外，你也可以使用如下的方法取得前面测试的结果：
grep -n'[[:digit:]]' manpath.config
关于语系编码：
不同语系的编码数据并不相同，所以会造成数据选取结果的区别。举例来说，在英文大小写的编码顺序中，zh_CN.big5及C这两种语系的输出结果分别是：
LANG=C时：012345...ABCD...Z...abcd...z
LANG=zh_CN时：012345...aAbBcCdD...zZ。
因此在使用正则表达式时，需要特别留意当时环境的语系为何，否则可能会发现与别人不同的选取结果。
另外，为了避免这样编码所造成的英文和数字的选取问题，有些特殊的符号需要我们了解以下。主要有下面这些：
[:alnum:]代表英文大小写字符及数字
[:blank:]代表空格和tab按键
[:punct:]代表标点符号
[:space:]任何会产生空白的字符
[:alpha:]代表任何英文大小写字符
[:digit:]代表数字
[:lower:]代表小写字符
[:upper:]代表大写字符
3、行首^和行尾$字符
列出行首为MANPATH_MAP的行：
grep -n'^MANPATH_MAP' manpath.config
列出开头是大写字符的那一行：
grep -n '^[A-Z]'manpath.config
列出开头不是英文字母的行：
grep -n'^[^a-zA-Z]' manpath.config
Note:那个^符号在字符集合（中括号[]）之内和外面是不同的！！！在[]内面代表反向选择，在[]外面代表定位在行首的意思。
反过来思考：我们要找出行尾结束为感叹号.的行：
grep -n '\.$'manpath.config
因为小数点具有特殊的意义，所以必须用转义字符加以解除其特殊意义。
查找出空白行：
grep -n '^$'manpath.config
4、任意一个字符.与重复字符*
在bash当中，通配符*可以用来代表任意（0或多个）字符，但是正则表达式并不是通配符，两者之间是不相同的。至于正则表达式当中：
.:代表绝对有一个字符的意思。
*:代表重复前一个字符0到无穷多次的意思，为组合形态。
实例：
查找包含一个o以上的行，需要oo*：
grep -n 'oo*'manpath.config
查找以g开头与以g结尾，中间至少存在一个o的行：
grep -n 'goo*g'manpath.config
5、限定连续RE字符范围
在前面的例题中，我们可以利用.与RE字符及*来设置0到无穷多个重复字符。那如果我要限制一个范围区间内的重复字符呢，比如要找出2-5个o的连续字符串，就要用到限定范围的字符{}了。
但是{}的符号在shell有特殊意义，因此要用到转义字符\。
实例：
找出g后面有两个到五个o后面再接一个g的字符串：
grep -n '\{2,5\}g'manpath.config
如果是2个以上呢：
grep -n '\{2,\}g'manpath.config
这样就OK了。
