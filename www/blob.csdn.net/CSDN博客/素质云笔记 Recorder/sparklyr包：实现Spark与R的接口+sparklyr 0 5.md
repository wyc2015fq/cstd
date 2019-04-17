# sparklyr包：实现Spark与R的接口+sparklyr 0.5 - 素质云笔记/Recorder... - CSDN博客





2016年10月07日 13:24:00[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5318








**本文转载于雪晴数据网**

**相关内容：**

[sparklyr包：实现Spark与R的接口，会用dplyr就能玩Spark](http://blog.csdn.net/sinat_26917383/article/details/52749138)

[Sparklyr与Docker的推荐系统实战](http://blog.csdn.net/sinat_26917383/article/details/52749159)




[R语言︱H2o深度学习的一些R语言实践——H2o包](http://blog.csdn.net/sinat_26917383/article/details/51219025)


[R用户的福音︱TensorFlow：TensorFlow的R接口](http://blog.csdn.net/sinat_26917383/article/details/52737913)
[mxnet：结合R与GPU加速深度学习](http://blog.csdn.net/sinat_26917383/article/details/52737901)

# 碎片︱R语言与深度学习

[————————————————————————————————————————](http://blog.csdn.net/sinat_26917383/article/details/52737901)





![RStudio又搞出了个大杀器！sparklyr包：实现Spark与R的接口，会用dplyr就能玩Spark](http://www.xueqing.tv/uploads/Article/face/thumbs/233_1467263104.png)

日前，Rstudio公司发布了`sparklyr`包。该包具有以下几个功能：
- 实现R与[Spark](http://spark.apache.org/)的连接—`sparklyr`包提供了一个完整的[dplyr](https://github.com/hadley/dplyr)后端
- 筛选并聚合Spark数据集，接着在R中实现分析与可视化
- 利用Spark的[MLlib](http://spark.apache.org/docs/latest/mllib-guide.html)机器学习库在R中实现分布式机器学习算法
- 可以创建一个[扩展](http://spark.rstudio.com/extensions.html)，用于调用Spark API。并为Spark的所有包集提供了一个接口
- 未来在RStudio IDE中集成支持Spark和sparklyr包

### 安装

通过`devtools`包实现`sparklyr`包的安装：

```
install.packages("devtools")
devtools::install_github("rstudio/sparklyr")
```

接着，我们需要在本地安装Spark：

```
library(sparklyr)
spark_install(version = "1.6.1")
```

如果用的是RStudio IDE，还需下载最新的[预览版](https://www.rstudio.com/products/rstudio/download/preview/)IDE。它包含有实现与Spark交互的若干增强功能(详情参考[RStudio IDE](http://spark.rstudio.com/index.html#rstudio-ide))。

### 连接Spark

安装好`sparklyr`包之后，我们连接本地的Spark，也可以连接远程的Spark集群。这里，我们使用[spark_connect](http://spark.rstudio.com/reference/sparklyr/latest/spark_connect.html)函数来连接本地的Spark：

```
library(sparklyr)
library(dplyr)
sc <- spark_connect(master = "local")
```

返回的Spark连接(`sc`)为Spark集群提供了一个远程的dplyr数据源。更多连接远程Spark集群的信息参考[这里](http://spark.rstudio.com/deployment.html)

### 读取数据

使用[copy_to](http://spark.rstudio.com/reference/sparklyr/latest/copy_to.html)函数可以实现将R中的数据框导入到Spark。下面我将R自带的iris数据集，nycflights13包的flights数据集，以及Lahman包的Batting数据集复制到Spark（请确保安装了这两个包）。

```
iris_tbl <- copy_to(sc, iris)
flights_tbl <- copy_to(sc, nycflights13::flights, "flights")
batting_tbl <- copy_to(sc, Lahman::Batting, "batting")
```

使用dplyr的`src_tbls`函数可以列出所有可用的表（包括预先加载在集群内的表）。

```
src_tbls(sc)

[1] "batting" "flights" "iris"
```

### 使用dplyr语法

我们利用dplyr语法来对集群内的所有表进行操作，下面是一个简单的数据筛选案例：

```
# 筛选出飞机晚点两分钟的航班信息
flights_tbl %>% filter(dep_delay == 2)
```

```
Source:   query [?? x 16]
Database: spark connection master=local app=sparklyr local=TRUE

    year month   day dep_time dep_delay arr_time arr_delay carrier tailnum flight origin  dest
   <int> <int> <int>    <int>     <dbl>    <int>     <dbl>   <chr>   <chr>  <int>  <chr> <chr>
1   2013     1     1      517         2      830        11      UA  N14228   1545    EWR   IAH
2   2013     1     1      542         2      923        33      AA  N619AA   1141    JFK   MIA
3   2013     1     1      702         2     1058        44      B6  N779JB    671    JFK   LAX
4   2013     1     1      715         2      911        21      UA  N841UA    544    EWR   ORD
5   2013     1     1      752         2     1025        -4      UA  N511UA    477    LGA   DEN
6   2013     1     1      917         2     1206        -5      B6  N568JB     41    JFK   MCO
7   2013     1     1      932         2     1219        -6      VX  N641VA    251    JFK   LAS
8   2013     1     1     1028         2     1350        11      UA  N76508   1004    LGA   IAH
9   2013     1     1     1042         2     1325        -1      B6  N529JB     31    JFK   MCO
10  2013     1     1     1231         2     1523        -6      UA  N402UA    428    EWR   FLL
..   ...   ...   ...      ...       ...      ...       ...     ...     ...    ...    ...   ...
Variables not shown: air_time <dbl>, distance <dbl>, hour <dbl>, minute <dbl>.
```

[dplyr导论](https://cran.rstudio.com/web/packages/dplyr/vignettes/introduction.html)提供了许多dplyr包中函数的使用案例。以下案例演示的是航班延误信息的数据可视化：

```
delay <- flights_tbl %>% 
  group_by(tailnum) %>%
  summarise(count = n(), dist = mean(distance), delay = mean(arr_delay)) %>%
  filter(count > 20, dist < 2000, !is.na(delay)) %>%
  collect

# 绘图
library(ggplot2)
ggplot(delay, aes(dist, delay)) +
  geom_point(aes(size = count), alpha = 1/2) +
  geom_smooth() +
  scale_size_area(max_size = 2)
```

### ![](http://www.xueqing.tv/uploads/attached/image/20160630/20160630150948_38035.png)

### 窗口函数

支持dplyr的[窗口函数](https://cran.r-project.org/web/packages/dplyr/vignettes/window-functions.html)。如下所示：

```
batting_tbl %>%
  select(playerID, yearID, teamID, G, AB:H) %>%
  arrange(playerID, yearID, teamID) %>%
  group_by(playerID) %>%
  filter(min_rank(desc(H)) <= 2 & H > 0)
```

```
Source:   query [?? x 7]
Database: spark connection master=local app=sparklyr local=TRUE
Groups: playerID

    playerID yearID teamID     G    AB     R     H
       <chr>  <int>  <chr> <int> <int> <int> <int>
1  anderal01   1941    PIT    70   223    32    48
2  anderal01   1942    PIT    54   166    24    45
3  balesco01   2008    WAS    15    15     1     3
4  balesco01   2009    WAS     7     8     0     1
5  bandoch01   1986    CLE    92   254    28    68
6  bandoch01   1984    CLE    75   220    38    64
7  bedelho01   1962    ML1    58   138    15    27
8  bedelho01   1968    PHI     9     7     0     1
9  biittla01   1977    CHN   138   493    74   147
10 biittla01   1975    MON   121   346    34   109
..       ...    ...    ...   ...   ...   ...   ...
```

更多dplyr在Spark中的用法参考[这里](http://spark.rstudio.com/dplyr.html)。

### 调用MLlib

利用sparklyr包中的[MLlib](http://spark.apache.org/docs/latest/mllib-guide.html)函数可以实现在Spark集群中调用机器学习算法。

这里，我们使用[ml_linear_regression](http://spark.rstudio.com/reference/sparklyr/latest/ml_linear_regression.html)函数来拟合一个线性回归模型。数据为内置的`mtcars`数据集，我们想看看能否通过汽车的重量(`wt`)和发动机的气缸数(`cyl`)来预测汽车的油耗(`mpg`)。我们假设`mpg`跟这两个变量之间的关系是线性的。

```
# 将mtcar数据集复制到spark
mtcars_tbl <- copy_to(sc, mtcars)

# 先对数据做变换，然后将数据集分割为训练集和测试集
partitions <- mtcars_tbl %>%
  filter(hp >= 100) %>%
  mutate(cyl8 = cyl == 8) %>%
  sdf_partition(training = 0.5, test = 0.5, seed = 1099)

# 对训练数据集做模型拟合
fit <- partitions$training %>%
  ml_linear_regression(response = "mpg", features = c("wt", "cyl"))
```

```
Call:
mpg ~ wt + cyl

Coefficients:
(Intercept)          wt         cyl 
  33.499452   -2.818463   -0.923187
```

对spark得到的线性回归模型，使用`summary()`函数可以查看模型的拟合效果以及每个预测指标的统计意义。
`summary(fit)`
```
Call:
mpg ~ wt + cyl

Residuals:
   Min     1Q Median     3Q    Max 
-1.752 -1.134 -0.499  1.296  2.282 

Coefficients:
            Estimate Std. Error t value  Pr(>|t|)    
(Intercept) 33.49945    3.62256  9.2475 0.0002485 ***
wt          -2.81846    0.96619 -2.9171 0.0331257 *  
cyl         -0.92319    0.54639 -1.6896 0.1518998    
---
Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1

R-Squared: 0.8274
Root Mean Squared Error: 1.422
```

Spark机器学习提供常用机器学习算法的实现和特征变换。更多信息请参考[这里](http://spark.rstudio.com/mllib.html)。

### RStudio IDE

RStudio的[最新预览版](https://www.rstudio.com/products/rstudio/download/preview/)集成支持Spark和sparklyr包。包含以下工具：
- 创建和管理Spark连接
- 浏览表格数据和Spark DataFrames的所有列
- 可以预览Spark DataFrames的前1000行

一旦成功安装完sparklyr包，我们可以在IDE中可以看到一个新的Spark窗口。该窗口包含一个New Connection对话框，用于连接本地或者远程的Spark。如下所示：

![](http://www.xueqing.tv/uploads/attached/image/20160630/20160630151003_59416.png)

![](http://www.xueqing.tv/uploads/attached/image/20160630/20160630151018_72313.png)

![](http://www.xueqing.tv/uploads/attached/image/20160630/20160630151032_10721.png)

`Sparklyr`包的[官方网站](http://spark.rstudio.com/index.html)提供了详尽的学习文档，感兴趣的读者可以自行了解：
- [dplyr包在Spark中的用法](http://spark.rstudio.com/dplyr.html)
- [MLlib](http://spark.rstudio.com/mllib.html)
- [创建sparklyr扩展](http://spark.rstudio.com/extensions.html)
- [部署和配置](http://spark.rstudio.com/deployment.html)
- [sparklyr包中的函数参考文档](http://spark.rstudio.com/reference/sparklyr/latest/index.html)

> 
本文由[雪晴数据网](http://www.xueqing.tv/)负责翻译整理，原文参考[sparklyr — R interface for Apache Spark](http://spark.rstudio.com/index.html)。转载本译文请注明链接[http://www.xueqing.tv/cms/article/232](http://www.xueqing.tv/cms/article/232)

















# 二、sparklyr 0.5







来自于R语言中文社区，文章《Sparklyr 0.5 已在CRAN正式发布》






Sparklyr 0.5 包已经在CRAN正式发布，新的版本在以下几个方面做了优化：

1）加强对dplyr支持，如do( )和n_distinct( )

2）增加新的函数，如sdf_quantile( )、ft_tokenizer( )、ft_regex_tokenizer( )

3）强化对R的兼容性，如缺失值na.action、dim( )、nrow( )、ncol( )

4）spark.rstudio.com 上部署新的案例等

其他的最新优化，可以到官网查看噢！

1. 强化对Dplyr支持

`sparklyr 0.5` 添加了n_distinct( )，在速度和准确度上与length(uqique(x))相当，同时支持do( )和group_by( )，程序如下：

```
library(sparklyr)sc <- spark_connect(master = "local")mtcars_tbl <- copy_to(sc, mtcars, overwrite = TRUE)by_cyl <- group_by(mtcars_tbl, cyl)fit_sparklyr <- by_cyl %>% 
   do(mod = ml_linear_regression(mpg ~ disp, data = .))# display resultsfit_sparklyr$mod
输出：

[[1]] Call: ml_linear_regression(mpg ~ disp, data = .) Coefficients: (Intercept)         disp 19.081987419  0.003605119 [[2]] Call: ml_linear_regression(mpg ~ disp, data = .) Coefficients: (Intercept)        disp 40.8719553  -0.1351418 [[3]] Call: ml_linear_regression(mpg ~ disp, data = .) Coefficients: (Intercept)        disp 22.03279891 -0.01963409
```



 在这里需要注意的是，当sparklyr提供对dplyr支持时，在使用sparklyr时，并不一定必须要使用dplyr。比如可以使用DBI 而不使用dplyr。

```
library(sparklyr)
library(DBI)sc <- spark_connect(master = "local")
sdf_copy_to(sc, iris)
dbGetQuery(sc, "SELECT * FROM iris LIMIT 4")
 Sepal_Length Sepal_Width Petal_Length Petal_Width Species 1          5.1         3.5          1.4         0.2  setosa 2          4.9         3.0          1.4         0.2  setosa 3          4.7         3.2          1.3         0.2  setosa 4          4.6         3.1          1.5         0.2  setosa 5          5.0         3.6          1.4         0.2  setosa
```



```

```

## 2. 新的函数

当使用ft_tokenizer( )和ft_regex_tokenizer( )通过空格或者正则模式划分字符串时，sdf_quantile( )函数计算近似分位数（存在一定误差）。下面以ft_tokenizer函数为例。

```
library(sparklyr)
library(janeaustenr)
library(dplyr)

sc %>%
  spark_dataframe() %>%
  na.omit() %>%
  ft_tokenizer(input.col = “text”, output.col = “tokens”) %>%
  head(4)
```



输出：

```
text                book     tokens
                  <chr>               <chr>     <list>
1 SENSE AND SENSIBILITY Sense & Sensibility <list [3]>
2                       Sense & Sensibility <list [1]>
3        by Jane Austen Sense & Sensibility <list [3]>
4                       Sense & Sensibility <list [1]>
```



3. 增强兼容性


```
library(sparklyr)
library(dplyr)
library(nycflights13)sc <- spark_connect(master = "local")flights_clean <- na.omit(copy_to(sc, flights))

ml_linear_regression(
  flights_tbl
  response = "dep_delay",
  features = c("arr_delay", "arr_time"))
* Dropped 9430 rows with 'na.omit' (336776 => 327346) Call: ml_linear_regression(flights_tbl, response = "dep_delay",features = c("arr_delay", "arr_time")) Coefficients: (Intercept)    arr_delay     arr_time 6.1001212994 0.8210307947 0.0005284729
```



除此之外，dim( )、nrow( )和ncol( )在sparklyr的数据框中也已经支持。

4. Livy连接使用

Livy是是Apache Spark的一个开源REST服务，现在可以在sparklyr 0.5中使用，在许多场景下，在Livy可用时，可以将RStudio和Apache Spark进行连接。

 在本地使用Livy时，可以使用livy_install( )安装Livy在本地环境，这与spark_install( )类似。Livy支持远程连接Apache Spark时，需要先使用livy_service_start( )。

```
livy_install()
livy_service_start()sc <- spark_connect(master = "http://localhost:8998",                    method = "livy")
copy_to(sc, iris)

spark_disconnect(sc)
livy_service_stop()
```



在使用RStudio时，远程连接样式如下：

![](http://note.youdao.com/yws/res/32379/063BCD5FA99F483DA10FA642E0240ECF)

## 5. 增强连接

`sparklyr 0.5 不再要求必须联网下载Apache Spark包`

```
> library(sparklyr)> sc <- spark_connect(master = "local",                      config = list("sparklyr.gateway.port" = "0"))Error in force(code) : 
  Failed while connecting to sparklyr to port (0) for sessionid (5305):   Gateway in port (0) did not respond.  Path: /spark-1.6.2-bin-hadoop2.6/bin/spark-submit
  Parameters: --class, sparklyr.Backend, 'sparklyr-1.6-2.10.jar', 0, 5305---- Output Log ----16/12/12 12:42:35 INFO sparklyr: Session (5305) starting---- Error Log ----
```

















