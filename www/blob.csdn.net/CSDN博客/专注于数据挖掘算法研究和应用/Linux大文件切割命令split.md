# Linux大文件切割命令split - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月22日 08:35:38[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：11538








场景：linux有些大文件处理起来不方便，需要将其分割，发现一个好用的命令。



```
bash-4.1$ split --help
用法：split [选项]... [输入 [前缀]]
将输入内容拆分为固定大小的分片并输出到"前缀aa"、"前缀ab",...；
默认以 1000 行为拆分单位，默认前缀为"x"。如果不指定文件，或
者文件为"-"，则从标准输入读取数据。

长选项必须使用的参数对于短选项时也是必需使用的。
  -a, --suffix-length=N 指定后缀长度为N (默认为2)
  -b, --bytes=大小              指定每个输出文件的字节大小
  -C, --line-bytes=大小 指定每个输出文件里最大行字节大小
  -d, --numeric-suffixes        使用数字后缀代替字母后缀
  -l, --lines=数值              指定每个输出文件有多少行
      --verbose         在每个输出文件打开前输出文件特征
      --help            显示此帮助信息并退出
      --version         显示版本信息并退出

SIZE 可以是一个可选的整数，后面跟着以下单位中的一个：
KB 1000，K 1024，MB 1000*1000，M 1024*1024，还有 G、T、P、E、Z、Y。

请向bug-coreutils@gnu.org 报告split 的错误
GNU coreutils 项目主页：<http://www.gnu.org/software/coreutils/>
GNU 软件一般性帮助：<http://www.gnu.org/gethelp/>
请向<http://translationproject.org/team/zh_CN.html> 报告split 的翻译错误
要获取完整文档，请运行：info coreutils 'split invocation'
```







按照行数分割，如下：


`split -l 10000 test.txt test`

会在test.txt当前目录下生成以test前缀的一系列文件



按照字节数分割，如下：


`split -b 100m test.txt test`

如果要切割文件指定命名，参考如下：


`split -l 2000 test.txt -d -a 2 lim_`

-l：按行分割，上面表示将urls.txt文件按2000行一个文件分割为多个文件


-d：添加数字后缀，如00、01、02


-a 2：表示用两位数据来顺序命名


lim_：用来定义分割后的文件名前面的部分。












