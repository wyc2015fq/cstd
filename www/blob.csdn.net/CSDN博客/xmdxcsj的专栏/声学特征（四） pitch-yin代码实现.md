# 声学特征（四） pitch-yin代码实现 - xmdxcsj的专栏 - CSDN博客





2016年05月17日 19:17:40[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1884








参考sphinxbase-0.8/src/libsphinxbase/fe目录下的yin.c

### yin_init

主要有三个参数，含义如下： 
**frame_size**: 一帧的数据个数，实际求相关函数的时候$\tau=frame\_size/2$，所以该值越大，计算量会越大。对应pitch最小频率为$f_{sample}/(frame\_size/2)$. 
**search_threshold**: 对应于step4，在获取最终的周期的时候，低于该值的第一个相关值对应的数据点即作为周期，如果所有相关值均大于该值，那么使用最小值对应的点作为周期。 
**search_range和smooth_window**: 对应于step6，在2*smooth_window+1对应的窗口内，找出所有帧的最小的相关值对应的数据点best，如果该最小值不是当前帧，那么在best-best*search_range和best+best*search_range数据点范围重新寻找周期值。
### yin_write
- cmn_diff 

计算一帧的相关函数值，对应于**step3**


$d'(\tau), \tau=1...frame\_size/2$- thresholded_search 

搜索低于**search_threshold**的第一个点（如果没有则使用全局最小值），作为最后的周期，对应于**step4**。

### yin_read

获得两个参数： 

out_period: 周期对应的第$\tau$个点，通过$f_{sample}/\tau$转化为f0频率，作为pitch的第一维 

out_bestdiff: $\tau$点对应的$d'(\tau)$值，通过$1-\tau$作为pitch的第二维 

对应于**step6**，具体流程： 

1. 在**smooth_window**窗口范围内找出最小的相关值。比如窗口大小为3，则在当前帧和前面2帧的最小值中找出最小值。 

2. 如果最小值就是当前值，那么直接返回当前帧的参数。 

3. 如果最小值不是当前值，那么在**search_range**范围再次查找。
比如前两帧的结果如下： 

best[0]=40,best_diff[0]=0.08 

best[1]=38,best_diff[0]=0.09 

当前帧的结果如下： 

best[2]=50,best_diff[0]=0.12 

假设search_range=0.2 

最小值为0.08，对应的best为40，那么需要在当前帧的40-40*0.2到40+40*0.2之间搜索，即32-48之间调用thresholded_search函数。






