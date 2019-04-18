# Pandas——练习题二 - wsp_1138886114的博客 - CSDN博客





2018年06月26日 14:52:36[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1128









- - [作业一：](#作业一)- - [Step 1.加载数据（datasets/users.csv）](#step-1加载数据datasetsuserscsv)
- [Step 2. 以occupation分组，求每一种职业所有用户的平均年龄](#step-2-以occupation分组求每一种职业所有用户的平均年龄)
- [Step 3. 求每一种职业男性的占比，作为新的一列(male_pct)添加到数据集中,并按照从低到高的顺序排列](#step-3-求每一种职业男性的占比作为新的一列malepct添加到数据集中并按照从低到高的顺序排列)
- [Step 4. 获取每一种职业对应的最大和最小的用户年龄](#step-4-获取每一种职业对应的最大和最小的用户年龄)


- [作业二：（数据过滤与排序）](#作业二数据过滤与排序)- - [Step 1. 导入数据并赋值给变量 chipo](#step-1-导入数据并赋值给变量-chipo)
- [Step 2. 计算出有多商品大于10美元（去除列数据中特殊字符）](#step-2-计算出有多商品大于10美元去除列数据中特殊字符)
- [Step 3. 每个项目的价格是多少？[指定列去重，筛选，排序]](#step-3-每个项目的价格是多少指定列去重筛选排序)
- [Step 4 根据商品的价格对数据进行排序](#step-4-根据商品的价格对数据进行排序)
- [Step 5.在所有商品订单中 最贵商品的数量（quantity）是多少？](#step-5在所有商品订单中-最贵商品的数量quantity是多少)
- [Step 6. 商品订购单中，商品 Veggie Salad Bowl 的订单数目？](#step-6-商品订购单中商品-veggie-salad-bowl-的订单数目)
- [Step 7. 在所有订单中，购买商品Canned Soda数量大于1的订单数有几条?](#step-7-在所有订单中购买商品canned-soda数量大于1的订单数有几条)






数据集下载页面:[https://download.csdn.net/download/wsp_1138886114/10563007](https://download.csdn.net/download/wsp_1138886114/10563007)

## 作业一：

```
import pandas as pd
import numpy as np
from pandas import DataFrame,Series
```

#### Step 1.加载数据（datasets/users.csv）

```
users = pd.read_csv("datasets/users.csv",sep = '|')
users
```
|$~$|**user_id**|**age**|**gender**|**occupation**|**zip_code**|
|----|----|----|----|----|----|
|**0**|1|24|M|technician|85711|
|**1**|2|53|F|other|94043|
|**2**|3|23|M|writer|32067|
|**3**|4|24|M|technician|43537|
|**4**|5|33|F|other|15213|
|**5**|6|42|M|executive|98101|
|**6**|7|57|M|administrator|91344|
|**7**|8|36|M|administrator|05201|
|**…**|…|…|…|…|…|
|**939**|940|32|M|administrator|02215|
|**940**|941|20|M|student|97229|
|**941**|942|48|F|librarian|78209|
|**942**|943|22|M|student|77841|

943 rows × 5 columns

#### Step 2. 以occupation分组，求每一种职业所有用户的平均年龄

```
#users.groupby('occupation').age.mean()                    #方法1
Ave_age = users["age"].groupby(users["occupation"]).mean() #方法2
Ave_age
```

```
# 输出：        所有职业的平均年龄
occupation
administrator    38.746835
artist           31.392857
doctor           43.571429
educator         42.010526
engineer         36.388060
entertainment    29.222222
...                ...
salesman         35.666667
scientist        35.548387
student          22.081633
technician       33.148148
writer           36.311111
Name: age, dtype: float64
```

#### Step 3. 求每一种职业男性的占比，作为新的一列(male_pct)添加到数据集中,并按照从低到高的顺序排列

```python
def gender_to_numeric(x):
    if x == 'M':
        return 1
    if x == 'F':
        return 0

# 调用函数对男性 M 计数
users['gender_n'] = users['gender'].apply(gender_to_numeric)

a = users.groupby('occupation').gender_n.sum() / users.occupation.value_counts() * 100 

# 降序排列
a.sort_values(ascending = False)
```

```
# 输出：      各种职业男性的占比：降序
doctor           100.000000
engineer          97.014925
technician        96.296296
retired           92.857143
programmer        90.909091
executive         90.625000
scientist         90.322581
entertainment     88.888889
lawyer            83.333333
salesman          75.000000
educator          72.631579
student           69.387755
other             65.714286
marketing         61.538462
writer            57.777778
none              55.555556
administrator     54.430380
artist            53.571429
librarian         43.137255
healthcare        31.250000
homemaker         14.285714
dtype: float64
```

#### Step 4. 获取每一种职业对应的最大和最小的用户年龄

```bash
users.groupby('occupation').age.agg(['min', 'max'])
```

```
# 输出    每一种职业对应的最小和最大的用户年龄    
               min  max
occupation      
administrator   21  70
artist          19  48
doctor          28  64
educator        23  63
engineer        22  70
...            ...  ...
retired         51  73
salesman        18  66
scientist       23  55
student         7   42
technician      21  55
writer          18  60
```

## 作业二：（数据过滤与排序）

```python
import pandas as pd
import numpy as np
from pandas import DataFrame,Series
```

#### Step 1. 导入数据并赋值给变量 chipo

```
chipo = pd.read_csv("./datasets/chipotle.csv",sep = "\t")
chipo 

#导入网络数据
#url = 'https://raw.githubusercontent.com/justmarkham/DAT8/master/data/chipotle.tsv'  
#chipo = pd.read_csv(url, sep = '\t')
```
|$~$|**order_id**|**quantity**|**item_name**|**choice_description**|**item_price**|
|----|----|----|----|----|----|
|**0**|1|1|Chips and Fresh Tomato Salsa|NaN|$2.39|
|**1**|1|1|Izze|[Clementine]|$3.39|
|**2**|1|1|Nantucket Nectar|[Apple]|$3.39|
|**3**|1|1|Chips and Tomatillo-Green Chili Salsa|NaN|$2.39|
|**4**|2|2|Chicken Bowl|[Tomatillo-Red Chili Salsa (Hot), [Black Beans…|$16.98|
|**5**|3|1|Chicken Bowl|[Fresh Tomato Salsa (Mild), [Rice, Cheese, Sou…|$10.98|
|**6**|3|1|Side of Chips|NaN|$1.69|
|**7**|4|1|Steak Burrito|[Tomatillo Red Chili Salsa, [Fajita Vegetables…|$11.75|
|**8**|4|1|Steak Soft Tacos|[Tomatillo Green Chili Salsa, [Pinto Beans, Ch…|$9.25|
|**9**|5|1|Steak Burrito|[Fresh Tomato Salsa, [Rice, Black Beans, Pinto…|$9.25|
|**10**|5|1|Chips and Guacamole|NaN|$4.45|
|**11**|6|1|Chicken Crispy Tacos|[Roasted Chili Corn Salsa, [Fajita Vegetables,…|$8.75|
|**12**|6|1|Chicken Soft Tacos|[Roasted Chili Corn Salsa, [Rice, Black Beans,…|$8.75|
|**13**|7|1|Chicken Bowl|[Fresh Tomato Salsa, [Fajita Vegetables, Rice,…|$11.25|
|**14**|7|1|Chips and Guacamole|NaN|$4.45|
|**15**|8|1|Chips and Tomatillo-Green Chili Salsa|NaN|$2.39|
|**16**|8|1|Chicken Burrito|[Tomatillo-Green Chili Salsa (Medium), [Pinto …|$8.49|
|**17**|9|1|Chicken Burrito|[Fresh Tomato Salsa (Mild), [Black Beans, Rice…|$8.49|
|**18**|9|2|Canned Soda|[Sprite]|$2.18|
|**19**|10|1|Chicken Bowl|[Tomatillo Red Chili Salsa, [Fajita Vegetables…|$8.75|
|**…**|…|…|…|…|…|
|**4617**|1833|1|Steak Burrito|[Fresh Tomato Salsa, [Rice, Black Beans, Sour …|$11.75|
|**4618**|1833|1|Steak Burrito|[Fresh Tomato Salsa, [Rice, Sour Cream, Cheese…|$11.75|
|**4619**|1834|1|Chicken Salad Bowl|[Fresh Tomato Salsa, [Fajita Vegetables, Pinto…|$11.25|
|**4620**|1834|1|Chicken Salad Bowl|[Fresh Tomato Salsa, [Fajita Vegetables, Lettu…|$8.75|
|**4621**|1834|1|Chicken Salad Bowl|[Fresh Tomato Salsa, [Fajita Vegetables, Pinto…|$8.75|

#### Step 2. 计算出有多商品大于10美元（去除列数据中特殊字符）

```
方法1：
#去除$符号并将object类型转成number类型
prices = [float(value[1 : -1]) for value in chipo.item_price]
chipo.item_price = prices 

# 筛选大于10美元的商品
chipo10 = chipo[chipo['item_price'] > 10.00]

#计数
len(chipo10)

◆ 输出：
1130
——————————————————————————————————————————————————————————————
方法2：
#去除$符号
chipo["item_price"] = chipo["item_price"].str.split('$').str[1]

#将object类型转成number类型，计数
chipo["item_price"] = pd.to_numeric(chipo["item_price"])
(chipo["item_price"]>10).value_counts()

◆ 输出：
False    3492
True     1130
Name: item_price, dtype: int64
```

#### Step 3. 每个项目的价格是多少？[指定列去重，筛选，排序]

输出一个只包含[ item_name ]和 [item_price]两列的dataframe

```
# 删除项目名称和数量中的重复项
chipo_filtered = chipo.drop_duplicates(['item_name','quantity'])

# 只选择数量等于1的产品
chipo_one_prod = chipo_filtered[chipo_filtered.quantity == 1]

# 只选择 item_name 和 item_price 价格列
price_per_item = chipo_one_prod[['item_name', 'item_price']]

# 以item_price列降序数据为标准对 price_per_item 整个数据进行排序
price_per_item.sort_values(by = "item_price", ascending = False)
```
|$~$|item_name|item_price|
|----|----|----|
|606|Steak Salad Bowl|11.89|
|1229|Barbacoa Salad Bowl|11.89|
|1132|Carnitas Salad Bowl|11.89|
|7|Steak Burrito|11.75|
|168|Barbacoa Crispy Tacos|11.75|
|39|Barbacoa Bowl|11.75|
|**…**|**…**|**…**|
|0|Chips and Fresh Tomato Salsa|2.39|
|40|Chips|2.15|
|6|Side of Chips|1.69|
|263|Canned Soft Drink|1.25|
|28|Canned Soda|1.09|
|34|Bottled Water|1.09|

#### Step 4 根据商品的价格对数据进行排序

```
方法1：
chipo.item_name.sort_values()

方法2：
chipo.sort_values(by = "item_name")
```

```
# 输出：    （...为省略部分）
3389    6 Pack Soft Drink
341     6 Pack Soft Drink
1849    6 Pack Soft Drink
              ...        
3141    6 Pack Soft Drink
639     6 Pack Soft Drink
1026    6 Pack Soft Drink
              ...        
2996         Veggie Salad
3163         Veggie Salad
              ...        
2756         Veggie Salad
4201    Veggie Salad Bowl
1884    Veggie Salad Bowl
              ...        
2683    Veggie Salad Bowl
496     Veggie Salad Bowl
4109    Veggie Salad Bowl
738     Veggie Soft Tacos
3889    Veggie Soft Tacos
              ...        
1699    Veggie Soft Tacos
1395    Veggie Soft Tacos
Name: item_name, dtype: object
```

#### Step 5.在所有商品订单中 最贵商品的数量（quantity）是多少？
`chipo.sort_values(by = "item_price", ascending = False).head(1)`|$~$|**order_id**|**quantity**|**item_name**|**choice_description**|**item_price**|
|----|----|----|----|----|----|
|**3598**|1443|15|Chips and Fresh Tomato Salsa|NaN|44.25|

#### Step 6. 商品订购单中，商品 Veggie Salad Bowl 的订单数目？

```
chipo_salad = chipo[chipo.item_name == "Veggie Salad Bowl"]
len(chipo_salad)
```

```
# 输出:
18
```

#### Step 7. 在所有订单中，购买商品Canned Soda数量大于1的订单数有几条?

```
chipo_drink_steak_bowl = chipo[(chipo.item_name == "Canned Soda")\&(chipo.quantity > 1)]
len(chipo_drink_steak_bowl)
```

```
# 输出:
20
```



