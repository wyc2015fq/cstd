# markdown数学公式编辑学习 - 约会远行的专栏 - CSDN博客
2018年07月10日 16:20:57[约会远行](https://me.csdn.net/yue530tomtom)阅读数：5725
纯属学习之用（备忘），在使用markdown编辑算法的wiki时，发现公式真的很难搞，就学习了一下mathjax

## 基本语法

公式定义格式：更多可以参考 [LaTex](https://math.meta.stackexchange.com/questions/5020/mathjax-basic-tutorial-and-quick-reference)

```
$...$
```

```
$${x}^{m}_{\sum_{i=1}^{n}y_i} \cdot {\sum_{i=1}^{n}y_i}$$
```

${x}^{m}_{\sum_{i=1}^{n}y_i} \cdot {\sum_{i=1}^{n}y_i}$

先说一下偷懒的方法：复杂的公式编辑可以借助在线公式编辑器，比如 [在线latex数学公式](http://latex.codecogs.com/eqneditor/editor.php) 再编辑完成后直接拷贝过来，省去记录复杂的各种符号

```
1、引入js(csdn不用引入是直接支持的公式的，可以直接写)
<scripttype="text/javascript"src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=default"></script>
2、编写公式表达式，两种
块级公式写法:$$公式表达式$$
行内公式写法:$公式表达式$
```

```
$$f=\lim_{x \to 0}\left ( \frac{\sin x}{x}\right )$$$f=\lim_{x \to 0}\left ( \frac{\sin x}{x}\right )$
```

分别显示为： 
$f=\lim_{x \to 0} \left ( \frac{\sin x}{x} \right )$

$f=\lim_{x \to 0} \left ( \frac{\sin x}{x} \right )$

# mathjax介绍

mathjax语法和符号，各种符号嵌套可以实现非常美观的复杂方程式(如傅立叶变换)

```
\hat{f}(\xi ):=\int_{-\infty}^{\infty}f(x)e^{-2\pi ix\xi}dx
```

$\hat{f}(\xi ):=\int_{-\infty }^{\infty }f(x)e^{-2\pi ix\xi}dx$

## 希腊字母
显示命令α$\alpha$γ$\gamma$
不一个个啰嗦了，其他希腊字母和其他符号说明详见[MathJax 支持的数学符号表](http://mirrors.cqu.edu.cn/CTAN/info/symbols/math/maths-symbols.pdf)（其中还包括：数学模式重音符号，希腊字母、二元关系符号、二元运算符号、大运算符号、箭头、定界符、大定界符、其他符号等）

大写的希腊字母，只需将命令的首字母大写即可(有的字母没有大写)

```
$\lambda$ & $\Lambda$
```

$\lambda$ & $\Lambda$

斜体希腊字母，在命令前加上var前缀即可

```
$\Lambda$ & $\varLambda$
```

$\Lambda$ & $\varLambda$

## 括号

{}有特殊作用因此当需要显示大括号时一般使用\lbrace和\rbrace来表示，小括号、中括号都不需要转义。

```
$$f(x, y,z) = x * \lbrace[(x + y) * x] +z\rbrace$$
```

$f(x, y,z) = x * \lbrace[(x + y) * x] +z\rbrace$

## 连线符号

```
$$\overline{a+b+c}$$$$\underline{a+b+c}$$$$\overbrace{a+\underbrace{b+c}_{小组合}+d}^{大组合}$$
```

$\overline{a+b+c}$
$\underline{a+b+c}$
$\overbrace{a+\underbrace{b+c}_{小组合}+d}^{大组合}$
## 矩阵

起始标记begin{矩阵边框} 

结束标记end{矩阵边框} 

每行末尾标记\ 

行间元素之间以&分隔
```
$$\begin{matrix}1&1&1\\1&1&1\\1&1&1\\\end{matrix}$$
或者
\begin{matrix}
1&1&1\\
1&1&1\\
1&1&1\\\end{matrix}
```

$\begin{matrix}1&1&1\\1&1&1\\1&1&1\\\end{matrix} $
矩阵边框样式matrix无样式pmatrix小括号边框bmatrix中括号边框Bmatrix大括号边框vmatrix单竖线边框Vmatrix双竖线边框
省略元素
样式标识横省略号\cdots竖省略号\vdots斜省略号\ddots
如,线性代数里常用的矩阵

```matlab
$$\begin{vmatrix}{a_{11}}&{a_{12}}&{\cdots}&{a_{1n}}\\
{a_{21}}&{a_{22}}&{\cdots}&{a_{2n}}\\
{\vdots}&{\vdots}&{\ddots}&{\vdots}\\
{a_{m1}}&{a_{m2}}&{\cdots}&{a_{mn}}\\
\end{vmatrix}$$
```

$\begin{vmatrix}{a_{11}}&{a_{12}}&{\cdots}&{a_{1n}}\\{a_{21}}&{a_{22}}&{\cdots}&{a_{2n}}\\{\vdots}&{\vdots}&{\ddots}&{\vdots}\\{a_{m1}}&{a_{m2}}&{\cdots}&{a_{mn}}\\\end{vmatrix}$

## 阵列

起始、结束处以{array}声明 

对齐方式：在{array}后以{}逐行统一声明  

左对齐：l；居中：c；右对齐：r； （left、center、right的缩写） 

竖直线：在声明对齐方式时，插入|建立竖直线 

插入水平线：\hline 

如
```
$$\begin{array}{c|cc}      xy &x_1 &x_2\\\hline      y_1 &x_1y_1 &x_2y_1\\      y_2 &x_1y_2 &x_2y_2\end{array}$$
```

$\begin {array} {c|lr}      xy &x_1 &x_2\\      \hline      y_1 &x_1y_1 &x_2y_1\\      y_2 &x_1y_2 &x_2y_2\end {array}$

## 方程组

起始、结束处以{cases}声明

```
$$\begin{cases}
a_1x+b_1y=100\\
a_2x+c_2z=200\\
a_3x+b_3y+c_3z=300
\end{cases}$$
```

$\begin{cases}a_1x+b_1y=100\\a_2x+c_2z=200\\a_3x+b_3y+c_3z=300\end{cases}$

若需要对齐,对齐项可以使用 & 连接

```matlab
\begin{equation}
\left\{\begin{matrix}
f(x)&=x^{b}\cdot\sum_{m}^{n}& x=10,&a<10\\ 
f(x)&=x^{a}& x=\sum_{m}^{n},&a<10
\end{matrix}\right.
\end{equation}
```

$\begin{equation}\left\{\begin{matrix}f(x)&=x^{b}\cdot\sum_{m}^{n}& x=10,&a<10\\ f(x)&=x^{a}& x=\sum_{m}^{n},&a<10\end{matrix}\right.\end{equation}$
