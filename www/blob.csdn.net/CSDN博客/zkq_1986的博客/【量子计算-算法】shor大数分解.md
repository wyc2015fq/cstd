# 【量子计算-算法】shor大数分解 - zkq_1986的博客 - CSDN博客





2018年01月10日 19:58:18[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：2421








shor大数分解算法

![](https://img-blog.csdn.net/20180110191932915)


![](https://img-blog.csdn.net/20180110192033677)


![](https://img-blog.csdn.net/20180110192047841)


![](https://img-blog.csdn.net/20180110192104602)







### Classical part[[edit](https://en.wikipedia.org/w/index.php?title=Shor%27s_algorithm&action=edit&section=2)]

- Pick a random number *a* < *N*.
- Compute [gcd](https://en.wikipedia.org/wiki/Greatest_common_divisor)(*a*, *N*). This may be done using
 the [Euclidean algorithm](https://en.wikipedia.org/wiki/Euclidean_algorithm).
- If gcd(*a*, *N*) ≠ 1, then this number is a [nontrivial](https://en.wikipedia.org/wiki/Nontrivial) factor
 of *N*, so we are done.
- Otherwise, use the period-finding subroutine (below) to find *r*, the [period](https://en.wikipedia.org/wiki/Periodic_function) of
 the following function:
f(x)=axmodN,{\displaystyle f(x)=a^{x}{\bmod {N}},}![f(x)=a^{x}{\bmod {N}},](https://wikimedia.org/api/rest_v1/media/math/render/svg/b9edd5a4873108bdc3e3d97dcf3270ccebe99994)
i.e. the [order](https://en.wikipedia.org/wiki/Order_(group_theory))r{\displaystyle
 r}![r](https://wikimedia.org/api/rest_v1/media/math/render/svg/0d1ecb613aa2984f0576f70f86650b7c2a132538) of a{\displaystyle
 a}![a](https://wikimedia.org/api/rest_v1/media/math/render/svg/ffd2487510aa438433a2579450ab2b3d557e5edc) in [(ZN)×{\displaystyle
 (\mathbb {Z} _{N})^{\times }}![(\mathbb {Z} _{N})^{\times }](https://wikimedia.org/api/rest_v1/media/math/render/svg/8e7664e43e8cc3725f83a4ab879d66a76143d908)](https://en.wikipedia.org/wiki/Multiplicative_group_of_integers_modulo_n),
 which is the smallest positive integer *r* for which f(x+r)=f(x){\displaystyle
 f(x+r)=f(x)}![f(x+r)=f(x)](https://wikimedia.org/api/rest_v1/media/math/render/svg/6510bc50bb8041372d942b2b9d0af2a4bb0d41cf),
 or f(x+r)=ax+rmodN≡axmodN.{\displaystyle f(x+r)=a^{x+r}{\bmod {N}}\equiv
 a^{x}{\bmod {N}}.}![{\displaystyle f(x+r)=a^{x+r}{\bmod {N}}\equiv a^{x}{\bmod {N}}.}](https://wikimedia.org/api/rest_v1/media/math/render/svg/90d7b4ace54a47ce8f04680d8ce41a1aa060b8ed)
- If *r* is odd, go back to step 1.
- If *a**r* /2≡{\displaystyle
 \equiv }![\equiv](https://wikimedia.org/api/rest_v1/media/math/render/svg/4c5c34250859b6f6d2a77b4e8a2ceaa90638076d) −1
 ([mod](https://en.wikipedia.org/wiki/Modular_arithmetic)*N*), go back to step 1.
- gcd(*a**r*/2 + 1, *N*) and gcd(*a**r*/2 - 1, *N*) are
 both nontrivial factors of *N*. We are done.





For example: N=15,a=7,r=4{\displaystyle N=15,a=7,r=4}![N=15,a=7,r=4](https://wikimedia.org/api/rest_v1/media/math/render/svg/983e79b01dfcddae92cd0444f680680d6ebaa5fa), gcd(72±1,15)=gcd(49±1,15){\displaystyle
 \mathrm {gcd} (7^{2}\pm 1,15)=\mathrm {gcd} (49\pm 1,15)}![\mathrm {gcd} (7^{2}\pm 1,15)=\mathrm {gcd} (49\pm 1,15)](https://wikimedia.org/api/rest_v1/media/math/render/svg/c4d5c8ba3e912dc911738914980d9180dbb33434),
 where gcd(48,15)=3{\displaystyle \mathrm {gcd} (48,15)=3}![\mathrm {gcd} (48,15)=3](https://wikimedia.org/api/rest_v1/media/math/render/svg/a65d3951f8268855008024fc9cf5f8ef349c4fa0),
 and gcd(50,15)=5{\displaystyle \mathrm {gcd} (50,15)=5}![\mathrm {gcd} (50,15)=5](https://wikimedia.org/api/rest_v1/media/math/render/svg/aff44f617d0c6a5e02813a367e3d1790fe2aed46).


![](https://img-blog.csdn.net/20180110193206281)


![](https://img-blog.csdn.net/20180110193217638)


![](https://img-blog.csdn.net/20180110201004267)


![](https://img-blog.csdn.net/20180110201011603)


![](https://img-blog.csdn.net/20180110201019238)


![](https://img-blog.csdn.net/20180110201025568)


![](https://img-blog.csdn.net/20180110201032412)


![](https://img-blog.csdn.net/20180110201038788)


![](https://img-blog.csdn.net/20180110201045762)





参考文献：

[1] Shor P W. Polynomial-Time Algorithms for Prime Factorization and Discrete Logarithms on a Quantum Computer[M]. Society for Industrial
 and Applied Mathematics, 1997.





