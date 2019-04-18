# pyspark_mllib_classifier—(LR) - wsp_1138886114的博客 - CSDN博客





2018年09月21日 22:32:41[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：95











### 文章目录
- [LogisticRegression 二分类](#LogisticRegression__1)
- [step1. 构建训练函数](#step1__8)
- [step2. 模型训练 与 评估](#step2____21)
- [step3 不同超参数组合，训练与评估，找到最佳模型](#step3__41)




### LogisticRegression 二分类
- 前期数据处理（[https://blog.csdn.net/wsp_1138886114/article/details/81318204）](https://blog.csdn.net/wsp_1138886114/article/details/81318204%EF%BC%89)

一、创建pysparkSession运行环境（略）

二、获取数据集并格式化数据（略）

三、特征工程—提取特征（略）
##### step1. 构建训练函数

```python
```python
from pyspark.mllib.evaluation import BinaryClassificationMetrics
from time import time

def evaluate_mode(model, validation_data):                   # 方便对模型评估，定义函数
    score = model.predict(validation_data\
                          .map(lambda lp: lp.features))\
                          .map(lambda x :float(x))
    score_and_label = score.zip(validation_data.map(lambda lp: lp.label))
    metrics = BinaryClassificationMetrics(score_and_label)   # 使用BinaryClassificationMetrics 计算AUC面积
    return metrics.areaUnderROC
```
```

##### step2. 模型训练 与 评估

```python
```python
def train_evaluate_model(train_data, iteration_num, regType_num,validation_datas):
    start_time = time()                                    # 模型训练开始时间
    
    # 训练模型
    LR_model = LogisticRegressionWithLBFGS\
               .train(train_rdd, iterations=iteration_num,regType= regType_num ) 
    
    # 模型验证评估
    auc = evaluate_mode(LR_model, validation_datas)
    
    # 计算花费时间
    duration = time() - start_time
    print ("训练评估使用参数：iterations = " + str(iteration_num) + \
        ", regType = " + str(regType_num) + \
        " => 所需时间 = " + str(duration) + ", 评估AUC = " + str(auc))

    return (auc, duration, iteration_num, regType_num,LR_model)
```
```

##### step3 不同超参数组合，训练与评估，找到最佳模型

```python
```python
# 编写函数，可以对任意的参数调整进行训练和评估
def train_evaluate_params(train_datas,  
                           iteration_num_list, regType_num_list,validation_datas):
    # 训练及评估返回值
    metrics_list = [ train_evaluate_model(train_rdd, iteration_num, regType_num, validation_datas)
                        for iteration_num in iteration_num_list
                        for regType_num in regType_num_list
                    ]
    
    # 针对 auc值降序排序，找出最佳模型；获取最佳模型
    sorted_metrics_list = sorted(metrics_list, key=lambda k: k[0], reverse=True)
    best_params = sorted_metrics_list[0]
    
    # 打印显示，最佳参数组合
    print("最佳参数组合: iteration_num -> " + str(best_params[2]) + \
             ", regType_num -> " + str(best_params[3]) + \
              "\n AUC -> " + str(best_params[0])
         )
    return best_params[4]        # 返回模型
```
```

```python
```python
print ('-------------------- 设置不同超参数的不同值进行训练评估 -------------------')
best_model = train_evaluate_params(train_rdd,
              [50, 100], ['l1','l2'], validation_rdd)
```
```




