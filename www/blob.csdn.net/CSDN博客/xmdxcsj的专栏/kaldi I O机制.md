# kaldi I/O机制 - xmdxcsj的专栏 - CSDN博客





2016年11月13日 20:44:08[xmucas](https://me.csdn.net/xmdxcsj)阅读数：867








## 扩展文件名

使用扩展的文件名来表示输入输出： 

- rxfilename表示读取的数据流，比如file/stream/standard input 

- wxfilename表示输出的数据流
1.**rxfilename**

类型包括 

- “-” or “” 表示标准输入 

- “command |” 命令通过管道作为输入，解析的时候去掉”|”，将剩下的command通过popen()交由shell处理 

- “/some/filename:12345” 文件有偏移 

- “/some/filename” 不满足上面三种形式的当做普通文件解析
2.**wxfilename**

类型包括 

- “-” or “” 标准输入 

- “|command” 输出经由管道传给command，同样通过调用popen()函数交由shell 

- “/some/filename” 不是以上两种形式的当作普通文件解析
## I/O类型

分为两类： 

- non-table 文件或者数据流包含一个或者两个object 

- table 文件或者数据流包含object的集合，使用string进行map
### non-table

可以使用copy相关的命令对文件进行二进制和文本之间的转化，比如”copy-matrix –binary=false foo.mat -“

### table

table有两种形式： 

- script file: map的value存放的是数据的存放路径 

- archive: map的value存放的是实际的数据
#### 文件格式

1.scp 

标准格式是

```xml
<key> <rxfilename>
```

其中rxfilename的形式可以是以下几种

```
#1.文件路径
/some/filename
#2.管道
gunzip -c /usr/data/file_010001.wav.gz |
#3.文件范围
foo.ark:89142[0:51,89:100]
```

2.ark 

标准格式是
`token1 [something]token2 [something]token3 [something] ....`
#### 文件使用

table类型的文件可以使用wspecifiers/rspecifiers作为文件名传入命令行，有效的使用方式：

```cpp
std::string rspecifier1 = "scp:data/train.scp"; // script file.
std::string rspecifier2 = "ark:-"; // archive read from stdin.
// write to a gzipped text archive.
std::string wspecifier1 = "ark,t:| gzip -c > /some/dir/foo.ark.gz";
std::string wspecifier2 = "ark,scp:data/my.ark,data/my.ark";
```

格式可以总结为以下形式：
`ark/scp,[option]: rxfilename/wxfilename`
相比于前面的rxfilename/wxfilename增加的前面的头表示table的格式以及读写的方式，option包括：

```
##wspecifiers相关的选项
b: 二进制格式
t: 文本格式
f: 每次写操作flush
nf: 与f相反
p: 当scp文件缺失某些项的时候，不报错
##rspecifiers相关的选项
o: once,每一个key只被查找一次
p: permissive,忽略错误
s: sorted,keys是排序的
cs: called-sorted,可以节省内存
```








