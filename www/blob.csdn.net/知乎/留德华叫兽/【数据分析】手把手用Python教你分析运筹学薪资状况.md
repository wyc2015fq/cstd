# 【数据分析】手把手用Python教你分析运筹学薪资状况 - 知乎
# 

> **文章作者：**周岩  东北大学流程工业综合自动化国家重点实验室控制理论与控制工程专业博士研究生
文章发表于**微信公众号【运筹OR帷幄】：**[【数据分析】手把手用Python教你分析运筹学薪资状况](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/OPeYhZQJk28FN9OUPcRNvg)
*欢迎原链接转发，转载请私信*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

由于近些年互联网和计算机的发展，数据对于各各行业来说是一块新的“金矿”，再加上近两年人工智能的兴起，数据的重要性更是越加的凸显，因此一些新的职业比如数据挖掘工程师，算法工程师等成为一个新兴的行业。那么经过了这几年的发展，这个行业在前景是怎样的呢？既然是数据行业，我们就用数据来说话吧。

        首先我们来找一些数据的来源，对于我们普通人来说，最好的数据源自然是数据开源网站kaggle [(https://www.kaggle.com/](https://link.zhihu.com/?target=https%3A//www.kaggle.com/))，那么我们就找了一个比较符合我们目标的数据集([https://www.kaggle.com/kaggle/kaggle-survey-2017](https://link.zhihu.com/?target=https%3A//www.kaggle.com/kaggle/kaggle-survey-2017))。这个数据集包含了很多信息，这里仅对其中的薪资分布做重点分析，其他更有趣的信息，各位同学可以自行参考本文进一步挖掘。

## **一、使用的工具及数据的处理**

        数据可以从上述提供的链接下载，本文主要通过python来进行数据的处理，主要的工具使用了jupyter，数据包包含python中的科学计算工具：numpy, pandas, matplotlib, seaborn, plotly等。

        首先我们来分析一下数据，由于数据不是很大，所以可以用最基本的Excel打开，其中最主要的文件是‘multipleChoiceResponses.csv’和‘conversionRates.csv’两个文件，前者是主要的数据文件，后者是当时的一个货币汇率表，由于我们需要进行薪资对比，所以需要将各个国家的货币统一转换为美元(USD)。

         那么接下来首先将数据解压后上传到jupyter中，然后引入必要的包并导入数据：

         由于数据中有一些信息我们暂时用不到，同时还有汇率数据需要整合，所以先做一下数据的筛选和拼接：

         这其中需要性别，国籍，年龄，全职/兼职，职业，学位，年薪等。然后我们主要对全职的年薪感兴趣，那么我们筛选这一部分数据出来，同时去掉一些信息空值的数据。

         下一步我们观察到数据中记录的薪资是以字符串形式记录的，那么我接下来将字符串转换为数字并按照汇率数据统一转换为美元。

         这里主要的数据前期处理工作已经可以结束了，但是在画图的时候发现中国有很多个名称，我们需要将相关的数据合并，尤其需要注意的是，台湾也应该算在中国范围内（特殊强调）。

         接下来我们来看一下处理好的数据：

```python
import numpy as np # linear algebra
import pandas as pd # data processing, CSV file I/O (e.g. pd.read_csv)

# Input data files are available in the "../input/" directory.
# For example, running this (by clicking run or pressing Shift+Enter) will list the files in the input directory

import os
# print(os.listdir("../input"))

# Any results you write to the current directory are saved as output.

data = pd.read_csv("../input/multipleChoiceResponses.csv", encoding = "ISO-8859-1")
chang_rate = pd.read_csv("../input/conversionRates.csv", encoding = "ISO-8859-1")
```

由于数据中有一些信息我们暂时用不到，同时还有汇率数据需要整合，所以先做一下数据的筛选和拼接：

```
data_merged = pd.merge(data,chang_rate, how = 'left',left_on='CompensationCurrency', right_on='originCountry' )
data_usefull = data_merged[['GenderSelect','Country', 'Age','EmploymentStatus','CodeWriter',
             'CurrentJobTitleSelect', 'CurrentEmployerType', 'LanguageRecommendationSelect',
             'FormalEducation', 'CompensationAmount', 'CompensationCurrency', 'exchangeRate']]
```

这其中需要性别，国籍，年龄，全职/兼职，职业，学位，年薪等。然后我们主要对全职的年薪感兴趣，那么我们筛选这一部分数据出来，同时去掉一些信息空值的数据。

```
data_selected = data_usefull[data_usefull['EmploymentStatus']=='Employed full-time']
data_selected = data_selected.dropna(axis=0, how = 'any', subset=['Country', 'CompensationAmount','exchangeRate'])
```

下一步我们观察到数据中记录的薪资是以字符串形式记录的，那么我接下来将字符串转换为数字并按照汇率数据统一转换为美元。

```
data_selected['CompensationAmount'] = data_selected['CompensationAmount'].apply(lambda x: x.replace(',',''))
data_selected.drop(data_selected[data_selected['CompensationAmount']=='-'].index,inplace=True)
data_selected['CompensationAmountExanged'] = data_selected.apply(lambda x : float(x['CompensationAmount'])*float(x['exchangeRate']),  axis=1)
```

这里主要的数据前期处理工作已经可以结束了，但是在画图的时候发现中国有很多个名称，我们需要将相关的数据合并，尤其需要注意的是，台湾也应该算在中国范围内（特殊强调）。

data_selected.replace({"People 's Republic of China":'China' , "Republic of China":'China', "Taiwan":'China'}, inplace = True)

接下来我们来看一下处理好的数据：

data_selected.head()
![](https://pic1.zhimg.com/v2-877c46fb8d2b12e3d2d66a3bcdf60d14_b.jpg)
可以看到数据已经规整的处理好了，那么数据的整体信息是什么样的呢？

```
print('The total number of respondents:',data_selected.shape[0])
print('Total number of Countries with respondents:',data_selected['Country'].nunique())
print('Country with highest respondents:',data_selected['Country'].value_counts().index[0],'with',data_selected['Country'].value_counts().values[0],'respondents')
print('Youngest respondent:',data_selected['Age'].min(),' and Oldest respondent:',data_selected['Age'].max())
```

The total number of respondents: 3774

Total number of Countries with respondents: 50

Country with highest respondents: United States with 1098 respondents

Youngest respondent: 0.0  and Oldest respondent: 100.0

数据一共筛选出了3774条，一共有50个国家，其中人数最多的国家仍然是美国，年龄分布从0-100（这个年龄没有做严格的筛选，肯定是有问题的）。

## **二、画图与数据分析**

首先导入一些需要要用的包：

```
import matplotlib.pyplot as plt
import seaborn as sns

#Import Plotly and use it in the Offline Mode
import plotly
import plotly.offline as py
py.init_notebook_mode(connected=True)
import plotly.tools as tls
import plotly.graph_objs as go
import plotly.tools as tls
import plotly.figure_factory as fig_fact
plotly.tools.set_config_file(world_readable=True, sharing='public')
```

**A . 女性能占半边天？**

先对性别的分布做一个统计：

```
sns.countplot(y='GenderSelect', data=data_selected)
plt.show()
```

![](https://pic4.zhimg.com/v2-ea97a97708dc25e787cb0fcef6bcf477_b.jpg)
可以明显的看到对于数据行列来说，男性仍然占据着一个比较主导的地位，当然还有一些其他的性别，除了人为的填写错误因素，那么我们真的需要承认现在的世界真是一个多元化的世界。

**B . 一骑绝尘的美利坚**

接下来统计一下各个国家的人员数量，由于数量过多，仅对前15名进行画图：

```
data_country = data_selected.drop(data_selected[data_selected['Country']=='Other'].index)['Country'].value_counts().to_frame()
sns.barplot(data_country['Country'][:15],data_country.index[:15],palette='inferno')
plt.title('Top 15 Countries by number of respondents')
plt.xlabel('')
fig=plt.gcf()
fig.set_size_inches(10,10)
plt.show()
data_country[:15]
```

![](https://pic3.zhimg.com/v2-05142bac67812f0bbf8ed0e149a099da_b.jpg)
经过这次统计，我们发现人数最多的的国家是美国，其次是印度和英国。不过这个结论还是比较符合我们认知的，不过奇怪的是英国和法国竟然人数在中国之上，但是也从另一个角度反映出，我们国家的数据行业的发展还处于未成熟的阶段，国内的人才缺口应该还是很大的。

为了更加直观一些，我们用地图来呈现一下：
![](https://pic4.zhimg.com/v2-5653723a4b4c526e842c3068bba85933_b.jpg)
**C . 中坚力量还是年轻人**

那么各个年龄段是如何分布的呢：

```
#Plot the Age distribution
fig = fig_fact.create_distplot([data_selected[data_selected['Age'] > 0]['Age']], ['age'], colors=['#BA68C8'])
py.iplot(fig, filename='Basic Distplot')
```

![](https://pic3.zhimg.com/v2-b431a955e9fcbb440f49acc253b4ad46_b.jpg)
可以发现这个行业仍然是年轻人的主战场，主要分布在30-40岁的范围，不过这也不奇怪，因为数据分析的行业是一个新兴行业，年轻人更容易进入行业中，相信随着行业的发展，年龄分布的中心有可能会向着40岁左右偏移，那时应该是更加稳定的行业年龄结构。

**D . 薪资起点高，差异化明显**

接下来我们来分析我们这次重头戏，首先来对所有人员的薪资做一个整体的分布图：

```
plt.subplots(figsize=(15,8))
data_salary=data_selected[data_selected['CompensationAmountExanged']<1000000]
sns.distplot(data_salary['CompensationAmountExanged'])
plt.title('Salary Distribution',size=15)
plt.show()
```

![](https://pic4.zhimg.com/v2-e89c48f3eb720d66fee5c04e32b81883_b.jpg)
可以看到总体的薪资还是十分可观，最高可以到$30000，不过大部分仍然是分布在低位的，这个差距从图上看还是很大的，这不排除和不同国家的基本情况有关。一般来说发达国家的薪资水平要高于发展中国家，那么是那些国家的水平高，那些国家的水平低呢？

```
ax[0].axvline(data_salary['CompensationAmountExanged'].median(),linestyle='dashed')
ax[0].set_title('Highest Salary Paying Countries')
ax[0].set_xlabel('')
resp_coun=data_country[:15]
max_coun=data_salary.groupby('Country')['CompensationAmountExanged'].median().to_frame()
max_coun=max_coun[max_coun.index.isin(resp_coun.index)]
max_coun.sort_values(by='CompensationAmountExanged',ascending=True).plot.barh(width=0.8,ax=ax[1],color=sns.color_palette('RdYlGn'))
ax[1].axvline(data_salary['CompensationAmountExanged'].median(),linestyle='dashed')
ax[1].set_title('Compensation of Top 15 Respondent Countries')
ax[1].set_xlabel('')
ax[1].set_ylabel('')
plt.subplots_adjust(wspace=0.8)
plt.show()
```

![](https://pic3.zhimg.com/v2-f79900cadeea1fbb2a6b3a0a4abafae2_b.jpg)
从图中可以用看到美国是人数最多也是薪资最高的国家，说明美国在数据科学领域仍然是当之无愧的老大。可以看到相比之下，中国和印度这些国家虽然人数上很多，但是整体的薪资却十分不尽如人意。而一些欧洲的发达国家虽然人数不多但是薪资确实很让人欣慰的。

那么结合性别我们再来看看分布情况：

```
plt.subplots(figsize=(10,8))
sns.boxplot(y='GenderSelect',x='CompensationAmountExanged',data=data_salary)
plt.ylabel('')
plt.show()
```

![](https://pic3.zhimg.com/v2-206a13c1b7342aa26431bcf9b8b3854e_b.jpg)
可以看从薪资水平上，性别差别并不大，但是一些高工资部分还是男性主导，这有可能有一些主要管理岗位上还是男性居多，当然这只是一个猜测。

**E . 最吃香的还是运筹学**

接下来，数据科学领域有很多职业，从这些职业上来看薪资是怎样的呢:

```
sal_job=data_salary.groupby('CurrentJobTitleSelect')['CompensationAmountExanged'].median().to_frame().sort_values(by='CompensationAmountExanged',ascending=False)
ax=sns.barplot(sal_job.CompensationAmountExanged,sal_job.index,palette=sns.color_palette('inferno',20))
plt.title('Compensation By Job Title',size=15)
for i, v in enumerate(sal_job.CompensationAmountExanged): 
    ax.text(.5, i, v,fontsize=10,color='white',weight='bold')
fig=plt.gcf()
fig.set_size_inches(8,8)
plt.show()
```

![](https://pic1.zhimg.com/v2-a0bc4364d501b5d5f6f7a4e70ed1cd90_b.jpg)
可以看到运筹学从业者占据了最高的位置，数据科学家仅位居第二，建模工程师第三，软件开发第四。总体上看还是算法要比工程赚钱多。

最后我们结合各个国家，不同职位的人数是怎么样的呢：
![](https://pic3.zhimg.com/v2-6aad6ce4900c332961a1cab20864a442_b.jpg)
可以看到各个国家的分布结构还是差别很大的，美国是数据科学家最多，而中国则是机器学习工程师最多，其他国家的分布大致与美国相似，看来中国对人工智能的重视还是要远高于其他国家的。

进一步用每个职位的人数的比例来更直观的观察一下各个国家的产业结构：
![](https://pic2.zhimg.com/v2-4d89957d74830bb640321174e5db4f95_b.jpg)
那么通过这个图可以更加明显的展示每个职位的比重，以每个国家排名前3的职位来看，虽然Data scientist作为大部分国家的主要职位，但是法国和西班牙是占比最多的国家，分别为43.7%和43%；而排名第二的多数是Data Analysis、Scientist Researcher，但是俄罗斯为Machine Learning engineer；排名第三的还出现了以印度为代表的Softwareengineer。从初步的分析结果上看基本符合各个国家的特点，比如在印度和巴西这样的国家里，软件工程一直占有比较大的比重。有一个值得注意的是，印度的机器学习排在第四位，那么可见亚洲国家对于机器学习比较重视，而欧美国家机器学习普遍偏低。

## **参考文献**
- 1. [https://www.kaggle.com/rounakbanik/data-science-faq](https://link.zhihu.com/?target=https%3A//www.kaggle.com/rounakbanik/data-science-faq)
- 2.[https://www.kaggle.com/ash316/novice-to-grandmaster](https://link.zhihu.com/?target=https%3A//www.kaggle.com/ash316/novice-to-grandmaster)
- 3. [https://www.kaggle.com/hakkisimsek/plotly-tutorial-1](https://link.zhihu.com/?target=https%3A//www.kaggle.com/hakkisimsek/plotly-tutorial-1)

> 如需转载请在公众号后台获取转载须知

可以在 **本公众号后台 **回复关键词：“**数据分析**”可以获得文中代码的下载链接！~

**【活动】**版块长期招收志愿者，如果你有线上/线下活动策划经验，社群活动策划经验， 或者想加入我们增长相关经验，欢迎加入我们。具体可以在微信公众号后台回复：“志愿者”关键词获取详细信息，申请时请注明【活动】版块  

**『运筹OR帷幄』**长期招收志愿者，如果你有一技之长，爱分享，欢迎加入我们。点击**下方图片**即可了解详情！

扫二维码关注『运筹OR帷幄』公众号：
![](https://pic1.zhimg.com/v2-872dc2d2c5eaf08b38d693490549136c_b.jpg)
[点击查看『运筹OR帷幄』志愿者招募介绍及加入方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486166%26idx%3D1%26sn%3Da598e1475ae4f91afb7c33114ee61a85%26chksm%3Df9706d88ce07e49ec590fc2589d251bbf2b7d605cd127b1a0bdff61b8bad985eddf42365d823%26scene%3D21%23wechat_redirect) ：
![](https://pic1.zhimg.com/v2-d1fbc7022dc62969dcf8a6afdacdfcb4_b.jpg)
[点击查看【骥寻伯乐】板块说明及投稿方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486137%26idx%3D2%26sn%3D24e9487d15a8c590645a4d83950c8f20%26chksm%3Df9706de7ce07e4f1507edd01511920bd8f0a3a80384aef8ee8768b68931f872bc6cff5d3485f%26scene%3D21%23wechat_redirect) ：
![](https://pic3.zhimg.com/v2-88ba2272456e93bb2f6b9bf2d55ef1d2_b.jpg)

