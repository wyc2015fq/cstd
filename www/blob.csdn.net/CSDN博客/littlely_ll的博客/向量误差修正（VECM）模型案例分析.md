# 向量误差修正（VECM）模型案例分析 - littlely_ll的博客 - CSDN博客





置顶2017年02月04日 21:26:45[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：11605









# 向量误差修正模型案例分析

## 生成数据
set.seed(12345)
u1<-rnorm(500)
u2<-arima.sim(list(ar=0.6),n=500) #生成模拟的一阶自回归模型
u3<-arima.sim(list(ar=.4),n=500)
y1<-cumsum(u1) #生成随机游走序列 y1
y2<-0.4*y1+u2
y3<-0.8*y1+u3
#调用urca包中的ca.jo()对时间序列y1 y2 y3进行Jonhansen协整检验
#2.Jonhansen协整检验
library(urca)
data<-data.frame(y1=y1,y2=y2,y3=y3) #将变量组织为数据框
## ca.jo(x, type = c("eigen", "trace"), ecdet = c("none", "const", "trend"), K = 2,spec=c("longrun", "transitory"), season = NULL, dumvar = NULL) 注意这里只是用默认设置。 
model.vecm<-ca.jo(data)
head(model.vecm@x) #ca.jo使用S4方法，故用@提取变量             y1          y2         y3
[1,]  0.5855288 -0.31135095 -1.0377854
[2,]  1.2949948  0.59430322 -0.5116634
[3,]  1.1856915  1.28751444 -0.1316301
[4,]  0.7321943  1.64792194  0.7132483
[5,]  1.3380818  0.09367809  1.3288343
[6,] -0.4798742 -0.61468043  0.1199645#使用slotNames()显示模型包含的全部对象类型
slotNames(model.vecm)[1] "x"         "Z0"        "Z1"        "ZK"        "type"      "model"     "ecdet"    
 [8] "lag"       "P"         "season"    "dumvar"    "cval"      "teststat"  "lambda"   
[15] "Vorg"      "V"         "W"         "PI"        "DELTA"     "GAMMA"     "R0"       
[22] "RK"        "bp"        "spec"      "call"      "test.name"summary(model.vecm)###################### 
# Johansen-Procedure # 
###################### 

Test type: maximal eigenvalue statistic (lambda max) , with linear trend Eigenvalues (lambda):
[1] 0.222707791 0.167079305 0.007684667

Values of teststatistic and critical values of test:

           test 10pct  5pct  1pct
r <= 2 |   3.84  6.50  8.18 11.65
r <= 1 |  91.04 12.91 14.90 19.19
r = 0  | 125.47 18.90 21.07 25.75

Eigenvectors, normalised to first column:
(These are the cointegration relations)

           y1.l2     y2.l2      y3.l2
y1.l2  1.0000000  1.000000  1.0000000
y2.l2 -0.2355148 -5.064504 -0.1799248
y3.l2 -1.1315152  1.143660 -0.1993207
Weights W:
(This is the loading matrix)

          y1.l2        y2.l2        y3.l2
y1.d 0.05151358  0.002693258 -0.008416933
y2.d 0.11164178  0.075923301 -0.002918384
y3.d 0.51768302 -0.015197036 -0.006078055从统计检验值可以看出，在r为2时接收原假设，即认为协整向量的秩为2
## 使用cajorls()估计VECM模型的系数矩阵
cajorls(model.vecm,r=2) #估计VECM模型 ，cajorls(z, r = 1, r      #   eg.number = NULL)，其中r为协整向量的秩$rlm

Call:


lm(formula = substitute(form1), data = data.mat)

Coefficients:
          y1.d      y2.d      y3.d    
ect1       0.05421   0.18757   0.50249
ect2      -0.02577  -0.41081  -0.04496
constant   0.08575   0.28405  -0.02490
y1.dl1     0.01915   0.22922   0.47143
y2.dl1     0.01735  -0.38180   0.04342
y3.dl1    -0.03389  -0.05120  -0.58422

$beta
               ect1       ect2
y1.l2  1.000000e+00  0.0000000
y2.l2  2.775558e-17  1.0000000
y3.l2 -1.242478e+00 -0.4711494计算结果中，第一部分（rlm)给出误差校正矩阵、常数项及差分解释变量的估计值；第二部分为标准化后的协整向量矩阵。估计的误差修正模型为：
## VECM模型转化为水平VAR模型
library(vars)
model.var<-vec2var(model.vecm,r=2) #获取与VECM模型等价########的VAR模型估计
model.var
Coefficient matrix of lagged endogenous variables:


A1:
       y1.l1      y2.l1       y3.l1
y1 1.0191535 0.01734796 -0.03389437
y2 0.2292213 0.61819733 -0.05119624
y3 0.4714272 0.04342308  0.41578278


A2:
         y1.l2       y2.l2       y3.l2
y1  0.03505334 -0.04312019 -0.02131386
y2 -0.04165626 -0.02900446  0.01170232
y3  0.03105876 -0.08837964 -0.01892923

Coefficient matrix of deterministic regressor(s).      constant
y1  0.08574980
y2  0.28405415
y3 -0.02490038
计算结果表明，与VECM模型等价的VAR模型估计为：



