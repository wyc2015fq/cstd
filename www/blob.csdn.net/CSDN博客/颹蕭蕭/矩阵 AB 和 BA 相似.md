
# 矩阵 AB 和 BA 相似 - 颹蕭蕭 - CSDN博客


2018年12月25日 22:15:37[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：2049标签：[相似																](https://so.csdn.net/so/search/s.do?q=相似&t=blog)[对角化																](https://so.csdn.net/so/search/s.do?q=对角化&t=blog)[矩阵																](https://so.csdn.net/so/search/s.do?q=矩阵&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=对角化&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=相似&t=blog)个人分类：[矩阵论																](https://blog.csdn.net/itnerd/article/category/8104256)
[
																								](https://so.csdn.net/so/search/s.do?q=相似&t=blog)


同阶矩阵 A 和 B**相似的充要条件**：特征多项式相同，即
$$
|\lambda I - A| = |\lambda I - B|
$$

---引理：假设

$$
A \in \R^{m \times n}
$$
---，

$$
B \in \R^{n \times m}
$$

$$
\left[\begin{matrix}
   AB &amp; 0  \\
   B &amp; 0  
  \end{matrix} \right]_{(m+n)\times(m+n)}
  \sim 
   \left[\begin{matrix}
   0 &amp; 0  \\
   B &amp; BA  
  \end{matrix} \right]_{(m+n)\times(m+n)}
$$

---证明：

$$
\left[\begin{matrix}
   I_m &amp; -A  \\
    &amp; I_n  
  \end{matrix} \right]
   \left[\begin{matrix}
   AB &amp; 0  \\
   B &amp; 0  
  \end{matrix} \right]
   \left[\begin{matrix}
   I_m &amp; A  \\
    &amp; I_n  
  \end{matrix} \right]=
   \left[\begin{matrix}
   0 &amp; 0  \\
   B &amp; BA  
  \end{matrix} \right]
$$

---由引理和相似的充要条件可得：

$$
\left|\begin{matrix}
   \lambda I_m-AB &amp; 0  \\
   B &amp; \lambda I_n  
  \end{matrix} \right|=
   \left|\begin{matrix}
   \lambda I_m &amp; 0  \\
   B &amp; \lambda I_n -BA  
  \end{matrix} \right|
$$
---因此有

$$
|\lambda I_m-AB|\lambda^n = \lambda^m |\lambda I_n -BA|
$$

---特别的，如果

$$
m = n
$$
---, 即可推出 AB 相似于 BA，因为

$$
|\lambda I-AB| =  |\lambda I -BA|.
$$

---由此可以得到一些有趣的结论，比如

$$
\forall x,y \in \R^{n \times 1}
$$
---，

$$
xy^T
$$
---可以对角化。

---因为

$$
xy^T =    
\left[\begin{matrix}
   x_1 \\
   \vdots  \\
   x_n  
  \end{matrix} \right]_{n\times 1}
  \left[\begin{matrix}
   y_1 &amp; \cdots &amp; y_n
  \end{matrix} \right]_{1\times n}=
  \left[\begin{matrix}
   x_1 &amp; 0 &amp;  \cdots &amp;0\\
   \vdots &amp; \vdots &amp;&amp; \vdots \\
   x_n  &amp; 0 &amp;  \cdots &amp; 0
  \end{matrix} \right]_{n\times n}
  \left[\begin{matrix}
   y_1 &amp; \cdots &amp; y_n\\
   0 &amp; \cdots &amp; 0  \\
   \vdots &amp; &amp; \vdots \\
  0 &amp; \cdots &amp; 0  
  \end{matrix} \right]_{n\times n}
$$

---将扩充的矩阵记为 X 和 Y，则有

$$
XY\sim YX
$$
---, 即

$$
xy^T = XY \sim YX =
    \left[\begin{matrix}
   y_1 &amp; \cdots &amp; y_n\\
   0 &amp; \cdots &amp; 0  \\
   \vdots &amp; &amp; \vdots \\
  0 &amp; \cdots &amp; 0  
  \end{matrix} \right]_{n\times n}
  \left[\begin{matrix}
   x_1 &amp; 0 &amp;  \cdots &amp;0\\
   \vdots &amp; \vdots &amp;&amp; \vdots \\
   x_n  &amp; 0 &amp;  \cdots &amp; 0
  \end{matrix} \right]_{n\times n}\\=
    \left[\begin{matrix}
   y^Tx  &amp;0&amp;\cdots &amp;0\\
   0 &amp; 0&amp;\cdots &amp; 0\\
   \vdots &amp;  \vdots&amp; &amp; \vdots\\
    0 &amp; 0 &amp;\cdots &amp; 0
  \end{matrix} \right]_{n\times n}
$$

---可见

$$
xy^T
$$
---相似于对角阵。


