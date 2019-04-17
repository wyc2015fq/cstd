# linux中shell教程 - miner_zhu的博客 - CSDN博客





2018年10月30日 16:27:00[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：40








# shell介绍

Shell 是一个用 C 语言编写的程序，它是用户使用 Linux 的桥梁。Shell 既是一种命令语言，又是一种程序设计语言。Shell 是指一种应用程序，这个应用程序提供了一个界面，用户通过这个界面访问操作系统内核的服务。

Shell 脚本（shell script），是一种为 shell 编写的脚本程序。业界所说的 shell 通常都是指 shell 脚本，但读者朋友要知道，shell 和 shell script 是两个不同的概念。由于习惯的原因，简洁起见，本文出现的 "shell编程" 都是指 shell 脚本编程，不是指开发 shell 自身。

# 基本操作

Linux 的 Shell 种类众多，本文讲的是 Bash，Bash 也是大多数Linux 系统默认的 Shell。

在一般情况下，人们并不区分 Bourne Shell 和 Bourne Again Shell，所以，像 **#!/bin/sh**，它同样也可以改为 **#!/bin/bash**。#! 告诉系统其后路径所指定的程序即是解释此脚本文件的 Shell 程序。

### 建立一个shell脚本

打开文本编辑器(可以使用 vi/vim 命令来创建文件)，新建一个文件 test.sh，扩展名为 sh（sh代表shell），扩展名并不影响脚本执行。

```bash
#!/bin/bash
echo "Hello World !"
```

#!  是一个约定的标记，它告诉系统这个脚本需要什么解释器来执行，即使用哪一种 Shell。

### 运行

在我们假设你写的这个程序档名是 /home/dmtsai/shell.sh 好了，那如何运行这个文件？很简单，可以有底下几个方法：


- 直接命令下达： shell.sh 文件必须要具备可读与可运行 (rx) 的权限，然后：
	- 绝对路径：使用 /home/dmtsai/shell.sh 来下达命令；
- 相对路径：假设工作目录在 /home/dmtsai/ ，则使用 ./shell.sh 来运行
- 变量『PATH』功能：将 shell.sh 放在 PATH 指定的目录内，例如： ~/bin/

- 以 bash 程序来运行：透过『 bash shell.sh 』或『 sh shell.sh 』来运行

### 变量

定义变量时，变量名不加美元符号（$，PHP语言中变量需要），如：
`your_name="runoob.com"`
注意，变量名和等号之间不能有空格，这可能和你熟悉的所有编程语言都不一样。

同时，变量名的命名须遵循如下规则：
- 命名只能使用英文字母，数字和下划线，首个字符不能以数字开头。
- 中间不能有空格，可以使用下划线（_）。
- 不能使用标点符号。
- 不能使用bash里的关键字（可用help命令查看保留关键字）。

### 使用变量

使用一个定义过的变量，只要在变量名前面加美元符号即可，如：

```
your_name="qinjx"
echo $your_name
echo ${your_name}
```

变量名外面的花括号是可加可不加，加花括号是为了帮助解释器识别变量的边界。推荐给所有变量加上花括号，这是个好的编程习惯。

### 其他
- 使用 readonly 命令可以将变量定义为只读变量，只读变量的值不能被改变。
- 
```bash
#!/bin/bash
myUrl="http://www.google.com"
readonly myUrl
```



- 使用 unset 命令可以删除变量。unset 命令不能删除只读变量。
	`unset variable_name`

### Shell 字符串

字符串是shell编程中最常用最有用的数据类型（除了数字和字符串，也没啥其它类型好用了），字符串可以用单引号，也可以用双引号，也可以不用引号。单双引号的区别跟PHP类似。

单引号字符串的限制：
- 单引号里的任何字符都会原样输出，单引号字符串中的变量是无效的；
- 单引号字串中不能出现单独一个的单引号（对单引号使用转义符后也不行），但可成对出现，作为字符串拼接使用。

双引号的优点：
- 双引号里可以有变量
- 双引号里可以出现转义字符

### 字符串操作
- 获取字符串长度

```
string="abcd"
echo ${#string} #输出 4
```
- 提取子字符串。 以下实例从字符串第 **2** 个字符开始截取 **4** 个字符：

```
string="runoob is a great site"
echo ${string:1:4} # 输出 unoo
```
- 查找子字符串。 查找字符 **i** 或 **o** 的位置(哪个字母先出现就计算哪个)：

```
string="runoob is a great site"
echo `expr index "$string" io`  # 输出 4
```

**注意：** 以上脚本中 ` 是反引号，而不是单引号 '，不要看错了哦。

### 运算符

** 算术运算符**

同其他语言

**关系运算符**

关系运算符只支持数字，不支持字符串，除非字符串的值是数字。

下表列出了常用的关系运算符，假定变量 a 为 10，变量 b 为 20：
|运算符|说明|举例|
|----|----|----|
|-eq|检测两个数是否相等，相等返回 true。|[ $a -eq $b ] 返回 false。|
|-ne|检测两个数是否不相等，不相等返回 true。|[ $a -ne $b ] 返回 true。|
|-gt|检测左边的数是否大于右边的，如果是，则返回 true。|[ $a -gt $b ] 返回 false。|
|-lt|检测左边的数是否小于右边的，如果是，则返回 true。|[ $a -lt $b ] 返回 true。|
|-ge|检测左边的数是否大于等于右边的，如果是，则返回 true。|[ $a -ge $b ] 返回 false。|
|-le|检测左边的数是否小于等于右边的，如果是，则返回 true。|[ $a -le $b ] 返回 true。|

**布尔运算符**

下表列出了常用的布尔运算符，假定变量 a 为 10，变量 b 为 20：
|运算符|说明|举例|
|----|----|----|
|!|非运算，表达式为 true 则返回 false，否则返回 true。|[ ! false ] 返回 true。|
|-o|或运算，有一个表达式为 true 则返回 true。|[ $a -lt 20 -o $b -gt 100 ] 返回 true。|
|-a|与运算，两个表达式都为 true 才返回 true。|[ $a -lt 20 -a $b -gt 100 ] 返回 false。|

**字符串运算符**

下表列出了常用的字符串运算符，假定变量 a 为 "abc"，变量 b 为 "efg"：
|运算符|说明|举例|
|----|----|----|
|=|检测两个字符串是否相等，相等返回 true。|[ $a = $b ] 返回 false。|
|!=|检测两个字符串是否相等，不相等返回 true。|[ $a != $b ] 返回 true。|
|-z|检测字符串长度是否为0，为0返回 true。|[ -z $a ] 返回 false。|
|-n|检测字符串长度是否为0，不为0返回 true。|[ -n "$a" ] 返回 true。|
|str|检测字符串是否为空，不为空返回 true。|[ $a ] 返回 true。|

**文件测试运算符**

文件测试运算符用于检测 Unix 文件的各种属性。属性检测描述如下：
|参数|说明||
|----|----|----|
|-e 文件名|如果文件存在则为真||
|-r 文件名|如果文件存在且可读则为真||
|-w 文件名|如果文件存在且可写则为真||
|-x 文件名|如果文件存在且可执行则为真||
|-s 文件名|如果文件存在且至少有一个字符则为真||
|-d 文件名|如果文件存在且为目录则为真||
|-f 文件名|如果文件存在且为普通文件则为真||
|-c 文件名|如果文件存在且为字符型特殊文件则为真||
|-b 文件名|如果文件存在且为块特殊文件则为真||

### echo命令

Shell 的 echo 指令与 PHP 的 echo 指令类似，都是用于字符串的输出。命令格式：

echo string- **显示换行**

echo -e "OK! \n" # -e 开启转义
echo "It is a test"- **显示结果定向至文件**

echo "It is a test" > myfile
### if else

if 语句语法格式：

if condition
then
    command1 
    command2
    ...
    commandN 
fi
## for 循环

for循环一般格式为：

for var in item1 item2 ... itemN
do
    command1
    command2
    ...
    commandN
done
# 函数

shell中函数的定义格式如下：

[ function ] funname [()]
{
    action;
    [return int;]
}
### 输入/输出重定向

大多数 UNIX 系统命令从你的终端接受输入并将所产生的输出发送回​​到您的终端。一个命令通常从一个叫标准输入的地方读取输入，默认情况下，这恰好是你的终端。同样，一个命令通常将其输出写入到标准输出，默认情况下，这也是你的终端。

重定向命令列表如下：
|命令|说明|
|----|----|
|command > file|将输出重定向到 file。|
|command < file|将输入重定向到 file。|
|command >> file|将输出以追加的方式重定向到 file。|
|n > file|将文件描述符为 n 的文件重定向到 file。|
|n >> file|将文件描述符为 n 的文件以追加的方式重定向到 file。|
|n >& m|将输出文件 m 和 n 合并。|
|n <& m|将输入文件 m 和 n 合并。|
|<< tag|将开始标记 tag 和结束标记 tag 之间的内容作为输入。|

> 
需要注意的是文件描述符 0 通常是标准输入（STDIN），1 是标准输出（STDOUT），2 是标准错误输出（STDERR）。






转载来源：[http://www.runoob.com/linux/linux-shell.html](http://www.runoob.com/linux/linux-shell.html)





