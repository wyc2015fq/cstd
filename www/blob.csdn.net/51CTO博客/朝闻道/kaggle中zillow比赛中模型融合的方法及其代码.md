# kaggle中zillow比赛中模型融合的方法及其代码-朝闻道-51CTO博客
在机器学习这个领域，尤其是做多媒体（声音、图像、视频）相关的机器学习方法研究，会涉及很多特征、分类模型（分类任务）的选择。以声音识别为例，常见的特征有MFCC、LPCC、spectrogram-like features 等，分类模型就很多了，有传统的分类模型SVM、KNN、Random Forest，还有现在比较火的深度模型DNN、CNN、RNN等。而往往单特征、单模型很难取得理想的性能（performance）。那么，如何高效的利用不同的特征和模型？
一个重要的方法就是进行融合（fusion）。典型的fusion方法有early fusion和late fusion。顾名思义，early fusion就是在特征上（feature-level）进行融合，进行不同特征的连接（concatenate），输入到一个模型中进行训练；late fusion指的是在预测分数（score-level）上进行融合，做法就是训练多个模型，每个模型都会有一个预测评分，我们对所有模型的结果进行fusion，得到最后的预测结果。常见的late fusion方法有取分数的平均值（average）、最大值（maximum）、加权平均（weighted average），另外还有采用Logistics Regression的方法进行late fusion。总之，方法很多，可视情况采取。
Fusion是一个提高模型性能的很好的方法，在参加kaggle比赛或者平时做项目上都是一个很常用的方法，尤其是像kaggle比赛这种比赛性质的，基本每一位参赛者的结果都是进行fusion后的结果，这里，模型融合也可以叫做ensemble，理解意思就好。
```
#
# import libariry
#
import numpy as np
import pandas as pd
# data precession
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import KFold
# model
from xgboost import XGBRegressor
from lightgbm import LGBMRegressor
from sklearn.svm import SVR
from sklearn.ensemble import RandomForestRegressor, ExtraTreesRegressor, AdaBoostRegressor
from sklearn.linear_model import LinearRegression
from sklearn.tree import DecisionTreeRegressor
#
#
#version 29 -> LB:0.6446
#   add more feature
#
#version 28 -> LB:0.6445
#   model params 'n_estimators' -> 100
#
# version 26 -> LB:0.6443
#   model params 'n_estimators' -> 50
#
def load_data():
    train_2016 = pd.read_csv('../input/train_2016_v2.csv')
    train_2017 = pd.read_csv('../input/train_2017.csv')
    train = pd.concat([train_2016, train_2017], ignore_index=True)
    properties = pd.read_csv('../input/properties_2017.csv')
    sample = pd.read_csv('../input/sample_submission.csv')
    print("Preprocessing...")
    for c, dtype in zip(properties.columns, properties.dtypes):
        if dtype == np.float64:
            properties[c] = properties[c].astype(np.float32)
    print("Set train/test data...")
    id_feature = ['heatingorsystemtypeid','propertylandusetypeid', 'storytypeid', 'airconditioningtypeid',
        'architecturalstyletypeid', 'buildingclasstypeid', 'buildingqualitytypeid', 'typeconstructiontypeid']
    for c in properties.columns:
        properties[c]=properties[c].fillna(-1)
        if properties[c].dtype == 'object':
            lbl = LabelEncoder()
            lbl.fit(list(properties[c].values))
            properties[c] = lbl.transform(list(properties[c].values))
        if c in id_feature:
            lbl = LabelEncoder()
            lbl.fit(list(properties[c].values))
            properties[c] = lbl.transform(list(properties[c].values))
            dum_df = pd.get_dummies(properties[c])
            dum_df = dum_df.rename(columns=lambda x:c+str(x))
            properties = pd.concat([properties,dum_df],axis=1)
            properties = properties.drop([c], axis=1)
            #print np.get_dummies(properties[c])
    #
    # Add Feature
    #
    # error in calculation of the finished living area of home
    properties['N-LivingAreaError'] = properties['calculatedfinishedsquarefeet'] / properties[
        'finishedsquarefeet12']
    #
    # Make train and test dataframe
    #
    train = train.merge(properties, on='parcelid', how='left')
    sample['parcelid'] = sample['ParcelId']
    test = sample.merge(properties, on='parcelid', how='left')
    # drop out ouliers
    train = train[train.logerror > -0.4]
    train = train[train.logerror < 0.419]
    train["transactiondate"] = pd.to_datetime(train["transactiondate"])
    train["Month"] = train["transactiondate"].dt.month
    train["quarter"] = train["transactiondate"].dt.quarter
    test["Month"] = 10
    test['quarter'] = 4
    x_train = train.drop(['parcelid', 'logerror','transactiondate', 'propertyzoningdesc', 'propertycountylandusecode'], axis=1)
    y_train = train["logerror"].values
    x_test = test[x_train.columns]
    del test, train    
    print(x_train.shape, y_train.shape, x_test.shape)
    return x_train, y_train, x_test
x_train, y_train, x_test = load_data()
class Ensemble(object):
    def __init__(self, n_splits, stacker, base_models):
        self.n_splits = n_splits
        self.stacker = stacker
        self.base_models = base_models
    def fit_predict(self, X, y, T):
        X = np.array(X)
        y = np.array(y)
        T = np.array(T)
        folds = list(KFold(n_splits=self.n_splits, shuffle=True, random_state=2016).split(X, y))
        S_train = np.zeros((X.shape[0], len(self.base_models)))
        S_test = np.zeros((T.shape[0], len(self.base_models)))
        for i, clf in enumerate(self.base_models):
            S_test_i = np.zeros((T.shape[0], self.n_splits))
            for j, (train_idx, test_idx) in enumerate(folds):
                X_train = X[train_idx]
                y_train = y[train_idx]
                X_holdout = X[test_idx]
                y_holdout = y[test_idx]
                print ("Fit Model %d fold %d" % (i, j))
                clf.fit(X_train, y_train)
                y_pred = clf.predict(X_holdout)[:]                
                S_train[test_idx, i] = y_pred
                S_test_i[:, j] = clf.predict(T)[:]
            S_test[:, i] = S_test_i.mean(axis=1)
        # results = cross_val_score(self.stacker, S_train, y, cv=5, scoring='r2')
        # print("Stacker score: %.4f (%.4f)" % (results.mean(), results.std()))
        # exit()
        self.stacker.fit(S_train, y)
        res = self.stacker.predict(S_test)[:]
        return res
# rf params
rf_params = {}
rf_params['n_estimators'] = 50
rf_params['max_depth'] = 8
rf_params['min_samples_split'] = 100
rf_params['min_samples_leaf'] = 30
# xgb params
xgb_params = {}
#xgb_params['n_estimators'] = 50
xgb_params['min_child_weight'] = 12
xgb_params['learning_rate'] = 0.37
xgb_params['max_depth'] = 6
xgb_params['subsample'] = 0.77
xgb_params['reg_lambda'] = 0.8
xgb_params['reg_alpha'] = 0.4
xgb_params['base_score'] = 0
#xgb_params['seed'] = 400
xgb_params['silent'] = 1
# lgb params
lgb_params = {}
#lgb_params['n_estimators'] = 50
lgb_params['max_bin'] = 8
lgb_params['learning_rate'] = 0.37 # shrinkage_rate
lgb_params['metric'] = 'l1'          # or 'mae'
lgb_params['sub_feature'] = 0.35    
lgb_params['bagging_fraction'] = 0.85 # sub_row
lgb_params['bagging_freq'] = 40
lgb_params['num_leaves'] = 512        # num_leaf
lgb_params['min_data'] = 500         # min_data_in_leaf
lgb_params['min_hessian'] = 0.05     # min_sum_hessian_in_leaf
lgb_params['verbose'] = 0
lgb_params['feature_fraction_seed'] = 2
lgb_params['bagging_seed'] = 3
# XGB model
xgb_model = XGBRegressor(**xgb_params)
# lgb model
lgb_model = LGBMRegressor(**lgb_params)
# RF model
rf_model = RandomForestRegressor(**rf_params)
# ET model
et_model = ExtraTreesRegressor()
# SVR model
# SVM is too slow in more then 10000 set
#svr_model = SVR(kernel='rbf', C=1.0, epsilon=0.05)
# DecsionTree model
dt_model = DecisionTreeRegressor()
# AdaBoost model
ada_model = AdaBoostRegressor()
stack = Ensemble(n_splits=5,
        stacker=LinearRegression(),
        base_models=(rf_model, xgb_model, lgb_model, et_model, ada_model))
y_test = stack.fit_predict(x_train, y_train, x_test)
from datetime import datetime
print("submit...")
pre = y_test
sub = pd.read_csv('../input/sample_submission.csv')
for c in sub.columns[sub.columns != 'ParcelId']:
    sub[c] = pre
submit_file = '{}.csv'.format(datetime.now().strftime('%Y%m%d_%H_%M'))
sub.to_csv(submit_file, index=False,  float_format='%.4f')
```
