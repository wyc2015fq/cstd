
# Pandas基础命令速查表 - leofionn的博客 - CSDN博客


2018年05月24日 19:33:22[leofionn](https://me.csdn.net/qq_36142114)阅读数：239


本文翻译整理自[Pandas Cheat Sheet - Python for Data Science](https://www.dataquest.io/blog/pandas-cheat-sheet/),结合K-Lab的工具属性，添加了具体的内容将速查清单里面的代码实践了一遍。
### 已更新至最新，欢迎点击右上角的Fork按钮上手实践
### 更多内容请关注知乎专栏
### [Data Trek](https://www.zhihu.com/people/bao-yixiong/columns)
速查表内容概要点击右上角的Fork按钮上手实践,即可点击标题实现内容跳转
[缩写解释 & 库的导入]
[数据的导入]
[数据的导出]
[创建测试对象]
[数据的查看与检查]
[数据的选取]
[数据的清洗]
[数据的过滤(filter),排序(sort)和分组(groupby)]
[数据的连接(join)与组合(combine)]
[数据的统计]


缩写解释 & 库的导入

`df`--- 任意的`pandas DataFrame`(数据框)对象
`s`--- 任意的`pandas Series`(数组)对象
`pandas`和`numpy`是用Python做数据分析最基础且最核心的库

In [1]:
importpandasaspd\# 导入pandas库并简写为pdimportnumpyasnp\# 导入numpy库并简写为np



数据的导入

pd.read_csv(filename)\# 导入csv格式文件中的数据pd.read_table(filename)\# 导入有分隔符的文本 (如TSV) 中的数据pd.read_excel(filename)\# 导入Excel格式文件中的数据pd.read_sql(query,connection_object)\# 导入SQL数据表/数据库中的数据pd.read_json(json_string)\# 导入JSON格式的字符，URL地址或者文件中的数据pd.read_html(url)\# 导入经过解析的URL地址中包含的数据框 (DataFrame) 数据pd.read_clipboard()\# 导入系统粘贴板里面的数据pd.DataFrame(dict)\# 导入Python字典 (dict) 里面的数据，其中key是数据框的表头，value是数据框的内容。


数据的导出

df.to_csv(filename)\# 将数据框 (DataFrame)中的数据导入csv格式的文件中df.to_excel(filename)\# 将数据框 (DataFrame)中的数据导入Excel格式的文件中df.to_sql(table_name,connection_object)\# 将数据框 (DataFrame)中的数据导入SQL数据表/数据库中df.to_json(filename)\# 将数据框 (DataFrame)中的数据导入JSON格式的文件中


创建测试对象

pd.DataFrame(np.random.rand(10,5))\# 创建一个5列10行的由随机浮点数组成的数据框 DataFrame

In [2]:
pd.DataFrame(np.random.rand(10,5))

Out[2]:
|0|1|2|3|4|
|---|---|---|---|---|---|
|0|0.647736|0.372628|0.255864|0.853542|0.613267|
|1|0.064364|0.156340|0.575021|0.561911|0.479901|
|2|0.036473|0.876819|0.255325|0.393240|0.543039|
|3|0.357489|0.006578|0.093966|0.531294|0.029009|
|4|0.550582|0.504600|0.273546|0.011693|0.052523|
|5|0.721563|0.170689|0.702163|0.447883|0.905983|
|6|0.839726|0.935997|0.343133|0.356957|0.377116|
|7|0.931894|0.026684|0.719148|0.911425|0.676187|
|8|0.115619|0.114894|0.130696|0.321598|0.170082|
|9|0.194649|0.526141|0.965442|0.275433|0.880765|



pd.Series(my_list)\# 从一个可迭代的对象 my_list 中创建一个数据组

In [3]:
my_list=['Kesci',100,'欢迎来到科赛网']pd.Series(my_list)

Out[3]:
0      Kesci
1        100
2    欢迎来到科赛网
dtype: object


df.index=pd.date_range('2017/1/1',periods=df.shape[0])\# 添加一个日期索引 index

In [4]:
df=pd.DataFrame(np.random.rand(10,5))df.index=pd.date_range('2017/1/1',periods=df.shape[0])df

Out[4]:
|0|1|2|3|4|
|---|---|---|---|---|---|
|2017-01-01|0.248515|0.647889|0.111346|0.540434|0.159914|
|2017-01-02|0.445073|0.329843|0.823678|0.737438|0.707598|
|2017-01-03|0.526543|0.876826|0.717986|0.271920|0.719657|
|2017-01-04|0.471256|0.657647|0.973484|0.598997|0.249301|
|2017-01-05|0.958465|0.474331|0.004078|0.842343|0.819295|
|2017-01-06|0.271308|0.271988|0.434776|0.449652|0.369188|
|2017-01-07|0.989573|0.928428|0.452436|0.058590|0.732283|
|2017-01-08|0.435328|0.730214|0.909400|0.683413|0.186820|
|2017-01-09|0.897414|0.687525|0.122937|0.018102|0.440427|
|2017-01-10|0.743821|0.134602|0.210326|0.877157|0.815462|



数据的查看与检查

df.head(n)\# 查看数据框的前n行

In [5]:
df=pd.DataFrame(np.random.rand(10,5))df.head(3)

Out[5]:
|0|1|2|3|4|
|---|---|---|---|---|---|
|0|0.705884|0.845813|0.770585|0.481049|0.381055|
|1|0.733309|0.542363|0.264334|0.254283|0.859442|
|2|0.497977|0.474898|0.806073|0.384412|0.242989|



df.tail(n)\# 查看数据框的最后n行

In [6]:
df=pd.DataFrame(np.random.rand(10,5))df.tail(3)

Out[6]:
|0|1|2|3|4|
|---|---|---|---|---|---|
|7|0.617289|0.009801|0.220155|0.992743|0.944472|
|8|0.261141|0.940925|0.063394|0.052104|0.517853|
|9|0.634541|0.897483|0.748453|0.805861|0.344938|



df.shape\# 查看数据框的行数与列数

In [7]:
df=pd.DataFrame(np.random.rand(10,5))df.shape

Out[7]:
(10, 5)


df.info()\# 查看数据框 (DataFrame) 的索引、数据类型及内存信息

In [8]:
df=pd.DataFrame(np.random.rand(10,5))df.info()


<class 'pandas.core.frame.DataFrame'>
RangeIndex: 10 entries, 0 to 9
Data columns (total 5 columns):
0    10 non-null float64
1    10 non-null float64
2    10 non-null float64
3    10 non-null float64
4    10 non-null float64
dtypes: float64(5)
memory usage: 480.0 bytes


df.describe()\# 对于数据类型为数值型的列，查询其描述性统计的内容

In [9]:
df.describe()

Out[9]:
|0|1|2|3|4|
|---|---|---|---|---|---|
|count|10.000000|10.000000|10.000000|10.000000|10.000000|
|mean|0.410631|0.497585|0.506200|0.322960|0.603119|
|std|0.280330|0.322573|0.254780|0.260299|0.256370|
|min|0.043731|0.031742|0.070668|0.044822|0.143786|
|25%|0.240661|0.211625|0.416827|0.145298|0.422969|
|50%|0.346297|0.544697|0.479648|0.217359|0.635974|
|75%|0.493105|0.669044|0.557353|0.468119|0.782573|
|max|0.937583|0.945573|0.987328|0.883157|0.992891|



s.value_counts(dropna=False)\# 查询每个独特数据值出现次数统计

In [10]:
s=pd.Series([1,2,3,3,4,np.nan,5,5,5,6,7])s.value_counts(dropna=False)

Out[10]:
5.0    3
 3.0    2
 7.0    1
 6.0    1
NaN     1
 4.0    1
 2.0    1
 1.0    1
dtype: int64


df.apply(pd.Series.value_counts)\# 查询数据框 (Data Frame) 中每个列的独特数据值出现次数统计


数据的选取

df[col]\# 以数组 Series 的形式返回选取的列

In [11]:
df=pd.DataFrame(np.random.rand(5,5),columns=list('ABCDE'))df['C']

Out[11]:
0    0.720965
1    0.360155
2    0.474067
3    0.116206
4    0.774503
Name: C, dtype: float64


df[[col1,col2]]\# 以新的数据框(DataFrame)的形式返回选取的列

In [12]:
df=pd.DataFrame(np.random.rand(5,5),columns=list('ABCDE'))df[['B','E']]

Out[12]:
|B|E|
|---|---|---|
|0|0.205912|0.333909|
|1|0.475620|0.540206|
|2|0.144041|0.065117|
|3|0.636970|0.406317|
|4|0.451541|0.944245|



s.iloc[0]\# 按照位置选取

In [13]:
s=pd.Series(np.array(['I','Love','Data']))s.iloc[0]

Out[13]:
'I'


s.loc['index_one']\# 按照索引选取

In [14]:
s=pd.Series(np.array(['I','Love','Data']))s.loc[1]

Out[14]:
'Love'


df.iloc[0,:]\# 选取第一行

In [15]:
df=pd.DataFrame(np.random.rand(5,5),columns=list('ABCDE'))df.iloc[0,:]

Out[15]:
A    0.234156
B    0.513754
C    0.593067
D    0.856575
E    0.291528
Name: 0, dtype: float64


df.iloc[0,0]\# 选取第一行的第一个元素

In [16]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.iloc[0,0]

Out[16]:
0.91525996455410763



数据的清洗

df.columns=['a','b']\# 重命名数据框的列名称

In [17]:
df=pd.DataFrame({'A':np.array([1,np.nan,2,3,6,np.nan]),'B':np.array([np.nan,4,np.nan,5,9,np.nan]),'C':'foo'})df.columns=['a','b','c']df

Out[17]:
|a|b|c|
|---|---|---|---|
|0|1.0|NaN|foo|
|1|NaN|4.0|foo|
|2|2.0|NaN|foo|
|3|3.0|5.0|foo|
|4|6.0|9.0|foo|
|5|NaN|NaN|foo|



pd.isnull()\# 检查数据中空值出现的情况，并返回一个由布尔值(True,Fale)组成的列

In [18]:
df=pd.DataFrame({'A':np.array([1,np.nan,2,3,6,np.nan]),'B':np.array([np.nan,4,np.nan,5,9,np.nan]),'C':'foo'})pd.isnull(df)

Out[18]:
|A|B|C|
|---|---|---|---|
|0|False|True|False|
|1|True|False|False|
|2|False|True|False|
|3|False|False|False|
|4|False|False|False|
|5|True|True|False|



pd.notnull()\# 检查数据中非空值出现的情况，并返回一个由布尔值(True,False)组成的列

In [19]:
df=pd.DataFrame({'A':np.array([1,np.nan,2,3,6,np.nan]),'B':np.array([np.nan,4,np.nan,5,9,np.nan]),'C':'foo'})pd.notnull(df)

Out[19]:
|A|B|C|
|---|---|---|---|
|0|True|False|True|
|1|False|True|True|
|2|True|False|True|
|3|True|True|True|
|4|True|True|True|
|5|False|False|True|



df.dropna()\# 移除数据框 DataFrame 中包含空值的行

In [20]:
df=pd.DataFrame({'A':np.array([1,np.nan,2,3,6,np.nan]),'B':np.array([np.nan,4,np.nan,5,9,np.nan]),'C':'foo'})df.dropna()

Out[20]:
|A|B|C|
|---|---|---|---|
|3|3.0|5.0|foo|
|4|6.0|9.0|foo|



df.dropna(axis=1)\# 移除数据框 DataFrame 中包含空值的列

In [21]:
df=pd.DataFrame({'A':np.array([1,np.nan,2,3,6,np.nan]),'B':np.array([np.nan,4,np.nan,5,9,np.nan]),'C':'foo'})df.dropna(axis=1)

Out[21]:
|C|
|---|---|
|0|foo|
|1|foo|
|2|foo|
|3|foo|
|4|foo|
|5|foo|



df.dropna(axis=0,thresh=n)\# 移除数据框df中空值个数大于等于n的行

In [22]:
df=pd.DataFrame({'A':np.array([1,np.nan,2,3,6,np.nan]),'B':np.array([np.nan,4,np.nan,5,9,np.nan]),'C':'foo'})test=df.dropna(axis=1,thresh=1)test

Out[22]:
|A|B|C|
|---|---|---|---|
|0|1.0|NaN|foo|
|1|NaN|4.0|foo|
|2|2.0|NaN|foo|
|3|3.0|5.0|foo|
|4|6.0|9.0|foo|
|5|NaN|NaN|foo|



df.fillna(x)\# 将数据框 DataFrame 中的所有空值替换为 x

In [23]:
df=pd.DataFrame({'A':np.array([1,np.nan,2,3,6,np.nan]),'B':np.array([np.nan,4,np.nan,5,9,np.nan]),'C':'foo'})df.fillna('Test')

Out[23]:
|A|B|C|
|---|---|---|---|
|0|1|Test|foo|
|1|Test|4|foo|
|2|2|Test|foo|
|3|3|5|foo|
|4|6|9|foo|
|5|Test|Test|foo|



`s.fillna(s.mean())`-> 将所有空值替换为平均值

In [24]:
s=pd.Series([1,3,5,np.nan,7,9,9])s.fillna(s.mean())

Out[24]:
0    1.000000
1    3.000000
2    5.000000
3    5.666667
4    7.000000
5    9.000000
6    9.000000
dtype: float64


s.astype(float)\# 将数组(Series)的格式转化为浮点数

In [25]:
s=pd.Series([1,3,5,np.nan,7,9,9])s.astype(float)

Out[25]:
0    1.0
1    3.0
2    5.0
3    NaN
4    7.0
5    9.0
6    9.0
dtype: float64


s.replace(1,'one')\# 将数组(Series)中的所有1替换为'one'

In [26]:
s=pd.Series([1,3,5,np.nan,7,9,9])s.replace(1,'one')

Out[26]:
0    one
1      3
2      5
3    NaN
4      7
5      9
6      9
dtype: object


s.replace([1,3],['one','three'])\# 将数组(Series)中所有的1替换为'one', 所有的3替换为'three'

In [27]:
s=pd.Series([1,3,5,np.nan,7,9,9])s.replace([1,3],['one','three'])

Out[27]:
0      one
1    three
2        5
3      NaN
4        7
5        9
6        9
dtype: object


df.rename(columns=lambdax:x+2)\# 将全体列重命名

In [28]:
df=pd.DataFrame(np.random.rand(4,4))df.rename(columns=lambdax:x+2)

Out[28]:
|2|3|4|5|
|---|---|---|---|---|
|0|0.753588|0.137984|0.022013|0.900072|
|1|0.947073|0.815182|0.769708|0.729688|
|2|0.334815|0.204315|0.707794|0.437704|
|3|0.467212|0.738360|0.853463|0.529946|



df.rename(columns={'old_name':'new_ name'})\# 将选择的列重命名

In [29]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.rename(columns={'A':'newA','C':'newC'})

Out[29]:
|newA|B|newC|D|E|
|---|---|---|---|---|---|
|0|0.169072|0.694563|0.069313|0.637560|0.475181|
|1|0.910271|0.800067|0.676448|0.934767|0.025608|
|2|0.825186|0.451545|0.135421|0.635303|0.419758|
|3|0.401979|0.510304|0.014901|0.209211|0.121889|
|4|0.579282|0.001947|0.036519|0.750415|0.453078|
|5|0.896213|0.557514|0.028147|0.527471|0.575772|
|6|0.443222|0.095459|0.319582|0.912069|0.781455|
|7|0.067923|0.590470|0.602999|0.507358|0.703022|
|8|0.301491|0.682629|0.283103|0.565754|0.089268|
|9|0.399671|0.925416|0.020578|0.278000|0.591522|



df.set_index('column_one')\# 改变索引

In [30]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.set_index('B')

Out[30]:
|A|C|D|E|
|B|
|---|---|---|---|---|
|0.311742|0.972069|0.557977|0.114267|0.795128|
|0.931644|0.725425|0.082130|0.993764|0.136923|
|0.206382|0.980647|0.947041|0.038841|0.879139|
|0.157801|0.402233|0.249151|0.724130|0.108238|
|0.314238|0.341221|0.512180|0.218882|0.046379|
|0.029040|0.470619|0.666784|0.036655|0.823498|
|0.843928|0.779437|0.926912|0.189213|0.624111|
|0.282773|0.993681|0.048483|0.135934|0.576662|
|0.759600|0.235513|0.359139|0.488255|0.669043|
|0.088552|0.893269|0.277296|0.889523|0.398392|



df.rename(index=lambdax:x+1)\# 改变全体索引

In [31]:
df=pd.DataFrame(np.random.rand(10,5))df.rename(index=lambdax:x+1)

Out[31]:
|0|1|2|3|4|
|---|---|---|---|---|---|
|1|0.386542|0.031932|0.963200|0.790339|0.602533|
|2|0.053492|0.652174|0.889465|0.465296|0.843528|
|3|0.411836|0.460788|0.110352|0.083247|0.389855|
|4|0.336156|0.830522|0.560991|0.667896|0.233841|
|5|0.307933|0.995207|0.506680|0.957895|0.636461|
|6|0.724975|0.842118|0.123139|0.244357|0.803936|
|7|0.059176|0.117784|0.330192|0.418764|0.464144|
|8|0.104323|0.222367|0.930414|0.659232|0.562155|
|9|0.484089|0.024045|0.879834|0.492231|0.949636|
|10|0.201583|0.280658|0.356804|0.890706|0.236174|



数据的过滤(```filter```),排序(```sort```)和分组(```groupby```)

df[df[col]>0.5]\# 选取数据框df中对应行的数值大于0.5的全部列

In [32]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df[df['A']>0.5]

Out[32]:
|A|B|C|D|E|
|---|---|---|---|---|---|
|0|0.534886|0.863546|0.236718|0.326766|0.415460|
|2|0.953931|0.070198|0.483749|0.922528|0.295505|
|8|0.880175|0.056811|0.520499|0.533152|0.548145|



df[(df[col]>0.5)&(df[col]<0.7)]\# 选取数据框df中对应行的数值大于0.5，并且小于0.7的全部列

In [33]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df[(df['C']>0.5)&(df['D']<0.7)]

Out[33]:
|A|B|C|D|E|
|---|---|---|---|---|---|
|2|0.953112|0.174517|0.645300|0.308216|0.171177|
|6|0.853087|0.863079|0.701823|0.354019|0.311754|



df.sort_values(col1)\# 按照数据框的列col1升序(ascending)的方式对数据框df做排序

In [34]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.sort_values('E')

Out[34]:
|A|B|C|D|E|
|---|---|---|---|---|---|
|3|0.024096|0.623842|0.775949|0.828343|0.317729|
|6|0.220055|0.381614|0.463676|0.762644|0.391758|
|4|0.589411|0.727439|0.064528|0.319521|0.413518|
|1|0.878490|0.229301|0.699506|0.726879|0.464106|
|8|0.438101|0.970649|0.050256|0.697440|0.499057|
|9|0.566100|0.558798|0.723253|0.254244|0.524486|
|7|0.613603|0.933109|0.677036|0.808160|0.544953|
|5|0.079326|0.711673|0.266434|0.910628|0.816783|
|2|0.132114|0.145395|0.908436|0.521271|0.889645|
|0|0.432677|0.216837|0.203532|0.093214|0.977671|



df.sort_values(col2,ascending=False)\# 按照数据框的列col2降序(descending)的方式对数据框df做排序

In [35]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.sort_values('A',ascending=False)

Out[35]:
|A|B|C|D|E|
|---|---|---|---|---|---|
|9|0.977172|0.930607|0.889285|0.475032|0.031715|
|0|0.864511|0.229990|0.678612|0.042491|0.148123|
|2|0.694747|0.580891|0.817524|0.392417|0.055003|
|6|0.684327|0.802028|0.862043|0.241838|0.800401|
|7|0.612324|0.099445|0.714120|0.215054|0.280343|
|8|0.441434|0.315553|0.564762|0.800143|0.330030|
|1|0.438734|0.161109|0.610750|0.647330|0.792404|
|4|0.365880|0.710768|0.344320|0.998757|0.979497|
|3|0.202511|0.769728|0.575057|0.511384|0.696753|
|5|0.029527|0.560114|0.224787|0.086291|0.318322|



df.sort_values([col1,col2],ascending=[True,False])\# 按照数据框的列col1升序，col2降序的方式对数据框df做排序

In [36]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.sort_values(['A','E'],ascending=[True,False])

Out[36]:
|A|B|C|D|E|
|---|---|---|---|---|---|
|6|0.075863|0.696980|0.648945|0.336977|0.113122|
|2|0.199316|0.632063|0.787358|0.133175|0.060568|
|5|0.242081|0.818550|0.618439|0.215761|0.924459|
|7|0.261237|0.400725|0.659224|0.555746|0.132572|
|0|0.390540|0.358432|0.754028|0.194403|0.889624|
|8|0.410481|0.463811|0.343021|0.736340|0.291121|
|4|0.578705|0.544711|0.881707|0.396593|0.414465|
|3|0.600541|0.459247|0.591303|0.027464|0.496864|
|9|0.720029|0.419921|0.740225|0.904391|0.226958|
|1|0.777955|0.992290|0.144495|0.600207|0.647018|



df.groupby(col)\# 按照某列对数据框df做分组

In [37]:
df=pd.DataFrame({'A':np.array(['foo','foo','foo','foo','bar','bar']),'B':np.array(['one','one','two','two','three','three']),'C':np.array(['small','medium','large','large','small','small']),'D':np.array([1,2,2,3,3,5])})df.groupby('A').count()

Out[37]:
|B|C|D|
|A|
|---|---|---|---|
|bar|2|2|2|
|foo|4|4|4|



df.groupby([col1,col2])\# 按照列col1和col2对数据框df做分组

In [38]:
df=pd.DataFrame({'A':np.array(['foo','foo','foo','foo','bar','bar']),'B':np.array(['one','one','two','two','three','three']),'C':np.array(['small','medium','large','large','small','small']),'D':np.array([1,2,2,3,3,5])})df.groupby(['B','C']).sum()

Out[38]:
|D|
|B|C|
|---|---|---|
|one|medium|2|
|small|1|
|three|small|8|
|two|large|5|



df.groupby(col1)[col2].mean()\# 按照列col1对数据框df做分组处理后，返回对应的col2的平均值

In [39]:
df=pd.DataFrame({'A':np.array(['foo','foo','foo','foo','bar','bar']),'B':np.array(['one','one','two','two','three','three']),'C':np.array(['small','medium','large','large','small','small']),'D':np.array([1,2,2,3,3,5])})df.groupby('B')['D'].mean()

Out[39]:
B
one      1.5
three    4.0
two      2.5
Name: D, dtype: float64


`pythyon
df.pivot_table(index=col1,values=[col2,col3],aggfunc=mean) # 做透视表，索引为col1,针对的数值列为col2和col3，分组函数为平均值`

In [40]:
df=pd.DataFrame({'A':np.array(['foo','foo','foo','foo','bar','bar']),'B':np.array(['one','one','two','two','three','three']),'C':np.array(['small','medium','large','large','small','small']),'D':np.array([1,2,2,3,3,5])})df.pivot_table(df,index=['A','B'],columns=['C'],aggfunc=np.sum)

Out[40]:
|D|
|C|large|medium|small|
|A|B|
|---|---|---|---|---|
|bar|three|NaN|NaN|8.0|
|foo|one|NaN|2.0|1.0|
|two|5.0|NaN|NaN|



df.groupby(col1).agg(np.mean)

In [41]:
df=pd.DataFrame({'A':np.array(['foo','foo','foo','foo','bar','bar']),'B':np.array(['one','one','two','two','three','three']),'C':np.array(['small','medium','large','large','small','small']),'D':np.array([1,2,2,3,3,5])})df.groupby('A').agg(np.mean)

Out[41]:
|D|
|A|
|---|---|
|bar|4|
|foo|2|



df.apply(np.mean)\# 对数据框df的每一列求平均值

In [42]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.apply(np.mean)

Out[42]:
A    0.388075
B    0.539564
C    0.607983
D    0.518634
E    0.482960
dtype: float64


df.apply(np.max,axis=1)\# 对数据框df的每一行求最大值

In [43]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.apply(np.max,axis=1)

Out[43]:
0    0.904163
1    0.804519
2    0.924102
3    0.761781
4    0.952084
5    0.923679
6    0.796320
7    0.582907
8    0.761310
9    0.893564
dtype: float64



数据的连接(```join```)与组合(```combine```)

df1.append(df2)\# 在数据框df2的末尾添加数据框df1，其中df1和df2的列数应该相等

In [44]:
df1=pd.DataFrame({'A':['A0','A1','A2','A3'],'B':['B0','B1','B2','B3'],'C':['C0','C1','C2','C3'],'D':['D0','D1','D2','D3']},index=[0,1,2,3])df2=pd.DataFrame({'A':['A4','A5','A6','A7'],'B':['B4','B5','B6','B7'],'C':['C4','C5','C6','C7'],'D':['D4','D5','D6','D7']},index=[4,5,6,7])df1.append(df2)

Out[44]:
|A|B|C|D|
|---|---|---|---|---|
|0|A0|B0|C0|D0|
|1|A1|B1|C1|D1|
|2|A2|B2|C2|D2|
|3|A3|B3|C3|D3|
|4|A4|B4|C4|D4|
|5|A5|B5|C5|D5|
|6|A6|B6|C6|D6|
|7|A7|B7|C7|D7|



pd.concat([df1,df2],axis=1)\# 在数据框df1的列最后添加数据框df2,其中df1和df2的行数应该相等

In [45]:
df1=pd.DataFrame({'A':['A0','A1','A2','A3'],'B':['B0','B1','B2','B3'],'C':['C0','C1','C2','C3'],'D':['D0','D1','D2','D3']},index=[0,1,2,3])df2=pd.DataFrame({'A':['A4','A5','A6','A7'],'B':['B4','B5','B6','B7'],'C':['C4','C5','C6','C7'],'D':['D4','D5','D6','D7']},index=[4,5,6,7])pd.concat([df1,df2],axis=1)

Out[45]:
|A|B|C|D|A|B|C|D|
|---|---|---|---|---|---|---|---|---|
|0|A0|B0|C0|D0|NaN|NaN|NaN|NaN|
|1|A1|B1|C1|D1|NaN|NaN|NaN|NaN|
|2|A2|B2|C2|D2|NaN|NaN|NaN|NaN|
|3|A3|B3|C3|D3|NaN|NaN|NaN|NaN|
|4|NaN|NaN|NaN|NaN|A4|B4|C4|D4|
|5|NaN|NaN|NaN|NaN|A5|B5|C5|D5|
|6|NaN|NaN|NaN|NaN|A6|B6|C6|D6|
|7|NaN|NaN|NaN|NaN|A7|B7|C7|D7|



df1.join(df2,on=col1,how='inner')\# 对数据框df1和df2做内连接，其中连接的列为col1

In [46]:
df1=pd.DataFrame({'A':['A0','A1','A2','A3'],'B':['B0','B1','B2','B3'],'key':['K0','K1','K0','K1']})df2=pd.DataFrame({'C':['C0','C1'],'D':['D0','D1']},index=['K0','K1'])df1.join(df2,on='key')

Out[46]:
|A|B|key|C|D|
|---|---|---|---|---|---|
|0|A0|B0|K0|C0|D0|
|1|A1|B1|K1|C1|D1|
|2|A2|B2|K0|C0|D0|
|3|A3|B3|K1|C1|D1|



## <div id = 'p10'>数据的统计</div>


df.describe()\# 得到数据框df每一列的描述性统计

In [47]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.describe()

Out[47]:
|A|B|C|D|E|
|---|---|---|---|---|---|
|count|10.000000|10.000000|10.000000|10.000000|10.000000|
|mean|0.398648|0.451699|0.443472|0.739478|0.412954|
|std|0.330605|0.221586|0.303084|0.308798|0.262148|
|min|0.004457|0.188689|0.079697|0.113562|0.052935|
|25%|0.088177|0.270355|0.205663|0.715005|0.205685|
|50%|0.315533|0.457229|0.332148|0.885872|0.400232|
|75%|0.749716|0.497208|0.737900|0.948651|0.634670|
|max|0.782956|0.825671|0.851065|0.962922|0.815447|



df.mean()\# 得到数据框df中每一列的平均值

In [48]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.mean()

Out[48]:
A    0.395643
B    0.528812
C    0.692011
D    0.446750
E    0.544759
dtype: float64


df.corr()\# 得到数据框df中每一列与其他列的相关系数

In [49]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.corr()

Out[49]:
|A|B|C|D|E|
|---|---|---|---|---|---|
|A|1.000000|-0.634931|-0.354824|-0.354131|0.170957|
|B|-0.634931|1.000000|0.225222|-0.338124|-0.043300|
|C|-0.354824|0.225222|1.000000|0.098285|0.297133|
|D|-0.354131|-0.338124|0.098285|1.000000|-0.324209|
|E|0.170957|-0.043300|0.297133|-0.324209|1.000000|



df.count()\# 得到数据框df中每一列的非空值个数

In [50]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.count()

Out[50]:
A    10
B    10
C    10
D    10
E    10
dtype: int64


df.max()\# 得到数据框df中每一列的最大值

In [51]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.max()

Out[51]:
A    0.933848
B    0.730197
C    0.921751
D    0.715280
E    0.940010
dtype: float64


df.min()\# 得到数据框df中每一列的最小值

In [52]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.min()

Out[52]:
A    0.107516
B    0.001635
C    0.024502
D    0.092810
E    0.019898
dtype: float64


df.median()\# 得到数据框df中每一列的中位数

In [53]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.median()

Out[53]:
A    0.497591
B    0.359854
C    0.661607
D    0.342418
E    0.588468
dtype: float64


df.std()\# 得到数据框df中每一列的标准差

In [54]:
df=pd.DataFrame(np.random.rand(10,5),columns=list('ABCDE'))df.std()

Out[54]:
A    0.231075
B    0.286691
C    0.276511
D    0.304167
E    0.272570
dtype: float64



