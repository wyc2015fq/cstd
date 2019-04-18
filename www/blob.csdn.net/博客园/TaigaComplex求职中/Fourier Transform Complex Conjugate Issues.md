# Fourier Transform Complex Conjugate Issues - TaigaComplex求职中 - 博客园







# [Fourier Transform Complex Conjugate Issues](https://www.cnblogs.com/TaigaCon/p/9125855.html)





FT of function $f(t)$ is to take integration of the product of $f(t)$ and $e^{-j\Omega t}$

$\displaystyle{ \mathcal{F}\Big( f(t) \Big) = \int_{-\infty}^{\infty}f(t)e^{-j\Omega t}dt }$

The complex conjugate issues of FT can be seen on the product of $f(t)$ and $e^{-j\Omega t}$.

$\begin{align*}
f( t)e^{-j\Omega t}
&=[f_R( t)+if_I( t)][cos(-\Omega t)+isin(-\Omega t)]\\
&=f_R( t)cos(-\Omega t)-f_I( t)sin(-\Omega t)+i[f_I( t)cos(-\Omega t)+f_R( t)sin(-\Omega t)]\\
\end{align*}$



Take FT on the time flipped function $f(-t)$

$\begin{align*}
f(- t)e^{-j\Omega t}
&=[f_R(- t)+if_I(- t)][cos(-\Omega t)+isin(-\Omega t)]\\
&=f_R(- t)cos(-\Omega t)-f_I(- t)sin(-\Omega t)+i[f_I(- t)cos(-\Omega t)+f_R(- t)sin(-\Omega t)]\\
&=f_R(v)cos(\Omega v)-f_I(v)sin(\Omega v)+i[f_I(v)cos(\Omega v)+f_R(v)sin(\Omega v)]\quad v = -t\\
&=f_R(v)cos(-\Omega v)+f_I(v)sin(-\Omega v)+i[f_I(v)cos(-\Omega v)-f_R(v)sin(-\Omega v)]\\
&=f_R(v)cos(-\Omega v)+f_I(v)sin(-\Omega v)-i[-f_I(v)cos(-\Omega v)+f_R(v)sin(-\Omega v)]\\
\end{align*}$

Compare the derivations. Only if the function $f(t)$ is real ($f_I = 0$) can we get the equation:

$f(t)e^{-j\Omega t} = \overline{f(-t)e^{-j\Omega t}}$

Substitute the result into FT, it can be concluded that **if $f(t)$ is real**, the FT of $f(t)$ is complex conjugate to the FT of $f(-t)$

$\color{red}{\mathcal{F}\Big(f(-t)\Big) = F^{*}(j\Omega) \qquad for\ f(t)\ is\ real}$





Take FT on the complex conjugate function $f^{*}(t) = f_R(t) – if_I(t)$

$\begin{align*}
f^*(t)e^{-j\Omega t}
&=[f_R(t)-if_I( t)][cos(-\Omega t)+isin(-\Omega t)]\\
&=f_R(t)cos(-\Omega t)+f_I(t)sin(-\Omega t)+i[-f_I(t)cos(-\Omega t)+f_R(t)sin(-\Omega t)]\\
&=f_R(t)cos(\Omega t)-f_I(t)sin(\Omega t)+i[-f_I(t)cos(\Omega t)-f_R(t)sin(\Omega t)]\\
&=f_R(t)cos(\Omega t)-f_I(t)sin(\Omega t)-i[f_I(t)cos(\Omega t)+f_R(t)sin(\Omega t)]\\
\end{align*}$

Compare the derivations. The sign of $\Omega$ and the sign of imagine part have changed. 

$f^*(t)e^{-j\Omega t} = \overline{f(t)e^{j\Omega t}}$

Substitute the result into FT

$\color{red}{\mathcal{F}\Big(f^*(t)\Big) = F^*(-j\Omega)}$












