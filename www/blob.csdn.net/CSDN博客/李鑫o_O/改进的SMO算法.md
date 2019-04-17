# 改进的SMO算法 - 李鑫o_O - CSDN博客





置顶2016年03月10日 21:15:14[hustlx](https://me.csdn.net/HUSTLX)阅读数：1140








S. S. Keerthi等人在Improvements to Platt’s SMO Algorithm for SVM Classifier Design一文中提出了对SMO算法的改进，纵观SMO算法，其核心是怎么选择每轮优化的两个拉格朗日乘子，标准的SMO算法是通过判断乘子是否违反原问题的KKT条件来选择待优化乘子的，由KKT条件：

![clip_image002[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101602725-1678169176.jpg)

是否违反它，与这几个因素相关：拉格朗日乘子![clip_image004[12]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101603569-1827662596.gif)、样本标记![clip_image006[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101604850-1851730747.gif)、偏置b 。 b的更新依赖于两个优化拉格朗日乘子，这就可能出现这种情况：拉格朗日乘子![clip_image004[13]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101605897-123836569.gif)已经能使目标函数达到最优，而SMO算法本身并不能确定当前由于两个优化拉格朗日乘子计算得到的b是否就是使目标函数达到最优的那个b，换句话说，对一些本来不违反KKT条件的点，由于上次迭代选择了不合适的，使得它们出现违反KKT条件的情况，导致后续出现一些耗时而无用的搜索，针对标准SMO的缺点，出现了以下改进方法。

对于SVM的最优化问题的解：

![clip_image008[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101607054-1008747285.jpg)

定义：![clip_image010[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101607819-1338986272.jpg)

![clip_image004[14]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101609725-1803244787.gif)是拉格朗日乘子，通过解下面对偶问题，我们可以得到![clip_image004[15]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101610319-237321099.gif)：

![clip_image013[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101610975-1600156849.jpg)

一旦![clip_image004[16]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101611569-1645410662.gif)确定，其他参数如：![clip_image015[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101612225-1487281961.gif)就很容易由KKT条件确定了，并且解是不唯一的，最后得拉格朗日函数如下：

![clip_image017[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101612991-1271377527.jpg)

定义：

![clip_image019[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101613710-1693904247.jpg)

则对偶问题的KKT条件如下：

![clip_image021[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101615210-787851369.jpg)

这个条件可以简化成下面三种情况：

1.![clip_image023[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101615897-759275218.gif)：

![clip_image025[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101616710-801207234.jpg)

2.![clip_image027[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101618382-1347263452.gif)

![clip_image029[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101619147-1964183643.jpg)

3．![clip_image031[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101619772-150650864.gif)

![clip_image033[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101620600-1877874935.jpg)

定义如下数集：*I*0 = {*i*: 0 *< αi < C*}; *I*1 ={*i*: *yi *= 1*,αi *= 0}; *I*2 = {*i*: *yi *=
 −1*,αi *= *C*}; *I*3 = {*i*: *yi *= 1*,αi *= *C*};*I*4 = {*i*:*yi *= −1*,αi *= 0}.

可以看到以上的KKT条件成立当且仅当有一个![clip_image035[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101621210-752178956.gif)使得下式成立：

![clip_image037[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101622804-1777519701.jpg)

定义：

![clip_image039[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101623835-783489304.jpg)

当且仅当*b*low ≤ *b*up*.*成立时KKT条件成立。更进一步KKT条件可以写成如下形式：

![clip_image041[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101624616-687514086.jpg)

![clip_image043[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101626272-474609534.gif)是一个正的容忍因子。




