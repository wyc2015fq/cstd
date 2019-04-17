# Linux使用记录2 - 浅梦的博客 - CSDN博客





2017年03月02日 16:34:40[浅梦s](https://me.csdn.net/u012151283)阅读数：224标签：[Linux](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)
个人分类：[Linux](https://blog.csdn.net/u012151283/article/category/6757866)









# 文本编辑器

## 基础编辑器nano,gedit
- 不使用编辑器创建文件 

重复使用`echo`

```bash
$ echo line one > myfile
$ echo line two >> myfile
$ echo line three >> myfile
```

`>`将指令输出到文件中，`>>`将指令的输出附加的已存在的文件中。 

第二种方法是使用`cat`和重定向

```bash
$ cat << EOF > myfile
> line one
> line two
> line three
> EOF
$
```

这两种方法产生的结果相同。 

2. nano 
`nano <filename>`打开一个文件，如果不存在将会被创建。 

3. gedit 

图形化界面的编辑器 
`gedit <filename>`
## 高级编辑器vi和emacs
- vi 
`vimtutor`查看教程 
![这里写图片描述](https://img-blog.csdn.net/20170302161951344?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Working with Files in vi 

下面的表格描述了vi中开始，退出，读写文件的最重要的命令。所有命令输入完毕都要按回车执行。|Command|Usage|
|----|----|
|vi myfile|开启vi编辑器，编辑myfile|
|vi -r myfile|从系统崩溃中以恢复模式开启vi并编辑|
|:r file2|读入file2，并插入到当前位置|
|:w|写到文件中|
|:w myfile|将文件写到myfile中|
|:w! file2|覆盖写file2|
|:x or :wq|退出并写回修改的文件|
|:q|退出vi|
|:q!|强制退出，不保存修改|

在vi中改变游标位置 

命令行模式指令（跟在:之后的）需要回车确认
|Key|Usage|
|----|----|
|方向键|上下左右|
|j or|下移一行|
|k|上移一行|
|h or Backspace|左移一个字符|
|l or Space|右移一个字符|
|0|移到行首|
|$|移到行尾|
|w|移到下一个单词开头|
|:0 or 1G|移到文件开头|
|:n or nG|移到第n行|
|:$ or G|移到最后一行|
|CTRL-F or PageDown|前进一页|
|CTRL-S or PageUP|后退一页|
|^l|刷新居中屏幕|

![这里写图片描述](https://img-blog.csdn.net/20170302162457971?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170302162548862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170302162723862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2. emacs
# Local Security Principle

## 理解Linux安全

![这里写图片描述](https://img-blog.csdn.net/20170302163431507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

账户类型 

Linux有以下4种账户类型 

- root 

- System 

- Normal 

- Network 

使用`last`命令可以查看用户最后登陆时间。
## sudo进程隔离，限制硬件访问

![这里写图片描述](https://img-blog.csdn.net/20170302164429114?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170302164556663?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170302164632757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170302164659106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## passwords

![这里写图片描述](https://img-blog.csdn.net/20170302173532464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## Securing the Boot Process and Hardware Resources

# 网络操作

## 网络
- IP地址分类
- IP地址分配 

静态分配：在分配IP地址前要先确定主机的范围来确认网络的大小。这决定了哪一类IP可以被使用。`ipcalc`可以用来确定主机范围。 

假设使用C类网络，`ipcalc 192.168.0.0/24`可以计算主机范围。- 域名解析 

使用`hostname`查看主机域名，或更改域名。
- 网络接口 
`ifconfig`查看
- 网络配置文件 
![这里写图片描述](https://img-blog.csdn.net/20170302181621682?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 网络配置命令 
`$ /sbin/ip addr show`查看IP地址 
`$ /sbin/ip route show`查看路由信息- ping
- route
|Task|Command|
|----|----|
|查看当前路由表|$ route -n|
|添加静态路由|$ route add -net address|
|删除静态路由|$ route del -net address|

9. traceroute 
**traceroute**用来追踪网络数据包从源主机到目的主机的路由途径，用来解决网络延迟和错误问题非常有用。使用**traceroute**可以分离跳与跳之间的连通性问题，加快解决速度。  
`traceroute <domain>`. 

10.  更多工具|网络工具|描述|
|----|----|
|ethtool|查询网络接口，设置不同的参数|
|netstat|展示所有活动的连接和路由表。监视性能和解决问题很有用|
|nmap|扫描开放端口|
|tcpdump|输出网络流量用于分析|
|iptraf|在文本模式下监视网络流量|

## 浏览器

1.图形和命令行浏览器 

图形界面 

- Firefox 

- Google Chrome 

- Chromium 

- Epiphany 

- Opera 

非图形浏览器|Non-Graphical Browsers|Description|
|----|----|
|lynx|Configurable text-based web browser; the earliest such browser and still in use.|
|links or elinks|Based on lynx. It can display tables and frames.|
|w3m|Newer text-based web browser with many features.|

2. wget 
`wget <url>`可以处理以下几种情况： 

- 大文件下载 

- 递归下载 

- 需要密码的下载 

- 多文件下载 

3.curl 
`curl <url>`读取网页内容，也可以用来存储网页内容。 
`curl -o saved.html http://www.mysite.com`
## 传输文件
- FTP 
- ftp
- sftp
- ncftp
- yafc(Yet Another FTP Client)

- SSH 
`ssh <remotesystem> my_command`
- SCP 
`scp <localfile> <user@remotesystem>:/home/user/`

# 文本处理

## cat and echo

cat是concatenate的缩写。通常用来读取和打印和简单查看文件内容。使用命令`$ cat <filename>`查看文件。通常用cat是将多个文件结合在一起。
|Command|Usage|
|----|----|
|car file1 file2|连接多个文件并输出|
|cat file1 file2 > newfile|连接多个文件并输出到新文件|
|cat file >> existingfile|将文件附加到存在文件的末尾|
|cat > file|后续的输入会到文件中直到遇到CTRL+D|
|cat >> file|后续输入会附加的文件中直到CTRL+D|

`tac`会相反顺序输出文件 

1. 交互式使用cat 
`cat`用来从标准输入中读取，如果没有指定输入文件。创建文件`cat > <filename>`，这个命令会创建一个新文件并等待用户输入，输入`CTRL+D`在下一行的开头来保存并退出编辑。 

另一个方式创建文件是`cat > <filename> << EOF`,输入`EOF`退出。 

2. echo 
`$ echo string`
`echo`可以用来在标准输出设备上展示字符串或输出到文件中使用`>`,`>>`。 
`-e`选项可以允许特殊字符序列，如 

- \n 代表新行 

- \t 代表水平tab|Command|Usage|
|----|----|
|echo string > newfile|将指定字符串输入到文件|
|echo string >> existingfile|将指定字符串附加在文件末尾|
|echo $variable|输出环境变量|

## 处理大型文件

`$ less <filename>`
`$ cat <filename>|less`

1. head 
`$ head -n 5 test.txt`

2. tail 
`$ tail -n 15 filename`
`$ tail -f filename`
`-f`会持续输出文件中的新行。 

3. z命令族 

对压缩文件进行查看可以在相应的指令前面加z。|Command|Description|
|----|----|
|$ zcat compressed-file.txt.gz|查看一个压缩文件|
|$ zless .gz or more|分页浏览压缩文件|
|$ zgrep -i less test.txt.gz|搜索压缩文件内部|
|$ zdiff file1.txt.gz file2.txt.gz|对比两个压缩文件|

在非压缩文件上用zless会自动跳过解压缩步骤。

## sed和awk

![这里写图片描述](https://img-blog.csdn.net/20170302222244390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

1. sed命令语法|Command|Usage|
|----|----|
|sed -e command|Specify editing commands at the command line, operate on file and put the output on standard out (e.g., the terminal)|
|sed -f scriptfile|Specify a scriptfile containing sed commands, operate on file and put output on standard out.|

`-e`命令选项允许同时指定多个编辑命令 

2. sed基本操作 
![这里写图片描述](https://img-blog.csdn.net/20170302223641522?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170302223713819?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3.awk 
![这里写图片描述](https://img-blog.csdn.net/20170302223954961?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)|Command|Usage|
|----|----|
|awk ‘command’ var=value file|Specify a command directly at the command line|
|awk -f scriptfile var=value file|Specify a file that contains the script to be executed along with f|

As with **sed**, short **awk** commands can be specified directly at the command line, but a more complex script can be saved in a file that you can specify using the `-f` option. 
![这里写图片描述](https://img-blog.csdn.net/20170302224159900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 文件处理工具

## grep和strings

`grep`被广泛地用于文本搜索工具。它扫描特定的模式，可以使用正则表达式。
|Command|Usage|
|----|----|
|grep [pattern]|搜索文件中的模式并输出所有匹配行|
|grep -v [pattern]|输出所有不匹配的行|
|-i|忽略大小写|
|grep [0-9]|输出所有包括0到9行|
|grep -c 3 [pattern]|Print context of lines (specified number of lines above and below the pattern) for matching the pattern. Here the number of lines is specified as 3.|

find命令与grep命令的区别 

- find命令： 在系统当中搜索符合条件的文件名，如果需要匹配，使用通配符匹配，通配符是完全匹配 。  

- grep命令：在文件当中搜索符合条件的字符串，如果需要匹配，使用正则表达式进行匹配，正则表达式是包含匹配 
## 各种文本工具
- tr
- tee 
`tee`使用任意命令的输出作为输入，在输出到标准输出的同时，也存储到文件中。 

如`ls -l | tee newfile`- wc 
`word count`计算文件中的行，单词和字符。 
`wc -l filename`|选项|描述|
|----|----|
|-l|行数|
|-c|字节bytes|
|-w|单词数|

4. cut 

cut用来处理基于列的文件，用来提取指定列。默认的列分隔符是**tab**。 

For example, to display the third column delimited by a blank space, at the command prompt type `ls -l | cut -d" " -f3` and press the **Enter** key.
# 打印























