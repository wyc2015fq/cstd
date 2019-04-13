
# 航空客户价值分析特色LRFMC模型——RFM升级 - 素质云笔记-Recorder... - CSDN博客

2016年05月04日 12:45:07[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：7934个人分类：[R︱精准营销																](https://blog.csdn.net/sinat_26917383/article/category/6093521)


本文转载自微信公众号TIpDM。


**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————

我们说RFM模型由R(最近消费时间间隔)、F(消费频次)和M(消费总额)三个指标构成，通过该模型识别出高价值客户。但该模型并不完全适合所有行业，如航空行业，直接使用M指标并不能反映客户的真实价值，因为“长途低等舱”可能没有“短途高等舱”价值高。所以得根据实际行业灵活调整RFM模型的指标，本文就拿航空公司的数据为例，将RFM模型构建成L(入会至当前时间的间隔，反映可能的活跃时长)、R(最近消费时间距当前的间隔，反映当前的活跃状态)、F(乘机次数，反映客户的忠诚度)、M(飞行里程数，反映客户对乘机的依赖性)和C(舱位等级对应的折扣系数，侧面反映客户价值高低)5个指标。下面就利用这5个指标进行客户价值分群的实战：

\#\#\#\#\#\#\#\#\#设置工作空间\#\#\#\#
setwd("D:/spss modeler/数据")
\#\#\#\#\#\#\#数据的读取
datafile=read.csv('./air_data.csv',header=T)

![](https://img-blog.csdn.net/20170818172813665)
该数据集包含了62988条会员记录，涉及会员号、入会时间、首次登机时间、性别等44个字段。发现这么多字段中，正真能使用到的字段只有FFP_DATE(入会时间)、LOAD_TIME(观测窗口结束时间，可理解为当前时间)、FLIGHT_COUNT(乘机次数)、SUM_YR_1(票价收入1)、SUM_YR_2(票价收入2)、SEG_KM_SUM(飞行里程数)、LAST_FLIGHT_DATE(最后一次乘机时间)和avg_discount(舱位等级对应的平均折扣系数)。下面来看一下这些数据的分布情况：

\#\#确定探索分析变量
col=c(15:18,20:29)\#去掉日期型变量
\#\#\#输出变量最值，缺失情况
summary(datafile[,col])
![](https://img-blog.csdn.net/20170818172826694)
发现数据中存在异常，如票价收入为空或0、舱位等级对应的平均折扣系数为0。这样的异常可能是由于客户没有实际登机造成，故考虑将这样的数据剔除。具体操作如下：

\#\#\#数据清洗代码\#\#\#
\#\#\#\#丢弃票价为空的记录
delet_na=datafile[-which(is.na(datafile$SUM_YR_1)|is.na(datafile$SUM_YR_2)),]
\#\#\#\#丢弃票价为0，平均折扣率不为0，总飞行公里数大于0的记录
index=((delet_na$SUM_YR_1==0&delet_na$SUM_YR_2==0)
*(delet_na$avg_discount!=0)
*(delet_na$SEG_KM_SUM>0))
deletdata=delet_na[-which(index==1),]
\#\#\#\#保存清洗后的数据
cleanedfile=deletdata
str(cleanedfile)
![](https://img-blog.csdn.net/20170818172833103)
属性的规约
原始数据中的属性太多，根据航空公司价值LRFMC模型，选择与LRFMC指标相关的6个属性：FFP_DATE(入会时间)、LOAD_TIME(观测窗口结束时间，可理解为当前时间)、FLIGHT_COUNT(乘机次数)、SUM_YR_1(票价收入1)、SUM_YR_2(票价收入2)、SEG_KM_SUM(飞行里程数)、LAST_FLIGHT_DATE(最后一次乘机时间)和avg_discount(舱位等级对应的平均折扣系数)。下面来看一下这些数据的分布情况：

LRFMC<-c('FFP_DATE','LOAD_TIME','FLIGHT_COUNT','SUM_YR_1','SUM_YR_2','SEG_KM_SUM','LAST_FLIGHT_DATE','avg_discount')
cleanedfile_1<-cleanedfile[,LRFMC]
summary(cleanedfile_1)
![](https://img-blog.csdn.net/20170818172837097)
str(cleanedfile_1)
![](https://img-blog.csdn.net/20170818172840115)
从上面可以看出FFP_DATE，LOAD_TIME，LAST_FLIGHT_DATE，并不是数值型数据而是发现三个关于时间的字段均为因子型数据，需要将其转换为日期格式，用于下面计算时间差：

\#\#\#\#时间数据的转化
\#\#\#\#时间数据的转化
cleanedfile_1$FFP_DATE <-as.Date(cleanedfile_1$FFP_DATE)
cleanedfile_1$LOAD_TIME <-as.Date(cleanedfile_1$LOAD_TIME)
cleanedfile_1$LAST_FLIGHT_DATE<-as.Date(cleanedfile_1$LAST_FLIGHT_DATE)

数据格式的转化后的数据如下：
\#L:入会至当前时间的间隔
\#R:最近登机时间距当前的间隔
cleanedfile_1<- transform(cleanedfile_1,L=difftime(LOAD_TIME,FFP_DATE, units = 'days')/30, R =difftime(LOAD_TIME,LAST_FLIGHT_DATE, units = 'days')/30)
str(cleanedfile_1)
![](https://img-blog.csdn.net/20170818172840115)
发现L和R这两个指标并不是数值型数据，而是difftime型，故需要将其转换为数值型：

\#\#数值型的转化
cleanedfile_1$L <- as.numeric(cleanedfile_1$L)
cleanedfile_1$R <- as.numeric(cleanedfile_1$R)
str(cleanedfile_1)
![](https://img-blog.csdn.net/20170818172844518)
发现缺失值，这里仍然将其剔除：
cleanedfile_1<-na.omit(cleanedfile_1)

目前5个指标值都有了，下面就需要根据每个客户的5个值对其进行分群，传统的方法是计算综合得分，然后排序一刀切，选出高价值、潜在价值和低价值客户。现在所使用的方法是k-means聚类算法，避免了人为的一刀切。由于k-means聚类算法是基于距离计算类与类之间的差别，然而这5个指标明显存在量纲上的差异，故需要标准化处理：

\#\#\#\#数据标准化
\#数据标准化处理
\#\#\#\#\#\#\#\#\#
cleanedfile_2=cleanedfile_1[,c('L','R','FLIGHT_COUNT','SEG_KM_SUM','avg_discount')]
\#构建标准化函数
standard <- function(x) {
(x-min(x))/(max(x)-min(x))
}
zsredfile<-sapply(X=cleanedfile_2,FUN=standard)
colnames(zsredfile)=c("ZL","ZR","ZF","ZM","ZC")
head(zsredfile)

数据标准化如下
![](https://img-blog.csdn.net/20170818172847652)
标准化数据之后，就可以使用k-means聚类算法将客户进行聚类，问题是该聚为几类呢？根据业务的理解与分析确定客户类别数量，即：分为五类，

\#k-means聚类，设置聚类个数为5
set.seed(1234)
result<- kmeans(x =zsredfile, centers = 5)
\#\#\#结果输出
type=result$cluster
table(type)\#\#\#查看类别的分布
centervec=result$center
![](https://img-blog.csdn.net/20170818172850476)

上图反馈了客户的聚类结果，但是从数据中很难快速的找出不同价值的客户，下面通过绘制雷达图来反映聚类结果：
\#\#\#展现分类图
library(fpc)
plotcluster(zsredfile, result$cluster)  \#展现分类图
![](https://img-blog.csdn.net/20170818172853150)
从上图可以看出五类人的基本分布情况，
\#绘制雷达图
install.packages('fmsb')
library(fmsb)
max <- apply(centervec, 2, max)
min <- apply(centervec,2,min)
df = data.frame(rbind(max,min,centervec))
radarchart(df = df, seg=5, plty=1,vlcex=0.7)
![](https://img-blog.csdn.net/20170818172856902)
从图中可知，黑色线是价值最高的，F和M值对应最高，C值次高，属于第2组人群;价值次高的是蓝色线人群，即第3组，该人群特征是C值最大；以此类推，海绿色线人群的价值最低，雷达图所围成的面积最小。还有一种办法能够最快的识别出价值由高到低的5类人群，即对5个人群各指标均值求和排序即可，因为数据都是标准化的，不受量纲影响，可直接求和排序：
order(apply(centervec,1,sum),decreasing = TRUE)

结果显示第2组人群最佳、其次是第3组人群，最差的是第5组人群。通过对比centers结果，能够很好的反映5组人群的价值高低：
|客户群
|排名
|排名含义
|
|客户群2
|1
|重要保持客户
|
|客户群3
|2
|重要发展客户
|
|客户群1
|3
|重要挽留客户
|
|客户群4
|4
|一般客户
|
|客户群5
|5
|低价值客户

|











**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


