# dplyr包介绍 - littlely_ll的博客 - CSDN博客





2017年02月04日 21:27:12[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：310









1. arrange用法：

按照变量管理行。

arrange(.data, ...)

data为要处理的列；...为要按照某变量排列，默认为升序排列。
`arrange(mtcars, cyl,disp) ##按照变量cyl, disp排序`> arrange(mtcars, cyl,disp) ##按照变量cyl, disp排序
    mpg cyl  disp  hp drat    wt  qsec vs am gear carb
1  33.9   4  71.1  65 4.22 1.835 19.90  1  1    4    1
2  30.4   4  75.7  52 4.93 1.615 18.52  1  1    4    2
3  32.4   4  78.7  66 4.08 2.200 19.47  1  1    4    1
4  27.3   4  79.0  66 4.08 1.935 18.90  1  1    4    1
5  30.4   4  95.1 113 3.77 1.513 16.90  1  1    5    2
6  22.8   4 108.0  93 3.85 2.320 18.61  1  1    4    1
7  21.5   4 120.1  97 3.70 2.465 20.01  1  0    3    1
8  26.0   4 120.3  91 4.43 2.140 16.70  0  1    5    2
9  21.4   4 121.0 109 4.11 2.780 18.60  1  1    4    2
10 22.8   4 140.8  95 3.92 3.150 22.90  1  0    4    2
11 24.4   4 146.7  62 3.69 3.190 20.00  1  0    4    2
12 19.7   6 145.0 175 3.62 2.770 15.50  0  1    5    6
13 21.0   6 160.0 110 3.90 2.620 16.46  0  1    4    4
14 21.0   6 160.0 110 3.90 2.875 17.02  0  1    4    4
`arrange(mtcars,desc(disp)) ##按照变量disp降序排列`> arrange(mtcars, desc(disp)) ##按照disp变量降序排列

 mpg cyl  disp  hp drat    wt  qsec vs am gear carb

1  10.4   8 472.0 205 2.93 5.250 17.98  0  0    3    42 

10.4   8 460.0 215 3.00 5.424 17.82  0  0    3    43  14.7   8 440.0 230 3.23 5.345 17.42  0  0    3    44  19.2   8 400.0 175 3.08 3.845 17.05  0  0    3    25  18.7   8 360.0 175 3.15 3.440 17.02  0  0    3    26  14.3   8 360.0 245 3.21 3.570 15.84  0  0    3    47  15.8   8 351.0 264 4.22 3.170 14.50  0  1    5    48  13.3   8 350.0 245 3.73 3.840 15.41  0  0    3    49  15.5   8 318.0 150 2.76 3.520 16.87  0  0    3    210 15.2   8 304.0 150 3.15 3.435 17.30  0  0    3    2


 2. filter的用法：

filter(.data, ...)

.data为tbl类型的数据，所有的主要动词为S3类；...为传递的条件，多个条件之间用&连接。

```python
filter(mtcars, cyl == 8) ##提取mtcars数据中变量cyl为8的数据
```
> filter(mtcars, cyl == 8) ##提取mtcars数据中变量cyl为8的数据
    mpg cyl  disp  hp drat    wt  qsec vs am gear carb
1  18.7   8 360.0 175 3.15 3.440 17.02  0  0    3    2
2  14.3   8 360.0 245 3.21 3.570 15.84  0  0    3    4
3  16.4   8 275.8 180 3.07 4.070 17.40  0  0    3    3
4  17.3   8 275.8 180 3.07 3.730 17.60  0  0    3    3
5  15.2   8 275.8 180 3.07 3.780 18.00  0  0    3    3
6  10.4   8 472.0 205 2.93 5.250 17.98  0  0    3    4
7  10.4   8 460.0 215 3.00 5.424 17.82  0  0    3    4
8  14.7   8 440.0 230 3.23 5.345 17.42  0  0    3    4
9  15.5   8 318.0 150 2.76 3.520 16.87  0  0    3    2
10 15.2   8 304.0 150 3.15 3.435 17.30  0  0    3    2
11 13.3   8 350.0 245 3.73 3.840 15.41  0  0    3    4
12 19.2   8 400.0 175 3.08 3.845 17.05  0  0    3    2
13 15.8   8 351.0 264 4.22 3.170 14.50  0  1    5    4
14 15.0   8 301.0 335 3.54 3.570 14.60  0  1    5    8
```python
filter(mtcars, cyl<6 | vs == 1)
```

> filter(mtcars, cyl<6 | vs == 1)    mpg cyl  disp  hp drat    wt  qsec vs am gear carb1  22.8   4 108.0  93 3.85 2.320 18.61  1  1    4    12  21.4   6 258.0 110 3.08 3.215 19.44  1  0    3    13  18.1   6 225.0 105 2.76 3.460 20.22  1  0    3    14  24.4   4 146.7  62 3.69 3.190 20.00  1  0    4    25  22.8   4 140.8  95 3.92 3.150 22.90  1  0    4    26  19.2   6 167.6 123 3.92 3.440 18.30  1  0    4    47  17.8   6 167.6 123 3.92 3.440 18.90  1  0    4    48  32.4   4  78.7  66 4.08 2.200 19.47  1  1    4    19  30.4   4  75.7  52 4.93 1.615 18.52  1  1    4    210 33.9   4  71.1  65 4.22 1.835 19.90  1  1    4    111 21.5   4 120.1  97 3.70 2.465 20.01  1  0    3    112 27.3   4  79.0  66 4.08 1.935 18.90  1  1    4    113 26.0   4 120.3  91 4.43 2.140 16.70  0  1    5    214 30.4   4  95.1 113 3.77 1.513 16.90  1  1    5    215 21.4   4 121.0 109 4.11 2.780 18.60  1  1    4    2
```python
filter(mtcars, cyl<6 & vs == 1)
```
> filter(mtcars, cyl<6 & vs == 1)    mpg cyl  disp  hp drat    wt  qsec vs am gear carb1  22.8   4 108.0  93 3.85 2.320 18.61  1  1    4    12  24.4   4 146.7  62 3.69 3.190 20.00  1  0    4    23  22.8   4 140.8  95 3.92 3.150 22.90  1  0    4    24  32.4   4  78.7  66 4.08 2.200 19.47  1  1    4    15  30.4   4  75.7  52 4.93 1.615 18.52  1  1    4    26  33.9   4  71.1  65 4.22 1.835 19.90  1  1    4    17  21.5   4 120.1  97 3.70 2.465 20.01  1  0    3    18  27.3   4  79.0  66 4.08 1.935 18.90  1  1    4    19  30.4   4  95.1 113 3.77 1.513 16.90  1  1    5    210 21.4   4 121.0 109 4.11 2.780 18.60  1  1    4    2
 3. group_by的用法：

group_by(.data, ..., add = FALSE)

data为S3类型的tbl数据；... 为要分组的变量，也可为表达式； add为是否添加已存在的组，默认为覆盖。

```python
grouped <- group_by(mtcars, cyl)  ##mtcars按照cyl分组
grouped
```

> grouped <- group_by(mtcars, cyl)  ##mtcars按照cyl分组
> grouped
Source: local data frame [32 x 11]
Groups: cyl [3]

     mpg   cyl  disp    hp  drat    wt  qsec    vs    am
*          
1   21.0     6 160.0   110  3.90 2.620 16.46     0     1
2   21.0     6 160.0   110  3.90 2.875 17.02     0     1
3   22.8     4 108.0    93  3.85 2.320 18.61     1     1
4   21.4     6 258.0   110  3.08 3.215 19.44     1     0
5   18.7     8 360.0   175  3.15 3.440 17.02     0     0
6   18.1     6 225.0   105  2.76 3.460 20.22     1     0
7   14.3     8 360.0   245  3.21 3.570 15.84     0     0
8   24.4     4 146.7    62  3.69 3.190 20.00     1     0
9   22.8     4 140.8    95  3.92 3.150 22.90     1     0
10  19.2     6 167.6   123  3.92 3.440 18.30     1     0
# ... with 22 more rows, and 2 more variables:
#   gear , carb 


`summarise(grouped, mean(disp), mean(hp))  ##对分组变量分别求均值`# A tibble: 3 × 3    cyl `mean(disp)` `mean(hp)`                1     4     105.1364   82.636362     6     183.3143  122.285713     8     353.1000  209.21429
```python
filter(grouped, disp == max(disp))  ##分别找出各自组中disp的最大值
```

> filter(grouped, disp == max(disp))  ##分别找出各自组中disp的最大值Source: local data frame [3 x 11]Groups: cyl [3]    mpg   cyl  disp    hp  drat    wt  qsec    vs    am          1  21.4     6 258.0   110  3.08 3.215 19.44     1     02  24.4     4 146.7    62  3.69 3.190 20.00     1     03  10.4     8 472.0   205  2.93 5.250 17.98     0     0# ... with 2 more variables: gear , carb 
```python
by_vs_am <- group_by(mtcars, vs, am)  ##按照两个变量vs, am分组
```



> by_vs_am <- group_by(mtcars, vs, am)  ##按照两个变量vs, am分组> by_vs_am
Source: local data frame [32 x 11]
Groups: vs, am [4]

     mpg   cyl  disp    hp  drat    wt  qsec    vs    am
*          
1   21.0     6 160.0   110  3.90 2.620 16.46     0     1
2   21.0     6 160.0   110  3.90 2.875 17.02     0     1
3   22.8     4 108.0    93  3.85 2.320 18.61     1     1
4   21.4     6 258.0   110  3.08 3.215 19.44     1     0
5   18.7     8 360.0   175  3.15 3.440 17.02     0     0
6   18.1     6 225.0   105  2.76 3.460 20.22     1     0
7   14.3     8 360.0   245  3.21 3.570 15.84     0     0
8   24.4     4 146.7    62  3.69 3.190 20.00     1     0
9   22.8     4 140.8    95  3.92 3.150 22.90     1     0
10  19.2     6 167.6   123  3.92 3.440 18.30     1     0
# ... with 22 more rows, and 2 more variables:
#   gear , carb 
```python
by_va <- summarise(by_vs_am, n = n())   ## 对分组变量进行计数，其中n()只应用于summarise, ##mutate和filter函数
by_va
```

> by_va <- summarise(by_vs_am, n = n())   ## 对分组变量进行计数，其中n()只应用于summarise, ##mutate和filter函数> by_vaSource: local data frame [4 x 3]Groups: vs [?]     vs    am     n    1     0     0    122     0     1     63     1     0     74     1     1     7
```python
summarise(by_va, n = sum(n))  ##汇总计数，但是只对第一列
```

> summarise(by_va, n = sum(n))  ##汇总计数，但是只对第一列# A tibble: 2 × 2     vs     n   1     0    182     1    14
```python
group_by(mtcars, vsam = vs + am) ##分组变量为表达式，默认添加这一列
```


> group_by(mtcars, vsam = vs + am) ##分组变量为表达式，默认添加这一列
Source: local data frame [32 x 12]
Groups: vsam [3]

     mpg   cyl  disp    hp  drat    wt  qsec    vs    am
           
1   21.0     6 160.0   110  3.90 2.620 16.46     0     1
2   21.0     6 160.0   110  3.90 2.875 17.02     0     1
3   22.8     4 108.0    93  3.85 2.320 18.61     1     1
4   21.4     6 258.0   110  3.08 3.215 19.44     1     0
5   18.7     8 360.0   175  3.15 3.440 17.02     0     0
6   18.1     6 225.0   105  2.76 3.460 20.22     1     0
7   14.3     8 360.0   245  3.21 3.570 15.84     0     0
8   24.4     4 146.7    62  3.69 3.190 20.00     1     0
9   22.8     4 140.8    95  3.92 3.150 22.90     1     0
10  19.2     6 167.6   123  3.92 3.440 18.30     1     0
# ... with 22 more rows, and 3 more variables:
#   gear , carb , vsam 

 4. mutate和transmute的用法：

 mutate(.data, ... )

data为tbl类型数据；... 为融合的变量。

mutate和transmute的区别主要是mutate保持原有变量并新增加变量，而transmute只有新增加的变量。

```python
mutate(mtcars, displ_l = disp / 61.0237)
```
> mutate(mtcars, displ_l = disp / 61.0237)
    mpg cyl  disp  hp drat    wt  qsec vs am gear carb  displ_l
1  21.0   6 160.0 110 3.90 2.620 16.46  0  1    4    4 2.621932
2  21.0   6 160.0 110 3.90 2.875 17.02  0  1    4    4 2.621932
3  22.8   4 108.0  93 3.85 2.320 18.61  1  1    4    1 1.769804
4  21.4   6 258.0 110 3.08 3.215 19.44  1  0    3    1 4.227866
5  18.7   8 360.0 175 3.15 3.440 17.02  0  0    3    2 5.899347
6  18.1   6 225.0 105 2.76 3.460 20.22  1  0    3    1 3.687092
7  14.3   8 360.0 245 3.21 3.570 15.84  0  0    3    4 5.899347
8  24.4   4 146.7  62 3.69 3.190 20.00  1  0    4    2 2.403984
9  22.8   4 140.8  95 3.92 3.150 22.90  1  0    4    2 2.307300
10 19.2   6 167.6 123 3.92 3.440 18.30  1  0    4    4 2.746474......................................................
...................................................... 
```python
transmute(mtcars, displ_l = disp / 61.0237)
```

> transmute(mtcars, displ_l = disp / 61.0237)
    displ_l
1  2.621932
2  2.621932
3  1.769804
4  4.227866
5  5.899347
6  3.687092
7  5.899347
8  2.403984
9  2.307300
10 2.746474
...........
...........
`mutate(mtcars, cyl = NULL) ##去掉cyl一列`
> mutate(mtcars, cyl = NULL) ##去掉cyl一列    mpg  disp  hp drat    wt  qsec vs am gear carb1  21.0 160.0 110 3.90 2.620 16.46  0  1    4    42  21.0 160.0 110 3.90 2.875 17.02  0  1    4    43  22.8 108.0  93 3.85 2.320 18.61  1  1    4    14  21.4 258.0 110 3.08 3.215 19.44  1  0    3    15  18.7 360.0 175 3.15 3.440 17.02  0  0    3    26  18.1 225.0 105 2.76 3.460 20.22  1  0    3    17  14.3 360.0 245 3.21 3.570 15.84  0  0    3    48  24.4 146.7  62 3.69 3.190 20.00  1  0    4    29  22.8 140.8  95 3.92 3.150 22.90  1  0    4    210 19.2 167.6 123 3.92 3.440 18.30  1  0    4    4..................................................
..................................................

 5. nth，n_distinct的用法：

nth(x, n, order_by = NULL, default = default_missing(x))

x为一个向量；order_by为可选的决定顺序的变量。
> x <- 1:10> nth(x,4)
[1] 4

n_distinct(x, na_rm = FALSE)

x为数值向量。

n_distinct要比length(unique(x))更快更精确。
> x <- sample(1:10, 1e5, rep = T)
> length(unique(x))
[1] 10
> n_distinct(x)
[1] 10

6. sample_n的用法：

sample_n(tbl, size, replace = FALSE, weight = NULL, .env =parent.frame())

tbl为数据框；size为抽取行数，replace为是否可重复抽样； weight为权重，非负向量，自动转化权重和为1.
> by_cyl <- mtcars %>% group_by(cyl)> sample_n(mtcars, 10, weight = mpg)
                mpg cyl  disp  hp drat    wt  qsec vs am gear
Honda Civic    30.4   4  75.7  52 4.93 1.615 18.52  1  1    4
Fiat 128       32.4   4  78.7  66 4.08 2.200 19.47  1  1    4
Merc 280       19.2   6 167.6 123 3.92 3.440 18.30  1  0    4
Volvo 142E     21.4   4 121.0 109 4.11 2.780 18.60  1  1    4
Hornet 4 Drive 21.4   6 258.0 110 3.08 3.215 19.44  1  0    3
Toyota Corona  21.5   4 120.1  97 3.70 2.465 20.01  1  0    3
Merc 240D      24.4   4 146.7  62 3.69 3.190 20.00  1  0    4
Toyota Corolla 33.9   4  71.1  65 4.22 1.835 19.90  1  1    4
Fiat X1-9      27.3   4  79.0  66 4.08 1.935 18.90  1  1    4
Mazda RX4 Wag  21.0   6 160.0 110 3.90 2.875 17.02  0  1    4
> sample_n(by_cyl, 3) ##对于分组的数据，是对每个组进行抽样
Source: local data frame [9 x 11]
Groups: cyl [3]

    mpg   cyl  disp    hp  drat    wt  qsec    vs    am  gear
           
1  21.5     4 120.1    97  3.70 2.465 20.01     1     0     3
2  22.8     4 140.8    95  3.92 3.150 22.90     1     0     4
3  21.4     4 121.0   109  4.11 2.780 18.60     1     1     4
4  21.4     6 258.0   110  3.08 3.215 19.44     1     0     3
5  17.8     6 167.6   123  3.92 3.440 18.90     1     0     4
6  19.7     6 145.0   175  3.62 2.770 15.50     0     1     5
7  14.7     8 440.0   230  3.23 5.345 17.42     0     0     3
8  13.3     8 350.0   245  3.73 3.840 15.41     0     0     3
9  15.0     8 301.0   335  3.54 3.570 14.60     0     1     5
# ... with 1 more variables: carb 


7. do的用法：

do(.data, ...)

data为tbl类型数据， ... 为应用于每组的表达式。
> by_cyl <- group_by(mtcars, cyl)
> models <- by_cyl %>% do(mod = lm(mpg ~ disp, data = .))
> models
Source: local data frame [3 x 2]
Groups: 

# A tibble: 3 × 2
    cyl      mod
*    
1     4 
2     6 
3     8 
> summarise(models, rsq = summary(mod)$r.squared)
# A tibble: 3 × 1
         rsq
       
1 0.64840514
2 0.01062604
3 0.27015777
> models %>% do(data.frame(coef = coef(.$mod)))
Source: local data frame [6 x 1]
Groups: 

# A tibble: 6 × 1
          coef
*        
1 40.871955322
2 -0.135141815
3 19.081987419
4  0.003605119
5 22.032798914
6 -0.019634095
> models %>% do(data.frame(
+     var = names(coef(.$mod)),
+     coef(summary(.$mod)))
+ )
Source: local data frame [6 x 5]
Groups: 

# A tibble: 6 × 5
          var     Estimate  Std..Error    t.value     Pr...t..
*                                   
1 (Intercept) 40.871955322 3.589605400 11.3861973 1.202715e-06
2        disp -0.135141815 0.033171608 -4.0740206 2.782827e-03
3 (Intercept) 19.081987419 2.913992892  6.5483988 1.243968e-03
4        disp  0.003605119 0.015557115  0.2317344 8.259297e-01
5 (Intercept) 22.032798914 3.345241115  6.5863112 2.588765e-05
6        disp -0.019634095 0.009315926 -2.1075838 5.677488e-02
8. summarise的用法：

summarise(.data, ...)

data为S3类型的数据，包括tbl_df,tbl_dt和tbl_sql.
> summarise(mtcars, mean(disp))
  mean(disp)
1   230.7219
> mean(mtcars$disp) ##和summarise对比
[1] 230.7219
> summarise(group_by(mtcars,cyl), mean(disp)) ##对对变量按分组变量分别求均值
# A tibble: 3 × 2
    cyl `mean(disp)`
          
1     4     105.1364
2     6     183.3143
3     8     353.1000
> summarise(group_by(mtcars,cyl), m = mean(disp), sd = sd(disp))
# A tibble: 3 × 3
    cyl        m       sd
          
1     4 105.1364 26.87159
2     6 183.3143 41.56246
3     8 353.1000 67.77132
> by_cyl <- mtcars %>% group_by(cyl)
> by_cyl %>% summarise(a = n(), b = a + 1)
# A tibble: 3 × 3
    cyl     a     b
    
1     4    11    12
2     6     7     8
3     8    14    15
 9.select用法：
> iris<- tbl_df(iris)> select(iris, starts_with("Petal"))
# A tibble: 150 × 2
   Petal.Length Petal.Width
                 
1           1.4         0.2
2           1.4         0.2
3           1.3         0.2
4           1.5         0.2
5           1.4         0.2
6           1.7         0.4
7           1.4         0.3
8           1.5         0.2
9           1.4         0.2
10          1.5         0.1
# ... with 140 more rows
> select(iris, ends_with("Width"))
# A tibble: 150 × 2
   Sepal.Width Petal.Width
                
1          3.5         0.2
2          3.0         0.2
3          3.2         0.2
4          3.1         0.2
5          3.6         0.2
6          3.9         0.4
7          3.4         0.3
8          3.4         0.2
9          2.9         0.2
10         3.1         0.1
# ... with 140 more rows
> select(iris, contains("etal"))
# A tibble: 150 × 2
   Petal.Length Petal.Width
                 
1           1.4         0.2
2           1.4         0.2
3           1.3         0.2
4           1.5         0.2
5           1.4         0.2
6           1.7         0.4
7           1.4         0.3
8           1.5         0.2
9           1.4         0.2
10          1.5         0.1
# ... with 140 more rows
> select(iris, matches(".t.")) ##".t."为正则表达式
# A tibble: 150 × 4
   Sepal.Length Sepal.Width Petal.Length Petal.Width
                                
1           5.1         3.5          1.4         0.2
2           4.9         3.0          1.4         0.2
3           4.7         3.2          1.3         0.2
4           4.6         3.1          1.5         0.2
5           5.0         3.6          1.4         0.2
6           5.4         3.9          1.7         0.4
7           4.6         3.4          1.4         0.3
8           5.0         3.4          1.5         0.2
9           4.4         2.9          1.4         0.2
10          4.9         3.1          1.5         0.1
# ... with 140 more rows
> select(iris, Petal.Length, Petal.Width)
# A tibble: 150 × 2
   Petal.Length Petal.Width
                 
1           1.4         0.2
2           1.4         0.2
3           1.3         0.2
4           1.5         0.2
5           1.4         0.2
6           1.7         0.4
7           1.4         0.3
8           1.5         0.2
9           1.4         0.2
10          1.5         0.1
# ... with 140 more rows
> vars <- c("Petal.Length", "Petal.Width")
> select(iris, one_of(vars))
# A tibble: 150 × 2
   Petal.Length Petal.Width
                 
1           1.4         0.2
2           1.4         0.2
3           1.3         0.2
4           1.5         0.2
5           1.4         0.2
6           1.7         0.4
7           1.4         0.3
8           1.5         0.2
9           1.4         0.2
10          1.5         0.1
# ... with 140 more rows
> select_(iris, ~Petal.Length)
# A tibble: 150 × 1
   Petal.Length
          
1           1.4
2           1.4
3           1.3
4           1.5
5           1.4
6           1.7
7           1.4
8           1.5
9           1.4
10          1.5
# ... with 140 more rows
> select_(iris, "Petal.Length")
# A tibble: 150 × 1
   Petal.Length
          
1           1.4
2           1.4
3           1.3
4           1.5
5           1.4
6           1.7
7           1.4
8           1.5
9           1.4
10          1.5
# ... with 140 more rows
去掉一列时只需要在变量前加“ - ”即可。select只保留你提到的变量，而rename保留所有的变量。




