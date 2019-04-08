# markdown中公式编辑教程





## 在线工具

[在线 LaTex 公式编辑器](http://latex.codecogs.com/eqneditor/editor.php)

http://latex.codecogs.com/eqneditor/editor.php

在线表格编辑工具   [Tables Generator](http://www.tablesgenerator.com/markdown_tables)



## 行内与独行

 行内公式：将公式插入到本行内，符号：`$公式内容$`，如：$xyz$
 独行公式：将公式插入到新的一行内，并且居中，符号：`$$公式内容$$`，如：
$$
xyz
$$


 行内公式：$ \Gamma(z) = \int_0^\infty t^{z-1}e^{-t}dt\,. ​$
 行间公式：
$$
\Gamma(z) = \int_0^\infty t^{z-1}e^{-t}dt\,.
$$
  对应的代码块为：

```
$ \Gamma(z) = \int_0^\infty t^{z-1}e^{-t}dt\,. $
$$
\Gamma(z) = \int_0^\infty t^{z-1}e^{-t}dt\,.
$$
```

  行内公式是在公式代码块的基础上前面加上**$** ，后面加上**$** 组成的，而行间公式则是在公式代码块前后使用**$$** 和**$$** 。

## 上标、下标与组合

1. 上标符号，符号：`^`，如：$x^4$
2. 下标符号，符号：`_`，如：$x_1$
3. 组合符号，符号：`{}`，如：${16}_{8}O{2+}_{2}$

## 汉字、字体与格式

1. 汉字形式，符号：`\mbox{}`，如：$V_{\mbox{初始}}$
2. 字体控制，符号：`\displaystyle`，如：$\displaystyle \frac{x+y}{y+z}$
3. 下划线符号，符号：`\underline`，如：$\underline{x+y}$
4. 标签，符号`\tag{数字}`，如：$\tag{11}$
5. 上大括号，符号：`\overbrace{算式}`，如：$\overbrace{a+b+c+d}^{2.0}$
6. 下大括号，符号：`\underbrace{算式}`，如：$a+\underbrace{b+c}_{1.0}+d$
7. 上位符号，符号：`\stacrel{上位符号}{基位符号}`，如：$\vec{x}\stackrel{\mathrm{def}}{=}{x_1,\dots,x_n}$

## 占位符

1. 两个quad空格，符号：`\qquad`，如：$x \qquad y$
2. quad空格，符号：`\quad`，如：$x \quad y$
3. 大空格，符号`\`，如：$x \ y$
4. 中空格，符号`\:`，如：$x : y$
5. 小空格，符号`\,`，如：$x , y$
6. 没有空格，符号``，如：$xy$
7. 紧贴，符号`\!`，如：$x ! y$

## 定界符与组合

1. 括号，符号：`（）\big(\big) \Big(\Big) \bigg(\bigg) \Bigg(\Bigg)`，如：$（）\big(\big) \Big(\Big) \bigg(\bigg) \Bigg(\Bigg)$
2. 中括号，符号：`[]`，如：$[x+y]$
3. 大括号，符号：`\{ \}`，如：${x+y}$
4. 自适应括号，符号：`\left \right`，如：$\left(x\right)$，$\left(x{yz}\right)$
5. 组合公式，符号：`{上位公式 \choose 下位公式}`，如：${n+1 \choose k}={n \choose k}+{n \choose k-1}$
6. 组合公式，符号：`{上位公式 \atop 下位公式}`，如：$\sum_{k_0,k_1,\ldots>0 \atop k_0+k_1+\cdots=n}A_{k_0}A_{k_1}\cdots$

## 四则运算

1. 加法运算，符号：`+`，如：$x+y=z$
2. 减法运算，符号：`-`，如：$x-y=z$
3. 加减运算，符号：`\pm`，如：$x \pm y=z$
4. 减加运算，符号：`\mp`，如：$x \mp y=z$
5. 乘法运算，符号：`\times`，如：$x \times y=z$
6. 点乘运算，符号：`\cdot`，如：$x \cdot y=z$
7. 星乘运算，符号：`\ast`，如：$x \ast y=z$
8. 除法运算，符号：`\div`，如：$x \div y=z$
9. 斜法运算，符号：`/`，如：$x/y=z$
10. 分式表示，符号：`\frac{分子}{分母}`，如：$\frac{x+y}{y+z}$
11. 分式表示，符号：`{分子} \voer {分母}`，如：${x+y} \over {y+z}$
12. 绝对值表示，符号：`||`，如：$|x+y|$

## 高级运算

1. 平均数运算，符号：`\overline{算式}`，如：$\overline{xyz}$
2. 开二次方运算，符号：`\sqrt`，如：$\sqrt x$
3. 开方运算，符号：`\sqrt[开方数]{被开方数}`，如：$\sqrt[3]{x+y}$
4. 对数运算，符号：`\log`，如：$\log(x)$
5. 极限运算，符号：`\lim`，如：$\lim^{x \to \infty}_{y \to 0}{\frac{x}{y}}$
6. 极限运算，符号：`\displaystyle \lim`，如：$\displaystyle \lim^{x \to \infty}_{y \to 0}{\frac{x}{y}}$
7. 求和运算，符号：`\sum`，如：$\sum^{x \to \infty}_{y \to 0}{\frac{x}{y}}$
8. 求和运算，符号：`\displaystyle \sum`，如：$\displaystyle \sum^{x \to \infty}_{y \to 0}{\frac{x}{y}}$
9. 积分运算，符号：`\int`，如：$\int^{\infty}_{0}{xdx}$
10. 积分运算，符号：`\displaystyle \int`，如：$\displaystyle \int^{\infty}_{0}{xdx}$
11. 微分运算，符号：`\partial`，如：$\frac{\partial x}{\partial y}$
12. 矩阵表示，符号：`\begin{matrix} \end{matrix}`，如：$\left[ \begin{matrix} 1 &2 &\cdots &4\5 &6 &\cdots &8\\vdots &\vdots &\ddots &\vdots\13 &14 &\cdots &16\end{matrix} \right]$

## 逻辑运算

1. 等于运算，符号：`=`，如：$x+y=z$
2. 大于运算，符号：`>`，如：$x+y>z$
3. 小于运算，符号：`<`，如：$x+y<z$
4. 大于等于运算，符号：`\geq`，如：$x+y \geq z$
5. 小于等于运算，符号：`\leq`，如：$x+y \leq z$
6. 不等于运算，符号：`\neq`，如：$x+y \neq z$
7. 不大于等于运算，符号：`\ngeq`，如：$x+y \ngeq z$
8. 不大于等于运算，符号：`\not\geq`，如：$x+y \not\geq z$
9. 不小于等于运算，符号：`\nleq`，如：$x+y \nleq z$
10. 不小于等于运算，符号：`\not\leq`，如：$x+y \not\leq z$
11. 约等于运算，符号：`\approx`，如：$x+y \approx z$
12. 恒定等于运算，符号：`\equiv`，如：$x+y \equiv z$

## 集合运算

1. 属于运算，符号：`\in`，如：$x \in y$
2. 不属于运算，符号：`\notin`，如：$x \notin y$
3. 不属于运算，符号：`\not\in`，如：$x \not\in y$
4. 子集运算，符号：`\subset`，如：$x \subset y$
5. 子集运算，符号：`\supset`，如：$x \supset y$
6. 真子集运算，符号：`\subseteq`，如：$x \subseteq y$
7. 非真子集运算，符号：`\subsetneq`，如：$x \subsetneq y$
8. 真子集运算，符号：`\supseteq`，如：$x \supseteq y$
9. 非真子集运算，符号：`\supsetneq`，如：$x \supsetneq y$
10. 非子集运算，符号：`\not\subset`，如：$x \not\subset y$
11. 非子集运算，符号：`\not\supset`，如：$x \not\supset y$
12. 并集运算，符号：`\cup`，如：$x \cup y$
13. 交集运算，符号：`\cap`，如：$x \cap y$
14. 差集运算，符号：`\setminus`，如：$x \setminus y$
15. 同或运算，符号：`\bigodot`，如：$x \bigodot y$
16. 同与运算，符号：`\bigotimes`，如：$x \bigotimes y$
17. 实数集合，符号：`\mathbb{R}`，如：`\mathbb{R}`
18. 自然数集合，符号：`\mathbb{Z}`，如：`\mathbb{Z}`
19. 空集，符号：`\emptyset`，如：$\emptyset$

## 数学符号

1. 无穷，符号：`\infty`，如：$\infty$
2. 虚数，符号：`\imath`，如：$\imath$
3. 虚数，符号：`\jmath`，如：$\jmath$
4. 数学符号，符号`\hat{a}`，如：$\hat{a}$
5. 数学符号，符号`\check{a}`，如：$\check{a}$
6. 数学符号，符号`\breve{a}`，如：$\breve{a}$
7. 数学符号，符号`\tilde{a}`，如：$\tilde{a}$
8. 数学符号，符号`\bar{a}`，如：$\bar{a}$
9. 矢量符号，符号`\vec{a}`，如：$\vec{a}$
10. 数学符号，符号`\acute{a}`，如：$\acute{a}$
11. 数学符号，符号`\grave{a}`，如：$\grave{a}$
12. 数学符号，符号`\mathring{a}`，如：$\mathring{a}$
13. 一阶导数符号，符号`\dot{a}`，如：$\dot{a}$
14. 二阶导数符号，符号`\ddot{a}`，如：$\ddot{a}$
15. 上箭头，符号：`\uparrow`，如：$\uparrow$
16. 上箭头，符号：`\Uparrow`，如：$\Uparrow$
17. 下箭头，符号：`\downarrow`，如：$\downarrow$
18. 下箭头，符号：`\Downarrow`，如：$\Downarrow$
19. 左箭头，符号：`\leftarrow`，如：$\leftarrow$
20. 左箭头，符号：`\Leftarrow`，如：$\Leftarrow$
21. 右箭头，符号：`\rightarrow`，如：$\rightarrow$
22. 右箭头，符号：`\Rightarrow`，如：$\Rightarrow$
23. 底端对齐的省略号，符号：`\ldots`，如：$1,2,\ldots,n$
24. 中线对齐的省略号，符号：`\cdots`，如：$x_1^2 + x_2^2 + \cdots + x_n^2$
25. 竖直对齐的省略号，符号：`\vdots`，如：$\vdots$
26. 斜对齐的省略号，符号：`\ddots`，如：$\ddots$



## **希腊字母**

| 名称    | 大写 | code    | 小写     | code     |
| ------- | ---- | ------- | -------- | -------- |
| alpha   | A    | A       | α        | \alpha   |
| beta    | B    | B       | β        | \beta    |
| gamma   | Γ    | \Gamma  | γ        | \gamma   |
| delta   | Δ    | \Delta  | δ        | \delta   |
| epsilon | E    | E       | ϵ        | \epsilon |
| zeta    | Z    | Z       | ζ        | \zeta    |
| eta     | H    | H       | η        | \eta     |
| theta   | Θ    | \Theta  | θ        | \theta   |
| iota    | I    | I       | ι        | \iota    |
| kappa   | K    | K       | κ        | \kappa   |
| lambda  | Λ    | \Lambda | λ        | \lambda  |
| mu      | M    | M       | μ        | \mu      |
| nu      | N    | N       | ν        | \nu      |
| xi      | Ξ    | \Xi     | ξ        | \xi      |
| omicron | O    | O       | ο        | \omicron |
| pi      | Π    | \Pi     | π        | \pi      |
| rho     | P    | P       | ρ        | \rho     |
| sigma   | Σ    | \Sigma  | σ        | \sigma   |
| tau     | T    | T       | τ        | \tau     |
| upsilon | Υ    | υ       | \upsilon |          |
| phi     | Φ    | \Phi    | ϕ        | \phi     |
| chi     | X    | X       | χ        | \chi     |
| psi     | Ψ    | \Psi    | ψ        | \psi     |
| omega   | Ω    | \Omega  | ω        | \omega   |

## **上标与下标**

  上标和下标分别使用`^` 与`_` ，例如`$x_i^2$`表示的是：$x_i^2$。
  默认情况下，上、下标符号仅仅对下一个组起作用。一个组即单个字符或者使用`{..}` 包裹起来的内容。如果使用`$10^10$` 表示的是$10^10$，而`$10^{10}$` 才是$10^{10}$。同时，大括号还能消除二义性，如`x^5^6` 将得到一个错误，必须使用大括号来界定^的结合性，如`${x^5}^6$` ：${x^5}^6$或者`$x^{5^6}$` ：$x^{5^6}$。

## **括号**

### 小括号与方括号

  使用原始的`( )` ，`[ ]` 即可，如`$(2+3)[4+4]$` ：![(2+3)](https://math.jianshu.com/math?formula=(2%2B3)) ![[4+4]](https://math.jianshu.com/math?formula=%5B4%2B4%5D)
  使用\left(或\right)使符号大小与邻近的公式相适应（该语句适用于所有括号类型），如`$\left(\frac{x}{y}\right)$` ：![\left(\frac{x}{y}\right)](https://math.jianshu.com/math?formula=%5Cleft(%5Cfrac%7Bx%7D%7By%7D%5Cright))

### 大括号

  由于大括号`{}` 被用于分组，因此需要使用`\{`和`\}`表示大括号，也可以使用`\lbrace` 和`\rbrace`来表示。如`$\{a\*b\}:a\∗b$` 或`$\lbrace a\*b\rbrace :a\*b$` 表示![\{a*b\}:a∗b](https://math.jianshu.com/math?formula=%5C%7Ba*b%5C%7D%3Aa%E2%88%97b)。

### 尖括号

  区分于小于号和大于号，使用`\langle` 和`\rangle` 表示左尖括号和右尖括号。如`$\langle x \rangle$` 表示：![\langle x \rangle](https://math.jianshu.com/math?formula=%5Clangle%20x%20%5Crangle)。

### 上取整

  使用`\lceil` 和 `\rceil` 表示。 如，`$\lceil x \rceil$`：![\lceil x \rceil](https://math.jianshu.com/math?formula=%5Clceil%20x%20%5Crceil)。

### 下取整

  使用`\lfloor` 和 `\rfloor` 表示。如，`$\lfloor x \rfloor$`：![\lfloor x \rfloor](https://math.jianshu.com/math?formula=%5Clfloor%20x%20%5Crfloor)。

## **求和与积分**

### 求和

  `\sum` 用来表示求和符号，其下标表示求和下限，上标表示上限。如:
  `$\sum_{r=1}^n$`表示：![\sum_{r=1}^n](https://math.jianshu.com/math?formula=%5Csum_%7Br%3D1%7D%5En)。
  `$$\sum_{r=1}^n$$`表示：![\sum_{r=1}^n](https://math.jianshu.com/math?formula=%5Csum_%7Br%3D1%7D%5En)

### 积分

  `\int` 用来表示积分符号，同样地，其上下标表示积分的上下限。如，`$\int_{r=1}^\infty$`：![\int_{r=1}^\infty](https://math.jianshu.com/math?formula=%5Cint_%7Br%3D1%7D%5E%5Cinfty)。
  多重积分同样使用 **int** ，通过 **i** 的数量表示积分导数：
  `$\iint$` ：![\iint](https://math.jianshu.com/math?formula=%5Ciint)
  `$\iiint$` ：![\iiint](https://math.jianshu.com/math?formula=%5Ciiint)
  `$\iiiint$` ：![\iiiint](https://math.jianshu.com/math?formula=%5Ciiiint)

### 连乘

  `$\prod {a+b}$`，输出：![\prod {a+b}](https://math.jianshu.com/math?formula=%5Cprod%20%7Ba%2Bb%7D)。
  `$\prod_{i=1}^{K}$`，输出：![\prod_{i=1}^{K}](https://math.jianshu.com/math?formula=%5Cprod_%7Bi%3D1%7D%5E%7BK%7D)。
  `$$\prod_{i=1}^{K}$$`，输出：![\prod_{i=1}^{K}](https://math.jianshu.com/math?formula=%5Cprod_%7Bi%3D1%7D%5E%7BK%7D)。

### 其他

  与此类似的符号还有，
  `$\prod$` ：$\prod$
  `$\bigcup$` ：$\bigcup$
  `$\bigcap$` ：$\bigcap$
  `$arg\,\max_{c_k}$`：$arg\,\max_{c_k}$
  `$arg\,\min_{c_k}$`：$arg\,\min_{c_k}​$
  `$\mathop {argmin}_{c_k}$`：![\mathop {argmin}_{c_k}](https://math.jianshu.com/math?formula=%5Cmathop%20%7Bargmin%7D_%7Bc_k%7D)
  `$\mathop {argmax}_{c_k}$`：![\mathop {argmax}_{c_k}](https://math.jianshu.com/math?formula=%5Cmathop%20%7Bargmax%7D_%7Bc_k%7D)
  `$\max_{c_k}$`：![\max_{c_k}](https://math.jianshu.com/math?formula=%5Cmax_%7Bc_k%7D)
  `$\min_{c_k}$`：![\min_{c_k}](https://math.jianshu.com/math?formula=%5Cmin_%7Bc_k%7D)

## **分式与根式**

### 分式

- 第一种，使用`\frac ab`，`\frac`作用于其后的两个组`a` ，`b` ，结果为![\frac ab](https://math.jianshu.com/math?formula=%5Cfrac%20ab)。如果你的分子或分母不是单个字符，请使用`{..}`来分组，比如`$\frac {a+c+1}{b+c+2}$`表示$\frac {a+c+1}{b+c+2}$。
- 第二种，使用`\over`来分隔一个组的前后两部分，如`${a+1\over b+1}$`：${a+1\over b+1}$连分数

  书写连分数表达式时，请使用`\cfrac`代替`\frac`或者`\over`两者效果对比如下：
  `\frac` 表示如下：

```
$$x=a_0 + \frac {1^2}{a_1 + \frac {2^2}{a_2 + \frac {3^2}{a_3 + \frac {4^2}{a_4 + ...}}}}$$
```

  显示如下：

$$x=a_0 + \frac {1^2}{a_1 + \frac {2^2}{a_2 + \frac {3^2}{a_3 + \frac {4^2}{a_4 + ...}}}}$$

  `\cfrac` 表示如下：

```
$$x=a_0 + \cfrac {1^2}{a_1 + \cfrac {2^2}{a_2 + \cfrac {3^2}{a_3 + \cfrac {4^2}{a_4 + ...}}}}$$
```

  显示如下：

$$x=a_0 + \cfrac {1^2}{a_1 + \cfrac {2^2}{a_2 + \cfrac {3^2}{a_3 + \cfrac {4^2}{a_4 + ...}}}}​$$

根式

  根式使用`\sqrt` 来表示。
  如开4次方：`$\sqrt[4]{\frac xy}$` ：$\sqrt[4]{\frac xy}​$。
  开平方：`$\sqrt {a+b}$`：$\sqrt {a+b}​$。

## **多行表达式**

### 分类表达式

  定义函数的时候经常需要分情况给出表达式，使用`\begin{cases}…\end{cases}` 。其中：

-   使用`\\` 来分类，
-   使用`&` 指示需要对齐的位置，
-   使用`\` +`空格`表示空格。

```
$$
f(n)
\begin{cases}
\cfrac n2, &if\ n\ is\ even\\
3n + 1, &if\  n\ is\ odd
\end{cases}
$$
```

  表示:
$$
f(n)
\begin{cases}
\cfrac n2, &if\ n\ is\ even\\
3n + 1, &if\  n\ is\ odd
\end{cases}
$$
```
$$
L(Y,f(X)) =
\begin{cases}
0, & \text{Y = f(X)}  \\
1, & \text{Y $\neq$ f(X)}
\end{cases}
$$
```

  表示:
$$
L(Y,f(X)) =
\begin{cases}
0, & \text{Y = f(X)}  \\
1, & \text{Y $\neq$ f(X)}
\end{cases}
$$
  如果想分类之间的垂直间隔变大，可以使用`\\[2ex]` 代替`\\` 来分隔不同的情况。(`3ex,4ex` 也可以用，`1ex` 相当于原始距离）。如下所示：

```
$$
L(Y,f(X)) =
\begin{cases}
0, & \text{Y = f(X)} \\[5ex]
1, & \text{Y $\neq$ f(X)}
\end{cases}
$$
```

  表示：
$$
L(Y,f(X)) =
\begin{cases}
0, & \text{Y = f(X)} \\[5ex]
1, & \text{Y $\neq$ f(X)}
\end{cases}
$$
### 多行表达式

  有时候需要将一行公式分多行进行显示。

```
$$
\begin{equation}\begin{split} 
a&=b+c-d \\ 
&\quad +e-f\\ 
&=g+h\\ 
& =i 
\end{split}\end{equation}
$$
```

  表示：
$$
\begin{equation}\begin{split} 
a&=b+c-d \\ 
&\quad +e-f\\ 
&=g+h\\ 
& =i 
\end{split}\end{equation}
$$
  其中`begin{equation}` 表示开始方程，`end{equation}` 表示方程结束；`begin{split}`表示开始多行公式，`end{split}` 表示结束；公式中用`\\` 表示回车到下一行，`&` 表示对齐的位置。

### 方程组

  使用`\begin{array}...\end{array}` 与`\left \{` 与`\right.` 配合表示方程组:

```
$$
\left \{ 
\begin{array}{c}
a_1x+b_1y+c_1z=d_1 \\ 
a_2x+b_2y+c_2z=d_2 \\ 
a_3x+b_3y+c_3z=d_3
\end{array}
\right.
$$
```

  表示：
$$
\left \{ 
\begin{array}{c}
a_1x+b_1y+c_1z=d_1 \\ 
a_2x+b_2y+c_2z=d_2 \\ 
a_3x+b_3y+c_3z=d_3
\end{array}
\right.
$$
  注意：通常MathJax通过内部策略自己管理公式内部的空间，因此`a…b` 与`a…….b`（`.`表示空格）都会显示为`ab` 。可以通过在`ab` 间加入`\` ,增加些许间隙，`\;` 增加较宽的间隙，`\quad` 与`\qquad` 会增加更大的间隙。

## **特殊函数与符号**

### 三角函数

  `$\sinx$` : $sinx​$
  `$\arctanx$` : ![arctanx](https://math.jianshu.com/math?formula=arctanx)

### 比较运算符

  小于(`\lt` )：$\lt$
  大于(`\gt` )：$\gt$
  小于等于(`\le` )：$\le$
  大于等于(`\ge` )：$\ge$
  不等于(`\ne` ) : $\ne$
  可以在这些运算符前面加上`\not` ，如`\not\lt` : ![\not\lt`](https://math.jianshu.com/math?formula=%5Cnot%5Clt%60)

### 集合关系与运算

  并集(`\cup` ): ![\cup](https://math.jianshu.com/math?formula=%5Ccup)
  交集(`\cap` ): ![\cap](https://math.jianshu.com/math?formula=%5Ccap)
  差集(`\setminus` ): ![\setminus](https://math.jianshu.com/math?formula=%5Csetminus)
  子集(`\subset` ): ![\subset](https://math.jianshu.com/math?formula=%5Csubset)
  子集(`\subseteq` ): ![\subseteq](https://math.jianshu.com/math?formula=%5Csubseteq)
  非子集(`\subsetneq` ): ![\subsetneq](https://math.jianshu.com/math?formula=%5Csubsetneq)
  父集(`\supset` ): ![\supset](https://math.jianshu.com/math?formula=%5Csupset)
  属于(`\in` ): ![\in](https://math.jianshu.com/math?formula=%5Cin)
  不属于(`\notin` ): ![\notin](https://math.jianshu.com/math?formula=%5Cnotin)
  空集(`\emptyset` ): ![\emptyset](https://math.jianshu.com/math?formula=%5Cemptyset)
  空(`\varnothing` ): ![\varnothing](https://math.jianshu.com/math?formula=%5Cvarnothing)

### 排列

  `$\binom{n+1}{2k}$` : $\binom{n+1}{2k}$
  `${n+1 \choose 2k}$` : ${n+1 \choose 2k}$

### 箭头

  (`\to` ):![\to](https://math.jianshu.com/math?formula=%5Cto)
  (`\rightarrow` ): ![\rightarrow](https://math.jianshu.com/math?formula=%5Crightarrow)
  (`\leftarrow` ): ![\leftarrow](https://math.jianshu.com/math?formula=%5Cleftarrow)
  (`\Rightarrow` ): ![\Rightarrow](https://math.jianshu.com/math?formula=%5CRightarrow)
  (`\Leftarrow` ): ![\Leftarrow](https://math.jianshu.com/math?formula=%5CLeftarrow)
  (`\mapsto` ): ![\mapsto](https://math.jianshu.com/math?formula=%5Cmapsto)

### 逻辑运算符

  (`\land` ): ![\land](https://math.jianshu.com/math?formula=%5Cland)
  (`\lor` ): ![\lor](https://math.jianshu.com/math?formula=%5Clor)
  (`\lnot` ): ![\lnot](https://math.jianshu.com/math?formula=%5Clnot)
  (`\forall` ): ![\forall](https://math.jianshu.com/math?formula=%5Cforall)
  (`\exists` ): ![\exists](https://math.jianshu.com/math?formula=%5Cexists)
  (`\top` ): ![\top](https://math.jianshu.com/math?formula=%5Ctop)
  (`\bot` ): ![\bot](https://math.jianshu.com/math?formula=%5Cbot)
  (`\vdash` ): ![\vdash](https://math.jianshu.com/math?formula=%5Cvdash)
  (`\vDash` ): ![\vDash](https://math.jianshu.com/math?formula=%5CvDash)

### 操作符

  (`\star` ): ![\star](https://math.jianshu.com/math?formula=%5Cstar)
  (`\ast` ): ![\ast](https://math.jianshu.com/math?formula=%5Cast)
  (`\oplus` ): ![\oplus](https://math.jianshu.com/math?formula=%5Coplus)
  (`\circ` ): ![\circ](https://math.jianshu.com/math?formula=%5Ccirc)
  (`\bullet` ): ![\bullet](https://math.jianshu.com/math?formula=%5Cbullet)

### 等于

  (`\approx` ): ![\approx](https://math.jianshu.com/math?formula=%5Capprox)
  (`\sim` ): ![\sim](https://math.jianshu.com/math?formula=%5Csim)
  (`\equiv` ): ![\equiv](https://math.jianshu.com/math?formula=%5Cequiv)
  (`\prec` ): ![\prec](https://math.jianshu.com/math?formula=%5Cprec)

### 范围

  (`\infty` ): ![\infty](https://math.jianshu.com/math?formula=%5Cinfty)
  (`\aleph_o` ): ![\aleph_o](https://math.jianshu.com/math?formula=%5Caleph_o)
  (`\nabla` ): ![\nabla](https://math.jianshu.com/math?formula=%5Cnabla)
  (`\Im` ): ![\Im](https://math.jianshu.com/math?formula=%5CIm)
  (`\Re` ): ![\Re](https://math.jianshu.com/math?formula=%5CRe)

### 模运算

  (`\pmod` ): ![b \pmod n](https://math.jianshu.com/math?formula=b%20%5Cpmod%20n)
  如`a \equiv b \pmod n` : ![a \equiv b \pmod n](https://math.jianshu.com/math?formula=a%20%5Cequiv%20b%20%5Cpmod%20n)

### 点

  (`\ldots` ): ![\ldots](https://math.jianshu.com/math?formula=%5Cldots)
  (`\cdots` ): ![\cdots](https://math.jianshu.com/math?formula=%5Ccdots)
  (`\cdot` ): ![\cdot](https://math.jianshu.com/math?formula=%5Ccdot)
  其区别是点的位置不同，`\ldots` 位置稍低，`\cdots` 位置居中。

```
$$
\begin{equation}
a_1+a_2+\ldots+a_n \\ 
a_1+a_2+\cdots+a_n
\end{equation}
$$
```

  表示：
$$
\begin{equation}
a_1+a_2+\ldots+a_n \\ 
a_1+a_2+\cdots+a_n
\end{equation}
$$
**顶部符号**

  对于单字符，`\hat x` ：![\hat x](https://math.jianshu.com/math?formula=%5Chat%20x)
  多字符可以使用`\widehat {xy}` ：![\widehat {xy}](https://math.jianshu.com/math?formula=%5Cwidehat%20%7Bxy%7D)
  类似的还有:
  (`\overline x` ): ![\overline x](https://math.jianshu.com/math?formula=%5Coverline%20x)
  矢量(`\vec` ): ![\vec x](https://math.jianshu.com/math?formula=%5Cvec%20x)
  向量(`\overrightarrow {xy}` ): ![\overrightarrow {xy}](https://math.jianshu.com/math?formula=%5Coverrightarrow%20%7Bxy%7D)
  (`\dot x` ): ![\dot x](https://math.jianshu.com/math?formula=%5Cdot%20x)
  (`\ddot x` ): ![\ddot x](https://math.jianshu.com/math?formula=%5Cddot%20x)
  (`\dot {\dot x}` ): ![\dot {\dot x}](https://math.jianshu.com/math?formula=%5Cdot%20%7B%5Cdot%20x%7D)

## **表格**

  使用`\begin{array}{列样式}…\end{array}` 这样的形式来创建表格，列样式可以是`clr`表示居中，左，右对齐，还可以使用`|` 表示一条竖线。表格中各行使用`\\` 分隔，各列使用`&` 分隔。使用`\hline` 在本行前加入一条直线。 例如:

```
$$
\begin{array}{c|lcr}
n & \text{Left} & \text{Center} & \text{Right} \\
\hline
1 & 0.24 & 1 & 125 \\
2 & -1 & 189 & -8 \\
3 & -20 & 2000 & 1+10i \\
\end{array}
$$
```

  得到：
$$
\begin{array}{c|lcr}
n & \text{Left} & \text{Center} & \text{Right} \\
\hline
1 & 0.24 & 1 & 125 \\
2 & -1 & 189 & -8 \\
3 & -20 & 2000 & 1+10i \\
\end{array}
$$
## **矩阵**

### 基本内容

  使用`\begin{matrix}…\end{matrix}` 这样的形式来表示矩阵，在`\begin` 与`\end` 之间加入矩阵中的元素即可。矩阵的行之间使用`\\` 分隔，列之间使用`&` 分隔，例如:

```
$$
\begin{matrix}
1 & x & x^2 \\
1 & y & y^2 \\
1 & z & z^2 \\
\end{matrix}
$$
```

  得到：
$$
\begin{matrix}
1 & x & x^2 \\
1 & y & y^2 \\
1 & z & z^2 \\
\end{matrix}
$$
### 括号

  如果要对矩阵加括号，可以像上文中提到的一样，使用`\left` 与`\right` 配合表示括号符号。也可以使用特殊的`matrix` 。即替换`\begin{matrix}…\end{matrix}` 中`matrix` 为`pmatrix` ，`bmatrix` ，`Bmatrix` ，`vmatrix` , `Vmatrix` 。

1. pmatrix`$\begin{pmatrix}1 & 2 \\ 3 & 4\\ \end{pmatrix}$` : $\begin{pmatrix}1 & 2 \\ 3 & 4\\ \end{pmatrix}$
2. bmatrix`$\begin{bmatrix}1 & 2 \\ 3 & 4\\ \end{bmatrix}$` : $\begin{bmatrix}1 & 2 \\ 3 & 4\\ \end{bmatrix}​$
3. Bmatrix`$\begin{Bmatrix}1 & 2 \\ 3 & 4\\ \end{Bmatrix}$` : $\begin{Bmatrix}1 & 2 \\ 3 & 4\\ \end{Bmatrix}$
4. vmatrix`$\begin{vmatrix}1 & 2 \\ 3 & 4\\ \end{vmatrix}$` : $\begin{vmatrix}1 & 2 \\ 3 & 4\\ \end{vmatrix}$
5. Vmatrix`$\begin{Vmatrix}1 & 2 \\ 3 & 4\\ \end{Vmatrix}$` : $\begin{Vmatrix}1 & 2 \\ 3 & 4\\ \end{Vmatrix}$

### 元素省略

  可以使用`\cdots` ：⋯，`\ddots`：⋱ ，`\vdots`：⋮ 来省略矩阵中的元素，如：

```
$$
\begin{pmatrix}
1&a_1&a_1^2&\cdots&a_1^n\\
1&a_2&a_2^2&\cdots&a_2^n\\
\vdots&\vdots&\vdots&\ddots&\vdots\\
1&a_m&a_m^2&\cdots&a_m^n\\
\end{pmatrix}
$$
```

  表示：
$$
\begin{pmatrix}
1&a_1&a_1^2&\cdots&a_1^n\\
1&a_2&a_2^2&\cdots&a_2^n\\
\vdots&\vdots&\vdots&\ddots&\vdots\\
1&a_m&a_m^2&\cdots&a_m^n\\
\end{pmatrix}
$$
### 增广矩阵

  增广矩阵需要使用前面的表格中使用到的`\begin{array} ... \end{array}` 来实现。

```
$$
\left[  \begin{array}  {c c | c} %这里的c表示数组中元素对其方式：c居中、r右对齐、l左对齐，竖线表示2、3列间插入竖线
1 & 2 & 3 \\
\hline %插入横线，如果去掉\hline就是增广矩阵
4 & 5 & 6
\end{array}  \right]
$$
```

显示为：
$$
\left[  \begin{array}  {c c | c} %这里的c表示数组中元素对其方式：c居中、r右对齐、l左对齐，竖线表示2、3列间插入竖线
1 & 2 & 3 \\
\hline %插入横线，如果去掉\hline就是增广矩阵
4 & 5 & 6
\end{array}  \right]
$$

## **公式标记与引用**

  使用`\tag{yourtag}` 来标记公式，如果想在之后引用该公式，则还需要加上`\label{yourlabel}` 在`\tag` 之后，如`$$a = x^2 - y^3 \tag{1}\label{1}$$` 显示为：
![a := x^2 - y^3 \tag{1}\label{311}](https://math.jianshu.com/math?formula=a%20%3A%3D%20x%5E2%20-%20y%5E3%20%5Ctag%7B1%7D%5Clabel%7B311%7D)
  如果不需要被引用，只使用`\tag{yourtag}` ，`$$x+y=z\tag{1.1}$$`显示为：
![x+y=z\tag{1.1}](https://math.jianshu.com/math?formula=x%2By%3Dz%5Ctag%7B1.1%7D)
  `\tab{yourtab}` 中的内容用于显示公式后面的标记。公式之间通过`\label{}` 设置的内容来引用。为了引用公式，可以使用`\eqref{yourlabel}` ，如`$$a + y^3 \stackrel{\eqref{1}}= x^2$$` 显示为：
![a + y^3 \stackrel{\eqref{1}}= x^2](https://math.jianshu.com/math?formula=a%20%2B%20y%5E3%20%5Cstackrel%7B%5Ceqref%7B1%7D%7D%3D%20x%5E2)

或者使用`\ref{yourlabel}` 不带括号引用，如`$$a + y^3 \stackrel{\ref{111}}= x^2$$` 显示为:
![a + y^3 \stackrel{\ref{1}}= x^2](https://math.jianshu.com/math?formula=a%20%2B%20y%5E3%20%5Cstackrel%7B%5Cref%7B1%7D%7D%3D%20x%5E2)

## **字体**

### 黑板粗体字

此字体经常用来表示代表实数、整数、有理数、复数的大写字母。
`$\mathbb ABCDEF$`：![\mathbb ABCDEF](https://math.jianshu.com/math?formula=%5Cmathbb%20ABCDEF)
`$\Bbb ABCDEF$`：![\Bbb ABCDEF](https://math.jianshu.com/math?formula=%5CBbb%20ABCDEF)

### 黑体字

`$\mathbf ABCDEFGHIJKLMNOPQRSTUVWXYZ$` :![\mathbf ABCDEFGHIJKLMNOPQRSTUVWXYZ](https://math.jianshu.com/math?formula=%5Cmathbf%20ABCDEFGHIJKLMNOPQRSTUVWXYZ)
`$\mathbf abcdefghijklmnopqrstuvwxyz$` :![\mathbf abcdefghijklmnopqrstuvwxyz](https://math.jianshu.com/math?formula=%5Cmathbf%20abcdefghijklmnopqrstuvwxyz)

### 打印机字体

`$\mathtt ABCDEFGHIJKLMNOPQRSTUVWXYZ$` :![\mathtt ABCDEFGHIJKLMNOPQRSTUVWXYZ](https://math.jianshu.com/math?formula=%5Cmathtt%20ABCDEFGHIJKLMNOPQRSTUVWXYZ)

## **参考文档**

| #    | 链接地址                                          | 文档名称                                                     |
| ---- | ------------------------------------------------- | ------------------------------------------------------------ |
| 1    | `blog.csdn.net/dabokele/article/details/79577072` | [Mathjax公式教程](https://blog.csdn.net/dabokele/article/details/79577072) |
| 2    | `blog.csdn.net/ethmery/article/details/50670297`  | [基本数学公式语法](https://blog.csdn.net/ethmery/article/details/50670297) |
| 3    | `blog.csdn.net/lilongsy/article/details/79378620` | [常用数学符号的LaTeX表示方法](https://blog.csdn.net/lilongsy/article/details/79378620) |
| 4    | `www.mathjax.org`                                 | [Beautiful math in all browsers](https://www.mathjax.org/)   |

小礼物走一走，来简书关注我

赞赏支持