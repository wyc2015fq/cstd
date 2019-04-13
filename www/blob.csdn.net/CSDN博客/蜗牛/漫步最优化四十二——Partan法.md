
# 漫步最优化四十二——Partan法 - 蜗牛 - CSDN博客


2017年11月10日 22:09:44[会敲键盘的猩猩](https://me.csdn.net/u010182633)阅读数：371



$\textbf{漆黑的冷空中有你，}$
$\textbf{惺忪的眼睛中有你，}$
$\textbf{心底的记忆中有你，}$
$\textbf{你留在我的脑海中，}$
$\textbf{一直这么挥之不去。}$
$\textbf{无论哪时哪刻，}$
$\textbf{心中都想着你的笑，}$
$\textbf{想着你到我侧相拥，}$
$\textbf{I can dream about you.}$
$\textbf{——畅宝宝的傻逼哥哥}$
在早期的最优化中，对于两变量函数来说，用最速下降法得出的解轨迹表征出zig-zag模式。对于某些性质较好的函数，相邻的解差不多组成两条线，他们在最小值的邻域内相交，如图1所示，因此比较明显的策略是连接初始点与第二个解，沿着这个方向执行最速下降法。对于凸二次函数，在$n$次迭代内就能收敛，这个方法也被称为parallel tangent法或着partan法，这是因为在二次函数的情况下，所得轮廓的正切属性。
![这里写图片描述](https://img-blog.csdn.net/20171110215303767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171110215303767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图1
Partan算法如图2所示，假设初始点为$\mathbf{x}_0$，并利用两次最速下降法得到点$\mathbf{x}_1,\mathbf{y}_1$，然后沿着$\mathbf{y}_1-\mathbf{x}_1$方向进行线搜索得到点$\mathbf{x}_2$，这就完成了第一次迭代。对于第二次迭代，对点$\mathbf{x}_2$执行最速下降得到点$\mathbf{y}_2$，沿着$\mathbf{y}_2-\mathbf{x}_1$方向得到点$\mathbf{x}_3$，一直重复此过程。从效果上看，图2中的点$\mathbf{y}_1,\mathbf{y}_2,\ldots$是通过最速下降法得到的而$\mathbf{x}_2,\mathbf{x}_3,\ldots$是沿着方向$\mathbf{y}_2-\mathbf{x}_1,\mathbf{y}_3-\mathbf{x}_2,\ldots$方向用线搜索得到的。
![这里写图片描述](https://img-blog.csdn.net/20171110215348162?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171110215348162?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图2
对于凸二次问题，连接$\mathbf{x}_1,\mathbf{x}_2,\ldots,\mathbf{x}_k$的线组成一个共轭梯度方向集，可以通过以下方法来证明：先假设$\mathbf{d}_0,\mathbf{d}_1,\ldots,\mathbf{d}_{k-1}$是共轭梯度方向集，然后说明$\mathbf{d}_k$是$\mathbf{d}_0,\mathbf{d}_1,\ldots,\mathbf{d}_{k-1}$的共轭梯度方向。
考虑图3所示的步骤，注意到

$$
\begin{equation}
\mathbf{g}_k^T\mathbf{d}_i=0\quad\text{for}\ 0\leq i<k\tag1
\end{equation}
$$
根据之前共轭梯度的结论可知点$\mathbf{x}_{k-1}$处的梯度可以写成

$$
\mathbf{g}_{k-1}=\sum_{i=0}^{k-1}a_i\mathbf{d}_i
$$
其中$a_i,i=0,1,\ldots,k-1$为常数，所以

$$
\begin{equation}
\mathbf{g}_{k}^T\mathbf{g}_{k-1}=\mathbf{g}_k^T(\mathbf{b}+\mathbf{Hx}_{k-1})=\sum_{i=0}^{k-1}a_i\mathbf{g}_k^T\mathbf{d}_i=0\tag2
\end{equation}
$$
或者

$$
\begin{equation}
\mathbf{g}_k^T\mathbf{b}=-\mathbf{g}_k^T\mathbf{Hx}_{k-1}\tag3
\end{equation}
$$
因为$\mathbf{y}_k$是点$\mathbf{x}_k$用最速下降法得到的，所以我们有

$$
\mathbf{y}_k-\mathbf{x}_k=-\mathbf{g}_k
$$
另外

$$
-\mathbf{g}(\mathbf{y}_k)^T\mathbf{g}_k=\mathbf{g}_k^T(\mathbf{b}+\mathbf{Hy}_k)=0
$$
或者

$$
\begin{equation}
\mathbf{g}_k^T\mathbf{b}=-\mathbf{g}_k^T\mathbf{Hy}_k\tag4
\end{equation}
$$
因此，根据等式3与4可得

$$
\begin{equation}
\mathbf{g}_k^T\mathbf{H}(\mathbf{y}_k-\mathbf{x}_{k-1})=0\tag5
\end{equation}
$$

![这里写图片描述](https://img-blog.csdn.net/20171110215440215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171110215440215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图3
因为

$$
\mathbf{y}_k-\mathbf{x}_{k-1}=\beta(\mathbf{x}_{k-1}-\mathbf{x}_{k-1})
$$
其中$\beta$是常数，等式5可以写成

$$
\mathbf{g}_k^T\mathbf{H}(\mathbf{x}_{k+1}-\mathbf{x}_{k-1})=0
$$
或者

$$
\begin{equation}
\mathbf{g}_k^T\mathbf{H}\mathbf{x}_{k+1}=\mathbf{g}_k^T\mathbf{H}\mathbf{x}_{k-1}\tag6
\end{equation}
$$
接下来我们能够写成

$$
\begin{equation}
\mathbf{g}_k^T\mathbf{Hx}_{k+1}=\mathbf{g}_k^T\mathbf{Hx}_{k-1}\tag7
\end{equation}
$$
那么根据

$$
\begin{equation}
\mathbf{g}_k^T\mathbf{g}_{k+1}=\mathbf{g}_k^T(\mathbf{b}+\mathbf{Hx}_{k+1})\tag8
\end{equation}
$$
以及等式2，等式6与等式9可得

$$
\begin{align}
\mathbf{g}_{k}^T\mathbf{g}_{k+1}
&=\mathbf{g}_k^T(\mathbf{b}+\mathbf{Hx}_{k-1})\notag\\
&=\mathbf{g}_k^T\mathbf{g}_{k-1}=0\tag9
\end{align}
$$
点$\mathbf{x}_{k+1}$是在$\mathbf{x}_{k+1}-\mathbf{y}_k$方向上使用线搜索得到的，因此

$$
\begin{equation}
\mathbf{g}_{k+1}^T(\mathbf{x}_{k+1}-\mathbf{y}_k)=0\tag{10}
\end{equation}
$$
从图3可以看出

$$
\begin{equation}
\mathbf{x}_{k+1}=\mathbf{x}_k+\mathbf{d}_k\tag{11}
\end{equation}
$$
且

$$
\begin{equation}
\mathbf{y}_k=\mathbf{x}_k-\alpha\mathbf{g}_k\tag{12}
\end{equation}
$$
其中$\alpha$是最小化$f(\mathbf{x}_k-\alpha\mathbf{g}_k)$的$\alpha$值，因此等式9，10与11得到

$$
\mathbf{g}_{k+1}^T(\mathbf{d}_k+\alpha_k\mathbf{g}_k)=0
$$
或者

$$
\begin{equation}
\mathbf{g}_{k+1}^T\mathbf{d}_{k}+\alpha_k\mathbf{g}_k^T\mathbf{g}_{k+1}=0\tag{13}
\end{equation}
$$
接下来根据等式8与12可得

$$
\mathbf{g}_{k+1}^T\mathbf{d}_k=0
$$
再结合等式1与13可得

$$
\mathbf{g}_{k+1}^T\mathbf{d}_i=0\quad\text{for}\ 0\leq i<k+1
$$


