# Shell中反引号（`）与$()用法的区别 - DoubleLi - 博客园






今天有人提问：


	echo `echo \\\\\\\w`
	echo $(echo \\\\\\\w)
	为什么输出的不一样？


这就引申出了另一个问题：反引号与$()有没有区别？

这是一个非常有意思的问题，但要解释这个问题确比较困难，我来换一个简单一点的例子，通俗的解释一下。

比如我们有一个shell脚本名为test.sh有一下两行

```
```bash
echo
```

```bash
`
```

```bash
echo
```

```bash
\$HOSTNAME`
```

```bash
//
```

```bash
反引号
```



```bash
echo
```

```bash
$(
```

```bash
echo
```

```bash
\$HOSTNAME)
```

```bash
//
```

```bash
$()
```
```

　　我们可以使用sh -x test.sh来分析bash是如何来解释这个脚本的，返回的信息如下：

![](http://img.blog.csdn.net/20150725134416001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

　　通过上图我们可以得出两个结论：
- 
反引号中\$并没有将$的特殊意义转换 反引号包含的内容 echo \$hostname 仍然被解释为一个echo $HOSTNAME 取到了这个变量的值并输出所以反引号返回的值为 CentOS-1

- 
$()则正好相反，$明显被\转义成了一个普通字符，所以并没有取到变量值，而是返回了字符串本身的意思，故而返回了$HOSTNAME


　　这说明反引号对反斜杠（\）有特殊的转化，至少我们看到当我们直接用1个\时，它本身并不会起到转义的作用。现在我们将脚本的内容做一些修改：

```
```bash
echo
```

```bash
`
```

```bash
echo
```

```bash
\\$HOSTNAME`
```

```bash
//
```

```bash
反引号
```



```bash
echo
```

```bash
$(
```

```bash
echo
```

```bash
\\$
```

```bash
hostname
```

```bash
)
```

```bash
//
```

```bash
$()
```
```

　　我们又多加了1个反斜杠，这次我们来看输出的情况：

![](http://img.blog.csdn.net/20150725134525472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

　　这次看起来似乎反转了：
- 
反引号反而输出了$HOSTNAME的字面意思

- 
$()如我们所愿的取到了变量值，并理所应当的输出了一个被转义的字符串\


　　我们再来做一个实验，再建立一个脚本名为a.sh:

```
```bash
echo
```

```bash
`
```

```bash
echo
```

```bash
\\\\ `
```



```bash
echo
```

```bash
$(
```

```bash
echo
```

```bash
\\\\ )
```
```

　　注意：最后一个反引号和）的前面都有一个空格，否则最后一个反引号和）都将被注释，不会被当做结束的替换符号。

　　使用sh -x a.sh的结果如下：

![](http://img.blog.csdn.net/20150725134543521?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

　　通过上图我们可以看到：
- 
反引号中4个\被输出为1个\

- 
$()中4个\被输出为2个\


　　我们将脚本修改为8个\：

```
```bash
echo
```

```bash
`
```

```bash
echo
```

```bash
\\\\\\\\ `
```



```bash
echo
```

```bash
$(
```

```bash
echo
```

```bash
\\\\\\\\ )
```
```

　　再看输出：

![](http://img.blog.csdn.net/20150725134548061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

　　由此，我们可以得出结论：
- 
反引号齐本身就对\进行了转义，保留了齐本身意思，如果我们想在反引号中起到\的特殊意义，我们必须使用2个\来进行表示。

所以我们可以简单的想象成反引号中：** \\ = \**

- 
$()中则不需要考虑\的问题，与我们平常使用的一样：**\ = \**


题外话： 反引号是老的用法，$()是新的用法，不管是在学习测试中，还是在实际工作中，**$()的用法都是被推荐的。**









