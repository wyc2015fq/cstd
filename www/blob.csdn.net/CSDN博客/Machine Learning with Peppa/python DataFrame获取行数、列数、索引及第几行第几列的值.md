# python DataFrame获取行数、列数、索引及第几行第几列的值 - Machine Learning with Peppa - CSDN博客





2018年07月15日 13:30:17[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1568








                print df.columns.size#列数 

print df.iloc[:,0].size#行数 
print df.ix[[0]].index.values[0]#索引值 
print df.ix[[0]].values[0][0]#第一行第一列的值 
print df.ix[[1]].values[0][1]#第二行第二列的值 



