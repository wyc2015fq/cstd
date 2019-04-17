# 《强化学习》 DP动态规划 - 浅梦的博客 - CSDN博客





2018年05月30日 12:48:33[浅梦s](https://me.csdn.net/u012151283)阅读数：567








# 奖赏设计

## 累计奖赏和折扣累计奖赏

![这里写图片描述](https://img-blog.csdn.net/20180530121828611?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180530122315510?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180530122639581?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

数学上看，折扣奖赏机制可以将累计回报转化为递推的形式：


$\begin{aligned}G_t &= R_t +\gamma(R_{t+1}+\gamma R_{t+2}+...)\\&=R_t + \gamma G_{t+1}\end{aligned}$

## 折扣是一种固定效应模型

![这里写图片描述](https://img-blog.csdn.net/20180530123644154?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 奖赏设计：不要平移，奖励做什么而不是怎么做

![这里写图片描述](https://img-blog.csdn.net/20180530124313363?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 奖赏设计：缩放，塑形

![这里写图片描述](https://img-blog.csdn.net/20180530124809960?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 贝尔曼等式

## 状态值函数

![这里写图片描述](https://img-blog.csdn.net/20180530143551290?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 值函数的贝尔曼期望等式

![这里写图片描述](https://img-blog.csdn.net/20180530144103571?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 动作值函数

![这里写图片描述](https://img-blog.csdn.net/20180530144243439?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 两者关系

![这里写图片描述](https://img-blog.csdn.net/20180530144522137?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 动作值函数的贝尔曼期望等式

![这里写图片描述](https://img-blog.csdn.net/20180530144738655?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 衡量策略优劣

![这里写图片描述](https://img-blog.csdn.net/20180530145654578?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 贝尔曼最优等式

![这里写图片描述](https://img-blog.csdn.net/20180530150907542?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 广义策略迭代GPI

## 策略评估

![这里写图片描述](https://img-blog.csdn.net/20180530165402701?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180530165509216?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 策略改进

![这里写图片描述](https://img-blog.csdn.net/20180530215016788?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180530215126968?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180530215153546?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## GPI

![这里写图片描述](https://img-blog.csdn.net/20180530215432489?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180530215459383?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 策略迭代

![这里写图片描述](https://img-blog.csdn.net/20180530215559900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 值迭代

![这里写图片描述](https://img-blog.csdn.net/20180530215630739?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 对比

![这里写图片描述](https://img-blog.csdn.net/20180530215705909?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







