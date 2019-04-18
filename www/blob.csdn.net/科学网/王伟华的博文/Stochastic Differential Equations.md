# 科学网—Stochastic Differential Equations - 王伟华的博文




# Stochastic Differential Equations                           

已有 7587 次阅读2010-1-11 21:58|个人分类:[非线性科学论文集](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=11814&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|关键词:Stochastic,Differential,Equations|[Differential](misc.php?mod=tag&id=35205), [Equations](misc.php?mod=tag&id=37757), [Stochastic](misc.php?mod=tag&id=48142)



Non-stochastic differential equations are models of [dynamical systems](http://www.cscs.umich.edu/~crshalizi/notebooks/chaos.html) where the state evolves continuously in time. If they are autonomous, then the state's future values depend only on the present state; if they are non-autonomous, it is allowed to depend on an exogeneous "driving" term as well. (This may not be the standard way of putting it, but I think it's both correct and more illuminating than the more analytical viewpoints, and anyway is the line taken by V. I. Arnol'd in his book on differential equations.) Stochastic differential equations (SDEs) are, conceptually, ones where the the exogeneous driving term is a [stochatic process](http://www.cscs.umich.edu/~crshalizi/notebooks/stochastic-processes.html). --- While "differential equation", unmodified, covers both ordinary differential equations, containing only time derivatives, and partial differential equations, containing both time and space derivatives, "stochastic differential equation", unmodified, refers only to the ordinary case. Stochastic partial differential equations are just what you'd think. 

The solution of an SDE is, itself, a stochastic process. Heuristically, the easiest way to think of how this works is via Euler's method for solving differential equations, which is itself about the simplest possible numerical approximation scheme for an ODE. (This line of thought was apparently introduced by Bernstein in the 1920s.) To solve dx/dt = f(x), with initial condition x(0) = y, Euler's method instructs us to pick a small increment of time h, and then say that x(t+h) = x(t) + hf(x), using straight-line interpolation between the points 0, h, 2h, 3h,... Under suitable conditions on vector field f, as h shrinks, the function we obtain in this way actually converges on the correct solution. To accomodate a stochastic term on the right-hand side, say dx/dt = f(x) + E(t), where E(t) is random noise, we approximate x(t+h) - x(t) by hf(x) + E(t+h) - E(t). Then, once again, we let the time-increment shrink to zero. Doing this with full generality requires a theory of the integrals of stochastic processes, which is made especially difficult by the fact that many of the stochastic forces one would most naturally want to use, such as white noise, are ones which don't fit very naturally into differential equations! The necessary theory of stochastic integrals was developed in the 1940s by M. Loeve, K. Ito, and R. Stratonovich (all building on earlier work by, among others, [N. Wiener](http://www.cscs.umich.edu/~crshalizi/notebooks/wiener.html)); the theory of SDEs more strictly by Ito and Stratonovich, in slightly different forms. 

Most of what one encounters, in applications, as the theory of SDEs assumes that the driving noise is in fact white, i.e., Gaussian and uncorrelated over time. On the one hand, this is less of a restriction than it might seem, because many other natural sorts of noise process can be represented as stochastic integrals of white noise. On the other hand, the same mathematical structure can be used directly to define stochastic integrals and stochastic DEs driven by a far broader class of stochastic processes; on this topic Kallenberg is a very good introduction. 
Recommended, more introductory:
    - Geoffrey Grimmett and David Stirzaker, Probability and Random Processes [The last chapter of the 3rd edition has a good, if rather heuristic, first-glimpse look at SDEs. I can't recall if it appeared in earlier editions or not.] 
- Josef Honerkamp, Stochastic Dynamical Systems
- Joel Keizer, Statistical Thermodynamics of Nonequilibrium Processes [Describes SDEs, in the Ito framework, from a heuristic viewpoint, motivated by, precisely, the need to model non-equilibrium thermodynamic processes; goes on to use them in many interesting physical applications.] 
- Andrzej Lasota and Michael C. Mackey, Chaos, Fractals and Noise: Stochastic Aspects of Dynamics [The later chapters give a solid introduction to SDEs, starting from the Euler-Bernstein approach, but to my mind somewhat slighting the quite real advantages of the Ito calculus for more advanced problems] 
- [Robert S. Liptser](http://www.eng.tau.ac.il/~liptser/), [Lectures on Stochastic Processes](http://www.eng.tau.ac.il/~liptser/list.html) [See especially [lecture 10](http://www.eng.tau.ac.il/~liptser/lectures/lect_new10.pdf), on white noise, and [lecture 12](http://www.eng.tau.ac.il/~liptser/lectures/lect_new12.pdf), on Ito integrals.] 
- Bernt Oksendal, Stochastic Differential Equations [First-rate introductory textbook, loaded with examples and intuition-building.] 
Recommended, more advanced:
    - I. I. Gikhman and A. V. Skorokhod, Introduction to the Theory of Random Processes [Excellent long chapter on stochastic integrals and SDEs; these authors went on to publish several more books on SDEs, but I confess I have not read them.] 
- Olav Kallenberg, Foundations of Modern Probability [Builds up the theory of stochastic integrals and stochastic differential equations from scratch, ending with a very general framework which makes it clear just which parts of the original approach, tied to the Wiener process, were necessary and which were accidental. However, Kallenberg's book is intended as a comprehensive textbook on [probability theory](http://www.cscs.umich.edu/~crshalizi/notebooks/probability.html), from measure theory through [large deviations](http://www.cscs.umich.edu/~crshalizi/notebooks/large-deviations.html). This means that it is both mathematically demanding, and that he takes a "spiral" approach, revisitng this topic, like many others, repeatedly through the text. There are, however, abundant cross-references.] 
- Robert S. Liptser and Albert N. Shiryaev, Statistics of Random Processes [Vol. I gives a very detailed account of the classical, Wiener-process theory and its uses in [optimal filtering](http://www.cscs.umich.edu/~crshalizi/notebooks/filtering.html); vol. II considers numerous applications in statistics and signal-processing, as well as some generalizations and extensions.] 
- Michel Loeve, Probability Theory [Gives a very elegant account of Loeve's contributions to the theory of stochastic integrals] 
- L. C. G. Rogers and D. Williams, Diffusions, Markov Processes, and Martingales [See especially Vol. II, Ito Calculus.] 
Modesty forbids me to recommend:
    - CRS with A. Kontorovich, [Almost None of the Theory of Stochastic Processes](http://www.stat.cmu.edu/~cshalizi/almost-none) [A fairly large chunk of this manuscript is devoted to stochastic integrals and SDEs.] 
To read:
    - Lakhdar Aggoun and Robert Elliott, Measure Theory and Filtering: Introduction with Applications
- David Applebaum, Lévy Processes and Stochastic Calculus
- Yuri Bakhtin and Jonathan C. Mattingly, "Stationary Solutions of Stochastic Differential Equation with Memory and Stochastic Partial Differential Equations", [math.PR/0509166](http://arxiv.org/abs/math.PR/0509166)
- Viorel Barbu, Philippe Blanchard, Giuseppe Da Prato, Michael Röckner, "Self-organized criticality via stochastic partial differential equations", [arxiv:0811.2093](http://arxiv.org/abs/0811.2093)
- Nicolas Bouleau and Dominique Lépngle, Numerical Methods for Stochastic Process
- A. A. Budini and M.O. Caceres, "Functional characterization of generalized Langevin equations", [cond-mat/0402311](http://arxiv.org/abs/cond-mat/0402311) [Abstract: "We present an exact functional formalism to deal with linear Langevin equations with arbitrary memory kernels and driven by any noise structure characterized through its characteristic functional. No others hypothesis are assumed over the noise, neither the fluctuation dissipation theorem. We found that the characteristic functional of the linear process can be expressed in terms of noise's functional and the Green function of the deterministic (memory-like) dissipative dynamics. This object allow us to get a procedure to calculate all the Kolmogorov hierarchy of the non-Markov process. As examples we have characterized through the 1-time probability a noise-induced interplay between the dissipative dynamics and the structure of different noises. Conditions that lead to non-Gaussian statistics and distributions with long tails are analyzed. The introduction of arbitrary fluctuations in fractional Langevin equations have also been pointed out."] 
- Emmanuelle Clément, Arturo Kohatsu-Higa, Damien Lamberton, "A duality approach for the weak approximation of stochastic differential equations", [math.PR/0610178](http://arxiv.org/abs/math.PR/0610178) = Annals of Applied Probability**16** (2006): 1124--1154 ["a new methodology to prove weak approximation results for general stochastic differential equations. Instead of using a partial differential equation approach as is usually done for diffusions, the approach considered here uses the properties of the linear equation satisfied by the error process"] 
- Jacky Cresson and Sébastien Darses, "Stochastic embedding of dynamical systems", [math.PR/0509713](http://arxiv.org/abs/math.PR/0509713) [112pp. "Most physical systems are modelled by an ordinary or a partial differential equation, like the n-body problem in celestial mechanics. In some cases, for example when studying the long term behaviour of the solar system or for complex systems, there exist elements which can influence the dynamics of the system which are not well modelled or even known. One way to take these problems into account consists of looking at the dynamics of the system on a larger class of objects, that are eventually stochastic. In this paper, we develop a theory for the stochastic embedding of ordinary differential equations. We apply this method to Lagrangian systems. In this particular case, we extend many results of classical mechanics namely, the least action principle, the Euler-Lagrange equations, and Noether's theorem. We also obtain a Hamiltonian formulation for our stochastic Lagrangian systems. Many applications are discussed at the end of the paper."] 
- A. M. Davie, "Uniqueness of solutions of stochastic differential equations", [arxiv:0709.4147](http://arxiv.org/abs/0709.4147)
- Hartin Hairer, "Exponential Mixing Properties of Stochastic PDEs Through Asymptotic Coupling," [math.PR/0109115](http://arxiv.org/abs/math/0109115)
- David Hochberg, Carmen Molina-Paris, Juan Pérez-Mercader and Matt Visser, "Effective Action for Stochastic Partial Differential Equations," [cond-mat/9904215](http://arxiv.org/abs/cond-mat/9904215)
- Helge Holden, Stochastic Partial Differential Equations: A Modeling, White Noise Functional Approach
- Yoshifusa Ito and Izumi Kubo, "Calculus on Gaussian and Poisson White Noises", Nagoya Mathematical Journal**111** (1988): 41--84 [[Full text PDF](http://projecteuclid.org/DPubS/Repository/1.0/Disseminate?view=body&id=pdf_1&handle=euclid.nmj/1118781051)] 
- Gopinath Kallianpur and Jie Xiong, Stochastic Differential Equations in Infinite Dimensional Spaces
- Karatzas and Shreve, Brownian Motion and Stochastic Calculus
- Peter Kotelenez, Stochastic Ordinary and Stochastic Partial Differential Equations: Transition from Microscopic to Macroscopic Equations [[blurb](http://www.springer.com/math/probability/book/978-0-387-74316-5)] 
- Venkatarama Krishnan, Nonlinear Filtering and Smoothing: An Introduction to Martingales, Stochastic Integrals and Estimation
- H. Kunita, Stochastic Flows and Stochastic Differential Equations [[blurb](http://cambridge.org/0521599253)] 
- S. V. Lototsky and B. L. Rozovskii
    - "Wiener Chaos Solutions of Linear Stochastic Evolution Equations", [math.PR/0504558](http://arxiv.org/abs/math.PR/0504558)
- "Stochastic Differential Equations: A Wiener Chaos Approach", [math.PR/0504559](http://arxiv.org/abs/math.PR/0504559)

- Jonathan C. Mattingly, Andrew M. Stuart, M.V. Tretyakov, "Convergence of Numerical Time-Averaging and Stationary Measures via Poisson Equations", [arxiv:0908.4450](http://arxiv.org/abs/0908.4450)
- Anatolii V. Mokshin, Renat M. Yulmetyev, and Peter Hänggi, "Simple Measure of Memory for Dynamical Processes Described by a Generalized Langevin Equation", [Physical Review Letters**95** (2005): 200601](http://dx.doi.org/10.1103/PhysRevLett.95.200601)
- Esteban Moro and Henri Schurz, "Non-negativity preserving numerical algorithms for stochastic differential equations", [math.NA/0509724](http://arxiv.org/abs/math.NA/0509724)
- Cyril Odasso, "Exponential mixing for stochastic PDEs: the non-additive case", [Probability Theory and Related Fields**140** (2008): 41--82](http://dx.doi.org/10.1007/s00440-007-0057-2)
- Fabien Panloup, "Recursive computation of the invariant measure of a stochastic differential equation driven by a L'{e}vy process", [math.PR/0509712](http://arxiv.org/abs/math.PR/0509712)
- S. Peszat and J. Zabczyk, Stochastic Partial Differential Equations with Lévy Noise: An evolution Equation approach [[blurb](http://cambridge.org/9780521879897)] 
- Philip Protter, Stochastic Integration and Differential Equations
- A. J. Roberts, "Normal form transforms separate slow and fast modes in stochastic dynamical systems", [math.DS/0701623](http://arxiv.org/abs/math.DS/0701623)
- Ramon van Handel, "Almost Global Stochastic Stability", [math.PR/0411311](http://arxiv.org/abs/math.PR/0411311) ["We develop a method to prove almost global stability of stochastic differential equations in the sense that almost every intial point ... is asymptotically attracted to the origin with unit probability."] 
- Wei Wang and Jinqiao Duan, "Invariant manifold reduction and bifurcation for stochastic partial differential equations", [math.DS/0607050](http://arxiv.org/abs/math.DS/0607050)
- [http://www.cscs.umich.edu/~crshalizi/notebooks/stoch-diff-eqs.html](http://www.cscs.umich.edu/~crshalizi/notebooks/stoch-diff-eqs.html)


转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-286350.html](http://blog.sciencenet.cn/blog-81613-286350.html)

上一篇：[中英文电子书免费下载网站大全](blog-81613-284320.html)
下一篇：[潮湿计算机”工程下月启动 获得欧盟注资](blog-81613-287101.html)


