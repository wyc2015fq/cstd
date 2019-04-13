
# Here document使用方法总结 - 知行合一 止于至善 - CSDN博客

2019年02月05日 16:50:40[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：812标签：[EOF																](https://so.csdn.net/so/search/s.do?q=EOF&t=blog)[here																](https://so.csdn.net/so/search/s.do?q=here&t=blog)[document																](https://so.csdn.net/so/search/s.do?q=document&t=blog)[shell																](https://so.csdn.net/so/search/s.do?q=shell&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=document&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=here&t=blog)个人分类：[Shell脚本																](https://blog.csdn.net/liumiaocn/article/category/8590657)
[
																								](https://so.csdn.net/so/search/s.do?q=here&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=EOF&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=EOF&t=blog)

可能很多人都熟悉cat <<EOF的写法和功能，但是对于这个被称为Here Document的可能还不是非常清楚，这篇文章稍微整理一下相关知识，并结合简单使用示例来进行说明。
# 什么是Here Document
Here Document也被称为here-document/here-text/heredoc/hereis/here-string/here-script，在Linux/Unix中的shell中被广泛地应用，尤其在于用于传入多行分割参数给执行命令。除了shell（包含sh/csh/tcsh/ksh/bash/zsh等），这种方式的功能也影响和很多其他语言诸如Perl，PHP以及Ruby等。这篇文章以bash为例进行使用说明。
# 使用方式&限制
使用格式如下所示：
> 命令 << 分隔串（最为常见的为EOF）

> 字符串1

> …

> 字符串n

> 分隔串

> 使用限制

> 分割串常见的为EOF，但不一定固定为EOF，可以使用开发者自行定义的，比如LIUMIAO

> 缺省方式下第二个分割串（EOF）必须顶格写，前后均不可有空格或者tab

> 缺省方式下第一个分割串（EOF）前后均可有空格或者tab，运行时会自动剔除，不会造成影响

> 使用示例

> liumiaocn:~ liumiao$ cat << LIUMIAO
> hello
> world
> LIUMIAO
hello
world
liumiaocn:~ liumiao$
> 使用场景示例：交互式命令行的多行输入转换为batch方式

> 这个场景的说明可能比较绕口，但是一旦涉及实际的使用例子就会非常清晰。

> 交互式的命令行：比如sftp或者oracle的sqlplus，或者mysql的命令控制台，以sftp为例子，当我们输入sftp 用户名@sftp服务器登录之后，需要在sftp>的提示下进行各种sftp命令的操作。

> 多行输入：在sftp登录之后，如果希望进行（确认当前目录=>确认文件aa是否存在=>下载aa文件）操作的话，这需要按顺序执行pwd=>ls aa=>get aa三条命令。

> 转化为batch方式：很多时候上述的sftp命令可能是应用处理到某个时点被自动触发，这种人工逐行输入命令的方式不再适合。

> 上述实际操作示例如下：

> liumiaocn:~ liumiao$ sftp root@host131
Connected to root@host131.
sftp> pwd
Remote working directory: /root
sftp> ls
aa                anaconda-ks.cfg   
sftp> get aa
Fetching /root/aa to aa
/root/aa                                                                                                             100%    9     0.7KB/s   00:00    
sftp> exit
liumiaocn:~ liumiao$ cat aa
aa infor
liumiaocn:~ liumiao$
> 可以看到，用户操作时需要输入密码，然后需要分别输入三条命令，如果希望一次执行完毕，可以使用如下Here Document方式

> liumiaocn:~ liumiao$ sftp root@host131 <<EOF
> pwd
> ls
> get aa
> exit
> EOF
Connected to root@host131.
sftp> pwd
Remote working directory: /root
sftp> ls
aa                  anaconda-ks.cfg     
sftp> get aa
Fetching /root/aa to aa
/root/aa                                                                                                             100%    9     7.6KB/s   00:00    
sftp> exit
liumiaocn:~ liumiao$
> mysql或sqlplus也是一样，一般类似的这种交互式的命令都支持here document，可以实现这些常见的手工操作的自动化。

> 使用场景示例：脚本中自带配置文件或者源码

> 比如在配置kubernetes或者docker的时候，需要新建一些配置文件，这需要几个脚本和多个配置文件，但是由于这些配置文件都非常简单，而使用者为了方便往往更倾向于给一个一键安装脚本包含所有内容，一般使用cat 和here document结合实现配置文件在脚本中自动生成。这里举一个例子，比如使用gcc 编译一个.c文件输出 hello liumiao，然后执行，一般来说是需要这样的：

> 前提：需要一个脚本自行编译和对编译结果的执行，同时需要这个hello world的.c文件

> 常见的方式是这样的，首先.c文件是这样打招呼的

> [root@platform ~]\# cat helloworld.c 
\#include <stdio.h>
void main(){
  printf("hello world\n");
}
[root@platform ~]\#
> 使用gcc生成可执行文件并执行

> [root@platform ~]\# gcc helloworld.c -o hello
[root@platform ~]\# ./hello 
hello world
[root@platform ~]\#
> 可以看到，普通方式需要helloworld.c这个文件，如果把这个文件以HereDocument的方式嵌到脚本中，可能示例代码则会如下所示：

> [root@platform ~]\# cat testheredoc.sh 
\#!/bin/sh
echo "\#\# create c source file ..."
cat << EOF >herehelloworld.c
\#include <stdio.h>
void main(){
  printf("hello world by using here document\n");
}
EOF
echo "\#\# check c source file ..."
ls herehelloworld.c
echo "\#\# create binary file by using gcc"
gcc herehelloworld.c -o herehello
echo "\#\# execute herehello"
./herehello
[root@platform ~]\#
> 执行效果如下所示

> [root@platform ~]\# sh testheredoc.sh 
\#\# create c source file ...
\#\# check c source file ...
herehelloworld.c
\#\# create binary file by using gcc
\#\# execute herehello
hello world by using here document
[root@platform ~]\#
> 当然至于这样的写法是好还是不好，很难说，虽然少了一个配置文件，但是强耦合似乎显得不太合适，万一要修改呢。而实际的使用场景中，cat后生成的文件往往是各类设定文件，比如systemd下的service文件。由于配置文件需要临时生成而且可能会多变，其实还是有一定的使用场景的。不过平心而论，这样写确实很像病毒，一些比较弱的病毒也经常使用这种方式来绕过检查。

> 使用场景：配置文件中的变量

> 如果配置文件中使用了变量，需要注意的是，变量替换还是不替换可能需要认真考虑的事情，比如systemd下的docker的service文件。

> 在脚本中变量的使用是以$为开始的，不希望相关内容被转义可以使用如下方式：

> 方式1: 使用\进行包装

> 将不希望被转义的$全部使用\$替代，这里不再使用例子进行介绍

> 方式2: 使用’或者"或者\对第一个EOF进行封装

> 正常使用的情况如下：

> liumiaocn:~ liumiao$ cat <<EOF >t1
> $CMD1
> $CMD2
> $CMD3
> EOF
liumiaocn:~ liumiao$ cat t1

liumiaocn:~ liumiao$
> 可以看到生成的t1中$CMD1等三行都被替换了，因为当前没有定义这三个变量所有有了几个空行。如果希望$CMD1等都不被替换的话，还可以使用如下几种：

> cat << \EOF

> …

> EOF

> 使用示例如下：

> liumiaocn:~ liumiao$ cat << \EOF >t1
> $CMD1
> $CMD2
> $CMD3
> EOF
liumiaocn:~ liumiao$ cat t1
$CMD1
$CMD2
$CMD3
liumiaocn:~ liumiao$
> cat << ‘EOF’

> …

> EOF

> 使用示例如下：

> liumiaocn:~ liumiao$ cat << 'EOF' >t1
> $CMD1
> $CMD2
> $CMD3
> EOF
liumiaocn:~ liumiao$ cat t1
$CMD1
$CMD2
$CMD3
liumiaocn:~ liumiao$
> cat <<“EOF”

> …

> EOF

> 使用示例如下：

> liumiaocn:~ liumiao$ cat << "EOF" >t1
> $CMD1
> $CMD2
> $CMD3
> EOF
liumiaocn:~ liumiao$ cat t1
$CMD1
$CMD2
$CMD3
liumiaocn:~ liumiao$
> 使用技巧：<<- 与 <<的区别

> 使用<<-代替<<唯一的作用在与分割串所扩起来的内容，顶格的tab会被删除，用于ident。注意看一下如下的例子和执行结果，在hello和world前后加上tab和space，用于确认结果。

> liumiaocn:~ liumiao$ cat testhere.sh 
\#!/bin/sh
echo "\#\# test space with <<-"
cat <<- EOF
  hello
  	world
EOF
echo "\#\# test space with <<-"
cat <<- EOF
hello  
world  
EOF
echo "\#\# test tab with <<- "
cat <<- EOF
	hello
	world
EOF
echo "\#\# test tab with <<- "
cat <<- EOF
hello	
world	
EOF
liumiaocn:~ liumiao$
> 执行结果如下所示

> liumiaocn:~ liumiao$ sh testhere.sh 
\#\# test space with <<-
  hello
  	world
\#\# test space with <<-
hello  
world  
\#\# test tab with <<- 
hello
world
\#\# test tab with <<- 
hello	
world	
liumiaocn:~ liumiao$
> 可以看到唯一起作用的是第三个例子，顶格的tab没有被显示（由于space和tab的信息显示清楚，请读者自行验证和确认）


