# sklearn 中 pipeline 或 LabelBinariy出现 'fit_transform() takes 2 positional arguments but 3 were given' - 墨流觞的博客 - CSDN博客





2018年09月25日 23:49:29[墨氲](https://me.csdn.net/dss_dssssd)阅读数：618








在学习OReilly.Hands-On.Machine.Learning.with.Scikit-Learn.and.TensorFlow.2017.3时，执行以下代码会出错：

```python
```python
from sklearn.pipeline import FeatureUnion
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
num_attribs = ['longitude',
 'latitude',
 'housing_median_age',
 'total_rooms',
 'total_bedrooms',
 'population',
 'households',
 'median_income']

cat_attribs = ["ocean_proximity"]
# 在sklearn 0.19下，由于LabelBinarizer的重写而导致代码失效
num_pipeline = Pipeline([
    ('selector', DataFrameSelector(num_attribs)),
    ('imputer', Imputer(strategy="median")),
    ('attribs_adder', CombinedAttributesAdder()),
    ('std_scaler', StandardScaler()),
])
cat_pipeline = Pipeline([
    ('selector', DataFrameSelector(cat_attribs)),
    ('label_binarizer', LabelBinarizer()),
])
full_pipeline = FeatureUnion(transformer_list=[
    ("num_pipeline", num_pipeline),
    ("cat_pipeline", cat_pipeline),
])

housing_prepared = full_pipeline.fit_transform(housing)
```
```

会报错`fit_transform() takes 2 positional arguments but 3 were given`

原因在于此行代码

```python
```python
cat_pipeline = Pipeline([
    ('selector', DataFrameSelector(cat_attribs)),
    ('label_binarizer', LabelBinarizer()),
])
```
```

sklearn0.19 重写了LabelBinarizer类中的fit_transform，只能接受两个参数，而看报错信息：

```python
```python
Xt, fit_params = self._fit(X, y, **fit_params)
    282         if hasattr(last_step, 'fit_transform'):
--> 283             return last_step.fit_transform(Xt, y, **fit_params)
    284         elif last_step is None:
    285             return Xt
```
```

箭头指向的行，传入fit_transform参数为三个`Xt, y, **fit_params`.

接下来是我修改的代码，单独处理文本和数值属性，然后利用numpy的concatenate合并两个数组。

```python
```python
def processed_data(data):
    # 获得数值和文本属性
    num_attribs = num_attribs = ['longitude',
                                'latitude',
                                'housing_median_age',
                                'total_rooms',
                                'total_bedrooms',
                                'population',
                                'households',
                                'median_income']
    
    cat_attribs = ["ocean_proximity"]
    
    # 处理数值属性
    num_pipeline = Pipeline([
    ('selector', DataFrameSelector(num_attribs)),
    ('imputer', Imputer(strategy="median")),
    ('attribs_adder', CombinedAttributesAdder()),
    ('std_scaler', StandardScaler()),
    ])
    
    num_pipeline_ = num_pipeline.fit_transform(data)
    
    # 处理文本和分类属性
    cat_pipeline =  LabelBinarizer() 
    cat_pipeline_ = cat_pipeline.fit_transform(data[cat_attribs])
    
    # 用numpy合并
    housing_prepared = np.concatenate((num_pipeline_, cat_pipeline_), axis=1)
    
    return housing_prepared
```
```



