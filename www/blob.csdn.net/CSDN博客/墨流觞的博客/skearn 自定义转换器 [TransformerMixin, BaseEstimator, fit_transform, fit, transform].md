# skearn 自定义转换器 [TransformerMixin, BaseEstimator, fit_transform, fit, transform] - 墨流觞的博客 - CSDN博客





2018年09月23日 21:13:52[墨氲](https://me.csdn.net/dss_dssssd)阅读数：838








自定义转换函器：

创建一个类，实现fit()[return self]、transform()和fit_transform(),

如果使用TransformerMixin作为基类，则自动实现fit_transform()函数，fit_transform() <==> fit().transform(),

如果添加BaseEstimator作为基类，,注意此时`__init__`函数不能接受 $*args$ 和 $**kwargs$，还可以使用两个额外的方法（get_params()和set_params()）,

这两个函数对于自动超参数调优有用处，下面是个列子，结合两个已经存在的属性作为一个新的属性值。
np.c_的用法[https://blog.csdn.net/dss_dssssd/article/details/82824588](https://blog.csdn.net/dss_dssssd/article/details/82824588)

```python
```python
from sklearn.base import BaseEstimator, TransformerMixin
# xxx_ix 表示第几列
room_ix, bedroom_ix, population_ix, household_ix = 3, 4, 5, 6
housing[:5]

# out:
     	longitude	latitude	housing_median_age	total_rooms	total_bedrooms	population	households	median_income	ocean_proximity
17606 	-121.89	     37.29	         38.0	          1568.0	   351.0	       710.0	  339.0  	2.7042  	<1H OCEAN
18632	-121.93	     37.05	         14.0	          679.0	      108.0	           306.0	  113.0	     6.4214	<1H OCEAN
14650	 -117.20	      32.77	         31.0	         1952.0	  471.0         	936.0	  462.0   	2.8621	    NEAR OCEAN
3230	 -119.61	     36.31	         25.0	         1847.0	   371.0	       1460.0	   353.0	1.8839   	INLAND
3555	-118.59	     34.23      	17.0	             6592.0	  1525.0	       4459.0	  1463.0	 3.0347	 <1H OCEAN


# transform类实现
class CombinedAttributesAdder(BaseEstimator, TransformerMixin):
    def __init__(self, add_bedrooms_per_room=True): # no *args and **kwargs
        # 是否添加每间屋子拥有卧室数量
        self.add_bedrooms_per_room = add_bedrooms_per_room
        super().__init__()
    
    def fit(self, X, y=None):
        return self  # nothing else to do
    
    def transform(self, X, y=None):
        # 每个家庭拥有的房子数量
        rooms_per_household = X[:, room_ix] / X[:, household_ix]
        population_per_household = X[:, population_ix] / X[:, household_ix]
        
        # 是否添加平均每间房子拥有的卧室数量
        if self.add_bedrooms_per_room:
            bedrooms_per_room = X[:, bedroom_ix] / X[:, room_ix]
            # 向二维数组添加新的列，作为新的属性值
            return np.c_[X, rooms_per_household, population_per_household, bedrooms_per_room]
        else:
            return np.c_[X, rooms_per_household, population_per_household]

# 添加新的属性
attr_adder = CombinedAttributesAdder(add_bedrooms_per_room=False)
housing_extra_attribs = attr_adder.transform(housing.values)
# 转化为DataFrame对象
pd.DataFrame(housing_extra_attribs[:5])
       0	 1	     2	      3	  4    	5	  6   7 	8      9	      10
0	-121.89	37.29	38	1568	351	710	339	2.7042	<1H OCEAN	4.62537	2.0944
1	-121.93	37.05	14	679	108	306	113	6.4214	<1H OCEAN	6.00885	2.70796
2	-117.2	32.77	31	1952	471	936	462	2.8621	NEAR OCEAN	4.22511	2.02597
3	-119.61	36.31	25	1847	371	1460	353	1.8839	INLAND	5.23229	4.13598
4	-118.59	34.23	17	6592	1525	4459	1463	3.0347	<1H OCEAN	4.50581	3.04785

# 使用fit_transform
housing_extra_attribs = attr_adder.fit_transform(housing.values)
pd.DataFrame(housing_extra_attribs[:5])
    0	 1	     2	      3	  4    	5	  6   7 	8      9	      10
0	-121.89	37.29	38	1568	351	710	339	2.7042	<1H OCEAN	4.62537	2.0944
1	-121.93	37.05	14	679	108	306	113	6.4214	<1H OCEAN	6.00885	2.70796
2	-117.2	32.77	31	1952	471	936	462	2.8621	NEAR OCEAN	4.22511	2.02597
3	-119.61	36.31	25	1847	371	1460	353	1.8839	INLAND	5.23229	4.13598
4	-118.59	34.23	17	6592	1525	4459	1463	3.0347	<1H OCEAN	4.50581	3.04785
```
```




