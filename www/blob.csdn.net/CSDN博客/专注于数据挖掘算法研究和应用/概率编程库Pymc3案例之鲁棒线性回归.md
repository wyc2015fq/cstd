# 概率编程库Pymc3案例之鲁棒线性回归 - 专注于数据挖掘算法研究和应用 - CSDN博客





2019年04月04日 20:09:53[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：24








参考：[https://twiecki.io/blog/2013/08/27/bayesian-glms-2/](https://twiecki.io/blog/2013/08/27/bayesian-glms-2/)

[https://twiecki.io/blog/2014/03/17/bayesian-glms-3/](https://twiecki.io/blog/2014/03/17/bayesian-glms-3/)

[https://twiecki.github.com/blog/2013/08/12/bayesian-glms-1/](https://twiecki.github.com/blog/2013/08/12/bayesian-glms-1/)

针对线性回归中异常点，利用t分布来替换正态分布构建贝叶斯模型。

```python
%matplotlib inline
import pymc3 as pm
import matplotlib.pyplot as plt
import numpy as np
import theano
size = 100
true_intercept = 1
true_slope = 2

x = np.linspace(0, 1, size)
# y = a + b*x
true_regression_line = true_intercept + true_slope * x
# add noise
y = true_regression_line + np.random.normal(scale=.5, size=size)
# Add outliers
x_out = np.append(x, [.1, .15, .2])
y_out = np.append(y, [8, 6, 9])
data = dict(x=x_out, y=y_out)
fig = plt.figure(figsize=(12, 12))
ax = fig.add_subplot(111, xlabel='x', ylabel='y', title='Generated data and underlying model')
ax.plot(x_out, y_out, 'x', label='sampled data')
ax.plot(x, true_regression_line, label='true regression line', lw=2.)
plt.legend(loc=0);
```

![](https://img-blog.csdnimg.cn/20190404200824305.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_16,color_FFFFFF,t_70)

```python
with pm.Model() as model_robust:
    family = pm.glm.families.StudentT()
    pm.GLM.from_formula('y ~ x', data, family=family)
    trace_robust = pm.sample(progressbar=False, tune=1000)

plt.figure(figsize=(12, 12))
plt.plot(x_out, y_out, 'x')
pm.plots.plot_posterior_predictive_glm(trace_robust,
                                       label='posterior predictive regression lines')
plt.plot(x, true_regression_line, 
         label='true regression line', lw=3., c='y')
plt.legend();
```

```
Auto-assigning NUTS sampler...
Initializing NUTS using jitter+adapt_diag...
Multiprocess sampling (4 chains in 4 jobs)
NUTS: [lam, x, Intercept]
```

![](https://img-blog.csdnimg.cn/20190404200917453.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_16,color_FFFFFF,t_70)

`PyMC3`'s `glm()` function allows you to pass in a `family` object that contains information about the likelihood.
- By changing the likelihood from a Normal distribution to a Student T distribution -- which has more mass in the tails -- we can perform *Robust Regression*.



