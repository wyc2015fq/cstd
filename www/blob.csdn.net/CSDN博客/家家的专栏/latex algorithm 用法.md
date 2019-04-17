# latex algorithm 用法 - 家家的专栏 - CSDN博客





2014年05月20日 19:06:28[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：22433
个人分类：[latex](https://blog.csdn.net/yihaizhiyan/article/category/1718689)









示例：

\usepackage{algorithm}

\usepackage{algorithmic}





\begin{algorithm}[t]

\caption{\small Solving problem (\ref{eq_genpro}) by IRNN}

\textbf{Input:} $\mu>L(f)$ - A Lipschitz constant of $\nabla f({\X})$.\\

\textbf{Initialize:} $k=0$, ${\X}^k$, and $w^k_i$, $i=1,\cdots,m$.\\

\textbf{Output:} $X^*$. \\

\textbf{while} not converge \textbf{do}

\begin{enumerate}

  \item Update ${\X}^{k+1}$ by solving problem (\ref{eq_updatexlin}).

%  \begin{equation}

%  \begin{split}

%  {\X}^{k+1}=&\arg\min\limits_{{\X}}\sum_{i=1}^mw_i^k\sigma_i\\

%  &+\frac{\mu}{2}\left\|{\X}-\left({\X}^k-\frac{1}{\mu}\nabla f({\X}^k)\right)\right\|_F^2.

%  \end{split}

%  \end{equation}

  \item Update the weights $w_i^{k+1}$, $i=1,\cdots,m$, by

    \begin{equation}

    w_i^{k+1}\in\partial g_\lambda\left(\sigma_i^k\right).

    \end{equation}

\textbf{end while}

\end{enumerate}

\label{alg_lirnn}

\end{algorithm}




输出结果：

![](https://img-blog.csdn.net/20140520190613906)







