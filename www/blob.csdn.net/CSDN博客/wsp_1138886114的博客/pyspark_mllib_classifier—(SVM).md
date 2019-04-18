# pyspark_mllib_classifier—(SVM) - wsp_1138886114的博客 - CSDN博客





2018年09月21日 22:30:49[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：127











### 文章目录
- [SVM 二分类](#SVM__8)
- [step1. 构建训练函数](#step1__9)
- [step2. 模型训练 与 评估](#step2____31)
- [step3 不同超参数组合，训练与评估，找到最佳模型](#step3__46)



开发环境 jupyter notebook
- 本项目基于：pyspark_mllib_classifier—(DecisionTree) 处理好的数据集
1、创建pysparkSession运行环境(略)

2、获取数据集并格式化数据(略)

3、特征工程—提取特征(略)
### SVM 二分类

##### step1. 构建训练函数

```python
```python
from pyspark.mllib.classification import SVMWithSGD
def train_evaluate_model(train_data, iteration_num, step_num, miniBatch_num, validation_datas):
    start_time = time()                              # 模型训练开始时间
    
    # 训练模型: help(SVMWithSGD)
    SVM_model = SVMWithSGD.train(train_rdd, iterations=iteration_num, 
                                 step=step_num, regParam=0.01,
                                 miniBatchFraction=miniBatch_num, regType='l2')
    
    auc = evaluate_mode(SVM_model, validation_datas)  # 模型验证评估
    
    duration = time() - start_time                    # 计算花费时间
    print ("训练评估使用参数：iterations = " + str(iteration_num) + \
        ", step = " + str(step_num) + \
        ", miniBatchFraction = " + str(miniBatch_num) + \
        " => 所需时间 = " + str(duration) + ", 评估AUC = " + str(auc))

    return (auc, duration, iteration_num, step_num, miniBatch_num, SVM_model)
```
```

##### step2. 模型训练 与 评估

```python
```python
from pyspark.mllib.evaluation import BinaryClassificationMetrics   
from time import time                                  # 用于统计运行时间

def evaluate_mode(model, validation_data):             # 方便对模型评估，定义函数
    score = model.predict(validation_data\
                         .map(lambda lp: lp.features))\
                         .map(lambda x :float(x))
    score_and_label = score.zip(validation_data.map(lambda lp: lp.label))
    metrics = BinaryClassificationMetrics(score_and_label)   
    # 使用BinaryClassificationMetrics 计算AUC面积
    return metrics.areaUnderROC
```
```

##### step3 不同超参数组合，训练与评估，找到最佳模型

```python
```python
def train_evaluate_params(train_datas,iteration_num_list, 
                          step_num_list, miniBatch_num_list,validation_datas):
    # 训练及评估返回值
    metrics_list = [ train_evaluate_model(train_rdd, iteration_num, step_num, miniBatch_num,validation_datas)
                        for iteration_num in iteration_num_list
                        for step_num in step_num_list
                        for miniBatch_num in miniBatch_num_list
                    ]
    
    # 针对 auc值降序排序，找出最佳模型；获取最佳模型
    sorted_metrics_list = sorted(metrics_list, key=lambda k: k[0], reverse=True)
    best_params = sorted_metrics_list[0]
    
    # 打印显示，最佳参数组合
    print("最佳参数组合: iteration_num -> " + str(best_params[2]) + \
             ", step_num -> " + str(best_params[3]) + \
              ", miniBatch_num -> " + str(best_params[4]) + \
              "\n AUC -> " + str(best_params[0])
         )
    return best_params[5]        # 返回模型
```
```

```python
```python
print ('-------------------- 设置不同超参数的不同值进行训练评估 -------------------')
best_model = train_evaluate_params(train_rdd,
              [50, 100], [0.1,1,2,5], [0.01,0.1], validation_rdd)
```
```



