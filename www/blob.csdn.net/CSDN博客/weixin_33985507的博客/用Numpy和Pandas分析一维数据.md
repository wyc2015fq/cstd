# 用Numpy和Pandas分析一维数据 - weixin_33985507的博客 - CSDN博客
2018年02月13日 17:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 1.Gapminder 数据
female_completion_rate:
Primary completion rate is the percentage of female students completing the
last year of primary school. It is calculated by taking the total number of
female students in the last grade of primary school, minus the number of
repeaters in that grade, divided by the total number of children of
official graduation age. The ratio can exceed 100% due to over-aged and
under-aged children who enter primary school late/early and/or repeat
grades. United Nations Educational, Scientific, and Cultural Organization
(UNESCO) Institute for Statistics.
male_completion_rate:
Same as female_completion_rate, but for male students.
life_expectancy:
The average number of years a newborn child would live if current mortality
patterns were to stay the same.
gpd_per_capita:
Gross Domestic Product per capita in constant 2000 US$. The inflation but
not the differences in the cost of living between countries has been taken
into account.
employment_above_15:
Percentage of total population, age above 15, that has been employed during
the given year.
question i thought of:
- how has employment in the US varied over time
- what are the highest and lowest employment levels
--which countries have them?
--where is the us on the spectrum
- same questions for other variables
- how do these variables relate to each other?
- are there consistent trends across countries?
# 2 numpy和pandas中的一维数组
**pandas读取csv文件**
```
import pandas as pd
daily_engagement = pd.read_csv('daily-engagement.csv')
len(daily_engagement['acct'].unique())
```
# 3.numpy数组
one-dimensional data structures
pandas       |     numpy(numerical python)
series         |            built on      |      array
more features | simpler
numpy arrays and python lists
numpy array np.array(['AL','AK','AZ','AR','CA'])
similarities:
- access elements by position
a[0] → 'AL'
- access a range of elements
a[1:3] → np.array(['AK','AZ'])
*use loops
for x in a :
differences:
- each element should have same type(string,int,boolean,etc)
*convenient functions
mean(),std()
- can be multi-dimensional
```
import numpy as np
countries = np.array([
    'Afghanistan', 'Albania', 'Algeria', 'Angola', 'Argentina',
    'Armenia', 'Australia', 'Austria', 'Azerbaijan', 'Bahamas',
    'Bahrain', 'Bangladesh', 'Barbados', 'Belarus', 'Belgium',
    'Belize', 'Benin', 'Bhutan', 'Bolivia',
    'Bosnia and Herzegovina'
])
employment = np.array([
    55.70000076,  51.40000153,  50.5       ,  75.69999695,
    58.40000153,  40.09999847,  61.5       ,  57.09999847,
    60.90000153,  66.59999847,  60.40000153,  68.09999847,
    66.90000153,  53.40000153,  48.59999847,  56.79999924,
    71.59999847,  58.40000153,  70.40000153,  41.20000076
])
```
**索引**
```
print countries[0]
print countries[3]
print countries[0:3]
print countries[:3]
print countries[17:]
print countries[:]
```
**查看numpy数组元素的类型**
```
print countries.dtype           #s22:s表示string,22表示数组中最长的字符串有22个字母
print employment.dtype     #float64:f表示float，64表示它们是以64位的格式储存的
print np.array([0, 1, 2, 3]).dtype           #查看numpy数组元素的类型
print np.array([1.0, 1.5, 2.0, 2.5]).dtype
print np.array([True, False, True]).dtype
print np.array(['AL', 'AK', 'AZ', 'AR', 'CA']).dtype
```
**python字符串的format函数**
```
for country in countries:
    print 'Examining country {}'.format(country) #python字符串的format函数
for i in range(len(countries)):
    country = countries[i]
    country_employment = employment[i]
    print 'Country {} has employment {}'.format(country,
        country_employment)
```
**数组的统计量**
```
print employment.mean()
print employment.std()
print employment.max()
print employment.sum()
```
**最大的就业量**
```
def max_employment(countries,employment):
    max_position = employment.argmax()        #返回最大值的位置
    max_employment = employment[max_position]
    max_country = countries[max_position]
    return (max_country,max_employment)
max_employment(countries,employment)
```
# 4.向量化运算
vectorized operations
a vector is a list of number
vector 1:[1,2,3]
vector 2:[4,5,6]
adding 2 vectors is called vector addition
may be result:
- [5,7,9]
vector addition in linear algebra and numpy
*[1,2,3,4,5,6]
list concatenation in python
- error:you cant add vectors ,only numbers
very common
# 5.乘以标量
vector [1,2,3] * scalar 3 =
may be result:
- [1,2,3,1,2,3,1,2,3]
python
- [3,6,9]
linear algebra and numpy
*error :you cant multiply a vector by a single number
# 6.计算整体完成率
**more vectorized operations**
math operations:
add +
subtract -
multiple *
divide /
exponentiate **
logical operations:
and &
or |
not ~
make sure your arrays contain booleans!
否则就是：按位与，按位和，按位取反
comparison operations:
greater >
greater or equal >=
less <
less or equal <=
equal ==
not equal !=
**数组与数组之间的运算**
```
import numpy as np
a = np.array([1, 2, 3, 4])
b = np.array([1, 2, 1, 2])  
print a                   
print a + b             #[2 4 4 6]
print a - b              #[0 0 2 2]
print a * b              #[1 4 3 8]
print a / b              #[1 1 3 2]
print a ** b            #[ 1  4  3 16]
```
**数组与数字之间的运算**
```
a = np.array([1, 2, 3, 4])
b = 2  
print a + b         #[3 4 5 6]
print a - b         #[-1  0  1  2]
print a * b         #[2 4 6 8]
print a / b         #[0 1 1 2]
print a ** b       #[ 1  4  9 16]
```
**布尔数组之间的运算**
```
a = np.array([True, True, False, False])
b = np.array([True, False, True, False])
print a & b            #[ True False False False]
print a | b            #[ True  True  True False]
print ~a              #[False False  True  True]
```
**布尔数组与单个布尔值之间的运算**
```
print a & True           #[ True  True False False]
print a & False         # [False False False False]
print a | True            # [ True  True  True  True]
print a | False           #  [ True  True False False]
```
**将两个数组进行比较，就是返回布尔数组的向量运算**
```
a = np.array([1, 2, 3, 4, 5])
b = np.array([5, 4, 3, 2, 1])
print a > b      #[False False False  True  True]
print a >= b      #[False False  True  True  True]
print a < b      #[ True  True False False False]
print a <= b       #[ True  True  True False False]
print a == b       #[False False  True False False]
print a != b        #[ True  True False  True  True]
```
**将数组与单个数字进行比较，就是返回布尔数组的向量运算**
```
a = np.array([1, 2, 3, 4])
b = 2    
print a > b         #[False False  True  True]
print a >= b       #[False  True  True  True]
print a < b         #[ True False False False]
print a <= b      #[ True  True False False]
print a == b     #[False  True False False]
print a != b      #[ True False  True  True]
```
**平均受教育程度**
```
countries = np.array([
       'Algeria', 'Argentina', 'Armenia', 'Aruba', 'Austria','Azerbaijan',
       'Bahamas', 'Barbados', 'Belarus', 'Belgium', 'Belize', 'Bolivia',
       'Botswana', 'Brunei', 'Bulgaria', 'Burkina Faso', 'Burundi',
       'Cambodia', 'Cameroon', 'Cape Verde'
])
female_completion = np.array([
    97.35583,  104.62379,  103.02998,   95.14321,  103.69019,
    98.49185,  100.88828,   95.43974,   92.11484,   91.54804,
    95.98029,   98.22902,   96.12179,  119.28105,   97.84627,
    29.07386,   38.41644,   90.70509,   51.7478 ,   95.45072
])
male_completion = np.array([
     95.47622,  100.66476,   99.7926 ,   91.48936,  103.22096,
     97.80458,  103.81398,   88.11736,   93.55611,   87.76347,
    102.45714,   98.73953,   92.22388,  115.3892 ,   98.70502,
     37.00692,   45.39401,   91.22084,   62.42028,   90.66958
])
def overall_completion_rate(female_completion, male_completion):
    return (female_completion+male_completion)/2
overall_completion_rate(female_completion,male_completion)
```
# 7.归一化数据
standardizing data
how does one data point compare to the rest?
e.g. employment in US vs other countries
to answer, convert  each data point to number of standard  deviations away from the mean ,this is called standardizing the data
in 2007,
mean employment rate:58.6%
standard deviation : 10.5%
united states : 62.3%      diff:3.7% or 0.35sd
mexico : 57.9%           mexico diff: -0.7%   or -0.067sd
```
import numpy as np
countries = np.array([
    'Afghanistan', 'Albania', 'Algeria', 'Angola', 'Argentina',
    'Armenia', 'Australia', 'Austria', 'Azerbaijan', 'Bahamas',
    'Bahrain', 'Bangladesh', 'Barbados', 'Belarus', 'Belgium',
    'Belize', 'Benin', 'Bhutan', 'Bolivia',
    'Bosnia and Herzegovina'
])
employment = np.array([
    55.70000076,  51.40000153,  50.5       ,  75.69999695,
    58.40000153,  40.09999847,  61.5       ,  57.09999847,
    60.90000153,  66.59999847,  60.40000153,  68.09999847,
    66.90000153,  53.40000153,  48.59999847,  56.79999924,
    71.59999847,  58.40000153,  70.40000153,  41.20000076
])
def standardize_data(values):
    return (values-values.mean())/values.std()
```
# 8. numpy索引数组
numpy index arrays
suppose you have two arrays of the same length,and the second contains booleans
a = [1,2,3,4,5]
b= [F,F,T,T,T] index array:它告诉你应保留第一个数组的哪些元素
b=a>2
a[b] = [3,4,5]=a[a>2]
```
import numpy as np
a = np.array([1, 2, 3, 4])
b = np.array([True, True, False, False]) 
print a[b]            #[1,2]
print a[np.array([True, False, True, False])]      #[1,3]
a = np.array([1, 2, 3, 2, 1])
b = (a >= 2)                           #[False,True,True,True,False]
print a[b]                             
print a[a >= 2]                      #[2,3,2]
a = np.array([1, 2, 3, 4, 5])
b = np.array([1, 2, 3, 2, 1])
print b == 2                   [False  True False  True False]
print a[b == 2]           #[2 4]
```
write a function that takes in `time_spent` and `days_to_cancel` and returns mean time spent for students who stay at least 7 days
```
time_spent = np.array([
       12.89697233,    0.        ,   64.55043217,    0.        ,
       24.2315615 ,   39.991625  ,    0.        ,    0.        ,
      147.20683783,    0.        ,    0.        ,    0.        ,
       45.18261617,  157.60454283,  133.2434615 ,   52.85000767,
        0.        ,   54.9204785 ,   26.78142417,    0.
])
days_to_cancel = np.array([
      4,   5,  37,   3,  12,   4,  35,  38,   5,  37,   3,   3,  68,
     38,  98,   2, 249,   2, 127,  35
])
def mean_time_for_paid_students(time_spent, days_to_cancel):
    return (time_spent[days_to_cancel >= 7]).mean()
```
# 9.+与+=
+=运算会改变现有数组
+则创建一个新数组
code snippet 1
![9246628-59132f88240ce5a3.png](https://upload-images.jianshu.io/upload_images/9246628-59132f88240ce5a3.png)
image.png
```
import numpy as np
a=np.array([1,2,3,4])
b=a
a+=np.array([1,1,1,1])
print b         # output:[2,3,4,5]
```
code snippet 2
![9246628-2352561ba9c13546.png](https://upload-images.jianshu.io/upload_images/9246628-2352561ba9c13546.png)
image.png
```
import numpy as np
a=np.array([1,2,3,4])
b=a
a= a+np.array([1,1,1,1])
print b         # output:[1,2,3,4]
```
# 10.原地与非原地
原位运算 operates in-place += 会将所有的新值储存在原值，而不是创建一个新的数组
非原位运算 operates not in-place +
原位运算的另一个例子：切片
![9246628-f0f96b08b3f11d64.png](https://upload-images.jianshu.io/upload_images/9246628-f0f96b08b3f11d64.png)
image.png
```
import numpy as np
a = np.array([1,2,3,4,5])
slice=a[:3]
slice[0]=100
print a                    #[100   2   3   4   5]
```
slice refers to what's called a view of the original array,it will look like a array,but if you modify it ,the original array is modified as well,this makes slicing a numpy very fast,since you don't have to create a new array or copy any new values,but it means you should be very careful any time you want to modify a slice of an array
# 11.pandas series
```
import pandas as pd
countries = ['Albania', 'Algeria', 'Andorra', 'Angola', 'Antigua and Barbuda',
             'Argentina', 'Armenia', 'Australia', 'Austria', 'Azerbaijan',
             'Bahamas', 'Bahrain', 'Bangladesh', 'Barbados', 'Belarus',
             'Belgium', 'Belize', 'Benin', 'Bhutan', 'Bolivia']
life_expectancy_values = [74.7,  75. ,  83.4,  57.6,  74.6,  75.4,  72.3,  81.5,  80.2,
                          70.3,  72.1,  76.4,  68.1,  75.2,  69.8,  79.4,  70.8,  62.7,
                          67.3,  70.6]
gdp_values = [ 1681.61390973,   2155.48523109,  21495.80508273,    562.98768478,
              13495.1274663 ,   9388.68852258,   1424.19056199,  24765.54890176,
              27036.48733192,   1945.63754911,  21721.61840978,  13373.21993972,
                483.97086804,   9783.98417323,   2253.46411147,  25034.66692293,
               3680.91642923,    366.04496652,   1175.92638695,   1132.21387981]
life_expectancy = pd.Series(life_expectancy_values)
gdp = pd.Series(gdp_values)
print life_expectancy
###
0     74.7
1     75.0
2     83.4
3     57.6
4     74.6
5     75.4
6     72.3
7     81.5
8     80.2
9     70.3
10    72.1
11    76.4
12    68.1
13    75.2
14    69.8
15    79.4
16    70.8
17    62.7
18    67.3
19    70.6
###
print life_expectancy[0]           #74.7
print gdp[3:6]   
for country_life_expectancy in life_expectancy:
print 'Examining life expectancy {}'.format(country_life_expectancy)
    
print life_expectancy.mean()
print life_expectancy.std()
print gdp.max()
print gdp.sum()
a = pd.Series([1, 2, 3, 4])
b = pd.Series([1, 2, 1, 2])
print a + b
print a * 2
print a >= 3
print a[a >= 3]
```
a series is similar to a numpy array,but with extra functionality
e.g. s.describe() 打印出平均值、标准偏差、中位数以及其他统计量
numpy和pandas的相似之处：
- accessing elements
s[0],s[3:7]
*looping
for x in s
- convenient functions
s.mean(),s.max
- vectorized operations
s1+s2
- implemented in C(fast!)
**exercise using series**
write a function that takes in 2 series (e.g. life expectancy and GDP in 2007)
when a country has a life expectancy above the mean ,will the GDP above the mean also?(or vice versa)
return 2 numbers:
1.number of countries where both values are above or both are below the mean
2.number of countries where one value is above and one is below the mean
hint: you can add booleans in python
true + true =2
```
def variable_correlation(variable1, variable2):
    
    mean1 = variable1.mean()
    mean2 = variable2.mean()
    
    same_direction = ((variable1 > mean1) & (variable2 > mean2)) | ((variable1 < mean1) & (variable2 < mean2))
    different_direction = ((variable1 > mean1) & (variable2 < mean2)) | ((variable1 < mean1) & (variable2 > mean2))
    
    num_same_direction = sum(same_direction)
    num_different_direction = sum(different_direction)
    return (num_same_direction, num_different_direction)
def variable_correlation(variable1, variable2):
    both_above = (variable1 > variable1.mean()) & (variable2 > variable2.mean())
    both_below = (variable1 < variable1.mean()) & (variable2 < variable2.mean())
    is_same_direction  = both_above | both_below
    num_same_direction = sum(is_same_direction)
    num_different_direction  = len(variable1) - num_same_direction
    return (num_same_direction, num_different_direction)
variable_correlation(life_expectancy,gdp)
```
# 12.series索引
numpy是增强版的python list
pandas 是Python list(元素按顺序排列，需要通过位置获取)和python dictionary(可通过key查找值)的结合
pandas has index
```
import pandas as pd
countries = ['Albania', 'Algeria', 'Andorra', 'Angola', 'Antigua and Barbuda',
             'Argentina', 'Armenia', 'Australia', 'Austria', 'Azerbaijan',
             'Bahamas', 'Bahrain', 'Bangladesh', 'Barbados', 'Belarus',
             'Belgium', 'Belize', 'Benin', 'Bhutan', 'Bolivia']
life_expectancy_values = [74.7,  75. ,  83.4,  57.6,  74.6,  75.4,  72.3,  81.5,  80.2,
                          70.3,  72.1,  76.4,  68.1,  75.2,  69.8,  79.4,  70.8,  62.7,
                          67.3,  70.6]
life_expectancy = pd.Series(life_expectancy_values,index = countries)
Albania                74.7
Algeria                75.0
Andorra                83.4
Angola                 57.6
Antigua and Barbuda    74.6
Argentina              75.4
Armenia                72.3
Australia              81.5
Austria                80.2
Azerbaijan             70.3
Bahamas                72.1
Bahrain                76.4
Bangladesh             68.1
Barbados               75.2
Belarus                69.8
Belgium                79.4
Belize                 70.8
Benin                  62.7
Bhutan                 67.3
Bolivia                70.6
life_expectancy[0]                   #通过位置获取值
life_expectancy.loc['Angola']    #通过索引值index获取值
life_expectancy.iloc[0]    #通过位置获取值
life_expectancy_noindex = pd.Series(life_expectancy_values)  #在不指定索引的情况下，数字0，1，2，3……称为索引
```
**就业量最大的国家**
```
import pandas as pd
countries = [
    'Afghanistan', 'Albania', 'Algeria', 'Angola',
    'Argentina', 'Armenia', 'Australia', 'Austria',
    'Azerbaijan', 'Bahamas', 'Bahrain', 'Bangladesh',
    'Barbados', 'Belarus', 'Belgium', 'Belize',
    'Benin', 'Bhutan', 'Bolivia', 'Bosnia and Herzegovina',
]
employment_values = [
    55.70000076,  51.40000153,  50.5       ,  75.69999695,
    58.40000153,  40.09999847,  61.5       ,  57.09999847,
    60.90000153,  66.59999847,  60.40000153,  68.09999847,
    66.90000153,  53.40000153,  48.59999847,  56.79999924,
    71.59999847,  58.40000153,  70.40000153,  41.20000076,
]
employment = pd.Series(employment_values, index=countries)
def max_employment(employment):
    max_country = employment.argmax()       # 返回最大值的index
    max_value = employment.loc[max_country]   # Replace this with your code
    return (max_country, max_value)
```
# 13. 向量化运算和series索引
```
import pandas as pd
s1 = pd.Series([1, 2, 3, 4], index=['a', 'b', 'c', 'd'])
s2 = pd.Series([10, 20, 30, 40], index=['a', 'b', 'c', 'd'])\
print s1 + s2   #将两个索引值相同的series相加
output:
a    11
b    22
c    33
d    44
dtype: int64
```
```
import pandas as pd
s1 = pd.Series([1, 2, 3, 4], index=['a', 'b', 'c', 'd'])
s2 = pd.Series([10, 20, 30, 40], index=['b', 'd', 'a', 'c'])
print s1 + s2
a    31
b    12
c    43
d    24
dtype: int64
```
```
s1 = pd.Series([1, 2, 3, 4], index=['a', 'b', 'c', 'd'])
s2 = pd.Series([10, 20, 30, 40], index=['c', 'd', 'e', 'f'])
print s1 + s2
a     NaN
b     NaN
c    13.0
d    24.0
e     NaN
f     NaN
dtype: float64
```
```
s1 = pd.Series([1, 2, 3, 4], index=['a', 'b', 'c', 'd'])
s2 = pd.Series([10, 20, 30, 40], index=['e', 'f', 'g', 'h'])
print s1 + s2
a   NaN
b   NaN
c   NaN
d   NaN
e   NaN
f   NaN
g   NaN
h   NaN
dtype: float64
```
series 的向量化运算，值的匹配是根据索引而不是位置进行的
# 14.填充缺失值
```
s1 = pd.Series([1, 2, 3, 4], index=['a', 'b', 'c', 'd'])
s2 = pd.Series([10, 20, 30, 40], index=['c', 'd', 'e', 'f'])
sum_result = s1+s2
sum_result.dropna()           #删除缺失值
output:
c     13
d     24
```
# 15.pandas series apply()
non built-in calculations
so far,you've used built-in functions (e.g. mean()) and operations (e.g. +)
但是在没有内置函数，也无法通过简单的向量运算进行，那么可考虑如下方法：
- treat the series as a list(for loops,etc)
- use the function apply()
apply() takes a series and a function,and returns a new series
```
import pandas as pd
s = pd.Series([1, 2, 3, 4, 5])
def add_one(x):
    return x + 1
print s.apply(add_one)
###
0    2
1    3
2    4
3    5
4    6
dtype: int64
###
```
```
states = pd.Series([
    'California',
    'OH',
    'Michigan',
    'NY'])
def clean_state(state):
    if len(state) ==2:
        return state
    elif state =='California':
        return "CA"
    elif state=='Michigan':
        return 'MI'
clean_states = states.apply(clean_state) 
print clean_states
```
与使用循环相比，apply的有点在于可使代码更简洁，运行速度也更快
```
names = pd.Series([
    'Andre Agassi',
    'Barry Bonds',
    'Christopher Columbus',
    'Daniel Defoe',
    'Emilio Estevez',
    'Fred Flintstone',
    'Greta Garbo',
    'Humbert Humbert',
    'Ivan Ilych',
    'James Joyce',
    'Keira Knightley',
    'Lois Lane',
    'Mike Myers',
    'Nick Nolte',
    'Ozzy Osbourne',
    'Pablo Picasso',
    'Quirinus Quirrell',
    'Rachael Ray',
    'Susan Sarandon',
    'Tina Turner',
    'Ugueth Urbina',
    'Vince Vaughn',
    'Woodrow Wilson',
    'Yoji Yamada',
    'Zinedine Zidane'
])
def reverse_name(name):
    splited_name = name.split(' ')
    firstname = splited_name[0]
    lastname = splited_name[1]
    return lastname+', '+firstname
    
def reverse_names(names):
    return names.apply(reverse_name)
```
# 16. 在 Pandas 中绘图
如果变量 data 是一个 NumPy 数组或 Pandas Series，就像它是一个列表一样，
代码
```
import matplotlib.pyplot as plt
plt.hist(data)
```
将创建数据的直方图。
Pandas 还有在后台使用 matplotlib 的内置绘图函数，因此如果 data 是一个 Series，你可以使用 `data.hist()`创建直方图。
在此情形中，这两者没有区别，但有时候 Pandas 封装器更加方便。例如，你可以使用 data.plot() 创建 Series 的线条图。Series 索引被用于 x 轴，值被用于 y 轴。
如果你在本地运行绘图代码，你可能会需要加入一行 plt.show() 代码。
```
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
%pylab inline  #使用jupyter notebook时需要添加这行代码
path = '/datasets/ud170/gapminder/'
employment = pd.read_csv(path + 'employment_above_15.csv', index_col='Country')
female_completion = pd.read_csv(path + 'female_completion_rate.csv', index_col='Country')
male_completion = pd.read_csv(path + 'male_completion_rate.csv', index_col='Country')
life_expectancy = pd.read_csv(path + 'life_expectancy.csv', index_col='Country')
gdp = pd.read_csv(path + 'gdp_per_capita.csv', index_col='Country')
# The following code creates a Pandas Series for each variable for the United States.
# You can change the string 'United States' to a country of your choice.
employment_us = employment.loc['United States']
female_completion_us = female_completion.loc['United States']
male_completion_us = male_completion.loc['United States']
life_expectancy_us = life_expectancy.loc['United States']
gdp_us = gdp.loc['United States']
employment_us.plot()   #绘制折线图，x轴是employment的index
```
employment_us.plot()
![9246628-881f72326e50cae2.png](https://upload-images.jianshu.io/upload_images/9246628-881f72326e50cae2.png)
image.png
