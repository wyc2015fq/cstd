# Absorbing markov chain - 家家的专栏 - CSDN博客





2013年11月13日 11:01:53[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1403
个人分类：[乐在科研中](https://blog.csdn.net/yihaizhiyan/article/category/742190)









In the mathematical theory of probability, an **absorbing Markov chain** is a Markov chain, in which every state can reach an absorbing state. An absorbing state is a state that, once entered, can not be left.




**Definition:**

   A Markov chain is an absorbing chain if,

  1) there is at least one absorbing state and

   2) it is possible to go from any state to at least one absorbing state in a finite number of steps.

   In an absorbing Markov chain, a state that is not absorbing is called transient.






### Canonical form




Let an absorbing Markov chain with transition matrix *P* have *t* transient states and *r* absorbing states. Then
![P =\left(\begin{array}{cc} Q & R\\ \mathbf{0} & I_r\end{array}\right),](http://upload.wikimedia.org/math/5/6/c/56cb3427b9bb584ca93ff81b6d6f1d94.png)

where *Q* is a *t*-by-*t* matrix, *R* is a nonzero *t*-by-*r* matrix, **0** is an *r*-by-*t* zero matrix, and *I**r* is the *r*-by-*r* identity
 matrix. Thus, *Q* describes the probability of transitioning from some transient state to another while *R* describes the probability of transitioning from some transient state to some absorbing state.





From:[http://en.wikipedia.org/wiki/Absorbing_Markov_chain](http://en.wikipedia.org/wiki/Absorbing_Markov_chain)



