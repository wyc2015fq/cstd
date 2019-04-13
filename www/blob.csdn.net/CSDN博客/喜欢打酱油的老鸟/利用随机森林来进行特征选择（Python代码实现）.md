
# 利用随机森林来进行特征选择（Python代码实现） - 喜欢打酱油的老鸟 - CSDN博客


2019年01月23日 08:40:00[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：1030


[https://www.toutiao.com/a6644771438534328836/](https://www.toutiao.com/a6644771438534328836/)
2019-01-10 15:46:50
当数据集的特征过多时，容易产生过拟合，可以用随机森林来在训练之后可以产生一个各个特征重要性的数据集，利用这个数据集，确定一个阈值，选出来对模型训练帮助最大的一些特征，筛选出重要变量后可以再训练模型；
本文所用数据集是从kaggle网站上下载的lend club数据，通过随机森林筛选出对预测是否逾期的重要性变量：
```python
# 首先导入数据，查看数据集的基本情况：
df = pd.read_csv('loan.csv')
df.head()
df.shape
数据集共887379行，74列
```
![利用随机森林来进行特征选择（Python代码实现）](http://p1.pstatp.com/large/dfic-imagehandler/11d1c59e-651b-4812-9750-6abf367add96)

```python
#然后转换目标值，将Charged Off和Late (31-120 days归为坏客户1，其余归为0
df['loan_status'].unique()
df['y'] = df['loan_status'].map(lambda x: int((x == 'Charged Off') | (x == 'Late (31-120 days')))
df.drop('loan_status', axis=1,inplace=True)
```
![利用随机森林来进行特征选择（Python代码实现）](http://p1.pstatp.com/large/dfic-imagehandler/d8be538c-e75f-4782-aed9-00a0c7fc6a19)

接下来处理特征变量，先剔除一些对预测无关的贷后和贷中变量：
```python
df.drop(['id', 'member_id', 'url','next_pymnt_d','last_pymnt_d',
 'pymnt_plan','last_credit_pull_d','total_rec_prncp',
 'total_rec_int','out_prncp','last_pymnt_amnt',
 'installment','total_pymnt_inv', 'total_rec_prncp',
 'total_rec_int'], axis=1,inplace=True)
```
再删除缺失值大于90%的变量：
```python
df.dropna(thresh = len(df) * 0.1 , axis=1,inplace=True)
```
接下来补全缺失值，对缺失值较多变量，将缺失值作为一种状态，对缺失值较少的变量，数值型用平均值补全，类别型用众数补全：
```python
#对于desc有描述的标为1，空值标为0
def Desc(x):
 if type(x).__name__ == 'float':
 return 0
 else:
 return 1
df['desc']= df['desc'].map(Desc)
#emp_title空值用“空值”补全
df['emp_title'] = df['emp_title'].fillna('missing')
'''mths_since_last_delinq，mths_since_last_record ，
mths_since_last_major_derog ,tot_coll_amt,tot_cur_bal,
total_rev_hi_lim 用-1代替'''
for col in ['mths_since_last_delinq','mths_since_last_record',
 'mths_since_last_major_derog','tot_coll_amt',
 'tot_cur_bal','total_rev_hi_lim']:
 df[col].fillna(-1,inplace=True)
#剩余缺失值较少的，数值型变量用平均值代替
for col in ['annual_inc','acc_now_delinq',
 'collections_12_mths_ex_med','total_acc',
 'pub_rec','open_acc','inq_last_6mths','delinq_2yrs',
 'revol_util']:
 df[col].fillna(df[col].mean(), inplace=True)
```
另外再构造一个衍生变量，算申请时间issue_d和第一次借贷时间earliest_cr_line之间的月份差值
```python
import datetime
def ConvertDate(x):
 mth_dict = {'Jan': 1, 'Feb': 2, 'Mar': 3, 'Apr': 4, 'May': 5, 
 'Jun': 6, 'Jul': 7, 'Aug': 8, 'Sep': 9, 'Oct': 10,
 'Nov': 11, 'Dec': 12}
 yr = int(x[4:6])
 mth = mth_dict[x[:3]]
 return datetime.datetime(yr, mth, 1)
df['issue_d'] = df['issue_d'].map(lambda x :ConvertDate(x))
df['earliest_cr_line'] = df['earliest_cr_line'].map(lambda x :ConvertDate(x))
from dateutil.relativedelta import relativedelta
def MonthGap(earlyDate, lateDate):
 if lateDate > earlyDate:
 gap = relativedelta(lateDate, earlyDate)
 yr = gap.years
 mth = gap.months
 return yr*12 + mth
 else:
 return 0
df['earliest_cr_to_app'] = df.apply(lambda x : MonthGap(x.earliest_cr_line, x.issue_d),axis=1)
df.drop(['issue_d', 'earliest_cr_line'],axis=1, inplace=True)
```
因为决策树只能处理数值型和标称型变量，所以转换一下类别变量
```python
'''因为本文只运用随机森林来判断变量的重要性，而树模型不需要One-Hot编码，
所以只对类型型变量LabelEncoding就可以'''
#term ,将months替换成空值
df['term']= df['term'].apply(lambda x :int(x.replace("months" , "")))
cols = df.select_dtypes(include=['O']).columns.tolist()
for col in cols:
 df[col] = preprocessing.LabelEncoder().fit_transform(df[col])
```
训练模型
```python
#训练模型，这里随机森林模型参数都用默认值
y = df['y']
x = df.drop('y', axis=1)
clf = RandomForestClassifier()
clf.fit(x, df['y'])
```
![利用随机森林来进行特征选择（Python代码实现）](http://p3.pstatp.com/large/dfic-imagehandler/b2f06f7c-5983-4a3c-8d01-2728b1f264ca)

筛选变量：
```python
importance = clf.feature_importances_
indices = np.argsort(importance)[::-1]
features = x.columns
for f in range(x.shape[1]):
 print(("%2d) %-*s %f" % (f + 1, 30, features[f], importance[indices[f]])))
```
![利用随机森林来进行特征选择（Python代码实现）](http://p3.pstatp.com/large/dfic-imagehandler/1e5c2ac3-deb8-4f9c-be92-d0e1f7dd78e4)

现在各变量对是否逾期的重要性就计算出来了，如果接下来运用这些变量预测是否逾期的模型是树模型或者朴素贝叶斯模型，就可以直接通过重要性大于某个阈值或者重要性排名大于某个某个阈值来筛选出变量训练模型了，如果是接下来用回归模型或者线性模型，还需要考虑各个变量之间的共线性；

