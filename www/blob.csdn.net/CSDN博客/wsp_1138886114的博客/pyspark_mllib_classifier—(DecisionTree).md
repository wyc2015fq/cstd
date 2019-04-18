# pyspark_mllib_classifier—(DecisionTree) - wsp_1138886114的博客 - CSDN博客





2018年08月01日 19:20:46[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：147










- - - - - [一、创建pysparkSession运行环境](#一创建pysparksession运行环境)
- [二、获取数据集并格式化数据](#二获取数据集并格式化数据)
- [三、特征工程—提取特征](#三特征工程提取特征)
- [四、DecisionTree 二分类](#四decisiontree-二分类)- [step1. 构建训练函数（DecisionTree）](#step1-构建训练函数decisiontree)
- [step2. 模型训练 与 评估](#step2-模型训练-与-评估)
- [step3 不同超参数组合，训练与评估，找到最佳模型](#step3-不同超参数组合训练与评估找到最佳模型)
- [step4 保存模型 加载模型-使用](#step4-保存模型-加载模型-使用)










`开发环境 jupyter notebook`


##### 一、创建pysparkSession运行环境

```python
import os
import time
from pyspark import SparkConf, SparkContext

# 设置环境变量
os.environ['JAVA_HOME'] = 'C:\Java\jdk1.8.0_91'
# HADOOP在Windows的兼容性问题  主要需要$HADOOP_HOME/lib中winutils.exe等文件
os.environ['HADOOP_HOME'] = 'C:\Java\hadoop-2.6.0-cdh5.7.6'
# 设置SPARK_HOME环境变量, 非常重要, 如果没有设置的话,SparkApplication运行不了
os.environ['SPARK_HOME'] = 'C:\Java\spark-2.2.0-bin-2.6.0-cdh5.7.6'

sparkConf = SparkConf()\                       # Create SparkConf
    .setAppName('Python_Spark_Classifier')\
    .setMaster('local[2]')

sc = SparkContext(conf=sparkConf)             # Create SparkContext
print(sc)                                     # 检测 SparkContext 是否创建成功
```

##### 二、获取数据集并格式化数据

```python
"""
        使用决策树二元分类分析StumbleUpon数据集，预测网页是暂时性（Ephemeral）或是长青的（Evergreen），
        并且调校参数找出最佳参数组合，提高预测准确度。
        决策树的优点：条例清晰、方法简单、易于理解、使用范围广等。 

    数据集中字段：
        -a. 每行数据的各个字段使用制表符隔开 \t
            文件的第一行为字段名称
        -b. 字段 0 - 2 
            表示的是 url网址、urlid网址ID、boilerplate连接的样本文字,此三个字段与判断网页是否长青性关系不大，忽略
        -c. 字段 3 - 25 
            总共23个字段属于特征字段值，基本上都是数值类型特征
        -d. 字段26
            属于 标签label，具有两个值
            - 0: 代表长青性（evergreen）
            - 1： 代表的是短暂性 

    数据分为三个部分：
        - 第一个部分：
            第一个字段：特征地址为 类别特征数据
        - 第二个部分：
            最后一个字段：label 字段
        - 第三个部分：
            其他字段：数值特征字段
""" 

# 读取数据集格式化： 预测网页是短暂的还是长青的
raw_rdd = sc.textFile('./datas/train.tsv')

#数据概要浏览
print raw_rdd.take(1)
len(raw_rdd.take(1)[0].split("\t"))
raw_rdd.take(2)                         # 获取原始数据集中前2条数据 

header_data = raw_rdd.first()           # 获取第一条数据

filter_rdd = raw_rdd\                   # 过滤第一条数据
            .filter(lambda line: line != header_data)

datas = filter_rdd \                    # 替换数据中 引号，每行按照制表符分割
    .map(lambda line: line.replace("\"", ""))\
    .map(lambda line: line.split("\t"))

print ("总共：" + str(datas.count()))
```

##### 三、特征工程—提取特征

```python
"""
  编写函数extract_feature实现特征字段的提取：
  1. 提取 类别特征 字段
  2. 提取 数字特征 字段
  3. 合并 类别特征 + 数值特征
""" 
# 类别特征数据转换：采用 1-of-K，其中K表示的就是类别的个数
catetory_dic = datas \                   # 构建 网页类别 字典
    .map(lambda fields: fields[3]) \
    .distinct() \
    .zipWithIndex() \
    .collectAsMap()

#catetory_dic 
print (len(catetory_dic))               # 查看字典的 项数
type(catetory_dic)                      # 查看类型 

import numpy as np


def conver_float(x):                    # 将字段的值为 ? 替换为数值 0。
    return (0 if x == "?" else float(x))

# 特征字段提取
def extract_features(fields, catetory_dic, end_index):
    # 类别字段
    category_index = catetory_dic[fields[3]]
    category_features = np.zeros(len(catetory_dic))
    category_features[category_index] = 1.0

    # 数值字段
    numeric_features = [ conver_float(column) for column in fields[4: end_index]]
    # print (numeric_features) (若使用贝叶斯算法，没有负数：abs(conver_float(column)))

    # 返回 类别特征 + 数值特征 
    return np.concatenate((category_features, numeric_features)) 

def extract_label(fields):               # 提取 标签字段
    label = fields[-1]
    return float(label)

sample_data = datas.first()             # 测试 提取特征函数
print (extract_features(sample_data, catetory_dic, len(sample_data)-1)) 

# 构建分类算法特征RDD： help(LabeledPoint)
from pyspark.mllib.regression import LabeledPoint

labelpoint_rdd = datas.map(lambda r:
                           LabeledPoint(extract_label(r), 
                           extract_features(r, catetory_dic, len(r)-1)))

print (datas.first()[3:])          # 获取数据对比   
print (labelpoint_rdd.take(1)) 

#拆分数据集（训练集、验证集和测试集）按照 8: 1: 1划分；  help(labelpoint_rdd.randomSplit) 
train_rdd, validation_rdd, test_rdd = labelpoint_rdd.randomSplit([8,1,1])

# 统计各个数据集条目数，并且将数据缓存起来
print ('训练数据集: ', train_rdd.cache().count())
print ('验证数据集: ', validation_rdd.cache().count())
print ('测试数据集: ', test_rdd.cache().count())
```

##### 四、DecisionTree 二分类

###### step1. 构建训练函数（DecisionTree）

```python
from pyspark.mllib.tree import DecisionTree
#help(DecisionTree)
dt_model = DecisionTree.trainClassifier(train_rdd, 2, {}, 
                                        impurity='entropy', 
                                        maxDepth=5, maxBins=32)
#print (dt_model)

# 基于训练的模型和验证书籍及 评估
score = dt_model.predict(validation_rdd.map(lambda lp: lp.features))

# 组合预测的值和实际真实的值
score_and_label = score.zip(validation_rdd.map(lambda lp: lp.label))

# 获取前十个数据打印
score_and_label.take(10) 

"""
    使用AUC（Area under the Curve of ROC）评估二分类模型，
    -a. AUC = 1
        最完美的情况，预测准确率到100%，但是不可能存在
    -b. 0.5  < AUC < 1
        优于随机猜测，具有预测的价值
    -c. AUC = 0.5
        余随机猜测一样，没有任何预测价值
    -d. AUC < 0.5
        适合于反向预测

    ROC曲线  PR曲线： 精确度precision 和 召回率 之间的关系
""" 

from pyspark.mllib.evaluation import BinaryClassificationMetrics

def evaluate_mode(model, validation_data):      # 方便对模型评估，定义函数
    # 基于训练的模型和验证书籍及 评估
    score = model.predict(validation_data.map(lambda lp: lp.features))

    # 组合预测的值和实际真实的值
    score_and_label = score.zip(validation_data.map(lambda lp: lp.label))

    # 使用BinaryClassificationMetrics 计算AUC面积
    metrics = BinaryClassificationMetrics(score_and_label)
    #print ("AUC: ", metrics.areaUnderROC)
    #print ("PR: ", metrics.areaUnderPR)

    return metrics.areaUnderROC
```

###### step2. 模型训练 与 评估

```python
from time import time               # 用于统计运行时间
def train_evaluate_model(train_data, validation_data, 
                         param_impurity, param_depth, param_bins):
    start_time = time()             # 模型训练开始时间

    # 训练模型
    model = DecisionTree.trainClassifier(train_rdd, 2, {}, 
            impurity=param_impurity, maxDepth=param_depth, maxBins=param_bins)

    # 模型验证评估
    auc = evaluate_mode(model, validation_data)

    # 计算花费时间
    duration = time() - start_time
    print ("训练评估使用参数：impurity = " + str(param_impurity) + \
        ", maxDepth = " + str(param_depth) + \
        ", maxBins = " + str(param_bins) + \
        " => 所需时间 = " + str(duration) + ", 评估AUC = " + str(auc))

    return (auc, duration, param_impurity, param_depth, param_bins, model)
```

###### step3 不同超参数组合，训练与评估，找到最佳模型

```python
def train_evaluate_params(train_datas, validation_datas, 
                          impurity_list, depth_list, bins_list):
    # 训练及评估返回值
    metrics_list = [ train_evaluate_model(train_rdd, validation_rdd, impurity, depth, bins)
                        for impurity in impurity_list
                        for depth in depth_list
                        for bins in bins_list
                    ]

    # 针对 auc值降序排序，找出最佳 模型
    sorted_metrics_list = sorted(metrics_list, 
                                 key=lambda k: k[0], reverse=True)

    # 获取最佳模型
    best_params = sorted_metrics_list[0]

    # 打印显示，最佳参数组合
    print("最佳参数组合: impurity -> " + str(best_params[2]) + \
             ", depth -> " + str(best_params[3]) + \
              ", bins -> " + str(best_params[4]) + \
              "\n AUC -> " + str(best_params[0])
         )

    return best_params[5]
```

```
print ('-------------------- 设置不同超参数的不同值进行训练评估 -------------------')
best_model = train_evaluate_params(train_rdd, validation_rdd,
              ['gini', 'entropy'], [5, 10, 15, 20, 25], [4, 8, 16, 32, 64])
```

```php
#使用 测试数据集检验：最佳模型是否过拟合
auc_metric = evaluate_mode(best_model, test_rdd)
print ('基于测试数据集评估AUC = ', auc_metric)
```

###### step4 保存模型 加载模型-使用

```bash
best_model.save(sc, path='./datas/dtc-best-model')  #sparkSession,保存路径
```

```python
from pyspark.mllib.tree import DecisionTreeModel 

load_dtc_model = DecisionTreeModel.load(sc, path='./datas/dtc-best-model')

load_dtc_model
help(load_dtc_model.predict)
```



