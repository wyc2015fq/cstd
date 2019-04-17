# Xgboost原理、代码、调参和上线实录 - Hxyue2018's Blog - CSDN博客





2018年12月01日 10:56:20[Hxyue2018](https://me.csdn.net/Super_Json)阅读数：74








对于一个算法工程师而言，xgboost应该算的上是起手式，网上也有各式各样的教程，这篇博客主要从原理、代码、调参和上线进行覆盖，进而构建一个直观的算法体系；







生成的二叉树是满二叉树还是完全二叉树？



调参方法

param = {

        # step size

        'eta': 0.1,

        # model param, the weight value of the number of leaves, larger -> under fitting

        # 'gamma': 0.1,

        'max_depth': depth,

        # pruning param, min instance weight in hessian needed in a child; larger -> under fitting

        # 'min_child_weight': 1,

        # pruning param, update constraint, larger -> under fitting

        # 'max_delta_step': 0,

        'subsample': 0.8,

        # column sample ratio each tree

        # 'colsample_bytree': 0.8,

        # column sample ratio each layer

        'colsample_bylevel': 0.3,

        # L2 regularization term

        # 'lambda': 1,

        # L1 regularization term

        'alpha': 0.1,

        # small data set -> exact, large data set -> approx, just choose auto

        # 'tree_method': 'auto',

        # 'sketch_eps': 0.03,

        # for unbalanced data set, pos: value, neg: 1

        # 'scale_pos_weight': 1 / weight,

        # "reg:linear" –linear regression

        # "reg:logistic" –logistic regression

        # "binary:logistic" –logistic regression for binary classification, output probability

        # "binary:logitraw" –logistic regression for binary classification, output score before logistic transformation

        # "count:poisson" –poisson regression for count data, output mean of poisson distribution, max_delta_step is set to 0.7 by default in poisson regression (used to safeguard optimization)

        # "multi:softmax" –set XGBoost to do multiclass classification using the softmax objective, you also need to set num_class(number of classes)

        # "multi:softprob" –same as softmax, but output a vector of ndata * nclass, which can be further reshaped to ndata, nclass matrix. The result contains predicted probability of each data point belonging to each class.

        # "rank:pairwise" –set XGBoost to do ranking task by minimizing the pairwise loss

        # "reg:gamma" –gamma regression for severity data, output mean of gamma distribution

        'objective': 'binary:logistic',

        # threshold

        'base_score': weight / (weight + 1),

        # "rmse": root mean square error

        # "mae": mean absolute error

        # "logloss": negative log-likelihood

        # "error": Binary classification error rate. It is calculated as # (wrong cases)/# (all cases). For the predictions, the evaluation will regard the instances with prediction value larger than 0.5 as positive instances, and the others as negative instances.

        # "merror": Multiclass classification error rate. It is calculated as # (wrong cases)/# (all cases).

        # "mlogloss": Multiclass logloss

        # "auc": Area under the curve for ranking evaluation.

        # "ndcg":Normalized Discounted Cumulative Gain

        # "map":Mean average precision

        # "ndcg@n","map@n": n can be assigned as an integer to cut off the top positions in the lists for evaluation.

        # "ndcg-","map-","ndcg@n-","map@n-": In XGBoost, NDCG and MAP will evaluate the score of a list without any positive samples as 1. By adding "-" in the evaluation metric XGBoost will evaluate these score as 0 to be consistent under some conditions. training repeatedly

        # "gamma-deviance": [residual deviance for gamma regression]

        'eval_metric': 'auc',

        'seed': 31,

    }



上线实录：

在实际生产过程中，难免遇到将模型上线的问题，这里将流程进行拆解；

1.模型训练完成—保存模型文件—解析模型文件—重写代码读取解析文件预测树；



Xgb的损失函数的作用；



Java解析源码，具体的代码和注释已经更新在github上，





