# Linux 命令执行顺序控制 - Alex_McAvoy的博客 - CSDN博客





2018年07月08日 19:15:02[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：642








# 【概述】

通常情况下，我们每次只能在终端输入一条命令，按下回车执行，执行完成后，我们再输入第二条命令，然后再按回车执行……

当有我们会一次输入多条命令，或者有选择性的执行命令时，我们可以用特定的语法结构来实现命令的执行顺序的控制问题。

# 【顺序执行多条命令】

简单的顺序执行，可以使用** ; **来完成。

**实例：**

当我们需要使用 apt-get 安装一个软件，然后安装完成后立即运行安装的软件，而恰巧主机刚更换的软件源还没有更新软件列表，那么会有如下一系列操作：

```
$ sudo apt-get update
# 等待——————————然后输入下面的命令
$ sudo apt-get install some-tool //这里some-tool是指具体的软件包，例如：banner
# 等待——————————然后输入下面的命令
$ some-tool
```

如果想一次性输完，然后让他依次执行各命令，可以使用 ; 在多条命令之间分隔。
`$ sudo apt-get update;sudo apt-get install some-tool;some-tool`
# 【有选择的执行命令】

如果我们在让自动顺序执行命令时，前面的命令执行不成功，而后面的命令又依赖于上一条命令的结果，那么就会造成花了时间，最终却得到一个错误的结果，而且有时候还无法判断结果是否正确。

因此我们需要能够有选择性的来执行命令，例如：上一条命令执行成功才继续下一条、上一条命令不成功时又该做出什么处理等。

同 C 语言中的逻辑与 && 和逻辑或 || 相同，在 Linux 里的 Shell 中，也同样存在逻辑与 && 和逻辑或 || ，但与 C 语言不同的是，Shell 中的这两个符号，除了也可用于表达逻辑与和逻辑或之外，还可以实现命令执行顺序的简单控制。

在实现命令执行顺序的简单控制时，&& 与 || 是根据其前面的命令执行结果来进行判断的。

**&& 表示如果前面的命令执行结果为 0 时，则执行后面的命令，否则不执行**

![](https://img-blog.csdn.net/20180708191427180?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**|| 有着与 && 相反的控制效果，其表示当前面的命令执行结果为 1 时，则执行它后面的命令，否则不执行**

![](https://img-blog.csdn.net/20180708191436298?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果想知道上一次命令的返回结果，可以使用 **echo $?** 获取结果。

如果执行成功，结果为 0 

![](https://img-blog.csdn.net/20180708191029363?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果执行不成功，结果为 1

![](https://img-blog.csdn.net/20180708191042887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



