
# pandas 数据集的端到端处理 - Zhang's Wikipedia - CSDN博客


2018年04月26日 21:53:47[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：220



## 1. 数据集基本信息
df = pd.read_csv()
df.head()：前五行；
[df.info](http://df.info)()：
rangeindex：行索引；
data columns：列索引；
dtypes：各个列的类型，
主体部分是各个列值的情况，比如可判断是否存在 NaN 值；
对于非数值型的属性列
df[‘some_categorical_columns’].value_counts()：取值分布；
df.describe()： 各个列的基本统计信息
count
mean
std
min/max
25%, 50%, 75%：分位数
df.hist(bins=50, figsize=(20, 15))：统计直方图；
对 df 的每一列进行展示：
train_prices = pd.DataFrame({'price': train_df.SalePrice, 
			     'log(price+1)': np.log1p(train_df.SalePrice)})
	\# train_prices 共两列，一列列名为 price，一列列名为 log(price+1)
train_prices.hist()
## 2. 数据集拆分
`def split_train_test(data, test_ratio=.3):
	shuffled_indices = np.random.permutation(len(data))
	test_size = int(len(data)*test_ratio)
	test_indices = shuffled_indices[:test_size]
	train_indices = shuffled_indices[test_size:]
	return data.iloc[train_indices], data.iloc[test_indices]`
## 3. 数据预处理
一键把 categorical 型特征（字符串类型）转化为数值型：
>> df['label'] = pd.Categorical(df['label']).codes
一键把 categorical 型特征（字符串类型）转化为 one-hot 编码：
>> df = pd.get_dummies(df)
null 值统计与填充：
>> df.isnull().sum().sort_values(ascending=False).head()
\# 填充为 mean 值
>> mean_cols = df.mean()
>> df = df.fillna(mean_cols)
>> df.isnull().sum().sum()
0

