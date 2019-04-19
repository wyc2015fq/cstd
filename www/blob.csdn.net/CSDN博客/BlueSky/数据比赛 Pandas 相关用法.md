# 数据比赛 Pandas 相关用法 - BlueSky - CSDN博客
2017年11月20日 14:20:11[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：310
1、one_hot 编码
df_train = pd.get_dummies(df_train)
2、删除某一列值为空的行
df = df.dropna(subset=['transactionid'])
3、统计df
# 统计地方为空
df.isnull().sum()
#统计df各列的count
df.counts()
4、映射值
order_df['price_real'] = np.round(5**order_df['price']-1, 2)
order_df['price_real'] = order_df['price_real'].apply(lambda x: log((1 + x),5))
 5、删除列及重命名
      fields_to_drop = ['loan_amount','loan_sum']
      loan_df = loan_df.drop( fields_to_drop, axis = 1 )
      loan_df = loan_df.rename(columns={'loan_amount_1': 'loan_amount'})
6、设置条件并赋值
# 设置筛选条件：选择 sex 为 male 
mask = (data['sex']=='male')
# .loc[] 赋值
data.loc[mask, 'id'] = 100
7、按某条件对某一列的值进行替换
train_df['price'].ix[train_df['price']>ulimit] = ulimit
8、根据条件选取df中的数据
sub1=data.loc [（data['列A‘]=='筛选的数值’）& data['列B‘].str.contains('筛选的关键字’），['C','D']]
9、对df根据某一列条件筛选后的df，再按条件排序
df_loan[df_loan['uid'] == 2360].sort_values(by='loan_time')
