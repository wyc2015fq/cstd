# 转载：matlab 字符串和变量名互换 - 去向前方的博客 - CSDN博客





2018年07月19日 19:31:08[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：678








原创文章，欢迎转载。转载请注明：转载自 祥的博客

原文链接：[https://blog.csdn.net/humanking7/article/details/80628757](https://blog.csdn.net/humanking7/article/details/80628757)

```
1. 问题引出
2 情况1: 用字符串作为变量名-eval()
3. 情况2: 变量名转化为字符串-inputname()
```
- 问题引出

由于Matlab的m语言是解释性语言，所以可以在代码运行过程中用字符串做为变量名。有时候我们需要知道变量在程序中的名字，就是将变量名转化为字符串。 

2 情况1: 用字符串作为变量名-eval()

```bash
第一个字符串str_var = 'varNumber'
    以字符串 varNumber为变量名声明一个变量
```

用eval()函数可以轻松搞定：

```bash
str_var = 'varNumber';
eval( [str_var, '= 10']);
```

运行后可以看到工作空间多了一个名叫varNumber的变量，值为10。

![pic1](https://img-blog.csdn.net/20180719192927453?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

eval()的作用解释起来就是：运行了在Matlab中运行了 [str_var, ‘= 10’] 这段代码，即就是：varNumber = 10。 

3. 情况2: 变量名转化为字符串-inputname()

```
定义一个数值类型变量 numberA = 123;。
现在我需要输出这个变量名，也就是字符串'numberA'
```

这需要自己写一个函数getVarName()，该函数的输入为要被转化的变量，输出为一个字符串，即就是该变量的变量名。

函数代码( getVarName.m文件 )

```
%% 将变量的变量名转换为字符串
function [ str_varName ] = getVarName( var )

    str_varName = sprintf('%s', inputname(1));

end
```

测试代码：

```
numberA = 123;
str_varName = getVarName(numberA);
disp(['变量的名字为: ' ,str_varName])
class(str_varName)
```

![pic2](https://img-blog.csdn.net/2018071919293967?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Matlab里面对其进行了解释，这个需求虽然小众，但还是有用的，比如我要把所有的变量和输入打印到文件中（将变量逆向转化为m文件，有时用m文件比mat文件更容易查询数据的问题

）

```
Description
    This command can be used only inside the body of a function.
    inputname(argnum) returns the workspace variable name corresponding to the argument number argnum. If the input argument has no name (for example, if it is an expression instead of a variable), the inputname command returns the empty string (”)
```



