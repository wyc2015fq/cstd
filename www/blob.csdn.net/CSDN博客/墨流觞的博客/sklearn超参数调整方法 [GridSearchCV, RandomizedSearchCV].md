# sklearn超参数调整方法  [GridSearchCV, RandomizedSearchCV] - 墨流觞的博客 - CSDN博客





2018年09月27日 14:12:03[墨氲](https://me.csdn.net/dss_dssssd)阅读数：625








模型调整， 假设已经找到了一些潜在的模型，下面是几种方法用于模型调整

### 1. 超参数修改
- 网格搜索 (grid searh)

一种方法是手动调整超参数(hyperparameters)。

GridSearchCV,参数为你想调整的超参数和该超参数的值。
`class sklearn.model_selection.GridSearchCV(estimator, param_grid, scoring=None, fit_params=None, n_jobs=None, iid=’warn’, refit=True, cv=’warn’, verbose=0, pre_dispatch=‘2*n_jobs’, error_score=’raise-deprecating’, return_train_score=’warn’)`

参数：- estimator sklearn实现的estimator接口，需要提供score函数，或者有scoring参数
- param_id: 字典或字典的列表

[{需要调整的超参数1： 超参数的可能的值}, {{需要调整的超参数2： 超参数的可能的值}}, ], 注意一个字典内为一组超参数的组合方式
- scoring: 误差函数
- cv  : 将训练集分为多少个folds

属性：
- 
best_params

- 
cv_results_


```python
```python
from sklearn.model_selection import GridSearchCV
param_grid = [
{'n_estimators': [3, 10, 30], 'max_features': [2, 4, 6, 8]},
{'bootstrap': [False], 'n_estimators': [3, 10], 'max_features': [2, 3, 4]},
]

forest_reg = RandomForestRegressor()
grid_search = GridSearchCV(forest_reg, param_grid, cv=5,
                          scoring='neg_mean_squared_error')

grid_search.fit(housing_prepared, housing_labels)
```
```

sklearn 根据param_grid的值，首先会评估$3 \times 4 = 12$种n_estimators和max_features的组合方式，接下来在会在bootstrap=False的情况下（默认该值为True），评估$2 \times3  =6$种12种n_estimators和max_features的组合方式，所以最终会有$12+6=18$种不同的超参数组合方式,

而每一种组合方式要在训练集上训练5次， 所以一共要训练$18 \times 5 = 90$次，当训练结束后，你可以通过best_params_获得最好的组合方式

```python
```python
grid_search.best_params_
```
```

out:

> 
{‘max_features’: 8, ‘n_estimators’: 30}


得到最好的模型

```python
```python
grid_search.best_estimator_
```
```

out:

> 
RandomForestRegressor(bootstrap=True, criterion=‘mse’, max_depth=None,

max_features=8, max_leaf_nodes=None, min_impurity_decrease=0.0,

min_impurity_split=None, min_samples_leaf=1,

min_samples_split=2, min_weight_fraction_leaf=0.0,

n_estimators=30, n_jobs=1, oob_score=False, random_state=None,

verbose=0, warm_start=False)
如果GridSearchCV初始化时，refit=True（默认的初始化值）,在交叉验证时，一旦发现最好的模型（estimator）,将会在整个训练集上重新训练，这通常是一个好主意，因为使用更多的数据集会提升模型的性能。

**cv_results_**：将结果存在一个字典里, 可以转化为DataFrame类型，每一行为一种超参数组合方式。

```python
```python
cv = pd.DataFrame(grid_search.cv_results_
cv

# out:



	mean_fit_time	std_fit_time	mean_score_time	std_score_time	param_max_features	param_n_estimators	param_bootstrap	params	split0_test_score	split1_test_score	...	mean_test_score	std_test_score	rank_test_score	split0_train_score	split1_train_score	split2_train_score	split3_train_score	split4_train_score	mean_train_score	std_train_score
0	0.085940	0.025568	0.004391	0.001827	2	3	NaN	{'max_features': 2, 'n_estimators': 3}	-3.827812e+09	-4.092971e+09	...	-4.139394e+09	1.959892e+08	18	-1.108742e+09	-1.076285e+09	-1.151262e+09	-1.127172e+09	-1.114365e+09	-1.115565e+09	2.449443e+07
1	0.237965	0.021394	0.010079	0.000971	2	10	NaN	{'max_features': 2, 'n_estimators': 10}	-2.742609e+09	-3.333789e+09	...	-3.113304e+09	2.395274e+08	11	-5.701092e+08	-6.094081e+08	-5.785905e+08	-5.931616e+08	-5.756676e+08	-5.853874e+08	1.422342e+07
2	0.767676	0.092776	0.029941	0.004852	2	30	NaN	{'max_features': 2, 'n_estimators': 30}	-2.715244e+09	-2.902911e+09	...	-2.802893e+09	1.542347e+08	8	-4.472685e+08	-4.271909e+08	-4.250249e+08	-4.289724e+08	-4.274656e+08	-4.311845e+08	8.140110e+06
3	0.107399	0.002467	0.003590	0.000482	4	3	NaN	{'max_features': 4, 'n_estimators': 3}	-3.698121e+09	-3.903447e+09	...	-3.666869e+09	2.123558e+08	15	-9.640156e+08	-9.257596e+08	-1.003782e+09	-9.539664e+08	-8.817442e+08	-9.458535e+08	4.065737e+07
4	0.348854	0.003953	0.009683	0.000399	4	10	NaN	{'max_features': 4, 'n_estimators': 10}	-2.721300e+09	-2.868056e+09	...	-2.788953e+09	1.090418e+08	7	-5.299723e+08	-4.985359e+08	-4.892567e+08	-5.204567e+08	-5.260378e+08	-5.128519e+08	1.604050e+07
5	1.058535	0.007244	0.027150	0.000750	4	30	NaN	{'max_features': 4, 'n_estimators': 30}	-2.412966e+09	-2.617706e+09	...	-2.575291e+09	1.220214e+08	3	-4.002692e+08	-3.978664e+08	-3.819384e+08	-4.050417e+08	-3.929495e+08	-3.956131e+08	7.870790e+06
6	0.157606	0.014043	0.003394	0.000200	6	3	NaN	{'max_features': 6, 'n_estimators': 3}	-3.292618e+09	-3.698856e+09	...	-3.535907e+09	1.405519e+08	14	-8.680883e+08	-9.112255e+08	-9.112524e+08	-1.004211e+09	-9.125849e+08	-9.214724e+08	4.468549e+07
7	0.491094	0.019871	0.009585	0.000372	6	10	NaN	{'max_features': 6, 'n_estimators': 10}	-2.553328e+09	-2.791239e+09	...	-2.750653e+09	1.162331e+08	6	-4.944491e+08	-4.779433e+08	-4.857806e+08	-5.040733e+08	-5.053104e+08	-4.935113e+08	1.052407e+07
8	1.446224	0.011595	0.027342	0.000976	6	30	NaN	{'max_features': 6, 'n_estimators': 30}	-2.358673e+09	-2.551963e+09	...	-2.516006e+09	1.406579e+08	2	-3.919785e+08	-3.910130e+08	-3.912702e+08	-3.954620e+08	-3.815968e+08	-3.902641e+08	4.618501e+06
9	0.184060	0.003479	0.003398	0.000202	8	3	NaN	{'max_features': 8, 'n_estimators': 3}	-3.214309e+09	-3.749000e+09	...	-3.492941e+09	2.259341e+08	13	-8.754845e+08	-9.164619e+08	-9.071570e+08	-9.245905e+08	-8.557108e+08	-8.958809e+08	2.609451e+07
10	0.613070	0.003497	0.009779	0.000676	8	10	NaN	{'max_features': 8, 'n_estimators': 10}	-2.524796e+09	-2.749792e+09	...	-2.695701e+09	1.184566e+08	4	-4.814605e+08	-4.924878e+08	-4.953451e+08	-5.081368e+08	-5.055688e+08	-4.965998e+08	9.604441e+06
11	1.843982	0.014717	0.026444	0.000943	8	30	NaN	{'max_features': 8, 'n_estimators': 30}	-2.375197e+09	-2.517777e+09	...	-2.506430e+09	1.330733e+08	1	-3.884794e+08	-3.844676e+08	-3.707626e+08	-3.920550e+08	-3.907572e+08	-3.853043e+08	7.714260e+06
12	0.105807	0.003103	0.003793	0.000399	2	3	False	{'bootstrap': False, 'max_features': 2, 'n_est...	-3.771914e+09	-3.740538e+09	...	-3.750444e+09	9.489691e+07	17	-0.000000e+00	-0.000000e+00	-0.000000e+00	-0.000000e+00	-0.000000e+00	0.000000e+00	0.000000e+00
13	0.347154	0.003689	0.010282	0.000509	2	10	False	{'bootstrap': False, 'max_features': 2, 'n_est...	-2.674186e+09	-2.990817e+09	...	-2.920952e+09	1.338905e+08	10	-4.739042e+02	-1.947960e+03	-1.514005e+02	-0.000000e+00	-7.418622e-01	-5.148012e+02	7.371504e+02
14	0.137443	0.003622	0.003394	0.000200	3	3	False	{'bootstrap': False, 'max_features': 3, 'n_est...	-3.262696e+09	-3.673369e+09	...	-3.667646e+09	3.201594e+08	16	-0.000000e+00	-0.000000e+00	-0.000000e+00	-0.000000e+00	-0.000000e+00	0.000000e+00	0.000000e+00
15	0.458953	0.010759	0.010680	0.000400	3	10	False	{'bootstrap': False, 'max_features': 3, 'n_est...	-2.672303e+09	-2.797555e+09	...	-2.819182e+09	1.591897e+08	9	-0.000000e+00	-0.000000e+00	-0.000000e+00	-0.000000e+00	-0.000000e+00	0.000000e+00	0.000000e+00
16	0.170678	0.001511	0.004197	0.000249	4	3	False	{'bootstrap': False, 'max_features': 4, 'n_est...	-3.360936e+09	-3.505000e+09	...	-3.424422e+09	1.686945e+08	12	-0.000000e+00	-0.000000e+00	-0.000000e+00	-0.000000e+00	-0.000000e+00	0.000000e+00	0.000000e+00
17	0.566040	0.004567	0.011083	0.000581	4	10	False	{'bootstrap': False, 'max_features': 4, 'n_est...	-2.507133e+09	-2.778602e+09	...	-2.701759e+09	1.294327e+08	5	-0.000000e+00	-0.000000e+00	-0.000000e+00	-0.000000e+00	-0.000000e+00	0.000000e+00	0.000000e+00
18 rows × 23 columns
```
```

可以看出，一共有18行，代表18中参数的组合方式。

将超参数参数的组合方式和想应的误差值

```python
```python
for mean_score, params in zip(cv["mean_test_score"], cv["params"]):
    print(np.sqrt(-mean_score), params)

# out:
64338.12255034114 {'max_features': 2, 'n_estimators': 3}
55796.98682826028 {'max_features': 2, 'n_estimators': 10}
52942.353620944974 {'max_features': 2, 'n_estimators': 30}
60554.68102018701 {'max_features': 4, 'n_estimators': 3}
52810.54319153441 {'max_features': 4, 'n_estimators': 10}
50747.320348497095 {'max_features': 4, 'n_estimators': 30}
59463.49112391854 {'max_features': 6, 'n_estimators': 3}
52446.67106233131 {'max_features': 6, 'n_estimators': 10}
50159.80848511047 {'max_features': 6, 'n_estimators': 30}
59101.107722861816 {'max_features': 8, 'n_estimators': 3}
51920.13895373909 {'max_features': 8, 'n_estimators': 10}
50064.25380671872 {'max_features': 8, 'n_estimators': 30}
61240.86657596463 {'bootstrap': False, 'max_features': 2, 'n_estimators': 3}
54045.83360004432 {'bootstrap': False, 'max_features': 2, 'n_estimators': 10}
60561.09106204302 {'bootstrap': False, 'max_features': 3, 'n_estimators': 3}
53095.965054469176 {'bootstrap': False, 'max_features': 3, 'n_estimators': 10}
58518.56200119758 {'bootstrap': False, 'max_features': 4, 'n_estimators': 3}
51978.44739222989 {'bootstrap': False, 'max_features': 4, 'n_estimators': 10}
```
```

最好的是’max_features’: 8, ‘n_estimators’: 30， 误差为50064，结果要好于默认的超参数，误差为52634
- RandomizedSearchCV

当要探索相对较少的几个超参数组合时，GridSearchCV已经可以了，但当要搜索的空间（比如有很多超参数）很大时，使用RandomizedSearchCV更好。RandomizedSearchCV的使用方法其实是和GridSearchCV一致的，但它以随机在参数空间中采样的方式代替了GridSearchCV对于参数的网格搜索，在对于有连续变量的参数时，RandomizedSearchCV会将其当作一个分布进行采样这是网格搜索做不到的，它的搜索能力取决于设定的n_iter参数.

[http://scikit-learn.org/stable/modules/generated/sklearn.model_selection.RandomizedSearchCV.html](http://scikit-learn.org/stable/modules/generated/sklearn.model_selection.RandomizedSearchCV.html)

### 2. 模型组合
- ensemble methods,

另一种调整模型的方法是将表现好的模型组合起来。比如随机森林就是讲决策树组合起来。






