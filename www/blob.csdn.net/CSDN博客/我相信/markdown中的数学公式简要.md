# markdown中的数学公式简要 - 我相信...... - CSDN博客





2017年04月24日 13:26:36[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：9326








在CSDN的博客开始支持Markdown编辑器后，自己完全转向了markdown编辑器。

Markdown的语法简洁明了、学习容易，而且功能比纯文本更强，因此用它写博客非常方便，还可以轻松的导出PDF或者HTML的文件。github上的wiki，各个project 中的readme，以及国内的“简书”都是Markdown 的用武之地。更重要的是，很多开发工具也都支持Markdown的插件，例如RStudio，Sublime Text等等。

Markdown 有很多现成的工具，例如Markdownpad，mou等，CSDN 的 Markdown 编辑器提供了示例模版，可以分分钟上手使用。网络上的资料更多：
- 
github的书写方式：  
[https://help.github.com/categories/writing-on-github/](https://help.github.com/categories/writing-on-github/)

- 
markdown 语法规范： 
[http://daringfireball.net/projects/markdown/syntax](http://daringfireball.net/projects/markdown/syntax)

- markdown 写作指南：  
[https://www.binarization.com/archive/2016/markdown-guide/#help](https://www.binarization.com/archive/2016/markdown-guide/#help)

老码农出于对开源工具的热爱，推荐在mac平台使用 MacDown 这一工具，可以到作者的blog上直接下载：

```
http://macdown.uranusjr.com/blog/
```

也可以到github上去源码编译: 

```
https://github.com/MacDownApp/macdown
```

为什么是Mac平台？请参见[《全栈必备 开发学习环境》](http://blog.csdn.net/wireless_com/article/details/53148971)一文。

在学习和实践的过程中，经常会遇到诸如算法之类的数学公式，在原始的Markdown中是不支持的，一般采用的方式： 

1） 在其他的编辑器（例如word）中编辑好，然后制成图片，插入到Markdown的文档中 

2）利用在线编辑器的web服务，将链接引入到markdown的文档中
这两种方式都不是很方便，实际上，很多的Markdown编辑器都是支持数学公式编辑的，通过的方式就是LaTeX 扩展，比如CSN的Markdown博客编辑器使用了[MathJax](http://math.stackexchange.com) 来支持LaTeX的：

![CSDN 的Markdown 编辑器](https://img-blog.csdn.net/20170423152239713?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

LaTeX 是大神Leslie Lamport 的杰作，该神是2013年图灵奖的获得者，感兴趣可以去瞻仰一下神人的相关著述： 
[http://lamport.azurewebsites.net/pubs/pubs.html](http://lamport.azurewebsites.net/pubs/pubs.html)

LaTeX是一种基于ΤΕΧ的排版系统，对于生成复杂表格和数学公式表现得尤为突出。LaTeX也是当今世界上最流行和使用最为广泛的TeX格式。它构筑在 PlainTeX的基础之上，并加进了很多功能以利用TeX的强大功能，更多内容可以参考一下《LaTeX Cookbook》或者国内的《LaTeX 入门》等书籍。

LaTeX 博大精深，但对写博客而言，或许只需要知道凤毛麟角就勉强够用了。在MacDown中，只需勾选响应的选项，就可以使用LaTeX的语法编辑数学公式了，如下图： 
![这里写图片描述](https://img-blog.csdn.net/20170423152618761?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
对于常用的数学公式语法，以及一般遇到的场景，简要汇总如下：

1)角标 和 开方：

```matlab
$c = \sqrt{a^{2}+b_{xy}^{2} +e^{x}}$
```



$c = \sqrt{a^{2}+b_{xy}^{2} +e^{x}}$

2)分数表达：
`    $\frac{x^{2}y^{2}}{x+y}$`


$\frac{x^{2}y^{2}}{x+y}$

3)求和与联乘：
`    $$\sum_{i=0}^{n}$  \prod_\epsilon$$`


$\sum_{i=0}^{n} \prod_\epsilon$

4)微分与积分
`    $\int_{a}^{\pi} f(x) \,dx$  `


$\int_{a}^{\pi} f(x) dx$

5)极限 与 偏导数

```
$$ \lim_{x \to +\infty}\frac{1}{x}$$

    $$\frac{\partial^2 u}{\partial z^2}$$
```



$\lim_{x \to +\infty}\frac{1}{x}$


$\frac{\partial^2 u}{\partial z^2}$

6.三角函数与矩阵

```
$$\cos2\theta = cos^2\theta - \sin^2 \theta \\ =  2\cos^2\theta -1$$

$$\begin{equation}
A=\left[
\begin{matrix}
1&2&3&\\
2&2&3&\\
3&2&3&
\end{matrix}
\right]
\end{equation}$$
```



$\cos2\theta = cos^2\theta - \sin^2 \theta \\ =  2\cos^2\theta -1$



$\begin{equation}A=\left[\begin{matrix}1&2&3&\\3&4&4&\\5&4&4&\end{matrix}\right]\end{equation}$

7.数学符号与运算符 

那些希腊字母，关系和运算符号还是查表吧。 
![](https://img-blog.csdn.net/20170423152920434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170423153258655?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170423153715939?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
更多用法参见[《一份不太简短的LaTex 介绍》](http://www.mohu.org/info/lshort-cn.pdf)。

最后，在这里写下传说中“上帝创造的公式”吧——欧拉公式。



$e^{i\pi} + 1 = 0$






