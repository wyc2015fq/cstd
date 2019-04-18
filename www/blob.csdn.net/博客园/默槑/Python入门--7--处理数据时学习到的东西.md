# Python入门--7--处理数据时学习到的东西 - 默槑 - 博客园







# [Python入门--7--处理数据时学习到的东西](https://www.cnblogs.com/modaidai/p/6801745.html)





一、数据导入（这里使用的是pands包）

import pands as pd

wenjian = pd.read_csv('路径')



二、数据变换

print wenjian.head()    #输出文件前五行

print wenjian.tail()　　　#输出文件倒数五行



print wenjian.columns  　　#输出列名

print wenjian.index　　　　#输出行名



print wenjian.T　　　　　　#数据转置，也就是行变列，列变行，让它“躺下”



print wenjian.ix[:,0].head()　　#提取文件的第一列的前五行，用的是ix函数，注意用R语言可以直接筛选，用Python需要加上ix。

print wenjian.ix[11:20,0:3]　　#提取文件从11行到20行的前三列数据

print wenjian.ix[11:20,['某列名','某列名','某列名']]　　　#跟上面的一样功能，只不过是用列名去检索的



print wenjian.drop(df.columns[[1,2]],axis = 1).head()　　#用drop舍弃列，这里是舍弃第二列和第三列，python是从零开始的，axis参数会告诉程序舍弃行还是列，=0是行，=1是列



print wenjian.describe()　　　　#会算出这几列数据的MAX，MIN，MEAN等等，很多的值，显示出来



这是一些非常非常简单的东西，继续努力吧














