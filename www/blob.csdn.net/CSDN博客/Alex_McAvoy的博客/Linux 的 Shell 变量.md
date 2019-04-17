# Linux 的 Shell 变量 - Alex_McAvoy的博客 - CSDN博客





2018年06月18日 22:53:38[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：48
个人分类：[Linux 学习笔记](https://blog.csdn.net/u011815404/article/category/7715914)









## 【简述】



变量即计算机中用于记录一个值的符号，而这些符号可用于不同的运算处理中。

通常变量与值是一对一的关系，可以通过表达式读取它的值并赋值给其它变量，也可以直接指定数值赋值给任意变量。

为便于运算和处理，大部分的编程语言会区分变量的类型，用于分别记录数值、字符或者字符串等等数据类型。

shell 变量也基本如此，有不同类型，可以参与运算，有作用域限定。



## 【变量类型】

在运行 shell 时，有三种变量：

1）局部变量：当前 shell 进程私有用户自定义变量，仅在当前 shell 实例中有效，其他 shell 启动的程序不能访问。

2）shell 变量：shell 变量是由 shell 程序本身内建的变量，其中有一部分是环境变量，有一部分是局部变量，这些变量保证了shell的正常运行。

3）环境变量： 由局部变量导出。所有的程序，都能访问环境变量，有些程序依靠环境变量来保证其正常运行。

## 【变量定义与使用】

shell 变量不需要进行任何声明，直接定义即可，但使用 shell 变量要用到$符号。

![](https://img-blog.csdn.net/20180618205733352?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

同时，变量名的命名须遵循如下规则：

1）变量名只能是英文字母、数字或者下划线，且不能以数字为开头。

2）不能使用 bash 里的关键字。

**注：**

**1.变量名和等号之间不能有空格，否则会被认为是命令。**

**2.使用变量时，加 {} 可以防止歧义**

![](https://img-blog.csdn.net/20180618210105577?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 【更改变量为只读】

使用 readonly 命令可以将变量定义为只读变量，只读变量的值不能被改变。

![](https://img-blog.csdn.net/20180618210516971?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 【修改已有变量】
|${变量名#匹配字串}|从头向后开始匹配，删除符合匹配字串的最短数据|
|----|----|
|${变量名##匹配字串}|从头向后开始匹配，删除符合匹配字串的最长数据|
|${变量名%匹配字串}|从尾向前开始匹配，删除符合匹配字串的最短数据|
|${变量名%%匹配字串}|从尾向前开始匹配，删除符合匹配字串的最长数据|
|${变量名/旧的字串/新的字串}|将符合旧字串的第一个字串替换为新的字串|
|${变量名//旧的字串/新的字串}|将符合旧字串的全部字串替换为新的字串|

例：

![](https://img-blog.csdn.net/2018061823433359?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 【删除已有变量】

使用 unset 命令可以删除变量，变量被删除后就无法再次使用。

![](https://img-blog.csdn.net/20180618210911281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**注：只读变量无法删除，只有重启后才会自动消失。**

## **【环境变量】**

在所有的 Linux 系统中，每个进程都有其各自的环境变量设置，且默认情况下，当一个进程被创建时，除非创建过程中明确指定，它将继承其父进程的绝大部分环境设置。

环境变量的作用域比自定义变量的要大，Shell 的环境变量作用于其自身和它的子进程，Shell 程序也作为一个进程运行在操作系统之上，而在 Shell 中运行的大部分命令都将以 Shell 的子进程的方式运行。

![](https://img-blog.csdn.net/20180619002224350?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



可以将环境变量简单地理解为在当前进程的子进程有效的变量，为与普通变量区分，通常习惯将环境变量名设为大写。

### 1.打印环境变量信息
|set|显示当前 Shell 所有变量|
|----|----|
|env|显示与当前用户相关的环境变量，也可让命令在指定环境中运行|
|export|显示从 Shell 中导出成环境变量的变量，也可将自定义变量导出为环境变量|

### 2.环境变量永久生效

按变量的生存周期来划分，Linux 变量可分为两类：

1）永久的：需要修改配置文件，变量永久生效；

2）临时的：使用 export 命令行声明即可，变量在关闭 shell 时失效。

当关机或关闭当前的 shell 之后，环境变量会消失。

在 /etc 下有两个重要文件 /etc/bashrc 和 /etc/profile ，它们分别存放的是 shell 变量和环境变量。

如果想要添加一个对所有用户永久生效的环境变量，只需要打开 /etc/profile，在里面加上想添加的环境变量即可。

**注：在每个用户目录下存在一个隐藏文件 .profile ，其只对当前用户永久生效。**

**![](https://img-blog.csdn.net/2018061822504027?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

### 3.命令的查找路径与顺序

在 Shell 中输入一个命令，Shell 是通过环境变量 PATH 来进行搜索然后执行的，这个 PATH 里面就保存了 Shell 中执行的命令的搜索路径。

![](https://img-blog.csdn.net/20180618231654953?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



通常 PATH 目录下放的都是可执行文件，当在 Shell 中执行一个命令时，系统就会按照 PATH 中设定的路径按照顺序依次到目录中去查找，如果存在同名的命令，则执行先找到的那个。



### 4.添加自定义路径到“ PATH ”环境变量

命令格式：**PATH=$PATH:绝对路径**

按如上命令给 PATH 环境变量追加了一个路径，但也只是在当前 Shell 有效，一旦退出终端，再打开就会发现又失效了。

为避免上述问题，可采用让其自动执行的方式配置脚本。

在每个用户的 home 目录中有一个 Shell 每次启动时会默认执行一个配置脚本，用于初始化环境，包括添加一些用户自定义环境变量等，如：zsh 的配置文件是 .zshrc，相应 Bash 的配置文件为 .bashrc ，它们在 etc 下还都有一或多个全局的配置文件，但一般只修改用户目录下的配置文件。

可采用下列命令直接添加内容到 .zshrc 中：**echo "PATH=$PATH:绝对路径" >> .zshrc**

**注：>> 表示将标准输出以追加的方式重定向到一个文件中（> 是以覆盖的方式重定向到一个文件中），在指定文件不存在的时会创建新的文件。**



### 5.让环境变量立即生效



在 Shell 中修改了一个配置脚本文件后，每次都要退出终端重新打开甚至重启主机之后其才能生效，为避免麻烦，可以使用 source 命令来让其立即生效。

命令格式：**source 配置文件名**

**![](https://img-blog.csdn.net/20180618233527594?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**











