# R语言操作(UDA)-- Part2: 探索单个变量 - weixin_33985507的博客 - CSDN博客
2017年01月23日 16:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
### 探索单一变量：
2.1 分析单一变量
To understand
- the types of values they take on,
- what the distributions look like,
- whether there are missing values or outliers，
通常用histograms, box plots, frequency polygons这些基础的但是很有用的TOOLS来分析单变量，经常还需要调整x轴的尺度和 bin width。
还经常做数据变形transforming to uncover hidden patterns of our data.
2.2 MARKDOWN Tutorial：
![3382609-1c891443957999df.png](https://upload-images.jianshu.io/upload_images/3382609-1c891443957999df.png)

![3382609-f97d955e354e5e61.png](https://upload-images.jianshu.io/upload_images/3382609-f97d955e354e5e61.png)

![3382609-7129f290434b5e8e.png](https://upload-images.jianshu.io/upload_images/3382609-7129f290434b5e8e.png)

![3382609-f83169e2b2524003.png](https://upload-images.jianshu.io/upload_images/3382609-f83169e2b2524003.png)

![3382609-0dc5234ab8a6b87a.png](https://upload-images.jianshu.io/upload_images/3382609-0dc5234ab8a6b87a.png)

![3382609-76c33bb8e351ca30.png](https://upload-images.jianshu.io/upload_images/3382609-76c33bb8e351ca30.png)

![3382609-1a43d20804c1a71f.png](https://upload-images.jianshu.io/upload_images/3382609-1a43d20804c1a71f.png)
2.3 伪Facebook用户数据的处理
- 查看文件夹中所有的文件：list.files()
- 设置目录路径: setwd('目录地址')
- 查看当前路径: getwd()
- 读入分隔符为制表符的文件: pf <- read.csv('xx.tsv', sep = '\t')
- 
查看数据集中的变量名：names(pf)   这里pf是数据集
![3382609-5189b5dce4403406.png](https://upload-images.jianshu.io/upload_images/3382609-5189b5dce4403406.png)
2.4 分面facet
> 
qplot(x = dob_day, data = pf) +
scale_x_continuous(breaks = 1:31) +
facet_wrap(~dob_month, ncol = 3)
![]([http://upload-images.jianshu.io/upload_images/3382609-e51a6178fd97ea4f.png](https://link.jianshu.com?t=http://upload-images.jianshu.io/upload_images/3382609-e51a6178fd97ea4f.png)?
imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![3382609-3e8af2af82369e91.png](https://upload-images.jianshu.io/upload_images/3382609-3e8af2af82369e91.png)
2.5 限制轴 Limiting the Axes
> 
qplot(x = friend_count, data = pf, xlim = c(0,1000))
> 
qplot(x = friend_count, data = pf) +
scale_x_continous(limits = c(0,1000))
2.6 调整组宽 Bin Width
> 
qplot(x = friend_count, data = pf, binwidth = 25) +
scale_x_continuous(limits = c(0, 1000), breaks = seq(0, 1000, 50))
![3382609-5e6c7c6c4e9fc27e.png](https://upload-images.jianshu.io/upload_images/3382609-5e6c7c6c4e9fc27e.png)
2.7 在上面已经调整Bin Width的情况下继续调整，观察不同性别的分组会有怎样的差异：
> 
qplot(x = friend_count, data = pf, binwidth = 25) +
scale_x_continuous(limits = c(0, 1000), breaks = seq(0,1000, 50)) +
facet_wrap(~gender)
![3382609-702dfb35c30cef48.png](https://upload-images.jianshu.io/upload_images/3382609-702dfb35c30cef48.png)
可见，上图中有男、女、NA，在R中NA代表missing value.
2.8 生成子数据集(subset)，排除掉NA:
> 
qplot(x = friend_count, data = subset(pf, !is.na(gender)),
binwidth = 10) +
scale_x_continuous(limits = c(0, 1000), breaks = seq(0, 1000, 50)) +
facet_wrap(~gender)
![3382609-17898a581fe265f5.png](https://upload-images.jianshu.io/upload_images/3382609-17898a581fe265f5.png)
2.9 除了subset取子集的方法，还有一种方法：
> 
qplot(x = friend_count, data = na.omit(pf), binwidth = 10) +
scale_x_continuous(lim = c(0, 1000), breaks = seq(0, 1000, 50)) +
facet_wrap(~gender)
![3382609-ae0c26c9b33b3cf5.png](https://upload-images.jianshu.io/upload_images/3382609-ae0c26c9b33b3cf5.png)
但是这种方法不仅是排除了gender 我NA的情况，也去掉了其他missing values.
2.10 按性别“划分”的统计学, “by” 命令，by命令的第1个参数是变量，第2个是分类变量，第3个是函数function.
> 
table(pf$gender)
by(pf$friend_count, pf$gender, summary)
可以看出来男性朋友多还是女性朋友多？我们通过中位数来看
![3382609-959685a07f51ebb4.png](https://upload-images.jianshu.io/upload_images/3382609-959685a07f51ebb4.png)
2.11 查看使用时长Tenure, 使用了颜色填充，在这里组宽是30，代表30天，一个月
> 
qplot(x = tenure, data = pf, binwidth = 30,
color = I('black'), fill = I('#099DD9'))
![3382609-f16155a94504eb92.png](https://upload-images.jianshu.io/upload_images/3382609-f16155a94504eb92.png)
2.12 上面是按月来看的，接下来看看每年用户数：
> 
qplot(x = tenure/365, data = pf, binwidth = 1,
color = I('black'), fill = I('#F79420'))
![3382609-1bcb3cc7f0f161dc.png](https://upload-images.jianshu.io/upload_images/3382609-1bcb3cc7f0f161dc.png)
2.13 为了使上面的图形更好看，将bin width调整成0.25
> 
qplot(x = tenure/365, data = pf, binwidth = 0.25,
color = I('black'), fill = I('#F79420'))
![3382609-768960f282311a05.png](https://upload-images.jianshu.io/upload_images/3382609-768960f282311a05.png)
2.14 为了使横坐标x更直观，调整一下x坐标轴的参数
> 
qplot(x = tenure/365, data = pf, binwidth = 0.25,
color = I('black'), fill = I('#F79420')) +
scale_x_continuous(breaks = seq(1,7,1), limits = c(0, 7))
![3382609-d7e9ff8778f07981.png](https://upload-images.jianshu.io/upload_images/3382609-d7e9ff8778f07981.png)
2.15 标记图形，Labeling Plots
> 
qplot(x = tenure/365, data = pf,
xlab = 'Number of years using Facebook',
ylab = 'Number of years in sample',
color = I('black'), fill = I('#F79420')) +
scale_x_continuous(breaks = seq(1,7,1), limits = c(0, 7))
![3382609-dfa8c1cb9332c9f9.png](https://upload-images.jianshu.io/upload_images/3382609-dfa8c1cb9332c9f9.png)
2.16 用户年龄
# 用户年龄：
> 
qplot(x = age, data = pf)
![3382609-8e5272607c83923a.png](https://upload-images.jianshu.io/upload_images/3382609-8e5272607c83923a.png)
> 
qplot(x = age, data = pf, binwidth = 1,
fill = I('#5760AB')) +
scale_x_continuous(breaks = seq(0, 113, 5))
![3382609-c0f88edfbb2469ab.png](https://upload-images.jianshu.io/upload_images/3382609-c0f88edfbb2469ab.png)
2.17 转换数据Transforming Data, 常用的方法是取log值、平方根
> 
qplot(x = friend_count, data = pf)
summary(pf$friend_count)
summary(log10(pf$friend_count + 1))
summary(sqrt(pf$friend_count))
2.18 将原始值和变形之后的值做出直方图
library(gridExtra)
- 方法1：
> 
p1 <- qplot(x = friend_count, data = pf)
p2 <- qplot(x = log10(friend_count + 1), data = pf)
p3 <- qplot(x = sqrt(friend_count), data = pf)
> 
grid.arrange(p1, p2, p3, ncol = 1)
![3382609-4db6285bd972eaff.png](https://upload-images.jianshu.io/upload_images/3382609-4db6285bd972eaff.png)
- 方法2:
> 
p1 <- ggplot(aes(x = friend_count), data = pf) + geom_histogram()
p2 <- p1 + scale_x_log10()
p3 <- p1 + scale_x_sqrt()
> 
grid.arrange(p1, p2, p3, ncol = 1)
![3382609-20016dc944be63b8.png](https://upload-images.jianshu.io/upload_images/3382609-20016dc944be63b8.png)
2.19  添加定标层Add a Scaling Layer
> 
logScale <- qplot(x = log10(friend_count), data = pf)
> 
countScale <- ggplot(aes(x = friend_count), data = pf) +
geom_histogram() +
scale_x_log10()
> 
grid.arrange(logScale, countScale, ncol = 2)
![3382609-14c1c6fda3dc67e5.png](https://upload-images.jianshu.io/upload_images/3382609-14c1c6fda3dc67e5.png)
- 添加了之后，x轴的label变成friend_count了：
> 
qplot(x = friend_count, data = pf) +
scale_x_log10()
![3382609-386201cc5a0e863d.png](https://upload-images.jianshu.io/upload_images/3382609-386201cc5a0e863d.png)
2.20 频率多边形
original:
> 
qplot(x = friend_count, data = subset(pf, !is.na(gender)),
binwidth = 10) +
scale_x_continuous(lim = c(0, 1000), breaks = seq(0, 1000, 50)) +
facet_wrap(~gender)
![3382609-95a34226a86a4f65.png](https://upload-images.jianshu.io/upload_images/3382609-95a34226a86a4f65.png)
modified to frequent polygons:因为qplot默认如果是单变量的话就是直方图
> 
qplot(x = friend_count, data = subset(pf, !is.na(gender)),
binwidth = 10, geom = 'freqpoly', color = gender) +
scale_x_continuous(lim = c(0, 1000), breaks = seq(0, 1000, 50))
![3382609-78c766c60bdbc12d.png](https://upload-images.jianshu.io/upload_images/3382609-78c766c60bdbc12d.png)
可见，好处就是一幅图中可以画出多个变量。然而，男性朋友多还是女性朋友多，不能单凭count值来判断，对y轴的变量做修改：
> 
qplot(x = friend_count, y = ..count../sum(..count..),
data = subset(pf, !is.na(gender)),
xlab = 'Friend Count',
ylab = 'Proportion of Users with that friend count',
binwidth = 10, geom = 'freqpoly', color = gender) +
scale_x_continuous(lim = c(0, 1000), breaks = seq(0, 1000, 50))
![3382609-b011b79bd0aee368.png](https://upload-images.jianshu.io/upload_images/3382609-b011b79bd0aee368.png)
频率多边形---- www_likes变量：
> 
qplot(x = www_likes, data = subset(pf, ![is.na(gender)](https://link.jianshu.com?t=http://is.na(gender))),
geom = 'freqpoly', color = gender) +
scale_x_continuous()
![3382609-545b150e9bc3590f.png](https://upload-images.jianshu.io/upload_images/3382609-545b150e9bc3590f.png)
这幅图是长尾的，看不出什么来，下面尝试变形取log值观察图形是否改善：
> 
qplot(x = www_likes, data = subset(pf, ![is.na(gender)](https://link.jianshu.com?t=http://is.na(gender))),
geom = 'freqpoly', color = gender) +
scale_x_continuous() +
scale_x_log10()
![3382609-22a5a54831340f13.png](https://upload-images.jianshu.io/upload_images/3382609-22a5a54831340f13.png)
2.21 网页端上的“点赞”数：男性多还是女性多？可以用by函数来回答：
![3382609-2d93a7b5f28e5d91.png](https://upload-images.jianshu.io/upload_images/3382609-2d93a7b5f28e5d91.png)
2.22 Histograms 直方图
> 
qplot(x = friend_count, data = subset(pf, !is.na(gender)),
binwidth = 25) +
scale_x_continuous(limits = c(0, 1000), breaks = seq(0, 1000, 50)) +
facet_wrap(~gender)
![3382609-da14e248d5f30446.png](https://upload-images.jianshu.io/upload_images/3382609-da14e248d5f30446.png)
Box Plots 箱线图
> 
qplot(x = gender, y = friend_count,
data = subset(pf, !is.na(gender)),
geom = 'boxplot')
![3382609-3f7b7e42a464ac06.png](https://upload-images.jianshu.io/upload_images/3382609-3f7b7e42a464ac06.png)
可见，异常点非常的多，这里异常点的定义是超过了1.5倍的IQR
箱子中间的横线是median值
让我们调整一下y轴的值域，使得箱型图内部更明晰，有2种方法做到：
方法1：使用ylim来调整：
> 
qplot(x = gender, y = friend_count,
data = subset(pf, !is.na(gender)),
geom = 'boxplot', ylim = c(0, 1000))
![3382609-9f416a08710aa2ad.png](https://upload-images.jianshu.io/upload_images/3382609-9f416a08710aa2ad.png)
方法2： 使用scale_y_continuous调整:
> 
qplot(x = gender, y = friend_count,
data = subset(pf, !is.na(gender)),
geom = 'boxplot') +
scale_y_continuous(limits = c(0, 1000))
![3382609-762bd2f38e8ba138.png](https://upload-images.jianshu.io/upload_images/3382609-762bd2f38e8ba138.png)
2者得到的图是一样的
在这里，使用 coord_cartersian方法比前面2种方法更好：
> 
qplot(x = gender, y = friend_count,
data = subset(pf, !is.na(gender)),
geom = 'boxplot') +
coord_cartesian(ylim = c(0, 1000))
![3382609-9246495b33a382f3.png](https://upload-images.jianshu.io/upload_images/3382609-9246495b33a382f3.png)
我们的目的是想知道男性朋友多还是女性朋友多，图中可见女性中位数值要略高，把图zoom in 之后看个清楚：
> 
qplot(x = gender, y = friend_count,
data = subset(pf, !is.na(gender)),
geom = 'boxplot') +
coord_cartesian(ylim = c(0, 250))
![3382609-415e2b88836fa04d.png](https://upload-images.jianshu.io/upload_images/3382609-415e2b88836fa04d.png)
可见，75%的女性有的朋友少于大概250， 男性更低。再查看统计量：
> 
by(pf$friend_count, pf$gender, summary)
![3382609-6d052d36dbfab120.png](https://upload-images.jianshu.io/upload_images/3382609-6d052d36dbfab120.png)
得到的统计数据显示，75%的女性的朋友数少于242个，25%的女性朋友书超过了244个。从数据来看，女性的朋友数多于男性的。
接着，来看一下friendships_initiated 朋友发起数这个变量的情况：
> 
qplot(x = gender, y = friendships_initiated,
data = subset(pf, !is.na(gender)), geom = 'boxplot') +
coord_cartesian(ylim = c(0, 500))
![3382609-50c36a5e86fa6400.png](https://upload-images.jianshu.io/upload_images/3382609-50c36a5e86fa6400.png)
看起来，女性的中位数似乎略高于男性的，zoom in一下查看：
> 
qplot(x = gender, y = friendships_initiated,
data = subset(pf, !is.na(gender)), geom = 'boxplot') +
coord_cartesian(ylim = c(0, 150))
![3382609-b379eefa8ade55e5.png](https://upload-images.jianshu.io/upload_images/3382609-b379eefa8ade55e5.png)
这样看起来，女性的确比男性略高，再用by函数查看具体统计量：
> 
by(pf$friendships_initiated, pf$gender, summary)
![3382609-2068072d3dc8f626.png](https://upload-images.jianshu.io/upload_images/3382609-2068072d3dc8f626.png)
用统计量，就能知道结果，为什么我们还要给他们制作箱线图呢？
- 这是因为，从箱线图中我们可以看到分布，我们的分类变量中每个分段的中间50%值，还能感知到异常值，所以得到的信息量大于仅仅只看统计值。
**回顾（Review):**
- 本小节学习了仔细观察数据集中单个变量的重要性，了解所呈现数值的类型和其分布的形式，以及是否有缺失值或异常值。
- 我们的做法是某种程度上使用直方图、箱线图和频数多边形，这些都是可视化和了解单变量的最基本的、最重要的工具。
- 我们还对这些图形进行了调整，比如，更改一些轴上的极限、调整了直方图上的组距、用对数对变量进行了变换、或者将其变成二进制来发现数据后面隐藏的模式(uncover hidden patterns)。
author: 快乐自由拉菲犬Celine Zhang
