# 机器学习-lesson3(决策树和随机森林) - 苝花向暖丨楠枝向寒 - CSDN博客

2018年12月11日 17:24:11[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：40


# 决策树

**熵： 表示随机事件不确定性的度量**

比如我们想预测一个人今天是否会出去打篮球，我们现有一些天气信息，如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209192132666.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)

然后我们根据下面的熵值计算公式算出熵值
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209191941432.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209192902431.png)
**信息增益：表示特征X使得类Y的不确定性减少程度。**

说白了就是通过一个决策节点以后熵值的减少量，减少量越大越好，通过对比每一种分类节点，选择信息增益值最大的作为决策树的根节点。

如下

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209195145290.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209195429514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)

可以看出当 outlook 为overcast时 结果均为yes 所以 说当 outlook等于overcast时商值为0 。

下面我们计算outlook总体的熵值
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209195948186.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)
**同样的方式我们计算其他特征的信息增益，选择出最大的就可以了。**

对于一些连续的字段，如身高年龄，我们需要选择合适的值来进行划分。如 年龄是否大于15 ，工资是否大于5000，那么这个值为多少才是最合适的呢？ 同样我们需要在每两个值之间去划分，然后依次去遍历计算信息增益值。找出合适的位置即可。

**决策树剪枝策略**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209202919945.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120920294432.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)

# 随机森林

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209205655613.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)

## Bagging模型

**可对数据进行随机采样，再对特征随机采样来保证每个树的结果不同，最后再算平均值。**
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120921043758.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)

**Bagging模型的优点**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181209211522984.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)
**Bagging模型的缺点**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211171834900.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)

## Boosting模型

将随机森林分为多个阶段，对第一阶段预测错误的部分作为输入进行第二阶段的预测。

## Stacking模型

随机森林中的树是不同的分类器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211172146885.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==,size_16,color_FFFFFF,t_70)

