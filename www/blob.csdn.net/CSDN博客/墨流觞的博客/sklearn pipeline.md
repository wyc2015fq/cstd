# sklearn  pipeline - 墨流觞的博客 - CSDN博客





2018年09月25日 17:15:36[墨氲](https://me.csdn.net/dss_dssssd)阅读数：1371








众所周知，有很多数据转换步骤需要按照正确的步骤执行，sklearn提供了Pipeline类来处理这种顺序的操作步骤

```python
```python
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
num_pipeline = Pipeline([
	('imputer', Imputer(strategy="median")),
	('attribs_adder', CombinedAttributesAdder()),
	('std_scaler', StandardScaler()),
])
housing_num_tr = num_pipeline.fit_transform(housing_num)
```
```

Pipeline构造器接受(name, transform) tuple的列表作为参数。按顺序执行列表中的transform，完成数据预处理

**注意：**
- 除了最后一个tansform，其余的transform必须实现fit_transform函数
- 上一个transform类中fit_transform函数的返回值作为下一个transform类fit_transform函数的参数，所以在自己实现自定义的transform类的时候必须要实现fit_transform函数
- fit_transform 返回值为 numpy array

所以 上述代码的处理步骤：

先在housing_num中利用Imputer处理缺失值，然后将返回值传给CombinedAttributesAdder，添加一些特征属性，接下来再将返回值传给StandardScaler，完成标准化。完成数据预处理

上述代码中：
- 关于自定义transform CombinedAttributesAdder在此文章中有详细说明[https://blog.csdn.net/dss_dssssd/article/details/82824979](https://blog.csdn.net/dss_dssssd/article/details/82824979)
- 关于 StandardScaler， 在此文章中有详细说明[https://blog.csdn.net/dss_dssssd/article/details/82834425](https://blog.csdn.net/dss_dssssd/article/details/82834425)
- 关于Imputer，在此文章中有详细说明[https://blog.csdn.net/dss_dssssd/article/details/82831240](https://blog.csdn.net/dss_dssssd/article/details/82831240)



