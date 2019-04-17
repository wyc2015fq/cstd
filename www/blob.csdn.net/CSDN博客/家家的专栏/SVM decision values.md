# SVM decision values - 家家的专栏 - CSDN博客





2014年05月15日 16:02:29[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1681








Some background:


The goal is to find the hyperplane which gives the maximum margin between the two classes:

![enter image description here](http://i.stack.imgur.com/QZovK.png)


So, the purpose is to maximize the margin, which is ![enter image description here](http://i.stack.imgur.com/NGNBM.png), therefore
 minimizing ![enter image description here](http://i.stack.imgur.com/FNGtI.png). Remember, usually when w is
 used to represent a hyperplane as the normal vector, ![](http://i.stack.imgur.com/FNGtI.png) is 1. That isn't the case here clearly,
 as there would be no optimization problem. Instead of keeping ![](http://i.stack.imgur.com/FNGtI.png) = 1 and varying the width
 of the margin, we've fixed the width of the margin to 2 and are allowing ![](http://i.stack.imgur.com/FNGtI.png) to vary in size
 instead.


This gives us the [primal optimization
 problem](http://en.wikipedia.org/wiki/Duality_%28optimization%29) (with soft margins):

![enter image description here](http://i.stack.imgur.com/5yblH.png)


This seems to be what you are referring to. However, this equation comes from basic soft maximum margin classifier, which is foundation of SVM. True SVM is formulated as a [Lagrangian
 dual](http://en.wikipedia.org/wiki/Duality_%28optimization%29) to allow the use of kernels. The neat thing about SVM is that when the above problem (and its constraints) are formulated in the Lagrangian, all the variables except for the lagrangian multipliers drop out, leaving us with the following problem:

![enter image description here](http://i.stack.imgur.com/f7v6g.png)


Notice there is no w. The training points x (y are
 the labels, 1 or -1), now only appear together as a dot product, allowing us to employ the [kernel
 trick](http://en.wikipedia.org/wiki/Kernel_trick) to obtain a non-linear model.


But if we don't have w what is our decision function? It becomes a function of our support
 vectors and the lagrangian multipliers we found.

![enter image description here](http://i.stack.imgur.com/hiYOK.gif)


This is what libsvm produces and what it stores as the model you have trained. It stores the support vectors and the associated alphas. For linear SVM, you can obtain the primal w, [this
 is explained here in the LibSVM FAQ](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#f804), but it is not going to be what you get back automatically from LibSVM, and this can only be done for the linear kernel.


The value of the SVM decision function based on the lagrangian multipliers and support vectors should only be interpreted by its sign as well.





转自：http://stackoverflow.com/questions/11030253/decision-values-in-libsvm



