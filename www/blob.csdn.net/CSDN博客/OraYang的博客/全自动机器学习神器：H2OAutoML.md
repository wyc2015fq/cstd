
# 全自动机器学习神器：H2OAutoML - OraYang的博客 - CSDN博客

2018年02月01日 10:35:32[OraYang](https://me.csdn.net/u010665216)阅读数：3097所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



# 引言
做机器学习的老铁们在平时训练模型时，对交叉验证、模型集成想必是绞尽了脑汁。现在我将给各位介绍一个神器。叫做[H2O](http://h2o-release.s3.amazonaws.com/h2o/master/3888/docs-website/h2o-docs/welcome.html)。在读了这篇文章后，你将会：
> 了解H2O是什么，在哪些地方大放异彩

> H2O的安装与初步使用

> 迫不及待地去安装使用（哈哈哈）

# H2O概述
H2O是一个开源的、内存、分布式、快速和可扩展的机器学习和预测分析平台，允许诸位在大数据上构建机器学习模型，并在企业环境中轻松实现这些模型的搭建。
H2O的核心代码是用Java编写的。在H2O中，使用分布式的Key/Value存储来访问和引用所有节点和机器上的数据、模型、对象等。这些算法是在H2O的分布式Map / Reduce框架之上实现的，并且利用Java Fork / Join框架来实现多线程。数据是并行读取的，并分布在整个集群中，并以压缩的方式以列状格式存储在内存中。 H2O的数据解析器具有内置的智能功能，可以猜测传入数据集的模式，并支持以多种格式从多个源获取数据。
H2O的REST API允许外部程序或脚本通过HTTP上的JSON访问H2O的所有功能。 Rest API使用H2O的Web界面（Flow UI），R binding（H2O-R）和Python binding（H2O-Python）。
深度学习，Tree Ensembles和GLRM等各种有监督和无监督算法的速度，质量，易用性和模型部署方便使得H2O成为大数据数据科学非常受欢迎的API。
# H2O的安装及AutoML的使用
## H2O的安装（python）
H2O对 Scala, R, and Python并没有硬性要求，但是Java是必须要会的。接下来我们就讲下在python环境中安装H2O。
首先安装依赖文件:
```python
$
```
```python
pip install requests
```
```python
$
```
```python
pip install tabulate
```
```python
$
```
```python
pip install scikit-learn
```
接下来下载安装H2O
```python
$ pip install -f http://h2o-release
```
```python
.s
```
```python
3
```
```python
.amazonaws
```
```python
.com
```
```python
/h2o/latest_stable_Py
```
```python
.html
```
```python
h2o
```
软件大小100多M。
## AutoML的使用
输入以下代码
```python
import
```
```python
h2o
```
```python
from
```
```python
h2o.automl
```
```python
import
```
```python
H2OAutoML
h2o.init()
```
```python
# Import a sample binary outcome train/test set into H2O
```
```python
train = h2o.import_file(
```
```python
"https://s3.amazonaws.com/erin-data/higgs/higgs_train_10k.csv"
```
```python
)
test = h2o.import_file(
```
```python
"https://s3.amazonaws.com/erin-data/higgs/higgs_test_5k.csv"
```
```python
)
```
```python
# Identify predictors and response
```
```python
x = train.columns
y =
```
```python
"response"
```
```python
x.remove(y)
```
```python
# For binary classification, response should be a factor
```
```python
train[y] = train[y].asfactor()
test[y] = test[y].asfactor()
```
```python
# Run AutoML for 30 seconds
```
```python
aml = H2OAutoML(max_runtime_secs =
```
```python
30
```
```python
)
aml.train(x = x, y = y,
          training_frame = train,
          leaderboard_frame = test)
```
```python
# View the AutoML Leaderboard
```
```python
lb = aml.leaderboard
lb
```
`Checking whether there is an H2O instance running at http://localhost:54321..... not found.
Attempting to start a local H2O server...
  Java Version: openjdk version "1.8.0_131"; OpenJDK Runtime Environment (build 1.8.0_131-8u131-b11-0ubuntu1.16.10.2-b11); OpenJDK 64-Bit Server VM (build 25.131-b11, mixed mode)
  Starting server from /home/ora/anaconda3/lib/python3.6/site-packages/h2o/backend/bin/h2o.jar
  Ice root: /tmp/tmphnkk6mvy
  JVM stdout: /tmp/tmphnkk6mvy/h2o_ora_started_from_python.out
  JVM stderr: /tmp/tmphnkk6mvy/h2o_ora_started_from_python.err
  Server is running at http://127.0.0.1:54321
Connecting to H2O server at http://127.0.0.1:54321... successful.`|H2O cluster uptime:|02 secs|
|H2O cluster version:|3.16.0.4|
|H2O cluster version age:|16 days|
|H2O cluster name:|H2O_from_python_ora_l1c8zv|
|H2O cluster total nodes:|1|
|H2O cluster free memory:|6.976 Gb|
|H2O cluster total cores:|4|
|H2O cluster allowed cores:|4|
|H2O cluster status:|accepting new members, healthy|
|H2O connection url:|http://127.0.0.1:54321|
|H2O connection proxy:|None|
|H2O internal security:|False|
|H2O API Extensions:|XGBoost, Algos, AutoML, Core V3, Core V4|
|Python version:|3.6.0 final|
`Parse progress: |█████████████████████████████████████████████████████████| 100%
Parse progress: |█████████████████████████████████████████████████████████| 100%
AutoML progress: |████████████████████████████████████████████████████████| 100%
Parse progress: |█████████████████████████████████████████████████████████| 100%`|model_id|auc|logloss|
|---|---|---|
|StackedEnsemble_AllModels_0_AutoML_20180201_101807|0.787269|0.554504|
|StackedEnsemble_BestOfFamily_0_AutoML_20180201_101807|0.783812|0.557977|
|GBM_grid_0_AutoML_20180201_101807_model_0|0.779296|0.562086|
|GBM_grid_0_AutoML_20180201_101807_model_2|0.779109|0.560944|
|GBM_grid_0_AutoML_20180201_101807_model_1|0.775373|0.564924|
|GBM_grid_0_AutoML_20180201_101807_model_3|0.773419|0.567071|
|GBM_grid_0_AutoML_20180201_101807_model_4|0.755339|0.630771|
|DRF_0_AutoML_20180201_101807|0.740823|0.605117|
|XRT_0_AutoML_20180201_101807|0.735793|0.604911|
|GLM_grid_0_AutoML_20180201_101807_model_0|0.686224|0.634806|
```python
# The leader model is stored here
```
```python
aml.leader
```
`Model Details
=============
H2OStackedEnsembleEstimator :  Stacked Ensemble
Model Key:  StackedEnsemble_AllModels_0_AutoML_20180201_101807
No model summary for this model

ModelMetricsBinomialGLM: stackedensemble
** Reported on train data. **
MSE: 0.10619313022292985
RMSE: 0.32587287432821077
LogLoss: 0.36728814169184465
Null degrees of freedom: 7993
Residual degrees of freedom: 7986
Null deviance: 11050.743244827558
Residual deviance: 5872.202809369212
AIC: 5888.202809369212
AUC: 0.9569241768110353
Gini: 0.9138483536220705
Confusion Matrix (Act/Pred) for max f1 @ threshold = 0.4510434688048974:`|0|1|Error|Rate|
|0|3039.0|708.0|0.189|(708.0/3747.0)|
|1|240.0|4007.0|0.0565|(240.0/4247.0)|
|Total|3279.0|4715.0|0.1186|(948.0/7994.0)|
`Maximum Metrics: Maximum metrics at their respective thresholds`|metric|threshold|value|idx|
|max f1|0.4510435|0.8942200|221.0|
|max f2|0.3725997|0.9326728|253.0|
|max f0point5|0.6114940|0.9036081|158.0|
|max accuracy|0.5056487|0.8855392|200.0|
|max precision|0.9380498|1.0|0.0|
|max recall|0.1695174|1.0|349.0|
|max specificity|0.9380498|1.0|0.0|
|max absolute_mcc|0.5056487|0.7701297|200.0|
|max min_per_class_accuracy|0.5333867|0.8849746|190.0|
|max mean_per_class_accuracy|0.5333867|0.8852705|190.0|
`Gains/Lift Table: Avg response rate: 53.13 %`|group|cumulative_data_fraction|lower_threshold|lift|cumulative_lift|response_rate|cumulative_response_rate|capture_rate|cumulative_capture_rate|gain|cumulative_gain|
|1|0.0100075|0.9118284|1.8822698|1.8822698|1.0|1.0|0.0188368|0.0188368|88.2269838|88.2269838|
|2|0.0200150|0.9032538|1.8822698|1.8822698|1.0|1.0|0.0188368|0.0376737|88.2269838|88.2269838|
|3|0.0300225|0.8976278|1.8822698|1.8822698|1.0|1.0|0.0188368|0.0565105|88.2269838|88.2269838|
|4|0.0400300|0.8922493|1.8822698|1.8822698|1.0|1.0|0.0188368|0.0753473|88.2269838|88.2269838|
|5|0.0500375|0.8878612|1.8822698|1.8822698|1.0|1.0|0.0188368|0.0941841|88.2269838|88.2269838|
|6|0.1000751|0.8646618|1.8822698|1.8822698|1.0|1.0|0.0941841|0.1883683|88.2269838|88.2269838|
|7|0.1499875|0.8402188|1.8775524|1.8807000|0.9974937|0.9991660|0.0937132|0.2820815|87.7552369|88.0699971|
|8|0.2000250|0.8133639|1.8446244|1.8716754|0.98|0.9943715|0.0923004|0.3743819|84.4624441|87.1675448|
|9|0.2999750|0.7490769|1.7951059|1.8461629|0.9536921|0.9808173|0.1794208|0.5538027|79.5105903|84.6162910|
|10|0.4000500|0.6641860|1.6281634|1.7916290|0.865|0.9518449|0.1629385|0.7167412|62.8163409|79.1628951|
|11|0.5|0.5605949|1.4158250|1.7165058|0.7521902|0.9119340|0.1415117|0.8582529|41.5824997|71.6505769|
|12|0.5999500|0.4321141|0.9258223|1.5847802|0.4918648|0.8419516|0.0925359|0.9507888|-7.4177664|58.4780151|
|13|0.7000250|0.3225593|0.3788068|1.4123751|0.20125|0.7503574|0.0379091|0.9886979|-62.1193195|41.2375098|
|14|0.7999750|0.2409276|0.0989428|1.2482731|0.0525657|0.6631744|0.0098893|0.9985872|-90.1057155|24.8273085|
|15|0.8999249|0.1645587|0.0141347|1.1112038|0.0075094|0.5903531|0.0014128|1.0|-98.5865308|11.1203781|
|16|1.0|0.0558224|0.0|1.0|0.0|0.5312735|0.0|1.0|-100.0|0.0|
`ModelMetricsBinomialGLM: stackedensemble
** Reported on validation data. **
MSE: 0.18783313364822057
RMSE: 0.4333972007849388
LogLoss: 0.555646918852381
Null degrees of freedom: 2005
Residual degrees of freedom: 1998
Null deviance: 2777.4964239309966
Residual deviance: 2229.2554384357527
AIC: 2245.2554384357527
AUC: 0.7876166353248658
Gini: 0.5752332706497316
Confusion Matrix (Act/Pred) for max f1 @ threshold = 0.3557814003482815:`|0|1|Error|Rate|
|0|463.0|495.0|0.5167|(495.0/958.0)|
|1|128.0|920.0|0.1221|(128.0/1048.0)|
|Total|591.0|1415.0|0.3106|(623.0/2006.0)|
`Maximum Metrics: Maximum metrics at their respective thresholds`|metric|threshold|value|idx|
|max f1|0.3557814|0.7470564|271.0|
|max f2|0.1919339|0.8565737|352.0|
|max f0point5|0.6149732|0.7418069|157.0|
|max accuracy|0.5108126|0.7228315|198.0|
|max precision|0.9245128|1.0|0.0|
|max recall|0.1152450|1.0|383.0|
|max specificity|0.9245128|1.0|0.0|
|max absolute_mcc|0.5108126|0.4439970|198.0|
|max min_per_class_accuracy|0.5377255|0.7156489|187.0|
|max mean_per_class_accuracy|0.5108126|0.7216001|198.0|
`Gains/Lift Table: Avg response rate: 52.24 %`|group|cumulative_data_fraction|lower_threshold|lift|cumulative_lift|response_rate|cumulative_response_rate|capture_rate|cumulative_capture_rate|gain|cumulative_gain|
|1|0.0104686|0.9059318|1.8229735|1.8229735|0.9523810|0.9523810|0.0190840|0.0190840|82.2973464|82.2973464|
|2|0.0204387|0.8987579|1.7227099|1.7740644|0.9|0.9268293|0.0171756|0.0362595|72.2709924|77.4064420|
|3|0.0304088|0.8917429|1.8184160|1.7886059|0.95|0.9344262|0.0181298|0.0543893|81.8416031|78.8605932|
|4|0.0403789|0.8877437|1.6270038|1.7487042|0.85|0.9135802|0.0162214|0.0706107|62.7003817|74.8704175|
|5|0.0503490|0.8811732|1.8184160|1.7625085|0.95|0.9207921|0.0181298|0.0887405|81.8416031|76.2508503|
|6|0.1001994|0.8536235|1.7227099|1.7427082|0.9|0.9104478|0.0858779|0.1746183|72.2709924|74.2708215|
|7|0.1500499|0.8239882|1.5887214|1.6915498|0.83|0.8837209|0.0791985|0.2538168|58.8721374|69.1549796|
|8|0.2003988|0.7921643|1.5540398|1.6570013|0.8118812|0.8656716|0.0782443|0.3320611|55.4039755|65.7001253|
|9|0.3000997|0.7256447|1.4068798|1.5739044|0.735|0.8222591|0.1402672|0.4723282|40.6879771|57.3904415|
|10|0.4002991|0.6465250|1.2760814|1.4993559|0.6666667|0.7833126|0.1278626|0.6001908|27.6081425|49.9355946|
|11|0.5|0.5432006|1.0814790|1.4160305|0.565|0.7397807|0.1078244|0.7080153|8.1479008|41.6030534|
|12|0.6001994|0.4459747|0.9046846|1.3306646|0.4726368|0.6951827|0.0906489|0.7986641|-9.5315408|33.0664642|
|13|0.6999003|0.3588501|0.7273664|1.2447247|0.38|0.6502849|0.0725191|0.8711832|-27.2633588|24.4724723|
|14|0.8000997|0.2856659|0.6094717|1.1651697|0.3184080|0.6087227|0.0610687|0.9322519|-39.0528275|16.5169675|
|15|0.8998006|0.2029565|0.4593893|1.0869669|0.24|0.5678670|0.0458015|0.9780534|-54.0610687|8.6966865|
|16|1.0|0.0667329|0.2190289|1.0|0.1144279|0.5224327|0.0219466|1.0|-78.0971099|0.0|
`ModelMetricsBinomialGLM: stackedensemble
** Reported on cross-validation data. **
MSE: 0.19021917932290436
RMSE: 0.4361412378151192
LogLoss: 0.5597680244722494
Null degrees of freedom: 7993
Residual degrees of freedom: 7986
Null deviance: 11053.314251577507
Residual deviance: 8949.571175262323
AIC: 8965.571175262323
AUC: 0.7816115854774708
Gini: 0.5632231709549416
Confusion Matrix (Act/Pred) for max f1 @ threshold = 0.39478250550345867:`|0|1|Error|Rate|
|0|2001.0|1746.0|0.466|(1746.0/3747.0)|
|1|689.0|3558.0|0.1622|(689.0/4247.0)|
|Total|2690.0|5304.0|0.3046|(2435.0/7994.0)|
`Maximum Metrics: Maximum metrics at their respective thresholds`|metric|threshold|value|idx|
|max f1|0.3947825|0.7450529|256.0|
|max f2|0.1915353|0.8603172|349.0|
|max f0point5|0.5904633|0.7343911|164.0|
|max accuracy|0.5084678|0.7069052|201.0|
|max precision|0.9356320|1.0|0.0|
|max recall|0.0908959|1.0|389.0|
|max specificity|0.9356320|1.0|0.0|
|max absolute_mcc|0.5756414|0.4173777|171.0|
|max min_per_class_accuracy|0.5307710|0.7045637|190.0|
|max mean_per_class_accuracy|0.5756414|0.7085280|171.0|
`Gains/Lift Table: Avg response rate: 53.13 %`|group|cumulative_data_fraction|lower_threshold|lift|cumulative_lift|response_rate|cumulative_response_rate|capture_rate|cumulative_capture_rate|gain|cumulative_gain|
|1|0.0100075|0.9088610|1.8352131|1.8352131|0.975|0.975|0.0183659|0.0183659|83.5213092|83.5213092|
|2|0.0200150|0.9004900|1.7410996|1.7881563|0.925|0.95|0.0174241|0.0357900|74.1099600|78.8156346|
|3|0.0300225|0.8947453|1.8587415|1.8116847|0.9875|0.9625|0.0186014|0.0543913|85.8741465|81.1684719|
|4|0.0400300|0.8870404|1.7175712|1.7881563|0.9125|0.95|0.0171886|0.0715799|71.7571227|78.8156346|
|5|0.0500375|0.8800252|1.7881563|1.7881563|0.95|0.95|0.0178950|0.0894749|78.8156346|78.8156346|
|6|0.1000751|0.8521482|1.6987485|1.7434524|0.9025|0.92625|0.0850012|0.1744761|69.8748528|74.3452437|
|7|0.1499875|0.8207969|1.6133741|1.7001653|0.8571429|0.9032527|0.0805274|0.2550035|61.3374146|70.0165333|
|8|0.2000250|0.7892257|1.5293442|1.6574334|0.8125|0.8805503|0.0765246|0.3315281|52.9344243|65.7433353|
|9|0.2999750|0.7135059|1.3569304|1.5573075|0.7209011|0.8273561|0.1356251|0.4671533|35.6930446|55.7307489|
|10|0.4000500|0.6280223|1.2046527|1.4690887|0.64|0.7804878|0.1205557|0.5877090|20.4652696|46.9088654|
|11|0.5|0.5422533|1.0412557|1.3835649|0.5531915|0.7350513|0.1040735|0.6917824|4.1255655|38.3564869|
|12|0.5999500|0.4489542|0.9281781|1.3076987|0.4931164|0.6947456|0.0927714|0.7845538|-7.1821882|30.7698728|
|13|0.7000250|0.3627716|0.7740835|1.2314135|0.41125|0.6542173|0.0774664|0.8620202|-22.5916529|23.1413487|
|14|0.7999750|0.2811813|0.6384169|1.1573237|0.3391740|0.6148554|0.0638097|0.9258300|-36.1583071|15.7323691|
|15|0.8999249|0.1935312|0.5394741|1.0887024|0.2866083|0.5783987|0.0539204|0.9797504|-46.0525916|8.8702362|
|16|1.0|0.0550341|0.2023440|1.0|0.1075|0.5312735|0.0202496|1.0|-79.7655992|0.0|

```python
# If you need to generate predictions on a test set, you can make
```
```python
# predictions directly on the `"H2OAutoML"` object, or on the leader
```
```python
# model object directly
```
```python
preds = aml.predict(test)
```
```python
# or:
```
```python
preds = aml.leader.predict(test)
```
`Parse progress: |█████████████████████████████████████████████████████████| 100%
stackedensemble prediction progress: |████████████████████████████████████| 100%
stackedensemble prediction progress: |████████████████████████████████████| 100%`大家可以发现，本质输入下面两行代码
```python
aml = H2OAutoML(max_runtime_secs =
```
```python
30
```
```python
)
aml.train(x = x, y = y,
          training_frame = train,
          leaderboard_frame = test)
```
**就能训练好我们的模型，上面两行代码不仅指定了模型运行的时间：30s，而且默认进行交叉验证以及训练了7个GBM1个DRF模型，及两个集成模型。关键是只要两行代码！！！**
# 总结
我给大家介绍的这个工具是不是很神奇?感兴趣的读者们，可以自行做进一步研究哈。
github：[H2OAutoML](https://github.com/shawshany/H2OAutoML)

