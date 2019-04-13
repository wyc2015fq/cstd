
# R语言使用逻辑回归分类算法 - jiahaowanhao的博客 - CSDN博客


2018年05月31日 13:37:45[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：295


[R语言使用逻辑回归分类算法](http://cda.pinggu.org/view/25696.html)
逻辑回归属于概率统计的分类算法模型的算法，是根据一个或者多个特征进行类别标号预测。在R语言中可以通过调用logit函数执行逻辑回归分类算法并预测输出概率。通过调用glm函数将family参数也就是响应分布指定为binominal（二项式），就是使用逻辑回归算法。
![](http://www.cda.cn/uploadfile/image/20180115/20180115074637_99475.png)
操作
同进述内容一样准备好训练数据集与测试数据集。
fit = glm(churn ~ .,data = trainset,family = binomial)
summary(fit)
Call:
glm(formula = churn ~ ., family = binomial, data = trainset)
Deviance Residuals:
Min       1Q   Median       3Q      Max
-3.1519   0.1983   0.3460   0.5186   2.1284
Coefficients:
Estimate Std. Error z value Pr(>|z|)
(Intercept)                    8.3462866  0.8364914   9.978  < 2e-16 ***
international_plan1           -2.0534243  0.1726694 -11.892  < 2e-16 ***
voice_mail_plan1               1.3445887  0.6618905   2.031 0.042211 *
number_vmail_messages         -0.0155101  0.0209220  -0.741 0.458496
total_day_minutes              0.2398946  3.9168466   0.061 0.951163
total_day_calls               -0.0014003  0.0032769  -0.427 0.669141
total_day_charge              -1.4855284 23.0402950  -0.064 0.948592
total_eve_minutes              0.3600678  1.9349825   0.186 0.852379
total_eve_calls               -0.0028484  0.0033061  -0.862 0.388928
total_eve_charge              -4.3204432 22.7644698  -0.190 0.849475
total_night_minutes            0.4431210  1.0478105   0.423 0.672367
total_night_calls              0.0003978  0.0033188   0.120 0.904588
total_night_charge            -9.9162795 23.2836376  -0.426 0.670188
total_intl_minutes             0.4587114  6.3524560   0.072 0.942435
total_intl_calls               0.1065264  0.0304318   3.500 0.000464 ***
total_intl_charge             -2.0803428 23.5262100  -0.088 0.929538
number_customer_service_calls -0.5109077  0.0476289 -10.727  < 2e-16 ***
---
Signif. codes:  0 ‘***’ 0.001 ‘**’ 0.01 ‘*’ 0.05 ‘.’ 0.1 ‘ ’ 1
(Dispersion parameter for binomial family taken to be 1)
Null deviance: 1938.8  on 2314  degrees of freedom
Residual deviance: 1515.3  on 2298  degrees of freedom
AIC: 1549.3
Number of Fisher Scoring iterations: 6
找到分类模型中包含的可能导致错误分类的非显著变量，仅使用显著的变量来训练分类模型。
fit = glm(churn ~ international_plan + voice_mail_plan + number_customer_service_calls,data = trainset,family = binomial)
summary(fit)
Call:
glm(formula = churn ~ international_plan + voice_mail_plan +
number_customer_service_calls, family = binomial, data = trainset)
Deviance Residuals:
Min       1Q   Median       3Q      Max
-2.6485   0.3067   0.4500   0.5542   1.6509
Coefficients:
Estimate Std. Error z value Pr(>|z|)
(Intercept)                    2.68272    0.12064  22.237  < 2e-16 ***
international_plan1           -1.97626    0.15998 -12.353  < 2e-16 ***
voice_mail_plan1               0.79423    0.16352   4.857 1.19e-06 ***
number_customer_service_calls -0.44341    0.04445  -9.975  < 2e-16 ***
---
Signif. codes:  0 ‘***’ 0.001 ‘**’ 0.01 ‘*’ 0.05 ‘.’ 0.1 ‘ ’ 1
(Dispersion parameter for binomial family taken to be 1)
Null deviance: 1938.8  on 2314  degrees of freedom
Residual deviance: 1678.5  on 2311  degrees of freedom
AIC: 1686.5
Number of Fisher Scoring iterations: 5
调用fit使用一个内置模型来预测testset数据集的输出，可以通过调整概率是否高于0.5来改变类别标记的输出结果。
\#这是选择预测之后的输出结果，这个参数能用在binomial数据，也就是响应变量是二分型的时候，这个参数选成type＝response，表示输出结果预测响应变量为1的概率。
pred = predict(fit,testset,type = "response")
\#将ped中概率大于0.5的设置TRUE,代表为“no”,没有流失客户，1
\#将ped中概率小于0.5的设置FALSE,代表为“yes”,有流失
客户，0
Class = pred > 0.5
summary(Class)
Mode   FALSE    TRUE
logical      28     990
对测试数据集的分类和预测结果进行统计分析计数：
tb = table(testset$churn,Class)
> tb
Class
FALSE TRUE
yes    15  126
no     13  864
将上一步骤的统计结果用分类形式表输出，并生成混淆矩阵
churn.mod = ifelse(testset$churn == "yes",1,0)
> churn.mod
[1] 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 1 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0
[44] 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 0 1 0 0 0 0 0 0
[87] 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 1 0 0 0 0
[130] 0 0 1 0 0 0 0 1 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1 0
[173] 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0
[216] 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 1 0 0 0
[259] 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 1 0 1 0 0 1 0
[302] 0 0 0 0 0 1 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 0
[345] 0 0 0 0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0
[388] 0 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 1 0 0 0 1 0 1 0 0 0 0 0 1 0 1 0 0 0 0 1 0 0 0 0
[431] 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0
[474] 0 0 0 1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 1 0 0
[517] 0 0 0 0 0 0 0 0 1 0 1 1 0 0 1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 1 0 0 0 0 0 0
[560] 0 0 0 0 0 0 0 1 0 1 0 1 1 0 1 0 1 0 0 0 0 1 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 1
[603] 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1 0 0 0 0 0 0 0 0 0 1 0 0 0 1 0 1 0 0 0 0
[646] 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 1 0 0 1 0 0 0 0 0
[689] 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 0 1 0 0 1 0 0 0 0 1 0 0 1 0
[732] 1 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0
[775] 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 1 0 0 0 0 1 0 0 1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0
[818] 0 0 0 0 0 0 0 0 0 1 0 1 0 0 0 1 1 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 1
[861] 1 1 0 0 1 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
[904] 0 0 1 1 0 0 0 1 0 0 1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 1
[947] 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0
[990] 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 1 0 0 0 0 1 0 0 0
将Class转化成numeric
ABC = as.numeric(Class)
ABC与churn.mod 中0，1代表的意思相反，将ABC进行数值取反
BC = 1 - ABC
计算混淆矩阵
confusionMatrix(churn.mod,BC)
Confusion Matrix and Statistics
Reference
Prediction   0   1
0 864  13
1 126  15
Accuracy : 0.8635
95% CI : (0.8408, 0.884)
No Information Rate : 0.9725
P-Value [Acc > NIR] : 1
Kappa : 0.138
Mcnemar's Test P-Value : <2e-16
Sensitivity : 0.8727
Specificity : 0.5357
Pos Pred Value : 0.9852
Neg Pred Value : 0.1064
Prevalence : 0.9725
Detection Rate : 0.8487
Detection Prevalence : 0.8615
Balanced Accuracy : 0.7042
'Positive' Class : 0
逻辑回归算法和线性回归非常相似，两者区别是在于线性回归算法中的变量是连续变量，而逻辑回归响应变量是二分类的变量（名义变量），使用逻辑回归算法主要目的是利用logit模型去预测和测量变量相关的名义变量的概率。逻辑回归公式：ln(P/(1-P)),P为某事情发生的概率。
逻辑回归的算法的优势是在于算法易于理解，能够直接输出预测模型的逻辑概率逻辑值以及结果的置信区间，与决策树难以更新模型不同，逻辑回归算法能够迅速在逻辑回归算法中合并新的数据，更新分类模型，逻辑回归算法的不足是无法处理多重共线性问题，因此解决变量必须线性无关。glm提供了一个通用的线性回归模型，可以通过设置family参数得到，当为binomial回归时，可以实现二元分类。
调用fit函数预测测试数据集testset的类别响应变量，fit函数能够输出类标号的概率，如果概率值小于等于0.5,意味预测得出的类标号与测试数据集的实际类标号不相符，如果大于0.5则说明两者是一致的，进一步调用summsary函数来得到预测的模型。最后进行计数统计与混淆矩阵。

