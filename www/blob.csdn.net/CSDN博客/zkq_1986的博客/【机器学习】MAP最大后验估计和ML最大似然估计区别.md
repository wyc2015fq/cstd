# 【机器学习】MAP最大后验估计和ML最大似然估计区别 - zkq_1986的博客 - CSDN博客





2017年10月16日 21:50:38[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1956











MAP：maximun a posteriori 最大后验估计

ML：maximun likelihood.




# 1 MAP

A maximum a posteriori probability (MAP) estimate is
 an estimate of an unknown quantity, that equals the [mode](https://en.wikipedia.org/wiki/Mode_(statistics)) of
 the [posterior distribution](https://en.wikipedia.org/wiki/Posterior_distribution).
 The MAP can be used to obtain a [point estimate](https://en.wikipedia.org/wiki/Point_estimation) of
 an unobserved quantity on the basis of empirical data. It is closely related to the method of [maximum
 likelihood](https://en.wikipedia.org/wiki/Maximum_likelihood) (ML) estimation, but employs an augmented [optimization
 objective](https://en.wikipedia.org/wiki/Optimization_(mathematics)) which incorporates a [prior
 distribution](https://en.wikipedia.org/wiki/Prior_distribution) (that quantifies the additional information available through prior knowledge of a related event) over the quantity one wants to estimate. MAP estimation can therefore be seen as a [regularization](https://en.wikipedia.org/wiki/Regularization_(mathematics)) of
 ML estimation.


![{\displaystyle {\hat {\theta }}_{\mathrm {MAP} }(x)={\underset {\theta }{\operatorname {arg\,max} }}\ f(\theta \mid x)={\underset {\theta }{\operatorname {arg\,max} }}\ {\frac {f(x\mid \theta )\,g(\theta )}{\displaystyle \int _{\vartheta }f(x\mid \vartheta )\,g(\vartheta )\,d\vartheta }}={\underset {\theta }{\operatorname {arg\,max} }}\ f(x\mid \theta )\,g(\theta ).\!}](https://wikimedia.org/api/rest_v1/media/math/render/svg/9133b8a614e21bb48a2afb5f02f6b410bb7b9649)


# 2 ML




![](http://images2015.cnblogs.com/blog/380470/201512/380470-20151218185339396-502244560.png)


![](http://images2015.cnblogs.com/blog/380470/201512/380470-20151218185601193-496236570.png)





# 3 区别

![](https://img-blog.csdn.net/20171016215726377)


正则化等价于对模型参数引入先验分布，而L1正则化相当于参数中引入拉普拉斯分布先验，而L2分布则相当于引入了高斯分布先验。













